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
echo "📦 Packaging the app..."
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

# Locate the .app we just built (first match wins)
APP_PATH="$(ls -d ${RELEASE_DIR}/**/${APP_NAME}.app 2>/dev/null | head -n 1 || true)"
if [[ -z "${APP_PATH}" ]]; then
  echo "❌ Could not find ${APP_NAME}.app under ${RELEASE_DIR}/"
  exit 1
fi
echo "✅ Found app: ${APP_PATH}"

# Zip the .app correctly (preserves attrs)
ZIP_PATH="${RELEASE_DIR}/${APP_NAME}.zip"
echo "🗜️  Creating notarization zip: ${ZIP_PATH}"
rm -f "${ZIP_PATH}"
ditto -c -k --sequesterRsrc --keepParent "${APP_PATH}" "${ZIP_PATH}"

# Submit to Apple Notary Service and WAIT for result
echo "🚀 Submitting to Apple Notary Service (blocking until done)..."
xcrun notarytool submit "${ZIP_PATH}" \
  --keychain-profile "${NOTARY_PROFILE}" \
  --wait --progress

# Staple the ticket to the .app
echo "📎 Stapling ticket to app..."
xcrun stapler staple "${APP_PATH}"
xcrun stapler validate "${APP_PATH}"

# Notarize the app bundle itself
echo "🚀 Notarizing app bundle..."
ZIP_PATH="${RELEASE_DIR}/${APP_NAME}-app.zip"
rm -f "${ZIP_PATH}"
ditto -c -k --sequesterRsrc --keepParent "${APP_PATH}" "${ZIP_PATH}"

echo "🚀 Submitting app bundle to Apple Notary Service..."
xcrun notarytool submit "${ZIP_PATH}" \
  --keychain-profile "${NOTARY_PROFILE}" \
  --wait --progress

# Staple the ticket to the app bundle
echo "📎 Stapling ticket to app bundle..."
xcrun stapler staple "${APP_PATH}"
xcrun stapler validate "${APP_PATH}"

# Rebuild a clean DMG from the stapled app
echo " Creating DMG..."
DMG_PATH="${RELEASE_DIR}/${DMG_NAME}"
rm -f "${DMG_PATH}"
hdiutil create -volname "${APP_NAME}" -srcfolder "${APP_PATH}" -ov -format UDZO "${DMG_PATH}"

# Sign the DMG
echo "🔐 Signing DMG..."
codesign --sign "Developer ID Application: EXOMEND LLC (Y57SJLCC9H)" "${DMG_PATH}"

# Optional: staple the DMG too (nice for offline checks)
echo "📎 Stapling ticket to DMG (optional)..."
xcrun stapler staple "${DMG_PATH}" || true

echo "✅ Done! Artifacts in ${RELEASE_DIR}/" 