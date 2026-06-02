const base = require('./package.json').build;

module.exports = {
  ...base,
  appId: 'com.asksayso.app.staging',
  productName: 'Sayso Staging',
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
    channel: 'latest-staging',
    releaseType: 'prerelease',
  },
  extraMetadata: {
    build_env: 'staging',
  },
};
