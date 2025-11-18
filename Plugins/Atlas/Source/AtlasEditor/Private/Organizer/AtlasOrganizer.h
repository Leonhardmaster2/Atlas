// Copyright Atlas Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AtlasTypes.h"

class UFactory;
class UMaterial;
class UTexture;

/**
 * Atlas Organizer handles automatic organization of imported assets
 * Normalizes naming, folder structure, and creates PBR material setups
 */
class FAtlasOrganizer
{
public:
	/**
	 * Initialize the organizer and hook into import events
	 */
	static void Initialize();

	/**
	 * Shutdown and unhook from import events
	 */
	static void Shutdown();

	/**
	 * Called when assets are imported
	 * @param Factory The factory that imported the assets
	 * @param Object The imported object
	 */
	static void OnPostImport(UFactory* Factory, UObject* Object);

	/**
	 * Normalize a group of related assets (mesh + textures + materials)
	 * @param Group The assets to normalize
	 */
	static void NormalizeGroup(const TArray<UObject*>& Group);

	/**
	 * Suggest a base name from a raw asset name
	 * @param RawName The original asset name
	 * @return Suggested base name
	 */
	static FString SuggestBaseName(const FString& RawName);

	/**
	 * Compute destination paths for organized assets
	 * @param BaseName The base name for the asset group
	 * @param OutMeshPath Output mesh folder path
	 * @param OutMatPath Output material folder path
	 * @param OutTexPath Output texture folder path
	 */
	static void ComputeDestPaths(const FString& BaseName, FString& OutMeshPath, FString& OutMatPath, FString& OutTexPath);

	/**
	 * Apply naming convention to an asset
	 * @param Asset The asset to rename
	 * @param BaseName The base name to use
	 */
	static void ApplyNaming(UObject* Asset, const FString& BaseName);

	/**
	 * Relink a material graph with detected textures
	 * @param Material The material to relink
	 * @param TexByType Map of texture type to texture asset
	 */
	static void RelinkMaterialGraph(UMaterial* Material, const TMap<EAtlasMapType, UTexture*>& TexByType);

	/**
	 * Group imported assets by base name
	 * @param ImportedAssets The assets that were imported together
	 * @return Map of base name to related assets
	 */
	static TMap<FString, TArray<UObject*>> GroupAssetsByBaseName(const TArray<UObject*>& ImportedAssets);

	/**
	 * Show organizer prompt dialog to user
	 * @param DetectedName The detected base name
	 * @param SuggestedName The suggested normalized name
	 * @return Result of the prompt
	 */
	static FAtlasOrganizerResult ShowOrganizerPrompt(const FString& DetectedName, const FString& SuggestedName);

private:
	static FDelegateHandle PostImportDelegateHandle;
	static TArray<UObject*> PendingImports;
};
