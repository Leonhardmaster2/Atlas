// Copyright Atlas Team. All Rights Reserved.

#include "AtlasEditor.h"
#include "AtlasEditorStyle.h"
#include "AtlasCommands.h"
#include "AtlasConsoleCommands.h"
#include "Core/AtlasRuleRegistry.h"
#include "Core/AtlasRuleInitializer.h"
#include "Organizer/AtlasOrganizer.h"
#include "Widgets/SAtlasOverviewPanel.h"
#include "Widgets/SAtlasValidatorPanel.h"
#include "IAtlasRule.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "WorkspaceMenuStructure.h"
#include "WorkspaceMenuStructureModule.h"
#include "Widgets/Docking/SDockTab.h"
#include "Framework/Docking/TabManager.h"

static const FName AtlasOverviewTabName("Atlas.Overview");
static const FName AtlasValidatorTabName("Atlas.Validator");

#define LOCTEXT_NAMESPACE "FAtlasEditorModule"

void FAtlasEditorModule::StartupModule()
{
	// Initialize style
	FAtlasEditorStyle::Initialize();
	FAtlasEditorStyle::ReloadTextures();

	// Register commands
	FAtlasCommands::Register();
	RegisterCommands();

	// Register validation rules
	FAtlasRuleInitializer::RegisterDefaultRules();

	// Initialize Atlas Organizer
	FAtlasOrganizer::Initialize();

	// Register console commands
	FAtlasConsoleCommands::Register();

	// Register UI extensions
	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FAtlasEditorModule::RegisterMenus));

	// Register tab spawners
	RegisterTabSpawners();

	UE_LOG(LogAtlas, Log, TEXT("AtlasEditor module started"));
}

void FAtlasEditorModule::ShutdownModule()
{
	// Shutdown Atlas Organizer
	FAtlasOrganizer::Shutdown();

	// Unregister console commands
	FAtlasConsoleCommands::Unregister();

	// Unregister all the asset types that we registered
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);

	FAtlasEditorStyle::Shutdown();
	FAtlasCommands::Unregister();

	UE_LOG(LogAtlas, Log, TEXT("AtlasEditor module shutdown"));
}

void FAtlasEditorModule::RegisterCommands()
{
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FAtlasCommands::Get().OpenOverview,
		FExecuteAction::CreateRaw(this, &FAtlasEditorModule::OpenOverview),
		FCanExecuteAction());

	PluginCommands->MapAction(
		FAtlasCommands::Get().OpenValidator,
		FExecuteAction::CreateLambda([]()
		{
			FGlobalTabmanager::Get()->TryInvokeTab(AtlasValidatorTabName);
		}),
		FCanExecuteAction());
}

void FAtlasEditorModule::RegisterTabSpawners()
{
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(AtlasOverviewTabName,
		FOnSpawnTab::CreateRaw(this, &FAtlasEditorModule::OnSpawnOverviewTab))
		.SetDisplayName(LOCTEXT("AtlasOverviewTabTitle", "Atlas Overview"))
		.SetMenuType(ETabSpawnerMenuType::Hidden)
		.SetIcon(FSlateIcon(FAtlasEditorStyle::GetStyleSetName(), "Atlas.Icon"));

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(AtlasValidatorTabName,
		FOnSpawnTab::CreateRaw(this, &FAtlasEditorModule::OnSpawnValidatorTab))
		.SetDisplayName(LOCTEXT("AtlasValidatorTabTitle", "Atlas Validator"))
		.SetMenuType(ETabSpawnerMenuType::Hidden)
		.SetIcon(FSlateIcon(FAtlasEditorStyle::GetStyleSetName(), "Atlas.Icon"));
}

void FAtlasEditorModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FAtlasCommands::Get().OpenOverview, PluginCommands);
			Section.AddMenuEntryWithCommandList(FAtlasCommands::Get().OpenValidator, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FAtlasCommands::Get().OpenOverview));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

TSharedRef<SDockTab> FAtlasEditorModule::OnSpawnOverviewTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SAtlasOverviewPanel)
		];
}

TSharedRef<SDockTab> FAtlasEditorModule::OnSpawnValidatorTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SAtlasValidatorPanel)
		];
}

void FAtlasEditorModule::RegisterRule(TSharedRef<IAtlasRule> Rule)
{
	FAtlasRuleRegistry::Get().Register(Rule);
}

void FAtlasEditorModule::RequestScan(const TArray<FAssetData>& OptionalSelection)
{
	// TODO: Implement scan request
	UE_LOG(LogAtlas, Log, TEXT("Scan requested for %d assets"), OptionalSelection.Num());
}

void FAtlasEditorModule::OpenOverview()
{
	FGlobalTabmanager::Get()->TryInvokeTab(AtlasOverviewTabName);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FAtlasEditorModule, AtlasEditor)
