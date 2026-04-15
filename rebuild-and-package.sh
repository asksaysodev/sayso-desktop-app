#!/usr/bin/env bash
set -Eeuo pipefail

echo "🔧 Rebuilding and packaging Sayso with notarization..."

# Load environment variables from .env.production for production build
if [ -f .env.production ]; then
  export $(cat .env.production | grep -v '^#' | xargs)
  echo "✅ Loaded environment variables from .env.production"
elif [ -f .env ]; then
  export $(cat .env | grep -v '^#' | xargs)
  echo "⚠️  Using .env (fallback - .env.production not found)"
fi

# ---------- Config ----------
APP_NAME="Sayso"                   # The .app bundle name without .app
NOTARY_PROFILE="NotaryProfile"     # Keychain profile created via: xcrun notarytool store-credentials ...
DMG_NAME="${APP_NAME}.dmg"
RELEASE_DIR="release"

# Read version from package.json
APP_VERSION=$(node -p "require('./package.json').version")
echo "📦 Building version: ${APP_VERSION}"
# -----------------------------

# No validation needed - we're using electron-builder's DMGs directly

# Clean previous builds (optional: remove node_modules only when needed)
echo "🧹 Cleaning previous builds..."
rm -rf "${RELEASE_DIR}/" dist/

# Reinstall deps + build
echo " Reinstalling dependencies..."
npm install
echo "🏗️ Building the app..."
npm run build

# Package (electron-builder / forge / your tool)
echo " Packaging the app..."
npm run package

# Sign the electron-builder created DMGs
echo "🔐 Signing electron-builder DMGs..."
if [ -f "${RELEASE_DIR}/Sayso-${APP_VERSION}.dmg" ]; then
  codesign --sign "Developer ID Application: EXOMEND LLC (Y57SJLCC9H)" "${RELEASE_DIR}/Sayso-${APP_VERSION}.dmg"
  echo "✅ Signed Intel DMG"
fi
if [ -f "${RELEASE_DIR}/Sayso-${APP_VERSION}-arm64.dmg" ]; then
  codesign --sign "Developer ID Application: EXOMEND LLC (Y57SJLCC9H)" "${RELEASE_DIR}/Sayso-${APP_VERSION}-arm64.dmg"
  echo "✅ Signed Apple Silicon DMG"
fi

# Find ALL .app bundles (both Intel and ARM64)
echo "🔍 Looking for .app bundles..."
APP_PATHS=($(ls -d ${RELEASE_DIR}/**/${APP_NAME}.app 2>/dev/null || true))

