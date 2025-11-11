// Copyright Atlas Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "AtlasEditorStyle.h"

/**
 * Atlas command definitions for UI bindings
 */
class FAtlasCommands : public TCommands<FAtlasCommands>
{
public:
	FAtlasCommands()
		: TCommands<FAtlasCommands>(
			TEXT("AtlasEditor"), // Context name for fast lookup
			NSLOCTEXT("Contexts", "AtlasEditor", "Atlas Editor"), // Localized context name for displaying
			NAME_None, // Parent context name
			FAtlasEditorStyle::GetStyleSetName() // Icon Style Set
		)
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr<FUICommandInfo> OpenOverview;
	TSharedPtr<FUICommandInfo> OpenValidator;
	TSharedPtr<FUICommandInfo> ScanAll;
	TSharedPtr<FUICommandInfo> ScanSelection;
	TSharedPtr<FUICommandInfo> FixAll;
	TSharedPtr<FUICommandInfo> OrganizeSelection;
};
