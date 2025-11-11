// Copyright Atlas Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "AtlasTypes.generated.h"

/**
 * Severity levels for Atlas issues
 */
UENUM(BlueprintType)
enum class EAtlasSeverity : uint8
{
	Info UMETA(DisplayName = "Info"),
	Warning UMETA(DisplayName = "Warning"),
	Error UMETA(DisplayName = "Error")
};

/**
 * Texture map types for PBR materials
 */
UENUM(BlueprintType)
enum class EAtlasMapType : uint8
{
	BaseColor UMETA(DisplayName = "Base Color"),
	Normal UMETA(DisplayName = "Normal"),
	Roughness UMETA(DisplayName = "Roughness"),
	Metalness UMETA(DisplayName = "Metallic"),
	AO UMETA(DisplayName = "Ambient Occlusion"),
	Height UMETA(DisplayName = "Height"),
	Emissive UMETA(DisplayName = "Emissive"),
	Opacity UMETA(DisplayName = "Opacity"),
	Unknown UMETA(DisplayName = "Unknown")
};

/**
 * Represents a single validation issue found by Atlas
 */
USTRUCT(BlueprintType)
struct FAtlasIssue
{
	GENERATED_BODY()

	/** Path to the asset with the issue */
	UPROPERTY(BlueprintReadOnly, Category = "Atlas")
	FString AssetPath;

	/** Name of the rule that found this issue */
	UPROPERTY(BlueprintReadOnly, Category = "Atlas")
	FName RuleName;

	/** Severity of the issue */
	UPROPERTY(BlueprintReadOnly, Category = "Atlas")
	EAtlasSeverity Severity;

	/** Description of the issue */
	UPROPERTY(BlueprintReadOnly, Category = "Atlas")
	FString Message;

	/** Whether this issue can be auto-fixed */
	UPROPERTY(BlueprintReadOnly, Category = "Atlas")
	bool bCanFix;

	/** Function to call to fix this issue (not serialized) */
	TFunction<bool()> FixFunction;

	FAtlasIssue()
		: Severity(EAtlasSeverity::Info)
		, bCanFix(false)
	{
	}

	FAtlasIssue(const FString& InAssetPath, FName InRuleName, EAtlasSeverity InSeverity, const FString& InMessage, bool bInCanFix = false)
		: AssetPath(InAssetPath)
		, RuleName(InRuleName)
		, Severity(InSeverity)
		, Message(InMessage)
		, bCanFix(bInCanFix)
	{
	}
};

/**
 * Statistics about a texture asset
 */
USTRUCT(BlueprintType)
struct FAtlasTextureStat
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Atlas")
	FString Path;

	UPROPERTY(BlueprintReadOnly, Category = "Atlas")
	int64 VRAMBytes;

	UPROPERTY(BlueprintReadOnly, Category = "Atlas")
	int32 MaxDimension;

	UPROPERTY(BlueprintReadOnly, Category = "Atlas")
	FString PixelFormat;

	FAtlasTextureStat()
		: VRAMBytes(0)
		, MaxDimension(0)
	{
	}
};

/**
 * Statistics about a mesh asset
 */
USTRUCT(BlueprintType)
struct FAtlasMeshStat
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Atlas")
	FString Path;

	UPROPERTY(BlueprintReadOnly, Category = "Atlas")
	int64 TriangleCount;

	UPROPERTY(BlueprintReadOnly, Category = "Atlas")
	int32 NumLODs;

	FAtlasMeshStat()
		: TriangleCount(0)
		, NumLODs(0)
	{
	}
};

/**
 * Statistics about a level asset
 */
USTRUCT(BlueprintType)
struct FAtlasLevelStat
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Atlas")
	FString Path;

	UPROPERTY(BlueprintReadOnly, Category = "Atlas")
	int64 TotalBytes;

	FAtlasLevelStat()
		: TotalBytes(0)
	{
	}
};

/**
 * Complete Atlas report for export
 */
USTRUCT(BlueprintType)
struct FAtlasReport
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Atlas")
	TArray<FAtlasIssue> Issues;

	UPROPERTY(BlueprintReadOnly, Category = "Atlas")
	TArray<FAtlasTextureStat> Textures;

	UPROPERTY(BlueprintReadOnly, Category = "Atlas")
	TArray<FAtlasMeshStat> Meshes;

	UPROPERTY(BlueprintReadOnly, Category = "Atlas")
	TArray<FAtlasLevelStat> Levels;

	UPROPERTY(BlueprintReadOnly, Category = "Atlas")
	int64 TotalProjectBytes;

	UPROPERTY(BlueprintReadOnly, Category = "Atlas")
	int64 TotalVRAMBytes;

	FAtlasReport()
		: TotalProjectBytes(0)
		, TotalVRAMBytes(0)
	{
	}
};

/**
 * Result from the Organizer prompt dialog
 */
USTRUCT()
struct FAtlasOrganizerResult
{
	GENERATED_BODY()

	UPROPERTY()
	bool bAccepted;

	UPROPERTY()
	FString BaseName;

	UPROPERTY()
	bool bRememberPattern;

	FAtlasOrganizerResult()
		: bAccepted(false)
		, bRememberPattern(false)
	{
	}
};
