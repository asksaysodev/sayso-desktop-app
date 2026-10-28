const { execSync } = require('child_process');
const path = require('path');
const fs = require('fs');

exports.default = async function(context) {
  // This hook assumes a macOS .app bundle layout and shells out to `file`/
  // `codesign` (both macOS-only) — mirrors the identical guard already used
  // by the sibling afterAllArtifactBuild hook (apply-dmg-background.js).
  if (process.platform !== 'darwin') return;

  const { appOutDir, packager } = context;
  const appName = packager.appInfo.productFilename;
  const appPath = path.join(appOutDir, `${appName}.app`);
  const nativeModulePath = path.join(
    appPath,
    'Contents',
    'Resources',
    'app.asar.unpacked',
    'electron',
    'native-audio',
    'build',
    'Release',
    'native_audio.node'
  );

  console.log('[AFTER_PACK] Verifying native module architecture...');
  console.log(`  App path: ${appPath}`);
  console.log(`  Native module: ${nativeModulePath}`);

  if (!fs.existsSync(nativeModulePath)) {
    console.error('[AFTER_PACK] Native module not found!');
    throw new Error(`Native module not found at: ${nativeModulePath}`);
  }

  let expectedArch = 'arm64';
  if (appOutDir.includes('mac-arm64')) {
    expectedArch = 'arm64';
  } else if (appOutDir.includes('mac') && !appOutDir.includes('arm64')) {
    expectedArch = 'x86_64';
  }

  let actualArch = '';
  try {
    const fileOutput = execSync(`file "${nativeModulePath}"`, { encoding: 'utf-8' });
    console.log(`  Current: ${fileOutput.trim()}`);
    if (fileOutput.includes('x86_64')) {
      actualArch = 'x86_64';
    } else if (fileOutput.includes('arm64')) {
      actualArch = 'arm64';
    }
  } catch (error) {
    console.error('[AFTER_PACK] Failed to check native module architecture:', error.message);
    throw error;
  }

  if (actualArch !== expectedArch) {
    console.log(`[AFTER_PACK] Architecture mismatch - expected ${expectedArch}, got ${actualArch}. Rebuilding...`);

    const nativeAudioDir = path.join(__dirname, '..', 'electron', 'native-audio');
    const electronVersion = (packager && packager.config && packager.config.electronVersion) || '36.9.1';
    const nodeGypArch = expectedArch === 'x86_64' ? 'x64' : 'arm64';

    try {
      const buildDir = path.join(nativeAudioDir, 'build');
      if (fs.existsSync(buildDir)) {
        fs.rmSync(buildDir, { recursive: true, force: true });
      }

      console.log(`[AFTER_PACK] Building for ${nodeGypArch}...`);
      execSync(
        `npx node-gyp rebuild --target=${electronVersion} --arch=${nodeGypArch} --dist-url=https://electronjs.org/headers`,
        { cwd: nativeAudioDir, stdio: 'inherit' }
      );

      const rebuiltModulePath = path.join(nativeAudioDir, 'build', 'Release', 'native_audio.node');
      if (!fs.existsSync(rebuiltModulePath)) {
        throw new Error('Rebuilt module not found');
      }

      fs.copyFileSync(rebuiltModulePath, nativeModulePath);
      const verifyOutput = execSync(`file "${nativeModulePath}"`, { encoding: 'utf-8' });
      console.log(`[AFTER_PACK] Rebuilt and copied: ${verifyOutput.trim()}`);
    } catch (error) {
      console.error('[AFTER_PACK] Failed to rebuild:', error.message);
      throw error;
    }
  } else {
    console.log(`[AFTER_PACK] Native module architecture matches (${actualArch})`);
  }

  const signingIdentity = process.env.CSC_NAME || 'Developer ID Application: AskSayso, Inc. (AFGHD8M3VK)';
  console.log('[AFTER_PACK] Signing native audio module...');
  try {
    execSync(
      `codesign --force --sign "${signingIdentity}" --options runtime --entitlements "${path.join(__dirname, '..', 'assets', 'entitlements.mac.plist')}" "${nativeModulePath}"`,
      { stdio: 'inherit' }
    );
    console.log('[AFTER_PACK] Native audio module signed successfully');
  } catch (error) {
    console.error('[AFTER_PACK] Failed to sign native audio module:', error.message);
    throw error;
  }

  verifyMicActivityModule(appPath, expectedArch, packager, signingIdentity);
};

// Call-detection addon (SAYSO-528). Runs only after native audio is verified and
// signed, and never throws: a problem here ships the app without call detection
// instead of failing the release.
function verifyMicActivityModule(appPath, expectedArch, packager, signingIdentity) {
  const { tryBuildMicActivity, modulePath: builtModulePath } = require('./rebuild-mic-activity');
  const packedModulePath = path.join(
    appPath, 'Contents', 'Resources', 'app.asar.unpacked',
    'electron', 'mic-activity', 'build', 'Release', 'mic_activity.node'
  );
  try {
    if (!fs.existsSync(packedModulePath)) {
      console.warn('[AFTER_PACK] ⚠️ mic-activity module missing — shipping without call detection');
      return;
    }
    const nodeGypArch = expectedArch === 'x86_64' ? 'x64' : 'arm64';
    const fileOutput = execSync(`file "${packedModulePath}"`, { encoding: 'utf-8' });
    if (!fileOutput.includes(expectedArch)) {
      console.log(`[AFTER_PACK] mic-activity architecture mismatch (${fileOutput.trim()}) — rebuilding for ${nodeGypArch}`);
      const electronVersion = packager && packager.config && packager.config.electronVersion;
      if (!tryBuildMicActivity(nodeGypArch, electronVersion)) {
        fs.rmSync(packedModulePath, { force: true });
        console.warn('[AFTER_PACK] ⚠️ removed wrong-arch mic-activity module — shipping without call detection');
        return;
      }
      fs.copyFileSync(builtModulePath, packedModulePath);
    }
    execSync(
      `codesign --force --sign "${signingIdentity}" --options runtime --entitlements "${path.join(__dirname, '..', 'assets', 'entitlements.mac.plist')}" "${packedModulePath}"`,
      { stdio: 'inherit' }
    );
    console.log('[AFTER_PACK] mic-activity module verified and signed');
  } catch (error) {
    fs.rmSync(packedModulePath, { force: true });
    console.warn(`[AFTER_PACK] ⚠️ mic-activity verify/sign failed (${error.message}) — removed it, shipping without call detection`);
  }
}
