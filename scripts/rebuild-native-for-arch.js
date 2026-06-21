const { execSync } = require('child_process');
const path = require('path');
const fs = require('fs');

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

    const fileOutput = execSync(`file "${modulePath}"`, { encoding: 'utf-8' });
    console.log(`[BEFORE_PACK] Native module built: ${fileOutput.trim()}`);

  } catch (error) {
    console.error(`[BEFORE_PACK] Failed to rebuild native module for ${arch}:`, error.message);
    throw error;
  }
};
