// Copyright Atlas Team. All Rights Reserved.

#include "AtlasNamingConventionRule.h"
#include "AtlasSettings.h"
#include "Core/AtlasAssetUtils.h"
#include "AtlasRuntime.h"
#include "Engine/StaticMesh.h"
#include "Engine/Texture2D.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstance.h"

FName FAtlasNamingConventionRule::GetRuleName() const
{
	return TEXT("NamingConvention");
}

FString FAtlasNamingConventionRule::GetDescription() const
{
	return TEXT("Ensures assets follow the naming conventions defined in Atlas settings");
}

bool FAtlasNamingConventionRule::IsEnabled() const
{
	return true;
}

void FAtlasNamingConventionRule::Validate(const UObject* Asset, TArray<FAtlasIssue>& OutIssues)
{
	if (!Asset)
	{
		return;
	}

	// Check prefix
	FString ExpectedPrefix;
	if (!HasCorrectPrefix(Asset, ExpectedPrefix))
	{
		FString AssetName = Asset->GetName();
		FString Message = FString::Printf(TEXT("Asset '%s' should start with prefix '%s'"), *AssetName, *ExpectedPrefix);

		FAtlasIssue Issue(
			Asset->GetPathName(),
			GetRuleName(),
			EAtlasSeverity::Warning,
			Message,
			true // Can fix
		);

		// Create fix function
		FString ExpectedName = ExpectedPrefix + AssetName;
		CreateFixFunction(Issue, Asset, ExpectedName);

		OutIssues.Add(Issue);
	}

	// Check path
	FString ExpectedPath;
	if (!HasCorrectPath(Asset, ExpectedPath))
	{
		FString CurrentPath = Asset->GetPathName();
		FString Message = FString::Printf(TEXT("Asset should be in '%s' folder"), *ExpectedPath);

		FAtlasIssue Issue(
			CurrentPath,
			GetRuleName(),
			EAtlasSeverity::Warning,
			Message,
			true // Can fix
		);

		OutIssues.Add(Issue);
	}
}

bool FAtlasNamingConventionRule::HasCorrectPrefix(const UObject* Asset, FString& OutExpectedPrefix) const
{
	if (!Asset)
	{
		return true;
	}

	FName ClassName = Asset->GetClass()->GetFName();
	OutExpectedPrefix = FAtlasAssetUtils::GetPrefixForAssetType(ClassName);

	if (OutExpectedPrefix.IsEmpty())
	{
		return true; // No prefix requirement
	}

	FString AssetName = Asset->GetName();
	return AssetName.StartsWith(OutExpectedPrefix);
}

bool FAtlasNamingConventionRule::HasCorrectPath(const UObject* Asset, FString& OutExpectedPath) const
{
	if (!Asset)
	{
		return true;
	}

	const UAtlasSettings* Settings = UAtlasSettings::Get();
	if (!Settings)
	{
		return true;
	}

	FString CurrentPath = Asset->GetPathName();

	// Determine expected path based on asset type
	if (Asset->IsA<UStaticMesh>())
	{
		OutExpectedPath = Settings->MeshRoot.Path;
	}
	else if (Asset->IsA<UTexture2D>())
	{
		OutExpectedPath = Settings->TextureRoot.Path;
	}
	else if (Asset->IsA<UMaterial>() || Asset->IsA<UMaterialInstance>())
	{
		OutExpectedPath = Settings->MaterialRoot.Path;
	}
	else
	{
		return true; // No path requirement for this type
	}

	return CurrentPath.StartsWith(OutExpectedPath);
}

void FAtlasNamingConventionRule::CreateFixFunction(FAtlasIssue& Issue, const UObject* Asset, const FString& ExpectedName)
{
	Issue.FixFunction = [Asset, ExpectedName]() -> bool
	{
		if (!Asset)
		{
			return false;
		}

		FName AssetPath = FName(*Asset->GetPathName());
		bool bSuccess = FAtlasAssetUtils::RenameAsset(AssetPath, ExpectedName);

		if (bSuccess)
		{
			UE_LOG(LogAtlas, Log, TEXT("Fixed naming: %s -> %s"), *Asset->GetName(), *ExpectedName);
		}

		return bSuccess;
	};
}
