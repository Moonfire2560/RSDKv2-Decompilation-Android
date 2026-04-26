# RSDKv2 Android Port - File Manifest

This document lists all files created for the Android port.

## 📁 Files Created

### Documentation (Root Level)
```
README_ANDROID.md          - Master README with quick navigation
QUICKSTART_ANDROID.md      - 30-minute quick start guide
BUILD_CHECKLIST.md         - Step-by-step verification checklist
ANDROID_PORT_SUMMARY.md    - Complete project overview
```

### Setup Scripts (Root Level)
```
setup_sdl2.sh              - Automated SDL2 download and build script
verify_setup.sh            - Setup verification and diagnostic script
```

### Android Build System
```
android/
├── .gitignore                              - Git ignore rules for Android
├── README.md                               - Detailed Android build documentation
├── build.gradle                            - Project-level Gradle configuration
├── settings.gradle                         - Gradle project settings
├── gradle.properties                       - Gradle build properties
├── local.properties.template               - Template for local SDK/NDK paths
│
├── gradle/
│   └── wrapper/
│       └── gradle-wrapper.properties       - Gradle wrapper configuration
│
└── app/
    ├── build.gradle                        - App-level build configuration
    ├── proguard-rules.pro                  - ProGuard rules for code obfuscation
    │
    └── src/
        └── main/
            ├── AndroidManifest.xml         - App manifest with permissions
            │
            ├── java/
            │   └── com/
            │       └── rsdkmodding/
            │           └── rsdkv2/
            │               └── RSDKv2Activity.java    - Main activity (SDL2)
            │
            ├── cpp/
            │   └── CMakeLists.txt          - Native code build configuration
            │
            └── res/
                ├── values/
                │   └── strings.xml         - App string resources
                │
                └── mipmap-*/               - Icon directories (empty placeholders)
                    ├── mipmap-mdpi/
                    ├── mipmap-hdpi/
                    ├── mipmap-xhdpi/
                    ├── mipmap-xxhdpi/
                    └── mipmap-xxxhdpi/
```

## 📋 File Purposes

### Documentation Files

#### README_ANDROID.md
- Quick navigation hub
- Links to all other docs
- Overview of project structure
- Quick start commands

#### QUICKSTART_ANDROID.md
- Step-by-step 30-minute guide
- Beginner-friendly
- Covers installation to first run
- Includes troubleshooting

#### BUILD_CHECKLIST.md
- Detailed verification checklist
- Pre-build requirements
- Build process steps
- Post-build verification
- Troubleshooting quick reference

#### ANDROID_PORT_SUMMARY.md
- Complete project overview
- Technical specifications
- Build configuration details
- Performance expectations
- Development roadmap

### Build Scripts

#### setup_sdl2.sh
- Downloads SDL2 2.30.4
- Extracts archive
- Configures CMake for Android
- Builds static library for arm64-v8a
- Verifies NDK installation

#### verify_setup.sh
- Checks project structure
- Verifies source files present
- Checks SDL2 built correctly
- Validates configuration files
- Reports errors and warnings
- Color-coded output

### Android Configuration Files

#### android/build.gradle (Root)
- Project-level Gradle configuration
- Defines repositories (Google, Maven)
- Sets Android Gradle Plugin version (8.1.0)

#### android/settings.gradle
- Defines project structure
- Includes app module

#### android/gradle.properties
- JVM memory settings
- AndroidX configuration
- Build optimization flags

#### android/local.properties.template
- Template for user-specific paths
- SDK location
- NDK location
- CMake location

#### android/app/build.gradle
- App-level build configuration
- Compile/target SDK versions (16/33)
- NDK version (22.1.7171670)
- CMake configuration
- ABI filters (arm64-v8a)
- Dependencies (AppCompat)

#### android/app/proguard-rules.pro
- ProGuard configuration
- Keeps SDL2 classes
- Keeps RSDKv2 classes
- Prevents obfuscation of critical code

### Android Source Files

#### AndroidManifest.xml
- App package name
- Permissions (storage access)
- Activity configuration
- Landscape orientation
- Fullscreen mode
- Export settings

#### RSDKv2Activity.java
- Extends SDLActivity
- Permission handling (storage)
- Android 11+ compatibility
- Game data path configuration
- Library loading order
- User notifications

