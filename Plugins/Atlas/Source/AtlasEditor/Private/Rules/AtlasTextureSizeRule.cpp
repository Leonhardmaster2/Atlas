// Copyright Atlas Team. All Rights Reserved.

#include "AtlasTextureSizeRule.h"
#include "AtlasSettings.h"
#include "Core/AtlasTextureUtils.h"
#include "AtlasRuntime.h"
#include "Engine/Texture2D.h"

FName FAtlasTextureSizeRule::GetRuleName() const
{
	return TEXT("TextureSize");
}

FString FAtlasTextureSizeRule::GetDescription() const
{
	return TEXT("Ensures textures don't exceed maximum dimension limits");
}

bool FAtlasTextureSizeRule::IsEnabled() const
{
	return true;
}

void FAtlasTextureSizeRule::Validate(const UObject* Asset, TArray<FAtlasIssue>& OutIssues)
{
	const UTexture2D* Texture = Cast<UTexture2D>(Asset);
	if (!Texture)
	{
		return;
	}

	const UAtlasSettings* Settings = UAtlasSettings::Get();
	if (!Settings)
	{
		return;
	}

	int32 MaxDim = FAtlasTextureUtils::GetMaxDimension(Texture);
	int32 MaxAllowed = Settings->MaxTextureSize;

	if (MaxDim > MaxAllowed)
	{
		FString Message = FString::Printf(
			TEXT("Texture '%s' dimension (%dx%d) exceeds maximum allowed (%d)"),
			*Texture->GetName(),
			Texture->GetSizeX(),
			Texture->GetSizeY(),
			MaxAllowed
		);

		FAtlasIssue Issue(
			Texture->GetPathName(),
			GetRuleName(),
			EAtlasSeverity::Warning,
			Message,
			true // Can fix
		);

		CreateFixFunction(Issue, const_cast<UTexture2D*>(Texture), MaxAllowed);
		OutIssues.Add(Issue);
	}
}

void FAtlasTextureSizeRule::CreateFixFunction(FAtlasIssue& Issue, UTexture2D* Texture, int32 MaxSize)
{
	Issue.FixFunction = [Texture, MaxSize]() -> bool
	{
		if (!Texture)
		{
			return false;
		}

		Texture->Modify();
		Texture->MaxTextureSize = MaxSize;
		Texture->PostEditChange();
		Texture->MarkPackageDirty();

		UE_LOG(LogAtlas, Log, TEXT("Set max texture size for '%s' to %d"), *Texture->GetName(), MaxSize);
		return true;
	};
}
