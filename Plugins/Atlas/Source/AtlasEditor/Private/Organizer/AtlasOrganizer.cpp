// Copyright Atlas Team. All Rights Reserved.

#include "AtlasOrganizer.h"
#include "AtlasSettings.h"
#include "Core/AtlasAssetUtils.h"
#include "Core/AtlasTextureUtils.h"
#include "AtlasRuntime.h"
#include "EditorFramework/AssetImportData.h"
#include "Factories/Factory.h"
#include "Engine/Texture2D.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialExpressionTextureSample.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Misc/MessageDialog.h"

FDelegateHandle FAtlasOrganizer::PostImportDelegateHandle;
TArray<UObject*> FAtlasOrganizer::PendingImports;

void FAtlasOrganizer::Initialize()
{
	// Hook into the post-import delegate
	PostImportDelegateHandle = FEditorDelegates::OnAssetPostImport.AddStatic(&FAtlasOrganizer::OnPostImport);
	UE_LOG(LogAtlas, Log, TEXT("Atlas Organizer initialized"));
}

void FAtlasOrganizer::Shutdown()
{
	// Unhook from the post-import delegate
	if (PostImportDelegateHandle.IsValid())
	{
		FEditorDelegates::OnAssetPostImport.Remove(PostImportDelegateHandle);
		PostImportDelegateHandle.Reset();
	}
	PendingImports.Empty();
	UE_LOG(LogAtlas, Log, TEXT("Atlas Organizer shutdown"));
}

void FAtlasOrganizer::OnPostImport(UFactory* Factory, UObject* Object)
{
	if (!Object)
	{
		return;
	}

	const UAtlasSettings* Settings = UAtlasSettings::Get();
	if (!Settings || !Settings->bAutoOrganizeOnImport)
	{
		return; // Auto-organization is disabled
	}

	// Add to pending imports
	PendingImports.AddUnique(Object);

	UE_LOG(LogAtlas, Verbose, TEXT("Asset imported: %s"), *Object->GetName());

	// TODO: Implement batching logic to group related imports
	// For now, process immediately
	if (PendingImports.Num() > 0)
	{
		TMap<FString, TArray<UObject*>> Groups = GroupAssetsByBaseName(PendingImports);

		for (const auto& Pair : Groups)
		{
			NormalizeGroup(Pair.Value);
		}

		PendingImports.Empty();
	}
}

void FAtlasOrganizer::NormalizeGroup(const TArray<UObject*>& Group)
{
	if (Group.Num() == 0)
	{
		return;
	}

	// Get the first asset to determine base name
	FString DetectedName = Group[0]->GetName();
	FString SuggestedName = SuggestBaseName(DetectedName);

	// Show prompt to user
	FAtlasOrganizerResult Result = ShowOrganizerPrompt(DetectedName, SuggestedName);

	if (!Result.bAccepted)
	{
		return; // User cancelled
	}

	FString BaseName = Result.BaseName;

	// Compute destination paths
	FString MeshPath, MatPath, TexPath;
	ComputeDestPaths(BaseName, MeshPath, MatPath, TexPath);

	// Organize assets by type
	TArray<UTexture2D*> Textures;
	TArray<UStaticMesh*> Meshes;
	TArray<UMaterial*> Materials;

	for (UObject* Asset : Group)
	{
		if (UTexture2D* Texture = Cast<UTexture2D>(Asset))
		{
			Textures.Add(Texture);
		}
		else if (UStaticMesh* Mesh = Cast<UStaticMesh>(Asset))
		{
			Meshes.Add(Mesh);
		}
		else if (UMaterial* Material = Cast<UMaterial>(Asset))
		{
			Materials.Add(Material);
		}
	}

	// Process textures
	TMap<EAtlasMapType, UTexture*> TexByType;
	for (UTexture2D* Texture : Textures)
	{
		EAtlasMapType MapType = FAtlasTextureUtils::DetectMapTypeByName(Texture->GetName());
		TexByType.Add(MapType, Texture);

		// Apply naming and move to texture folder
		ApplyNaming(Texture, BaseName);
		FString NewPath = TexPath / Texture->GetName();
		FAtlasAssetUtils::MoveAsset(FName(*Texture->GetPathName()), NewPath);
	}

	// Process meshes
	for (UStaticMesh* Mesh : Meshes)
	{
		ApplyNaming(Mesh, BaseName);
		FString NewPath = MeshPath / Mesh->GetName();
		FAtlasAssetUtils::MoveAsset(FName(*Mesh->GetPathName()), NewPath);
	}

	// Create or update material
	if (Materials.Num() == 0 && TexByType.Num() > 0)
	{
		// TODO: Create material from template
		UE_LOG(LogAtlas, Log, TEXT("Would create material for %s with %d textures"), *BaseName, TexByType.Num());
	}
	else if (Materials.Num() > 0)
	{
		// Relink existing material
		for (UMaterial* Material : Materials)
		{
			RelinkMaterialGraph(Material, TexByType);
			ApplyNaming(Material, BaseName);
			FString NewPath = MatPath / Material->GetName();
			FAtlasAssetUtils::MoveAsset(FName(*Material->GetPathName()), NewPath);
		}
	}

	UE_LOG(LogAtlas, Log, TEXT("Normalized asset group '%s': %d textures, %d meshes, %d materials"),
		*BaseName, Textures.Num(), Meshes.Num(), Materials.Num());
}

