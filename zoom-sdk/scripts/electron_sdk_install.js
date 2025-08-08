const { execSync } = require('child_process')
const { existsSync } = require('fs')
const { platform } = process

if (platform == 'darwin') {
  if (!existsSync('./node_modules/electron/dist/Electron.app/Contents/Frameworks/ZoomSDK.framework')) {
    execSync('chmod 777 ./node_modules/electron/dist/Electron.app/Contents/', {stdio: [0, 1, 2]})
    execSync('cp -Rf ./sdk/mac/Plugins/* ./node_modules/electron/dist/Electron.app/Contents', {stdio: [0, 1, 2]})
    execSync('cp -Rf ./sdk/mac/ZoomSDK/* ./node_modules/electron/dist/Electron.app/Contents/Frameworks', {stdio: [0, 1, 2]})
  }
}