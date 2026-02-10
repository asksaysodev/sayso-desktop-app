/// <reference path="./globals.d.ts" />
import type { 
  BrowserWindow as BrowserWindowType, 
  DevicePermissionHandlerHandlerDetails, 
  Tray as TrayType,
  WebContents
} from 'electron';
import type { Event } from 'electron';
import type { 
  AuthUser, 
  AudioQueueItem, 
  AudioCaptureOptions, 
  CueInsight 
} from './globals';

import { app, BrowserWindow, ipcMain, screen as electronScreen, shell, systemPreferences, globalShortcut, dialog, Tray, Menu } from 'electron';
import path from 'node:path';
import fs from 'node:fs';
import { nativeImage } from 'electron/common';
import * as Sentry from '@sentry/electron/main';
import sentryConfig from './sentry.config';
import { WindowManager } from './utils/windowManager';

Sentry.init(sentryConfig);

let autoUpdater: import('electron-updater').AppUpdater | null = null;

// Global error handler to prevent app crashes from unhandled exc eptions
// (e.g. native module failures on unsupported hardware)
process.on('uncaughtException', (error) => {
  console.error('[MAIN] Uncaught Exception:', error);
  Sentry.captureException(error);
  // Do NOT exit the process. This allows the app to stay alive
  // so the auto-updater can still run or the user can see an error UI.
});

if (app.isPackaged) {
  const { autoUpdater: updater } = require('electron-updater');
  const log = require('electron-log');

  updater.logger = log;
  updater.logger.transports.file.level = 'info';
  log.info('Auto-updater initialized');

  updater.autoDownload = true;
  updater.autoInstallOnAppQuit = true;
  updater.allowDowngrade = false;

  updater.on('checking-for-update', () => {
    log.info('Checking for updates...');
  });

  updater.on('update-available', (info: { version: string }) => {
    log.info('Update available:', info.version);
    log.info('Downloading update...');
  });

  updater.on('update-not-available', (info: { version: string }) => {
    log.info('Update not available. Current version:', info.version);
  });

  updater.on('error', (err: Error) => {
    log.error('Error in auto-updater:', err);
    Sentry.captureException(err);
  });

  updater.on('download-progress', (progressObj: { percent?: number; transferred?: number; total?: number; bytesPerSecond?: number }) => {
    const percent = progressObj.percent ? progressObj.percent.toFixed(2) : 0;
    const transferred = progressObj.transferred || 0;
    const total = progressObj.total || 0;
    const speed = progressObj.bytesPerSecond || 0;
    log.info(`Download progress: ${percent}% (${transferred}/${total} bytes) - Speed: ${speed} bytes/sec`);
  });

  updater.on('update-downloaded', (info: { version: string }) => {
    log.info('Update downloaded:', info.version);

    dialog.showMessageBox({
      type: 'info',
      title: 'Update Ready',
      message: `Version ${info.version} has been downloaded`,
      detail: 'The update will be installed when you quit and restart the app.',
      buttons: ['Restart Now', 'Later']
    }).then((result: { response: number }) => {
      if (result.response === 0) {
        // User clicked "Restart Now"
        setImmediate(() => {
          app.removeAllListeners('window-all-closed');
          updater.quitAndInstall(false, true);
        });
      }
    });
  });

  // Assign to module-level variable for use elsewhere
  autoUpdater = updater;
}

// Native audio module - will be loaded after logging is set up
let nativeAudio: any = null;

// Add file logging for production
function setupLogging() {
  // Always create a debug file to see what's happening
  const debugPath = path.join(app.getPath('userData'), 'debug-startup.log');
  
  try {
    const debugInfo = {
      timestamp: new Date().toISOString(),
      appName: app.getName(),
      nodeEnv: process.env.NODE_ENV,
      userDataPath: app.getPath('userData'),
      processType: process.type
    };

    fs.writeFileSync(debugPath, JSON.stringify(debugInfo, null, 2));
  } catch (error) {
    console.error('[DEBUG] Error creating debug file:', error);
    Sentry.captureException(error);
  }
  
  // Then proceed with normal logging setup
  if (process.env.NODE_ENV === 'production') {
    const logDir = path.join(app.getPath('userData'), 'logs');
    if (!fs.existsSync(logDir)) {
      fs.mkdirSync(logDir, { recursive: true });
    }
    
    const logFile = path.join(logDir, `sayso-${new Date().toISOString().split('T')[0]}.log`);
    
    // Redirect console.log to both console and file
    const originalLog = console.log;
    const originalError = console.error;
    const originalWarn = console.warn;
    
    function writeToFile(level: string, ...args: any[]) {
      const timestamp = new Date().toISOString();
      const message = args.map(arg => 
        typeof arg === 'object' ? JSON.stringify(arg, null, 2) : String(arg)
      ).join(' ');
      
      const logEntry = `[${timestamp}] [${level}] ${message}\n`;
      
      try {
        fs.appendFileSync(logFile, logEntry);
      } catch (err: any) {
        // Fallback to original console if file writing fails
        originalError(`Failed to write to log file: ${err.message}`);
      }
    }
    
    console.log = (...args) => {
      originalLog(...args);
      writeToFile('INFO', ...args);
    };
    
    console.error = (...args) => {
      originalError(...args);
      writeToFile('ERROR', ...args);
    };
    
    console.warn = (...args) => {
      originalWarn(...args);
      writeToFile('WARN', ...args);
    };
    
    console.log(`[MAIN] Logging to file: ${logFile}`);
  }
}

// ===== HELPER FUNCTIONS =====
function isCoachWindowOpen() {
  return global.coachWindow && !global.coachWindow.isDestroyed();
}

// ===== CUSTOM TRAY MENU WINDOW =====
let tray: TrayType | null = null;
let trayMenuWindow: BrowserWindowType | null = null;

/**
 * Creates and positions the custom tray menu window near the tray icon
 */
function createTrayMenuWindow() {
  if (trayMenuWindow && !trayMenuWindow.isDestroyed()) {
    if (trayMenuWindow.isVisible()) {
      hideTrayMenu();
    } else {
      showTrayMenu();
    }
    return;
  }

  const preloadScriptPath = path.join(__dirname, 'preload.js');
  const allowVibrancy: boolean = process.platform === 'darwin' && process.arch !== 'x64'; 

  // Create a frameless, always-on-top window
  trayMenuWindow = new BrowserWindow({
    width: 264,
    height: 128,
    show: false,
    frame: false,
    transparent: true,
    resizable: false,
    movable: false,
    minimizable: false,
    maximizable: false,
    alwaysOnTop: true,
    skipTaskbar: true,
    hasShadow: true,
    vibrancy: allowVibrancy ? 'menu' : undefined,
    visualEffectState: allowVibrancy ? 'active' : undefined,
    backgroundColor: allowVibrancy ? '#00000000' : '#F9FAFB',
    webPreferences: {
      preload: preloadScriptPath,
      contextIsolation: true,
      nodeIntegration: false,
      webSecurity: true,
      backgroundThrottling: false,
    },
  });

  const trayMenuUrl = isDev
    ? 'http://localhost:5173/tray-menu.html'
    : `file://${path.join(__dirname, '../dist/tray-menu.html')}`;

  if (trayMenuWindow) {
    trayMenuWindow.loadURL(trayMenuUrl);
    
    trayMenuWindow.on('closed', () => {
      trayMenuWindow = null;
    });
  }
}

/**
 * Shows the tray menu window positioned near the tray icon
 */
function showTrayMenu() {
  if (!trayMenuWindow || trayMenuWindow.isDestroyed()) {
    createTrayMenuWindow();
    // Wait a bit for the window to be created before positioning
    setTimeout(() => {
      if (trayMenuWindow && !trayMenuWindow.isDestroyed()) {
        positionTrayMenu();
        trayMenuWindow.show();
      }
    }, 100);
  } else {
    positionTrayMenu();
    trayMenuWindow.show();
  }

  // Send initial coach window state
  if (trayMenuWindow && !trayMenuWindow.isDestroyed()) {
    trayMenuWindow.webContents.send('coach-window-state', {
      isOpen: isCoachWindowOpen()
    });
  }
}

