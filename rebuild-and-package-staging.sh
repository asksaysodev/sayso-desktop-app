#!/usr/bin/env bash
set -Eeuo pipefail

echo "🔧 Rebuilding and packaging Sayso Staging with notarization..."

if [ -f .env.production ]; then
  export $(cat .env.production | grep -v '^#' | xargs)
  echo "✅ Loaded environment variables from .env.production"
elif [ -f .env ]; then
  export $(cat .env | grep -v '^#' | xargs)
  echo "⚠️  Using .env (fallback)"
fi

APP_NAME="Sayso Staging"
NOTARY_PROFILE="NotaryProfile"
RELEASE_DIR="release-staging"

APP_VERSION=$(node -p "require('./package.json').version")
echo "📦 Building staging version: ${APP_VERSION}"

echo "🧹 Cleaning previous staging builds..."
rm -rf "${RELEASE_DIR}/" dist/

echo "📦 Reinstalling dependencies..."
npm install
echo "🏗️ Building the app (staging mode)..."
npm run build:electron
npm run build:staging

echo "📦 Packaging the staging app..."
npm run package:staging

# Sign the DMGs
echo "🔐 Signing staging DMGs..."
if [ -f "${RELEASE_DIR}/${APP_NAME}-${APP_VERSION}.dmg" ]; then
  codesign --sign "Developer ID Application: EXOMEND LLC (Y57SJLCC9H)" "${RELEASE_DIR}/${APP_NAME}-${APP_VERSION}.dmg"
  echo "✅ Signed Intel DMG"
fi
if [ -f "${RELEASE_DIR}/${APP_NAME}-${APP_VERSION}-arm64.dmg" ]; then
  codesign --sign "Developer ID Application: EXOMEND LLC (Y57SJLCC9H)" "${RELEASE_DIR}/${APP_NAME}-${APP_VERSION}-arm64.dmg"
  echo "✅ Signed Apple Silicon DMG"
fi

echo "🔍 Looking for .app bundles..."
APP_PATHS=($(ls -d "${RELEASE_DIR}/**/${APP_NAME}.app" 2>/dev/null || true))

if [[ ${#APP_PATHS[@]} -eq 0 ]]; then
  echo "❌ Could not find any ${APP_NAME}.app under ${RELEASE_DIR}/"
  exit 1
fi

echo "✅ Found ${#APP_PATHS[@]} app bundle(s):"
for app_path in "${APP_PATHS[@]}"; do
  echo "  - ${app_path}"
done

echo "🔍 Verifying NotaryProfile '${NOTARY_PROFILE}' is accessible..."
if ! xcrun notarytool history --keychain-profile "${NOTARY_PROFILE}" &>/dev/null; then
  echo "❌ ERROR: NotaryProfile '${NOTARY_PROFILE}' not found or not accessible!"
  exit 1
fi
echo "✅ NotaryProfile verified"

notarize_app() {
  local app_path="$1"
  local arch_name="$2"

  echo "🚀 Notarizing ${arch_name} app: ${app_path}"

  local temp_zip_path="${RELEASE_DIR}/${APP_NAME}-${arch_name}-temp.zip"
  rm -f "${temp_zip_path}"
  ditto -c -k --sequesterRsrc --keepParent "${app_path}" "${temp_zip_path}"

  xcrun notarytool submit "${temp_zip_path}" \
    --keychain-profile "${NOTARY_PROFILE}" \
    --wait --progress

  xcrun stapler staple "${app_path}"
  xcrun stapler validate "${app_path}"

  local final_zip_name=""
  if [ "$arch_name" == "Intel" ]; then
    final_zip_name="${APP_NAME}-${APP_VERSION}-mac.zip"
  elif [ "$arch_name" == "ARM64" ]; then
    final_zip_name="${APP_NAME}-${APP_VERSION}-arm64-mac.zip"
  fi

  local zip_path="${RELEASE_DIR}/${final_zip_name}"
  rm -f "${zip_path}"
  ditto -c -k --sequesterRsrc --keepParent "${app_path}" "${zip_path}"
  rm -f "${temp_zip_path}"

  echo "✅ ${arch_name} app notarized and stapled successfully!"
}

echo "🧹 Cleaning test files from app bundles..."
for app_path in "${APP_PATHS[@]}"; do
  rm -rf "${app_path}/Contents/Resources/app/electron/full_recordings"
  rm -rf "${app_path}/Contents/Resources/app/electron/chunks"
done

for app_path in "${APP_PATHS[@]}"; do
  if [[ "$app_path" == *"mac-arm64"* ]]; then
    arch_name="ARM64"
  elif [[ "$app_path" == *"mac"* ]]; then
    arch_name="Intel"
  else
    arch_name="Unknown"
  fi
  notarize_app "$app_path" "$arch_name"
done

notarize_dmg() {
  local dmg_path="$1"
  local arch_name="$2"

  echo "🚀 Notarizing ${arch_name} DMG: ${dmg_path}"
  xcrun notarytool submit "${dmg_path}" \
    --keychain-profile "${NOTARY_PROFILE}" \
    --wait --progress
  xcrun stapler staple "${dmg_path}"
  xcrun stapler validate "${dmg_path}"
  echo "✅ ${arch_name} DMG notarized and stapled!"
}

if [ -f "${RELEASE_DIR}/${APP_NAME}-${APP_VERSION}-arm64.dmg" ]; then
  notarize_dmg "${RELEASE_DIR}/${APP_NAME}-${APP_VERSION}-arm64.dmg" "ARM64"
fi
if [ -f "${RELEASE_DIR}/${APP_NAME}-${APP_VERSION}.dmg" ]; then
  notarize_dmg "${RELEASE_DIR}/${APP_NAME}-${APP_VERSION}.dmg" "Intel"
fi

echo "🔄 Updating latest-staging-mac.yml with new hashes..."
node scripts/update-latest-yaml.js --staging

RELEASE_TAG="v${APP_VERSION}-staging"

echo "🚀 Creating GitHub Pre-Release ${RELEASE_TAG}..."

RELEASE_FILES=(
  "${RELEASE_DIR}/${APP_NAME}-${APP_VERSION}-mac.zip"
  "${RELEASE_DIR}/${APP_NAME}-${APP_VERSION}-arm64-mac.zip"
  "${RELEASE_DIR}/${APP_NAME}-${APP_VERSION}.dmg"
  "${RELEASE_DIR}/${APP_NAME}-${APP_VERSION}-arm64.dmg"
  "${RELEASE_DIR}/latest-staging-mac.yml"
)

for f in "${RELEASE_FILES[@]}"; do
  if [ ! -f "$f" ]; then
    echo "❌ Missing release file: $f"
    exit 1
  fi
done

gh release create "${RELEASE_TAG}" \
  --title "${RELEASE_TAG}" \
  --prerelease \
  --draft \
  --notes "Staging release ${RELEASE_TAG}" \
  "${RELEASE_FILES[@]}"

echo "✅ GitHub Pre-Release draft ${RELEASE_TAG} created — publish manually when ready!"
