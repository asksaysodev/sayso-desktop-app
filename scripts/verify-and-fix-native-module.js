const { execSync } = require('child_process');
const path = require('path');
const fs = require('fs');

/**
 * Verifies and fixes the native module architecture after packaging
 * This ensures the native module matches the app's architecture
 */
exports.default = async function(context) {
  const { appOutDir, packager } = context;
  const appName = packager.appInfo.productFilename;
  const appPath = path.join(appOutDir, `${appName}.app`);
  const nativeModulePath = path.join(
    appPath,
    'Contents',
    'Resources',
    'app',
    'electron',
    'native-audio',
    'build',
    'Release',
    'native_audio.node'
  );

  console.log('🔍 [AFTER_PACK] Verifying native module architecture...');
  console.log(`  App path: ${appPath}`);
  console.log(`  Native module: ${nativeModulePath}`);

  if (!fs.existsSync(nativeModulePath)) {
    console.error('❌ [AFTER_PACK] Native module not found!');
    return;
  }

  // Get the app's architecture from the path
  let expectedArch = 'arm64'; // Default
  if (appOutDir.includes('mac-arm64')) {
    expectedArch = 'arm64';
  } else if (appOutDir.includes('mac') && !appOutDir.includes('arm64')) {
    expectedArch = 'x86_64';
  }

  // Check the native module's architecture
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
    console.error('❌ [AFTER_PACK] Failed to check native module architecture:', error.message);
    return;
  }

  // If architectures don't match, rebuild
  if (actualArch !== expectedArch) {
    console.log(`⚠️  [AFTER_PACK] Architecture mismatch! Expected ${expectedArch}, got ${actualArch}`);
    console.log(`🔧 [AFTER_PACK] Rebuilding native module for ${expectedArch}...`);
    
    const nativeAudioDir = path.join(__dirname, '..', 'electron', 'native-audio');
    const electronVersion = (packager && packager.config && packager.config.electronVersion) || '36.9.1';
    
    // Map expected arch to node-gyp arch
    const nodeGypArch = expectedArch === 'x86_64' ? 'x64' : 'arm64';
    
    try {
      // Clean and rebuild
      const buildDir = path.join(nativeAudioDir, 'build');
      if (fs.existsSync(buildDir)) {
        fs.rmSync(buildDir, { recursive: true, force: true });
      }
      
      console.log(`🏗️  Building for ${nodeGypArch}...`);
      execSync(
        `npx node-gyp rebuild --target=${electronVersion} --arch=${nodeGypArch} --dist-url=https://electronjs.org/headers`,
        { 
          cwd: nativeAudioDir,
          stdio: 'inherit' 
        }
      );
      
      // Copy the rebuilt module to the app
      const rebuiltModulePath = path.join(nativeAudioDir, 'build', 'Release', 'native_audio.node');
      if (fs.existsSync(rebuiltModulePath)) {
        fs.copyFileSync(rebuiltModulePath, nativeModulePath);
        console.log(`✅ [AFTER_PACK] Copied rebuilt ${expectedArch} module to app`);
        
      // Verify the fix
      const verifyOutput = execSync(`file "${nativeModulePath}"`, { encoding: 'utf-8' });
      console.log(`✅ [AFTER_PACK] Verified: ${verifyOutput.trim()}`);
    } else {
      console.error('❌ [AFTER_PACK] Rebuilt module not found!');
    }
  } catch (error) {
    console.error(`❌ [AFTER_PACK] Failed to rebuild:`, error.message);
    throw error;
  }
} else {
  console.log(`✅ [AFTER_PACK] Native module architecture matches (${actualArch})`);
}

// Sign the native module (always, whether we rebuilt or not)
console.log('🔐 [AFTER_PACK] Signing native audio module with entitlements...');
try {
  execSync(
    `codesign --force --sign "Developer ID Application: AskSayso, Inc. (AFGHD8M3VK)" --options runtime --entitlements "${path.join(__dirname, '..', 'assets', 'entitlements.mac.plist')}" "${nativeModulePath}"`,
    { stdio: 'inherit' }
  );
  console.log('✅ [AFTER_PACK] Native audio module signed successfully');
} catch (error) {
  console.error('❌ [AFTER_PACK] Failed to sign native audio module:', error.message);
  throw error;
}
};

