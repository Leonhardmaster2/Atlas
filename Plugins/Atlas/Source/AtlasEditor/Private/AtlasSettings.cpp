// Copyright Atlas Team. All Rights Reserved.

#include "AtlasSettings.h"

#define LOCTEXT_NAMESPACE "AtlasSettings"

UAtlasSettings::UAtlasSettings()
{
	// Set default paths
	MeshRoot.Path = TEXT("/Game/Meshes");
	MaterialRoot.Path = TEXT("/Game/Materials");
	TextureRoot.Path = TEXT("/Game/Textures");

	// Set default naming prefixes
	NamingPrefixes.Add(TEXT("StaticMesh"), TEXT("SM_"));
	NamingPrefixes.Add(TEXT("SkeletalMesh"), TEXT("SK_"));
	NamingPrefixes.Add(TEXT("Material"), TEXT("M_"));
	NamingPrefixes.Add(TEXT("MaterialInstance"), TEXT("MI_"));
	NamingPrefixes.Add(TEXT("Texture"), TEXT("T_"));
	NamingPrefixes.Add(TEXT("Texture2D"), TEXT("T_"));
	NamingPrefixes.Add(TEXT("TextureCube"), TEXT("TC_"));
	NamingPrefixes.Add(TEXT("Blueprint"), TEXT("BP_"));

	// Set default thresholds
	MaxTextureSize = 2048;
	MaxTriCount = 25000;
	MaxProjectSizeMB = 81920; // 80 GB
	TargetVRAMMB = 2560; // 2.5 GB

	// Set default behavior
	bBlockBuildOnRed = true;
	bAutoOrganizeOnImport = false;
	bAutoFixOnScan = false;
	bShowInfoMessages = true;
}

const UAtlasSettings* UAtlasSettings::Get()
{
	return GetDefault<UAtlasSettings>();
}

FName UAtlasSettings::GetCategoryName() const
{
	return TEXT("Plugins");
}

FText UAtlasSettings::GetSectionText() const
{
	return LOCTEXT("AtlasSettingsSection", "Atlas");
}

#undef LOCTEXT_NAMESPACE
