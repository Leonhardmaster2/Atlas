// Copyright Atlas Team. All Rights Reserved.

using UnrealBuildTool;

public class AtlasEditor : ModuleRules
{
	public AtlasEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
		);

		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
		);

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"AtlasRuntime",
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"ToolMenus",
				"UnrealEd",
				"AssetRegistry",
				"Projects",
				"ContentBrowser",
				"EditorSubsystem",
				"Json",
				"JsonUtilities",
				"DeveloperSettings",
				"RHI",
				"RenderCore",
				"InputCore",
				"EditorStyle",
				"PropertyEditor",
				"WorkspaceMenuStructure",
			}
		);

		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
		);
	}
}