/**
 * Hides the tray menu window
 */
function hideTrayMenu() {
  if (trayMenuWindow && !trayMenuWindow.isDestroyed()) {
    trayMenuWindow.hide();
  }
}

/**
 * Positions the tray menu window near the tray icon
 * macOS: positions below the menu bar on the right side
 */
function positionTrayMenu() {
  if (!trayMenuWindow || trayMenuWindow.isDestroyed() || !tray) return;

  const trayBounds = tray.getBounds();
  const windowBounds = trayMenuWindow.getBounds();
  const primaryDisplay = electronScreen.getPrimaryDisplay();
  const workArea = primaryDisplay.workArea;

  let x, y;

  if (process.platform === 'darwin') {
    // macOS: Position below menu bar, aligned with tray icon
    x = Math.round(trayBounds.x + (trayBounds.width / 2) - (windowBounds.width / 2));
    y = Math.round(trayBounds.y + trayBounds.height + 5);
    
    // Ensure window stays within screen bounds
    if (x + windowBounds.width > workArea.x + workArea.width) {
      x = workArea.x + workArea.width - windowBounds.width - 5;
    }
    if (x < workArea.x) {
      x = workArea.x + 5;
    }
  } else if (process.platform === 'win32') {
    // Windows: Position above taskbar, aligned with tray icon
    x = Math.round(trayBounds.x + (trayBounds.width / 2) - (windowBounds.width / 2));
    y = Math.round(trayBounds.y - windowBounds.height - 5);
  } else {
    // Linux: Position below tray icon
    x = Math.round(trayBounds.x + (trayBounds.width / 2) - (windowBounds.width / 2));
    y = Math.round(trayBounds.y + trayBounds.height + 5);
  }

  trayMenuWindow.setPosition(x, y, false);
}

/**
 * Registers the tray icon and sets up click handlers
 */
function registerTrayIconMenu() {
  const iconPath = path.join(__dirname, '../public/assets/tray-icon44Template.png');
  
  let icon = nativeImage.createFromPath(iconPath);
  
  if (icon.isEmpty()) {
    console.error('Tray icon failed to load! Icon is empty.');
    Sentry.captureMessage('Tray icon failed to load - icon is empty', 'error');
    return;
  }
  
  icon = icon.resize({ width: 19, height: 19 });
  icon.setTemplateImage(true);
  
  tray = new Tray(icon);
  if (tray) {
    tray.setToolTip('Sayso');

    tray.on('click', () => {
      if (trayMenuWindow && !trayMenuWindow.isDestroyed() && trayMenuWindow.isVisible()) {
        hideTrayMenu();
      } else {
        showTrayMenu();
      }
    });

    tray.on('right-click', () => {
      if (trayMenuWindow && !trayMenuWindow.isDestroyed() && trayMenuWindow.isVisible()) {
        hideTrayMenu();
      } else {
        showTrayMenu();
      }
    });
  }
}

/**
 * Updates tray menu state when coach window opens/closes
 */
function updateTrayMenu() {
  if (trayMenuWindow && !trayMenuWindow.isDestroyed()) {
    trayMenuWindow.webContents.send('coach-window-state', {
      isOpen: isCoachWindowOpen()
    });
  }
}

// ===== GLOBAL SHORTCUTS ===== 
const shortcuts = [
  {
    // Open coach window widget
    fn: () => {
      if (!global.authUser || global.authUser?.subscription_plan_id === null) return;
      
      if (isCoachWindowOpen()) {
        global.coachWindow?.close();
      } else {
        createCoachWindow();
      }
    },
    keyCombination: 'Control+S'
  }
];

function setupGlobalShortcut() {
  shortcuts.forEach(({ fn, keyCombination }) => {
    globalShortcut.register(keyCombination, () => {
      fn();
    });
  })
}

function unregisterGlobalShortcuts() {
  shortcuts.forEach(({ keyCombination }) => {
    globalShortcut.unregister(keyCombination);
  })
}

// Load environment variables FIRST, before any other modules
if (!process.env.NODE_ENV) {
  process.env.NODE_ENV = 'production';
}

// Store active recording session metadata

// Store AudioStreamer instance
let audioStreamer: any = null;

// Store Cue instances (separate from regular streaming)
let cueAudioStreamer: any = null;

/**
 * Cleanup all audio capture resources (screen capture, microphone, streams)
 * Called when coach window closes or app quits to ensure permissions are released
 */
async function cleanupAllAudioCapture() {
  if (isDev) {
    console.log('[Cleanup] Starting audio capture cleanup...');
  }
  
  try {
    // 1. Stop user streaming (handles global.userStreamingProcess)
    await stopUserStreaming();
    
    // 2. Stop system audio capture (screen recording) via native module
    if (nativeAudio) {
      await nativeAudio.stopSystemAudioCapture();
      nativeAudio.setStreamingCallback(null);
    }
    
    // 3. Stop user full recording FFmpeg process
    if (global.userFullRecordingProcess) {
      try {
        global.userFullRecordingProcess.kill('SIGINT');
        await new Promise(resolve => setTimeout(resolve, 200));
        global.userFullRecordingProcess = null;
      } catch (error) {
        console.error('[Cleanup] Error stopping user recording process:', error);
        Sentry.captureException(error);
        global.userFullRecordingProcess = null;
      }
    }
    
    // 4. Stop user MediaRecorder
    if (global.userMediaRecorder) {
      try {
        global.userMediaRecorder.stop();
        global.userMediaRecorder = null;
      } catch (error) {
        console.error('[Cleanup] Error stopping user MediaRecorder:', error);
        Sentry.captureException(error);
        global.userMediaRecorder = null;
      }
    }
    
    // 5. Stop user audio stream tracks
    if (global.userAudioStream) {
      try {
        global.userAudioStream.getTracks().forEach(track => track.stop());
        global.userAudioStream = null;
      } catch (error) {
        console.error('[Cleanup] Error stopping user audio stream:', error);
        Sentry.captureException(error);
        global.userAudioStream = null;
      }
    }
    
    // 6. Stop prospect MediaRecorder
    if (global.mediaRecorder) {
      try {
        global.mediaRecorder.stop();
        global.mediaRecorder = null;
      } catch (error) {
        console.error('[Cleanup] Error stopping prospect MediaRecorder:', error);
        Sentry.captureException(error);
        global.mediaRecorder = null;
      }
    }
    
    // 7. Stop prospect audio stream tracks
    if (global.prospectAudioStream) {
      try {
        global.prospectAudioStream.getTracks().forEach(track => track.stop());
        global.prospectAudioStream = null;
      } catch (error) {
        console.error('[Cleanup] Error stopping prospect audio stream:', error);
        Sentry.captureException(error);
        global.prospectAudioStream = null;
      }
    }
    
    // 8. Stop legacy ScreenCaptureKit instance
    if (global.screenCapture) {
      try {
        await global.screenCapture.stopSystemAudioCapture();
        global.screenCapture = null;
      } catch (error) {
        console.error('[Cleanup] Error stopping ScreenCaptureKit:', error);
        Sentry.captureException(error);
        global.screenCapture = null;
      }
    }
    
    // 9. Stop Cue audio streamer
    if (cueAudioStreamer) {
      try {
        await cueAudioStreamer.stop(false);
        cueAudioStreamer = null;
      } catch (error) {
        console.error('[Cleanup] Error stopping cue audio streamer:', error);
        Sentry.captureException(error);
        cueAudioStreamer = null;
      }
    }
    
    // 10. Stop regular audio streamer
    if (audioStreamer) {
      try {
        await audioStreamer.stop(false);
        audioStreamer = null;
      } catch (error) {
        console.error('[Cleanup] Error stopping audio streamer:', error);
        Sentry.captureException(error);
        audioStreamer = null;
      }
    }
    
    // Clear file path globals
    global.userRecordingFile = null;
    global.prospectRecordingFile = null;
    global.userActualStartMs = null;
    
    if (isDev) {
      console.log('[Cleanup] All audio capture cleaned up');
    }
  } catch (error) {
    console.error('[Cleanup] Error during audio cleanup:', error);
    Sentry.captureException(error);
  }
}

