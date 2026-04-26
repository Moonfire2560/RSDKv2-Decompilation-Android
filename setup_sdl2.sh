#!/bin/bash

# SDL2 Setup Script for RSDKv2 Android Build
# This script downloads and builds SDL2 2.30.4 for Android

set -e

SDL_VERSION="2.30.4"
SDL_DIR="android/app/src/main/cpp"
SDL_ARCHIVE="SDL2-${SDL_VERSION}.tar.gz"
SDL_URL="https://www.libsdl.org/release/${SDL_ARCHIVE}"

echo "=== RSDKv2 Android - SDL2 Setup ==="
echo ""

# Check if we're in the right directory
if [ ! -d "android" ]; then
    echo "Error: Please run this script from the RSDKv2-Decompilation root directory"
    exit 1
fi

# Create SDL directory if it doesn't exist
mkdir -p "$SDL_DIR"
cd "$SDL_DIR"

# Download SDL2 if not already present
if [ ! -f "$SDL_ARCHIVE" ]; then
    echo "Downloading SDL2 ${SDL_VERSION}..."
    wget "$SDL_URL" || curl -O "$SDL_URL"
    echo "Download complete!"
else
    echo "SDL2 archive already exists, skipping download."
fi

# Extract if not already extracted
if [ ! -d "SDL2-${SDL_VERSION}" ]; then
    echo "Extracting SDL2..."
    tar -xzf "$SDL_ARCHIVE"
    echo "Extraction complete!"
else
    echo "SDL2 already extracted."
fi

cd "SDL2-${SDL_VERSION}"

# Check for NDK
if [ -z "$NDK_PATH" ]; then
    # Try common NDK locations
    if [ -d "$HOME/Android/Sdk/ndk/22.1.7171670" ]; then
        export NDK_PATH="$HOME/Android/Sdk/ndk/22.1.7171670"
    elif [ -d "$ANDROID_HOME/ndk/22.1.7171670" ]; then
        export NDK_PATH="$ANDROID_HOME/ndk/22.1.7171670"
    elif [ -d "$ANDROID_SDK_ROOT/ndk/22.1.7171670" ]; then
        export NDK_PATH="$ANDROID_SDK_ROOT/ndk/22.1.7171670"
    else
        echo ""
        echo "Error: NDK not found!"
        echo "Please set NDK_PATH environment variable:"
        echo "  export NDK_PATH=/path/to/ndk/22.1.7171670"
        echo ""
        echo "Or install NDK 22.1.7171670 via Android Studio SDK Manager"
        exit 1
    fi
fi

echo ""
echo "Using NDK: $NDK_PATH"
echo ""

# Build SDL2
echo "Building SDL2 for arm64-v8a..."
mkdir -p build/arm64-v8a
cd build/arm64-v8a

cmake ../.. \
    -DCMAKE_TOOLCHAIN_FILE="$NDK_PATH/build/cmake/android.toolchain.cmake" \
    -DANDROID_ABI=arm64-v8a \
    -DANDROID_PLATFORM=android-16 \
    -DCMAKE_BUILD_TYPE=Release \
    -DSDL_SHARED=OFF \
    -DSDL_STATIC=ON \
    -DSDL_STATIC_PIC=ON

make -j$(nproc)

echo ""
echo "=== Build Complete! ==="
echo ""
echo "SDL2 library built at:"
echo "  $(pwd)/libSDL2.a"
echo ""
echo "You can now build the Android project:"
echo "  cd android"
echo "  ./gradlew assembleDebug"
echo ""
