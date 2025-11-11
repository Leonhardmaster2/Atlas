// Copyright Atlas Team. All Rights Reserved.

#include "AtlasRuleRegistry.h"
#include "IAtlasRule.h"
#include "AtlasRuntime.h"

FAtlasRuleRegistry& FAtlasRuleRegistry::Get()
{
	static FAtlasRuleRegistry Instance;
	return Instance;
}

void FAtlasRuleRegistry::Register(TSharedRef<IAtlasRule> Rule)
{
	// Check if rule with this name already exists
	FName RuleName = Rule->GetRuleName();
	for (const TSharedRef<IAtlasRule>& ExistingRule : Rules)
	{
		if (ExistingRule->GetRuleName() == RuleName)
		{
			UE_LOG(LogAtlas, Warning, TEXT("Rule '%s' is already registered. Skipping."), *RuleName.ToString());
			return;
		}
	}

	Rules.Add(Rule);
	UE_LOG(LogAtlas, Log, TEXT("Registered Atlas rule: %s"), *RuleName.ToString());
}

const TArray<TSharedRef<IAtlasRule>>& FAtlasRuleRegistry::All() const
{
	return Rules;
}

TSharedPtr<IAtlasRule> FAtlasRuleRegistry::FindRule(FName RuleName) const
{
	for (const TSharedRef<IAtlasRule>& Rule : Rules)
	{
		if (Rule->GetRuleName() == RuleName)
		{
			return Rule;
		}
	}
	return nullptr;
}

void FAtlasRuleRegistry::Clear()
{
	Rules.Empty();
	UE_LOG(LogAtlas, Log, TEXT("Cleared all Atlas rules"));
}