// Get streaming status
ipcMain.handle('get-streaming-status', async () => {
  if (!audioStreamer) {
    return { isStreaming: false };
  }
  return {
    isStreaming: audioStreamer.isStreamingActive(),
    userState: audioStreamer.getUserState(),
    prospectState: audioStreamer.getProspectState(),
    sessionId: audioStreamer.sessionId
  };
});

// Start Cue (handles 2 audio websockets: user + prospect)
ipcMain.handle('start-cue', async (event: Electron.IpcMainInvokeEvent, { sessionId, token }: { sessionId: string, token: string }) => {
  try {
    
    if (!token) {
      throw new Error('Token is required');
    }

    if (!sessionId) {
      throw new Error('SessionId is required');
    }

    if (!nativeAudio) {
      throw new Error('Native audio module not loaded. Please wait for app initialization.');
    }

    // Create AudioStreamer for 2 audio websockets (user + prospect)
    cueAudioStreamer = new AudioStreamer({
      sessionId: sessionId, // Use provided sessionId from backend
      onUserConnected: () => {
        try {
          if (!event.sender.isDestroyed()) {
            event.sender.send('cue-status', { user: 'connected' });
          }
        } catch (error) {
          console.error('[Cue] Error sending user connected status:', error);
        }
      },
      onProspectConnected: () => {
        try {
          if (!event.sender.isDestroyed()) {
            event.sender.send('cue-status', { prospect: 'connected' });
          }
        } catch (error) {
          console.error('[Cue] Error sending prospect connected status:', error);
        }
      },
      onError: (stream: string, error: Error) => {
        console.error(`[Cue] ${stream} stream error:`, error);
        try {
          if (!event.sender.isDestroyed()) {
            event.sender.send('cue-error', { stream, error: error.message });
          }
        } catch (err) {
          console.error('[Cue] Error sending error status:', err);
        }
      },
      onMessage: (message: any) => {
        try {
          // Forward insight messages to renderer process
          if (message && message.type === 'insight' && message.data) {
            // Forward to coach window if it exists
            if (global.coachWindow && !global.coachWindow.isDestroyed()) {
              global.coachWindow.webContents.send('cue-insight', message.data);
              if (isDev) {
                console.log('[MAIN] Insight forwarded to coach window:', message.data);
              }
            }
          }

          if (message && message.type === 'auto_stop') {
            if (global.coachWindow && !global.coachWindow.isDestroyed()) {
              global.coachWindow.webContents.send('cue-auto-stop');
              if (isDev) {
                console.log('[MAIN] Auto stop forwarded to coach window');
              }
              
              if (process.platform === 'darwin') {
                app.setBadgeCount(app.getBadgeCount() + 1);
                if (app.dock) {
                  app.dock.bounce('critical');
                }
              }
            } 
          }
        } catch (error) {
          console.error('[Cue] Error handling message:', error);
        }
      }
    });

    // Start audio streaming (2 websockets)
    await cueAudioStreamer.start(token);

    // Set up audio capture callbacks (streaming only - no file saving)
    await startUserStreaming({
      streamingCallback: (buffer: Buffer, format: string) => {
        if (cueAudioStreamer) {
          cueAudioStreamer.addUserAudio(buffer, format);
        }
      }
    });

    if (!nativeAudio) {
      throw new Error('Native audio module not loaded. Please wait for app initialization.');
    }

    if (typeof nativeAudio.startProspectStreaming !== 'function') {
      throw new Error('startProspectStreaming method not available. Native module may need to be rebuilt.');
    }

    await nativeAudio.startProspectStreaming({
      streamingCallback: (buffer: Buffer, format: string) => {
        if (cueAudioStreamer) {
          cueAudioStreamer.addProspectAudio(buffer, format);
        }
      }
    });

    return { 
      success: true, 
      sessionId: sessionId 
    };
  } catch (error: any) {
    console.error('[MAIN] Error starting Cue:', error);
    Sentry.captureException(error);
    // Clean up on error
    cueAudioStreamer = null;
    return { success: false, error: error.message };
  }
});

// Stop Cue (closes all websockets)
ipcMain.handle('stop-cue', async (event: Electron.IpcMainInvokeEvent) => {
  try {
    // Stop audio streaming (streaming only - no file recording to stop)
    await stopUserStreaming();
    if (nativeAudio) {
      await nativeAudio.stopSystemAudioCapture();
      nativeAudio.setStreamingCallback(null);
    }

    // Stop audio websockets (2)
    if (cueAudioStreamer) {
      await cueAudioStreamer.stop(false); // Don't send termination message
      cueAudioStreamer = null;
    }

    // TODO: Stop insights websocket (1) when implemented
    // if (cueInsightsWebSocket) {
    //   await cueInsightsWebSocket.disconnect();
    //   cueInsightsWebSocket = null;
    // }

    return { success: true };
  } catch (error: any) {
    console.error('[MAIN] Error stopping Cue:', error);
    Sentry.captureException(error);
    // Force cleanup on error
    cueAudioStreamer = null;
    return { success: false, error: error.message };
  }
});


// Function to load environment variables
function loadEnvironmentVariables() {
  const isDev = process.env.NODE_ENV !== 'production';
  
  if (isDev) {
    // Development: load from project directory
    require('dotenv').config({ path: path.resolve(__dirname, '../.env') });
  } else {
    // Production: try multiple locations in order of preference
    const possiblePaths = [
      path.resolve(__dirname, '.env.production'), // In electron directory
      path.resolve(__dirname, '../.env.production'), // In dist directory
      path.resolve(__dirname, '../dist/.env.production'), // Alternative dist path
    ];
    
    let loaded = false;
    for (const envPath of possiblePaths) {
      if (fs.existsSync(envPath)) {
        require('dotenv').config({ path: envPath });
        if (isDev) {
          console.log(`[MAIN] Loaded environment from: ${envPath}`);
        }
        loaded = true;
        break;
      } else {
        if (isDev) {
          console.log(`[MAIN] Not found: ${envPath}`);
        }
      }
    }
    
    if (!loaded) {
      console.warn('[MAIN] No .env.production file found, using defaults');
      // Set production defaults
      process.env.VITE_BACKEND_BASE_URL = 'https://your-production-server.com';
    }
  }
}

// Load environment variables
loadEnvironmentVariables();

// Now require other modules that depend on environment variables
const wav = require('wav');
const NodeFormData = require('form-data');
const axios = require('axios');
const { 
  stopUserStreaming,
  startUserStreaming
} = require('./recorder');
const audioQueue = require('./audioQueue');
const { AudioStreamer } = require('./streaming/audioStreamer');
const { getAuthToken } = require('./utils/authTokens');

// Add command line switches for better camera support
app.commandLine.appendSwitch('enable-features', 'WebRTC,MediaDevices,MediaStream');
app.commandLine.appendSwitch('enable-media-stream');
app.commandLine.appendSwitch('enable-usermedia-screen-capturing');
app.commandLine.appendSwitch('allow-running-insecure-content');
app.commandLine.appendSwitch('disable-web-security');
app.commandLine.appendSwitch('disable-features', 'VizDisplayCompositor');

// <<< Disable hardware acceleration >>>
// This can fix GPU process crashes on some systems
// app.disableHardwareAcceleration();
app.setAsDefaultProtocolClient('sayso');

// Handle creating/removing shortcuts on Windows when installing/uninstalling.
if (require('electron-squirrel-startup')) {
  app.quit();
}

// Prefer Electron's packaging flag to detect development vs production
const isDev = !app.isPackaged;

// Keep track of window instances
let dashboardWindowInstance: BrowserWindowType | null = null;

