// Copyright Atlas Team. All Rights Reserved.

#include "AtlasTextureUtils.h"
#include "AtlasRuntime.h"
#include "Engine/Texture2D.h"
#include "Engine/Texture.h"

EAtlasMapType FAtlasTextureUtils::DetectMapTypeByName(const FString& Name)
{
	FString LowerName = Name.ToLower();

	// Check for common naming patterns
	if (LowerName.Contains(TEXT("normal")) || LowerName.Contains(TEXT("_n")) || LowerName.EndsWith(TEXT("_n")))
	{
		return EAtlasMapType::Normal;
	}
	else if (LowerName.Contains(TEXT("basecolor")) || LowerName.Contains(TEXT("albedo")) || LowerName.Contains(TEXT("diffuse")) || LowerName.Contains(TEXT("_d")) || LowerName.Contains(TEXT("_bc")))
	{
		return EAtlasMapType::BaseColor;
	}
	else if (LowerName.Contains(TEXT("roughness")) || LowerName.Contains(TEXT("_r")))
	{
		return EAtlasMapType::Roughness;
	}
	else if (LowerName.Contains(TEXT("metallic")) || LowerName.Contains(TEXT("metalness")) || LowerName.Contains(TEXT("_m")))
	{
		return EAtlasMapType::Metalness;
	}
	else if (LowerName.Contains(TEXT("ao")) || LowerName.Contains(TEXT("ambient")) || LowerName.Contains(TEXT("occlusion")))
	{
		return EAtlasMapType::AO;
	}
	else if (LowerName.Contains(TEXT("height")) || LowerName.Contains(TEXT("displacement")) || LowerName.Contains(TEXT("_h")))
	{
		return EAtlasMapType::Height;
	}
	else if (LowerName.Contains(TEXT("emissive")) || LowerName.Contains(TEXT("emission")) || LowerName.Contains(TEXT("_e")))
	{
		return EAtlasMapType::Emissive;
	}
	else if (LowerName.Contains(TEXT("opacity")) || LowerName.Contains(TEXT("alpha")) || LowerName.Contains(TEXT("_o")))
	{
		return EAtlasMapType::Opacity;
	}

	return EAtlasMapType::Unknown;
}

bool FAtlasTextureUtils::IsLikelyNormal(const UTexture2D* Texture)
{
	if (!Texture)
	{
		return false;
	}

	// Check compression settings
	if (Texture->CompressionSettings == TextureCompressionSettings::TC_Normalmap ||
		Texture->CompressionSettings == TextureCompressionSettings::TC_BC5)
	{
		return true;
	}

	// Check if SRGB is disabled (normal maps should not use SRGB)
	if (!Texture->SRGB)
	{
		// Additional heuristic: check name
		EAtlasMapType DetectedType = DetectMapTypeByName(Texture->GetName());
		if (DetectedType == EAtlasMapType::Normal)
		{
			return true;
		}
	}

	return false;
}

int64 FAtlasTextureUtils::EstimateVRAMBytes(const UTexture* Texture)
{
	if (!Texture)
	{
		return 0;
	}

	// This is a rough estimation
	// Actual VRAM usage depends on platform, compression, mipmaps, etc.
	int64 EstimatedSize = Texture->CalcTextureMemorySizeEnum(TMC_ResidentMips);

	return EstimatedSize;
}

int32 FAtlasTextureUtils::GetMaxDimension(const UTexture2D* Texture)
{
	if (!Texture)
	{
		return 0;
	}

	int32 Width = Texture->GetSizeX();
	int32 Height = Texture->GetSizeY();

	return FMath::Max(Width, Height);
}

bool FAtlasTextureUtils::HasCorrectCompression(const UTexture2D* Texture, EAtlasMapType MapType)
{
	if (!Texture)
	{
		return false;
	}

	switch (MapType)
	{
	case EAtlasMapType::Normal:
		return Texture->CompressionSettings == TextureCompressionSettings::TC_Normalmap ||
			   Texture->CompressionSettings == TextureCompressionSettings::TC_BC5;

	case EAtlasMapType::BaseColor:
	case EAtlasMapType::Emissive:
		return Texture->CompressionSettings == TextureCompressionSettings::TC_Default ||
			   Texture->CompressionSettings == TextureCompressionSettings::TC_BC7;

	case EAtlasMapType::Roughness:
	case EAtlasMapType::Metalness:
	case EAtlasMapType::AO:
		// These can use BC4 or default compression
		return true;

	default:
		return true;
	}
}

uint8 FAtlasTextureUtils::GetRecommendedCompression(EAtlasMapType MapType)
{
	switch (MapType)
	{
	case EAtlasMapType::Normal:
		return static_cast<uint8>(TextureCompressionSettings::TC_Normalmap);

	case EAtlasMapType::BaseColor:
	case EAtlasMapType::Emissive:
		return static_cast<uint8>(TextureCompressionSettings::TC_Default);

	case EAtlasMapType::Roughness:
	case EAtlasMapType::Metalness:
	case EAtlasMapType::AO:
	case EAtlasMapType::Height:
		return static_cast<uint8>(TextureCompressionSettings::TC_Grayscale);

	default:
		return static_cast<uint8>(TextureCompressionSettings::TC_Default);
	}
}

FString FAtlasTextureUtils::GetPixelFormatName(const UTexture* Texture)
{
	if (!Texture)
	{
		return TEXT("Unknown");
	}

	EPixelFormat Format = Texture->GetPixelFormat();
	return GetPixelFormatString(Format);
}
