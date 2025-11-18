// Copyright Atlas Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/SListView.h"
#include "AtlasTypes.h"

/**
 * Validator panel widget showing validation issues and fix options
 */
class SAtlasValidatorPanel : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAtlasValidatorPanel) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	/** Refresh the issue list by running a scan */
	void RefreshIssues();

private:
	/** Generate a row widget for an issue */
	TSharedRef<ITableRow> OnGenerateRow(TSharedPtr<FAtlasIssue> Issue, const TSharedRef<STableViewBase>& OwnerTable);

	/** Handle double-click on an issue row */
	void OnRowDoubleClicked(TSharedPtr<FAtlasIssue> Issue);

	/** Handle Scan All button click */
	FReply OnScanAllClicked();

	/** Handle Scan Selection button click */
	FReply OnScanSelectionClicked();

	/** Handle Fix Selected button click */
	FReply OnFixSelectedClicked();

	/** Handle Fix All button click */
	FReply OnFixAllClicked();

	/** Fix a single issue */
	void FixIssue(TSharedPtr<FAtlasIssue> Issue);

	/** Get the severity color for display */
	FSlateColor GetSeverityColor(EAtlasSeverity Severity) const;

	/** Get severity text */
	FText GetSeverityText(EAtlasSeverity Severity) const;

private:
	/** List of current issues */
	TArray<TSharedPtr<FAtlasIssue>> Issues;

	/** List view widget */
	TSharedPtr<SListView<TSharedPtr<FAtlasIssue>>> IssueListView;
};