// --- Dashboard Window (Standard Window) ---
const createDashboardWindow = () => {
  if (dashboardWindowInstance) {
      dashboardWindowInstance.focus();
      return;
  }
  const preloadScriptPath = path.join(__dirname, 'preload.js');
  if (isDev) {
    console.log(`[MAIN]: Dashboard preload path calculated as: ${preloadScriptPath}`);
    if (fs.existsSync(preloadScriptPath)) {
      console.log(`[MAIN]: Preload script FOUND at: ${preloadScriptPath}`);
    }
  }
  if (!fs.existsSync(preloadScriptPath)) {
    console.error(`[MAIN]: Preload script NOT FOUND at: ${preloadScriptPath}`);
  }
  const indexHtmlPath = path.join(process.resourcesPath, 'dist', 'index.html');
  const allowVibrancy: boolean = process.platform === 'darwin' && process.arch !== 'x64'; 
  const dashboardWindow = new BrowserWindow({
    width: 1400,
    height: 900,
    icon: path.join(__dirname, '../public/assets/icon.icns'),
    // Prevent fullscreen and maximize, but allow manual resizing
    maximizable: false,
    fullscreenable: false,
    roundedCorners: true,
    vibrancy: allowVibrancy ? 'under-window': undefined,
    visualEffectState: allowVibrancy ? 'active' : undefined,
    titleBarStyle: 'hiddenInset',
    // titleBarStyle: 'hidden',
    titleBarOverlay: {
      color: '#ffffff',
      symbolColor: '#000000',
      height: 30
    },
    webPreferences: {
      preload: preloadScriptPath,
      contextIsolation: true,
      nodeIntegration: false,
      webSecurity: true,
      // Enhanced media permissions for packaged app
      enableBlinkFeatures: 'MediaDevices,MediaStream,WebRTC',
      // permissions: ['media', 'microphone'], 'permissions' does not exist in type 'WebPreferences'.
      // Add these for better camera support
      allowRunningInsecureContent: false,
      experimentalFeatures: false,
      additionalArguments: [`--indexHtmlPath=${indexHtmlPath}`]
    },
  });
  dashboardWindowInstance = dashboardWindow;
  global.mainWindow = dashboardWindow;
  const dashboardUrl = isDev 
    ? 'http://localhost:5173/#/' 
    : `file://${path.join(__dirname, '../dist/index.html')}#/`;
  dashboardWindow.loadURL(dashboardUrl);
  if (isDev) {
    dashboardWindow.webContents.openDevTools();
  }
  dashboardWindow.on('closed', () => {
      dashboardWindowInstance = null;
  });

  // Enhanced media permissions handler for packaged app
  dashboardWindow.webContents.session.setPermissionRequestHandler((webContents: WebContents, permission: string, callback: (allowed: boolean) => void) => {
    const allowedPermissions = ['media', 'microphone', 'display-capture'];
    if (isDev) {
      console.log(`[MAIN] Permission requested: ${permission}`);
    }
    
    if (allowedPermissions.includes(permission)) {
      if (isDev) {
        console.log(`[MAIN] Granting permission for: ${permission}`);
      }
      // Grant permission immediately for media access
      callback(true);
    } else {
      if (isDev) {
        console.log(`[MAIN] Denying permission for: ${permission}`);
      }
      callback(false);
    }
  });

  // Enhanced device permission handler
  dashboardWindow.webContents.session.setDevicePermissionHandler((details): boolean => {
    const { deviceType, origin, device } = details;
    if (isDev) {
      const deviceId = 'deviceId' in device ? device.deviceId : 'unknown';
      console.log(`[MAIN] Device permission requested for: ${deviceType} from ${origin} (${deviceId})`);
    }
    // Always allow device access for camera/microphone
    return true;
  });

  // Initialize media devices with better error handling
  dashboardWindow.webContents.on('did-finish-load', () => {
    if (isDev) {
      console.log('[MAIN] Window loaded, initializing media devices...');
    }
    dashboardWindow.webContents.executeJavaScript(`
      (function() {
        ${isDev ? "console.log('[MAIN] Initializing media devices in renderer...');" : ''}
        
        // Check if mediaDevices is available
        if (!navigator.mediaDevices) {
          console.error('[MAIN] navigator.mediaDevices is not available');
          return;
        }
        
        // Do NOT auto-request permissions here. We only show macOS dialogs
        // after the user confirms our custom modal (via permissions.requestAll).
        ${isDev ? "console.log('[MAIN] Skipping automatic getUserMedia to avoid prompting macOS dialogs early.');" : ''}
        
        // Enumerate devices
        navigator.mediaDevices.enumerateDevices()
          .then(devices => {
            ${isDev ? "console.log('[MAIN] Available media devices:', devices.map(d => ({ kind: d.kind, deviceId: d.deviceId, label: d.label })));" : ''}
          })
          .catch(err => {
            console.error('[MAIN] Error enumerating devices:', err);
          });
      })();
    `);
  });

  // Handle device change events
  dashboardWindow.webContents.on('media-devices-changed' as any, () => {
    if (isDev) {
      console.log('[MAIN] Media devices changed, reinitializing...');
    }
    dashboardWindow.webContents.executeJavaScript(`
      if (navigator.mediaDevices && navigator.mediaDevices.enumerateDevices) {
        navigator.mediaDevices.enumerateDevices()
          .then(devices => {
            ${isDev ? "console.log('[MAIN] Updated media devices:', devices.map(d => ({ kind: d.kind, deviceId: d.deviceId, label: d.label })));" : ''}
          })
          .catch(err => console.error('[MAIN] Error enumerating devices:', err));
      }
    `);
  });

  // Enhanced CSP for better camera support
  dashboardWindow.webContents.session.webRequest.onHeadersReceived((details: Electron.OnHeadersReceivedListenerDetails, callback: (response: Electron.HeadersReceivedResponse) => void) => {
    const cspHeader = "default-src * 'unsafe-inline' 'unsafe-eval' data: blob:; " +
      "script-src * 'unsafe-inline' 'unsafe-eval' data: blob:; " +
      "style-src * 'unsafe-inline' data: blob:; " +
      "img-src * data: blob:; " +
      "connect-src * data: blob:; " +
      "font-src * data: blob:; " +
      "media-src * data: blob:; " +
      "frame-src * data: blob:; " +
      "worker-src * data: blob:; " +
      "child-src * data: blob:;";

    callback({
      responseHeaders: {
        ...details.responseHeaders,
        "Content-Security-Policy": [cspHeader]
      }
    });
  });

  dashboardWindow.webContents.setWindowOpenHandler(({ url }: { url: string }) => {
    if (isDev) {
      console.log('[Electron][setWindowOpenHandler] Attempt to open URL:', url);
    }
    // If the url is a file:// index.html with a hash, route it in the main window
    if (url.startsWith('file://') && url.includes('index.html#/post-call/')) {
      if (isDev) {
        console.log('[Electron][setWindowOpenHandler] Intercepted leaveUrl, loading in main window:', url);
      }
      dashboardWindow.loadURL(url);
      return { action: 'deny' }; // Prevent new window
    }
    return { action: 'allow' };
  });

  dashboardWindow.webContents.on('will-navigate', (event: Event, url: string) => {
    if (isDev) {
      console.log('[Electron][will-navigate] Navigation attempt to:', url);
    }

    // Patch for malformed protocol
    if (url.startsWith('https://file///')) {
      event.preventDefault();
      // Fix to 'file:///' (with colon and three slashes)
      const fixedUrl = url.replace('https://file///', 'file:///');
      if (isDev) {
        console.log('[Electron][will-navigate] Fixed protocol, loading:', fixedUrl);
      }
      dashboardWindow.loadURL(fixedUrl);
    }
  });

};

// Handler for opening URLs externally
ipcMain.on('open-external', (event: Electron.IpcMainInvokeEvent, url: string) => {
  if (isDev) {
    console.log('[MAIN] [Electron][open-external] IPC event received!');
    console.log('[MAIN] [Electron][open-external] URL:', url);
    console.log('[MAIN] [Electron][open-external] Event sender window ID:', event.sender.id);
  }
  
  try {
    shell.openExternal(url);
    if (isDev) {
      console.log('[MAIN] [Electron][open-external] URL opened externally successfully');
    }
    
    // If it's a Slack OAuth URL, send reset-to-home
    if (url.includes('slack/auth')) {
      if (isDev) {
        console.log('[MAIN] [Electron][open-external] Slack auth detected, sending reset-to-home [TRIGGER #1]');
      }
      BrowserWindow.getAllWindows().forEach((win: BrowserWindowType) => {
        if (isDev) {
          console.log('[MAIN] [Electron][open-external] Sending reset-to-home to window:', win.id);
        }
        win.webContents.send('reset-to-home', { source: 'open-external-ipc', service: 'slack' });
      });
    }
  } catch (error) {
    console.error('[MAIN] [Electron][open-external] Error opening URL externally:', error);
    Sentry.captureException(error);
  }
});

