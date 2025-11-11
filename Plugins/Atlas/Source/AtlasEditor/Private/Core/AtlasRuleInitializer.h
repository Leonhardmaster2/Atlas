// Copyright Atlas Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * Registers all built-in Atlas validation rules
 */
class FAtlasRuleInitializer
{
public:
	/**
	 * Register all default rules with the rule registry
	 */
	static void RegisterDefaultRules();
};
