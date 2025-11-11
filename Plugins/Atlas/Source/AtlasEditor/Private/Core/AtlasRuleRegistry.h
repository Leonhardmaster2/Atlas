// Copyright Atlas Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class IAtlasRule;

/**
 * Central registry for all Atlas validation rules
 * Manages rule registration and lookup
 */
class FAtlasRuleRegistry
{
public:
	/**
	 * Get the singleton instance
	 */
	static FAtlasRuleRegistry& Get();

	/**
	 * Register a new validation rule
	 * @param Rule The rule to register
	 */
	void Register(TSharedRef<IAtlasRule> Rule);

	/**
	 * Get all registered rules
	 * @return Array of all registered rules
	 */
	const TArray<TSharedRef<IAtlasRule>>& All() const;

	/**
	 * Find a rule by name
	 * @param RuleName The name of the rule to find
	 * @return The rule if found, nullptr otherwise
	 */
	TSharedPtr<IAtlasRule> FindRule(FName RuleName) const;

	/**
	 * Unregister all rules
	 */
	void Clear();

private:
	FAtlasRuleRegistry() = default;
	~FAtlasRuleRegistry() = default;

	// Non-copyable
	FAtlasRuleRegistry(const FAtlasRuleRegistry&) = delete;
	FAtlasRuleRegistry& operator=(const FAtlasRuleRegistry&) = delete;

private:
	TArray<TSharedRef<IAtlasRule>> Rules;
};
