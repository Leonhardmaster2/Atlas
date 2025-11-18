// Copyright Atlas Team. All Rights Reserved.

#include "SAtlasOverviewPanel.h"
#include "Overview/AtlasOverviewStats.h"
#include "AtlasSettings.h"
#include "AtlasRuntime.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "EditorStyleSet.h"

#define LOCTEXT_NAMESPACE "AtlasOverviewPanel"

void SAtlasOverviewPanel::Construct(const FArguments& InArgs)
{
	ProjectBytes = 0;
	VRAMBytes = 0;

	ChildSlot
	[
		SNew(SVerticalBox)

		// Title and Refresh Button
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(4.0f)
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.FillWidth(1.0f)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("Title", "Atlas Project Overview"))
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 16))
			]

			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SButton)
				.Text(LOCTEXT("Refresh", "Refresh"))
				.OnClicked(this, &SAtlasOverviewPanel::OnRefreshClicked)
			]
		]

		// Status Cards
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(4.0f)
		[
			SNew(SHorizontalBox)

			// VRAM Card
			+ SHorizontalBox::Slot()
			.FillWidth(0.5f)
			.Padding(4.0f)
			[
				SNew(SBorder)
				.BorderBackgroundColor(this, &SAtlasOverviewPanel::GetVRAMStatusColor)
				.Padding(8.0f)
				[
					SNew(SVerticalBox)

					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(STextBlock)
						.Text(LOCTEXT("VRAMTitle", "VRAM Usage"))
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
					]

					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(STextBlock)
						.Text(this, &SAtlasOverviewPanel::GetVRAMStatusText)
					]
				]
			]

			// Project Size Card
			+ SHorizontalBox::Slot()
			.FillWidth(0.5f)
			.Padding(4.0f)
			[
				SNew(SBorder)
				.BorderBackgroundColor(this, &SAtlasOverviewPanel::GetProjectSizeStatusColor)
				.Padding(8.0f)
				[
					SNew(SVerticalBox)

					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(STextBlock)
						.Text(LOCTEXT("ProjectSizeTitle", "Project Size"))
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
					]

					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(STextBlock)
						.Text(this, &SAtlasOverviewPanel::GetProjectSizeStatusText)
					]
				]
			]
		]

		// Top Textures List
		+ SVerticalBox::Slot()
		.FillHeight(0.5f)
		.Padding(4.0f)
		[
			SNew(SVerticalBox)

			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(STextBlock)
				.Text(LOCTEXT("TopTextures", "Top Textures by VRAM"))
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
			]

			+ SVerticalBox::Slot()
			.FillHeight(1.0f)
			[
				SAssignNew(TextureListView, SListView<TSharedPtr<FAtlasTextureStat>>)
				.ListItemsSource(&Textures)
				.OnGenerateRow(this, &SAtlasOverviewPanel::OnGenerateTextureRow)
				.SelectionMode(ESelectionMode::Single)
			]
		]

		// Top Meshes List
		+ SVerticalBox::Slot()
		.FillHeight(0.5f)
		.Padding(4.0f)
		[
			SNew(SVerticalBox)

			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(STextBlock)
				.Text(LOCTEXT("TopMeshes", "Top Meshes by Triangle Count"))
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
			]

			+ SVerticalBox::Slot()
			.FillHeight(1.0f)
			[
				SAssignNew(MeshListView, SListView<TSharedPtr<FAtlasMeshStat>>)
				.ListItemsSource(&Meshes)
				.OnGenerateRow(this, &SAtlasOverviewPanel::OnGenerateMeshRow)
				.SelectionMode(ESelectionMode::Single)
			]
		]
	];

	// Initial stats collection
	RefreshStats();
}

void SAtlasOverviewPanel::RefreshStats()
{
	Textures.Empty();
	Meshes.Empty();
	Levels.Empty();

	TArray<FAtlasTextureStat> TextureStats;
	TArray<FAtlasMeshStat> MeshStats;
	TArray<FAtlasLevelStat> LevelStats;

	FAtlasOverviewStats::CollectAll(TextureStats, MeshStats, LevelStats, ProjectBytes, VRAMBytes);

	// Convert to shared pointers and take top 20
	int32 MaxItems = 20;

	for (int32 i = 0; i < FMath::Min(TextureStats.Num(), MaxItems); ++i)
	{
		Textures.Add(MakeShared<FAtlasTextureStat>(TextureStats[i]));
	}

	for (int32 i = 0; i < FMath::Min(MeshStats.Num(), MaxItems); ++i)
	{
		Meshes.Add(MakeShared<FAtlasMeshStat>(MeshStats[i]));
	}

	for (int32 i = 0; i < FMath::Min(LevelStats.Num(), MaxItems); ++i)
	{
		Levels.Add(MakeShared<FAtlasLevelStat>(LevelStats[i]));
	}

	if (TextureListView.IsValid())
	{
		TextureListView->RequestListRefresh();
	}

	if (MeshListView.IsValid())
	{
		MeshListView->RequestListRefresh();
	}

	UE_LOG(LogAtlas, Log, TEXT("Overview refreshed: %lld MB VRAM, %lld MB project size"),
		VRAMBytes / (1024 * 1024), ProjectBytes / (1024 * 1024));
}

