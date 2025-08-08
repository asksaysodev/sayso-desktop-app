import { app, BrowserWindow, ipcMain } from 'electron'
import { createProtocol } from 'vue-cli-plugin-electron-builder/lib'
const { platform, arch } = process
const { ZoomSDK } = require('../lib/zoom_sdk.js');
const isDevelopment = process.env.NODE_ENV !== 'production'
const remote = require('@electron/remote/main')
remote.initialize()

const zoomSdk = ZoomSDK.getInstance();
(app as any).zoomSdk = zoomSdk;
(app as any).platform = platform;
(app as any).arch = arch;
let win: BrowserWindow

if (platform === 'win32') {
  app.disableHardwareAcceleration()
  let gotTheLock = app.requestSingleInstanceLock()
  if (!gotTheLock) {
    app.quit()
  } else {
    app.on('second-instance', (event, commandLine, workingDirectory) => {
      // Someone tried to run a second instance, we should focus our window.
      if (win) {
        if (win.isMinimized()) win.restore()
        win.focus()
      }
    })
  }
}

async function createWindow() {
  win = new BrowserWindow({
    width: 800,
    height: 700,
    minWidth: 600,
    minHeight: 750,
    center: true,
    hasShadow: true,
    webPreferences: {
      nodeIntegration: true,
      contextIsolation: false
    }
  })
  
  if (process.env.WEBPACK_DEV_SERVER_URL) {
    await win.loadURL(process.env.WEBPACK_DEV_SERVER_URL as string)
    if (!process.env.IS_TEST) win.webContents.openDevTools()
  } else {
    createProtocol('app')
    win.loadURL('app://./index.html')
  }

  win.on('ready-to-show', () => {
    console.log('ready-to-show')
    win.show()
  })

  win.on('close', () => { 
    let zoomSdkModule = (app as any).zoomSdkModule
    if (zoomSdkModule) {
      zoomSdkModule.Meeting.LeaveMeeting()
    }
  })
}

(global as any).platform = platform

ipcMain.on('auth-success', (event, arg) => {
  (app as any).zoomSdkModule = { Auth: zoomSdk.GetAuth(), Meeting: zoomSdk.GetMeeting(), Setting: zoomSdk.GetSetting() }
})

app.on('will-quit', () => {
  zoomSdk.CleanUPSDK();
  app.quit()
})

app.on('activate', () => {
  if (BrowserWindow.getAllWindows().length === 0) createWindow()
})

app.on('ready', async () => {
  createWindow()
})

app.on("browser-window-created", (e, win) => {
  win.center()
});

app.on("web-contents-created", (e, webContents) => {
  remote.enable(webContents);
});

if (isDevelopment) {
  if (process.platform === 'win32') {
    process.on('message', (data) => {
      if (data === 'graceful-exit') {
        app.quit()
      }
    })
  } else {
    process.on('SIGTERM', () => {
      app.quit()
    })
  }
}
