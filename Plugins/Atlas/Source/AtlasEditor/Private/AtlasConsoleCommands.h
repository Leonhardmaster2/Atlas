// Copyright Atlas Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * Console commands for Atlas
 */
class FAtlasConsoleCommands
{
public:
	/** Register all Atlas console commands */
	static void Register();

	/** Unregister all Atlas console commands */
	static void Unregister();

private:
	/** Scan all assets */
	static void ScanAll(const TArray<FString>& Args);

	/** Validate selection */
	static void ValidateSelection(const TArray<FString>& Args);

	/** Organize selection */
	static void OrganizeSelection(const TArray<FString>& Args);

	/** Export report */
	static void ExportReport(const TArray<FString>& Args);

private:
	static TArray<IConsoleObject*> ConsoleCommands;
};