TSharedRef<ITableRow> SAtlasOverviewPanel::OnGenerateTextureRow(TSharedPtr<FAtlasTextureStat> Stat, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedPtr<FAtlasTextureStat>>, OwnerTable)
	[
		SNew(SHorizontalBox)

		+ SHorizontalBox::Slot()
		.FillWidth(0.6f)
		.Padding(4.0f)
		[
			SNew(STextBlock)
			.Text(FText::FromString(FPaths::GetCleanFilename(Stat->Path)))
			.ToolTipText(FText::FromString(Stat->Path))
		]

		+ SHorizontalBox::Slot()
		.FillWidth(0.2f)
		.Padding(4.0f)
		[
			SNew(STextBlock)
			.Text(FText::FromString(FString::Printf(TEXT("%d x %d"), Stat->MaxDimension, Stat->MaxDimension)))
		]

		+ SHorizontalBox::Slot()
		.FillWidth(0.2f)
		.Padding(4.0f)
		[
			SNew(STextBlock)
			.Text(FText::FromString(FString::Printf(TEXT("%.2f MB"), Stat->VRAMBytes / (1024.0f * 1024.0f))))
		]
	];
}

TSharedRef<ITableRow> SAtlasOverviewPanel::OnGenerateMeshRow(TSharedPtr<FAtlasMeshStat> Stat, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedPtr<FAtlasMeshStat>>, OwnerTable)
	[
		SNew(SHorizontalBox)

		+ SHorizontalBox::Slot()
		.FillWidth(0.6f)
		.Padding(4.0f)
		[
			SNew(STextBlock)
			.Text(FText::FromString(FPaths::GetCleanFilename(Stat->Path)))
			.ToolTipText(FText::FromString(Stat->Path))
		]

		+ SHorizontalBox::Slot()
		.FillWidth(0.2f)
		.Padding(4.0f)
		[
			SNew(STextBlock)
			.Text(FText::FromString(FString::Printf(TEXT("%lld tris"), Stat->TriangleCount)))
		]

		+ SHorizontalBox::Slot()
		.FillWidth(0.2f)
		.Padding(4.0f)
		[
			SNew(STextBlock)
			.Text(FText::FromString(FString::Printf(TEXT("%d LODs"), Stat->NumLODs)))
		]
	];
}

FReply SAtlasOverviewPanel::OnRefreshClicked()
{
	RefreshStats();
	return FReply::Handled();
}

FSlateColor SAtlasOverviewPanel::GetVRAMStatusColor() const
{
	const UAtlasSettings* Settings = UAtlasSettings::Get();
	if (!Settings)
	{
		return FLinearColor::Gray;
	}

	int64 TargetMB = Settings->TargetVRAMMB;
	int64 CurrentMB = VRAMBytes / (1024 * 1024);

	if (CurrentMB > TargetMB * 1.5f)
	{
		return FLinearColor(0.8f, 0.2f, 0.2f, 0.3f); // Red
	}
	else if (CurrentMB > TargetMB)
	{
		return FLinearColor(0.8f, 0.8f, 0.2f, 0.3f); // Yellow
	}
	else
	{
		return FLinearColor(0.2f, 0.8f, 0.2f, 0.3f); // Green
	}
}

FSlateColor SAtlasOverviewPanel::GetProjectSizeStatusColor() const
{
	const UAtlasSettings* Settings = UAtlasSettings::Get();
	if (!Settings)
	{
		return FLinearColor::Gray;
	}

	int64 MaxMB = Settings->MaxProjectSizeMB;
	int64 CurrentMB = ProjectBytes / (1024 * 1024);

	if (CurrentMB > MaxMB)
	{
		return FLinearColor(0.8f, 0.2f, 0.2f, 0.3f); // Red
	}
	else if (CurrentMB > MaxMB * 0.8f)
	{
		return FLinearColor(0.8f, 0.8f, 0.2f, 0.3f); // Yellow
	}
	else
	{
		return FLinearColor(0.2f, 0.8f, 0.2f, 0.3f); // Green
	}
}

FText SAtlasOverviewPanel::GetVRAMStatusText() const
{
	const UAtlasSettings* Settings = UAtlasSettings::Get();
	int64 TargetMB = Settings ? Settings->TargetVRAMMB : 2560;
	int64 CurrentMB = VRAMBytes / (1024 * 1024);

	return FText::FromString(FString::Printf(TEXT("%lld MB / %lld MB"), CurrentMB, TargetMB));
}

FText SAtlasOverviewPanel::GetProjectSizeStatusText() const
{
	const UAtlasSettings* Settings = UAtlasSettings::Get();
	int64 MaxMB = Settings ? Settings->MaxProjectSizeMB : 81920;
	int64 CurrentMB = ProjectBytes / (1024 * 1024);

	return FText::FromString(FString::Printf(TEXT("%lld MB / %lld MB"), CurrentMB, MaxMB));
}

#undef LOCTEXT_NAMESPACE
