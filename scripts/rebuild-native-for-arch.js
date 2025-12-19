const { execSync } = require('child_process');
const path = require('path');
const fs = require('fs');

/**
 * Rebuilds the native audio module for the target architecture
 * This is called by electron-builder's beforePack hook for each architecture
 */
exports.default = async function(context) {
  // Extract relevant info from context (avoid circular references)
  // arch might be an enum (1 = x64, 2 = arm64) or a string
  let arch = context.arch;
  const packager = context.packager;
  const appOutDir = context.appOutDir;
  
  // Get electron version from packager.config (found via exploration)
  const electronVersion = (packager && packager.config && packager.config.electronVersion) || '36.9.1';
  
  // Log all context info for debugging
  console.log(`🔍 [BEFORE_PACK] Hook called - arch: ${arch}, appOutDir: ${appOutDir}`);
  
  // Convert arch enum to string if needed
  // 1 = x64, 2 = arm64, 3 = ia32 (based on electron-builder's Arch enum)
  if (typeof arch === 'number') {
    const archEnumMap = {
      1: 'x64',
      2: 'arm64',
      3: 'ia32'
    };
    arch = archEnumMap[arch] || 'x64';
    console.log(`🔍 [BEFORE_PACK] Converted arch enum ${context.arch} to ${arch}`);
  }
  
  if (!arch) {
    console.error('❌ [BEFORE_PACK] No architecture specified in context!');
    throw new Error('Architecture not specified in beforePack context');
  }
  
  // Skip ia32 (32-bit) - not supported on macOS anymore
  if (arch === 'ia32') {
    console.log(`⚠️  [BEFORE_PACK] Skipping ia32 build (not supported on macOS)`);
    return; // Exit early, don't rebuild
  }
  
  // Map electron-builder arch names to node-gyp arch names
  const archMap = {
    'x64': 'x64',
    'arm64': 'arm64'
  };
  
  const nodeGypArch = archMap[arch] || arch;
  
  console.log(`🔧 [BEFORE_PACK] Rebuilding native audio module for ${arch} (electron ${electronVersion})`);
  
  const nativeAudioDir = path.join(__dirname, '..', 'electron', 'native-audio');
  
  try {
    // Clean previous build (just the build directory, not node_modules)
    console.log('🧹 Cleaning previous build...');
    const buildDir = path.join(nativeAudioDir, 'build');
    if (fs.existsSync(buildDir)) {
      fs.rmSync(buildDir, { recursive: true, force: true });
    }
    
    // Rebuild for target architecture
    console.log(`🏗️  Building for ${nodeGypArch}...`);
    execSync(
      `npx node-gyp rebuild --target=${electronVersion} --arch=${nodeGypArch} --dist-url=https://electronjs.org/headers`,
      { 
        cwd: nativeAudioDir,
        stdio: 'inherit' 
      }
    );
    
    // Verify the build
    const modulePath = path.join(nativeAudioDir, 'build', 'Release', 'native_audio.node');
    if (!fs.existsSync(modulePath)) {
      throw new Error(`Native module not found at ${modulePath}`);
    }
    
    const fileOutput = execSync(`file "${modulePath}"`, { encoding: 'utf-8' });
    console.log(`✅ Native module built: ${fileOutput.trim()}`);
    
  } catch (error) {
    console.error(`❌ Failed to rebuild native module for ${arch}:`, error.message);
    throw error;
  }
};

