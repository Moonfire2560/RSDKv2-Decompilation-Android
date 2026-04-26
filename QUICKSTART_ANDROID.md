# Quick Start Guide - RSDKv2 Android Port

## For Odin 2 Portal Owners

This guide will get you from zero to playing in ~30 minutes.

### Step 1: Install Prerequisites (5 min)

1. Download Android Studio: https://developer.android.com/studio
2. Install it (accept all defaults)
3. Launch Android Studio
4. Click "More Actions" → "SDK Manager"
5. Install these (check the boxes):
   - Android SDK Platform 33
   - NDK 22.1.7171670
   - CMake 3.22.1

### Step 2: Set Up SDL2 (5 min)

Open a terminal in the RSDKv2-Decompilation folder:

```bash
# Run the setup script
./setup_sdl2.sh
```

This will:
- Download SDL2 2.30.4
- Build it for arm64-v8a
- Create the static library

**Troubleshooting**: If the script fails, manually set your NDK path:
```bash
export NDK_PATH="/path/to/Android/Sdk/ndk/22.1.7171670"
./setup_sdl2.sh
```

### Step 3: Configure Android Studio (2 min)

1. Copy `android/local.properties.template` to `android/local.properties`
2. Edit `local.properties` and set your SDK path:

**Windows**:
```
sdk.dir=C\:\\Users\\YourName\\AppData\\Local\\Android\\Sdk
```

**Mac**:
```
sdk.dir=/Users/YourName/Library/Android/sdk
```

**Linux**:
```
sdk.dir=/home/yourname/Android/Sdk
```

### Step 4: Build the APK (5 min)

Option A - Command Line (faster):
```bash
cd android
./gradlew assembleDebug
```

Option B - Android Studio (easier):
1. File → Open → Select the `android` folder
2. Wait for "Gradle sync" to finish
3. Build → Build Bundle(s) / APK(s) → Build APK(s)

**Output**: `android/app/build/outputs/apk/debug/app-debug.apk`

### Step 5: Prepare Your Odin 2 (5 min)

#### Enable USB Debugging:
1. Settings → About Device
2. Tap "Build Number" 7 times (enables Developer Mode)
3. Settings → System → Developer Options
4. Enable "USB Debugging"

#### Connect and Install:
```bash
# Connect Odin 2 via USB
adb devices  # Should show your device

# Install the APK
adb install android/app/build/outputs/apk/debug/app-debug.apk
```

### Step 6: Add Game Data (5 min)

```bash
# Create the directory
adb shell mkdir -p /sdcard/RSDK/v2

# Copy your game files
adb push Data.rsdk /sdcard/RSDK/v2/
adb push *.bin /sdcard/RSDK/v2/

# Verify
adb shell ls -l /sdcard/RSDK/v2/
```

### Step 7: Launch and Play! (1 min)

1. On your Odin 2, find "RSDKv2" in the app drawer
2. Launch it
3. Grant storage permissions when prompted
4. Play!

## Troubleshooting

### "SDL2 not found" during build
- Make sure `setup_sdl2.sh` completed successfully
- Check that `android/app/src/main/cpp/SDL2-2.30.4/build/arm64-v8a/libSDL2.a` exists

### "Game data not found" on launch
- Verify files: `adb shell ls -l /sdcard/RSDK/v2/`
- Make sure `Data.rsdk` is there
- Check file permissions

### App crashes immediately
```bash
# Check the crash log
adb logcat | grep -i "rsdk\|fatal\|crash"
```

### Black screen or no controls
- Try restarting the app
- Check that your Odin 2 controller is in Android mode
- SDL2 should auto-detect the controller

### Permission denied errors
- Settings → Apps → RSDKv2 → Permissions
- Enable "Storage"
- On Android 13+, enable "Files and media"

## What's in the Box

Your RSDKv2 Android port includes:
- ✅ Full RSDKv2 engine
- ✅ SDL2 2.30.4 for rendering and input
- ✅ Mod loader support
- ✅ Touch controls (basic)
- ✅ Game controller support
- ✅ Landscape mode (native for Odin 2)
- ✅ Save/load support

## Next Steps

Want to improve the port? Check out:
- `android/README.md` - Full build documentation
- Add more controller ABIs for other devices
- Implement touch controls overlay
- Add in-game menu for settings
- Create a nice app icon

## Getting Help

- Build issues → Check `android/README.md`
- Engine issues → https://github.com/RSDKModding/RSDKv2-Decompilation
- SDL2 issues → https://wiki.libsdl.org/

Enjoy playing on your Odin 2 Portal! 🎮
