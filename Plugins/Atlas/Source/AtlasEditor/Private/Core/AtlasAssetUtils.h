// Copyright Atlas Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * Utility functions for asset operations
 */
class FAtlasAssetUtils
{
public:
	/**
	 * Move an asset to a new path
	 * @param SrcPath Source asset path
	 * @param DestPath Destination path
	 * @return True if successful
	 */
	static bool MoveAsset(const FName& SrcPath, const FString& DestPath);

	/**
	 * Rename an asset
	 * @param AssetPath Current asset path
	 * @param NewName New name for the asset
	 * @return True if successful
	 */
	static bool RenameAsset(const FName& AssetPath, const FString& NewName);

	/**
	 * Reimport an asset from its source file
	 * @param Asset The asset to reimport
	 * @return True if successful
	 */
	static bool Reimport(UObject* Asset);

	/**
	 * Sync asset to Content Browser
	 * @param Path Asset path
	 * @return True if successful
	 */
	static bool SyncToCB(const FString& Path);

	/**
	 * Load an asset by its path
	 * @param AssetPath The path to the asset
	 * @return The loaded asset, or nullptr if not found
	 */
	static UObject* LoadAsset(const FString& AssetPath);

	/**
	 * Get all assets of a specific class
	 * @param ClassName The class name to filter by
	 * @param OutAssets Array to fill with found assets
	 */
	static void GetAllAssetsOfClass(const FName& ClassName, TArray<struct FAssetData>& OutAssets);

	/**
	 * Check if an asset path is valid
	 * @param AssetPath The path to check
	 * @return True if the path is valid
	 */
	static bool IsValidAssetPath(const FString& AssetPath);

	/**
	 * Get the prefix for an asset type from settings
	 * @param AssetType The asset type name
	 * @return The prefix, or empty string if not found
	 */
	static FString GetPrefixForAssetType(const FName& AssetType);
};