// --- Permissions Handlers ---
// Check current permissions (non-interactive)
ipcMain.handle('permissions-check', async () => {
  try {
    const micStatus = systemPreferences.getMediaAccessStatus('microphone');
    if (isDev) {
      console.log('[Permissions] check: micStatus =', micStatus, '(screen reported as false by design)');
    }
    const mic = micStatus === 'granted';
    // macOS does not provide a reliable non-interactive API to check ScreenCaptureKit permission.
    // We'll return false here and request explicitly via native module when needed.
    const screen = false;
    return { mic, screen };
  } catch (e: any) {
    console.error('[MAIN] [Permissions] Error checking permissions:', e);
    Sentry.captureException(e);
    return { mic: false, screen: false, error: e.message };
  }
});

// Request microphone + screen recording permissions (interactive)
ipcMain.handle('permissions-request-all', async () => {
  try {
    // 1) MICROPHONE FIRST (never triggers app restart)
    const micStatus = systemPreferences.getMediaAccessStatus('microphone');
    if (isDev) {
      console.log('[Permissions] requestAll: initial micStatus =', micStatus);
    }
    let mic = false;
    let micAction = 'none';
    if (micStatus === 'granted') {
      mic = true;
      micAction = 'already-granted';
    } else if (micStatus === 'not-determined') {
      mic = await systemPreferences.askForMediaAccess('microphone');
      micAction = 'asked';
    } else {
      // denied, restricted, unknown — open System Settings to guide the user
      mic = false;
      micAction = 'open-settings';
      try {
        await (systemPreferences as any).openSystemPreferences('privacy', 'Microphone');
      } catch (e: any) {
        console.warn('[MAIN] [Permissions] Could not open System Settings for Microphone:', e?.message || e);
      }
    }

    // If mic isn't granted, stop here and let the UI guide the user
    if (!mic) {
      if (isDev) {
        console.log('[Permissions] requestAll: mic not granted, skipping screen request');
      }
      return { mic, screen: false, micStatus, screenStatus: 'skipped', micAction };
    }

    // 2) SCREEN RECORDING (may require restart on first grant)
    let screen = false;
    let screenRequested = false;
    try {
      if (nativeAudio && typeof nativeAudio.requestScreenRecordingPermission === 'function') {
        const res = await nativeAudio.requestScreenRecordingPermission();
        screen = !!res;
        screenRequested = true;
      } else {
        console.warn('[MAIN] [Permissions] Native module missing requestScreenRecordingPermission');
      }
    } catch (err: any) {
      console.warn('[MAIN] [Permissions] Screen permission request failed:', err?.message || err);
      screen = false;
    }
    if (isDev) {
      console.log('[Permissions] requestAll result:', { mic, screen, micAction, screenRequested });
    }
    return { mic, screen, micAction, screenRequested };
  } catch (e: any) {
    console.error('[MAIN] [Permissions] Error requesting permissions:', e);
    Sentry.captureException(e);
    return { mic: false, screen: false, error: e.message };
  }
});

// Upload file handler - reads file from disk and uploads to server
ipcMain.handle('upload-file', async (event: Electron.IpcMainInvokeEvent, { filePath, type, parentId, accessToken, fileName, data }: { filePath: string, type: string, parentId: string, accessToken: string, fileName: string, data: any }) => {
  if (isDev) {
    console.log('[Main Process] Uploading file:', { filePath, type, parentId, fileName });
  }
  
  try {
    if (!fs.existsSync(filePath)) {
      throw new Error(`File not found: ${filePath}`);
    }
    
    if (!type || !parentId || !accessToken) {
      throw new Error('Missing required parameters: type, parentId, and accessToken are required');
    }
    
    const baseUrl = process.env.VITE_BACKEND_BASE_URL || 'http://localhost:4000';
    const fileStats = fs.statSync(filePath);
    
    if (isDev) {
      console.log('[Main Process] File stats:', {
        size: fileStats.size,
        path: filePath
      });
    }
    
    // Create FormData with file stream
    const formData = new NodeFormData();
    const fileStream = fs.createReadStream(filePath);
    
    // Use provided fileName or generate from filePath
    const finalFileName = fileName || path.basename(filePath);
    formData.append('audio', fileStream, finalFileName);
    formData.append('type', type);
    formData.append('prospectId', parentId);
    
    // Append data if provided, stringify if it's an object
    if (data !== undefined && data !== null) {
      const dataString = typeof data === 'string' ? data : JSON.stringify(data);
      formData.append('data', dataString);
    }
    
    // Upload to server
    const url = `${baseUrl}/audio/transcript/upload`;
    if (isDev) {
      console.log('[Main Process] Uploading to:', url);
    }
    
    const response = await axios.post(url, formData, {
      headers: {
        ...formData.getHeaders(),
        'Authorization': `Bearer ${accessToken}`
      },
      maxContentLength: Infinity,
      maxBodyLength: Infinity,
      timeout: 300000, // 5 minute timeout for large files
    });
    
    if (isDev) {
      console.log('[Main Process] File upload successful:', response.data);
    }
    return response.data;
    
  } catch (error: any) {
    console.error('[Main Process] Error uploading file:', error);
    Sentry.captureException(error);

    // Create descriptive error message
    let errorMessage = `Failed to upload file: ${error.message}`;
    if (error.response?.data?.error) {
      errorMessage += ` - Server error: ${error.response.data.error}`;
    }
    if (error.code === 'ECONNREFUSED') {
      errorMessage = 'Could not connect to server. Please check if the server is running.';
    }
    if (error.code === 'ETIMEDOUT') {
      errorMessage = 'Upload timed out. Please try again.';
    }

    const uploadError = new Error(errorMessage) as UploadError;
    uploadError.originalError = error;
    uploadError.filePath = filePath;
    throw uploadError;
  }
});

