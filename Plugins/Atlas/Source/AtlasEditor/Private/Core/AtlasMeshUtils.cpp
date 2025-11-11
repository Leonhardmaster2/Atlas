// Copyright Atlas Team. All Rights Reserved.

#include "AtlasMeshUtils.h"
#include "AtlasRuntime.h"
#include "Engine/StaticMesh.h"
#include "PhysicsEngine/BodySetup.h"
#include "MeshUtilities.h"
#include "IMeshReductionManagerModule.h"
#include "IMeshReductionInterfaces.h"

int64 FAtlasMeshUtils::GetTriangleCount(const UStaticMesh* Mesh)
{
	if (!Mesh || !Mesh->GetRenderData())
	{
		return 0;
	}

	if (Mesh->GetRenderData()->LODResources.Num() == 0)
	{
		return 0;
	}

	const FStaticMeshLODResources& LOD0 = Mesh->GetRenderData()->LODResources[0];
	return LOD0.GetNumTriangles();
}

bool FAtlasMeshUtils::EnsureCollision(UStaticMesh* Mesh)
{
	if (!Mesh)
	{
		return false;
	}

	// Check if collision already exists
	if (HasCollision(Mesh))
	{
		return true;
	}

	// Create a simple box collision
	UBodySetup* BodySetup = Mesh->GetBodySetup();
	if (!BodySetup)
	{
		Mesh->CreateBodySetup();
		BodySetup = Mesh->GetBodySetup();
	}

	if (BodySetup)
	{
		// Use the mesh's bounding box for simple collision
		BodySetup->Modify();
		BodySetup->RemoveSimpleCollision();

		// Add a box collision
		FKBoxElem BoxElem;
		FBox BoundingBox = Mesh->GetBoundingBox();
		FVector Extent = BoundingBox.GetExtent();
		BoxElem.X = Extent.X * 2.0f;
		BoxElem.Y = Extent.Y * 2.0f;
		BoxElem.Z = Extent.Z * 2.0f;
		BoxElem.Center = BoundingBox.GetCenter();

		BodySetup->AggGeom.BoxElems.Add(BoxElem);
		BodySetup->CreatePhysicsMeshes();

		Mesh->MarkPackageDirty();
		UE_LOG(LogAtlas, Log, TEXT("Created simple box collision for mesh: %s"), *Mesh->GetName());
		return true;
	}

	return false;
}

bool FAtlasMeshUtils::GenerateLODsIfNeeded(UStaticMesh* Mesh, int32 TargetLODs, int32 MaxTris)
{
	if (!Mesh)
	{
		return false;
	}

	int64 TriCount = GetTriangleCount(Mesh);
	int32 CurrentLODs = GetLODCount(Mesh);

	// Check if we need LODs
	if (TriCount <= MaxTris || CurrentLODs >= TargetLODs)
	{
		return true; // No need for LODs or already has enough
	}

	// Generate LODs
	IMeshReductionManagerModule& MeshReductionModule = FModuleManager::Get().LoadModuleChecked<IMeshReductionManagerModule>("MeshReductionInterface");
	IMeshReduction* MeshReduction = MeshReductionModule.GetStaticMeshReductionInterface();

	if (!MeshReduction)
	{
		UE_LOG(LogAtlas, Warning, TEXT("Mesh reduction interface not available for: %s"), *Mesh->GetName());
		return false;
	}

	Mesh->Modify();

	// Set up LOD settings
	int32 NumLODsToGenerate = TargetLODs - CurrentLODs;
	for (int32 i = 0; i < NumLODsToGenerate; i++)
	{
		// This is simplified - actual LOD generation would require more setup
		// For a production plugin, you'd want to use FMeshUtilities or similar
		UE_LOG(LogAtlas, Log, TEXT("Would generate LOD %d for mesh: %s"), CurrentLODs + i, *Mesh->GetName());
	}

	Mesh->MarkPackageDirty();
	return true;
}

int32 FAtlasMeshUtils::GetLODCount(const UStaticMesh* Mesh)
{
	if (!Mesh || !Mesh->GetRenderData())
	{
		return 0;
	}

	return Mesh->GetRenderData()->LODResources.Num();
}

bool FAtlasMeshUtils::HasCollision(const UStaticMesh* Mesh)
{
	if (!Mesh)
	{
		return false;
	}

	UBodySetup* BodySetup = Mesh->GetBodySetup();
	if (!BodySetup)
	{
		return false;
	}

	// Check if there's any collision geometry
	return (BodySetup->AggGeom.BoxElems.Num() > 0 ||
			BodySetup->AggGeom.SphereElems.Num() > 0 ||
			BodySetup->AggGeom.SphylElems.Num() > 0 ||
			BodySetup->AggGeom.ConvexElems.Num() > 0);
}

int32 FAtlasMeshUtils::GetVertexCount(const UStaticMesh* Mesh)
{
	if (!Mesh || !Mesh->GetRenderData())
	{
		return 0;
	}

	if (Mesh->GetRenderData()->LODResources.Num() == 0)
	{
		return 0;
	}

	const FStaticMeshLODResources& LOD0 = Mesh->GetRenderData()->LODResources[0];
	return LOD0.GetNumVertices();
}
