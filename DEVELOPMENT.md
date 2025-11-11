# Atlas Development Guide

## Current Implementation Progress

This document tracks what has been implemented and what remains from the original TODO specification.

### ✅ Fully Implemented (Sections 0-6 Core Infrastructure)

#### Section 0: Plugin & Module Setup
- ✅ `Atlas.uplugin` with AtlasEditor and AtlasRuntime modules
- ✅ `AtlasEditor.Build.cs` with all required dependencies
- ✅ `AtlasRuntime.Build.cs` with core dependencies
- ✅ Module initialization code

#### Section 1: Settings & Config
- ✅ `UAtlasSettings` class with all properties
- ✅ `DefaultEditor.ini` with seed values
- ✅ Configuration for paths, naming, thresholds

#### Section 2: Common Core (Editor)
- ✅ `AtlasTypes.h` with enums (EAtlasSeverity, EAtlasMapType)
- ✅ `FAtlasIssue` struct with fix function support
- ✅ `IAtlasRule` interface
- ✅ `AtlasRuleRegistry` with Register/All/FindRule
- ✅ `AtlasScanExecutor` with ScanAll/ScanSelection
- ✅ `AtlasAssetUtils` with Move/Rename/Reimport/SyncToCB
- ✅ `AtlasTextureUtils` with DetectMapType/EstimateVRAM/IsLikelyNormal
- ✅ `AtlasMeshUtils` with GetTriangleCount/EnsureCollision/LOD checks

#### Section 4: Atlas Validator (Partial)
- ✅ Three example rules implemented:
  - `FAtlasNamingConventionRule` - checks prefixes and paths
  - `FAtlasTextureCompressionRule` - checks texture compression
  - `FAtlasStaticMeshCollisionRule` - checks mesh collision
- ✅ All rules support auto-fix functionality
- ✅ Rule registration system via `FAtlasRuleInitializer`

#### Section 6: Content Browser Integration (Partial)
- ✅ Command definitions in `FAtlasCommands`
- ✅ Menu registration in Window menu and toolbar

#### Section 15: Error & Log
- ✅ `DEFINE_LOG_CATEGORY(LogAtlas)` in AtlasRuntime

#### Section 16: UI IDs & Registration
- ✅ Tab IDs: `Atlas.Overview`, `Atlas.Validator`
- ✅ Style set: `AtlasStyle`
- ✅ Command context: `AtlasEditor`

### 🚧 Partially Implemented

#### UI Framework
- ✅ Tab spawners registered
- ✅ Style system created
- ⚠️ Actual UI panels show placeholders only
- ⚠️ Need to implement full Slate widgets

### ❌ Not Yet Implemented

#### Section 3: Atlas Organizer
The entire Organizer system needs implementation:
- Import hook: `FEditorDelegates::OnAssetPostImport`
- `FAtlasOrganizer` class
- `SAtlasOrganizerPrompt` Slate widget
- Material template creation
- Texture map type mapping
- Bulk organization mode

**Implementation Priority**: HIGH
**Location**: `Source/AtlasEditor/Private/Organizer/`

#### Section 4: Additional Validation Rules
Need to implement:
- `FAtlasTextureSizeRule` - Check max texture dimensions
- `FAtlasStaticMeshLODRule` - Check LOD requirements
- `FAtlasMaterialInstanceDupRule` - Find duplicate materials
- `FAtlasAudioNormalizationRule` - Audio validation

**Implementation Priority**: MEDIUM
**Location**: `Source/AtlasEditor/Private/Rules/`

#### Section 5: Atlas Overview Dashboard
Complete dashboard needs implementation:
- `FAtlasOverviewStats` data collector
- `SAtlasOverviewPanel` Slate widget with:
  - VRAM/Project Size cards
  - Top textures list
  - Top meshes list
  - Levels by size
- `FAtlasThresholds` evaluation engine
- JSON report export

**Implementation Priority**: HIGH
**Location**: `Source/AtlasEditor/Private/Overview/`

