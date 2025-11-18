// Copyright Atlas Team. All Rights Reserved.

#include "AtlasRuleInitializer.h"
#include "AtlasRuleRegistry.h"
#include "AtlasRuntime.h"
#include "Rules/AtlasNamingConventionRule.h"
#include "Rules/AtlasTextureCompressionRule.h"
#include "Rules/AtlasStaticMeshCollisionRule.h"
#include "Rules/AtlasTextureSizeRule.h"
#include "Rules/AtlasStaticMeshLODRule.h"

void FAtlasRuleInitializer::RegisterDefaultRules()
{
	UE_LOG(LogAtlas, Log, TEXT("Registering default Atlas validation rules"));

	FAtlasRuleRegistry& Registry = FAtlasRuleRegistry::Get();

	// Register Naming Convention Rule
	Registry.Register(MakeShared<FAtlasNamingConventionRule>());

	// Register Texture Rules
	Registry.Register(MakeShared<FAtlasTextureCompressionRule>());
	Registry.Register(MakeShared<FAtlasTextureSizeRule>());

	// Register Mesh Rules
	Registry.Register(MakeShared<FAtlasStaticMeshCollisionRule>());
	Registry.Register(MakeShared<FAtlasStaticMeshLODRule>());

	UE_LOG(LogAtlas, Log, TEXT("Registered %d validation rules"), Registry.All().Num());
}
