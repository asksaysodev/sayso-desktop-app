#!/usr/bin/env bash
set -Eeuo pipefail

echo "🔍 Verifying FFmpeg static binary..."

FFMPEG_PATH="${1:-client/electron/bin/ffmpeg}"

if [ ! -f "$FFMPEG_PATH" ]; then
    echo "❌ Error: FFmpeg binary not found at: $FFMPEG_PATH"
    exit 1
fi

echo "📁 Binary location: $FFMPEG_PATH"
echo ""

# Check file type
echo "📋 File type:"
file "$FFMPEG_PATH"
echo ""

# Check architecture
echo "🏗️  Architecture:"
lipo -info "$FFMPEG_PATH"
echo ""

# Check dynamic library dependencies
echo "📚 Dynamic library dependencies:"
DEPS=$(otool -L "$FFMPEG_PATH")
echo "$DEPS"
echo ""

# Check for problematic dependencies
PROBLEMATIC_DEPS=$(echo "$DEPS" | grep -E "(Cellar|homebrew|/usr/local/lib|/opt/homebrew/lib)" || true)

if [ -n "$PROBLEMATIC_DEPS" ]; then
    echo "❌ PROBLEM: Binary has external dependencies:"
    echo "$PROBLEMATIC_DEPS"
    exit 1
else
    echo "✅ No external dependencies found (only system frameworks)"
fi

# Test execution
echo ""
echo "🧪 Testing FFmpeg execution..."
VERSION_OUTPUT=$("$FFMPEG_PATH" -version 2>&1 | head -3 || echo "FAILED")
if [[ "$VERSION_OUTPUT" == *"FAILED"* ]] || [[ "$VERSION_OUTPUT" == *"Abort"* ]] || [[ "$VERSION_OUTPUT" == *"Library not loaded"* ]]; then
    echo "❌ FFmpeg execution failed:"
    echo "$VERSION_OUTPUT"
    exit 1
else
    echo "✅ FFmpeg executes successfully:"
    echo "$VERSION_OUTPUT"
fi

# Test avfoundation support
echo ""
echo "🎤 Testing avfoundation support..."
AVFOUNDATION_TEST=$("$FFMPEG_PATH" -f avfoundation -list_devices true -i "" 2>&1 || echo "FAILED")
if [[ "$AVFOUNDATION_TEST" == *"FAILED"* ]] || [[ "$AVFOUNDATION_TEST" == *"Abort"* ]] || [[ "$AVFOUNDATION_TEST" == *"Library not loaded"* ]]; then
    echo "❌ avfoundation test failed:"
    echo "$AVFOUNDATION_TEST" | head -5
    exit 1
else
    echo "✅ avfoundation support confirmed"
    echo "$AVFOUNDATION_TEST" | head -10
fi

echo ""
echo "✅ All checks passed! FFmpeg binary is statically linked and functional."

