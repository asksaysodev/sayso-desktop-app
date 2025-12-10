const { app, BrowserWindow, ipcMain, screen, shell, systemPreferences, globalShortcut, dialog, Tray, Menu } = require('electron');
const path = require('node:path');
const fs = require('node:fs');
const { WindowManager } = require('./utils/windowManager');
const { nativeImage } = require('electron/common');

// Native audio module - will be loaded after logging is set up
let nativeAudio = null;

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
    console.error('🔍 [DEBUG] Error creating debug file:', error);
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
    
    function writeToFile(level, ...args) {
      const timestamp = new Date().toISOString();
      const message = args.map(arg => 
        typeof arg === 'object' ? JSON.stringify(arg, null, 2) : String(arg)
      ).join(' ');
      
      const logEntry = `[${timestamp}] [${level}] ${message}\n`;
      
      try {
        fs.appendFileSync(logFile, logEntry);
      } catch (err) {
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
    
    console.log(`[Main Process] Logging to file: ${logFile}`);
  }
}

// ===== HELPER FUNCTIONS =====
function isCoachWindowOpen() {
  return global.coachWindow && !global.coachWindow.isDestroyed();
}

// ===== CUSTOM TRAY MENU WINDOW ===== 
let tray = null;
let trayMenuWindow = null;

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
    vibrancy: 'menu',
    visualEffectState: 'active',
    backgroundColor: '#00000000',
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

  trayMenuWindow.loadURL(trayMenuUrl);

  // Handle blur (click outside) to close the menu
  // trayMenuWindow.on('blur', () => {
  //   if (trayMenuWindow && !trayMenuWindow.isDestroyed()) {
  //     // hideTrayMenu();
  //   }
  // });

  trayMenuWindow.on('closed', () => {
    trayMenuWindow = null;
  });
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
  const primaryDisplay = screen.getPrimaryDisplay();
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
    console.error('❌ Tray icon failed to load! Icon is empty.');
    return;
  }
  
  icon = icon.resize({ width: 19, height: 19 });
  icon.setTemplateImage(true);
  
  tray = new Tray(icon);
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
      if (isCoachWindowOpen()) {
        global.coachWindow.close();
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
let activeRecordingSession = null;

// Store AudioStreamer instance
let audioStreamer = null;

// Store Cue instances (separate from regular streaming)
let cueAudioStreamer = null;

// Start audio streaming
ipcMain.handle('start-audio-streaming', async (event, { token }) => {
  try {
    // Create AudioStreamer instance
    audioStreamer = new AudioStreamer({
      onUserConnected: () => {
        event.sender.send('streaming-status', { user: 'connected' });
      },
      onProspectConnected: () => {
        event.sender.send('streaming-status', { prospect: 'connected' });
      },
      onError: (stream, error) => {
        event.sender.send('streaming-error', { stream, error: error.message });
      }
    });

    // Start streaming (only needs token - gets sessionId internally)
    await audioStreamer.start(token);

    // Set up streaming callbacks
    await startUserFullRecording({
      metadata: { 
        sessionId: audioStreamer.sessionId,
        userStartMs: Date.now() 
      },
      streamingCallback: (buffer, format) => {
        if (audioStreamer) {
          audioStreamer.addUserAudio(buffer, format);
        }
      }
    });

    if (!nativeAudio) {
      throw new Error('Native audio module not loaded. Please wait for app initialization.');
    }

    await nativeAudio.startSystemAudioCapture({
      streamingCallback: (buffer, format) => {
        if (audioStreamer) {
          audioStreamer.addProspectAudio(buffer, format);
        }
      }
    });

    return { 
      success: true, 
      sessionId: audioStreamer.sessionId 
    };
  } catch (error) {
    console.error('[Main Process] ❌ Error starting audio streaming:', error);
    return { success: false, error: error.message };
  }
});

// Stop audio streaming
ipcMain.handle('stop-audio-streaming', async (event, { sendTermination = true } = {}) => {
  try {
    // Stop streaming callbacks
    await stopUserStreaming();
    if (nativeAudio) {
      nativeAudio.setStreamingCallback(null);
    }

    // Stop AudioStreamer (disconnects WebSockets)
    if (audioStreamer) {
      await audioStreamer.stop(sendTermination);
      audioStreamer = null;
    }

    return { success: true };
  } catch (error) {
    console.error('[Main Process] ❌ Error stopping audio streaming:', error);
    return { success: false, error: error.message };
  }
});

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
// TODO: Add insights websocket (3rd websocket) later
ipcMain.handle('start-cue', async (event, { sessionId, token }) => {
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
        event.sender.send('cue-status', { user: 'connected' });
      },
      onProspectConnected: () => {
        event.sender.send('cue-status', { prospect: 'connected' });
      },
      onError: (stream, error) => {
        console.error(`❌ [Cue] ${stream} stream error:`, error);
        event.sender.send('cue-error', { stream, error: error.message });
      }
    });

    // Start audio streaming (2 websockets)
    await cueAudioStreamer.start(token);

    // TODO: Create insights websocket (3rd websocket) here
    // cueInsightsWebSocket = new WebSocketClient('insights', STREAMING_ENDPOINTS.insights, {
    //   token: token,
    //   sessionId: sessionId,
    //   onConnected: () => {
    //     console.log('✅ [Cue] Insights websocket connected');
    //     event.sender.send('cue-status', { insights: 'connected' });
    //   },
    //   onError: (error) => {
    //     console.error('❌ [Cue] Insights websocket error:', error);
    //     event.sender.send('cue-error', { stream: 'insights', error: error.message });
    //   }
    // });
    // cueInsightsWebSocket.on('message', (message) => {
    //   event.sender.send('cue-insight', message);
    // });
    // await cueInsightsWebSocket.connect();

    // Set up audio capture callbacks (streaming only - no file saving)
    await startUserStreaming({
      streamingCallback: (buffer, format) => {
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
      streamingCallback: (buffer, format) => {
        if (cueAudioStreamer) {
          cueAudioStreamer.addProspectAudio(buffer, format);
        }
      }
    });

    return { 
      success: true, 
      sessionId: sessionId 
    };
  } catch (error) {
    console.error('[Main Process] ❌ Error starting Cue:', error);
    // Clean up on error
    cueAudioStreamer = null;
    return { success: false, error: error.message };
  }
});

