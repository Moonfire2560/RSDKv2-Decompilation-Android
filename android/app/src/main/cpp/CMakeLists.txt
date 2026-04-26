cmake_minimum_required(VERSION 3.22.1)
project(RSDKv2)
# Enable C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Options
option(RETRO_MOD_LOADER "Enables or disables the mod loader." ON)
option(RETRO_USE_ORIGINAL_CODE "Removes any change that differs from the original code." OFF)
# Get the path to the RSDKv2-Decompilation root
set(RSDK_ROOT "C:/Github/RSDKv2-Decompilation-Android")
# SDL2 Configuration
set(SDL2_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SDL2-2.30.4")
set(SDL2_INCLUDE_DIR "${SDL2_DIR}/include")
# libogg source files
set(OGG_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libogg-1.3.5")
add_library(libogg STATIC
    ${OGG_DIR}/src/bitwise.c
    ${OGG_DIR}/src/framing.c
)
target_include_directories(libogg PUBLIC ${OGG_DIR}/include)
# libvorbis source files
set(VORBIS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libvorbis-1.3.7")
add_library(libvorbis STATIC
    ${VORBIS_DIR}/lib/analysis.c
    ${VORBIS_DIR}/lib/bitrate.c
    ${VORBIS_DIR}/lib/block.c
    ${VORBIS_DIR}/lib/codebook.c
    ${VORBIS_DIR}/lib/envelope.c
    ${VORBIS_DIR}/lib/floor0.c
    ${VORBIS_DIR}/lib/floor1.c
    ${VORBIS_DIR}/lib/info.c
    ${VORBIS_DIR}/lib/lookup.c
    ${VORBIS_DIR}/lib/lpc.c
    ${VORBIS_DIR}/lib/lsp.c
    ${VORBIS_DIR}/lib/mapping0.c
    ${VORBIS_DIR}/lib/mdct.c
    ${VORBIS_DIR}/lib/psy.c
    ${VORBIS_DIR}/lib/registry.c
    ${VORBIS_DIR}/lib/res0.c
    ${VORBIS_DIR}/lib/sharedbook.c
    ${VORBIS_DIR}/lib/smallft.c
    ${VORBIS_DIR}/lib/synthesis.c
    ${VORBIS_DIR}/lib/vorbisfile.c
    ${VORBIS_DIR}/lib/window.c
)
target_include_directories(libvorbis PUBLIC
    ${VORBIS_DIR}/include
    ${VORBIS_DIR}/lib
    ${OGG_DIR}/include
)
target_link_libraries(libvorbis libogg)
# Source files from the original project
set(RETRO_FILES
    ${RSDK_ROOT}/RSDKv2/Animation.cpp
    ${RSDK_ROOT}/RSDKv2/Audio.cpp
    ${RSDK_ROOT}/RSDKv2/Collision.cpp
    ${RSDK_ROOT}/RSDKv2/Debug.cpp
    ${RSDK_ROOT}/RSDKv2/Drawing.cpp
    ${RSDK_ROOT}/RSDKv2/Ini.cpp
    ${RSDK_ROOT}/RSDKv2/Input.cpp
    ${RSDK_ROOT}/RSDKv2/fcaseopen.c
    ${RSDK_ROOT}/RSDKv2/main.cpp
    ${RSDK_ROOT}/RSDKv2/Math.cpp
    ${RSDK_ROOT}/RSDKv2/ModAPI.cpp
    ${RSDK_ROOT}/RSDKv2/Object.cpp
    ${RSDK_ROOT}/RSDKv2/Palette.cpp
    ${RSDK_ROOT}/RSDKv2/Player.cpp
    ${RSDK_ROOT}/RSDKv2/Reader.cpp
    ${RSDK_ROOT}/RSDKv2/RetroEngine.cpp
    ${RSDK_ROOT}/RSDKv2/Scene.cpp
    ${RSDK_ROOT}/RSDKv2/Script.cpp
    ${RSDK_ROOT}/RSDKv2/Sprite.cpp
    ${RSDK_ROOT}/RSDKv2/String.cpp
    ${RSDK_ROOT}/RSDKv2/Text.cpp
    ${RSDK_ROOT}/RSDKv2/Userdata.cpp
    ${RSDK_ROOT}/RSDKv2/Video.cpp
)
# Create main shared library
add_library(RSDKv2 SHARED ${RETRO_FILES})
# Include directories
target_include_directories(RSDKv2 PRIVATE
    ${RSDK_ROOT}/RSDKv2/
    ${SDL2_INCLUDE_DIR}
    ${OGG_DIR}/include
    ${VORBIS_DIR}/include
)
# Compile definitions
target_compile_definitions(RSDKv2 PRIVATE
    RETRO_USE_MOD_LOADER=$<BOOL:${RETRO_MOD_LOADER}>
    FORCE_CASE_INSENSITIVE=1
    RETRO_USE_ORIGINAL_CODE=$<BOOL:${RETRO_USE_ORIGINAL_CODE}>
    BASE_PATH="/sdcard/RSDK/v2"
    RETRO_PLATFORM=5
)
# Find and link required libraries
find_library(log-lib log)
find_library(android-lib android)
# SDL2 static library
add_library(SDL2 STATIC IMPORTED)
set_target_properties(SDL2 PROPERTIES
    IMPORTED_LOCATION "${SDL2_DIR}/build/${ANDROID_ABI}/libSDL2.a"
)
# cpufeatures library (only needed for armeabi-v7a)
if(ANDROID_ABI STREQUAL "armeabi-v7a")
    add_library(cpufeatures STATIC IMPORTED)
    set_target_properties(cpufeatures PROPERTIES
        IMPORTED_LOCATION "${SDL2_DIR}/obj/local/${ANDROID_ABI}/libcpufeatures.a"
    )
    set(CPUFEATURES_LIB cpufeatures)
else()
    set(CPUFEATURES_LIB "")
endif()
# Link libraries
target_link_libraries(RSDKv2
    SDL2
    ${CPUFEATURES_LIB}
    libvorbis
    libogg
    ${log-lib}
    ${android-lib}
    GLESv1_CM
    GLESv2
    OpenSLES
    dl
    m
)