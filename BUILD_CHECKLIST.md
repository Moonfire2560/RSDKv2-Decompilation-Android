# RSDKv2 Android Build Checklist

Use this checklist to ensure you have everything configured correctly.

## ✅ Pre-Build Checklist

### 1. Android Studio Setup
- [ ] Android Studio installed
- [ ] SDK Platform 33 installed
- [ ] NDK 22.1.7171670 installed
- [ ] CMake 3.22.1+ installed

### 2. SDL2 Setup
- [ ] SDL2-2.30.4.tar.gz downloaded
- [ ] SDL2 extracted to `android/app/src/main/cpp/SDL2-2.30.4/`
- [ ] SDL2 built for arm64-v8a
- [ ] `libSDL2.a` exists at `android/app/src/main/cpp/SDL2-2.30.4/build/arm64-v8a/libSDL2.a`

Quick verification:
```bash
ls -lh android/app/src/main/cpp/SDL2-2.30.4/build/arm64-v8a/libSDL2.a
```

### 3. SDL2 Java Classes
- [ ] SDL Java classes copied to project
```bash
cp -r android/app/src/main/cpp/SDL2-2.30.4/android-project/app/src/main/java/org \
      android/app/src/main/java/
```

### 4. Project Configuration
- [ ] `android/local.properties` created from template
- [ ] SDK path set correctly in `local.properties`
- [ ] NDK path set correctly (if needed)

Verify paths:
```bash
cat android/local.properties
```

### 5. Source Code
- [ ] RSDKv2 source files present in `RSDKv2/` directory
- [ ] All `.cpp` and `.hpp` files accessible
- [ ] CMakeLists.txt points to correct source locations

Quick check:
```bash
ls -1 RSDKv2/*.cpp | wc -l  # Should show 23 files
```

## ✅ Build Process Checklist

### 6. Initial Build
- [ ] Changed to android directory: `cd android`
- [ ] Gradle wrapper is executable: `chmod +x gradlew`
- [ ] Clean build started: `./gradlew clean`
- [ ] No configuration errors

### 7. Gradle Sync
- [ ] Gradle sync completed successfully
- [ ] No dependency resolution errors
- [ ] CMake configuration succeeded
- [ ] All native libraries found

Watch for:
```
BUILD SUCCESSFUL
```

### 8. APK Build
- [ ] Debug build completed: `./gradlew assembleDebug`
- [ ] APK created at `app/build/outputs/apk/debug/app-debug.apk`
- [ ] APK size is reasonable (5-15 MB typical)

Verify APK:
```bash
ls -lh app/build/outputs/apk/debug/app-debug.apk
```

### 9. APK Installation
- [ ] Device connected: `adb devices` shows your device
- [ ] USB debugging enabled on device
- [ ] Installation successful: `adb install -r app/build/outputs/apk/debug/app-debug.apk`

Expected output:
```
Success
```

## ✅ Post-Build Checklist

### 10. Game Data Setup
- [ ] Data directory created: `/sdcard/RSDK/v2`
- [ ] `Data.rsdk` copied to device
- [ ] Script files (`.bin`) copied if needed
- [ ] File permissions verified

Verify data:
```bash
adb shell ls -l /sdcard/RSDK/v2/
```

### 11. App Launch
- [ ] App icon appears in launcher
- [ ] App launches without crash
- [ ] Storage permissions granted
- [ ] No immediate fatal errors

### 12. Runtime Verification
- [ ] Game data loads successfully
- [ ] Graphics render correctly
- [ ] Audio plays (if available)
- [ ] Touch input works
- [ ] Controller input works (Odin 2)
- [ ] Game runs at acceptable framerate

Check logs if issues:
```bash
adb logcat -s SDL:V RSDKv2:V *:E
```

## 🔍 Troubleshooting Quick Reference

### Issue: SDL2 not found
```bash
# Verify SDL2 library exists
ls android/app/src/main/cpp/SDL2-2.30.4/build/arm64-v8a/libSDL2.a

# Rebuild SDL2 if needed
cd android/app/src/main/cpp/SDL2-2.30.4
./setup_sdl2.sh
```

### Issue: CMake errors
```bash
# Check CMake version
cmake --version  # Should be 3.22.1+

# Verify NDK path
echo $NDK_PATH
# Or check local.properties
cat android/local.properties
```

### Issue: Gradle sync fails
```bash
# Clear Gradle cache
cd android
./gradlew clean
rm -rf .gradle build
./gradlew assembleDebug
```

### Issue: App crashes on launch
```bash
# Get detailed crash log
adb logcat -c  # Clear log
# Launch app on device
adb logcat -s AndroidRuntime:E SDL:E RSDKv2:E

# Check native library loading
adb logcat | grep "dlopen\|\.so"
```

### Issue: Game data not found
```bash
# Verify directory and files
adb shell ls -lR /sdcard/RSDK/v2/

# Check app permissions
adb shell dumpsys package com.rsdkmodding.rsdkv2 | grep -A5 "granted=true"

# Try manual directory creation
adb shell mkdir -p /sdcard/RSDK/v2
adb push Data.rsdk /sdcard/RSDK/v2/
```

## 📊 Build Time Estimates

| Task | First Time | Subsequent |
|------|------------|------------|
| SDL2 Download | 2 min | - |
| SDL2 Build | 3-5 min | - |
| Gradle Sync | 2-3 min | 30 sec |
| APK Build | 3-5 min | 1-2 min |
| Total | ~15 min | ~3 min |

## ✨ Success Indicators

You know everything is working when:
- ✅ `./gradlew assembleDebug` completes with "BUILD SUCCESSFUL"
- ✅ APK installs without errors
- ✅ App launches and shows the game
- ✅ No crashes in first 30 seconds
- ✅ Game responds to input
- ✅ Audio plays (if applicable)

## 🎯 Ready to Ship?

For a release build:
- [ ] Test on multiple devices/Android versions
- [ ] Verify save/load functionality
- [ ] Test all game features
- [ ] Create proper app icon
- [ ] Sign APK with release keystore
- [ ] Test release APK separately

Release build command:
```bash
./gradlew assembleRelease
```

---

**Last Updated**: Build configuration for arm64-v8a with SDL2 2.30.4
