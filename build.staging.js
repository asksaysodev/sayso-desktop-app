const base = require('./package.json').build;

const STAGING_APP_ID = 'com.asksayso.app.staging';

module.exports = {
  ...base,
  appId: STAGING_APP_ID,
  productName: 'Sayso [beta]',
  directories: {
    ...base.directories,
    output: 'release-staging',
  },
  mac: {
    ...base.mac,
    artifactName: 'Sayso-Beta-${version}-${arch}-mac.${ext}',
    icon: 'assets/icon-staging.icns',
  },
  win: {
    ...base.win,
    artifactName: 'Sayso-Beta-${version}-${arch}-win.${ext}',
    icon: 'assets/icon-staging.ico',
  },
  publish: {
    ...base.publish,
    channel: 'staging',
    releaseType: 'prerelease',
  },
  extraMetadata: {
    build_env: 'staging',
    name: 'sayso-app-staging',
    // main.ts reads build.appId to pin the Windows AppUserModelId, which names the
    // Launch at Login registry value. The appId above only reaches electron-builder,
    // so without this the packaged staging app.asar would still carry production's
    // appId and the two channels would fight over one Run value. extraMetadata is
    // deep-merged into package.json, so this overrides build.appId and leaves the
    // rest of the build block intact.
    build: {
      appId: STAGING_APP_ID,
    },
  },
};
