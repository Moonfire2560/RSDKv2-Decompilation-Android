#!/bin/bash

# RSDKv2 Android Port - Setup Verification Script
# This script checks if everything is ready to build

echo "================================================"
echo "  RSDKv2 Android Port - Setup Verification"
echo "================================================"
echo ""

ERRORS=0
WARNINGS=0

# Color codes
RED='\033[0;31m'
YELLOW='\033[1;33m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

error() {
    echo -e "${RED}✗ ERROR: $1${NC}"
    ERRORS=$((ERRORS + 1))
}

warning() {
    echo -e "${YELLOW}⚠ WARNING: $1${NC}"
    WARNINGS=$((WARNINGS + 1))
}

success() {
    echo -e "${GREEN}✓ $1${NC}"
}

info() {
    echo "  $1"
}

# Check if we're in the right directory
if [ ! -d "android" ] || [ ! -d "RSDKv2" ]; then
    error "Not in RSDKv2-Decompilation root directory"
    echo ""
    echo "Please run this script from the root of the RSDKv2-Decompilation repository"
    exit 1
fi

echo "Checking project structure..."
echo ""

# Check RSDKv2 source files
echo "[1] RSDKv2 Source Files"
if [ -d "RSDKv2" ]; then
    CPP_COUNT=$(ls RSDKv2/*.cpp 2>/dev/null | wc -l)
    if [ "$CPP_COUNT" -ge 20 ]; then
        success "Found $CPP_COUNT C++ source files"
    else
        error "Only found $CPP_COUNT C++ files (expected ~23)"
    fi
else
    error "RSDKv2 directory not found"
fi
echo ""

# Check Android project structure
echo "[2] Android Project Structure"
if [ -f "android/app/build.gradle" ]; then
    success "Android app build.gradle exists"
else
    error "android/app/build.gradle not found"
fi

if [ -f "android/app/src/main/AndroidManifest.xml" ]; then
    success "AndroidManifest.xml exists"
else
    error "AndroidManifest.xml not found"
fi

if [ -f "android/app/src/main/java/com/rsdkmodding/rsdkv2/RSDKv2Activity.java" ]; then
    success "RSDKv2Activity.java exists"
else
    error "RSDKv2Activity.java not found"
fi

if [ -f "android/app/src/main/cpp/CMakeLists.txt" ]; then
    success "Native CMakeLists.txt exists"
else
    error "Native CMakeLists.txt not found"
fi
echo ""

# Check for local.properties
echo "[3] Build Configuration"
if [ -f "android/local.properties" ]; then
    success "local.properties exists"
    
    # Check SDK path
    if grep -q "sdk.dir" android/local.properties; then
        SDK_PATH=$(grep "sdk.dir" android/local.properties | cut -d'=' -f2 | tr -d '\r' | sed 's/\\:/:/g')
        if [ -d "$SDK_PATH" ] || [ -d "${SDK_PATH//\\/}" ]; then
            success "SDK path configured"
        else
            warning "SDK path set but directory doesn't exist: $SDK_PATH"
        fi
    else
        error "SDK path not set in local.properties"
    fi
else
    warning "local.properties not found (copy from template)"
    info "Run: cp android/local.properties.template android/local.properties"
fi
echo ""

# Check SDL2
echo "[4] SDL2 Setup"
SDL2_DIR="android/app/src/main/cpp/SDL2-2.30.4"
if [ -d "$SDL2_DIR" ]; then
    success "SDL2-2.30.4 directory exists"
    
    # Check for built library
    SDL2_LIB="$SDL2_DIR/build/arm64-v8a/libSDL2.a"
    if [ -f "$SDL2_LIB" ]; then
        SIZE=$(du -h "$SDL2_LIB" | cut -f1)
        success "SDL2 library built (size: $SIZE)"
    else
        warning "SDL2 not built yet"
        info "Run: ./setup_sdl2.sh"
    fi
else
    warning "SDL2 not downloaded/extracted"
    info "Run: ./setup_sdl2.sh"
fi

# Check for SDL2 Java classes
if [ -d "android/app/src/main/java/org/libsdl" ]; then
    success "SDL2 Java classes found"
else
    warning "SDL2 Java classes not copied"
    info "After extracting SDL2, run:"
    info "cp -r $SDL2_DIR/android-project/app/src/main/java/org android/app/src/main/java/"
fi
echo ""

# Check for Android SDK/NDK
echo "[5] Android SDK/NDK"
if [ -n "$ANDROID_HOME" ] || [ -n "$ANDROID_SDK_ROOT" ]; then
    success "Android SDK environment variable set"
    SDK_ROOT="${ANDROID_HOME:-$ANDROID_SDK_ROOT}"
    
    # Check for NDK
    if [ -d "$SDK_ROOT/ndk/22.1.7171670" ]; then
        success "NDK 22.1.7171670 found"
    else
        warning "NDK 22.1.7171670 not found in SDK"
        info "Install via Android Studio SDK Manager"
    fi
    
    # Check for CMake
    if [ -d "$SDK_ROOT/cmake" ]; then
        CMAKE_VER=$(ls -1 "$SDK_ROOT/cmake" 2>/dev/null | head -1)
        if [ -n "$CMAKE_VER" ]; then
            success "CMake found: $CMAKE_VER"
        fi
    else
        warning "CMake not found in SDK"
    fi
else
    warning "ANDROID_HOME or ANDROID_SDK_ROOT not set"
    info "Set in local.properties or as environment variable"
fi
echo ""

# Check Gradle
echo "[6] Gradle Wrapper"
if [ -f "android/gradlew" ]; then
    success "Gradle wrapper exists"
    if [ -x "android/gradlew" ]; then
        success "Gradle wrapper is executable"
    else
        warning "Gradle wrapper not executable"
        info "Run: chmod +x android/gradlew"
    fi
else
    error "Gradle wrapper not found"
fi
echo ""

# Check documentation
echo "[7] Documentation"
DOCS=(
    "QUICKSTART_ANDROID.md"
    "android/README.md"
    "BUILD_CHECKLIST.md"
    "ANDROID_PORT_SUMMARY.md"
)
DOC_COUNT=0
for doc in "${DOCS[@]}"; do
    if [ -f "$doc" ]; then
        DOC_COUNT=$((DOC_COUNT + 1))
    fi
done
if [ $DOC_COUNT -eq ${#DOCS[@]} ]; then
    success "All documentation files present ($DOC_COUNT/${#DOCS[@]})"
else
    warning "Some documentation missing ($DOC_COUNT/${#DOCS[@]})"
fi
echo ""

# Summary
echo "================================================"
echo "  Summary"
echo "================================================"
echo ""

if [ $ERRORS -eq 0 ] && [ $WARNINGS -eq 0 ]; then
    echo -e "${GREEN}✓ Everything looks good! Ready to build.${NC}"
    echo ""
    echo "Next steps:"
    echo "  1. Run: ./setup_sdl2.sh (if SDL2 not built)"
    echo "  2. Run: cd android && ./gradlew assembleDebug"
    echo ""
elif [ $ERRORS -eq 0 ]; then
    echo -e "${YELLOW}⚠ Setup mostly complete with $WARNINGS warning(s)${NC}"
    echo ""
    echo "You can probably still build, but review the warnings above."
    echo ""
else
    echo -e "${RED}✗ Found $ERRORS error(s) and $WARNINGS warning(s)${NC}"
    echo ""
    echo "Please fix the errors above before building."
    echo ""
fi

echo "For detailed instructions, see:"
echo "  - QUICKSTART_ANDROID.md (30-min quick setup)"
echo "  - BUILD_CHECKLIST.md (step-by-step guide)"
echo "  - android/README.md (complete documentation)"
echo ""

exit $ERRORS
