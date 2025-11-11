// Copyright Atlas Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IAtlasRule.h"

/**
 * Validates that assets follow naming conventions defined in settings
 */
class FAtlasNamingConventionRule : public IAtlasRule
{
public:
	virtual ~FAtlasNamingConventionRule() = default;

	virtual FName GetRuleName() const override;
	virtual void Validate(const UObject* Asset, TArray<FAtlasIssue>& OutIssues) override;
	virtual FString GetDescription() const override;
	virtual bool IsEnabled() const override;

private:
	bool HasCorrectPrefix(const UObject* Asset, FString& OutExpectedPrefix) const;
	bool HasCorrectPath(const UObject* Asset, FString& OutExpectedPath) const;
	void CreateFixFunction(FAtlasIssue& Issue, const UObject* Asset, const FString& ExpectedName);
};
