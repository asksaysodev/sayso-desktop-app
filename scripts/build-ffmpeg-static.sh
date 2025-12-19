#!/usr/bin/env bash
set -Eeuo pipefail

echo "🔧 Building/Downloading static FFmpeg universal binary..."

# Configuration
FFMPEG_VERSION="8.0.1"
BUILD_DIR="/tmp/ffmpeg-static-build"
OUTPUT_DIR="$(cd "$(dirname "$0")/../electron/bin" && pwd)"
OUTPUT_FILE="$OUTPUT_DIR/ffmpeg"

# Colors
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'

# Cleanup function
cleanup() {
    if [ -d "$BUILD_DIR" ]; then
        echo -e "${YELLOW}Cleaning up build directory...${NC}"
        rm -rf "$BUILD_DIR"
    fi
}
trap cleanup EXIT

# Try to download pre-built static binary first (much faster)
echo -e "${GREEN}Attempting to download pre-built static FFmpeg binary...${NC}"
echo -e "${YELLOW}Note: If download fails, will build from source (this takes 30+ minutes)${NC}"

# Try BtbN FFmpeg builds (popular static builds)
DOWNLOAD_URL=""
if [ -z "${DOWNLOAD_URL:-}" ]; then
    echo -e "${YELLOW}No pre-built binary URL configured. Building from source...${NC}"
    BUILD_FROM_SOURCE=true
else
    BUILD_FROM_SOURCE=false
    # Download logic would go here if we had a reliable source
fi

# Create build directory
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

if [ "$BUILD_FROM_SOURCE" = true ]; then
    # Download FFmpeg source if not already present
    if [ ! -d "ffmpeg-$FFMPEG_VERSION" ]; then
        echo -e "${GREEN}Downloading FFmpeg source (version $FFMPEG_VERSION)...${NC}"
        curl -L "https://ffmpeg.org/releases/ffmpeg-$FFMPEG_VERSION.tar.xz" -o "ffmpeg-$FFMPEG_VERSION.tar.xz"
        tar -xf "ffmpeg-$FFMPEG_VERSION.tar.xz"
    fi
    
    cd "ffmpeg-$FFMPEG_VERSION"
fi

# Build configuration - minimal codecs for avfoundation audio capture
CONFIGURE_FLAGS=(
    --prefix="$BUILD_DIR/install"
    --enable-static
    --disable-shared
    --disable-debug
    --disable-doc
    --disable-ffplay
    --disable-ffprobe
    --enable-avfoundation
    --enable-audiotoolbox
    --enable-videotoolbox
    --enable-pthreads
    --enable-version3
    --cc=clang
    --host-cflags="-arch x86_64 -arch arm64"
    --host-ldflags="-arch x86_64 -arch arm64"
    --disable-encoders
    --disable-decoders
    --enable-encoder=pcm_s16le
    --enable-decoder=pcm_s16le
    --enable-encoder=libmp3lame
    --enable-decoder=mp3
    --enable-libmp3lame
    --enable-filter=aresample
    --disable-network
    --disable-protocols
    --enable-protocol=file
    --disable-demuxers
    --enable-demuxer=mov
    --enable-demuxer=mp3
    --enable-demuxer=wav
    --disable-muxers
    --enable-muxer=mp3
    --enable-muxer=wav
    --enable-muxer=caf
    --enable-muxer=segment
    --disable-parsers
    --enable-parser=mpegaudio
    --enable-parser=pcm
    --disable-bsfs
    --disable-indevs
    --enable-indev=avfoundation
    --disable-outdevs
    --disable-filters
    --enable-filter=aresample
    --disable-postproc
    --disable-avfilter
    --disable-swscale
    --disable-swresample
)

