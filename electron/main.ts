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

import { app, BrowserWindow, ipcMain, screen as electronScreen, shell, systemPreferences, globalShortcut, dialog, Tray, Menu, nativeTheme } from 'electron';
import path from 'node:path';
import fs from 'node:fs';
import { nativeImage } from 'electron/common';
import * as Sentry from '@sentry/electron/main';
import sentryConfig from './sentry.config';
import { WindowManager } from './utils/windowManager';
import { clearRefreshToken, loadRefreshToken, saveRefreshToken } from './utils/tokenStore';
import { AuthManager } from './auth/AuthManager';
import type { AuthState } from './auth/AuthManager';

Sentry.init(sentryConfig);

// ─── Auth: single source of truth ────────────────────────────────────────────
// Owns all token state for the app's lifetime. Renderers ask main via IPC.
export const authManager = new AuthManager();

function broadcastToAllWindows(channel: string, data?: unknown): void {
  BrowserWindow.getAllWindows().forEach((win: BrowserWindowType) => {
    if (!win.isDestroyed()) win.webContents.send(channel, data);
  });
}

authManager.on('signed-in', (state: AuthState) => {
  console.log('[AuthManager] signed-in:', state.user?.email);
  global.authAccessToken = state.accessToken;
  broadcastToAllWindows('auth:state', { user: state.user, isAuthenticated: state.isAuthenticated, accessToken: state.accessToken });
});

authManager.on('signed-out', () => {
  console.log('[AuthManager] signed-out');
  global.authAccessToken = null;
  global.authRefreshToken = null;
  broadcastToAllWindows('auth:state', { user: null, isAuthenticated: false, accessToken: null });
  broadcastToAllWindows('auth-session-expired');   // backward-compat for unmigrated windows
  broadcastToAllWindows('auth:session-expired');
});

authManager.on('token-refreshed', (state: AuthState) => {
  console.log('[AuthManager] token-refreshed');
  global.authAccessToken = state.accessToken;
  broadcastToAllWindows('auth:state', { user: state.user, isAuthenticated: state.isAuthenticated, accessToken: state.accessToken });
  broadcastToAllWindows('auth:token-refreshed');
  // Also broadcast old event so any remaining unmigrated axios listeners stay warm
  broadcastToAllWindows('auth-tokens-refreshed', { accessToken: state.accessToken, refreshToken: '' });
  // Keep the active audio WebSocket's token current so reconnects after a refresh
  // don't fail with an expired JWT. updateToken() stores the value and the next
  // _connect() call will embed it in the WebSocket URL query string.
  if (state.accessToken) {
    if (cueAudioStreamer) cueAudioStreamer.updateToken(state.accessToken);
    if (audioStreamer)    audioStreamer.updateToken(state.accessToken);
  }
});