// Stop Cue (closes all websockets)
ipcMain.handle('stop-cue', async (event) => {
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
  } catch (error) {
    console.error('[Main Process] ❌ Error stopping Cue:', error);
    // Force cleanup on error
    cueAudioStreamer = null;
    return { success: false, error: error.message };
  }
});

ipcMain.handle('start-audio-capture', async (event, params = {}) => {
  if (isDev) {
    console.log('[Main Process] Starting dual channel recording...');
    console.log('[Main Process] Parameters:', JSON.stringify(params, null, 2));
  }
  
  try {
    const sessionId = params.sessionId || `session-${Date.now()}`;
    
    // Store session metadata for later reference
    activeRecordingSession = {
      sessionId,
      prospectId: params.prospectId || null,
    timestamp: Date.now(),
    userStartMs: null,
    systemStartMs: null
    };
    
    // Pre-initialize user microphone to avoid start delay
    try {
      await prepareUserMicrophone();
    } catch (e) {
      console.warn('[Main Process] ⚠️ Mic prep failed, continuing with defaults:', e.message);
    }

  // Record start times just before invoking each start
  const systemStartMs = Date.now();
  activeRecordingSession.systemStartMs = systemStartMs;
  const userStartMs = Date.now();
  activeRecordingSession.userStartMs = userStartMs;

  // Start both recordings in parallel
  const [prospectResult, userResult] = await Promise.all([
      // Prospect audio (system audio via native module)
      nativeAudio.startSystemAudioCapture(params),
      
      // User audio (microphone via recorder)
    startUserFullRecording({ ...params, metadata: { ...(params.metadata||{}), sessionId, prospectId: params.prospectId || null, userStartMs } })
    ]);
    
    if (isDev) {
      console.log('[Main Process] ✅ Dual channel recording started successfully');
      console.log('[Main Process] Session ID:', sessionId);
      console.log('[Main Process] Prospect file:', prospectResult?.filePath || 'Will be available on stop');
      console.log('[Main Process] User file:', userResult || 'No file path returned');
    }
    
    return {
      success: true,
      sessionId,
      prospectFile: prospectResult?.filePath || null, // May be null until recording stops
      userFile: userResult || null
    };
  } catch (error) {
    console.error('[Main Process] ❌ Error starting dual recording:', error);
    return {
      success: false,
      error: error.message,
      prospectFile: null,
      userFile: null
    };
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
          console.log(`[Main Process] ✅ Loaded environment from: ${envPath}`);
        }
        loaded = true;
        break;
      } else {
        if (isDev) {
          console.log(`[Main Process] ❌ Not found: ${envPath}`);
        }
      }
    }
    
    if (!loaded) {
      console.warn('[Main Process] ⚠️ No .env.production file found, using defaults');
      // Set production defaults
      process.env.VITE_BACKEND_BASE_URL = 'https://your-production-server.com';
    }
  }
}

