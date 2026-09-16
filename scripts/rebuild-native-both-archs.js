const { execSync } = require('child_process');
const path = require('path');
const fs = require('fs');
const { archOfBinary } = require('./binary-arch');

/**
 * Rebuilds the native audio module for the host's default architecture.
 * The beforePack hook will rebuild for the correct architecture during packaging
 * This is just a fallback to ensure we have at least one architecture built
 */
// win32 is pinned ahead of process.arch on purpose: a Windows-on-ARM box would
// otherwise select arm64 and fail with MSB8020, since a standard C++ Build Tools
// install carries no ARM64 MSVC toolset.
const defaultArch =
  process.platform === 'win32' ? 'x64' : process.arch === 'arm64' ? 'arm64' : 'x64';

console.log(`🔧 Rebuilding native audio module (${defaultArch} as default)...`);
console.log('📝 Note: beforePack hook will rebuild for each architecture during packaging');

const nativeAudioDir = path.join(__dirname, '..', 'electron', 'native-audio');
const electronVersion = '36.9.1';

try {
  // Clean previous build
  const buildDir = path.join(nativeAudioDir, 'build');
  if (fs.existsSync(buildDir)) {
    fs.rmSync(buildDir, { recursive: true, force: true });
  }
  
  // Rebuild for the default arch (will be rebuilt by beforePack hook for each arch)
  console.log(`\n🏗️  Building for ${defaultArch} (default)...`);
  execSync(
    `npx node-gyp rebuild --target=${electronVersion} --arch=${defaultArch} --dist-url=https://electronjs.org/headers`,
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
  
  const builtArch = archOfBinary(modulePath);
  console.log(`✅ Default native module built: ${modulePath} (${builtArch})`);
  if (builtArch !== 'unknown' && builtArch !== defaultArch) {
    console.warn(`⚠️  Architecture mismatch: expected ${defaultArch}, built ${builtArch}`);
  }
  console.log('✅ The beforePack hook will rebuild this for each architecture during packaging');
  
} catch (error) {
  console.error(`❌ Failed to rebuild native module:`, error.message);
  throw error;
}

