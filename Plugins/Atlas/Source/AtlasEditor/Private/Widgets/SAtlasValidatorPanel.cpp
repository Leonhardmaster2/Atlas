// Copyright Atlas Team. All Rights Reserved.

#include "SAtlasValidatorPanel.h"
#include "Core/AtlasScanExecutor.h"
#include "Core/AtlasAssetUtils.h"
#include "AtlasRuntime.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SBox.h"
#include "EditorStyleSet.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"

#define LOCTEXT_NAMESPACE "AtlasValidatorPanel"

void SAtlasValidatorPanel::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SVerticalBox)

		// Toolbar
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(4.0f)
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(2.0f)
			[
				SNew(SButton)
				.Text(LOCTEXT("ScanAll", "Scan All"))
				.OnClicked(this, &SAtlasValidatorPanel::OnScanAllClicked)
			]

			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(2.0f)
			[
				SNew(SButton)
				.Text(LOCTEXT("ScanSelection", "Scan Selection"))
				.OnClicked(this, &SAtlasValidatorPanel::OnScanSelectionClicked)
			]

			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(2.0f)
			[
				SNew(SButton)
				.Text(LOCTEXT("FixSelected", "Fix Selected"))
				.OnClicked(this, &SAtlasValidatorPanel::OnFixSelectedClicked)
			]

			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(2.0f)
			[
				SNew(SButton)
				.Text(LOCTEXT("FixAll", "Fix All"))
				.OnClicked(this, &SAtlasValidatorPanel::OnFixAllClicked)
			]
		]

		// Issue count
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(4.0f)
		[
			SNew(STextBlock)
			.Text_Lambda([this]() {
				return FText::FromString(FString::Printf(TEXT("Issues: %d"), Issues.Num()));
			})
		]

		// Issue list
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		.Padding(4.0f)
		[
			SAssignNew(IssueListView, SListView<TSharedPtr<FAtlasIssue>>)
			.ListItemsSource(&Issues)
			.OnGenerateRow(this, &SAtlasValidatorPanel::OnGenerateRow)
			.OnMouseButtonDoubleClick(this, &SAtlasValidatorPanel::OnRowDoubleClicked)
			.SelectionMode(ESelectionMode::Multi)
		]
	];

	// Initial scan
	RefreshIssues();
}

void SAtlasValidatorPanel::RefreshIssues()
{
	Issues.Empty();

	TArray<FAtlasIssue> FoundIssues;
	FAtlasScanExecutor::ScanAll(FoundIssues);

	for (const FAtlasIssue& Issue : FoundIssues)
	{
		Issues.Add(MakeShared<FAtlasIssue>(Issue));
	}

	if (IssueListView.IsValid())
	{
		IssueListView->RequestListRefresh();
	}

	UE_LOG(LogAtlas, Log, TEXT("Validator panel refreshed: %d issues found"), Issues.Num());
}

TSharedRef<ITableRow> SAtlasValidatorPanel::OnGenerateRow(TSharedPtr<FAtlasIssue> Issue, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedPtr<FAtlasIssue>>, OwnerTable)
	[
		SNew(SHorizontalBox)

		// Severity
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(4.0f)
		[
			SNew(SBox)
			.WidthOverride(80.0f)
			[
				SNew(STextBlock)
				.Text(GetSeverityText(Issue->Severity))
				.ColorAndOpacity(GetSeverityColor(Issue->Severity))
			]
		]

		// Rule Name
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(4.0f)
		[
			SNew(SBox)
			.WidthOverride(150.0f)
			[
				SNew(STextBlock)
				.Text(FText::FromName(Issue->RuleName))
			]
		]

		// Asset Path
		+ SHorizontalBox::Slot()
		.FillWidth(0.4f)
		.Padding(4.0f)
		[
			SNew(STextBlock)
			.Text(FText::FromString(FPaths::GetCleanFilename(Issue->AssetPath)))
			.ToolTipText(FText::FromString(Issue->AssetPath))
		]

		// Message
		+ SHorizontalBox::Slot()
		.FillWidth(0.6f)
		.Padding(4.0f)
		[
			SNew(STextBlock)
			.Text(FText::FromString(Issue->Message))
			.AutoWrapText(true)
		]

		// Fix Button
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(4.0f)
		[
			SNew(SButton)
			.Text(LOCTEXT("Fix", "Fix"))
			.IsEnabled(Issue->bCanFix && Issue->FixFunction != nullptr)
			.OnClicked_Lambda([this, Issue]() {
				FixIssue(Issue);
				return FReply::Handled();
			})
		]
	];
}

