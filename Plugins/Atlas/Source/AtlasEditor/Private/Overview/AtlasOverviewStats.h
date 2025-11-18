// Copyright Atlas Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AtlasTypes.h"

/**
 * Collects and manages project-wide statistics for Atlas Overview
 */
class FAtlasOverviewStats
{
public:
	/**
	 * Collect all project statistics
	 * @param OutTextures Texture statistics
	 * @param OutMeshes Mesh statistics
	 * @param OutLevels Level statistics
	 * @param OutProjectBytes Total project size in bytes
	 * @param OutVRAMBytes Total estimated VRAM usage in bytes
	 */
	static void CollectAll(
		TArray<FAtlasTextureStat>& OutTextures,
		TArray<FAtlasMeshStat>& OutMeshes,
		TArray<FAtlasLevelStat>& OutLevels,
		int64& OutProjectBytes,
		int64& OutVRAMBytes
	);

	/**
	 * Collect texture statistics
	 * @param OutTextures Output array of texture stats
	 * @param OutTotalVRAM Total VRAM usage
	 */
	static void CollectTextureStats(TArray<FAtlasTextureStat>& OutTextures, int64& OutTotalVRAM);

	/**
	 * Collect mesh statistics
	 * @param OutMeshes Output array of mesh stats
	 */
	static void CollectMeshStats(TArray<FAtlasMeshStat>& OutMeshes);

	/**
	 * Collect level statistics
	 * @param OutLevels Output array of level stats
	 */
	static void CollectLevelStats(TArray<FAtlasLevelStat>& OutLevels);

	/**
	 * Calculate total project size
	 * @return Total size in bytes
	 */
	static int64 CalculateProjectSize();
};
