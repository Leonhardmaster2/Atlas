// Copyright Atlas Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AtlasTypes.h"

class UTexture2D;
class UTexture;

/**
 * Utility functions for texture operations
 */
class FAtlasTextureUtils
{
public:
	/**
	 * Detect texture map type based on filename
	 * @param Name The texture name or filename
	 * @return The detected map type
	 */
	static EAtlasMapType DetectMapTypeByName(const FString& Name);

	/**
	 * Check if a texture is likely a normal map based on its properties
	 * @param Texture The texture to check
	 * @return True if likely a normal map
	 */
	static bool IsLikelyNormal(const UTexture2D* Texture);

	/**
	 * Estimate VRAM usage for a texture
	 * @param Texture The texture to analyze
	 * @return Estimated VRAM bytes
	 */
	static int64 EstimateVRAMBytes(const UTexture* Texture);

	/**
	 * Get the maximum dimension (width or height) of a texture
	 * @param Texture The texture to measure
	 * @return Maximum dimension
	 */
	static int32 GetMaxDimension(const UTexture2D* Texture);

	/**
	 * Check if texture compression settings are appropriate for its type
	 * @param Texture The texture to check
	 * @param MapType The texture's map type
	 * @return True if compression is appropriate
	 */
	static bool HasCorrectCompression(const UTexture2D* Texture, EAtlasMapType MapType);

	/**
	 * Get the recommended compression settings for a texture map type
	 * @param MapType The texture's map type
	 * @return Recommended compression settings enum value
	 */
	static uint8 GetRecommendedCompression(EAtlasMapType MapType);

	/**
	 * Get the pixel format name as a string
	 * @param Texture The texture
	 * @return Pixel format name
	 */
	static FString GetPixelFormatName(const UTexture* Texture);
};
