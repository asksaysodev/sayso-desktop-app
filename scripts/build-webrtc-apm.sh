#!/bin/sh
set -eu

APM_TAG="v2.1"
ABSL_DIR="abseil-cpp-20240722.0"
DEPLOYMENT_TARGET="11.0"

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
TP="$ROOT/electron/native-audio/third_party/webrtc-apm"
WORK="${APM_WORK:-$HOME/.cache/sayso/webrtc-apm}"
SRC="${APM_SRC:-$WORK/src}"

mkdir -p "$WORK"
if [ ! -d "$SRC" ]; then
  git clone -q --depth 1 --branch "$APM_TAG" https://gitlab.freedesktop.org/pulseaudio/webrtc-audio-processing.git "$SRC"
fi

if ! command -v meson >/dev/null 2>&1 || ! command -v ninja >/dev/null 2>&1; then
  [ -d "$WORK/venv" ] || python3 -m venv "$WORK/venv"
  "$WORK/venv/bin/pip" -q install meson ninja
  PATH="$WORK/venv/bin:$PATH"; export PATH
fi

cat > "$WORK/x64-cross.ini" <<INI
[binaries]
c = ['clang', '-arch', 'x86_64']
cpp = ['clang++', '-arch', 'x86_64']
ar = 'ar'
strip = 'strip'
[host_machine]
system = 'darwin'
subsystem = 'macos'
kernel = 'xnu'
cpu_family = 'x86_64'
cpu = 'x86_64'
endian = 'little'
[built-in options]
c_args = ['-arch', 'x86_64', '-mmacosx-version-min=$DEPLOYMENT_TARGET']
cpp_args = ['-arch', 'x86_64', '-mmacosx-version-min=$DEPLOYMENT_TARGET']
c_link_args = ['-arch', 'x86_64']
cpp_link_args = ['-arch', 'x86_64']
INI

export MACOSX_DEPLOYMENT_TARGET="$DEPLOYMENT_TARGET"
if [ ! -f "$SRC/build/build.ninja" ]; then
  (cd "$SRC" && meson setup build --default-library=static --buildtype=release -Dcpp_std=c++17)
fi
if [ ! -f "$SRC/build-x64/build.ninja" ]; then
  (cd "$SRC" && meson setup build-x64 --cross-file "$WORK/x64-cross.ini" --default-library=static --buildtype=release -Dcpp_std=c++17)
fi
ninja -C "$SRC/build"
ninja -C "$SRC/build-x64"

VERSION="webrtc-audio-processing ${APM_TAG#v}"
for pair in "build:arm64:arm64" "build-x64:x86_64:x64"; do
  BDIR="${pair%%:*}"; rest="${pair#*:}"; CARCH="${rest%%:*}"; LIBARCH="${rest##*:}"
  B="$SRC/$BDIR"; A="$B/subprojects/$ABSL_DIR"
  OBJ="$WORK/obj-$LIBARCH"; mkdir -p "$OBJ" "$TP/lib/$LIBARCH"
  c++ -arch "$CARCH" -std=c++17 -O2 -fPIC -mmacosx-version-min="$DEPLOYMENT_TARGET" \
    -I"$SRC" -I"$SRC/webrtc" -I"$SRC/subprojects/$ABSL_DIR" -I"$TP/include" \
    -DSAYSO_APM_VERSION="\"$VERSION\"" -c "$TP/src/sayso_apm.cpp" -o "$OBJ/sayso_apm.o"
  rm -f "$TP/lib/$LIBARCH/libsayso_apm.a"
  libtool -static -o "$TP/lib/$LIBARCH/libsayso_apm.a" "$OBJ/sayso_apm.o" \
    "$B/webrtc/modules/audio_processing/libwebrtc-audio-processing-2.a" \
    "$A/libabsl_base.a" "$A/libabsl_flags.a" "$A/libabsl_container.a" "$A/libabsl_debugging.a" \
    "$A/libabsl_strings.a" "$A/libabsl_crc.a" "$A/libabsl_numeric.a" "$A/libabsl_profiling.a" \
    "$A/libabsl_hash.a" "$A/libabsl_synchronization.a" "$A/libabsl_time.a" "$A/libabsl_types.a" 2>/dev/null
done

cp "$SRC/COPYING" "$TP/COPYING"
printf '%s\nabseil %s\nbuilt %s\n' "$VERSION" "$ABSL_DIR" "$(date -u +%Y-%m-%dT%H:%M:%SZ)" > "$TP/VERSION"
echo "APM ready:"; ls -la "$TP/lib/arm64/libsayso_apm.a" "$TP/lib/x64/libsayso_apm.a"
