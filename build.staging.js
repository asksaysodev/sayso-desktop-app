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
    // The appId above only reaches electron-builder. main.ts needs it at RUNTIME to
    // pin the Windows AppUserModelId, which names the Launch at Login registry value,
    // so it has to be published into the packaged package.json — otherwise staging
    // runs under production's identity and the two channels fight over one Run value.
    // It has to ride on a top-level key: electron-builder strips `build` wholesale
    // when writing that file, after extraMetadata is applied, so nesting it under
    // `build` here would silently vanish. Keep in sync with appId above via the const.
    app_id: STAGING_APP_ID,
  },
};
