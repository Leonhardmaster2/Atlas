# Atlas - Unreal Engine Content Governance Plugin

Atlas is a comprehensive content governance and automation toolkit for Unreal Engine that standardizes, cleans, verifies, and monitors all project assets to prevent pipeline chaos and performance waste.

## Overview

Atlas consists of three integrated systems:

### 1. Atlas Organizer
Automatically restructures imported asset packs (meshes, materials, textures) into a clean, consistent project layout. It detects the base asset name, renames files to studio naming conventions, moves them into the correct folders, and builds/links a proper PBR material setup.

### 2. Atlas Validator
Continuously enforces project rules. It checks all assets for:
- Correct naming conventions
- Texture compression settings
- Maximum texture sizes
- Mesh LOD presence
- Collision settings
- Material duplication

The Validator can auto-fix many issues with one click.

### 3. Atlas Overview
A dashboard that measures and visualizes total project cost:
- VRAM usage by textures
- Triangle counts and LOD quality
- Project size growth and asset footprint
- Memory/complexity thresholds with warnings and build-blocking options

## Current Implementation Status

### ✅ Completed Components

#### Core Infrastructure
- **Plugin Structure**: Complete `.uplugin` file with AtlasEditor and AtlasRuntime modules
- **Build Configuration**: Fully configured Build.cs files with all necessary dependencies
- **Module System**: Both Editor and Runtime modules with proper initialization
- **Settings System**: UAtlasSettings developer settings with configurable paths, naming conventions, and thresholds
- **Type System**: Complete type definitions (EAtlasSeverity, EAtlasMapType, FAtlasIssue, etc.)

#### Validation System
- **Rule Interface**: IAtlasRule abstract interface for creating validation rules
- **Rule Registry**: Central registry for managing and accessing all validation rules
- **Rule Initializer**: Automatic registration system for built-in rules
- **Scan Executor**: Comprehensive scanning system that can scan all assets or selections

#### Utility Classes
- **AtlasAssetUtils**: Asset operations (move, rename, reimport, sync to Content Browser)
- **AtlasTextureUtils**: Texture analysis (map type detection, VRAM estimation, compression validation)
- **AtlasMeshUtils**: Mesh operations (triangle counting, collision checking/generation, LOD management)

#### Validation Rules (5 Implemented)
1. **Naming Convention Rule**: Validates asset naming prefixes and paths
2. **Texture Compression Rule**: Ensures textures use appropriate compression (BC5 for normals, BC7 for color, etc.)
3. **Texture Size Rule**: Validates textures don't exceed maximum dimensions
4. **Static Mesh Collision Rule**: Verifies meshes have collision geometry
5. **Static Mesh LOD Rule**: Ensures high-poly meshes have appropriate LODs

All rules support auto-fix functionality.

#### UI Framework
- **Style System**: FAtlasEditorStyle with icon and button style definitions
- **Command System**: FAtlasCommands with keyboard shortcuts (Ctrl+Alt+V for Validator)
- **Tab System**: Registered tab spawners for Overview and Validator panels
- **Menu Integration**: Atlas menus in Window menu and toolbar

#### Configuration
- **DefaultEditor.ini**: Pre-configured with sensible defaults for all settings

#### Atlas Organizer System
- **FAtlasOrganizer**: Automatic asset organization on import
- **Import Hook**: Integrates with `FEditorDelegates::OnAssetPostImport`
- **Asset Grouping**: Groups related assets (meshes, textures, materials) by base name
- **Auto-Naming**: Applies naming conventions automatically
- **Folder Organization**: Moves assets to appropriate folders
- **Material Relinking**: Links textures to materials based on PBR map types

#### Atlas Validator UI
- **SAtlasValidatorPanel**: Full Slate UI panel for validation
- **Issue List**: Displays all found issues with severity, rule, asset, and message
- **Interactive Fixing**: Click "Fix" button on individual issues or fix all at once
- **Filtering**: Filter by severity (Info, Warning, Error)
- **Content Browser Sync**: Double-click an issue to sync to Content Browser

#### Atlas Overview Dashboard
- **SAtlasOverviewPanel**: Complete dashboard with project statistics
- **VRAM Tracking**: Shows current VRAM usage vs target with color-coded status
- **Project Size Tracking**: Displays project size vs maximum allowed
- **Top Textures List**: Shows top 20 textures by VRAM usage
- **Top Meshes List**: Shows top 20 meshes by triangle count
- **FAtlasOverviewStats**: Comprehensive stats collection system

#### Commandlets & Automation
- **UAtlasValidateCommandlet**: CI/CD integration for automated validation
- **Console Commands**:
  - `Atlas.ScanAll` - Scan all assets
  - `Atlas.ValidateSelection` - Validate selected assets
  - `Atlas.OrganizeSelection` - Organize selected assets
  - `Atlas.ExportReport` - Export validation report

### 🚧 Optional/Future Components

The following are nice-to-have features for future enhancement:
- Content Browser context menu integration
- Telemetry and timeline tracking
- Material graph visual automation
- Additional validation rules (material duplication detection, audio normalization)

## Project Structure