#### Section 5: Complete Validator UI
The validator panel needs full implementation:
- `SAtlasValidatorPanel` Slate widget
- Issue list view with columns
- Toolbar with Scan/Fix buttons
- Severity filtering
- Row double-click to sync
- Fix Selected/Fix All actions

**Implementation Priority**: HIGH
**Location**: `Source/AtlasEditor/Private/Validator/`

#### Section 6: Complete Content Browser Integration
Need to add context menus:
- "Atlas → Organize Selection"
- "Atlas → Validate Selection"
- "Atlas → Send to Overview"

**Implementation Priority**: MEDIUM
**Location**: `Source/AtlasEditor/Private/ContentBrowser/`

#### Section 7: Commandlets (CI)
Implement commandlets for automation:
- `UAtlasValidateCommandlet` - Run validation in CI
- `UAtlasOverviewCommandlet` - Generate reports
- Exit code handling
- JSON output

**Implementation Priority**: MEDIUM
**Location**: `Source/AtlasRuntime/Private/Commandlets/`

#### Section 8: Telemetry
Optional telemetry system:
- `FAtlasTimeline` class
- `FAtlasSnapshot` struct
- History tracking

**Implementation Priority**: LOW

#### Section 9: Performance & Safety
Enhancements needed:
- Async scanning with thread pool
- `FScopedSlowTask` progress UI
- Redirector fixup after moves
- Transaction support for undo
- Hot-reload safety

**Implementation Priority**: MEDIUM

#### Section 10: Tests
Automation testing:
- Import test
- Validation test
- Overview calculation test
- Golden data

**Implementation Priority**: LOW

#### Section 12: Console Commands
Implement console commands:
- `Atlas.ScanAll`
- `Atlas.ValidateSelection`
- `Atlas.OrganizeSelection`
- `Atlas.ExportReport`

**Implementation Priority**: LOW
**Location**: `Source/AtlasEditor/Private/Commands/`

#### Section 13: Rules Config DataAsset
Optional rule configuration system:
- `UAtlasRuleProfile` DataAsset
- Per-rule enable/disable

**Implementation Priority**: LOW

#### Section 14: Material Graph Helpers
Material automation:
- `FAtlasMaterialBuilder` class
- Blueprint material library integration
- PBR parameter mapping

**Implementation Priority**: MEDIUM

#### Section 17: Minimal Assets
Need actual editor content:
- Replace placeholder icons with real PNG assets
- Create `M_MegaPBR_Master` material template

**Implementation Priority**: MEDIUM

## Next Steps for Development

### Phase 1: Complete Core Features (High Priority)
1. Implement Atlas Validator UI panel
   - Create `SAtlasValidatorPanel.h/.cpp`
   - Add issue list view
   - Add toolbar with actions
   - Wire up to `FAtlasScanExecutor`

2. Implement Atlas Overview Dashboard
   - Create `AtlasOverviewStats.h/.cpp`
   - Create `SAtlasOverviewPanel.h/.cpp`
   - Implement VRAM and size calculations
   - Add top assets lists

3. Implement Atlas Organizer
   - Hook into `OnAssetPostImport`
   - Create `FAtlasOrganizer` logic
   - Create `SAtlasOrganizerPrompt` dialog
   - Implement material creation

### Phase 2: Expand Validation Rules (Medium Priority)
4. Add remaining validation rules
   - Texture size rule
   - Mesh LOD rule
   - Material duplication rule
   - Audio rule (stub)

5. Add Content Browser context menus
   - Organize selection
   - Validate selection
   - Send to overview

### Phase 3: CI/CD Integration (Medium Priority)
6. Implement commandlets
   - Validate commandlet
   - Overview commandlet
   - JSON output format

### Phase 4: Polish & Optimization (Low Priority)
7. Add async scanning
8. Add progress dialogs
9. Add console commands
10. Create automation tests

## Code Patterns & Conventions

