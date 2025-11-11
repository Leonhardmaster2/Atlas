// Copyright Atlas Team. All Rights Reserved.

#include "AtlasEditorStyle.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define IMAGE_BRUSH(RelativePath, ...) FSlateImageBrush(Style->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define BOX_BRUSH(RelativePath, ...) FSlateBoxBrush(Style->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define BORDER_BRUSH(RelativePath, ...) FSlateBorderBrush(Style->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)

TSharedPtr<FSlateStyleSet> FAtlasEditorStyle::StyleInstance = nullptr;

void FAtlasEditorStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FAtlasEditorStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FAtlasEditorStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("AtlasStyle"));
	return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon40x40(40.0f, 40.0f);
const FVector2D Icon64x64(64.0f, 64.0f);

TSharedRef<FSlateStyleSet> FAtlasEditorStyle::Create()
{
	TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet("AtlasStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("Atlas")->GetBaseDir() / TEXT("Resources"));

	// Define icon styles
	Style->Set("Atlas.Icon", new IMAGE_BRUSH(TEXT("Atlas_64"), Icon40x40));
	Style->Set("Atlas.Icon.Small", new IMAGE_BRUSH(TEXT("Atlas_64"), Icon20x20));

	// Button styles
	Style->Set("Atlas.Button.Fix", FButtonStyle()
		.SetNormal(BOX_BRUSH("Button", FMargin(4.0f / 16.0f)))
		.SetHovered(BOX_BRUSH("Button_Hovered", FMargin(4.0f / 16.0f)))
		.SetPressed(BOX_BRUSH("Button_Pressed", FMargin(4.0f / 16.0f)))
		.SetNormalPadding(FMargin(2, 2, 2, 2))
		.SetPressedPadding(FMargin(2, 3, 2, 1)));

	return Style;
}

void FAtlasEditorStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FAtlasEditorStyle::Get()
{
	return *StyleInstance;
}

#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef BORDER_BRUSH
