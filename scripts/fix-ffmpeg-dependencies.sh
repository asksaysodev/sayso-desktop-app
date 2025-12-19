#!/usr/bin/env bash
set -Eeuo pipefail

echo "🔧 Fixing FFmpeg dependencies by bundling dylibs..."

# Configuration
FFMPEG_PATH="$(cd "$(dirname "$0")/../electron/bin" && pwd)/ffmpeg"
LIBS_DIR="$(cd "$(dirname "$0")/../electron/bin" && pwd)/lib"
BACKUP_PATH="${FFMPEG_PATH}.backup-$(date +%Y%m%d-%H%M%S)"

# Colors
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'

if [ ! -f "$FFMPEG_PATH" ]; then
    echo -e "${RED}❌ FFmpeg binary not found at: $FFMPEG_PATH${NC}"
    exit 1
fi

# Backup existing binary
echo -e "${GREEN}Backing up existing FFmpeg...${NC}"
cp "$FFMPEG_PATH" "$BACKUP_PATH"

# Create libs directory
mkdir -p "$LIBS_DIR"

# Get list of dependencies that need to be bundled
echo -e "${GREEN}Analyzing dependencies...${NC}"
DEPS=$(otool -L "$FFMPEG_PATH" | grep -E "(Cellar|homebrew|/usr/local/lib)" | awk '{print $1}' | sed 's/^[[:space:]]*//' | sed 's/:$//')

if [ -z "$DEPS" ]; then
    echo -e "${GREEN}✅ No external dependencies found - binary is already static or fixed!${NC}"
    exit 0
fi

echo -e "${YELLOW}Found dependencies to bundle:${NC}"
echo "$DEPS"
echo ""

# Copy dependencies to libs directory
echo -e "${GREEN}Copying dependencies to libs directory...${NC}"
for dep in $DEPS; do
    if [ -f "$dep" ]; then
        dep_name=$(basename "$dep")
        echo "  Copying $dep_name..."
        cp "$dep" "$LIBS_DIR/"
        
        # Fix the dependency's own dependencies
        install_name_tool -id "@rpath/$dep_name" "$LIBS_DIR/$dep_name" 2>/dev/null || true
        
        # Update dependencies of this dylib
        dep_deps=$(otool -L "$LIBS_DIR/$dep_name" | grep -E "(Cellar|homebrew|/usr/local/lib)" | awk '{print $1}' | sed 's/^[[:space:]]*//' | sed 's/:$//')
        for dep_dep in $dep_deps; do
            if [ -f "$dep_dep" ]; then
                dep_dep_name=$(basename "$dep_dep")
                install_name_tool -change "$dep_dep" "@rpath/$dep_dep_name" "$LIBS_DIR/$dep_name" 2>/dev/null || true
            fi
        done
    fi
done

# Update FFmpeg binary to use @rpath
echo -e "${GREEN}Updating FFmpeg binary to use @rpath...${NC}"
for dep in $DEPS; do
    if [ -f "$dep" ]; then
        dep_name=$(basename "$dep")
        install_name_tool -change "$dep" "@rpath/$dep_name" "$FFMPEG_PATH" 2>/dev/null || true
    fi
done

# Add @rpath to binary
install_name_tool -add_rpath "@executable_path/../lib" "$FFMPEG_PATH" 2>/dev/null || true

echo -e "${GREEN}✅ Dependencies bundled and paths updated!${NC}"
echo -e "${GREEN}Libraries are in: $LIBS_DIR${NC}"