FString FAtlasOrganizer::SuggestBaseName(const FString& RawName)
{
	FString BaseName = RawName;

	// Remove common suffixes
	TArray<FString> SuffixesToRemove = {
		TEXT("_BaseColor"), TEXT("_Normal"), TEXT("_Roughness"), TEXT("_Metallic"),
		TEXT("_AO"), TEXT("_Height"), TEXT("_Emissive"),
		TEXT("_BC"), TEXT("_N"), TEXT("_R"), TEXT("_M"),
		TEXT("_diffuse"), TEXT("_normal"), TEXT("_spec"),
		TEXT("_01"), TEXT("_02"), TEXT("_03"),
		TEXT("_low"), TEXT("_high"),
		TEXT("_LOD0"), TEXT("_LOD1"),
		TEXT(".001"), TEXT(".002"),
	};

	for (const FString& Suffix : SuffixesToRemove)
	{
		if (BaseName.EndsWith(Suffix, ESearchCase::IgnoreCase))
		{
			BaseName = BaseName.LeftChop(Suffix.Len());
		}
	}

	// Remove common prefixes
	TArray<FString> PrefixesToRemove = {
		TEXT("T_"), TEXT("M_"), TEXT("SM_"), TEXT("MI_"),
		TEXT("Tex_"), TEXT("Mat_"), TEXT("Mesh_"),
	};

	for (const FString& Prefix : PrefixesToRemove)
	{
		if (BaseName.StartsWith(Prefix, ESearchCase::IgnoreCase))
		{
			BaseName = BaseName.RightChop(Prefix.Len());
		}
	}

	// Clean up the name
	BaseName = BaseName.TrimStartAndEnd();
	BaseName.ReplaceInline(TEXT("  "), TEXT(" "));
	BaseName.ReplaceInline(TEXT(" "), TEXT("_"));

	return BaseName;
}

void FAtlasOrganizer::ComputeDestPaths(const FString& BaseName, FString& OutMeshPath, FString& OutMatPath, FString& OutTexPath)
{
	const UAtlasSettings* Settings = UAtlasSettings::Get();
	if (Settings)
	{
		OutMeshPath = Settings->MeshRoot.Path;
		OutMatPath = Settings->MaterialRoot.Path;
		OutTexPath = Settings->TextureRoot.Path;
	}
	else
	{
		OutMeshPath = TEXT("/Game/Meshes");
		OutMatPath = TEXT("/Game/Materials");
		OutTexPath = TEXT("/Game/Textures");
	}
}

