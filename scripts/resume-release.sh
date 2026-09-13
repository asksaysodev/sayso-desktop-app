#!/usr/bin/env bash
set -Eeuo pipefail

# Resume the GitHub Release step of rebuild-and-package.sh without rebuilding
# or re-notarizing. Safe to run multiple times: it pushes the tag if needed,
# creates the release if it's missing, and uploads the 5 final assets.
#
# NOTE on what "re-upload" means since SAYSO-403. Into a DRAFT this still
# replaces every asset. Into a PUBLISHED release it only adds the ones that are
# absent — so re-running this to repair a truncated or wrong-hash asset on a
# published release uploads NOTHING and still prints a success line. That is
# deliberate: replacing an asset users already downloaded invalidates the
# sha512 in the manifest beside it. If you genuinely must, do it by hand and
# know why: gh release upload <tag> --clobber <file>

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

# Create the release if missing, otherwise upload assets to the existing one.
# The tag was pushed above, so --target only matters on a first-ever create.
node scripts/release-upload.js \
  --tag "${TAG}" \
  --channel production \
  "${RELEASE_FILES[@]}"

echo ""
echo "✅ Release ${TAG} now has all notarized macOS assets."
echo "   Publish with: node scripts/release-preflight.js ${TAG} --publish"
