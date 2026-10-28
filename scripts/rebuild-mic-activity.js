const { execSync } = require('child_process');
const path = require('path');
const fs = require('fs');
const { archOfBinary } = require('./binary-arch');

/**
 * Builds the call-detection addon (electron/mic-activity) against Electron's
 * headers for one arch.
 *
 * Deliberately separate from the native-audio build (rebuild-native-for-arch.js,
 * owned by another team): it runs after it, in its own folder, and packaging
 * treats a failure here as non-fatal — the app then ships without call detection
 * (micActivityMonitor.ts tolerates a missing .node) rather than not shipping.
 *
 * CLI (dev): node scripts/rebuild-mic-activity.js [arm64|x64] — exits 1 on failure.
 */

const DEFAULT_ELECTRON_VERSION = '36.9.1';
const micActivityDir = path.join(__dirname, '..', 'electron', 'mic-activity');
const modulePath = path.join(micActivityDir, 'build', 'Release', 'mic_activity.node');

function defaultArch() {
  // Same pinning as rebuild-native-both-archs.js: Windows builds x64 only.
  if (process.platform === 'win32') return 'x64';
  return process.arch === 'arm64' ? 'arm64' : 'x64';
}

/** Throws on failure; returns the built module path. */
function buildMicActivity(arch, electronVersion = DEFAULT_ELECTRON_VERSION) {
  if (!fs.existsSync(path.join(micActivityDir, 'node_modules', 'nan'))) {
    // Own package.json + lockfile, not covered by the root install.
    execSync('npm ci --ignore-scripts', { cwd: micActivityDir, stdio: 'inherit' });
  }
  fs.rmSync(path.join(micActivityDir, 'build'), { recursive: true, force: true });
  execSync(
    `npx node-gyp rebuild --target=${electronVersion} --arch=${arch} --dist-url=https://electronjs.org/headers`,
    { cwd: micActivityDir, stdio: 'inherit' }
  );
  if (!fs.existsSync(modulePath)) {
    throw new Error(`mic-activity module not found at ${modulePath}`);
  }
  const builtArch = archOfBinary(modulePath);
  if (builtArch !== 'unknown' && builtArch !== arch) {
    throw new Error(`mic-activity architecture mismatch: expected ${arch}, built ${builtArch}`);
  }
  return modulePath;
}

/** Packaging variant: never throws, logs loudly instead. Returns success. */
function tryBuildMicActivity(arch, electronVersion) {
  try {
    const built = buildMicActivity(arch, electronVersion);
    console.log(`[MIC_ACTIVITY] Built ${built} (${arch})`);
    return true;
  } catch (error) {
    console.warn('');
    console.warn('⚠️  [MIC_ACTIVITY] Build failed — packaging continues WITHOUT call detection.');
    console.warn(`⚠️  [MIC_ACTIVITY] ${error.message}`);
    console.warn('');
    return false;
  }
}

module.exports = { buildMicActivity, tryBuildMicActivity, micActivityDir, modulePath };

if (require.main === module) {
  const arch = process.argv[2] || defaultArch();
  try {
    buildMicActivity(arch);
    console.log(`✅ mic-activity built for ${arch}`);
  } catch (error) {
    console.error(`❌ mic-activity build failed: ${error.message}`);
    process.exit(1);
  }
}
