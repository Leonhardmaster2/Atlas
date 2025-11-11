// Copyright Atlas Team. All Rights Reserved.

#include "AtlasRuleInitializer.h"
#include "AtlasRuleRegistry.h"
#include "AtlasRuntime.h"
#include "Rules/AtlasNamingConventionRule.h"
#include "Rules/AtlasTextureCompressionRule.h"
#include "Rules/AtlasStaticMeshCollisionRule.h"

void FAtlasRuleInitializer::RegisterDefaultRules()
{
	UE_LOG(LogAtlas, Log, TEXT("Registering default Atlas validation rules"));

	FAtlasRuleRegistry& Registry = FAtlasRuleRegistry::Get();

	// Register Naming Convention Rule
	Registry.Register(MakeShared<FAtlasNamingConventionRule>());

	// Register Texture Compression Rule
	Registry.Register(MakeShared<FAtlasTextureCompressionRule>());

	// Register Static Mesh Collision Rule
	Registry.Register(MakeShared<FAtlasStaticMeshCollisionRule>());

	// Additional rules can be registered here following the same pattern
	// Registry.Register(MakeShared<FAtlasTextureSizeRule>());
	// Registry.Register(MakeShared<FAtlasStaticMeshLODRule>());
	// Registry.Register(MakeShared<FAtlasMaterialInstanceDupRule>());
	// etc.

	UE_LOG(LogAtlas, Log, TEXT("Registered %d validation rules"), Registry.All().Num());
}
