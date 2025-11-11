// Copyright Atlas Team. All Rights Reserved.

#include "AtlasTextureCompressionRule.h"
#include "Core/AtlasTextureUtils.h"
#include "AtlasRuntime.h"
#include "Engine/Texture2D.h"

FName FAtlasTextureCompressionRule::GetRuleName() const
{
	return TEXT("TextureCompression");
}

FString FAtlasTextureCompressionRule::GetDescription() const
{
	return TEXT("Ensures textures use appropriate compression settings (BC5 for normals, BC7 for color, etc.)");
}

bool FAtlasTextureCompressionRule::IsEnabled() const
{
	return true;
}

void FAtlasTextureCompressionRule::Validate(const UObject* Asset, TArray<FAtlasIssue>& OutIssues)
{
	const UTexture2D* Texture = Cast<UTexture2D>(Asset);
	if (!Texture)
	{
		return;
	}

	// Detect texture type
	EAtlasMapType MapType = FAtlasTextureUtils::DetectMapTypeByName(Texture->GetName());
	if (MapType == EAtlasMapType::Unknown)
	{
		// Try to detect if it's a normal map
		if (FAtlasTextureUtils::IsLikelyNormal(Texture))
		{
			MapType = EAtlasMapType::Normal;
		}
		else
		{
			return; // Can't determine type, skip validation
		}
	}

	// Check if compression is correct
	if (!FAtlasTextureUtils::HasCorrectCompression(Texture, MapType))
	{
		uint8 RecommendedCompression = FAtlasTextureUtils::GetRecommendedCompression(MapType);

		FString MapTypeName;
		switch (MapType)
		{
		case EAtlasMapType::Normal: MapTypeName = TEXT("Normal"); break;
		case EAtlasMapType::BaseColor: MapTypeName = TEXT("Base Color"); break;
		case EAtlasMapType::Roughness: MapTypeName = TEXT("Roughness"); break;
		case EAtlasMapType::Metalness: MapTypeName = TEXT("Metallic"); break;
		case EAtlasMapType::AO: MapTypeName = TEXT("AO"); break;
		default: MapTypeName = TEXT("Unknown"); break;
		}

		FString Message = FString::Printf(
			TEXT("Texture '%s' (type: %s) has incorrect compression settings"),
			*Texture->GetName(),
			*MapTypeName
		);

		FAtlasIssue Issue(
			Texture->GetPathName(),
			GetRuleName(),
			EAtlasSeverity::Warning,
			Message,
			true // Can fix
		);

		// Create fix function
		CreateFixFunction(Issue, const_cast<UTexture2D*>(Texture), RecommendedCompression);

		OutIssues.Add(Issue);
	}

	// Check if SRGB is correct for the type
	if (MapType == EAtlasMapType::Normal && Texture->SRGB)
	{
		FString Message = FString::Printf(
			TEXT("Normal map '%s' should not use SRGB"),
			*Texture->GetName()
		);

		FAtlasIssue Issue(
			Texture->GetPathName(),
			GetRuleName(),
			EAtlasSeverity::Error,
			Message,
			true // Can fix
		);

		Issue.FixFunction = [Texture]() -> bool
		{
			UTexture2D* MutableTexture = const_cast<UTexture2D*>(Texture);
			if (MutableTexture)
			{
				MutableTexture->Modify();
				MutableTexture->SRGB = false;
				MutableTexture->PostEditChange();
				MutableTexture->MarkPackageDirty();
				UE_LOG(LogAtlas, Log, TEXT("Disabled SRGB for normal map: %s"), *Texture->GetName());
				return true;
			}
			return false;
		};

		OutIssues.Add(Issue);
	}
}

void FAtlasTextureCompressionRule::CreateFixFunction(FAtlasIssue& Issue, UTexture2D* Texture, uint8 RecommendedCompression)
{
	Issue.FixFunction = [Texture, RecommendedCompression]() -> bool
	{
		if (!Texture)
		{
			return false;
		}

		Texture->Modify();
		Texture->CompressionSettings = static_cast<TextureCompressionSettings>(RecommendedCompression);
		Texture->PostEditChange();
		Texture->MarkPackageDirty();

		UE_LOG(LogAtlas, Log, TEXT("Fixed compression for texture: %s"), *Texture->GetName());
		return true;
	};
}