#### CMakeLists.txt (Native)
- Native build configuration
- SDL2 integration
- Source file locations
- Include directories
- Compile definitions (BASE_PATH, etc.)
- Library linking
- Android-specific libraries

#### strings.xml
- App name resource
- Localization ready

### Support Files

#### .gitignore
- Ignores build artifacts
- Ignores IDE files
- Ignores SDL2 downloads
- Ignores local.properties
- Keeps repository clean

## 📊 File Statistics

```
Documentation:      4 files (~15 KB text)
Scripts:            2 files (executable)
Gradle:             5 files
Android XML:        2 files
Java:               1 file (~100 lines)
CMake:              1 file (~90 lines)
Configuration:      3 files
Total:             18 core files
```

## 🎯 Critical Files (Must Have)

These files are essential for building:

1. **android/app/build.gradle** - Without this, Gradle can't build
2. **android/app/src/main/cpp/CMakeLists.txt** - Native code won't compile
3. **android/app/src/main/AndroidManifest.xml** - Android requires this
4. **android/app/src/main/java/.../RSDKv2Activity.java** - Entry point
5. **setup_sdl2.sh** - SDL2 setup automation
6. **android/local.properties** - SDK/NDK paths (user creates from template)

## 🔄 File Dependencies

```
Build Process Flow:
1. local.properties → Provides SDK/NDK paths
2. gradle.properties → Sets build parameters
3. settings.gradle → Defines project structure
4. build.gradle (root) → Configures Android plugin
5. app/build.gradle → Configures app build
6. AndroidManifest.xml → Defines app structure
7. CMakeLists.txt → Builds native code
8. RSDKv2Activity.java → Main entry point
```

## 📝 Files User Must Create/Modify

### Must Create:
- `android/local.properties` (from template)

### Must Add:
- SDL2-2.30.4 source (via setup_sdl2.sh)
- SDL2 Java classes (copy from SDL2 android-project)
- App icons (optional, placeholders provided)

### Should Modify:
- `android/app/build.gradle` - Add more ABIs if needed
- `AndroidManifest.xml` - Customize app name/icon
- `strings.xml` - Add localization

## 🚫 Files NOT Included (Will Be Generated)

These are created by the build process:

```
android/
├── .gradle/                    - Gradle cache
├── build/                      - Build outputs
├── app/
│   ├── .cxx/                  - CMake build files
│   ├── build/                 - App build outputs
│   │   ├── intermediates/
│   │   └── outputs/
│   │       └── apk/
│   │           ├── debug/
│   │           │   └── app-debug.apk     ← Your APK!
│   │           └── release/
└── local.properties           - Created by user from template
```

## 📦 Files for Distribution

If you want to share just the Android port files:

### Minimum Package:
```
android/                       - Complete directory
setup_sdl2.sh                  - SDL2 setup script
verify_setup.sh                - Verification script
QUICKSTART_ANDROID.md          - Quick start guide
```

### Complete Package:
```
All of the above, plus:
BUILD_CHECKLIST.md
ANDROID_PORT_SUMMARY.md
README_ANDROID.md
```

## 🔧 Customization Points

Files you might want to customize:

1. **app/build.gradle**
   - Add more ABIs (armeabi-v7a, x86, x86_64)
   - Change version code/name
   - Adjust min/target SDK

2. **CMakeLists.txt**
   - Change BASE_PATH for different data location
   - Add more compile flags
   - Link additional libraries

3. **AndroidManifest.xml**
   - Change app name
   - Add more permissions
   - Adjust screen orientation

4. **RSDKv2Activity.java**
   - Customize permission handling
   - Add settings UI
   - Implement file browser

## ✅ Verification Checklist

Run this to verify all files are present:

```bash
./verify_setup.sh
```

Expected structure:
```
✓ RSDKv2 source files (23 .cpp files)
✓ Android build.gradle files (2 files)
✓ AndroidManifest.xml
✓ RSDKv2Activity.java
✓ CMakeLists.txt
✓ Documentation (4 files)
✓ Setup scripts (2 files)
```

## 📌 Notes

- All created files are ASCII/UTF-8 text
- Scripts are bash-compatible
- No binary files included (except eventual builds)
- All documentation is Markdown format
- Java code follows Android conventions
- CMake follows SDL2 Android patterns

---

**Last Updated**: April 2026
**Configuration**: Odin 2 Portal optimized (arm64-v8a)
**Total Files Created**: 18 core files + documentation
