// Copyright Atlas Team. All Rights Reserved.

#include "AtlasCommands.h"

#define LOCTEXT_NAMESPACE "FAtlasEditorModule"

void FAtlasCommands::RegisterCommands()
{
	UI_COMMAND(OpenOverview, "Atlas Overview", "Open the Atlas Overview dashboard", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(OpenValidator, "Atlas Validator", "Open the Atlas Validator panel", EUserInterfaceActionType::Button, FInputChord(EModifierKey::Control | EModifierKey::Alt, EKeys::V));
	UI_COMMAND(ScanAll, "Scan All Assets", "Scan all assets for validation issues", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(ScanSelection, "Scan Selection", "Scan selected assets for validation issues", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(FixAll, "Fix All Issues", "Automatically fix all issues that can be fixed", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(OrganizeSelection, "Organize Selection", "Run Atlas Organizer on selected assets", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
