#!/usr/bin/env bash
set -Eeuo pipefail

echo "🔧 Bundling FFmpeg dylibs to eliminate external dependencies..."

# Configuration
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
FFMPEG_PATH="$SCRIPT_DIR/../electron/bin/ffmpeg"
LIBS_DIR="$SCRIPT_DIR/../electron/bin/lib"
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
echo -e "${GREEN}📦 Backing up existing FFmpeg...${NC}"
cp "$FFMPEG_PATH" "$BACKUP_PATH"
echo -e "${GREEN}✅ Backup created: $BACKUP_PATH${NC}"

# Create libs directory
mkdir -p "$LIBS_DIR"

# Function to get dependencies for a specific architecture
get_deps_for_arch() {
    local file=$1
    local arch=$2
    otool -L -arch "$arch" "$file" 2>/dev/null | grep -E "(Cellar|homebrew|/usr/local/lib|/usr/local/opt)" | awk '{print $1}' | sed 's/^[[:space:]]*//' | sed 's/:$//' | sort -u
}

# Function to copy and fix a dylib
copy_and_fix_dylib() {
    local src=$1
    local dest=$2
    
    if [ ! -f "$src" ]; then
        return 1
    fi
    
    # Skip if already copied (but still need to fix its dependencies)
    local is_new=false
    if [ ! -f "$dest" ]; then
        is_new=true
        echo "  📋 Copying $(basename "$src")..."
        cp "$src" "$dest"
    fi
    
    # Fix the dylib's ID to use @rpath
    local dylib_name=$(basename "$dest")
    install_name_tool -id "@rpath/$dylib_name" "$dest" 2>/dev/null || true
    
    # Get all dependencies (check both architectures for universal binaries)
    local all_deps=""
    if lipo -info "$dest" 2>/dev/null | grep -q "universal"; then
        # Universal binary - check both architectures
        local arm64_deps=$(get_deps_for_arch "$dest" "arm64")
        local x86_64_deps=$(get_deps_for_arch "$dest" "x86_64")
        all_deps=$(echo -e "$arm64_deps\n$x86_64_deps" | sort -u)
    else
        # Single architecture
        local arch=$(lipo -info "$dest" 2>/dev/null | grep -oE "(x86_64|arm64)" | head -1 || echo "arm64")
        all_deps=$(get_deps_for_arch "$dest" "$arch")
    fi
    
    # Recursively fix dependencies
    for dep in $all_deps; do
        if [ -f "$dep" ]; then
            local dep_name=$(basename "$dep")
            local dep_dest="$LIBS_DIR/$dep_name"
            
            # Copy the dependency if not already there
            copy_and_fix_dylib "$dep" "$dep_dest"
            
            # Update reference in current dylib (for all architectures if universal)
            install_name_tool -change "$dep" "@rpath/$dep_name" "$dest" 2>/dev/null || true
        fi
    done
}

# Get dependencies for both architectures
echo -e "${GREEN}🔍 Analyzing dependencies...${NC}"
ARM64_DEPS=$(get_deps_for_arch "$FFMPEG_PATH" "arm64")
X86_64_DEPS=$(get_deps_for_arch "$FFMPEG_PATH" "x86_64")
ALL_DEPS=$(echo -e "$ARM64_DEPS\n$X86_64_DEPS" | sort -u)

if [ -z "$ALL_DEPS" ]; then
    echo -e "${GREEN}✅ No external dependencies found - binary is already self-contained!${NC}"
    exit 0
fi

echo -e "${YELLOW}Found dependencies to bundle:${NC}"
echo "$ALL_DEPS" | sed 's/^/  - /'
echo ""

# Copy and fix all dependencies recursively
echo -e "${GREEN}📦 Copying and fixing dependencies...${NC}"
for dep in $ALL_DEPS; do
    if [ -f "$dep" ]; then
        dep_name=$(basename "$dep")
        copy_and_fix_dylib "$dep" "$LIBS_DIR/$dep_name"
    fi
done

# Update FFmpeg binary to use @rpath for all dependencies
echo -e "${GREEN}🔧 Updating FFmpeg binary to use @rpath...${NC}"
for dep in $ALL_DEPS; do
    if [ -f "$dep" ]; then
        dep_name=$(basename "$dep")
        # Update for both architectures - need to specify arch for universal binary
        install_name_tool -change "$dep" "@rpath/$dep_name" "$FFMPEG_PATH" 2>/dev/null || true
    fi
done

# Also fix any remaining /usr/local/opt paths that might have been missed
echo -e "${GREEN}🔧 Fixing remaining /usr/local/opt paths...${NC}"
REMAINING_LOCAL_OPTS=$(otool -L "$FFMPEG_PATH" | grep "/usr/local/opt" | awk '{print $1}' | sed 's/^[[:space:]]*//' | sed 's/:$//' | sort -u)
for dep in $REMAINING_LOCAL_OPTS; do
    if [ -f "$dep" ]; then
        dep_name=$(basename "$dep")
        # Check if we already have this dylib bundled
        if [ -f "$LIBS_DIR/$dep_name" ]; then
            install_name_tool -change "$dep" "@rpath/$dep_name" "$FFMPEG_PATH" 2>/dev/null || true
        else
            # Copy it if we don't have it
            copy_and_fix_dylib "$dep" "$LIBS_DIR/$dep_name"
            install_name_tool -change "$dep" "@rpath/$dep_name" "$FFMPEG_PATH" 2>/dev/null || true
        fi
    fi
done

# Add @rpath to binary (relative to the binary location)
install_name_tool -add_rpath "@executable_path/lib" "$FFMPEG_PATH" 2>/dev/null || true

# Verify the fix
echo ""
echo -e "${GREEN}✅ Verification:${NC}"
echo "Checking for remaining external dependencies..."
REMAINING_DEPS=$(otool -L "$FFMPEG_PATH" | grep -E "(Cellar|homebrew|/usr/local/lib|/usr/local/opt)" || true)
if [ -n "$REMAINING_DEPS" ]; then
    echo -e "${YELLOW}⚠️  Some dependencies may still need manual fixing:${NC}"
    echo "$REMAINING_DEPS"
else
    echo -e "${GREEN}✅ All external dependencies have been replaced with @rpath!${NC}"
fi

echo ""
echo -e "${GREEN}✅ Dependencies bundled successfully!${NC}"
echo -e "${GREEN}📁 Libraries are in: $LIBS_DIR${NC}"
echo -e "${GREEN}📁 FFmpeg binary: $FFMPEG_PATH${NC}"
echo ""
echo -e "${YELLOW}Next steps:${NC}"
echo "1. Update package.json to bundle the lib/ directory"
echo "2. Test the binary on a clean macOS system"
echo "3. Run: npm run verify-ffmpeg"

