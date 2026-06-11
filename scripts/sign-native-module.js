const { execSync } = require('child_process');
const path = require('path');

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

  console.log('🔐 Signing native audio module with entitlements...');
  console.log(`  Path: ${nativeModulePath}`);

  try {
    execSync(
      `codesign --force --sign "Developer ID Application: AskSayso, Inc. (AFGHD8M3VK)" --options runtime --entitlements "${path.join(__dirname, '..', 'assets', 'entitlements.mac.plist')}" "${nativeModulePath}"`,
      { stdio: 'inherit' }
    );
    console.log('✅ Native audio module signed successfully');
  } catch (error) {
    console.error('❌ Failed to sign native audio module:', error.message);
    throw error;
  }
};