// Load environment variables
loadEnvironmentVariables();

// Now require other modules that depend on environment variables
const wav = require('wav');
const FormData = require('form-data');
const axios = require('axios');
const { 
  startUserRecording, 
  startProspectRecording, 
  stopRecording,
  startUserFullRecording,
  startProspectFullRecording,
  stopFullRecording,
  stopUserStreaming,
  startUserStreaming,
  prepareUserMicrophone,
  compressAudioFile
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

// --- Transcription State (Main Process) ---
let isTranscribingActive = false;
let currentAudioBuffer = [];
const SAMPLE_RATE = 44100;
const TARGET_DURATION_MS = 5000;
const BUFFER_THRESHOLD = TARGET_DURATION_MS / 1000 * SAMPLE_RATE * 2;
let transcriptionWindow = null;
let isCurrentlyTranscribing = false;

// --- Helper Function: Convert Float32 to Int16 --- 
function float32ToInt16Array(buffer) {
  let l = buffer.length;
  const buf = new Int16Array(l);
  while (l--) {
    const clamped = Math.max(-1, Math.min(1, buffer[l]));
    buf[l] = clamped * 32767;
  }
  return buf;
}


// Keep track of window instances
let mainTipWindowInstance = null;
let sideInfoWindowInstance = null;
let dashboardWindowInstance = null;

// --- Dashboard Window (Standard Window) ---
const createDashboardWindow = () => {
  if (dashboardWindowInstance) {
      dashboardWindowInstance.focus();
      return;
  }
  const preloadScriptPath = path.join(__dirname, 'preload.js');
  if (isDev) {
    console.log(`MAIN: Dashboard preload path calculated as: ${preloadScriptPath}`);
    if (fs.existsSync(preloadScriptPath)) {
      console.log(`MAIN: Preload script FOUND at: ${preloadScriptPath}`);
    }
  }
  if (!fs.existsSync(preloadScriptPath)) {
    console.error(`🔴🔴🔴 MAIN: Preload script NOT FOUND at: ${preloadScriptPath}`);
  }
  const indexHtmlPath = path.join(process.resourcesPath, 'dist', 'index.html');
  const dashboardWindow = new BrowserWindow({
    width: 1400,
    height: 900,
    icon: path.join(__dirname, '../public/assets/icon.icns'),
    // Prevent fullscreen and maximize, but allow manual resizing
    maximizable: false,
    fullscreenable: false,
    roundedCorners: true,
    vibrancy: 'under-window',
    visualEffectState: 'active',
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
      permissions: ['media', 'microphone'],
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
      isTranscribingActive = false;
      currentAudioBuffer = [];
      transcriptionWindow = null;
  });

  // Enhanced media permissions handler for packaged app
  dashboardWindow.webContents.session.setPermissionRequestHandler((webContents, permission, callback) => {
    const allowedPermissions = ['media', 'microphone', 'display-capture'];
    if (isDev) {
      console.log(`🔐 Permission requested: ${permission}`);
    }
    
    if (allowedPermissions.includes(permission)) {
      if (isDev) {
        console.log(`✅ Granting permission for: ${permission}`);
      }
      // Grant permission immediately for media access
      callback(true);
    } else {
      if (isDev) {
        console.log(`❌ Denying permission for: ${permission}`);
      }
      callback(false);
    }
  });

  // Enhanced device permission handler
  dashboardWindow.webContents.session.setDevicePermissionHandler((webContents, permission, deviceId) => {
    if (isDev) {
      console.log(`📱 Device permission requested for: ${permission} (${deviceId})`);
    }
    // Always allow device access for camera/microphone
    return true;
  });

  // Initialize media devices with better error handling
  dashboardWindow.webContents.on('did-finish-load', () => {
    if (isDev) {
      console.log('🚀 Window loaded, initializing media devices...');
    }
    dashboardWindow.webContents.executeJavaScript(`
      (function() {
        ${isDev ? "console.log('🔍 Initializing media devices in renderer...');" : ''}
        
        // Check if mediaDevices is available
        if (!navigator.mediaDevices) {
          console.error('❌ navigator.mediaDevices is not available');
          return;
        }
        
        // Do NOT auto-request permissions here. We only show macOS dialogs
        // after the user confirms our custom modal (via permissions.requestAll).
        ${isDev ? "console.log('ℹ️ Skipping automatic getUserMedia to avoid prompting macOS dialogs early.');" : ''}
        
        // Enumerate devices
        navigator.mediaDevices.enumerateDevices()
          .then(devices => {
            ${isDev ? "console.log('📱 Available media devices:', devices.map(d => ({ kind: d.kind, deviceId: d.deviceId, label: d.label })));" : ''}
          })
          .catch(err => {
            console.error('❌ Error enumerating devices:', err);
          });
      })();
    `);
  });

  // Handle device change events
  dashboardWindow.webContents.on('media-devices-changed', () => {
    if (isDev) {
      console.log('🔄 Media devices changed, reinitializing...');
    }
    dashboardWindow.webContents.executeJavaScript(`
      if (navigator.mediaDevices && navigator.mediaDevices.enumerateDevices) {
        navigator.mediaDevices.enumerateDevices()
          .then(devices => {
            ${isDev ? "console.log('📱 Updated media devices:', devices.map(d => ({ kind: d.kind, deviceId: d.deviceId, label: d.label })));" : ''}
          })
          .catch(err => console.error('❌ Error enumerating devices:', err));
      }
    `);
  });

  // Enhanced CSP for better camera support
  dashboardWindow.webContents.session.webRequest.onHeadersReceived((details, callback) => {
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

  dashboardWindow.webContents.setWindowOpenHandler(({ url }) => {
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

  dashboardWindow.webContents.on('will-navigate', (event, url) => {
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

// --- IPC Handlers for Floating Windows ---
ipcMain.on('launch-call-windows', () => {
  if (isDev) {
    console.log('IPC: Received launch-call-windows');
  }
  createMainTipWindow();
  createSideInfoWindow();
});

// Handler for opening URLs externally
ipcMain.on('open-external', (event, url) => {
  if (isDev) {
    console.log('🎯 [Electron][open-external] IPC event received!');
    console.log('🎯 [Electron][open-external] URL:', url);
    console.log('🎯 [Electron][open-external] Event sender window ID:', event.sender.id);
  }
  
  try {
    shell.openExternal(url);
    if (isDev) {
      console.log('✅ [Electron][open-external] URL opened externally successfully');
    }
    
    // If it's a Zoom OAuth URL, send reset-to-home
    if (url.includes('zoom/auth')) {
      if (isDev) {
        console.log('🔄 [Electron][open-external] Zoom auth detected, sending reset-to-home [TRIGGER #1]');
      }
      BrowserWindow.getAllWindows().forEach(win => {
        if (isDev) {
          console.log('📤 [Electron][open-external] Sending reset-to-home to window:', win.id);
        }
        win.webContents.send('reset-to-home', { source: 'open-external-ipc', service: 'zoom' });
      });
    }
    
    // If it's a Slack OAuth URL, send reset-to-home
    if (url.includes('slack/auth')) {
      if (isDev) {
        console.log('🔄 [Electron][open-external] Slack auth detected, sending reset-to-home [TRIGGER #1]');
      }
      BrowserWindow.getAllWindows().forEach(win => {
        if (isDev) {
          console.log('📤 [Electron][open-external] Sending reset-to-home to window:', win.id);
        }
        win.webContents.send('reset-to-home', { source: 'open-external-ipc', service: 'slack' });
      });
    }
  } catch (error) {
    console.error('❌ [Electron][open-external] Error opening URL externally:', error);
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
  } catch (e) {
    console.error('[Permissions] ❌ Error checking permissions:', e);
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
        await systemPreferences.openSystemPreferences('privacy', 'Microphone');
      } catch (e) {
        console.warn('[Permissions] ⚠️ Could not open System Settings for Microphone:', e?.message || e);
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
        console.warn('[Permissions] ⚠️ Native module missing requestScreenRecordingPermission');
      }
    } catch (err) {
      console.warn('[Permissions] ⚠️ Screen permission request failed:', err?.message || err);
      screen = false;
    }
    if (isDev) {
      console.log('[Permissions] requestAll result:', { mic, screen, micAction, screenRequested });
    }
    return { mic, screen, micAction, screenRequested };
  } catch (e) {
    console.error('[Permissions] ❌ Error requesting permissions:', e);
    return { mic: false, screen: false, error: e.message };
  }
});

ipcMain.on('close-call-windows', () => {
  if (isDev) {
    console.log('IPC: Received close-call-windows');
  }
  if (mainTipWindowInstance) {
    mainTipWindowInstance.close();
    mainTipWindowInstance = null; // Ensure it's cleared
  }
  if (sideInfoWindowInstance) {
    sideInfoWindowInstance.close();
    sideInfoWindowInstance = null; // Ensure it's cleared
  }
});


// Handler to start transcription process
ipcMain.on('connect-whisper', (event) => {
  if (isDev) {
    console.log('IPC: Received connect-whisper (Start Transcription)');
  }
  isTranscribingActive = true;
  currentAudioBuffer = [];
  isCurrentlyTranscribing = false;
  transcriptionWindow = BrowserWindow.fromWebContents(event.sender);
  if (transcriptionWindow) {
      if (isDev) {
        console.log('MAIN: Transcription activated for window.');
      }
  } else {
       console.error('IPC connect-whisper: Could not find sender window.');
  }
});

// Handler to stop transcription process
ipcMain.on('disconnect-whisper', () => {
    // console.log('IPC: Received disconnect-whisper (Stop Transcription)');
    isTranscribingActive = false;
    currentAudioBuffer = [];
    transcriptionWindow = null;
});

// Handler to receive audio chunks from renderer
ipcMain.on('send-audio-chunk', (_event, float32AudioChunk) => {
    if (!isTranscribingActive || !transcriptionWindow) {
        return;
    }
    
    try {
        const int16Chunk = float32ToInt16Array(float32AudioChunk);
        currentAudioBuffer.push(int16Chunk);

        const currentBufferSize = currentAudioBuffer.reduce((sum, arr) => sum + arr.byteLength, 0);
        if (currentBufferSize >= BUFFER_THRESHOLD) {
            transcribeAudioBuffer();
        }
    } catch (error) {
        console.error("MAIN: Error processing audio chunk:", error);
    }
});


ipcMain.handle('stop-audio-capture', async () => {
  if (isDev) {
    console.log('[Main Process] Stopping dual channel recording...');
  }
  
  try {
    // Stop both recordings in parallel
    const [prospectResult, userResult] = await Promise.all([
      nativeAudio.stopSystemAudioCapture(),
      stopFullRecording()
    ]);
    
    // Extract file paths and actual start times
    let prospectFile = prospectResult?.filePath || null;
    let userFile = userResult?.userFile || null;
    // Pull start times from native/recorder, with session fallbacks
    const prospectActualStartMs = prospectResult?.actualStartMs || activeRecordingSession?.systemStartMs || null;
    const userActualStartMs = userResult?.actualStartMs || activeRecordingSession?.userStartMs || null;
    
    // Get session metadata (stored during start)
    const sessionId = activeRecordingSession?.sessionId || null;
    const prospectId = activeRecordingSession?.prospectId || null;
    const userStartMs = activeRecordingSession?.userStartMs || null;
    const systemStartMs = activeRecordingSession?.systemStartMs || null;

    // Rename files to include millisecond start timestamps (for precise alignment)
    try {
      if (prospectFile && systemStartMs) {
        const dir = path.dirname(prospectFile);
        const newProspect = path.join(dir, `system-${sessionId}-${systemStartMs}.caf`);
        try { fs.renameSync(prospectFile, newProspect); prospectFile = newProspect; } catch (e) { console.warn('[Main Process] ⚠️ Could not rename system file:', e.message); }
      }
      if (userFile && userStartMs) {
        const dirU = path.dirname(userFile);
        const newUser = path.join(dirU, `user-${sessionId}-${userStartMs}.caf`);
        if (path.basename(userFile) !== path.basename(newUser)) {
          try { fs.renameSync(userFile, newUser); userFile = newUser; } catch (e) { console.warn('[Main Process] ⚠️ Could not rename user file:', e.message); }
        }
      }
    } catch (e) {
      console.warn('[Main Process] ⚠️ Rename step failed:', e.message);
    }
    
    if (isDev) {
      console.log('[Main Process] ✅ Dual channel recording stopped successfully');
      console.log('[Main Process] Session ID:', sessionId);
      console.log('[Main Process] Prospect file:', prospectFile || 'No file path returned');
      console.log('[Main Process] User file:', userFile || 'No file path returned');
    }
    
    // Clear session metadata
    const savedSession = activeRecordingSession;
    activeRecordingSession = null;
    
    // Return clean combined result for easy reference in compression/upload
    return { 
      success: prospectResult?.success !== false && !!userFile,
      sessionId,
      prospectId,
      prospectFile,
      userFile,
      prospectActualStartMs,
      userActualStartMs,
      // Ready for file processing:
      // await processAndUploadFiles({ prospectFile, userFile, sessionId, prospectId })
    };
  } catch (error) {
    console.error('[Main Process] ❌ Error stopping dual recording:', error);
    // Clear session on error
    activeRecordingSession = null;
    return {
      success: false,
      error: error.message,
      sessionId: null,
      prospectId: null,
      prospectFile: null,
      userFile: null
    };
  }
});

// Compress audio file handler
ipcMain.handle('compress-audio', async (event, options) => {
  if (isDev) {
    console.log('[Main Process] Compressing audio file:', options);
  }
  try {
    const result = await compressAudioFile(options);
    if (isDev) {
      console.log('[Main Process] ✅ Audio compression successful:', result);
    }
    return result;
  } catch (error) {
    console.error('[Main Process] ❌ Error compressing audio:', error);
    throw error;
  }
});

// Upload file handler - reads file from disk and uploads to server
ipcMain.handle('upload-file', async (event, { filePath, type, parentId, accessToken, fileName, data }) => {
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
    const formData = new FormData();
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
      console.log('[Main Process] ✅ File upload successful:', response.data);
    }
    return response.data;
    
  } catch (error) {
    console.error('[Main Process] ❌ Error uploading file:', error);
    
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
    
    const uploadError = new Error(errorMessage);
    uploadError.originalError = error;
    uploadError.filePath = filePath;
    throw uploadError;
  }
});

// Upload both files handler - reads both files from disk and uploads to server together
ipcMain.handle('upload-both-files', async (event, { user, prospect, sessionId, accessToken }) => {
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
    const formData = new FormData();
    
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
      console.log('[Main Process] ✅ Both files upload successful:', response.data);
    }
    return response.data;
    
  } catch (error) {
    console.error('[Main Process] ❌ Error uploading both files:', error);
    
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
    
    const uploadError = new Error(errorMessage);
    uploadError.originalError = error;
    throw uploadError;
  }
});

// --- Audio Queue Event Handlers ---
audioQueue.on('queued', (item) => {
  if (isDev) {
    console.log(`[Audio Queue] Queued new audio chunk: ${item.filePath} (${item.speaker})`);
  }
  if (dashboardWindowInstance) {
    dashboardWindowInstance.webContents.send('audio-queue-update', audioQueue.getStatus());
  }
});

audioQueue.on('processing', (item) => {
  if (isDev) {
    console.log(`[Audio Queue] Processing audio chunk: ${item.filePath} (${item.speaker})`);
  }
  if (dashboardWindowInstance) {
    dashboardWindowInstance.webContents.send('audio-queue-update', audioQueue.getStatus());
  }
});

audioQueue.on('completed', (item) => {
  if (isDev) {
    console.log(`[Audio Queue] Completed processing audio chunk: ${item.filePath} (${item.speaker})`);
  }
  if (dashboardWindowInstance) {
    dashboardWindowInstance.webContents.send('audio-queue-update', audioQueue.getStatus());
  }
});

audioQueue.on('failed', (item) => {
  console.error(`[Audio Queue] Failed to process audio chunk after ${item.retries} retries: ${item.filePath} (${item.speaker})`);
  if (dashboardWindowInstance) {
    dashboardWindowInstance.webContents.send('audio-queue-update', audioQueue.getStatus());
  }
});

audioQueue.on('retrying', (item) => {
  if (isDev) {
    console.log(`[Audio Queue] Retrying audio chunk (attempt ${item.retries}): ${item.filePath} (${item.speaker})`);
  }
  if (dashboardWindowInstance) {
    dashboardWindowInstance.webContents.send('audio-queue-update', audioQueue.getStatus());
  }
});

audioQueue.on('queueEmpty', () => {
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
    console.log('🔄 [Electron] Reloading page...');
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
app.on('open-url', (event, url) => {
  if (isDev) {
    console.log('[Electron] open-url event:', url);
    console.log('Protocol URL received:', url);
  }
  event.preventDefault();
  
  // Parse the URL to extract parameters
  const urlObj = new URL(url);
  const params = new URLSearchParams(urlObj.search);
  
  // Handle zoom callback
  if (urlObj.pathname === '/zoom-callback') {
    const connected = params.get('connected');
    if (isDev) {
      console.log('Zoom callback received, connected:', connected);
    }
    
    // Send the callback data to the renderer process
    if (dashboardWindowInstance) {
      dashboardWindowInstance.webContents.send('zoom-callback', { connected });
    }
  }
});

// Handle second instance (when app is already running and opened via protocol)
app.on('second-instance', (event, commandLine, workingDirectory) => {
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
  
  // Load native audio module AFTER logging is set up
  try {
    nativeAudio = require('./native-audio');
    
    // Register IPC handlers AFTER native module is loaded
    // Initialize native audio module
    ipcMain.handle('native-audio-initialize', async () => {
      console.log('11111')
      try {
        await nativeAudio.initialize();
        return { success: true };
      } catch (error) {
        console.error('🎤 [MAIN] Failed to initialize native audio:', error);
        return { success: false, error: error.message };
      }
    });

    // List output devices
    ipcMain.handle('native-audio-list-devices', async () => {
      try {
        const devices = await nativeAudio.listOutputDevices();
        return { success: true, devices };
      } catch (error) {
        console.error('🎤 [MAIN] Failed to list devices:', error);
        return { success: false, error: error.message };
      }
    });

    // Create multi-output device
    ipcMain.handle('native-audio-create-device', async (event, { name, subDevices }) => {
      try {
        const deviceId = await nativeAudio.createMultiOutputDevice(name, subDevices);
        return { success: true, deviceId };
      } catch (error) {
        console.error('🎤 [MAIN] Failed to create device:', error);
        return { success: false, error: error.message };
      }
    });

    // Delete multi-output device
    ipcMain.handle('native-audio-delete-device', async (event, { deviceId }) => {
      try {
        const result = await nativeAudio.deleteMultiOutputDevice(deviceId);
        return { success: result };
      } catch (error) {
        console.error('🎤 [MAIN] Failed to delete device:', error);
        return { success: false, error: error.message };
      }
    });

    // Request screen recording permission
    ipcMain.handle('native-audio-request-permission', async () => {
      try {
        const result = await nativeAudio.requestScreenRecordingPermission();
        return { success: result };
      } catch (error) {
        console.error('🎤 [MAIN] Failed to request permission:', error);
        return { success: false, error: error.message };
      }
    });

    // Start system audio capture
    ipcMain.handle('native-audio-start-capture', async (event, options = {}) => {
      try {
        const result = await nativeAudio.startSystemAudioCapture(options);
        return { success: result };
      } catch (error) {
        console.error('🎤 [MAIN] Failed to start capture:', error);
        return { success: false, error: error.message };
      }
    });

    // Stop system audio capture
    ipcMain.handle('native-audio-stop-capture', async () => {
      try {
        const result = await nativeAudio.stopSystemAudioCapture();
        // Result is now {success, filePath}
        return result;
      } catch (error) {
        console.error('🎤 [MAIN] Failed to stop capture:', error);
        return { success: false, error: error.message, filePath: null };
      }
    });

    // Check if system audio capture is active
    ipcMain.handle('native-audio-is-capturing', async () => {
      try {
        const result = await nativeAudio.isSystemAudioCaptureActive();
        return { success: true, isCapturing: result };
      } catch (error) {
        console.error('🎤 [MAIN] Failed to check capture status:', error);
        return { success: false, error: error.message };
      }
    });
  } catch (error) {
    console.error('🎤 [MAIN] Failed to load native audio module');
    console.error('🎤 [MAIN] Error message:', error.message);
    console.error('🎤 [MAIN] Error code:', error.code);
    console.error('🎤 [MAIN] Error stack:', error.stack);
    console.error('🎤 [MAIN] Full error:', error);
  }
  
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

let lastLeaveUrl = null;
let lastLeaveUrlTime = 0;
let isProcessingPostCall = false;

// Intercept navigation in ALL windows
app.on('web-contents-created', (event, contents) => {
  contents.on('will-navigate', (event, url) => {
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
      BrowserWindow.getAllWindows().forEach(win => {
        if (isDev) {
          console.log('[Electron][DEBUG] will-navigate: Sending reset-to-home to window:', win.id, { meetingId, prospectId, sessionId });
        }
        win.webContents.send('reset-to-home', { meetingId, prospectId, sessionId });
      });
    }
  });

  contents.setWindowOpenHandler(({ url }) => {
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
      BrowserWindow.getAllWindows().forEach(win => {
        if (isDev) {
          console.log('[Electron][setWindowOpenHandler] Sending reset-to-home to window:', win.id, { meetingId, prospectId, sessionId });
        }
        win.webContents.send('reset-to-home', { meetingId, prospectId, sessionId });
      });
      return { action: 'deny' };
    }
    return { action: 'allow' };
  });

  contents.session.webRequest.onBeforeRequest({ urls: ['*://*/*'] }, (details, callback) => {
    if (isDev) {
      console.log('[Electron][webRequest.onBeforeRequest] URL:', details.url);
    }
    callback({});
  });
});

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
ipcMain.on('get-coach-window-state', (event) => {
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
      permissions: ['media', 'microphone'],
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

  coachWindow.on('closed', () => {
    if (isDev) {
      console.log('Coach window closed event fired, cleaning up reference');
    }
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
ipcMain.on('resize-coach-window', (event, width, height) => {
  if (isDev) {
    console.log(`IPC: Received resize-coach-window request: ${width}x${height}`);
  }
  WindowManager.resizeCoachWindow(global.coachWindow, width, height);
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

// Handler for demo insights from AdminPanel - forwards to coach window
ipcMain.on('demo-insight', (event, insightData) => {
  if (isDev) {
    console.log('📤 [Main Process] Received demo-insight:', insightData);
  }
  
  // Forward to coach window if it exists and is not destroyed
  if (global.coachWindow && !global.coachWindow.isDestroyed()) {
    global.coachWindow.webContents.send('cue-insight', insightData);
    if (isDev) {
      console.log('✅ [Main Process] Demo insight forwarded to coach window');
    }
  } else {
    if (isDev) {
      console.warn('⚠️ [Main Process] Coach window not available, cannot forward demo insight');
    }
  }
});
