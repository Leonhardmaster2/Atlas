// Copyright Atlas Team. All Rights Reserved.

#include "AtlasRuntime.h"

#define LOCTEXT_NAMESPACE "FAtlasRuntimeModule"

DEFINE_LOG_CATEGORY(LogAtlas);

void FAtlasRuntimeModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	UE_LOG(LogAtlas, Log, TEXT("AtlasRuntime module started"));
}

void FAtlasRuntimeModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	UE_LOG(LogAtlas, Log, TEXT("AtlasRuntime module shutdown"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FAtlasRuntimeModule, AtlasRuntime)