void SAtlasValidatorPanel::OnRowDoubleClicked(TSharedPtr<FAtlasIssue> Issue)
{
	if (Issue.IsValid())
	{
		FAtlasAssetUtils::SyncToCB(Issue->AssetPath);
	}
}

FReply SAtlasValidatorPanel::OnScanAllClicked()
{
	RefreshIssues();
	return FReply::Handled();
}

FReply SAtlasValidatorPanel::OnScanSelectionClicked()
{
	// TODO: Get selected assets from Content Browser
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	TArray<FAssetData> SelectedAssets;
	ContentBrowserModule.Get().GetSelectedAssets(SelectedAssets);

	Issues.Empty();
	TArray<FAtlasIssue> FoundIssues;
	FAtlasScanExecutor::ScanSelection(SelectedAssets, FoundIssues);

	for (const FAtlasIssue& Issue : FoundIssues)
	{
		Issues.Add(MakeShared<FAtlasIssue>(Issue));
	}

	if (IssueListView.IsValid())
	{
		IssueListView->RequestListRefresh();
	}

	return FReply::Handled();
}

FReply SAtlasValidatorPanel::OnFixSelectedClicked()
{
	TArray<TSharedPtr<FAtlasIssue>> SelectedIssues;
	if (IssueListView.IsValid())
	{
		SelectedIssues = IssueListView->GetSelectedItems();
	}

	for (TSharedPtr<FAtlasIssue> Issue : SelectedIssues)
	{
		FixIssue(Issue);
	}

	// Refresh after fixing
	RefreshIssues();

	return FReply::Handled();
}

FReply SAtlasValidatorPanel::OnFixAllClicked()
{
	int32 FixedCount = 0;
	for (TSharedPtr<FAtlasIssue> Issue : Issues)
	{
		if (Issue.IsValid() && Issue->bCanFix && Issue->FixFunction)
		{
			if (Issue->FixFunction())
			{
				FixedCount++;
			}
		}
	}

	UE_LOG(LogAtlas, Log, TEXT("Fixed %d issues"), FixedCount);

	// Refresh after fixing
	RefreshIssues();

	return FReply::Handled();
}

void SAtlasValidatorPanel::FixIssue(TSharedPtr<FAtlasIssue> Issue)
{
	if (Issue.IsValid() && Issue->bCanFix && Issue->FixFunction)
	{
		if (Issue->FixFunction())
		{
			UE_LOG(LogAtlas, Log, TEXT("Fixed issue: %s"), *Issue->Message);
		}
		else
		{
			UE_LOG(LogAtlas, Warning, TEXT("Failed to fix issue: %s"), *Issue->Message);
		}
	}
}

FSlateColor SAtlasValidatorPanel::GetSeverityColor(EAtlasSeverity Severity) const
{
	switch (Severity)
	{
	case EAtlasSeverity::Error:
		return FLinearColor::Red;
	case EAtlasSeverity::Warning:
		return FLinearColor::Yellow;
	case EAtlasSeverity::Info:
	default:
		return FLinearColor::White;
	}
}

FText SAtlasValidatorPanel::GetSeverityText(EAtlasSeverity Severity) const
{
	switch (Severity)
	{
	case EAtlasSeverity::Error:
		return LOCTEXT("Error", "Error");
	case EAtlasSeverity::Warning:
		return LOCTEXT("Warning", "Warning");
	case EAtlasSeverity::Info:
	default:
		return LOCTEXT("Info", "Info");
	}
}

#undef LOCTEXT_NAMESPACE
