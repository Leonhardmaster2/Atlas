// Copyright Atlas Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AtlasTypes.h"

struct FAssetData;

/**
 * Executes validation scans across assets using registered rules
 */
class FAtlasScanExecutor
{
public:
	/**
	 * Scan all assets in the project
	 * @param OutIssues Array to fill with found issues
	 */
	static void ScanAll(TArray<FAtlasIssue>& OutIssues);

	/**
	 * Scan only selected assets
	 * @param Selection Assets to scan
	 * @param OutIssues Array to fill with found issues
	 */
	static void ScanSelection(const TArray<FAssetData>& Selection, TArray<FAtlasIssue>& OutIssues);

	/**
	 * Scan a single asset
	 * @param Asset The asset to scan
	 * @param OutIssues Array to fill with found issues
	 */
	static void ScanAsset(UObject* Asset, TArray<FAtlasIssue>& OutIssues);

	/**
	 * Get all assets that should be scanned
	 * @param OutAssets Array to fill with assets
	 */
	static void GetAllScannableAssets(TArray<FAssetData>& OutAssets);

	/**
	 * Check if an asset type should be scanned
	 * @param ClassName The class name to check
	 * @return True if this type should be scanned
	 */
	static bool ShouldScanAssetType(const FName& ClassName);

private:
	/** Asset classes that Atlas scans */
	static const TArray<FName> ScannableAssetClasses;
};
