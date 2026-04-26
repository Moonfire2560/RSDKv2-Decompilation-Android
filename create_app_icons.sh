#!/bin/bash

# Check if an image file was provided
if [ $# -eq 0 ]; then
    echo "ERROR: No image file provided!"
    echo "Usage: Drag and drop an image onto this script, or run:"
    echo "  ./create_app_icons.sh <image_file>"
    read -p "Press Enter to exit..."
    exit 1
fi

IMAGE_FILE="$1"

# Check if file exists
if [ ! -f "$IMAGE_FILE" ]; then
    echo "ERROR: File '$IMAGE_FILE' not found!"
    read -p "Press Enter to exit..."
    exit 1
fi

# Check if ImageMagick is available
if ! command -v magick &> /dev/null && ! command -v convert &> /dev/null; then
    echo "ERROR: ImageMagick is not installed!"
    echo "Download from: https://imagemagick.org/script/download.php#windows"
    read -p "Press Enter to exit..."
    exit 1
fi

# Determine which command to use
if command -v magick &> /dev/null; then
    MAGICK_CMD="magick"
else
    MAGICK_CMD="convert"
fi

echo "Creating Android app icons from: $IMAGE_FILE"
echo ""

# Get the directory where this script is located
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
RES_DIR="$SCRIPT_DIR/android/app/src/main/res"

# Define sizes for each density
declare -A sizes=(
    ["mipmap-mdpi"]=48
    ["mipmap-hdpi"]=72
    ["mipmap-xhdpi"]=96
    ["mipmap-xxhdpi"]=144
    ["mipmap-xxxhdpi"]=192
)

# Create icons for each density
SUCCESS=true
for folder in "${!sizes[@]}"; do
    size=${sizes[$folder]}
    output_path="$RES_DIR/$folder/ic_launcher.png"
    
    $MAGICK_CMD "$IMAGE_FILE" -resize ${size}x${size} "$output_path"
    
    if [ $? -eq 0 ]; then
        echo "✓ Created $folder/ic_launcher.png (${size}x${size})"
    else
        echo "✗ Failed to create $folder/ic_launcher.png"
        SUCCESS=false
    fi
done

echo ""
if [ "$SUCCESS" = true ]; then
    echo "SUCCESS! App icons have been created."
    echo "Now rebuild your APK to see the new icon."
else
    echo "ERROR: Some icons failed to create!"
fi

read -p "Press Enter to exit..."
