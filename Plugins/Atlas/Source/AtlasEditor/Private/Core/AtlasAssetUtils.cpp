// Copyright Atlas Team. All Rights Reserved.

#include "AtlasAssetUtils.h"
#include "AtlasSettings.h"
#include "AtlasRuntime.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "EditorAssetLibrary.h"
#include "FileHelpers.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"

bool FAtlasAssetUtils::MoveAsset(const FName& SrcPath, const FString& DestPath)
{
	FString SrcPathStr = SrcPath.ToString();

	if (!UEditorAssetLibrary::DoesAssetExist(SrcPathStr))
	{
		UE_LOG(LogAtlas, Warning, TEXT("Cannot move asset - source does not exist: %s"), *SrcPathStr);
		return false;
	}

	if (UEditorAssetLibrary::RenameAsset(SrcPathStr, DestPath))
	{
		UE_LOG(LogAtlas, Log, TEXT("Moved asset from %s to %s"), *SrcPathStr, *DestPath);
		return true;
	}

	return false;
}

bool FAtlasAssetUtils::RenameAsset(const FName& AssetPath, const FString& NewName)
{
	FString AssetPathStr = AssetPath.ToString();

	if (!UEditorAssetLibrary::DoesAssetExist(AssetPathStr))
	{
		UE_LOG(LogAtlas, Warning, TEXT("Cannot rename asset - does not exist: %s"), *AssetPathStr);
		return false;
	}

	FString Directory = FPaths::GetPath(AssetPathStr);
	FString NewPath = Directory / NewName;

	if (UEditorAssetLibrary::RenameAsset(AssetPathStr, NewPath))
	{
		UE_LOG(LogAtlas, Log, TEXT("Renamed asset %s to %s"), *AssetPathStr, *NewName);
		return true;
	}

	return false;
}

bool FAtlasAssetUtils::Reimport(UObject* Asset)
{
	if (!Asset)
	{
		return false;
	}

	if (FReimportManager::Instance()->Reimport(Asset, false))
	{
		UE_LOG(LogAtlas, Log, TEXT("Reimported asset: %s"), *Asset->GetName());
		return true;
	}

	return false;
}

bool FAtlasAssetUtils::SyncToCB(const FString& Path)
{
	if (!UEditorAssetLibrary::DoesAssetExist(Path))
	{
		return false;
	}

	TArray<FString> Paths;
	Paths.Add(Path);

	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	ContentBrowserModule.Get().SyncBrowserToAssets(Paths);

	return true;
}

UObject* FAtlasAssetUtils::LoadAsset(const FString& AssetPath)
{
	return UEditorAssetLibrary::LoadAsset(AssetPath);
}

void FAtlasAssetUtils::GetAllAssetsOfClass(const FName& ClassName, TArray<FAssetData>& OutAssets)
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	FARFilter Filter;
	Filter.ClassNames.Add(ClassName);
	Filter.bRecursiveClasses = true;

	AssetRegistry.GetAssets(Filter, OutAssets);
}

bool FAtlasAssetUtils::IsValidAssetPath(const FString& AssetPath)
{
	return AssetPath.StartsWith(TEXT("/Game/")) || AssetPath.StartsWith(TEXT("/Engine/"));
}

FString FAtlasAssetUtils::GetPrefixForAssetType(const FName& AssetType)
{
	const UAtlasSettings* Settings = UAtlasSettings::Get();
	if (Settings)
	{
		const FString* Prefix = Settings->NamingPrefixes.Find(AssetType);
		if (Prefix)
		{
			return *Prefix;
		}
	}
	return FString();
}