// Upload both files handler - reads both files from disk and uploads to server together
ipcMain.handle('upload-both-files', async (event: Electron.IpcMainInvokeEvent, { user, prospect, sessionId, accessToken }: { user: { file: string, actualStartMs: number }, prospect: { file: string, actualStartMs: number }, sessionId: string, accessToken: string }) => {
  if (isDev) {
    console.log('[Main Process] Uploading both files:', { 
      userFile: user?.file, 
      prospectFile: prospect?.file, 
      sessionId 
    });
  }
  
  try {
    // Validate inputs
    if (!user?.file || user?.actualStartMs === undefined) {
      throw new Error('User file and actualStartMs are required');
    }

    if (!prospect?.file || prospect?.actualStartMs === undefined) {
      throw new Error('Prospect file and actualStartMs are required');
    }

    if (!sessionId) {
      throw new Error('sessionId is required');
    }

    if (!accessToken) {
      throw new Error('accessToken is required');
    }

    // Check if files exist
    if (!fs.existsSync(user.file)) {
      throw new Error(`User file not found: ${user.file}`);
    }

    if (!fs.existsSync(prospect.file)) {
      throw new Error(`Prospect file not found: ${prospect.file}`);
    }

    const baseUrl = process.env.VITE_BACKEND_BASE_URL || 'http://localhost:4000';
    
    // Get file stats
    const userFileStats = fs.statSync(user.file);
    const prospectFileStats = fs.statSync(prospect.file);
    
    if (isDev) {
      console.log('[Main Process] File stats:', {
        userFile: { size: userFileStats.size, path: user.file },
        prospectFile: { size: prospectFileStats.size, path: prospect.file }
      });
    }
    
    // Create FormData with both file streams
    const formData = new NodeFormData();
    
    // Append user file
    const userFileStream = fs.createReadStream(user.file);
    const userFileName = path.basename(user.file);
    formData.append('userAudio', userFileStream, userFileName);
    
    // Append prospect file
    const prospectFileStream = fs.createReadStream(prospect.file);
    const prospectFileName = path.basename(prospect.file);
    formData.append('prospectAudio', prospectFileStream, prospectFileName);
    
    // Append metadata
    const metadata = {
      sessionId,
      user: {
        actualStartMs: user.actualStartMs
      },
      prospect: {
        actualStartMs: prospect.actualStartMs
      }
    };
    formData.append('data', JSON.stringify(metadata));
    
    // Upload to server
    const url = `${baseUrl}/audio/transcript/upload`;
    if (isDev) {
      console.log('[Main Process] Uploading both files to:', url);
    }
    
    const response = await axios.post(url, formData, {
      headers: {
        ...formData.getHeaders(),
        'Authorization': `Bearer ${accessToken}`
      },
      maxContentLength: Infinity,
      maxBodyLength: Infinity,
      timeout: 600000, // 10 minute timeout for large files and processing
    });
    
    if (isDev) {
      console.log('[Main Process] Both files upload successful:', response.data);
    }
    return response.data;
    
  } catch (error: any) {
    console.error('[Main Process] Error uploading both files:', error);
    Sentry.captureException(error);

    // Create descriptive error message
    let errorMessage = `Failed to upload both files: ${error.message}`;
    if (error.response?.data?.error) {
      errorMessage += ` - Server error: ${error.response.data.error}`;
    }
    if (error.code === 'ECONNREFUSED') {
      errorMessage = 'Could not connect to server. Please check if the server is running.';
    }
    if (error.code === 'ETIMEDOUT') {
      errorMessage = 'Upload timed out. Please try again.';
    }

    const uploadError = new Error(errorMessage) as UploadError;
    uploadError.originalError = error;
    throw uploadError;
  }
});

// --- Audio Queue Event Handlers ---
audioQueue.on('queued', (item: AudioQueueItem) => {
  if (isDev) {
    console.log(`[Audio Queue] Queued new audio chunk: ${item.filePath} (${item.speaker})`);
  }
  if (dashboardWindowInstance) {
    dashboardWindowInstance.webContents.send('audio-queue-update', audioQueue.getStatus());
  }
});

audioQueue.on('processing', (item: AudioQueueItem) => {
  if (isDev) {
    console.log(`[Audio Queue] Processing audio chunk: ${item.filePath} (${item.speaker})`);
  }
  if (dashboardWindowInstance) {
    dashboardWindowInstance.webContents.send('audio-queue-update', audioQueue.getStatus());
  }
});

audioQueue.on('completed', (item: AudioQueueItem) => {
  if (isDev) {
    console.log(`[Audio Queue] Completed processing audio chunk: ${item.filePath} (${item.speaker})`);
  }
  if (dashboardWindowInstance) {
    dashboardWindowInstance.webContents.send('audio-queue-update', audioQueue.getStatus());
  }
});

audioQueue.on('failed', (item: AudioQueueItem) => {
  console.error(`[Audio Queue] Failed to process audio chunk after ${item.retries} retries: ${item.filePath} (${item.speaker})`);
  Sentry.captureMessage(`Audio queue failed: ${item.filePath} (${item.speaker}) after ${item.retries} retries`, 'error');
  if (dashboardWindowInstance) {
    dashboardWindowInstance.webContents.send('audio-queue-update', audioQueue.getStatus());
  }
});

audioQueue.on('retrying', (item: AudioQueueItem) => {
  if (isDev) {
    console.log(`[Audio Queue] Retrying audio chunk (attempt ${item.retries}): ${item.filePath} (${item.speaker})`);
  }
  if (dashboardWindowInstance) {
    dashboardWindowInstance.webContents.send('audio-queue-update', audioQueue.getStatus());
  }
});

audioQueue.on('queueEmpty', (): void => {
  if (isDev) {
    console.log('[Audio Queue] Queue is now empty');
  }
  if (dashboardWindowInstance) {
    dashboardWindowInstance.webContents.send('audio-queue-update', audioQueue.getStatus());
  }
});

// Add IPC handler for getting queue status
ipcMain.handle('get-audio-queue-status', () => {
  return audioQueue.getStatus();
});

// Add IPC handler for reloading the page
ipcMain.handle('reload-page', () => {
  if (isDev) {
    console.log('[MAIN] Reloading page...');
  }
  if (dashboardWindowInstance) {
    dashboardWindowInstance.reload();
  }
  return { status: "Page reloaded" };
});

// Add a simple test handler to verify IPC is working
ipcMain.handle('test-simple', () => {
  return { success: true, message: 'Simple test handler works!' };
});

// Native Audio Module IPC Handlers moved to app.whenReady() after module loads

// Handle protocol activation (when app is opened via sayso:// URL)
app.on('open-url', (event: Event, url: string) => {
  if (isDev) {
    console.log('[Electron] open-url event:', url);
    console.log('Protocol URL received:', url);
  }
  event.preventDefault();
  
  const urlObj = new URL(url);
  const params = new URLSearchParams(urlObj.search);
  
  if (urlObj.hash) {
    const hashParams = new URLSearchParams(urlObj.hash.substring(1));
    const accessToken = hashParams.get('access_token');
    const type = hashParams.get('type');
    
    if (accessToken && type === 'recovery') {
      if (dashboardWindowInstance) {
        const queryString = Array.from(hashParams.entries())
          .map(([key, value]) => `${key}=${encodeURIComponent(value)}`)
          .join('&');
        
        const resetPasswordUrl = isDev 
          ? `http://localhost:5173/#/reset-password?${queryString}`
          : `file://${path.join(__dirname, '../dist/index.html')}#/reset-password?${queryString}`;
        
        dashboardWindowInstance.loadURL(resetPasswordUrl);
        
        if (dashboardWindowInstance.isMinimized()) {
          dashboardWindowInstance.restore();
        }
        dashboardWindowInstance.focus();
      }
      return;
    }
  }

  // Handle checkout callback
  if (urlObj.pathname === '/checkout') {
    const success = params.get('success');
    if (isDev) {
      console.log('Checkout callback received, success:', success);
    }
    
    // Navigate to checkout route with query params
    if (dashboardWindowInstance) {
      const checkoutUrl = isDev 
        ? `http://localhost:5173/#/checkout?success=${success}`
        : `file://${path.join(__dirname, '../dist/index.html')}#/checkout?success=${success}`;
      
      dashboardWindowInstance.loadURL(checkoutUrl);
      
      // Focus the window
      if (dashboardWindowInstance.isMinimized()) {
        dashboardWindowInstance.restore();
      }
      dashboardWindowInstance.focus();
    }
  }
});

// Handle second instance (when app is already running and opened via protocol)
app.on('second-instance', (event: Event, commandLine: string[], workingDirectory: string) => {
  if (isDev) {
    console.log('Second instance detected, command line:', commandLine);
  }
  
  // Focus the existing window
  if (dashboardWindowInstance) {
    if (dashboardWindowInstance.isMinimized()) {
      dashboardWindowInstance.restore();
    }
    dashboardWindowInstance.focus();
  }
  
  // Check if there's a protocol URL in the command line
  const protocolUrl = commandLine.find(arg => arg.startsWith('sayso://'));
  if (protocolUrl) {
    // Trigger the same handling as open-url
    app.emit('open-url', { preventDefault: () => {} }, protocolUrl);
  }
});

