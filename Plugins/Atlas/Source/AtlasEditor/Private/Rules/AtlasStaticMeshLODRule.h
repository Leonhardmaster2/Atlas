// Copyright Atlas Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IAtlasRule.h"

/**
 * Validates that high-poly static meshes have appropriate LODs
 */
class FAtlasStaticMeshLODRule : public IAtlasRule
{
public:
	virtual ~FAtlasStaticMeshLODRule() = default;

	virtual FName GetRuleName() const override;
	virtual void Validate(const UObject* Asset, TArray<FAtlasIssue>& OutIssues) override;
	virtual FString GetDescription() const override;
	virtual bool IsEnabled() const override;

private:
	void CreateFixFunction(FAtlasIssue& Issue, class UStaticMesh* Mesh, int32 TargetLODs);
};
