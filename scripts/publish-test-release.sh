#!/usr/bin/env bash
# Creates a test staging release by reusing existing binaries under a bumped version.
# Usage: ./scripts/publish-test-release.sh <from-version> <to-version>
# Example: ./scripts/publish-test-release.sh 1.1.2 1.1.3
set -Eeuo pipefail

FROM_VERSION="${1:-}"
TO_VERSION="${2:-}"

if [[ -z "$FROM_VERSION" || -z "$TO_VERSION" ]]; then
  echo "Usage: $0 <from-version> <to-version>"
  echo "Example: $0 1.1.2 1.1.3"
  exit 1
fi

APP_NAME="Sayso [beta]"
RELEASE_DIR="release-staging"
TEST_DIR="${RELEASE_DIR}/test-release-${TO_VERSION}"
RELEASE_TAG="v${TO_VERSION}-staging"

echo "📦 Creating test release ${RELEASE_TAG} from v${FROM_VERSION} binaries..."

# Verify source files exist
SOURCE_FILES=(
  "${RELEASE_DIR}/${APP_NAME}-${FROM_VERSION}-x64-mac.zip"
  "${RELEASE_DIR}/${APP_NAME}-${FROM_VERSION}-arm64-mac.zip"
  "${RELEASE_DIR}/${APP_NAME}-${FROM_VERSION}-x64-mac.dmg"
  "${RELEASE_DIR}/${APP_NAME}-${FROM_VERSION}-arm64-mac.dmg"
  "${RELEASE_DIR}/staging-mac.yml"
)

for f in "${SOURCE_FILES[@]}"; do
  if [ ! -f "$f" ]; then
    echo "❌ Missing source file: $f"
    exit 1
  fi
done

rm -rf "$TEST_DIR"
mkdir -p "$TEST_DIR"

# Copy and rename binaries to the new version
echo "📋 Copying binaries..."
cp "${RELEASE_DIR}/${APP_NAME}-${FROM_VERSION}-x64-mac.zip"    "${TEST_DIR}/${APP_NAME}-${TO_VERSION}-x64-mac.zip"
cp "${RELEASE_DIR}/${APP_NAME}-${FROM_VERSION}-arm64-mac.zip"  "${TEST_DIR}/${APP_NAME}-${TO_VERSION}-arm64-mac.zip"
cp "${RELEASE_DIR}/${APP_NAME}-${FROM_VERSION}-x64-mac.dmg"    "${TEST_DIR}/${APP_NAME}-${TO_VERSION}-x64-mac.dmg"
cp "${RELEASE_DIR}/${APP_NAME}-${FROM_VERSION}-arm64-mac.dmg"  "${TEST_DIR}/${APP_NAME}-${TO_VERSION}-arm64-mac.dmg"

# Patch the yml: update version and filenames
echo "📝 Patching staging-mac.yml..."
sed \
  -e "s/^version: .*/version: ${TO_VERSION}/" \
  -e "s/${APP_NAME}-${FROM_VERSION}-x64-mac\.zip/${APP_NAME}-${TO_VERSION}-x64-mac.zip/g" \
  -e "s/${APP_NAME}-${FROM_VERSION}-arm64-mac\.zip/${APP_NAME}-${TO_VERSION}-arm64-mac.zip/g" \
  "${RELEASE_DIR}/staging-mac.yml" > "${TEST_DIR}/staging-mac.yml"

echo "🔍 Resulting staging-mac.yml:"
cat "${TEST_DIR}/staging-mac.yml"

# Escape brackets for gh CLI
GH_APP_NAME="${APP_NAME//\[/\\[}"
GH_APP_NAME="${GH_APP_NAME//\]/\\]}"

echo ""
echo "🚀 Creating GitHub Pre-Release draft ${RELEASE_TAG}..."
gh release create "${RELEASE_TAG}" \
  --title "${RELEASE_TAG}" \
  --prerelease \
  --draft \
  --notes "Test release ${RELEASE_TAG} (binaries from v${FROM_VERSION})" \
  "${TEST_DIR}/${GH_APP_NAME}-${TO_VERSION}-x64-mac.zip" \
  "${TEST_DIR}/${GH_APP_NAME}-${TO_VERSION}-arm64-mac.zip" \
  "${TEST_DIR}/${GH_APP_NAME}-${TO_VERSION}-x64-mac.dmg" \
  "${TEST_DIR}/${GH_APP_NAME}-${TO_VERSION}-arm64-mac.dmg" \
  "${TEST_DIR}/staging-mac.yml"

echo ""
echo "✅ Draft ${RELEASE_TAG} created — publish it on GitHub, then test the auto-updater."
echo "🧹 Temp files in ${TEST_DIR} — safe to delete after the release is published."