// This method will be called when Electron has finished
// initialization and is ready to create browser windows.
// Some APIs can only be used after this event occurs.
app.whenReady().then(() => {
  setupLogging();

  // Run auto-updater check FIRST, before any potential native module crashes
  if (autoUpdater) {
    // Check immediately (with small delay to ensure network is ready)
    setTimeout(() => {
      autoUpdater.checkForUpdates().catch(err => {
        console.error('Failed to check for updates:', err);
        Sentry.captureException(err);
      });
    }, 1000); // 1 second delay
    
    // Check every hour
    setInterval(() => {
      autoUpdater.checkForUpdates().catch(err => {
        console.error('Failed to check for updates:', err);
        Sentry.captureException(err);
      });
    }, 60 * 60 * 1000);
  }

  app.on('browser-window-focus', () => {
    if (process.platform === 'darwin') {
      app.setBadgeCount(0);
    }
  });
  
  // Load native audio module AFTER logging is set up
  try {
    nativeAudio = require('./native-audio');
  } catch (error) {
    Sentry.captureException(error);
  }

  // Register IPC handlers safely (even if module failed to load)
  // This prevents "No handler registered" errors in the renderer
  
  // Initialize native audio module
  ipcMain.handle('native-audio-initialize', async () => {
    if (!nativeAudio) return { success: false, error: 'Native audio module not loaded' };
    try {
      await nativeAudio.initialize();
      return { success: true };
    } catch (error: any) {
      console.error('[MAIN] Failed to initialize native audio:', error);
      Sentry.captureException(error);
      return { success: false, error: error.message };
    }
  });

  // List output devices
  ipcMain.handle('native-audio-list-devices', async () => {
    if (!nativeAudio) return { success: false, error: 'Native audio module not loaded' };
    try {
      const devices = await nativeAudio.listOutputDevices();
      return { success: true, devices };
    } catch (error: any) {
      console.error('[MAIN] Failed to list devices:', error);
      Sentry.captureException(error);
      return { success: false, error: error.message };
    }
  });

  // Create multi-output device
  ipcMain.handle('native-audio-create-device', async (event: Electron.IpcMainInvokeEvent, { name, subDevices }: { name: string, subDevices: string[] }) => {
    if (!nativeAudio) return { success: false, error: 'Native audio module not loaded' };
    try {
      const deviceId = await nativeAudio.createMultiOutputDevice(name, subDevices);
      return { success: true, deviceId };
    } catch (error: any) {
      console.error('[MAIN] Failed to create device:', error);
      Sentry.captureException(error);
      return { success: false, error: error.message };
    }
  });

  // Delete multi-output device
  ipcMain.handle('native-audio-delete-device', async (event: Electron.IpcMainInvokeEvent, { deviceId }: { deviceId: string }) => {
    if (!nativeAudio) return { success: false, error: 'Native audio module not loaded' };
    try {
      const result = await nativeAudio.deleteMultiOutputDevice(deviceId);
      return { success: result };
    } catch (error: any) {
      console.error('[MAIN] Failed to delete device:', error);
      Sentry.captureException(error);
      return { success: false, error: error.message };
    }
  });

  // Request screen recording permission
  ipcMain.handle('native-audio-request-permission', async (): Promise<{ success: boolean, error?: string }> => {
    if (!nativeAudio) return { success: false, error: 'Native audio module not loaded' };
    try {
      const result = await nativeAudio.requestScreenRecordingPermission();
      return { success: result };
    } catch (error: any) {
      console.error('[MAIN] Failed to request permission:', error);
      Sentry.captureException(error);
      return { success: false, error: error.message };
    }
  });

  // Start system audio capture
  ipcMain.handle('native-audio-start-capture', async (event: Electron.IpcMainInvokeEvent, options: AudioCaptureOptions = {}) => {
    if (!nativeAudio) return { success: false, error: 'Native audio module not loaded' };
    try {
      const result = await nativeAudio.startSystemAudioCapture(options);
      return { success: result };
    } catch (error: any) {
      console.error('[MAIN] Failed to start capture:', error);
      Sentry.captureException(error);
      return { success: false, error: error.message };
    }
  });

  // Stop system audio capture
  ipcMain.handle('native-audio-stop-capture', async () => {
    if (!nativeAudio) return { success: false, error: 'Native audio module not loaded', filePath: null };
    try {
      const result = await nativeAudio.stopSystemAudioCapture();
      // Result is now {success, filePath}
      return result;
    } catch (error: any) {
      console.error('[MAIN] Failed to stop capture:', error);
      Sentry.captureException(error);
      return { success: false, error: error.message, filePath: null };
    }
  });

  // Check if system audio capture is active
  ipcMain.handle('native-audio-is-capturing', async () => {
    if (!nativeAudio) return { success: true, isCapturing: false };
    try {
      const result = await nativeAudio.isSystemAudioCaptureActive();
      return { success: true, isCapturing: result };
    } catch (error: any) {
      console.error('[MAIN] Failed to check capture status:', error);
      Sentry.captureException(error);
      return { success: false, error: error.message };
    }
  });
  
  // ONLY create the dashboard window initially
  createDashboardWindow(); 
  registerTrayIconMenu();
  setupGlobalShortcut();
  

  
  app.on('activate', () => {
    // On macOS it's common to re-create a window in the app when the
    // dock icon is clicked and there are no other windows open.
    if (BrowserWindow.getAllWindows().length === 0) {
      // Recreate dashboard if no windows exist
      createDashboardWindow();
    }
     // If dashboard exists but is minimized/hidden, restore and focus.
     else if (dashboardWindowInstance) {
        dashboardWindowInstance.restore(); 
        dashboardWindowInstance.focus(); 
     }
  });
});

// Cleanup audio capture before app quits
app.on('before-quit', async (event: Event) => {
  if (isDev) {
    console.log('App quitting - cleaning up audio capture...');
  }
  
  // Force cleanup of all audio capture before quitting
  await cleanupAllAudioCapture();
});

// Modify window-all-closed to NOT quit if dashboard is meant to be main interface
app.on('window-all-closed', () => {
  // Standard macOS behavior: quit only if platform is not darwin
  if (process.platform !== 'darwin') {
    app.quit();
  }

  unregisterGlobalShortcuts();

  // If you want the app to quit when the dashboard closes even on macOS,
  // you would add app.quit() here.
});

let lastLeaveUrl: string | null = null;
let lastLeaveUrlTime = 0;
let isProcessingPostCall = false;

// Intercept navigation in ALL windows
app.on('web-contents-created', (event: Event, contents: WebContents) => {
  contents.on('will-navigate', (event: Event, url: string) => {
    if (isDev) {
      console.log('[Electron][DEBUG] will-navigate triggered:', {
        url,
        windowId: contents.id,
        stack: new Error().stack
      });
    }
    if (url.includes('post-call')) {
      const now = Date.now();
      // Enhanced duplicate prevention
      if ((url === lastLeaveUrl && now - lastLeaveUrlTime < 3000) || isProcessingPostCall) {
        if (isDev) {
          console.log('[Electron][DEBUG] Skipping duplicate leaveUrl open:', url);
        }
        event.preventDefault();
        return;
      }
      
      isProcessingPostCall = true;
      lastLeaveUrl = url;
      lastLeaveUrlTime = now;
      
      // Reset processing flag after a delay
      setTimeout(() => {
        isProcessingPostCall = false;
      }, 3000);
      
      event.preventDefault();
      shell.openExternal(url);
      // Extract meetingId and prospectId from query parameters
      const urlObj = new URL(url);
      const params = new URLSearchParams(urlObj.search);
      const meetingId = params.get('meetingId');
      const prospectId = params.get('prospectId');
      const sessionId = params.get('sessionId'); // Added sessionId extraction
      if (isDev) {
        console.log('[Electron][DEBUG] will-navigate: Extracted params:', { meetingId, prospectId, sessionId });
      }
            // Send IPC to ALL windows
      BrowserWindow.getAllWindows().forEach((win: BrowserWindowType) => {
        if (isDev) {
          console.log('[Electron][DEBUG] will-navigate: Sending reset-to-home to window:', win.id, { meetingId, prospectId, sessionId });
        }
        win.webContents.send('reset-to-home', { meetingId, prospectId, sessionId });
      });
    }
  });

  contents.setWindowOpenHandler(({ url }: { url: string }) => {
    if (isDev) {
      console.log('[Electron][setWindowOpenHandler] Attempt to open URL:', url);
    }
    if (url.includes('post-call')) {
      const now = Date.now();
      // Enhanced duplicate prevention
      if ((url === lastLeaveUrl && now - lastLeaveUrlTime < 3000) || isProcessingPostCall) {
        if (isDev) {
          console.log('[Electron][DEBUG] Skipping duplicate post-call open in setWindowOpenHandler:', url);
        }
        return { action: 'deny' };
      }
      
      isProcessingPostCall = true;
      lastLeaveUrl = url;
      lastLeaveUrlTime = now;
      
      // Reset processing flag after a delay
      setTimeout(() => {
        isProcessingPostCall = false;
      }, 3000);
      
      shell.openExternal(url);
      // Extract meetingId and prospectId from query parameters
      const urlObj = new URL(url);
      const params = new URLSearchParams(urlObj.search);
      const meetingId = params.get('meetingId');
      const prospectId = params.get('prospectId');
      const sessionId = params.get('sessionId'); // Added sessionId extraction
      if (isDev) {
        console.log('[Electron][setWindowOpenHandler] Extracted params:', { meetingId, prospectId, sessionId });
      }
      BrowserWindow.getAllWindows().forEach((win: BrowserWindowType) => {
        if (isDev) {
          console.log('[Electron][setWindowOpenHandler] Sending reset-to-home to window:', win.id, { meetingId, prospectId, sessionId });
        }
        win.webContents.send('reset-to-home', { meetingId, prospectId, sessionId });
      });
      return { action: 'deny' };
    }
    return { action: 'allow' };
  });

  contents.session.webRequest.onBeforeRequest({ urls: ['*://*/*'] }, (details: Electron.OnBeforeRequestListenerDetails, callback: (response: { cancel?: boolean; redirectURL?: string }) => void) => {
    if (isDev) {
      console.log('[Electron][webRequest.onBeforeRequest] URL:', details.url);
    }
    callback({});
  });
});