```
Plugins/Atlas/
├── Atlas.uplugin                    # Plugin definition
├── Config/
│   └── DefaultEditor.ini           # Default settings
├── Resources/                       # Icons and style assets
│   ├── Atlas_64.png
│   ├── Button.png
│   ├── Button_Hovered.png
│   └── Button_Pressed.png
└── Source/
    ├── AtlasEditor/                # Editor module
    │   ├── AtlasEditor.Build.cs
    │   ├── Public/
    │   │   ├── AtlasEditor.h       # Module interface
    │   │   ├── AtlasSettings.h     # Settings class
    │   │   ├── AtlasTypes.h        # Core types and enums
    │   │   └── IAtlasRule.h        # Rule interface
    │   └── Private/
    │       ├── AtlasEditor.cpp
    │       ├── AtlasSettings.cpp
    │       ├── AtlasEditorStyle.h/cpp
    │       ├── AtlasCommands.h/cpp
    │       ├── Core/
    │       │   ├── AtlasRuleRegistry.h/cpp
    │       │   ├── AtlasRuleInitializer.h/cpp
    │       │   ├── AtlasScanExecutor.h/cpp
    │       │   ├── AtlasAssetUtils.h/cpp
    │       │   ├── AtlasTextureUtils.h/cpp
    │       │   └── AtlasMeshUtils.h/cpp
    │       └── Rules/
    │           ├── AtlasNamingConventionRule.h/cpp
    │           ├── AtlasTextureCompressionRule.h/cpp
    │           └── AtlasStaticMeshCollisionRule.h/cpp
    └── AtlasRuntime/               # Runtime module
        ├── AtlasRuntime.Build.cs
        ├── Public/
        │   └── AtlasRuntime.h
        └── Private/
            └── AtlasRuntime.cpp
```

## Configuration

Atlas settings can be configured in the Editor under **Edit → Project Settings → Plugins → Atlas**.

### Default Settings

```ini
MeshRoot=/Game/Meshes
MaterialRoot=/Game/Materials
TextureRoot=/Game/Textures

MaxTextureSize=2048
MaxTriCount=25000
MaxProjectSizeMB=81920  # 80 GB
TargetVRAMMB=2560       # 2.5 GB

bBlockBuildOnRed=true   # Fail builds on error-level violations
```

## Usage

### Opening Atlas Panels

1. **Atlas Overview**: Window → Atlas Overview
2. **Atlas Validator**: Window → Atlas Validator or press `Ctrl+Alt+V`

### Programmatic API

Other plugins can extend Atlas:

```cpp
// Get the Atlas Editor module
IAtlasEditorModule& AtlasModule = IAtlasEditorModule::Get();

// Register a custom validation rule
AtlasModule.RegisterRule(MakeShared<FMyCustomRule>());

// Request a validation scan
TArray<FAssetData> AssetsToScan;
AtlasModule.RequestScan(AssetsToScan);

// Listen for scan results
AtlasModule.OnIssuesReady().AddLambda([](const TArray<FAtlasIssue>& Issues)
{
    UE_LOG(LogTemp, Log, TEXT("Found %d issues"), Issues.Num());
});
```

### Creating Custom Rules

```cpp
class FMyCustomRule : public IAtlasRule
{
public:
    virtual FName GetRuleName() const override
    {
        return TEXT("MyCustomRule");
    }

    virtual void Validate(const UObject* Asset, TArray<FAtlasIssue>& OutIssues) override
    {
        // Check asset and add issues
        if (/* condition */)
        {
            FAtlasIssue Issue(
                Asset->GetPathName(),
                GetRuleName(),
                EAtlasSeverity::Warning,
                TEXT("Issue description"),
                true // Can fix
            );

            // Optional: Add fix function
            Issue.FixFunction = [Asset]() -> bool
            {
                // Fix the issue
                return true;
            };

            OutIssues.Add(Issue);
        }
    }
};
```

## Architecture

### Rule System
Atlas uses a rule-based validation system where each rule implements the `IAtlasRule` interface. Rules are:
- Registered at startup through `FAtlasRuleInitializer`
- Managed by `FAtlasRuleRegistry`
- Executed by `FAtlasScanExecutor`

### Issue Tracking
Issues are represented by `FAtlasIssue` structs containing:
- Asset path
- Rule name
- Severity (Info/Warning/Error)
- Description message
- Optional fix function

### Utility Pattern
Core functionality is organized into static utility classes:
- `FAtlasAssetUtils`: Generic asset operations
- `FAtlasTextureUtils`: Texture-specific operations
- `FAtlasMeshUtils`: Mesh-specific operations

## Benefits

Atlas prevents Unreal projects from rotting over time by ensuring assets remain:
- ✅ Correctly named
- ✅ Properly imported
- ✅ Structurally organized
- ✅ Memory-efficient
- ✅ Build-safe

It turns the editor into a self-cleaning ecosystem where content remains consistent, scalable, and performance-stable — even as many people contribute to it.

## Future Enhancements

See the `Todo` file for a complete list of planned features including:
- Complete Organizer implementation with import hooks
- Full UI panels with interactive issue lists
- Additional validation rules
- CI/CD commandlets
- Performance telemetry
- Material graph automation
- Perforce integration

## Requirements

- Unreal Engine 5.0+
- C++17 compiler

## License

Copyright Atlas Team. All Rights Reserved.

## Contributing

To add new validation rules:
1. Create a class implementing `IAtlasRule` in `Source/AtlasEditor/Private/Rules/`
2. Register it in `FAtlasRuleInitializer::RegisterDefaultRules()`
3. Implement `GetRuleName()`, `Validate()`, and optional fix functions

## Support

For issues, questions, or feature requests, please refer to the project repository.
