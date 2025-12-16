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

# Function to create DMG with proper installer UI (like electron-builder does)
create_dmg_with_ui() {
  local app_path="$1"
  local dmg_path="$2"
  local arch_name="$3"
  
  echo " Creating ${arch_name} DMG with installer UI: $(basename "${dmg_path}")"
  
  # Create temporary DMG
  temp_dmg="${RELEASE_DIR}/temp-$(basename "${dmg_path}")"
  rm -f "${temp_dmg}" "${dmg_path}"
  
  # Calculate app size and add 20% buffer for DMG overhead
  app_size=$(du -sm "${app_path}" | awk '{print $1}')
  dmg_size=$((app_size + app_size / 5 + 100))
  
  echo "  App size: ${app_size}MB, Allocating DMG size: ${dmg_size}MB"
  
  # Create read-write DMG with dynamic size
  hdiutil create -srcfolder "${app_path}" -volname "${APP_NAME}" -fs HFS+ -fsargs "-c c=64,a=16,e=16" -format UDRW -size ${dmg_size}m "${temp_dmg}"
  
  # Mount the DMG
  device=$(hdiutil attach -readwrite -noverify -noautoopen "${temp_dmg}" | egrep '^/dev/' | sed 1q | awk '{print $1}')
  mount_point=$(hdiutil info | grep "$device" | awk '{print $3}')
  
  # Create Applications symlink
  ln -s /Applications "${mount_point}/Applications"
  
  # Set window layout using AppleScript (replicates electron-builder's default layout)
  osascript <<EOF
    tell application "Finder"
      tell disk "${APP_NAME}"
        open
        set current view of container window to icon view
        set toolbar visible of container window to false
        set statusbar visible of container window to false
        set the bounds of container window to {400, 100, 920, 420}
        set viewOptions to the icon view options of container window
        set arrangement of viewOptions to not arranged
        set icon size of viewOptions to 72
        set position of item "${APP_NAME}.app" of container window to {130, 220}
        set position of item "Applications" of container window to {410, 220}
        close
        open
        update without registering applications
        delay 2
      end tell
    end tell
EOF
  
  # Unmount the DMG
  hdiutil detach "$device"
  
  # Convert to compressed read-only DMG
  hdiutil convert "${temp_dmg}" -format UDZO -imagekey zlib-level=9 -o "${dmg_path}"
  rm -f "${temp_dmg}"
  
  echo "✅ ${arch_name} DMG created with installer UI"
}

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

# Create DMGs from notarized app bundles with proper installer UI
echo " Creating DMGs with installer UI for each architecture..."
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
  
  dmg_path="${RELEASE_DIR}/${dmg_name}"
  
  # Create DMG with proper installer UI
  create_dmg_with_ui "${app_path}" "${dmg_path}" "${arch_name}"
  
  # Sign the DMG
  echo "🔐 Signing ${arch_name} DMG..."
  codesign --sign "Developer ID Application: EXOMEND LLC (Y57SJLCC9H)" "${dmg_path}"
  
  # Notarize the DMG
  notarize_dmg "${dmg_path}" "${arch_name}"
  
  echo "✅ ${arch_name} DMG created, signed, and notarized: ${dmg_path}"
done

echo "✅ Done! All artifacts in ${RELEASE_DIR}/"
echo "📦 Created DMGs:"
ls -la "${RELEASE_DIR}"/*.dmg 2>/dev/null || echo "No DMGs found" 