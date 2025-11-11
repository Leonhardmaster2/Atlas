// Copyright Atlas Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "AtlasSettings.generated.h"

/**
 * Atlas plugin settings
 * Configure paths, naming conventions, and thresholds for asset management
 */
UCLASS(Config=Editor, DefaultConfig, meta=(DisplayName="Atlas"))
class ATLASEDITOR_API UAtlasSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UAtlasSettings();

	/** Get the Atlas settings singleton */
	static const UAtlasSettings* Get();

	//~ Begin UDeveloperSettings Interface
	virtual FName GetCategoryName() const override;
	virtual FText GetSectionText() const override;
	//~ End UDeveloperSettings Interface

public:
	/** Root directory for mesh assets */
	UPROPERTY(Config, EditAnywhere, Category = "Paths", meta = (RelativeToGameContentDir))
	FDirectoryPath MeshRoot;

	/** Root directory for material assets */
	UPROPERTY(Config, EditAnywhere, Category = "Paths", meta = (RelativeToGameContentDir))
	FDirectoryPath MaterialRoot;

	/** Root directory for texture assets */
	UPROPERTY(Config, EditAnywhere, Category = "Paths", meta = (RelativeToGameContentDir))
	FDirectoryPath TextureRoot;

	/** Naming prefixes for different asset types */
	UPROPERTY(Config, EditAnywhere, Category = "Naming Conventions")
	TMap<FName, FString> NamingPrefixes;

	/** Maximum texture dimension (width or height) */
	UPROPERTY(Config, EditAnywhere, Category = "Thresholds", meta = (ClampMin = "128", ClampMax = "8192"))
	int32 MaxTextureSize;

	/** Maximum triangle count for meshes before requiring LODs */
	UPROPERTY(Config, EditAnywhere, Category = "Thresholds", meta = (ClampMin = "1000", ClampMax = "100000"))
	int32 MaxTriCount;

	/** Maximum project size in megabytes */
	UPROPERTY(Config, EditAnywhere, Category = "Thresholds", meta = (ClampMin = "1024"))
	int64 MaxProjectSizeMB;

	/** Target VRAM budget in megabytes */
	UPROPERTY(Config, EditAnywhere, Category = "Thresholds", meta = (ClampMin = "512"))
	int64 TargetVRAMMB;

	/** Block builds when red (error) violations are present */
	UPROPERTY(Config, EditAnywhere, Category = "Validation")
	bool bBlockBuildOnRed;

	/** Enable automatic organization on asset import */
	UPROPERTY(Config, EditAnywhere, Category = "Organizer")
	bool bAutoOrganizeOnImport;

	/** Automatically fix issues when possible */
	UPROPERTY(Config, EditAnywhere, Category = "Validator")
	bool bAutoFixOnScan;

	/** Show detailed messages for info-level issues */
	UPROPERTY(Config, EditAnywhere, Category = "Validator")
	bool bShowInfoMessages;
};
