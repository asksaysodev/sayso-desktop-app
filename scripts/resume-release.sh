#!/usr/bin/env bash
set -Eeuo pipefail

# Resume the GitHub Release step of rebuild-and-package.sh without rebuilding
# or re-notarizing. Safe to run multiple times: it pushes the tag if needed,
# creates the release if it's missing, and (re)uploads all 5 final assets.

# ---------- Config ----------
APP_NAME="Sayso"
RELEASE_DIR="release"
APP_VERSION=$(node -p "require('./package.json').version")
TAG="v${APP_VERSION}"
echo "📦 Resuming release for: ${TAG}"
# -----------------------------

RELEASE_FILES=(
  "${RELEASE_DIR}/${APP_NAME}-${APP_VERSION}-x64-mac.zip"
  "${RELEASE_DIR}/${APP_NAME}-${APP_VERSION}-arm64-mac.zip"
  "${RELEASE_DIR}/${APP_NAME}-${APP_VERSION}-x64-mac.dmg"
  "${RELEASE_DIR}/${APP_NAME}-${APP_VERSION}-arm64-mac.dmg"
  "${RELEASE_DIR}/latest-mac.yml"
)

# Verify all release files exist before uploading
for f in "${RELEASE_FILES[@]}"; do
  if [ ! -f "$f" ]; then
    echo "❌ Missing release file: $f"
    exit 1
  fi
done
echo "✅ All ${#RELEASE_FILES[@]} release files present."

# Make sure the tag is on the remote (gh release needs it there)
if ! git ls-remote --exit-code --tags origin "${TAG}" >/dev/null 2>&1; then
  echo "⤴️  Tag ${TAG} not on remote — pushing it..."
  git push origin "${TAG}"
else
  echo "✅ Tag ${TAG} already on remote."
fi

# Create the release if missing, otherwise upload assets to the existing one
if gh release view "${TAG}" >/dev/null 2>&1; then
  echo "ℹ️  Release ${TAG} already exists — (re)uploading assets..."
  gh release upload "${TAG}" --clobber "${RELEASE_FILES[@]}"
else
  echo "🚀 Creating GitHub Release ${TAG}..."
  gh release create "${TAG}" \
    --title "${TAG}" \
    --draft \
    --notes "Release ${TAG}" \
    "${RELEASE_FILES[@]}"
fi

echo "✅ Release ${TAG} now has all notarized assets — publish manually when ready!"