if [[ ${#APP_PATHS[@]} -eq 0 ]]; then
  echo "❌ Could not find any ${APP_NAME}.app under ${RELEASE_DIR}/"
  exit 1
fi

echo "✅ Found ${#APP_PATHS[@]} app bundle(s):"
for app_path in "${APP_PATHS[@]}"; do
  echo "  - ${app_path}"
done

# Check if NotaryProfile exists and is accessible
echo "🔍 Verifying NotaryProfile '${NOTARY_PROFILE}' is accessible..."
if ! xcrun notarytool history --keychain-profile "${NOTARY_PROFILE}" &>/dev/null; then
  echo ""
  echo "❌ ERROR: NotaryProfile '${NOTARY_PROFILE}' not found or not accessible!"
  echo ""
  echo "To fix this, create the profile with:"
  echo "  xcrun notarytool store-credentials ${NOTARY_PROFILE} \\"
  echo "    --apple-id YOUR_APPLE_ID \\"
  echo "    --team-id Y57SJLCC9H"
  echo ""
  echo "You'll need an App-Specific Password from:"
  echo "  https://appleid.apple.com/account/manage → Security → App-Specific Passwords"
  echo ""
  exit 1
fi
echo "✅ NotaryProfile verified and accessible"

# Function to notarize a single app bundle
notarize_app() {
  local app_path="$1"
  local arch_name="$2"
  
  echo "🚀 Notarizing ${arch_name} app: ${app_path}"
  
  # Create temp ZIP for notarization submission (before stapling)
  local temp_zip_path="${RELEASE_DIR}/${APP_NAME}-${arch_name}-temp.zip"
  echo "🗜️  Creating temporary zip for notarization: ${temp_zip_path}"
  rm -f "${temp_zip_path}"
  ditto -c -k --sequesterRsrc --keepParent "${app_path}" "${temp_zip_path}"

  # Submit to Apple Notary Service and WAIT for result
  echo "🚀 Submitting ${arch_name} to Apple Notary Service (blocking until done)..."
  xcrun notarytool submit "${temp_zip_path}" \
    --keychain-profile "${NOTARY_PROFILE}" \
    --wait --progress

  # Staple the ticket to the .app
  echo "📎 Stapling ticket to ${arch_name} app..."
  xcrun stapler staple "${app_path}"
  xcrun stapler validate "${app_path}"
  
  # NOW create the final ZIP with the stapled .app (for electron-updater)
  # RENAME to match the standard name electron-builder used (and listed in latest-mac.yml)
  local final_zip_name=""
  if [ "$arch_name" == "Intel" ]; then
    final_zip_name="${APP_NAME}-${APP_VERSION}-mac.zip"
  elif [ "$arch_name" == "ARM64" ]; then
    final_zip_name="${APP_NAME}-${APP_VERSION}-arm64-mac.zip"
  fi
  
  local zip_path="${RELEASE_DIR}/${final_zip_name}"
  echo "🗜️  Creating final ZIP with stapled app: ${zip_path}"
  
  # Remove the old un-notarized zip if it exists
  rm -f "${zip_path}"
  
  # Create new notarized zip
  ditto -c -k --sequesterRsrc --keepParent "${app_path}" "${zip_path}"
  
  # Clean up temp zip
  rm -f "${temp_zip_path}"
  
  echo "✅ ${arch_name} app notarized and stapled successfully!"
}

# Clean up test/development files from app bundles before notarization
echo "🧹 Cleaning test files from app bundles..."
for app_path in "${APP_PATHS[@]}"; do
  # Remove full_recordings folder (test/development audio files)
  recordings_path="${app_path}/Contents/Resources/app/electron/full_recordings"
  if [ -d "${recordings_path}" ]; then
    echo "  Removing full_recordings from $(basename "$(dirname "$app_path")")..."
    rm -rf "${recordings_path}"
  fi
  
  # Remove chunks folder if it exists
  chunks_path="${app_path}/Contents/Resources/app/electron/chunks"
  if [ -d "${chunks_path}" ]; then
    echo "  Removing chunks from $(basename "$(dirname "$app_path")")..."
    rm -rf "${chunks_path}"
  fi
done

# Notarize each app bundle
for app_path in "${APP_PATHS[@]}"; do
  # Determine architecture based on path
  if [[ "$app_path" == *"mac-arm64"* ]]; then
    arch_name="ARM64"
  elif [[ "$app_path" == *"mac"* ]]; then
    arch_name="Intel"
  else
    arch_name="Unknown"
  fi
  
  notarize_app "$app_path" "$arch_name"
done

# Note: We're using electron-builder's DMGs directly - they already have the installer UI
# We just need to notarize them after the apps inside are notarized

# Function to notarize a DMG
notarize_dmg() {
  local dmg_path="$1"
  local arch_name="$2"
  
  echo "🚀 Notarizing ${arch_name} DMG: ${dmg_path}"
  
  # Submit DMG to Apple Notary Service and WAIT for result
  echo "🚀 Submitting ${arch_name} DMG to Apple Notary Service (blocking until done)..."
  xcrun notarytool submit "${dmg_path}" \
    --keychain-profile "${NOTARY_PROFILE}" \
    --wait --progress
  
  # Staple the ticket to the DMG
  echo "📎 Stapling ticket to ${arch_name} DMG..."
  xcrun stapler staple "${dmg_path}"
  xcrun stapler validate "${dmg_path}"
  
  echo "✅ ${arch_name} DMG notarized and stapled successfully!"
}

# Use electron-builder's DMGs directly - they already have the installer UI
# We just need to notarize them (keeping version numbers for auto-updater compatibility)
echo "📦 Using electron-builder DMGs (they already have installer UI)..."

# Notarize electron-builder DMGs (keep original names with version numbers)
if [ -f "${RELEASE_DIR}/Sayso-${APP_VERSION}-arm64.dmg" ]; then
  dmg_path="${RELEASE_DIR}/Sayso-${APP_VERSION}-arm64.dmg"
  echo "🔐 Notarizing ARM64 DMG..."
  notarize_dmg "${dmg_path}" "ARM64"
  echo "✅ ARM64 DMG notarized: ${dmg_path}"
fi

if [ -f "${RELEASE_DIR}/Sayso-${APP_VERSION}.dmg" ]; then
  dmg_path="${RELEASE_DIR}/Sayso-${APP_VERSION}.dmg"
  echo "🔐 Notarizing Intel DMG..."
  notarize_dmg "${dmg_path}" "Intel"
  echo "✅ Intel DMG notarized: ${dmg_path}"
fi

echo "✅ Done! All artifacts in ${RELEASE_DIR}/"
echo "📦 Created DMGs:"
ls -la "${RELEASE_DIR}"/*.dmg 2>/dev/null || echo "No DMGs found"

echo "🔄 Updating latest-mac.yml with new hashes..."
node scripts/update-latest-yaml.js

# Create GitHub Release with only the 5 final notarized files
echo "🚀 Creating GitHub Release v${APP_VERSION}..."

RELEASE_FILES=(
  "${RELEASE_DIR}/Sayso-${APP_VERSION}-mac.zip"
  "${RELEASE_DIR}/Sayso-${APP_VERSION}-arm64-mac.zip"
  "${RELEASE_DIR}/Sayso-${APP_VERSION}.dmg"
  "${RELEASE_DIR}/Sayso-${APP_VERSION}-arm64.dmg"
  "${RELEASE_DIR}/latest-mac.yml"
)

# Verify all release files exist before uploading
for f in "${RELEASE_FILES[@]}"; do
  if [ ! -f "$f" ]; then
    echo "❌ Missing release file: $f"
    exit 1
  fi
done

gh release create "v${APP_VERSION}" \
  --title "v${APP_VERSION}" \
  --draft \
  --notes "Release v${APP_VERSION}" \
  "${RELEASE_FILES[@]}"

echo "✅ GitHub Release draft v${APP_VERSION} created with notarized files — publish manually when ready!" 