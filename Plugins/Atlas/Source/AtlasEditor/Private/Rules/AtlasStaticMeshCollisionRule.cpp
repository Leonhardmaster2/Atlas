// Copyright Atlas Team. All Rights Reserved.

#include "AtlasStaticMeshCollisionRule.h"
#include "Core/AtlasMeshUtils.h"
#include "AtlasRuntime.h"
#include "Engine/StaticMesh.h"

FName FAtlasStaticMeshCollisionRule::GetRuleName() const
{
	return TEXT("StaticMeshCollision");
}

FString FAtlasStaticMeshCollisionRule::GetDescription() const
{
	return TEXT("Ensures static meshes have collision geometry");
}

bool FAtlasStaticMeshCollisionRule::IsEnabled() const
{
	return true;
}

void FAtlasStaticMeshCollisionRule::Validate(const UObject* Asset, TArray<FAtlasIssue>& OutIssues)
{
	const UStaticMesh* Mesh = Cast<UStaticMesh>(Asset);
	if (!Mesh)
	{
		return;
	}

	// Check if mesh has collision
	if (!FAtlasMeshUtils::HasCollision(Mesh))
	{
		FString Message = FString::Printf(
			TEXT("Static mesh '%s' has no collision geometry"),
			*Mesh->GetName()
		);

		FAtlasIssue Issue(
			Mesh->GetPathName(),
			GetRuleName(),
			EAtlasSeverity::Error,
			Message,
			true // Can fix
		);

		// Create fix function
		CreateFixFunction(Issue, const_cast<UStaticMesh*>(Mesh));

		OutIssues.Add(Issue);
	}
}

void FAtlasStaticMeshCollisionRule::CreateFixFunction(FAtlasIssue& Issue, UStaticMesh* Mesh)
{
	Issue.FixFunction = [Mesh]() -> bool
	{
		if (!Mesh)
		{
			return false;
		}

		bool bSuccess = FAtlasMeshUtils::EnsureCollision(Mesh);

		if (bSuccess)
		{
			UE_LOG(LogAtlas, Log, TEXT("Created collision for mesh: %s"), *Mesh->GetName());
		}
		else
		{
			UE_LOG(LogAtlas, Warning, TEXT("Failed to create collision for mesh: %s"), *Mesh->GetName());
		}

		return bSuccess;
	};
}
