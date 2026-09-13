const base = require('./package.json').build;

module.exports = {
  ...base,
  appId: 'com.asksayso.app.staging',
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
  },
};
