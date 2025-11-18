// Copyright Atlas Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/SListView.h"
#include "AtlasTypes.h"

/**
 * Overview panel widget showing project statistics and health
 */
class SAtlasOverviewPanel : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAtlasOverviewPanel) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	/** Refresh statistics */
	void RefreshStats();

private:
	/** Generate row for texture list */
	TSharedRef<ITableRow> OnGenerateTextureRow(TSharedPtr<FAtlasTextureStat> Stat, const TSharedRef<STableViewBase>& OwnerTable);

	/** Generate row for mesh list */
	TSharedRef<ITableRow> OnGenerateMeshRow(TSharedPtr<FAtlasMeshStat> Stat, const TSharedRef<STableViewBase>& OwnerTable);

	/** Handle refresh button click */
	FReply OnRefreshClicked();

	/** Get VRAM status color */
	FSlateColor GetVRAMStatusColor() const;

	/** Get project size status color */
	FSlateColor GetProjectSizeStatusColor() const;

	/** Get VRAM status text */
	FText GetVRAMStatusText() const;

	/** Get project size status text */
	FText GetProjectSizeStatusText() const;

private:
	/** Collected statistics */
	TArray<TSharedPtr<FAtlasTextureStat>> Textures;
	TArray<TSharedPtr<FAtlasMeshStat>> Meshes;
	TArray<TSharedPtr<FAtlasLevelStat>> Levels;
	int64 ProjectBytes;
	int64 VRAMBytes;

	/** List views */
	TSharedPtr<SListView<TSharedPtr<FAtlasTextureStat>>> TextureListView;
	TSharedPtr<SListView<TSharedPtr<FAtlasMeshStat>>> MeshListView;
};
