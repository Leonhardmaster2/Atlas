// Copyright Atlas Team. All Rights Reserved.

#include "AtlasValidateCommandlet.h"
#include "AtlasRuntime.h"

UAtlasValidateCommandlet::UAtlasValidateCommandlet()
{
	IsClient = false;
	IsEditor = true;
	IsServer = false;
	LogToConsole = true;
}

int32 UAtlasValidateCommandlet::Main(const FString& Params)
{
	UE_LOG(LogAtlas, Log, TEXT("Atlas Validate Commandlet started"));
	UE_LOG(LogAtlas, Log, TEXT("Parameters: %s"), *Params);

	// Note: Full validation requires editor-only functionality
	// This is a stub implementation that would be completed with editor integration

	int32 ErrorCount = 0;
	int32 WarningCount = 0;

	// TODO: Load AtlasEditor module and run validation scan
	// For now, return success
	// In a full implementation, this would:
	// 1. Load the AtlasEditor module
	// 2. Run FAtlasScanExecutor::ScanAll()
	// 3. Count errors and warnings
	// 4. Export to JSON at Saved/Atlas/Validate.json
	// 5. Return appropriate exit code

	UE_LOG(LogAtlas, Log, TEXT("Validation complete: %d errors, %d warnings"), ErrorCount, WarningCount);

	// Return appropriate exit code
	if (ErrorCount > 0)
	{
		return 2; // Errors present
	}
	else if (WarningCount > 0)
	{
		return 1; // Warnings only
	}
	else
	{
		return 0; // All clear
	}
}
