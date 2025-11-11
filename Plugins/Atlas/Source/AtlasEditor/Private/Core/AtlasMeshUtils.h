// Copyright Atlas Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class UStaticMesh;

/**
 * Utility functions for mesh operations
 */
class FAtlasMeshUtils
{
public:
	/**
	 * Get the total triangle count for a static mesh (LOD 0)
	 * @param Mesh The mesh to analyze
	 * @return Triangle count
	 */
	static int64 GetTriangleCount(const UStaticMesh* Mesh);

	/**
	 * Ensure a mesh has collision geometry
	 * @param Mesh The mesh to check/modify
	 * @return True if collision exists or was created successfully
	 */
	static bool EnsureCollision(UStaticMesh* Mesh);

	/**
	 * Generate LODs for a mesh if needed
	 * @param Mesh The mesh to process
	 * @param TargetLODs Number of LODs to generate
	 * @param MaxTris Maximum triangles for LOD 0 before generating LODs
	 * @return True if successful
	 */
	static bool GenerateLODsIfNeeded(UStaticMesh* Mesh, int32 TargetLODs, int32 MaxTris);

	/**
	 * Get the number of LODs for a mesh
	 * @param Mesh The mesh to check
	 * @return Number of LODs
	 */
	static int32 GetLODCount(const UStaticMesh* Mesh);

	/**
	 * Check if a mesh has valid collision
	 * @param Mesh The mesh to check
	 * @return True if has collision
	 */
	static bool HasCollision(const UStaticMesh* Mesh);

	/**
	 * Get the vertex count for a mesh (LOD 0)
	 * @param Mesh The mesh to analyze
	 * @return Vertex count
	 */
	static int32 GetVertexCount(const UStaticMesh* Mesh);
};