authManager.on('session-expired', () => {
  console.log('[AuthManager] session-expired');
  global.authAccessToken = null;
  global.authRefreshToken = null;
  broadcastToAllWindows('auth:state', { user: null, isAuthenticated: false, accessToken: null });
  broadcastToAllWindows('auth-session-expired');   // backward-compat
  broadcastToAllWindows('auth:session-expired');
  // Stop WebSocket reconnect loops — there is no valid token to reconnect with
  if (cueAudioStreamer) { cueAudioStreamer.shouldReconnect = false; cueAudioStreamer.stop(false).catch(() => {}); }
  if (audioStreamer)    { audioStreamer.shouldReconnect    = false; audioStreamer.stop(false).catch(() => {}); }
});

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
    if (splashWindowInstance && !splashWindowInstance.isDestroyed()) {
      splashWindowInstance.webContents.send('update-available', { version: info.version });
    }
  });

  updater.on('update-not-available', (info: { version: string }) => {
    log.info('Update not available. Current version:', info.version);
    if (splashWindowInstance && !splashWindowInstance.isDestroyed()) {
      splashWindowInstance.webContents.send('update-check-complete');
    }
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
    if (splashWindowInstance && !splashWindowInstance.isDestroyed()) {
      splashWindowInstance.webContents.send('download-progress', {
        percent: progressObj.percent ?? 0,
        bytesPerSecond: speed,
        transferred,
        total,
      });
    }
  });

  updater.on('update-downloaded', (info: { version: string }) => {
    log.info('Update downloaded:', info.version);
    if (splashWindowInstance && !splashWindowInstance.isDestroyed()) {
      splashWindowInstance.webContents.send('update-downloaded', { version: info.version });
    }
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

// ===== FONT SIZE CACHE =====
let cachedFontSize: string = 's';

// ===== HELPER FUNCTIONS =====
function isCoachWindowOpen() {
  return global.coachWindow && !global.coachWindow.isDestroyed();
}
function isAppSettingsWindowOpen() {
  return global.appSettingsWindow && !global.appSettingsWindow.isDestroyed();
}
function isPlaybookWindowOpen() {
  return global.playbookWindow && !global.playbookWindow.isDestroyed();
}
function broadcastAppSettingsWindowState(isOpen: boolean) {
  const payload = { isOpen };
  if (global.coachWindow && !global.coachWindow.isDestroyed()) {
    global.coachWindow.webContents.send('app-settings-window-state', payload);
  }
  if (trayMenuWindow && !trayMenuWindow.isDestroyed()) {
    trayMenuWindow.webContents.send('app-settings-window-state', payload);
  }
}
function broadcastPlaybookWindowState(isOpen: boolean) {
  const payload = { isOpen };
  if (global.coachWindow && !global.coachWindow.isDestroyed()) {
    global.coachWindow.webContents.send('playbook-window-state', payload);
  }
}

// ===== CUSTOM TRAY MENU WINDOW =====
let tray: TrayType | null = null;
let trayMenuWindow: BrowserWindowType | null = null;
let onboardingWindowInstance: BrowserWindowType | null = null;
let onboardingClosedIntentionally = false;

function sendToOnboardingWindow(channel: string) {
  if (onboardingWindowInstance && !onboardingWindowInstance.isDestroyed()) {
    onboardingWindowInstance.webContents.send(channel);
  }
}

/**
 * Creates and positions the custom tray menu window near the tray icon
 */
const TRAY_MENU_WIDTH = 230; 
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
    width: TRAY_MENU_WIDTH,
    height: 172,
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
    backgroundColor: allowVibrancy ? '#00000000' : (nativeTheme.shouldUseDarkColors ? '#1f2937' : '#F9FAFB'),
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

    trayMenuWindow.on('blur', () => {
        hideTrayMenu();
    });
    
    if (!allowVibrancy) {
      nativeTheme.on('updated', () => {
        if (trayMenuWindow && !trayMenuWindow.isDestroyed()) {
          trayMenuWindow.setBackgroundColor(nativeTheme.shouldUseDarkColors ? '#1f2937' : '#F9FAFB');
        }
      });
    }
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
        trayMenuWindow.setVisibleOnAllWorkspaces(true, { visibleOnFullScreen: true });
        positionTrayMenu();
        trayMenuWindow.show();
        trayMenuWindow.setVisibleOnAllWorkspaces(false, { visibleOnFullScreen: true });
      }
    }, 100);
  } else {
    trayMenuWindow.setVisibleOnAllWorkspaces(true, { visibleOnFullScreen: true });
    positionTrayMenu();
    trayMenuWindow.show();
    trayMenuWindow.setVisibleOnAllWorkspaces(false, { visibleOnFullScreen: true });
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
      sendToOnboardingWindow('onboarding:tray-clicked');
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
        sendToOnboardingWindow('onboarding:coach-opened');
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

/** Serialize stop-cue: overlapping IPC invokes await the same teardown (no double native stop). */
let cueStopInFlight: Promise<{ success: boolean; error?: string; deduped?: boolean }> | null = null;

/** Per-session telemetry: chunks + bytes forwarded from native callbacks into AudioStreamer */
let cueCaptureStats = {
  userChunks: 0,
  prospectChunks: 0,
  userBytes: 0,
  prospectBytes: 0
};

let cueLowAudioTimer: ReturnType<typeof setTimeout> | null = null;

function clearCueLowAudioTimer() {
  if (cueLowAudioTimer) {
    clearTimeout(cueLowAudioTimer);
    cueLowAudioTimer = null;
  }
}

function scheduleCueLowAudioCheck(sessionId: string) {
  clearCueLowAudioTimer();
  cueLowAudioTimer = setTimeout(() => {
    cueLowAudioTimer = null;
    if (!cueAudioStreamer || cueAudioStreamer.sessionId !== sessionId) return;
    if (cueCaptureStats.userChunks > 0) return;
    try {
      if (global.coachWindow && !global.coachWindow.isDestroyed()) {
        global.coachWindow.webContents.send('cue-low-user-audio', { sessionId });
      }
    } catch {
      /* ignore */
    }
  }, 4000);
}

/**
 * Stop Cue mic + SCK + websocket streamer (shared by stop-cue and defensive start-cue).
 * Does not touch cueStopInFlight mutex.
 */
async function teardownCueStreamsAndNative(): Promise<void> {
  clearCueLowAudioTimer();
  await stopUserStreaming();
  if (nativeAudio) {
    await nativeAudio.stopSystemAudioCapture();
    nativeAudio.setStreamingCallback(null);
  }
  if (cueAudioStreamer) {
    await cueAudioStreamer.stop(false);
    cueAudioStreamer = null;
  }
}

/**
 * Cleanup all audio capture resources (screen capture, microphone, streams)
 * Called when coach window closes or app quits to ensure permissions are released
 */
async function cleanupAllAudioCapture() {
  if (isDev) {
    console.log('[Cleanup] Starting audio capture cleanup...');
  }
  
  try {
    if (cueStopInFlight) {
      await cueStopInFlight;
    }

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

    if (cueStopInFlight) {
      await cueStopInFlight;
    }

    let systemCaptureActive = false;
    let micCaptureActive = false;
    try {
      if (typeof nativeAudio.isSystemAudioCaptureActive === 'function') {
        systemCaptureActive = !!(await nativeAudio.isSystemAudioCaptureActive());
      }
      if (typeof nativeAudio.isMicrophoneCaptureActive === 'function') {
        micCaptureActive = !!(await nativeAudio.isMicrophoneCaptureActive());
      }
    } catch (probeErr) {
      console.warn('[Cue] Could not probe native capture state:', probeErr);
    }

    if (cueAudioStreamer || systemCaptureActive || micCaptureActive) {
      console.warn('[Cue] Guard: leftover streamer or native capture — running teardown before start', {
        hadStreamer: !!cueAudioStreamer,
        systemCaptureActive,
        micCaptureActive
      });
      await teardownCueStreamsAndNative();
    }

    cueCaptureStats = { userChunks: 0, prospectChunks: 0, userBytes: 0, prospectBytes: 0 };

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

          if (message && message.type === 'smart_capture' && message.data) {
            if (global.coachWindow && !global.coachWindow.isDestroyed()) {
              global.coachWindow.webContents.send('cue-smart-capture', message.data);
              if (isDev) {
                console.log('[MAIN] Smart capture forwarded to coach window:', message.data);
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
    const cueUserStreamingCallback = (buffer: Buffer, format: unknown) => {
      if (cueAudioStreamer) {
        cueCaptureStats.userChunks += 1;
        cueCaptureStats.userBytes += buffer?.length ?? 0;
        cueAudioStreamer.addUserAudio(buffer, format);
      }
    };
    await startUserStreaming({ streamingCallback: cueUserStreamingCallback });
    await ensureCueUserMicDeliversJsChunks(sessionId, cueUserStreamingCallback);

    if (!nativeAudio) {
      throw new Error('Native audio module not loaded. Please wait for app initialization.');
    }

    if (typeof nativeAudio.startProspectStreaming !== 'function') {
      throw new Error('startProspectStreaming method not available. Native module may need to be rebuilt.');
    }

    await nativeAudio.startProspectStreaming({
      streamingCallback: (buffer: Buffer, format: string) => {
        if (cueAudioStreamer) {
          cueCaptureStats.prospectChunks += 1;
          cueCaptureStats.prospectBytes += buffer?.length ?? 0;
          cueAudioStreamer.addProspectAudio(buffer, format);
        }
      }
    });

    scheduleCueLowAudioCheck(sessionId);

    if (isDev) {
      console.log('[Cue] Started session', sessionId, '— stats reset; low-audio check in 4s if no user chunks');
    }

    sendToOnboardingWindow('onboarding:session-started');
    return {
      success: true,
      sessionId: sessionId
    };
  } catch (error: any) {
    console.error('[MAIN] Error starting Cue:', error);
    Sentry.captureException(error);
    clearCueLowAudioTimer();
    try {
      await teardownCueStreamsAndNative();
    } catch (teardownErr: any) {
      console.error('[Cue] Error tearing down after failed start:', teardownErr);
      cueAudioStreamer = null;
    }
    return { success: false, error: error.message };
  }
});

// Stop Cue (closes all websockets)
ipcMain.handle('stop-cue', async (_event: Electron.IpcMainInvokeEvent) => {
  if (cueStopInFlight) {
    const result = await cueStopInFlight;
    return { ...result, deduped: true };
  }

  const stopWork = (async (): Promise<{ success: boolean; error?: string }> => {
    const statsAtStop = { ...cueCaptureStats };
    try {
      await teardownCueStreamsAndNative();
      sendToOnboardingWindow('onboarding:session-stopped');
      console.log(
        `[Cue] Session teardown complete — capture stats: userChunks=${statsAtStop.userChunks} prospectChunks=${statsAtStop.prospectChunks} userBytes=${statsAtStop.userBytes} prospectBytes=${statsAtStop.prospectBytes}`
      );
      cueCaptureStats = { userChunks: 0, prospectChunks: 0, userBytes: 0, prospectBytes: 0 };
      return { success: true };
    } catch (error: any) {
      console.error('[MAIN] Error stopping Cue:', error);
      Sentry.captureException(error);
      cueAudioStreamer = null;
      cueCaptureStats = { userChunks: 0, prospectChunks: 0, userBytes: 0, prospectBytes: 0 };
      return { success: false, error: error.message };
    } finally {
      cueStopInFlight = null;
    }
  })();

  cueStopInFlight = stopWork;
  return stopWork;
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

const CUE_MIC_JS_WARMUP_MS = 500;
const CUE_MIC_JS_RESTART_WAIT_MS = 700;

async function waitForCueUserChunks(sessionId: string, maxMs: number): Promise<boolean> {
  const deadline = Date.now() + maxMs;
  while (Date.now() < deadline) {
    if (cueCaptureStats.userChunks > 0) return true;
    if (!cueAudioStreamer || cueAudioStreamer.sessionId !== sessionId) return false;
    await new Promise<void>((resolve) => setImmediate(resolve));
  }
  return cueCaptureStats.userChunks > 0;
}

/** If Node never receives mic buffers after native start, stop/start mic once (belt-and-suspenders). */
async function ensureCueUserMicDeliversJsChunks(
  sessionId: string,
  streamingCallback: (buffer: Buffer, format: unknown) => void
): Promise<void> {
  if (await waitForCueUserChunks(sessionId, CUE_MIC_JS_WARMUP_MS)) return;
  if (!cueAudioStreamer || cueAudioStreamer.sessionId !== sessionId) return;
  console.warn('[Cue] No user chunks on JS side after native mic start — restarting user streaming once', {
    sessionId,
  });
  await stopUserStreaming();
  if (!cueAudioStreamer || cueAudioStreamer.sessionId !== sessionId) return;
  await startUserStreaming({ streamingCallback });
  await waitForCueUserChunks(sessionId, CUE_MIC_JS_RESTART_WAIT_MS);
}

const audioQueue = require('./audioQueue');
const { AudioStreamer } = require('./streaming/audioStreamer');
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
let splashWindowInstance: BrowserWindowType | null = null;

// --- Splash Window (Auth / Loading Screen) ---
const createSplashWindow = (opts: { logout?: boolean; reason?: 'session-expired' } | boolean = {}) => {
  // Support legacy boolean call sites (createSplashWindow(true))
  const { logout = false, reason } = typeof opts === 'boolean' ? { logout: opts, reason: undefined } : opts;

  if (splashWindowInstance && !splashWindowInstance.isDestroyed()) {
    if (reason) splashWindowInstance.webContents.send('splash:show-reason', reason);
    splashWindowInstance.focus();
    return;
  }

  const preloadScriptPath = path.join(__dirname, 'preload.js');

const splashWindow = new BrowserWindow({
    show: false,
    width: 380,
    height: 560,
    center: true,
    resizable: false,
    maximizable: false,
    fullscreenable: false,
    roundedCorners: true,
    titleBarStyle: 'hiddenInset',
    webPreferences: {
        preload: preloadScriptPath,
        contextIsolation: true,
        nodeIntegration: false,
        webSecurity: true,
    },
});

  splashWindowInstance = splashWindow;

  const params = new URLSearchParams();
  if (logout) params.set('logout', 'true');
  if (reason) params.set('reason', reason);
  const query = params.toString() ? `?${params.toString()}` : '';
  const splashUrl = isDev
    ? `http://localhost:5173/splash-window.html${query}`
    : `file://${path.join(__dirname, '../dist/splash-window.html')}${query}`;

  splashWindow.once('ready-to-show', () => {
    splashWindow.show();
  });

  splashWindow.loadURL(splashUrl);

  if (isDev) {
    splashWindow.webContents.openDevTools();
  }

  splashWindow.on('closed', () => {
    splashWindowInstance = null;
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
});

audioQueue.on('processing', (item: AudioQueueItem) => {
  if (isDev) {
    console.log(`[Audio Queue] Processing audio chunk: ${item.filePath} (${item.speaker})`);
  }
});

audioQueue.on('completed', (item: AudioQueueItem) => {
  if (isDev) {
    console.log(`[Audio Queue] Completed processing audio chunk: ${item.filePath} (${item.speaker})`);
  }
});

audioQueue.on('failed', (item: AudioQueueItem) => {
  console.error(`[Audio Queue] Failed to process audio chunk after ${item.retries} retries: ${item.filePath} (${item.speaker})`);
  Sentry.captureMessage(`Audio queue failed: ${item.filePath} (${item.speaker}) after ${item.retries} retries`, 'error');
});

audioQueue.on('retrying', (item: AudioQueueItem) => {
  if (isDev) {
    console.log(`[Audio Queue] Retrying audio chunk (attempt ${item.retries}): ${item.filePath} (${item.speaker})`);
  }
});

audioQueue.on('queueEmpty', (): void => {
  if (isDev) {
    console.log('[Audio Queue] Queue is now empty');
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

  if (urlObj.hostname === 'launch-coach') {
    if (!isCoachWindowOpen()) {
      createCoachWindow();
    } else {
      global.coachWindow?.focus();
    }
  }
});

// Handle second instance (when app is already running and opened via protocol)
app.on('second-instance', (event: Event, commandLine: string[], workingDirectory: string) => {
  if (isDev) {
    console.log('Second instance detected, command line:', commandLine);
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
app.whenReady().then(async () => {
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
      return await nativeAudio.startSystemAudioCapture(options);
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
  
  // Always attempt silent auth via AuthManager first.
  // init() reads the persisted refresh token, exchanges it for a fresh access
  // token, and schedules the proactive refresh timer. If it fails or there is
  // no stored token it returns cleanly and we fall through to the splash.
  await authManager.init();

  const authState = authManager.getState();
  if (authState.isAuthenticated) {
    // Fetch the full account profile so the tray menu reflects logged-in state.
    try {
      const baseUrl = process.env.VITE_BACKEND_BASE_URL || 'http://localhost:4000';
      const profileRes = await axios.get(`${baseUrl}/accounts/${authState.user?.email}`, {
        headers: { Authorization: `Bearer ${authState.accessToken}` },
        timeout: 5000,
      });
      global.authUser = profileRes.data.data;
    } catch (profileErr) {
      console.warn('[MAIN] Silent auth succeeded but profile fetch failed — tray will show logged-out state', profileErr);
      Sentry.captureException(profileErr);
    }
  } else {
    createSplashWindow();
  }
  registerTrayIconMenu();
  setupGlobalShortcut();

  app.on('activate', () => {
    // On macOS it's common to re-create a window in the app when the
    // dock icon is clicked and there are no other windows open.
    if (BrowserWindow.getAllWindows().length === 0) {
      createSplashWindow();
      setupGlobalShortcut();
    } else if (splashWindowInstance && !splashWindowInstance.isDestroyed()) {
      splashWindowInstance.restore();
      splashWindowInstance.focus();
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
  unregisterGlobalShortcuts();
});

// Modify window-all-closed to NOT quit if dashboard is meant to be main interface
app.on('window-all-closed', () => {
  // Standard macOS behavior: quit only if platform is not darwin
  if (process.platform !== 'darwin') {
    app.quit();
  }

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

ipcMain.handle('get-launch-at-login', () => {
  return app.getLoginItemSettings().openAtLogin;
});

ipcMain.handle('set-launch-at-login', (_event: Electron.IpcMainInvokeEvent, enabled: boolean) => {
  app.setLoginItemSettings({ openAtLogin: enabled, openAsHidden: true });
});

ipcMain.handle('auth:sign-in', async (_event, { email, password }: { email: string; password: string }) => {
  return authManager.signIn(email, password);
});

ipcMain.handle('auth:verify-mfa', async (_event, { factorId, code }: { factorId: string; code: string }) => {
  return authManager.verifyMFA(factorId, code);
});

ipcMain.handle('auth:sign-out', async () => {
  await authManager.signOut();
});

/**
 * Returns a valid access token, proactively refreshing if within 60 s of expiry.
 * All windows call this instead of caching a token themselves.
 */
ipcMain.handle('auth:get-token', async () => {
  return authManager.getAccessToken();
});

ipcMain.handle('auth:get-state', () => {
  return authManager.getState();
});

// ─────────────────────────────────────────────────────────────────────────────

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
// Handle for opening Coach settings window
ipcMain.on('open-app-settings-window', () => {
    createAppSettingsWindow();
})
ipcMain.on('close-app-settings-window', () => {
    if (global.appSettingsWindow && !global.appSettingsWindow.isDestroyed()) {
        global.appSettingsWindow.close();
        global.appSettingsWindow = null;
    } else {
        global.appSettingsWindow = null;
    }
})

ipcMain.on('app-settings:session-expired-redirect', () => {
    createSplashWindow({ reason: 'session-expired' });
    if (global.appSettingsWindow && !global.appSettingsWindow.isDestroyed()) {
        global.appSettingsWindow.close();
        global.appSettingsWindow = null;
    } else {
        global.appSettingsWindow = null;
    }
})
ipcMain.on('get-app-settings-window-state', (event: Electron.IpcMainInvokeEvent) => {
    event.sender.send('app-settings-window-state', {
        isOpen: isAppSettingsWindowOpen()
    })
})
ipcMain.handle('get-app-settings-window-open-state', () => {
    return isAppSettingsWindowOpen();
})

// Handler for opening coach window — checks mic permission first; if missing, opens splash for permissions flow
ipcMain.on('open-coach-window', () => {
  if (isDev) {
    console.log('IPC: Received open-coach-window request');
    console.log('IPC: Current global.coachWindow state:', !!global.coachWindow);
  }
  const micStatus = systemPreferences.getMediaAccessStatus('microphone');
  if (micStatus !== 'granted') {
    createSplashWindow();
    return;
  }
  createCoachWindow();
  sendToOnboardingWindow('onboarding:coach-opened');
});
ipcMain.on('close-coach-window', () => {
  if (isDev) {
    console.log('IPC: Received close-coach-window request');
  }
  if (global.coachWindow && !global.coachWindow.isDestroyed()) {
    if (isDev) {
      console.log('Closing coach window...');
    }
    global.coachWindow.close();
    global.coachWindow = null;

  } else {
    if (isDev) {
      console.log('No valid coach window to close');
    }
    global.coachWindow = null;
  }
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

// Handler for the renderer to trigger update installation (after user clicks "Restart Now")
ipcMain.on('install-update', () => {
  if (autoUpdater) {
    setImmediate(() => {
      app.removeAllListeners('window-all-closed');
      autoUpdater!.quitAndInstall(false, true);
    });
  }
});

ipcMain.on('set-font-size', (_event, size: string) => {
  cachedFontSize = size;
  if (isCoachWindowOpen()) {
    global.coachWindow!.webContents.send('font-size-changed', size);
  }
  if (isPlaybookWindowOpen()) {
    global.playbookWindow!.webContents.send('font-size-changed', size);
  }
});

ipcMain.on('open-onboarding-window', () => createOnboardingWindow());

ipcMain.on('close-onboarding-window', (_event) => {
  onboardingClosedIntentionally = true;
  const win = BrowserWindow.fromWebContents(_event.sender);
  if (win && !win.isDestroyed()) win.close();
});

ipcMain.on('complete-onboarding', (_event) => {
  onboardingClosedIntentionally = true;
  const win = BrowserWindow.fromWebContents(_event.sender);
  if (win && !win.isDestroyed()) win.close();
});

// Handler for the splash window to signal successful login — closes the splash window
ipcMain.on('splash-login-success', () => {
  if (splashWindowInstance && !splashWindowInstance.isDestroyed()) {
    splashWindowInstance.close();
  }
});

// Handler for showing the splash window from the tray menu (e.g. Log In)
ipcMain.on('tray-show-window', () => {
  if (!splashWindowInstance || splashWindowInstance.isDestroyed()) {
    createSplashWindow();
  } else {
    if (splashWindowInstance.isMinimized()) splashWindowInstance.restore();
    splashWindowInstance.show();
    splashWindowInstance.focus();
  }
});

// Handler for triggering logout from the tray menu — opens splash window with sign-out flag
ipcMain.on('tray-logout', () => {
  hideTrayMenu();
  clearRefreshToken();
  if (!splashWindowInstance || splashWindowInstance.isDestroyed()) {
    createSplashWindow({ logout: true });
  } else {
    const logoutUrl = isDev
      ? 'http://localhost:5173/splash-window.html?logout=true'
      : `file://${path.join(__dirname, '../dist/splash-window.html')}?logout=true`;
    if (splashWindowInstance.isMinimized()) splashWindowInstance.restore();
    splashWindowInstance.show();
    splashWindowInstance.focus();
    splashWindowInstance.loadURL(logoutUrl);
  }
});

// Handler for resizing the tray menu window (e.g. when items are shown/hidden)
ipcMain.on('set-tray-menu-height', (_event: Electron.IpcMainEvent, height: number) => {
  if (trayMenuWindow && !trayMenuWindow.isDestroyed()) {
    trayMenuWindow.setSize(TRAY_MENU_WIDTH, height, false);
    positionTrayMenu();
  }
});

// Handler for quitting the app
ipcMain.on('quit-app', () => {
  app.quit();
});

// --- Coach Settings Window ---
const createOnboardingWindow = () => {
  const preloadScriptPath = path.join(__dirname, 'preload.js');
  const onboardingWindow = new BrowserWindow({
    width: 720,
    height: 560,
    titleBarStyle: 'hiddenInset',
    resizable: false,
    maximizable: false,
    minimizable: false,
    fullscreenable: false,
	backgroundColor: '#2a3f5f',
	roundedCorners: true,
    webPreferences: {
      preload: preloadScriptPath,
      contextIsolation: true,
      nodeIntegration: false,
      webSecurity: true,
    },
  });

  const onboardingUrl = isDev
    ? 'http://localhost:5173/onboarding-window.html'
    : `file://${path.join(__dirname, '../dist/onboarding-window.html')}`;

  onboardingWindow.loadURL(onboardingUrl);

  onboardingWindowInstance = onboardingWindow;

  onboardingWindow.on('close', async (e) => {
    if (!onboardingClosedIntentionally) {
      e.preventDefault();
      const ts = Date.now() + 24 * 60 * 60 * 1000;
      try {
        await onboardingWindow.webContents.executeJavaScript(
          `localStorage.setItem('onboarding_remind_after', '${ts}')`
        );
      } catch (_) {}
      onboardingClosedIntentionally = true;
      onboardingWindow.close();
      return;
    }
    onboardingClosedIntentionally = false;
  });

  onboardingWindow.on('closed', () => { onboardingWindowInstance = null; });

  if (isDev) {
    onboardingWindow.webContents.openDevTools({ mode: 'detach' });
  }
};

const createAppSettingsWindow = () => {
    if (global.appSettingsWindow && !global.appSettingsWindow.isDestroyed()) {
        if (isDev) {
            console.log('Coach window already exists and is not destroyed, returning...');
        }
        return;
    }
    
    if (global.appSettingsWindow && global.appSettingsWindow.isDestroyed()) {
        global.appSettingsWindow = null;
    }
    
    const preloadScriptPath = path.join(__dirname, 'preload.js');
    const windowConfig = WindowManager.getAppSettingsWindowConfig();
    const appSettingsWindow = new BrowserWindow({
        ...windowConfig,
        icon: path.join(__dirname, '../public/assets/icon.icns'),
        titleBarStyle: 'hiddenInset',
        titleBarOverlay: {
          color: '#02192f',
          symbolColor: '#FFF',
          height: 30,
        },
        webPreferences: {
            preload: preloadScriptPath,
            contextIsolation: true,
            nodeIntegration: false,
            webSecurity: true,
            enableBlinkFeatures: 'MediaDevices,MediaStream,WebRTC',
            allowRunningInsecureContent: false,
            experimentalFeatures: false
        },
    });
    
    global.appSettingsWindow = appSettingsWindow;
    
    // dev vs prod URL for the coach window (use the HTML that bootstraps src/coachWindow/index.jsx)
    const appSettingsUrl = isDev
      ? 'http://localhost:5173/app-settings-window.html'
      : `file://${path.join(__dirname, '../dist/app-settings-window.html')}`;
  
    appSettingsWindow.loadURL(appSettingsUrl);
    broadcastAppSettingsWindowState(true);

    appSettingsWindow.on('closed', () => {
        global.appSettingsWindow = null;
        broadcastAppSettingsWindowState(false);
    })
}

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
    ? `http://localhost:5173/coach-window.html?fontSize=${cachedFontSize}`
    : `file://${path.join(__dirname, '../dist/coach-window.html')}?fontSize=${cachedFontSize}`;

  coachWindow.loadURL(coachUrl);
  
  if (isDev) {
    // coachWindow.webContents.openDevTools();
  }

  coachWindow.on('closed', async () => {
    if (isDev) {
      console.log('Coach window closed event fired, cleaning up reference');
    }

    if (isPlaybookWindowOpen()) {
      global.playbookWindow!.close();
    }

    global.playbooksCache = null;

    // Force cleanup of all audio capture when coach window closes
    await cleanupAllAudioCapture();

    global.coachWindow = null;

    updateTrayMenu();
  });

  if (isDev) {
    console.log('Coach window created successfully at position:', { x: windowConfig.x, y: windowConfig.y });
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

// --- Playbook Window (anchored to the coach window, only while coach is open) ---
const createPlaybookWindow = () => {
  if (!isCoachWindowOpen()) {
    if (isDev) {
      console.log('Playbook window: refused to open, coach window is not open');
    }
    return;
  }

  if (global.playbookWindow && !global.playbookWindow.isDestroyed()) {
    return;
  }

  if (global.playbookWindow && global.playbookWindow.isDestroyed()) {
    global.playbookWindow = null;
  }

  const preloadScriptPath = path.join(__dirname, 'preload.js');
  const coachBounds = global.coachWindow!.getBounds();
  const windowConfig = WindowManager.getPlaybookWindowConfig(coachBounds);

  const playbookWindow = new BrowserWindow({
    ...windowConfig,
    icon: path.join(__dirname, '../public/assets/icon.icns'),
    webPreferences: {
      preload: preloadScriptPath,
      contextIsolation: true,
      nodeIntegration: false,
      webSecurity: true,
      allowRunningInsecureContent: false,
      experimentalFeatures: false
    }
  });

  global.playbookWindow = playbookWindow;

  const playbookUrl = isDev
    ? `http://localhost:5173/playbook-window.html?fontSize=${cachedFontSize}`
    : `file://${path.join(__dirname, '../dist/playbook-window.html')}?fontSize=${cachedFontSize}`;

  playbookWindow.loadURL(playbookUrl);
  broadcastPlaybookWindowState(true);

  playbookWindow.on('closed', () => {
    global.playbookWindow = null;
    broadcastPlaybookWindowState(false);
  });
};

ipcMain.on('open-playbook-window', () => {
  createPlaybookWindow();
});

ipcMain.on('close-playbook-window', () => {
  if (isPlaybookWindowOpen()) {
    global.playbookWindow!.close();
  }
});

ipcMain.on('get-playbook-window-state', (event: Electron.IpcMainInvokeEvent) => {
  event.sender.send('playbook-window-state', { isOpen: isPlaybookWindowOpen() });
});

ipcMain.handle('get-playbook-window-position', () => {
  if (isPlaybookWindowOpen()) {
    return global.playbookWindow!.getPosition();
  }
  return [0, 0];
});

ipcMain.on('set-playbook-window-position', (_event: Electron.IpcMainInvokeEvent, x: number, y: number) => {
  if (isPlaybookWindowOpen()) {
    global.playbookWindow!.setPosition(Math.round(x), Math.round(y));
  }
});

// --- Playbooks data cache (prewarmed by coach window, consumed by playbook window) ---
ipcMain.on('set-playbooks-cache', (_event, payload: { playbooks: unknown[] | null; error: string | null }) => {
  global.playbooksCache = payload;
  if (isPlaybookWindowOpen()) {
    global.playbookWindow!.webContents.send('playbooks-updated', payload);
  }
});

ipcMain.handle('get-playbooks-cache', () => {
  return global.playbooksCache ?? { playbooks: null, error: null };
});
