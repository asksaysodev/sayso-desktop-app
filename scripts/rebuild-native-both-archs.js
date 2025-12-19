const { execSync } = require('child_process');
const path = require('path');
const fs = require('fs');

/**
 * Rebuilds the native audio module for arm64 (default)
 * The beforePack hook will rebuild for the correct architecture during packaging
 * This is just a fallback to ensure we have at least one architecture built
 */
console.log('🔧 Rebuilding native audio module (arm64 as default)...');
console.log('📝 Note: beforePack hook will rebuild for each architecture during packaging');

const nativeAudioDir = path.join(__dirname, '..', 'electron', 'native-audio');
const electronVersion = '36.9.1';

try {
  // Clean previous build
  const buildDir = path.join(nativeAudioDir, 'build');
  if (fs.existsSync(buildDir)) {
    fs.rmSync(buildDir, { recursive: true, force: true });
  }
  
  // Rebuild for arm64 (default, will be rebuilt by beforePack hook for each arch)
  console.log(`\n🏗️  Building for arm64 (default)...`);
  execSync(
    `npx node-gyp rebuild --target=${electronVersion} --arch=arm64 --dist-url=https://electronjs.org/headers`,
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
  console.log(`✅ Default native module built: ${fileOutput.trim()}`);
  console.log('✅ The beforePack hook will rebuild this for each architecture during packaging');
  
} catch (error) {
  console.error(`❌ Failed to rebuild native module:`, error.message);
  throw error;
}

