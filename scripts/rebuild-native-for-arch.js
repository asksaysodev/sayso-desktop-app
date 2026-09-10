const { execSync } = require('child_process');
const path = require('path');
const fs = require('fs');
const { archOfBinary } = require('./binary-arch');

exports.default = async function(context) {
  let arch = context.arch;
  const packager = context.packager;
  const appOutDir = context.appOutDir;

  const electronVersion = (packager && packager.config && packager.config.electronVersion) || '36.9.1';

  console.log(`[BEFORE_PACK] Hook called - arch: ${arch}, appOutDir: ${appOutDir}`);

  // electron-builder Arch enum: 0=ia32, 1=x64, 2=armv7l, 3=arm64, 4=universal
  if (typeof arch === 'number') {
    const archEnumMap = {
      0: 'ia32',
      1: 'x64',
      2: 'armv7l',
      3: 'arm64',
      4: 'universal',
    };
    arch = archEnumMap[arch] || 'x64';
    console.log(`[BEFORE_PACK] Converted arch enum ${context.arch} to ${arch}`);
  }

  if (!arch) {
    console.error('[BEFORE_PACK] No architecture specified in context!');
    throw new Error('Architecture not specified in beforePack context');
  }

  // Only build for architectures we actually support
  if (arch !== 'x64' && arch !== 'arm64') {
    console.log(`[BEFORE_PACK] Skipping unsupported arch: ${arch}`);
    return;
  }

  const nodeGypArch = arch;

  console.log(`[BEFORE_PACK] Rebuilding native audio module for ${arch} (electron ${electronVersion})`);

  const nativeAudioDir = path.join(__dirname, '..', 'electron', 'native-audio');
  let builtArch = 'unknown';

  try {
    console.log('Cleaning previous build...');
    const buildDir = path.join(nativeAudioDir, 'build');
    if (fs.existsSync(buildDir)) {
      fs.rmSync(buildDir, { recursive: true, force: true });
    }

    console.log(`Building for ${nodeGypArch}...`);
    execSync(
      `npx node-gyp rebuild --target=${electronVersion} --arch=${nodeGypArch} --dist-url=https://electronjs.org/headers`,
      {
        cwd: nativeAudioDir,
        stdio: 'inherit'
      }
    );

    const modulePath = path.join(nativeAudioDir, 'build', 'Release', 'native_audio.node');
    if (!fs.existsSync(modulePath)) {
      throw new Error(`Native module not found at ${modulePath}`);
    }

    builtArch = archOfBinary(modulePath);
    console.log(`[BEFORE_PACK] Native module built: ${modulePath} (${builtArch})`);

  } catch (error) {
    console.error(`[BEFORE_PACK] Failed to rebuild native module for ${arch}:`, error.message);
    throw error;
  }

  // Deliberately outside the try: a mismatch is not a rebuild failure, and the
  // catch above would relabel it as one.
  if (builtArch !== 'unknown' && builtArch !== arch) {
    const message = `[BEFORE_PACK] Architecture mismatch: expected ${arch}, built ${builtArch}`;

    // macOS repairs itself — the afterPack hook (verify-and-fix-native-module.js)
    // rebuilds and copies over the packed module. Nothing does on Windows, where
    // that hook returns early on non-darwin, so a wrong-arch module would ship
    // silently inside the installer and only fail when a user opens Cue.
    if (process.platform === 'darwin') {
      console.warn(`${message} — afterPack will rebuild`);
    } else {
      throw new Error(message);
    }
  }
};
