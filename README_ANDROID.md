# RSDKv2 Android Port for Odin 2 Portal

This repository contains a complete Android port of the RSDKv2 Decompilation project, specifically configured for the Odin 2 Portal handheld gaming device.

## 🚀 Quick Start (Choose Your Path)

### Option 1: I want to build it NOW (30 minutes)
👉 **Read: [QUICKSTART_ANDROID.md](QUICKSTART_ANDROID.md)**

### Option 2: I want to understand everything first
👉 **Read: [android/README.md](android/README.md)**

### Option 3: I want a step-by-step checklist
👉 **Read: [BUILD_CHECKLIST.md](BUILD_CHECKLIST.md)**

### Option 4: I want a project overview
👉 **Read: [ANDROID_PORT_SUMMARY.md](ANDROID_PORT_SUMMARY.md)**

## 🎯 What You Get

- ✅ Full RSDKv2 engine running on Android
- ✅ Optimized for Odin 2 Portal (arm64-v8a)
- ✅ SDL2 2.30.4 integration
- ✅ Game data loads from `/sdcard/RSDK/v2`
- ✅ Native controller support
- ✅ Mod loader enabled
- ✅ Landscape mode for handheld gaming

## 📋 Requirements

- Android Studio
- NDK 22.1.7171670
- CMake 3.22.1+
- SDL2 2.30.4 (setup script provided)
- Your RSDKv2 game data files

## 🔧 Setup Overview

```bash
# 1. Verify your setup
./verify_setup.sh

# 2. Setup SDL2 (automated)
./setup_sdl2.sh

# 3. Build the APK
cd android
./gradlew assembleDebug

# 4. Install on your Odin 2
adb install app/build/outputs/apk/debug/app-debug.apk

# 5. Copy game data
adb push Data.rsdk /sdcard/RSDK/v2/
```

## 📁 Project Structure

```
RSDKv2-Decompilation/
├── RSDKv2/                          # Original engine source
├── android/                         # Android build system
│   ├── app/
│   │   ├── build.gradle            # Build configuration
│   │   └── src/main/
│   │       ├── AndroidManifest.xml
│   │       ├── java/               # Java activity
│   │       ├── cpp/                # Native build (CMake)
│   │       └── res/                # Resources
│   ├── build.gradle
│   ├── gradle.properties
│   └── README.md                    # Detailed build docs
├── setup_sdl2.sh                    # SDL2 automated setup
├── verify_setup.sh                  # Check if ready to build
├── QUICKSTART_ANDROID.md            # 30-minute guide
├── BUILD_CHECKLIST.md               # Step-by-step checklist
└── ANDROID_PORT_SUMMARY.md          # Complete overview
```

## 🎮 For Odin 2 Portal Users

Your device specs:
- **Architecture**: arm64-v8a ✅
- **Screen**: 1920x1200 landscape
- **Controller**: Built-in (auto-detected)
- **OS**: Android 13

This port is specifically optimized for these specs.

## 📖 Documentation

| Document | What It's For | Read Time |
|----------|--------------|-----------|
| [QUICKSTART_ANDROID.md](QUICKSTART_ANDROID.md) | Get building fast | 5 min |
| [android/README.md](android/README.md) | Complete reference | 15 min |
| [BUILD_CHECKLIST.md](BUILD_CHECKLIST.md) | Verify each step | 10 min |
| [ANDROID_PORT_SUMMARY.md](ANDROID_PORT_SUMMARY.md) | Project overview | 10 min |

## 🐛 Troubleshooting

Run the verification script first:
```bash
./verify_setup.sh
```

Common issues:
- **SDL2 not found**: Run `./setup_sdl2.sh`
- **NDK not found**: Install via Android Studio SDK Manager
- **Game data missing**: Copy files to `/sdcard/RSDK/v2/`

See [BUILD_CHECKLIST.md](BUILD_CHECKLIST.md) for detailed troubleshooting.

## 🎯 Build Configuration

- **Min SDK**: 16 (Android 4.1)
- **Target SDK**: 33 (Android 13)
- **NDK**: 22.1.7171670
- **CMake**: 3.22.1+
- **SDL2**: 2.30.4 (static)
- **ABI**: arm64-v8a

## 📦 What's Included

### Android Application:
- Main activity (RSDKv2Activity.java)
- SDL2 integration layer
- CMake native build system
- Gradle build configuration
- Storage permission handling

### Build Scripts:
- `setup_sdl2.sh` - Download and build SDL2
- `verify_setup.sh` - Verify configuration

### Documentation:
- Quick start guide
- Complete build manual
- Step-by-step checklist
- Project summary

## 🚀 Building

### First Time Build:
```bash
./verify_setup.sh          # Check prerequisites
./setup_sdl2.sh            # Setup SDL2 (5 min)
cd android
./gradlew assembleDebug    # Build APK (5 min)
```

### Subsequent Builds:
```bash
cd android
./gradlew assembleDebug    # ~2 minutes
```

## 📱 Installing

### Via ADB:
```bash
adb install app/build/outputs/apk/debug/app-debug.apk
```

### Via Android Studio:
Run → Run 'app' (Shift+F10)

## 🎮 Game Data Setup

```bash
# Create directory
adb shell mkdir -p /sdcard/RSDK/v2

# Copy game files
adb push Data.rsdk /sdcard/RSDK/v2/
adb push *.bin /sdcard/RSDK/v2/

# Verify
adb shell ls -l /sdcard/RSDK/v2/
```

## ✅ Success Indicators

You're ready when:
- `./verify_setup.sh` shows all green ✓
- `./gradlew assembleDebug` completes successfully
- APK installs without errors
- App launches and shows game
- Controller input works

## 🤝 Contributing

This is a clean port of RSDKv2-Decompilation for Android. To contribute:

1. Test on different Android devices
2. Report issues with device specs
3. Submit improvements via PR
4. Add support for more ABIs

## 📜 License

Same as [RSDKv2-Decompilation](https://github.com/RSDKModding/RSDKv2-Decompilation)

## 🔗 Links

- **Original Project**: https://github.com/RSDKModding/RSDKv2-Decompilation
- **SDL2**: https://www.libsdl.org/
- **Odin 2**: https://www.ayn.hk/

## 💬 Support

- Build issues? See [BUILD_CHECKLIST.md](BUILD_CHECKLIST.md)
- Quick questions? See [QUICKSTART_ANDROID.md](QUICKSTART_ANDROID.md)
- Complete docs? See [android/README.md](android/README.md)
- Engine issues? See original RSDKv2-Decompilation repo

---

**Ready to build?** → Start with `./verify_setup.sh` 🚀
