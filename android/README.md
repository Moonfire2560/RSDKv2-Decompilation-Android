# RSDKv2 Android Port - Build Instructions

## Overview
This is an Android port of the RSDKv2 Decompilation project, configured to run on Odin 2 Portal and other Android devices. Game data is loaded from `/sdcard/RSDK/v2`.

## Build Configuration
- **NDK Version**: 22.1.7171670
- **CMake Version**: 3.22.1+
- **SDL2 Version**: 2.30.4 (statically linked)
- **Target ABI**: arm64-v8a
- **Min SDK**: 16 (Android 4.1)
- **Target SDK**: 33 (Android 13)

## Prerequisites

### 1. Install Android Studio
Download and install Android Studio from: https://developer.android.com/studio

### 2. Install Required SDK Components
Open Android Studio → Tools → SDK Manager and install:
- Android SDK Platform 33
- Android SDK Build-Tools 33.0.0+
- NDK 22.1.7171670
- CMake 3.22.1

### 3. Download and Build SDL2

#### Step 1: Download SDL2 2.30.4
```bash
cd android/app/src/main/cpp/
wget https://www.libsdl.org/release/SDL2-2.30.4.tar.gz
tar -xzf SDL2-2.30.4.tar.gz
```

#### Step 2: Build SDL2 for Android
```bash
cd SDL2-2.30.4
mkdir build
cd build

# Set your NDK path
export NDK_PATH="$HOME/Android/Sdk/ndk/22.1.7171670"
# Or on Windows: set NDK_PATH=C:\Users\YourName\AppData\Local\Android\Sdk\ndk\22.1.7171670

# Build for arm64-v8a
cmake .. \
    -DCMAKE_TOOLCHAIN_FILE=$NDK_PATH/build/cmake/android.toolchain.cmake \
    -DANDROID_ABI=arm64-v8a \
    -DANDROID_PLATFORM=android-16 \
    -DCMAKE_BUILD_TYPE=Release \
    -DSDL_SHARED=OFF \
    -DSDL_STATIC=ON

make -j$(nproc)
```

The built library will be at: `SDL2-2.30.4/build/arm64-v8a/libSDL2.a`

## Building the APK

### Option 1: Command Line (Gradle)

```bash
cd android

# Clean build
./gradlew clean

# Build debug APK
./gradlew assembleDebug

# Build release APK
./gradlew assembleRelease

# Install on connected device
./gradlew installDebug
```

Output APK location:
- Debug: `android/app/build/outputs/apk/debug/app-debug.apk`
- Release: `android/app/build/outputs/apk/release/app-release.apk`

### Option 2: Android Studio

1. Open Android Studio
2. File → Open → Select the `android` folder
3. Wait for Gradle sync to complete
4. Build → Make Project (Ctrl+F9)
5. Build → Build Bundle(s) / APK(s) → Build APK(s)

## Setting Up Game Data

### 1. Create the data directory on your device:
```bash
adb shell mkdir -p /sdcard/RSDK/v2
```

### 2. Copy your game files:
```bash
# Copy Data.rsdk
adb push Data.rsdk /sdcard/RSDK/v2/

# Copy any other required files (scripts, audio, etc.)
adb push *.bin /sdcard/RSDK/v2/
```

### Required files in `/sdcard/RSDK/v2/`:
- `Data.rsdk` - Main game data file
- Any `.bin` script files
- Optional: Music/audio files

## Permissions

The app requests the following permissions:
- `READ_EXTERNAL_STORAGE` - To read game data
- `WRITE_EXTERNAL_STORAGE` - To save game progress
- `MANAGE_EXTERNAL_STORAGE` (Android 11+) - For full file access

**Note**: On first launch, grant storage permissions when prompted.

## Troubleshooting

### Build Errors

#### "SDL2 not found"
- Make sure SDL2 is built and located at: `android/app/src/main/cpp/SDL2-2.30.4/build/arm64-v8a/libSDL2.a`
- Check CMakeLists.txt paths are correct

#### "NDK not found"
- Install NDK 22.1.7171670 via SDK Manager
- Verify path in `local.properties`: `ndk.dir=/path/to/ndk`

#### CMake errors
- Ensure CMake 3.22.1+ is installed
- Check `android/app/build.gradle` cmake version setting

### Runtime Errors

#### "Game data not found"
- Verify files are in `/sdcard/RSDK/v2/`
- Check file permissions
- Use `adb shell ls -l /sdcard/RSDK/v2/` to verify

#### Permission denied
- Go to Android Settings → Apps → RSDKv2 → Permissions
- Enable Storage permission
- On Android 11+, enable "All files access"

#### App crashes on launch
- Check logcat: `adb logcat | grep RSDKv2`
- Verify all native libraries loaded correctly
- Ensure device is arm64-v8a architecture

## Device Testing

### Check device ABI:
```bash
adb shell getprop ro.product.cpu.abi
```
Should return `arm64-v8a` for Odin 2 Portal

### Monitor logs:
```bash
adb logcat -s SDL:V RSDKv2:V *:E
```

## Project Structure

```
android/
├── app/
│   ├── src/
│   │   └── main/
│   │       ├── cpp/
│   │       │   ├── CMakeLists.txt          # Native build config
│   │       │   └── SDL2-2.30.4/            # SDL2 source (to be added)
│   │       ├── java/
│   │       │   └── com/rsdkmodding/rsdkv2/
│   │       │       └── RSDKv2Activity.java # Main activity
│   │       ├── res/
│   │       │   └── values/
│   │       │       └── strings.xml
│   │       └── AndroidManifest.xml
│   ├── build.gradle                        # App build config
│   └── proguard-rules.pro
├── gradle/
│   └── wrapper/
│       └── gradle-wrapper.properties
├── build.gradle                            # Project build config
├── settings.gradle
└── gradle.properties
```

## Adding SDL2 Java Classes

SDL2 includes Java helper classes that need to be copied to your project:

```bash
# After extracting SDL2-2.30.4.tar.gz
cp -r SDL2-2.30.4/android-project/app/src/main/java/org \
      android/app/src/main/java/
```

This copies the `org.libsdl.app.SDLActivity` class that `RSDKv2Activity` extends.

## Next Steps

1. **Add more ABIs**: Modify `android/app/build.gradle` to include `armeabi-v7a`, `x86`, `x86_64`
2. **Add app icon**: Replace placeholder icons in `res/mipmap-*` directories
3. **Optimize release builds**: Configure ProGuard rules and enable minification
4. **Add in-app file browser**: Let users select game data location
5. **Implement gamepad support**: Map SDL controller events

## Notes for Odin 2 Portal

- The Odin 2 Portal uses arm64-v8a architecture
- Screen resolution: 1920x1200 (landscape)
- Has built-in game controller
- SDL2 should automatically detect and map the controller

## Support

For issues specific to:
- **RSDKv2 engine**: https://github.com/RSDKModding/RSDKv2-Decompilation
- **Android build**: Check this README and project issues
- **SDL2**: https://wiki.libsdl.org/

## License

This Android port maintains the same license as the original RSDKv2-Decompilation project.