# Function to build for a specific architecture
build_for_arch() {
    local arch=$1
    local build_dir="$BUILD_DIR/build-$arch"
    
    echo -e "${GREEN}Building FFmpeg for $arch...${NC}"
    
    mkdir -p "$build_dir"
    cd "$BUILD_DIR/ffmpeg-$FFMPEG_VERSION"
    
    # Clean previous build
    make distclean 2>/dev/null || true
    
    # Configure for specific architecture
    CFLAGS="-arch $arch -mmacosx-version-min=10.15"
    LDFLAGS="-arch $arch -mmacosx-version-min=10.15"
    
    ./configure \
        "${CONFIGURE_FLAGS[@]}" \
        --prefix="$build_dir/install" \
        --extra-cflags="$CFLAGS" \
        --extra-ldflags="$LDFLAGS" \
        --arch=$arch \
        --target-os=darwin \
        2>&1 | tee "$build_dir/configure.log"
    
    # Build
    echo -e "${GREEN}Compiling for $arch (this may take a while)...${NC}"
    make -j$(sysctl -n hw.ncpu) 2>&1 | tee "$build_dir/build.log"
    
    # Install
    make install 2>&1 | tee "$build_dir/install.log"
    
    echo -e "${GREEN}✅ Build complete for $arch${NC}"
}

# Detect current architecture
CURRENT_ARCH=$(uname -m)

# Build for both architectures
if [ "$CURRENT_ARCH" = "arm64" ]; then
    # On Apple Silicon, build arm64 natively, x86_64 via Rosetta
    echo -e "${GREEN}Detected Apple Silicon Mac${NC}"
    build_for_arch "arm64"
    build_for_arch "x86_64"
elif [ "$CURRENT_ARCH" = "x86_64" ]; then
    # On Intel, build x86_64 natively, arm64 via cross-compilation (if possible)
    echo -e "${GREEN}Detected Intel Mac${NC}"
    build_for_arch "x86_64"
    echo -e "${YELLOW}Note: ARM64 build on Intel Mac requires cross-compilation setup${NC}"
    echo -e "${YELLOW}You may need to build ARM64 on an Apple Silicon Mac${NC}"
    # For now, we'll try to build arm64 anyway
    build_for_arch "arm64" || echo -e "${RED}ARM64 build failed - you may need to build on Apple Silicon Mac${NC}"
else
    echo -e "${RED}Unknown architecture: $CURRENT_ARCH${NC}"
    exit 1
fi

# Create universal binary using lipo
echo -e "${GREEN}Creating universal binary...${NC}"

ARM64_BINARY="$BUILD_DIR/build-arm64/install/bin/ffmpeg"
X86_64_BINARY="$BUILD_DIR/build-x86_64/install/bin/ffmpeg"

if [ ! -f "$ARM64_BINARY" ] || [ ! -f "$X86_64_BINARY" ]; then
    echo -e "${RED}❌ Error: One or both architecture builds failed${NC}"
    echo "ARM64 binary exists: $([ -f "$ARM64_BINARY" ] && echo "yes" || echo "no")"
    echo "x86_64 binary exists: $([ -f "$X86_64_BINARY" ] && echo "yes" || echo "no")"
    exit 1
fi

# Backup existing binary
if [ -f "$OUTPUT_FILE" ]; then
    echo -e "${YELLOW}Backing up existing FFmpeg binary...${NC}"
    mv "$OUTPUT_FILE" "$OUTPUT_FILE.backup-$(date +%Y%m%d-%H%M%S)"
fi

# Create universal binary
lipo -create "$ARM64_BINARY" "$X86_64_BINARY" -output "$OUTPUT_FILE"

# Verify the universal binary
echo -e "${GREEN}Verifying universal binary...${NC}"
file "$OUTPUT_FILE"
echo ""
echo -e "${GREEN}Checking for dynamic library dependencies...${NC}"
otool -L "$OUTPUT_FILE" | head -20

# Check if it's truly static (should only show system frameworks)
DYNAMIC_DEPS=$(otool -L "$OUTPUT_FILE" | grep -E "(Cellar|homebrew|/usr/local/lib)" || true)
if [ -n "$DYNAMIC_DEPS" ]; then
    echo -e "${RED}⚠️  Warning: Binary still has dynamic dependencies:${NC}"
    echo "$DYNAMIC_DEPS"
else
    echo -e "${GREEN}✅ Binary appears to be statically linked (only system frameworks)${NC}"
fi

# Test the binary
echo -e "${GREEN}Testing FFmpeg binary...${NC}"
"$OUTPUT_FILE" -version 2>&1 | head -3

echo ""
echo -e "${GREEN}✅ Static FFmpeg universal binary built successfully!${NC}"
echo -e "${GREEN}Output: $OUTPUT_FILE${NC}"

