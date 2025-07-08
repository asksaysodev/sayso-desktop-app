const { app, BrowserWindow, ipcMain, screen, shell } = require('electron');
const path = require('node:path');
const fs = require('node:fs');

// Load environment variables FIRST, before any other modules
if (!process.env.NODE_ENV) {
  process.env.NODE_ENV = 'production';
}

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
        console.log(`[Main Process] ✅ Loaded environment from: ${envPath}`);
        loaded = true;
        break;
      } else {
        console.log(`[Main Process] ❌ Not found: ${envPath}`);
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

// Log environment for debugging
console.log('[Main Process] Environment loaded:', {
  NODE_ENV: process.env.NODE_ENV,
  VITE_BACKEND_BASE_URL: process.env.VITE_BACKEND_BASE_URL
});

// Add this after environment loading
console.log('[Main Process] Environment check:', {
  NODE_ENV: process.env.NODE_ENV,
  VITE_BACKEND_BASE_URL: process.env.VITE_BACKEND_BASE_URL,
  'env file path': process.env.NODE_ENV === 'production' 
    ? path.resolve(__dirname, '../.env.production')
    : path.resolve(__dirname, '../.env')
});

// Now require other modules that depend on environment variables
const wav = require('wav');
const FormData = require('form-data');
const axios = require('axios');
const { startUserRecording, startProspectRecording, stopRecording } = require('./recorder');
const audioQueue = require('./audioQueue');

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

const isDev = process.env.NODE_ENV !== 'production';

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
  console.log(`MAIN: Dashboard preload path calculated as: ${preloadScriptPath}`);
  if (fs.existsSync(preloadScriptPath)) {
    console.log(`MAIN: Preload script FOUND at: ${preloadScriptPath}`);
  } else {
    console.error(`🔴🔴🔴 MAIN: Preload script NOT FOUND at: ${preloadScriptPath}`);
  }
  const indexHtmlPath = path.join(process.resourcesPath, 'dist', 'index.html');
  const dashboardWindow = new BrowserWindow({
    width: 1024,
    height: 768,
    icon: path.join(__dirname, '../public/assets/icon.icns'),
    webPreferences: {
      preload: preloadScriptPath,
      contextIsolation: true,
      nodeIntegration: false,
      webSecurity: true,
      // Enhanced media permissions for packaged app
      enableBlinkFeatures: 'MediaDevices,MediaStream,WebRTC',
      permissions: ['media', 'camera', 'microphone'],
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
    const allowedPermissions = ['media', 'camera', 'microphone', 'display-capture'];
    console.log(`🔐 Permission requested: ${permission}`);
    
    if (allowedPermissions.includes(permission)) {
      console.log(`✅ Granting permission for: ${permission}`);
      // Grant permission immediately for media access
      callback(true);
    } else {
      console.log(`❌ Denying permission for: ${permission}`);
      callback(false);
    }
  });

  // Enhanced device permission handler
  dashboardWindow.webContents.session.setDevicePermissionHandler((webContents, permission, deviceId) => {
    console.log(`📱 Device permission requested for: ${permission} (${deviceId})`);
    // Always allow device access for camera/microphone
    return true;
  });

  // Initialize media devices with better error handling
  dashboardWindow.webContents.on('did-finish-load', () => {
    console.log('🚀 Window loaded, initializing media devices...');
    dashboardWindow.webContents.executeJavaScript(`
      console.log('🔍 Initializing media devices in renderer...');
      
      // Check if mediaDevices is available
      if (!navigator.mediaDevices) {
        console.error('❌ navigator.mediaDevices is not available');
        return;
      }
      
      // Request permissions early
      navigator.mediaDevices.getUserMedia({ audio: true, video: true })
        .then(stream => {
          console.log('✅ Media permissions granted, stopping test stream');
          stream.getTracks().forEach(track => track.stop());
        })
        .catch(err => {
          console.error('❌ Error getting media permissions:', err);
        });
      
      // Enumerate devices
      navigator.mediaDevices.enumerateDevices()
        .then(devices => {
          console.log('📱 Available media devices:', devices.map(d => ({
            kind: d.kind,
            deviceId: d.deviceId,
            label: d.label
          })));
        })
        .catch(err => {
          console.error('❌ Error enumerating devices:', err);
        });
    `);
  });

  // Handle device change events
  dashboardWindow.webContents.on('media-devices-changed', () => {
    console.log('🔄 Media devices changed, reinitializing...');
    dashboardWindow.webContents.executeJavaScript(`
      if (navigator.mediaDevices && navigator.mediaDevices.enumerateDevices) {
        navigator.mediaDevices.enumerateDevices()
          .then(devices => {
            console.log('📱 Updated media devices:', devices.map(d => ({
              kind: d.kind,
              deviceId: d.deviceId,
              label: d.label
            })));
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
    console.log('[Electron][setWindowOpenHandler] Attempt to open URL:', url);
    // If the url is a file:// index.html with a hash, route it in the main window
    if (url.startsWith('file://') && url.includes('index.html#/post-call/')) {
      console.log('[Electron][setWindowOpenHandler] Intercepted leaveUrl, loading in main window:', url);
      dashboardWindow.loadURL(url);
      return { action: 'deny' }; // Prevent new window
    }
    return { action: 'allow' };
  });

  dashboardWindow.webContents.on('will-navigate', (event, url) => {
    console.log('[Electron][will-navigate] Navigation attempt to:', url);

    // Patch for malformed protocol
    if (url.startsWith('https://file///')) {
      event.preventDefault();
      // Fix to 'file:///' (with colon and three slashes)
      const fixedUrl = url.replace('https://file///', 'file:///');
      console.log('[Electron][will-navigate] Fixed protocol, loading:', fixedUrl);
      dashboardWindow.loadURL(fixedUrl);
    }
  });

  // Now it's safe to access webContents!
  dashboardWindow.webContents.on('will-navigate', (event, url) => {
    console.log('[Electron] 🔍 DEBUG: will-navigate triggered with URL:', url);
    
    if (url.startsWith('https://google.com')) {
      event.preventDefault();
      shell.openExternal(url);
      
      // Extract meetingId and prospectId from query parameters
      const urlObj = new URL(url);
      const params = new URLSearchParams(urlObj.search);
      const meetingId = params.get('meetingId');
      const prospectId = params.get('prospectId');
      
      console.log('[Electron] 🔍 DEBUG: Extracted params - meetingId:', meetingId, 'prospectId:', prospectId);
      console.log('[Electron] 🚀 Sending reset-to-home with params:', { meetingId, prospectId });
      dashboardWindow.webContents.send('reset-to-home', { meetingId, prospectId });
    }
  });

  dashboardWindow.webContents.setWindowOpenHandler(({ url }) => {
    console.log('[Electron] 🔍 DEBUG: setWindowOpenHandler triggered with URL:', url);
    
    if (url.startsWith('https://google.com')) {
      shell.openExternal(url);
      
      // Extract meetingId and prospectId from query parameters
      const urlObj = new URL(url);
      const params = new URLSearchParams(urlObj.search);
      const meetingId = params.get('meetingId');
      const prospectId = params.get('prospectId');
      
      console.log('[Electron] 🔍 DEBUG: Extracted params - meetingId:', meetingId, 'prospectId:', prospectId);
      console.log('[Electron] 🚀 Sending reset-to-home with params:', { meetingId, prospectId });
      dashboardWindow.webContents.send('reset-to-home', { meetingId, prospectId });
      return { action: 'deny' };
    }
    return { action: 'allow' };
  });
};

// --- IPC Handlers for Floating Windows ---
ipcMain.on('launch-call-windows', () => {
  console.log('IPC: Received launch-call-windows');
  createMainTipWindow();
  createSideInfoWindow();
});

ipcMain.on('close-call-windows', () => {
  console.log('IPC: Received close-call-windows');
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
  console.log('IPC: Received connect-whisper (Start Transcription)');
  isTranscribingActive = true;
  currentAudioBuffer = [];
  isCurrentlyTranscribing = false;
  transcriptionWindow = BrowserWindow.fromWebContents(event.sender);
  if (transcriptionWindow) {
      console.log('MAIN: Transcription activated for window.');
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

// --- Audio Capture Handlers ---
ipcMain.handle('start-audio-capture', async (event, params) => {
  console.log('MAIN: Received start-audio-capture');

  try {
    // Start both user and prospect recording
    startUserRecording({
      duration: 8,
      metadata: {
        accountId: params.accountId,
        prospectId: params.prospectId,
        meetingId: params.meetingId
      },
      onChunk: (filePath, speaker) => {
        console.log(`[Main Process] Received audio chunk from ${speaker}: ${filePath}`);
      }
    });
    console.log('[Main Process] User recording started.');

    startProspectRecording({
      duration: 8,
      metadata: {
        accountId: params.accountId,
        prospectId: params.prospectId,
        meetingId: params.meetingId
      },
      onChunk: (filePath, speaker) => {
        console.log(`[Main Process] Received audio chunk from ${speaker}: ${filePath}`);
      }
    });
    console.log('[Main Process] Prospect recording started.');

    return { status: "Audio capture processes started for user and prospect" };
  } catch (error) {
    console.error('[Main Process] Error starting one or more audio capture processes:', error);
    // Attempt to stop any potentially started processes
    stopRecording(); 
    return { status: "Failed to start audio capture", error: error.message };
  }
});

ipcMain.handle('stop-audio-capture', async () => { // Made async to align with start
  console.log('[Main Process] Received stop-audio-capture request.');
  try {
    stopRecording(); // Assuming this is synchronous or manages its own async cleanup
    console.log('[Main Process] stopRecording called.');
    return { status: "Audio capture stopped" }; // Example response
  } catch (error) {
    console.error('[Main Process] Error during stopRecording:', error);
    throw error; // Propagate error to renderer
  }
});

// --- Audio Queue Event Handlers ---
audioQueue.on('queued', (item) => {
  console.log(`[Audio Queue] Queued new audio chunk: ${item.filePath} (${item.speaker})`);
  if (dashboardWindowInstance) {
    dashboardWindowInstance.webContents.send('audio-queue-update', audioQueue.getStatus());
  }
});

audioQueue.on('processing', (item) => {
  console.log(`[Audio Queue] Processing audio chunk: ${item.filePath} (${item.speaker})`);
  if (dashboardWindowInstance) {
    dashboardWindowInstance.webContents.send('audio-queue-update', audioQueue.getStatus());
  }
});

audioQueue.on('completed', (item) => {
  console.log(`[Audio Queue] Completed processing audio chunk: ${item.filePath} (${item.speaker})`);
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
  console.log(`[Audio Queue] Retrying audio chunk (attempt ${item.retries}): ${item.filePath} (${item.speaker})`);
  if (dashboardWindowInstance) {
    dashboardWindowInstance.webContents.send('audio-queue-update', audioQueue.getStatus());
  }
});

audioQueue.on('queueEmpty', () => {
  console.log('[Audio Queue] Queue is now empty');
  if (dashboardWindowInstance) {
    dashboardWindowInstance.webContents.send('audio-queue-update', audioQueue.getStatus());
  }
});

// Add IPC handler for getting queue status
ipcMain.handle('get-audio-queue-status', () => {
  return audioQueue.getStatus();
});

// Handle protocol activation (when app is opened via sayso:// URL)
app.on('open-url', (event, url) => {
  console.log('[Electron] open-url event:', url);
  event.preventDefault();
  console.log('Protocol URL received:', url);
  
  // Parse the URL to extract parameters
  const urlObj = new URL(url);
  const params = new URLSearchParams(urlObj.search);
  
  // Handle zoom callback
  if (urlObj.pathname === '/zoom-callback') {
    const connected = params.get('connected');
    console.log('Zoom callback received, connected:', connected);
    
    // Send the callback data to the renderer process
    if (dashboardWindowInstance) {
      dashboardWindowInstance.webContents.send('zoom-callback', { connected });
    }
  }
});

// Handle second instance (when app is already running and opened via protocol)
app.on('second-instance', (event, commandLine, workingDirectory) => {
  console.log('Second instance detected, command line:', commandLine);
  
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
  // ONLY create the dashboard window initially
  createDashboardWindow(); 
  
  // DO NOT create floating windows here anymore
  // createMainTipWindow(); 
  // createSideInfoWindow(); 

  // Show dock icon ONLY if the dashboard is the intended primary interface
  // app.dock?.hide(); // Comment this out or make conditional

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
  // If you want the app to quit when the dashboard closes even on macOS,
  // you would add app.quit() here.
});

// app.commandLine.appendSwitch('disable-gpu'); // <--- comment this out
// app.commandLine.appendSwitch('disable-software-rasterizer', 'false'); // <--- comment this out
// app.commandLine.appendSwitch('ignore-gpu-blacklist');
// app.commandLine.appendSwitch('enable-webgl');
// app.commandLine.appendSwitch('enable-accelerated-video');

console.log("NODE_ENV:", process.env.NODE_ENV);
console.log("Loaded FRONTEND_BASE_URL:", process.env.VITE_FRONTEND_BASE_URL);

function shouldOpenExternally(url) {
  // For now, just match google.com
  return url.startsWith('https://google.com');
}

// Intercept navigation in ALL windows
app.on('web-contents-created', (event, contents) => {
  contents.on('will-navigate', (event, url) => {
    console.log('[Electron] 🔍 DEBUG: web-contents-created will-navigate triggered with URL:', url);
    
    if (url.startsWith('https://google.com')) {
      event.preventDefault();
      shell.openExternal(url);

      // Extract meetingId and prospectId from query parameters
      const urlObj = new URL(url);
      const params = new URLSearchParams(urlObj.search);
      const meetingId = params.get('meetingId');
      const prospectId = params.get('prospectId');
      
      console.log('[Electron] 🔍 DEBUG: Extracted params - meetingId:', meetingId, 'prospectId:', prospectId);

      // Send IPC to ALL windows
      BrowserWindow.getAllWindows().forEach(win => {
        console.log('[Electron] 🚀 Sending reset-to-home to window with params:', { meetingId, prospectId });
        win.webContents.send('reset-to-home', { meetingId, prospectId });
      });
    }
  });

  contents.setWindowOpenHandler(({ url }) => {
    console.log('[Electron] 🔍 DEBUG: web-contents-created setWindowOpenHandler triggered with URL:', url);
    
    if (url.startsWith('https://google.com')) {
      shell.openExternal(url);
      
      // Extract meetingId and prospectId from query parameters
      const urlObj = new URL(url);
      const params = new URLSearchParams(urlObj.search);
      const meetingId = params.get('meetingId');
      const prospectId = params.get('prospectId');
      
      console.log('[Electron] 🔍 DEBUG: Extracted params - meetingId:', meetingId, 'prospectId:', prospectId);
      
      BrowserWindow.getAllWindows().forEach(win => {
        console.log('[Electron] 🚀 Sending reset-to-home to window with params:', { meetingId, prospectId });
        win.webContents.send('reset-to-home', { meetingId, prospectId });
      });
      return { action: 'deny' };
    }
    return { action: 'allow' };
  });
});

