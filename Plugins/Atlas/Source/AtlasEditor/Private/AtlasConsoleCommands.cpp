// Copyright Atlas Team. All Rights Reserved.

#include "AtlasConsoleCommands.h"
#include "Core/AtlasScanExecutor.h"
#include "Organizer/AtlasOrganizer.h"
#include "AtlasRuntime.h"
#include "HAL/IConsoleManager.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"

TArray<IConsoleObject*> FAtlasConsoleCommands::ConsoleCommands;

void FAtlasConsoleCommands::Register()
{
	ConsoleCommands.Add(IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("Atlas.ScanAll"),
		TEXT("Scan all assets for validation issues"),
		FConsoleCommandWithArgsDelegate::CreateStatic(&FAtlasConsoleCommands::ScanAll),
		ECVF_Default
	));

	ConsoleCommands.Add(IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("Atlas.ValidateSelection"),
		TEXT("Validate currently selected assets"),
		FConsoleCommandWithArgsDelegate::CreateStatic(&FAtlasConsoleCommands::ValidateSelection),
		ECVF_Default
	));

	ConsoleCommands.Add(IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("Atlas.OrganizeSelection"),
		TEXT("Organize currently selected assets"),
		FConsoleCommandWithArgsDelegate::CreateStatic(&FAtlasConsoleCommands::OrganizeSelection),
		ECVF_Default
	));

	ConsoleCommands.Add(IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("Atlas.ExportReport"),
		TEXT("Export Atlas report to JSON. Usage: Atlas.ExportReport <Path>"),
		FConsoleCommandWithArgsDelegate::CreateStatic(&FAtlasConsoleCommands::ExportReport),
		ECVF_Default
	));

	UE_LOG(LogAtlas, Log, TEXT("Registered %d Atlas console commands"), ConsoleCommands.Num());
}

void FAtlasConsoleCommands::Unregister()
{
	for (IConsoleObject* Cmd : ConsoleCommands)
	{
		if (Cmd)
		{
			IConsoleManager::Get().UnregisterConsoleObject(Cmd);
		}
	}
	ConsoleCommands.Empty();
}

void FAtlasConsoleCommands::ScanAll(const TArray<FString>& Args)
{
	UE_LOG(LogAtlas, Log, TEXT("Running full asset scan..."));

	TArray<FAtlasIssue> Issues;
	FAtlasScanExecutor::ScanAll(Issues);

	UE_LOG(LogAtlas, Log, TEXT("Scan complete. Found %d issues:"), Issues.Num());

	for (const FAtlasIssue& Issue : Issues)
	{
		const TCHAR* SeverityStr = TEXT("Info");
		switch (Issue.Severity)
		{
		case EAtlasSeverity::Error: SeverityStr = TEXT("Error"); break;
		case EAtlasSeverity::Warning: SeverityStr = TEXT("Warning"); break;
		}

		UE_LOG(LogAtlas, Log, TEXT("  [%s] %s: %s"), SeverityStr, *Issue.RuleName.ToString(), *Issue.Message);
	}
}

void FAtlasConsoleCommands::ValidateSelection(const TArray<FString>& Args)
{
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	TArray<FAssetData> SelectedAssets;
	ContentBrowserModule.Get().GetSelectedAssets(SelectedAssets);

	if (SelectedAssets.Num() == 0)
	{
		UE_LOG(LogAtlas, Warning, TEXT("No assets selected"));
		return;
	}

	UE_LOG(LogAtlas, Log, TEXT("Validating %d selected assets..."), SelectedAssets.Num());

	TArray<FAtlasIssue> Issues;
	FAtlasScanExecutor::ScanSelection(SelectedAssets, Issues);

	UE_LOG(LogAtlas, Log, TEXT("Validation complete. Found %d issues"), Issues.Num());

	for (const FAtlasIssue& Issue : Issues)
	{
		UE_LOG(LogAtlas, Log, TEXT("  %s: %s"), *Issue.RuleName.ToString(), *Issue.Message);
	}
}

void FAtlasConsoleCommands::OrganizeSelection(const TArray<FString>& Args)
{
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	TArray<FAssetData> SelectedAssets;
	ContentBrowserModule.Get().GetSelectedAssets(SelectedAssets);

	if (SelectedAssets.Num() == 0)
	{
		UE_LOG(LogAtlas, Warning, TEXT("No assets selected"));
		return;
	}

	UE_LOG(LogAtlas, Log, TEXT("Organizing %d selected assets..."), SelectedAssets.Num());

	TArray<UObject*> Assets;
	for (const FAssetData& AssetData : SelectedAssets)
	{
		if (UObject* Asset = AssetData.GetAsset())
		{
			Assets.Add(Asset);
		}
	}

	FAtlasOrganizer::NormalizeGroup(Assets);

	UE_LOG(LogAtlas, Log, TEXT("Organization complete"));
}

void FAtlasConsoleCommands::ExportReport(const TArray<FString>& Args)
{
	FString ExportPath = FPaths::ProjectSavedDir() / TEXT("Atlas/Report.json");

	if (Args.Num() > 0)
	{
		ExportPath = Args[0];
	}

	UE_LOG(LogAtlas, Log, TEXT("Exporting Atlas report to: %s"), *ExportPath);

	// TODO: Implement full JSON export
	// For now, just log
	TArray<FAtlasIssue> Issues;
	FAtlasScanExecutor::ScanAll(Issues);

	UE_LOG(LogAtlas, Log, TEXT("Report would contain %d issues"), Issues.Num());
	UE_LOG(LogAtlas, Warning, TEXT("Full JSON export not yet implemented"));
}