### Adding a New Validation Rule

1. Create header in `Source/AtlasEditor/Private/Rules/`:
```cpp
class FAtlasMyNewRule : public IAtlasRule
{
public:
    virtual FName GetRuleName() const override;
    virtual void Validate(const UObject* Asset, TArray<FAtlasIssue>& OutIssues) override;
    virtual FString GetDescription() const override;
    virtual bool IsEnabled() const override;
};
```

2. Implement validation logic in `.cpp` file
3. Register in `FAtlasRuleInitializer::RegisterDefaultRules()`:
```cpp
Registry.Register(MakeShared<FAtlasMyNewRule>());
```

### Creating Slate UI Widgets

All UI panels should:
- Inherit from `SCompoundWidget`
- Use `SLATE_BEGIN_ARGS` / `SLATE_END_ARGS` macros
- Follow Unreal's Slate architecture
- Be located in `Source/AtlasEditor/Private/Widgets/`

### Utility Functions

When adding utility functions:
- Make them static
- Group by asset type (Texture, Mesh, Material, etc.)
- Add to appropriate Utils class
- Include null checks
- Log errors/warnings appropriately

## Building the Plugin

1. Place the `Plugins/Atlas/` folder in your Unreal project's `Plugins/` directory
2. Delete `Binaries/` and `Intermediate/` folders if they exist
3. Right-click your `.uproject` file → "Generate Visual Studio project files"
4. Build in Visual Studio or Rider
5. Launch the editor - Atlas will appear in the Window menu

## Testing

### Manual Testing Checklist
- [ ] Plugin loads without errors
- [ ] Atlas settings appear in Project Settings
- [ ] Atlas Overview tab opens
- [ ] Atlas Validator tab opens
- [ ] Validation scan completes
- [ ] Issues are reported
- [ ] Fix functions work
- [ ] Settings are persisted

### Automated Testing (To Be Implemented)
See Section 10 of the original TODO for test specifications.

## Debugging

Enable verbose logging:
```cpp
UE_LOG(LogAtlas, VeryVerbose, TEXT("Debug message"));
```

View logs in:
- Output Log panel in editor
- `Saved/Logs/` directory

## Architecture Decisions

### Why Static Utility Classes?
- No state needed
- Easy to call from anywhere
- Clear organization by domain
- Follows Unreal patterns (FPaths, FMath, etc.)

### Why TFunction for Fix Functions?
- Allows capturing context
- Deferred execution
- Can be serialized out to JSON (with limitations)
- Simple to implement

### Why Separate Editor and Runtime Modules?
- Runtime module can be packaged with games if needed
- Editor module keeps heavy dependencies separate
- Follows Unreal best practices
- Allows for in-game telemetry in future

## Performance Considerations

### Current Limitations
- Full scans load all assets into memory
- Synchronous validation blocks UI
- No caching of scan results

### Recommended Improvements
1. Use `FAssetData` for lightweight checks where possible
2. Implement async scanning with `Async()`
3. Add progress dialogs with `FScopedSlowTask`
4. Cache results between scans
5. Add incremental validation option

## Questions & Support

For questions about implementation:
1. Review this document
2. Check the original `Todo` file for specifications
3. Study the existing code patterns
4. Refer to Unreal Engine documentation

## Contributing

When adding features:
1. Follow existing code style
2. Add copyright headers
3. Update this document
4. Test thoroughly
5. Document public APIs

## Useful References

- [Unreal Slate Documentation](https://docs.unrealengine.com/en-US/ProgrammingAndScripting/Slate/)
- [Plugin Documentation](https://docs.unrealengine.com/en-US/ProductionPipelines/Plugins/)
- [Asset Registry](https://docs.unrealengine.com/en-US/ProgrammingAndScripting/ProgrammingWithCPP/Assets/Registry/)
- [Developer Settings](https://docs.unrealengine.com/en-US/API/Runtime/DeveloperSettings/UDeveloperSettings/)
