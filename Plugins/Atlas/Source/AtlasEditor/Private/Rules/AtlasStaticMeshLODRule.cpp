// Copyright Atlas Team. All Rights Reserved.

#include "AtlasStaticMeshLODRule.h"
#include "AtlasSettings.h"
#include "Core/AtlasMeshUtils.h"
#include "AtlasRuntime.h"
#include "Engine/StaticMesh.h"

FName FAtlasStaticMeshLODRule::GetRuleName() const
{
	return TEXT("StaticMeshLOD");
}

FString FAtlasStaticMeshLODRule::GetDescription() const
{
	return TEXT("Ensures high-poly meshes have appropriate LOD levels");
}

bool FAtlasStaticMeshLODRule::IsEnabled() const
{
	return true;
}

void FAtlasStaticMeshLODRule::Validate(const UObject* Asset, TArray<FAtlasIssue>& OutIssues)
{
	const UStaticMesh* Mesh = Cast<UStaticMesh>(Asset);
	if (!Mesh)
	{
		return;
	}

	const UAtlasSettings* Settings = UAtlasSettings::Get();
	if (!Settings)
	{
		return;
	}

	int64 TriCount = FAtlasMeshUtils::GetTriangleCount(Mesh);
	int32 NumLODs = FAtlasMeshUtils::GetLODCount(Mesh);

	// If tri count exceeds threshold and mesh has insufficient LODs
	if (TriCount > Settings->MaxTriCount && NumLODs < 3)
	{
		FString Message = FString::Printf(
			TEXT("High-poly mesh '%s' (%lld tris) should have LODs (currently has %d)"),
			*Mesh->GetName(),
			TriCount,
			NumLODs
		);

		FAtlasIssue Issue(
			Mesh->GetPathName(),
			GetRuleName(),
			EAtlasSeverity::Warning,
			Message,
			true // Can fix
		);

		CreateFixFunction(Issue, const_cast<UStaticMesh*>(Mesh), 3);
		OutIssues.Add(Issue);
	}
}

void FAtlasStaticMeshLODRule::CreateFixFunction(FAtlasIssue& Issue, UStaticMesh* Mesh, int32 TargetLODs)
{
	Issue.FixFunction = [Mesh, TargetLODs]() -> bool
	{
		if (!Mesh)
		{
			return false;
		}

		const UAtlasSettings* Settings = UAtlasSettings::Get();
		int32 MaxTris = Settings ? Settings->MaxTriCount : 25000;

		bool bSuccess = FAtlasMeshUtils::GenerateLODsIfNeeded(Mesh, TargetLODs, MaxTris);

		if (bSuccess)
		{
			UE_LOG(LogAtlas, Log, TEXT("Generated LODs for mesh: %s"), *Mesh->GetName());
		}
		else
		{
			UE_LOG(LogAtlas, Warning, TEXT("Failed to generate LODs for mesh: %s"), *Mesh->GetName());
		}

		return bSuccess;
	};
}
