# Agent Configuration

## System Context
- **Project**: PauseRemapper - Beat Saber 1.37 Quest standalone mod
- **Working Directory**: /media/hostsrc/PauseRemapper
- **Platform**: Arch Linux with Android NDK
- **Build System**: CMake + Ninja
- **Mod Version**: 1.2.3

## Project Structure

### Key Directories
- `src/` - C++ source files
  - `UI/UIManager.cpp` - BSML UI implementation
  - `PauseRemapperController.cpp` - Core pause logic
  - `Installers/PauseRemapperInstaller.cpp` - Zenject bindings
  - `main.cpp` - Entry point
- `include/` - Header files
  - `main.hpp` - Export macros, includes
  - `MainConfig.hpp` - Config definitions
- `cmake/` - CMake modules
- `scripts/` - Build/copy PowerShell scripts
- `assets/` - BSML files embedded in binary
- `extern/` - QPM dependencies

### Important Files
- `CMakeLists.txt` - Modern cmake structure (3.22+)
- `qpm.json` - Dependencies (beatsaber-hook, bsml, custom-types, lapiz, config-utils, scotland2)
- `mod.template.json` - QMod template
- `assets_include.cmake` - Binary asset embedding

## Core Principles

### Build Workflow
- Use `qpm restore` to fetch dependencies
- Set `ANDROID_NDK_HOME` environment variable
- Build: `cmake -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo -B build && cmake --build ./build`
- Create qmod: `zip -j PauseRemapper.zip mod.json build/libPauseRemapper.so PauseRemapper.gif`

### BSML UI Pattern
- Use `DECLARE_CLASS_CODEGEN` with `HMUI::ViewController` base
- Override `DidActivate()` for UI creation
- Use `BSML::Lite::CreateToggle()` etc. for controls
- Pass `UIManager::New_ctor()` to registration, not singleton pattern
- **Avoid** `DECLARE_BSML_PROPERTY` - causes null pointer crashes in 1.37

### Code Conventions
- Use `PaperLogger` from main.hpp for logging
- Export functions: `MOD_EXTERN_FUNC void func() noexcept`
- Config: Use `DECLARE_CONFIG` macro with `CONFIG_VALUE` entries
- Hooks: Use `MAKE_HOOK_MATCH` and `INSTALL_HOOK(PaperLogger, hookName)`
- Zenject: Use `zenjector->Install<InstallerType*, TargetInstaller*>()`

### Migration Notes (1.28 → 1.37)
- Scotland2 is required modloader (add to qpm.json)
- Use `modFiles` not `lateModFiles` in mod.template.json
- CMake 3.22+ with modular structure in cmake/ directory
- Add `"cmake": true` to qpm.json info.additionalData
- Paper logger replaces custom logging macros
- Build as RelWithDebInfo, strip debug symbols post-build

## Dependencies

### Required QPM Packages
- `beatsaber-hook` (^5.1.0) - Hooking framework
- `bsml` (^0.4.43) - UI library
- `custom-types` (^0.17.10) - Il2Cpp type patching
- `lapiz` (^0.2.12) - Zenject utilities
- `config-utils` (^1.4.2) - Configuration management
- `scotland2` (^0.1.4) - Modloader (includeQmod: false)
- `bs-cordl` (^3700.0.0) - Codegen headers

## Common Tasks

### Adding a new setting
1. Add `CONFIG_VALUE(name, type, "Display Name", default)` to MainConfig.hpp
2. Add toggle in UIManager::DidActivate with getter/setter lambdas
3. Rebuild and test

### Debugging crashes
- Check `logs/crash.log` for tombstone
- Look for null pointer dereferences (fault addr 0x28 usually BSML invoker issues)
- Ensure custom_types::Register::AutoRegister() called in late_load
- Verify BSML::Init() before UI registration

### Building for release
1. `qpm restore` - ensure deps up to date
2. `rm -rf build` - clean build
3. `export ANDROID_NDK_HOME=/path/to/ndk`
4. Build with cmake
5. Strip symbols automatically via cmake/strip.cmake
6. Package as qmod

## Troubleshooting

### Build errors
- "No NDK": Set ANDROID_NDK_HOME environment variable
- "Ninja not found": Install ninja build tool
- Asset errors: Ensure assets_include.cmake included in CMakeLists.txt

### Runtime errors
- Crash in custom_types::invoker_creator: UI manager not properly instantiated
- UI not appearing: Check BSML::Register::RegisterGameplaySetupTab call
- Config not saving: Verify getMainConfig() is accessible in lambda scope

## External References
- Beat Saber version: 1.37.0_9064817954
- Target platform: Meta Quest (Android 12-14)
- Architecture: arm64-v8a
- Unity version: 2021.3.16f1
