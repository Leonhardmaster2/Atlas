// Copyright Atlas Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IAtlasRule.h"

/**
 * Validates that textures don't exceed maximum dimension limits
 */
class FAtlasTextureSizeRule : public IAtlasRule
{
public:
	virtual ~FAtlasTextureSizeRule() = default;

	virtual FName GetRuleName() const override;
	virtual void Validate(const UObject* Asset, TArray<FAtlasIssue>& OutIssues) override;
	virtual FString GetDescription() const override;
	virtual bool IsEnabled() const override;

private:
	void CreateFixFunction(FAtlasIssue& Issue, class UTexture2D* Texture, int32 MaxSize);
};
