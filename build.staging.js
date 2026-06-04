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
    icon: 'assets/icon-staging.icns',
  },
  publish: {
    ...base.publish,
    channel: 'staging',
    releaseType: 'prerelease',
  },
  extraMetadata: {
    build_env: 'staging',
  },
};
