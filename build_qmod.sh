#!/bin/bash

# Build script for PauseRemapper mod
# Creates a .qmod file from source

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}=== PauseRemapper Build Script ===${NC}"

# Get the directory where this script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

# Set Android NDK path (modify if your NDK is in a different location)
if [ -z "$ANDROID_NDK_HOME" ]; then
    ANDROID_NDK_HOME="$SCRIPT_DIR/android-sdk/ndk/26.1.10909125"
    echo -e "${YELLOW}ANDROID_NDK_HOME not set, using: $ANDROID_NDK_HOME${NC}"
fi

# Verify NDK exists
if [ ! -d "$ANDROID_NDK_HOME" ]; then
    echo -e "${RED}Error: Android NDK not found at $ANDROID_NDK_HOME${NC}"
    echo "Please install the Android NDK or set ANDROID_NDK_HOME environment variable"
    exit 1
fi

# Export for cmake
export ANDROID_NDK_HOME

echo -e "${GREEN}Step 1: Restoring dependencies...${NC}"
qpm restore

echo -e "${GREEN}Step 2: Configuring build...${NC}"
if [ -d "build" ]; then
    echo "Cleaning old build directory..."
    rm -rf build
fi

cmake -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo -B build .

echo -e "${GREEN}Step 3: Building...${NC}"
cmake --build ./build

echo -e "${GREEN}Step 4: Creating .qmod package...${NC}"
QMOD_FILE="PauseRemapper.qmod"

# Remove old .qmod if it exists
if [ -f "$QMOD_FILE" ]; then
    rm "$QMOD_FILE"
    echo "Removed old $QMOD_FILE"
fi

# Create the .qmod file (which is just a zip)
zip -j "$QMOD_FILE" mod.json build/libPauseRemapper.so PauseRemapper.gif

# Get file size
FILE_SIZE=$(du -h "$QMOD_FILE" | cut -f1)

echo ""
echo -e "${GREEN}=== Build Complete! ===${NC}"
echo -e "Created: ${YELLOW}$QMOD_FILE${NC} (${FILE_SIZE})"
echo -e "Location: ${YELLOW}$(pwd)/$QMOD_FILE${NC}"
