// Copyright Atlas Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AtlasTypes.h"

/**
 * Interface for Atlas validation rules
 * Each rule checks assets for specific issues and can optionally auto-fix them
 */
class IAtlasRule
{
public:
	virtual ~IAtlasRule() = default;

	/**
	 * Get the unique name of this rule
	 * @return The rule name (e.g. "NamingConvention", "TextureCompression")
	 */
	virtual FName GetRuleName() const = 0;

	/**
	 * Validate an asset against this rule
	 * @param Asset The asset to validate
	 * @param OutIssues Array to append any found issues to
	 */
	virtual void Validate(const UObject* Asset, TArray<FAtlasIssue>& OutIssues) = 0;

	/**
	 * Get a description of what this rule checks
	 * @return Human-readable description
	 */
	virtual FString GetDescription() const { return TEXT(""); }

	/**
	 * Check if this rule is enabled
	 * @return True if the rule should run
	 */
	virtual bool IsEnabled() const { return true; }
};