void FAtlasOrganizer::ApplyNaming(UObject* Asset, const FString& BaseName)
{
	if (!Asset)
	{
		return;
	}

	FString Prefix = FAtlasAssetUtils::GetPrefixForAssetType(Asset->GetClass()->GetFName());
	FString NewName = Prefix + BaseName;

	// For textures, append the map type
	if (UTexture2D* Texture = Cast<UTexture2D>(Asset))
	{
		EAtlasMapType MapType = FAtlasTextureUtils::DetectMapTypeByName(Texture->GetName());
		FString MapTypeSuffix;

		switch (MapType)
		{
		case EAtlasMapType::BaseColor: MapTypeSuffix = TEXT("_BC"); break;
		case EAtlasMapType::Normal: MapTypeSuffix = TEXT("_N"); break;
		case EAtlasMapType::Roughness: MapTypeSuffix = TEXT("_R"); break;
		case EAtlasMapType::Metalness: MapTypeSuffix = TEXT("_M"); break;
		case EAtlasMapType::AO: MapTypeSuffix = TEXT("_AO"); break;
		case EAtlasMapType::Height: MapTypeSuffix = TEXT("_H"); break;
		case EAtlasMapType::Emissive: MapTypeSuffix = TEXT("_E"); break;
		case EAtlasMapType::Opacity: MapTypeSuffix = TEXT("_O"); break;
		default: MapTypeSuffix = TEXT(""); break;
		}

		NewName += MapTypeSuffix;
	}

	if (NewName != Asset->GetName())
	{
		FAtlasAssetUtils::RenameAsset(FName(*Asset->GetPathName()), NewName);
	}
}

void FAtlasOrganizer::RelinkMaterialGraph(UMaterial* Material, const TMap<EAtlasMapType, UTexture*>& TexByType)
{
	if (!Material)
	{
		return;
	}

	// This is a simplified implementation
	// In a full implementation, you'd use MaterialEditingLibrary or create expression nodes

	Material->Modify();

	// Clear existing texture sample expressions (simplified approach)
	// In production, you'd want to be more selective

	UE_LOG(LogAtlas, Log, TEXT("Relinking material '%s' with %d textures"), *Material->GetName(), TexByType.Num());

	// TODO: Implement full material graph relinking using UMaterialEditingLibrary
	// For now, just log what would be done
	for (const auto& Pair : TexByType)
	{
		const TCHAR* MapName = TEXT("Unknown");
		switch (Pair.Key)
		{
		case EAtlasMapType::BaseColor: MapName = TEXT("BaseColor"); break;
		case EAtlasMapType::Normal: MapName = TEXT("Normal"); break;
		case EAtlasMapType::Roughness: MapName = TEXT("Roughness"); break;
		case EAtlasMapType::Metalness: MapName = TEXT("Metalness"); break;
		case EAtlasMapType::AO: MapName = TEXT("AO"); break;
		}

		UE_LOG(LogAtlas, Verbose, TEXT("  Would link %s texture: %s"), MapName, *Pair.Value->GetName());
	}

	Material->PostEditChange();
}

TMap<FString, TArray<UObject*>> FAtlasOrganizer::GroupAssetsByBaseName(const TArray<UObject*>& ImportedAssets)
{
	TMap<FString, TArray<UObject*>> Groups;

	for (UObject* Asset : ImportedAssets)
	{
		if (!Asset)
		{
			continue;
		}

		FString BaseName = SuggestBaseName(Asset->GetName());
		if (!Groups.Contains(BaseName))
		{
			Groups.Add(BaseName, TArray<UObject*>());
		}
		Groups[BaseName].Add(Asset);
	}

	return Groups;
}

FAtlasOrganizerResult FAtlasOrganizer::ShowOrganizerPrompt(const FString& DetectedName, const FString& SuggestedName)
{
	FAtlasOrganizerResult Result;
	Result.bAccepted = false;
	Result.BaseName = SuggestedName;

	// For now, use a simple message dialog
	// TODO: Implement custom Slate dialog SAtlasOrganizerPrompt

	FText Title = FText::FromString(TEXT("Atlas Organizer"));
	FText Message = FText::FromString(FString::Printf(
		TEXT("Organize imported assets?\n\nDetected: %s\nSuggested: %s\n\nProceed with organization?"),
		*DetectedName, *SuggestedName
	));

	EAppReturnType::Type Response = FMessageDialog::Open(EAppMsgType::YesNo, Message, &Title);
	Result.bAccepted = (Response == EAppReturnType::Yes);

	return Result;
}
