// Copyright Atlas Team. All Rights Reserved.

#include "AtlasOverviewStats.h"
#include "Core/AtlasTextureUtils.h"
#include "Core/AtlasMeshUtils.h"
#include "Core/AtlasAssetUtils.h"
#include "AtlasRuntime.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/Texture2D.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "HAL/FileManager.h"

void FAtlasOverviewStats::CollectAll(
	TArray<FAtlasTextureStat>& OutTextures,
	TArray<FAtlasMeshStat>& OutMeshes,
	TArray<FAtlasLevelStat>& OutLevels,
	int64& OutProjectBytes,
	int64& OutVRAMBytes)
{
	UE_LOG(LogAtlas, Log, TEXT("Collecting project statistics..."));

	// Collect texture stats
	CollectTextureStats(OutTextures, OutVRAMBytes);

	// Collect mesh stats
	CollectMeshStats(OutMeshes);

	// Collect level stats
	CollectLevelStats(OutLevels);

	// Calculate project size
	OutProjectBytes = CalculateProjectSize();

	UE_LOG(LogAtlas, Log, TEXT("Statistics collected: %d textures, %d meshes, %d levels, Project: %lld MB, VRAM: %lld MB"),
		OutTextures.Num(), OutMeshes.Num(), OutLevels.Num(),
		OutProjectBytes / (1024 * 1024), OutVRAMBytes / (1024 * 1024));
}

void FAtlasOverviewStats::CollectTextureStats(TArray<FAtlasTextureStat>& OutTextures, int64& OutTotalVRAM)
{
	OutTextures.Empty();
	OutTotalVRAM = 0;

	TArray<FAssetData> TextureAssets;
	FAtlasAssetUtils::GetAllAssetsOfClass(FName(TEXT("Texture2D")), TextureAssets);

	for (const FAssetData& AssetData : TextureAssets)
	{
		UTexture2D* Texture = Cast<UTexture2D>(AssetData.GetAsset());
		if (!Texture)
		{
			continue;
		}

		FAtlasTextureStat Stat;
		Stat.Path = Texture->GetPathName();
		Stat.VRAMBytes = FAtlasTextureUtils::EstimateVRAMBytes(Texture);
		Stat.MaxDimension = FAtlasTextureUtils::GetMaxDimension(Texture);
		Stat.PixelFormat = FAtlasTextureUtils::GetPixelFormatName(Texture);

		OutTextures.Add(Stat);
		OutTotalVRAM += Stat.VRAMBytes;
	}

	// Sort by VRAM usage (descending)
	OutTextures.Sort([](const FAtlasTextureStat& A, const FAtlasTextureStat& B) {
		return A.VRAMBytes > B.VRAMBytes;
	});
}

void FAtlasOverviewStats::CollectMeshStats(TArray<FAtlasMeshStat>& OutMeshes)
{
	OutMeshes.Empty();

	TArray<FAssetData> MeshAssets;
	FAtlasAssetUtils::GetAllAssetsOfClass(FName(TEXT("StaticMesh")), MeshAssets);

	for (const FAssetData& AssetData : MeshAssets)
	{
		UStaticMesh* Mesh = Cast<UStaticMesh>(AssetData.GetAsset());
		if (!Mesh)
		{
			continue;
		}

		FAtlasMeshStat Stat;
		Stat.Path = Mesh->GetPathName();
		Stat.TriangleCount = FAtlasMeshUtils::GetTriangleCount(Mesh);
		Stat.NumLODs = FAtlasMeshUtils::GetLODCount(Mesh);

		OutMeshes.Add(Stat);
	}

	// Sort by triangle count (descending)
	OutMeshes.Sort([](const FAtlasMeshStat& A, const FAtlasMeshStat& B) {
		return A.TriangleCount > B.TriangleCount;
	});
}

void FAtlasOverviewStats::CollectLevelStats(TArray<FAtlasLevelStat>& OutLevels)
{
	OutLevels.Empty();

	TArray<FAssetData> LevelAssets;
	FAtlasAssetUtils::GetAllAssetsOfClass(FName(TEXT("World")), LevelAssets);

	for (const FAssetData& AssetData : LevelAssets)
	{
		FAtlasLevelStat Stat;
		Stat.Path = AssetData.ObjectPath.ToString();

		// Get package file size
		FString PackageFilename = AssetData.PackageName.ToString();
		if (FPackageName::DoesPackageExist(PackageFilename, nullptr, &PackageFilename))
		{
			Stat.TotalBytes = IFileManager::Get().FileSize(*PackageFilename);
		}
		else
		{
			Stat.TotalBytes = 0;
		}

		OutLevels.Add(Stat);
	}

	// Sort by size (descending)
	OutLevels.Sort([](const FAtlasLevelStat& A, const FAtlasLevelStat& B) {
		return A.TotalBytes > B.TotalBytes;
	});
}

int64 FAtlasOverviewStats::CalculateProjectSize()
{
	int64 TotalSize = 0;

	// Get Content directory
	FString ContentDir = FPaths::ProjectContentDir();

	// Recursively calculate size
	TArray<FString> Files;
	IFileManager::Get().FindFilesRecursive(Files, *ContentDir, TEXT("*.*"), true, false);

	for (const FString& File : Files)
	{
		int64 FileSize = IFileManager::Get().FileSize(*File);
		if (FileSize > 0)
		{
			TotalSize += FileSize;
		}
	}

	return TotalSize;
}
