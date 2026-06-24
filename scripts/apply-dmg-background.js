'use strict';

const { execSync } = require('child_process');
const path = require('path');
const fs = require('fs');
const os = require('os');

const BACKGROUND = path.resolve(__dirname, '../assets/dmg-background.tiff');

// electron-builder afterAllArtifactBuild hook
exports.default = async function applyDmgBackground(buildResult) {
  if (process.platform !== 'darwin') return;

  const dmgs = (buildResult.artifactPaths || []).filter(p => p.endsWith('.dmg'));

  if (dmgs.length === 0) return;

  for (const dmg of dmgs) {
    console.log(`\nApplying DMG background to ${path.basename(dmg)}...`);
    try {
      await applyBackground(dmg);
    } catch (err) {
      console.error(`Failed to apply background to ${path.basename(dmg)}: ${err.message}`);
    }
  }
};

async function applyBackground(dmgPath) {
  const tmp = path.join(os.tmpdir(), `sayso-rw-${Date.now()}.dmg`);
  const scriptPath = path.join(os.tmpdir(), `sayso-bg-${Date.now()}.applescript`);
  let volumePath;

  try {
    // Convert to read-write so Finder can modify .DS_Store
    execSync(`hdiutil convert "${dmgPath}" -format UDRW -o "${tmp}" -quiet`);

    // Mount read-write so Finder can modify .DS_Store
    const mountOut = execSync(`hdiutil attach "${tmp}" -readwrite -noverify`).toString();
    volumePath = mountOut
      .split('\n')
      .map(line => line.match(/\t(\/Volumes\/.+)/)?.[1]?.trim())
      .find(Boolean);

    if (!volumePath) throw new Error(`Could not determine mount point:\n${mountOut}`);

    const volName = path.basename(volumePath);
    console.log(`  Mounted: ${volumePath}`);

    // Ship the background image *inside* the DMG so it travels with the volume.
    // Referencing an absolute build-machine path here means the alias stored in
    // .DS_Store only resolves on the build machine — every other user gets a
    // plain window. Copy it into .background/ and reference it volume-relative.
    const bgDir = path.join(volumePath, '.background');
    fs.mkdirSync(bgDir, { recursive: true });
    fs.copyFileSync(BACKGROUND, path.join(bgDir, 'background.tiff'));

    // Let Finder write the .DS_Store itself — this is what works on macOS 14+/APFS
    fs.writeFileSync(scriptPath, `
tell application "Finder"
  tell disk "${volName}"
    open
    set current view of container window to icon view
    set toolbar visible of container window to false
    set statusbar visible of container window to false
    set bounds of container window to {100, 100, 700, 500}
    set theViewOptions to icon view options of container window
    set arrangement of theViewOptions to not arranged
    set icon size of theViewOptions to 110
    set background picture of theViewOptions to file ".background:background.tiff"
    update without registering applications
    delay 5
    close
  end tell
end tell
`);

    try {
      execSync(`osascript "${scriptPath}"`, { stdio: 'inherit' });
      console.log('  Finder applied background successfully');
    } finally {
      try { fs.unlinkSync(scriptPath); } catch (_) {}
    }

    // Give Finder a moment to flush .DS_Store to disk
    execSync('sync');

    // Unmount
    execSync(`hdiutil detach "${volumePath}" -quiet`);
    volumePath = null;

    // Replace original DMG with re-compressed version
    fs.unlinkSync(dmgPath);
    execSync(`hdiutil convert "${tmp}" -format UDZO -imagekey zlib-level=9 -o "${dmgPath}" -quiet`);
    fs.unlinkSync(tmp);

    // Regenerate blockmap so auto-update deltas stay valid
    const blockmapPath = `${dmgPath}.blockmap`;
    if (fs.existsSync(blockmapPath)) fs.unlinkSync(blockmapPath);
    try {
      const appBuilder = path.resolve(
        __dirname,
        '../node_modules/app-builder-bin/mac/app-builder'
      );
      if (fs.existsSync(appBuilder)) {
        execSync(`"${appBuilder}" blockmap --input="${dmgPath}" --output="${blockmapPath}"`);
        console.log('  Blockmap regenerated');
      }
    } catch (_) {
      console.warn('  Could not regenerate blockmap — rebuild if you need delta updates');
    }

    console.log(`  Done: ${path.basename(dmgPath)}`);
  } finally {
    if (volumePath) {
      try { execSync(`hdiutil detach "${volumePath}" -quiet -force`); } catch (_) {}
    }
    if (fs.existsSync(tmp)) {
      try { fs.unlinkSync(tmp); } catch (_) {}
    }
  }
}