// Authenticated User
global.authUser = false;
/**
 * Handler for getting user auth state
 * Sends current authentication status to requesting window
 */
ipcMain.on('get-user-auth', (event: Electron.IpcMainInvokeEvent) => {
  event.sender.send('user-auth', {
    authUser: global.authUser
  });
});

/**
 * Handler for updating user auth state
 */
ipcMain.on('update-user-auth', (event: Electron.IpcMainInvokeEvent, { userAuthenticated }: { userAuthenticated: AuthUser | null }) => {
  global.authUser = userAuthenticated;
  
  if (trayMenuWindow && !trayMenuWindow.isDestroyed()) {
    trayMenuWindow.webContents.send('user-auth', {
      authUser: global.authUser
    });
  }
  
  if (global.mainWindow && !global.mainWindow.isDestroyed()) {
    global.mainWindow.webContents.send('user-auth', {
      authUser: global.authUser
    });
  }
})

// Handler for opening coach window
ipcMain.on('open-coach-window', () => {
  if (isDev) {
    console.log('IPC: Received open-coach-window request');
    console.log('IPC: Current global.coachWindow state:', !!global.coachWindow);
  }
  createCoachWindow();
  // Don't hide tray menu - let user interact with it while it's open
});

// Handler for getting coach window state
ipcMain.on('get-coach-window-state', (event: Electron.IpcMainInvokeEvent) => {
  event.sender.send('coach-window-state', {
    isOpen: isCoachWindowOpen()
  });
});

// Handler for getting coach window state (async version for invoke)
ipcMain.handle('get-coach-window-open-state', () => {
  return isCoachWindowOpen();
});

// Handler for quitting the app
ipcMain.on('quit-app', () => {
  app.quit();
});

// --- Coach Window (Sales Coach Interface) ---
const createCoachWindow = () => {
  if (global.coachWindow && !global.coachWindow.isDestroyed()) {
    if (isDev) {
      console.log('Coach window already exists and is not destroyed, returning...');
    }
    return;
  }
  
  if (global.coachWindow && global.coachWindow.isDestroyed()) {
    global.coachWindow = null;
  }
  
  const preloadScriptPath = path.join(__dirname, 'preload.js');
  
  const windowConfig = WindowManager.getCoachWindowConfig();
  
  const coachWindow = new BrowserWindow({
    ...windowConfig,
    icon: path.join(__dirname, '../public/assets/icon.icns'),
    webPreferences: {
      preload: preloadScriptPath,
      contextIsolation: true,
      nodeIntegration: false,
      webSecurity: true,
      enableBlinkFeatures: 'MediaDevices,MediaStream,WebRTC',
      // permissions: ['media', 'microphone'], 'permissions' does not exist in type 'WebPreferences'.
      allowRunningInsecureContent: false,
      experimentalFeatures: false
    },
  });

  global.coachWindow = coachWindow;

  // dev vs prod URL for the coach window (use the HTML that bootstraps src/coachWindow/index.jsx)
  const coachUrl = isDev
    ? 'http://localhost:5173/coach-window.html'
    : `file://${path.join(__dirname, '../dist/coach-window.html')}`;

  coachWindow.loadURL(coachUrl);
  
  // Ensure main window maintains its properties after coach window creation
  if (dashboardWindowInstance && !dashboardWindowInstance.isDestroyed()) {
    // Restore main window's frame and visibility properties
    dashboardWindowInstance.setVisibleOnAllWorkspaces(false);
    if (isDev) {
      console.log('Main window visibility properties restored');
    }
  }
  
  if (isDev) {
    // coachWindow.webContents.openDevTools();
  }

  coachWindow.on('closed', async () => {
    if (isDev) {
      console.log('Coach window closed event fired, cleaning up reference');
    }
    
    // Force cleanup of all audio capture when coach window closes
    await cleanupAllAudioCapture();
    
    global.coachWindow = null;
    
    // Notify the main window that the coach window has closed
    if (dashboardWindowInstance) {
      dashboardWindowInstance.webContents.send('coach-window-closed');
    }
    
    updateTrayMenu();
  });

  if (isDev) {
    console.log('Coach window created successfully at position:', { x: windowConfig.x, y: windowConfig.y });
  }

  if (dashboardWindowInstance) {
    dashboardWindowInstance.webContents.send('coach-window-opened');
  }

  updateTrayMenu();
};

// Update the resize handler to use WindowManager
ipcMain.on('resize-coach-window', (event: Electron.IpcMainInvokeEvent, width: number, height: number) => {
  if (isDev) {
    console.log(`IPC: Received resize-coach-window request: ${width}x${height}`);
  }
  if (global.coachWindow) {
    WindowManager.resizeCoachWindow(global.coachWindow, width, height);
  }
});

// Add this handler after the other IPC handlers around line 850
ipcMain.on('close-coach-window', () => {
  if (isDev) {
    console.log('IPC: Received close-coach-window request');
  }
  if (global.coachWindow && !global.coachWindow.isDestroyed()) {
    if (isDev) {
      console.log('Closing coach window...');
    }
    global.coachWindow.close();
    // Immediately clean up the reference since we're closing it
    global.coachWindow = null;
  } else {
    if (isDev) {
      console.log('No valid coach window to close');
    }
    global.coachWindow = null; // Clean up stale reference
  }
  // Don't hide tray menu - let user continue interacting with it
});

// Handler for manual window dragging
ipcMain.handle('get-window-position', () => {
  if (global.coachWindow && !global.coachWindow.isDestroyed()) {
    return global.coachWindow.getPosition();
  }
  return [0, 0];
});

ipcMain.on('set-window-position', (event: Electron.IpcMainInvokeEvent, x: number, y: number) => {
  if (global.coachWindow && !global.coachWindow.isDestroyed()) {
    global.coachWindow.setPosition(Math.round(x), Math.round(y));
  }
});

// Handler for demo insights from AdminPanel - forwards to coach window
ipcMain.on('demo-insight', (event: Electron.IpcMainInvokeEvent, insightData: CueInsight) => {
  if (isDev) {
    console.log('[MAIN] Received demo-insight:', insightData);
  }
  
  // Forward to coach window if it exists and is not destroyed
  if (global.coachWindow && !global.coachWindow.isDestroyed()) {
    global.coachWindow.webContents.send('cue-insight', insightData);
    if (isDev) {
      console.log('[MAIN] Demo insight forwarded to coach window');
    }
  } else {
    if (isDev) {
      console.warn('[MAIN] Coach window not available, cannot forward demo insight');
    }
  }
});
