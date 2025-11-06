#!/usr/bin/env bash
set -Eeuo pipefail

echo "🔧 Rebuilding and packaging Sayso with notarization..."

# ---------- Config ----------
APP_NAME="Sayso"                   # The .app bundle name without .app
NOTARY_PROFILE="NotaryProfile"     # Keychain profile created via: xcrun notarytool store-credentials ...
DMG_NAME="${APP_NAME}.dmg"
RELEASE_DIR="release"
# -----------------------------

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
if [ -f "${RELEASE_DIR}/Sayso-1.0.0.dmg" ]; then
  codesign --sign "Developer ID Application: EXOMEND LLC (Y57SJLCC9H)" "${RELEASE_DIR}/Sayso-1.0.0.dmg"
  echo "✅ Signed Intel DMG"
fi
if [ -f "${RELEASE_DIR}/Sayso-1.0.0-arm64.dmg" ]; then
  codesign --sign "Developer ID Application: EXOMEND LLC (Y57SJLCC9H)" "${RELEASE_DIR}/Sayso-1.0.0-arm64.dmg"
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
  
  # Zip the .app correctly (preserves attrs)
  local zip_path="${RELEASE_DIR}/${APP_NAME}-${arch_name}.zip"
  echo "🗜️  Creating notarization zip: ${zip_path}"
  rm -f "${zip_path}"
  ditto -c -k --sequesterRsrc --keepParent "${app_path}" "${zip_path}"

  # Submit to Apple Notary Service and WAIT for result
  echo "🚀 Submitting ${arch_name} to Apple Notary Service (blocking until done)..."
  xcrun notarytool submit "${zip_path}" \
    --keychain-profile "${NOTARY_PROFILE}" \
    --wait --progress

  # Staple the ticket to the .app
  echo "📎 Stapling ticket to ${arch_name} app..."
  xcrun stapler staple "${app_path}"
  xcrun stapler validate "${app_path}"
  
  echo "✅ ${arch_name} app notarized and stapled successfully!"
}

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

# Create DMGs for each notarized app
echo " Creating DMGs for each architecture..."
for app_path in "${APP_PATHS[@]}"; do
  # Determine architecture and DMG name
  if [[ "$app_path" == *"mac-arm64"* ]]; then
    arch_name="ARM64"
    dmg_name="${APP_NAME}-arm64.dmg"
  elif [[ "$app_path" == *"mac"* ]]; then
    arch_name="Intel"
    dmg_name="${APP_NAME}-intel.dmg"
  else
    arch_name="Unknown"
    dmg_name="${APP_NAME}.dmg"
  fi
  
  echo " Creating ${arch_name} DMG: ${dmg_name}"
  dmg_path="${RELEASE_DIR}/${dmg_name}"
  rm -f "${dmg_path}"
  hdiutil create -volname "${APP_NAME}" -srcfolder "${app_path}" -ov -format UDZO "${dmg_path}"

  # Sign the DMG
  echo " Signing ${arch_name} DMG..."
  codesign --sign "Developer ID Application: EXOMEND LLC (Y57SJLCC9H)" "${dmg_path}"

  # Optional: staple the DMG too (nice for offline checks)
  echo "📎 Stapling ticket to ${arch_name} DMG (optional)..."
  xcrun stapler staple "${dmg_path}" || true
  
  echo "✅ ${arch_name} DMG created: ${dmg_path}"
done

echo "✅ Done! All artifacts in ${RELEASE_DIR}/"
echo "📦 Created DMGs:"
ls -la "${RELEASE_DIR}"/*.dmg 2>/dev/null || echo "No DMGs found" 