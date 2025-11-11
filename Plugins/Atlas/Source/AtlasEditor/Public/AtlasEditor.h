// Copyright Atlas Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class IAtlasRule;
struct FAtlasIssue;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAtlasIssuesReady, const TArray<FAtlasIssue>&);

/**
 * Public interface for the Atlas Editor module
 * Allows other plugins to register custom rules and interact with Atlas
 */
class IAtlasEditorModule : public IModuleInterface
{
public:
	/**
	 * Singleton-like access to this module's interface.
	 * Beware of calling this during the shutdown phase, though. Your module might have been unloaded already.
	 *
	 * @return Returns singleton instance, loading the module on demand if needed
	 */
	static inline IAtlasEditorModule& Get()
	{
		return FModuleManager::LoadModuleChecked<IAtlasEditorModule>("AtlasEditor");
	}

	/**
	 * Checks to see if this module is loaded and ready.
	 *
	 * @return True if the module is loaded and ready to use
	 */
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("AtlasEditor");
	}

	/** Register a custom validation rule */
	virtual void RegisterRule(TSharedRef<IAtlasRule> Rule) = 0;

	/** Request a validation scan */
	virtual void RequestScan(const TArray<struct FAssetData>& OptionalSelection) = 0;

	/** Open the Overview dashboard */
	virtual void OpenOverview() = 0;

	/** Event fired when validation scan completes */
	virtual FOnAtlasIssuesReady& OnIssuesReady() = 0;
};

/**
 * The Atlas Editor module implementation
 */
class FAtlasEditorModule : public IAtlasEditorModule
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/** IAtlasEditorModule implementation */
	virtual void RegisterRule(TSharedRef<IAtlasRule> Rule) override;
	virtual void RequestScan(const TArray<struct FAssetData>& OptionalSelection) override;
	virtual void OpenOverview() override;
	virtual FOnAtlasIssuesReady& OnIssuesReady() override { return OnIssuesReadyDelegate; }

private:
	void RegisterMenus();
	void RegisterCommands();
	void RegisterTabSpawners();

	TSharedRef<class SDockTab> OnSpawnOverviewTab(const class FSpawnTabArgs& SpawnTabArgs);
	TSharedRef<class SDockTab> OnSpawnValidatorTab(const class FSpawnTabArgs& SpawnTabArgs);

private:
	FOnAtlasIssuesReady OnIssuesReadyDelegate;
	TSharedPtr<class FUICommandList> PluginCommands;
};
