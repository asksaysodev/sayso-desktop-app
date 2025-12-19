#!/usr/bin/env bash
set -Eeuo pipefail

echo "🔧 Complete FFmpeg dependency fix (simplified approach)..."

FFMPEG_PATH="$(cd "$(dirname "$0")/../electron/bin" && pwd)/ffmpeg"
LIBS_DIR="$(cd "$(dirname "$0")/../electron/bin" && pwd)/lib"

# Backup
if [ ! -f "${FFMPEG_PATH}.backup-original" ]; then
    cp "$FFMPEG_PATH" "${FFMPEG_PATH}.backup-original"
fi

mkdir -p "$LIBS_DIR"

# Get ALL dependencies from both architectures
echo "Collecting all dependencies..."
ALL_PATHS=$(otool -L "$FFMPEG_PATH" | grep -E "(Cellar|homebrew|/usr/local)" | awk '{print $1}' | sed 's/^[[:space:]]*//' | sed 's/:$//' | sort -u)

# Copy all libraries first
echo "Copying libraries..."
for lib_path in $ALL_PATHS; do
    if [ -f "$lib_path" ]; then
        lib_name=$(basename "$lib_path")
        if [ ! -f "$LIBS_DIR/$lib_name" ]; then
            echo "  Copying $lib_name..."
            cp "$lib_path" "$LIBS_DIR/$lib_name"
            install_name_tool -id "@rpath/$lib_name" "$LIBS_DIR/$lib_name" 2>/dev/null || true
        fi
    fi
done

# Fix FFmpeg binary
echo "Fixing FFmpeg binary paths..."
for lib_path in $ALL_PATHS; do
    if [ -f "$lib_path" ]; then
        lib_name=$(basename "$lib_path")
        install_name_tool -change "$lib_path" "@rpath/$lib_name" "$FFMPEG_PATH" 2>/dev/null || true
    fi
done

# Add rpath (use @loader_path which is relative to the binary itself)
install_name_tool -add_rpath "@loader_path/lib" "$FFMPEG_PATH" 2>/dev/null || true

# Fix libraries recursively (multiple passes until no more changes)
echo "Fixing library dependencies recursively..."
MAX_PASSES=10
PASS=1
CHANGED=true

while [ "$PASS" -le "$MAX_PASSES" ] && [ "$CHANGED" = true ]; do
    CHANGED=false
    echo "  Pass $PASS..."
    
    for lib_file in "$LIBS_DIR"/*.dylib; do
        [ -f "$lib_file" ] || continue
        lib_deps=$(otool -L "$lib_file" 2>/dev/null | grep -E "(Cellar|homebrew|/usr/local)" | awk '{print $1}' | sed 's/^[[:space:]]*//' | sed 's/:$//' | grep -v "^$")
        
        for dep in $lib_deps; do
            if [ -f "$dep" ]; then
                dep_name=$(basename "$dep")
                
                # Copy if missing
                if [ ! -f "$LIBS_DIR/$dep_name" ]; then
                    cp "$dep" "$LIBS_DIR/$dep_name"
                    install_name_tool -id "@rpath/$dep_name" "$LIBS_DIR/$dep_name" 2>/dev/null || true
                    CHANGED=true
                fi
                
                # Fix reference (always try, in case it wasn't fixed before)
                if install_name_tool -change "$dep" "@rpath/$dep_name" "$lib_file" 2>/dev/null; then
                    CHANGED=true
                fi
            fi
        done
    done
    
    PASS=$((PASS + 1))
done

echo "Recursive fix completed after $((PASS - 1)) passes"

echo "✅ Complete fix finished!"
echo "Libraries: $(ls -1 "$LIBS_DIR"/*.dylib 2>/dev/null | wc -l | xargs)"
echo "Remaining deps: $(otool -L "$FFMPEG_PATH" | grep -E "(Cellar|homebrew|/usr/local)" | wc -l | xargs)"

