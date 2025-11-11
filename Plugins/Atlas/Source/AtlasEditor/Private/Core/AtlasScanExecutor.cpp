// Copyright Atlas Team. All Rights Reserved.

#include "AtlasScanExecutor.h"
#include "AtlasRuleRegistry.h"
#include "AtlasAssetUtils.h"
#include "IAtlasRule.h"
#include "AtlasRuntime.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Misc/ScopedSlowTask.h"

const TArray<FName> FAtlasScanExecutor::ScannableAssetClasses = {
	TEXT("StaticMesh"),
	TEXT("SkeletalMesh"),
	TEXT("Texture2D"),
	TEXT("Material"),
	TEXT("MaterialInstance"),
	TEXT("MaterialInstanceConstant"),
	TEXT("SoundWave"),
	TEXT("Blueprint")
};

void FAtlasScanExecutor::ScanAll(TArray<FAtlasIssue>& OutIssues)
{
	TArray<FAssetData> AllAssets;
	GetAllScannableAssets(AllAssets);

	UE_LOG(LogAtlas, Log, TEXT("Starting full scan of %d assets"), AllAssets.Num());

	FScopedSlowTask Progress(AllAssets.Num(), FText::FromString(TEXT("Scanning all assets...")));
	Progress.MakeDialog();

	ScanSelection(AllAssets, OutIssues);

	UE_LOG(LogAtlas, Log, TEXT("Scan complete. Found %d issues"), OutIssues.Num());
}

void FAtlasScanExecutor::ScanSelection(const TArray<FAssetData>& Selection, TArray<FAtlasIssue>& OutIssues)
{
	const TArray<TSharedRef<IAtlasRule>>& Rules = FAtlasRuleRegistry::Get().All();

	if (Rules.Num() == 0)
	{
		UE_LOG(LogAtlas, Warning, TEXT("No validation rules registered"));
		return;
	}

	FScopedSlowTask Progress(Selection.Num(), FText::FromString(TEXT("Scanning selected assets...")));
	Progress.MakeDialog();

	for (const FAssetData& AssetData : Selection)
	{
		Progress.EnterProgressFrame(1.0f);

		// Load the asset
		UObject* Asset = AssetData.GetAsset();
		if (!Asset)
		{
			continue;
		}

		ScanAsset(Asset, OutIssues);
	}
}

void FAtlasScanExecutor::ScanAsset(UObject* Asset, TArray<FAtlasIssue>& OutIssues)
{
	if (!Asset)
	{
		return;
	}

	const TArray<TSharedRef<IAtlasRule>>& Rules = FAtlasRuleRegistry::Get().All();

	for (const TSharedRef<IAtlasRule>& Rule : Rules)
	{
		if (!Rule->IsEnabled())
		{
			continue;
		}

		TArray<FAtlasIssue> RuleIssues;
		Rule->Validate(Asset, RuleIssues);

		OutIssues.Append(RuleIssues);
	}
}

void FAtlasScanExecutor::GetAllScannableAssets(TArray<FAssetData>& OutAssets)
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	for (const FName& ClassName : ScannableAssetClasses)
	{
		TArray<FAssetData> ClassAssets;
		FARFilter Filter;
		Filter.ClassNames.Add(ClassName);
		Filter.bRecursiveClasses = true;
		Filter.PackagePaths.Add("/Game"); // Only scan game content

		AssetRegistry.GetAssets(Filter, ClassAssets);
		OutAssets.Append(ClassAssets);
	}
}

bool FAtlasScanExecutor::ShouldScanAssetType(const FName& ClassName)
{
	return ScannableAssetClasses.Contains(ClassName);
}
