const fs = require('fs');
const path = require('path');
const { spawn } = require('child_process');
const { app } = require('electron');
const audioQueue = require('./audioQueue');
const uploadService = require('./uploadService');
const { exec } = require('child_process');
const isDev = process.env.NODE_ENV !== 'production';

// Add this function right after the imports
function getFfmpegPath() {
  // 1. Try bundled FFmpeg first (most reliable)
  const bundleFfmpeg = path.join(process.resourcesPath, 'bin', 'ffmpeg');
  if (fs.existsSync(bundleFfmpeg)) {
    console.log('🎤 [RECORDER] Using bundled FFmpeg:', bundleFfmpeg);
    return bundleFfmpeg;
  }
  
  // 2. Try local bin directory (for development)
  const localFfmpeg = path.join(__dirname, 'bin', 'ffmpeg');
  if (fs.existsSync(localFfmpeg)) {
    console.log('🎤 [RECORDER] Using local FFmpeg:', localFfmpeg);
    return localFfmpeg;
  }
  
  // 3. Try system FFmpeg as fallback
  try {
    const { execSync } = require('child_process');
    const ffmpegPath = execSync('which ffmpeg', { encoding: 'utf8' }).trim();
    if (ffmpegPath) {
      console.log('🎤 [RECORDER] Using system FFmpeg:', ffmpegPath);
      return ffmpegPath;
    }
  } catch (error) {
    console.log('🎤 [RECORDER] System FFmpeg not found');
  }
  
  // 4. Last resort
  console.warn('🎤 [RECORDER] FFmpeg not found anywhere, trying PATH');
  return 'ffmpeg';
}

// Cached device list to avoid querying FFmpeg every time
let cachedInputDevices = null;
let deviceListCacheTime = null;
const DEVICE_CACHE_DURATION = 5 * 60 * 1000; // Cache for 5 minutes

// Helper function to get available audio input devices from FFmpeg
async function getInputDevices() {
  return new Promise((resolve, reject) => {
    const ffmpegPath = getFfmpegPath();
    
    // FFmpeg command to list avfoundation devices
    // Note: FFmpeg outputs device list to stderr
    const command = `"${ffmpegPath}" -f avfoundation -list_devices true -i "" 2>&1`;
    
    console.log('🎤 [RECORDER] Querying FFmpeg for input devices...');
    
    exec(command, { maxBuffer: 1024 * 1024 }, (error, stdout, stderr) => {
      const output = stderr || stdout;
      const devices = [];
      
      // Parse FFmpeg output to extract input devices
      // Example format:
      // [AVFoundation input device @ 0x...] AVFoundation video devices:
      // [AVFoundation input device @ 0x...] [0] FaceTime HD Camera
      // [AVFoundation input device @ 0x] AVFoundation audio devices:
      // [AVFoundation input device @ 0x] [0] Built-in Microphone
      // [AVFoundation input device @ 0x] [1] External USB Microphone
      
      const lines = output.split('\n');
      let inAudioSection = false;
      
      for (const line of lines) {
        // Look for audio devices section
        if (line.includes('AVFoundation audio devices:')) {
          inAudioSection = true;
          continue;
        }
        
        // Look for video devices section or next section (end of audio section)
        if (line.includes('AVFoundation video devices:') || 
            (line.includes('[in#') || line.includes('Error opening'))) {
          if (inAudioSection) {
            // We've passed the audio section
            break;
          }
          continue;
        }
        
        // Parse device lines: [AVFoundation indev @ 0x...] [X] Device Name
        // Match pattern like: [AVFoundation indev @ 0x7faf36a04700] [3] MacBook Pro Microphone
        const match = line.match(/\[AVFoundation.*?\]\s+\[(\d+)\]\s+(.+)/);
        if (match && inAudioSection) {
          devices.push({
            index: parseInt(match[1]),
            name: match[2].trim()
          });
        }
      }
      
      if (devices.length === 0) {
        console.warn('🎤 [RECORDER] No input devices found in FFmpeg output');
        console.warn('🎤 [RECORDER] FFmpeg output:', output.substring(0, 500));
      }
      
      resolve(devices);
    });
  });
}

// Helper function to cleanup FFmpeg listeners
function cleanupFFmpegListeners() {
  if (global.userFullRecordingProcess) {
    try {
      // Remove event listeners if they exist
      if (global.userFullRecordingProcess._stderrHandler) {
        global.userFullRecordingProcess.stderr.removeListener('data', global.userFullRecordingProcess._stderrHandler);
        delete global.userFullRecordingProcess._stderrHandler;
      }
      if (global.userFullRecordingProcess._errorHandler) {
        global.userFullRecordingProcess.removeListener('error', global.userFullRecordingProcess._errorHandler);
        delete global.userFullRecordingProcess._errorHandler;
      }
      if (global.userFullRecordingProcess._exitHandler) {
        global.userFullRecordingProcess.removeListener('exit', global.userFullRecordingProcess._exitHandler);
        delete global.userFullRecordingProcess._exitHandler;
      }
      console.log('[Recording Control] ✅ FFmpeg listeners cleaned up');
    } catch (error) {
      console.warn('[Recording Control] ⚠️ Error cleaning up FFmpeg listeners:', error.message);
    }
  }
}

// Probe candidate avfoundation audio indices by doing a short capture
async function probeForWorkingMicrophone(candidateIndices = [0, 1, 2, 3]) {
  const ffmpegPath = getFfmpegPath();
  const tempDir = getWritableTempDir('full_recordings');
  if (!fs.existsSync(tempDir)) {
    fs.mkdirSync(tempDir, { recursive: true });
  }
  
  console.log(`🎤 [RECORDER] Probing microphone indices: ${candidateIndices.join(', ')}`);
  
  for (const idx of candidateIndices) {
    const probeFile = path.join(tempDir, `mic-probe-${idx}.caf`);
    console.log(`🎤 [RECORDER] Probing index ${idx}...`);
    
    try {
      const probeResult = await new Promise((resolve, reject) => {
        const args = [
          '-f', 'avfoundation',
          '-i', `:${idx}`,
          '-t', '1', // Reduced to 1 second for faster probing
          '-ar', AUDIO_SAMPLE_RATE,
          '-ac', AUDIO_CHANNELS,
          '-c:a', 'pcm_s16le',
          '-f', 'caf',
          '-y', probeFile
        ];
        const proc = spawn(ffmpegPath, args);
        let stderrOutput = '';
        let exitCode = null;
        
        // Collect stderr (FFmpeg logs to stderr)
        proc.stderr.on('data', (data) => {
          stderrOutput += data.toString();
        });
        
        proc.on('error', (e) => {
          console.log(`🎤 [RECORDER] Probe ${idx} process error:`, e.message);
          reject(e);
        });
        
        proc.on('exit', (code) => {
          exitCode = code;
          resolve({ code, stderrOutput });
        });
        
        // Timeout after 3 seconds
        setTimeout(() => {
          if (exitCode === null) {
            console.log(`🎤 [RECORDER] Probe ${idx} timed out, killing...`);
            proc.kill('SIGKILL');
            reject(new Error(`Probe timeout for index ${idx}`));
          }
        }, 3000);
      });
      
      // Check if file was created and has content
      if (fs.existsSync(probeFile)) {
        const size = fs.statSync(probeFile).size;
        const hasError = probeResult.stderrOutput.toLowerCase().includes('error') || 
                        probeResult.stderrOutput.toLowerCase().includes('no such file') ||
                        probeResult.stderrOutput.toLowerCase().includes('invalid data');
        
        console.log(`🎤 [RECORDER] Probe ${idx} result: exit code=${probeResult.code}, size=${size} bytes, hasError=${hasError}`);
        
        // Clean up probe file
        try {
          fs.unlinkSync(probeFile);
        } catch (e) {
          // Ignore cleanup errors
        }
        
        // Success if: exit code is 0 or 255 (normal/terminated), file size > 5KB, and no errors
        if ((probeResult.code === 0 || probeResult.code === 255) && size > 5000 && !hasError) {
          console.log(`🎤 [RECORDER] ✅ Probe ${idx} successful - found working microphone!`);
          return idx;
        } else {
          console.log(`🎤 [RECORDER] ❌ Probe ${idx} failed: code=${probeResult.code}, size=${size}, hasError=${hasError}`);
        }
      } else {
        console.log(`🎤 [RECORDER] ❌ Probe ${idx} failed: no output file created`);
      }
    } catch (e) {
      console.log(`🎤 [RECORDER] ❌ Probe ${idx} exception:`, e.message);
      // Clean up probe file on error
      try { 
        if (fs.existsSync(probeFile)) fs.unlinkSync(probeFile); 
      } catch {}
    }
  }
  
  console.warn('🎤 [RECORDER] ❌ All probe attempts failed');
  return null;
}

// Helper to select the best microphone from available devices
function selectBestMicrophone(devices) {
  if (!devices || devices.length === 0) {
    console.log('🎤 [RECORDER] No devices found, using system default (:0)');
    return { index: 0, name: 'System Default (:0)' };
  }
  
  // Virtual devices to exclude (these are software/audio routing tools)
  const virtualDeviceKeywords = ['blackhole', 'loom', 'teams', 'zoom', 'boom'];
  
  // Try to find a physical microphone
  // Priority: devices with "Microphone" in name that aren't virtual
  const physicalMics = devices.filter(device => {
    const nameLower = device.name.toLowerCase();
    const isMicrophone = nameLower.includes('microphone');
    const isNotVirtual = !virtualDeviceKeywords.some(keyword => nameLower.includes(keyword));
    return isMicrophone && isNotVirtual;
  });
  
  if (physicalMics.length > 0) {
    // Use the first physical microphone found
    // Often this will be "MacBook Pro Microphone" or similar
    console.log(`🎤 [RECORDER] Found ${physicalMics.length} physical microphone(s), using first: [${physicalMics[0].index}] ${physicalMics[0].name}`);
    return physicalMics[0];
  }
  
  // If no physical microphone with "Microphone" in name, check index 0
  // Note: In FFmpeg avfoundation, :0 refers to system default, which might map to any index
  const deviceAtZero = devices.find(d => d.index === 0);
  if (deviceAtZero) {
    console.log(`🎤 [RECORDER] Using device at index 0 (system default): [${deviceAtZero.index}] ${deviceAtZero.name}`);
    return deviceAtZero;
  }
  
  // Fallback: use first available device
  console.log(`🎤 [RECORDER] Using first available device: [${devices[0].index}] ${devices[0].name}`);
  return devices[0];
}

// Helper function to detect the active/default microphone
// Tries to find a physical microphone, falls back to system default (:0)
async function detectActiveMicrophone() {
  const now = Date.now();
  
  // Check cache first
  if (cachedInputDevices && Array.isArray(cachedInputDevices) && cachedInputDevices.length > 0 &&
      deviceListCacheTime && (now - deviceListCacheTime) < DEVICE_CACHE_DURATION) {
    console.log('🎤 [RECORDER] Using cached device list');
    const selectedDevice = selectBestMicrophone(cachedInputDevices);
    console.log(`🎤 [RECORDER] ✅ Using microphone: [${selectedDevice.index}] ${selectedDevice.name}`);
    return {
      index: selectedDevice.index,
      name: selectedDevice.name,
      devices: cachedInputDevices
    };
  }
  
  try {
    console.log('🎤 [RECORDER] Detecting available microphone devices...');
    const devices = await getInputDevices();
    
    // Cache the device list
    cachedInputDevices = devices;
    deviceListCacheTime = Date.now();
    
    // Log all available devices
    console.log('🎤 [RECORDER] Available input devices:');
    devices.forEach(device => {
      console.log(`🎤 [RECORDER]   [${device.index}] ${device.name}`);
    });
    
    // Select best microphone (prefer physical mics over virtual devices)
    const selectedDevice = selectBestMicrophone(devices);
    
    console.log(`🎤 [RECORDER] ✅ Selected microphone: [${selectedDevice.index}] ${selectedDevice.name}`);
    
    return {
      index: selectedDevice.index,
      name: selectedDevice.name,
      devices: devices
    };
  } catch (error) {
    console.error('🎤 [RECORDER] ❌ Error detecting microphone:', error);
    console.warn('🎤 [RECORDER] Falling back to system default device (:0)');
    return { index: 0, name: 'System Default (:0)', devices: [] };
  }
}

// Pre-initialize microphone selection to minimize start delay
// Returns the selected device info and warms the cache
async function prepareUserMicrophone() {
  try {
    // If cache is valid, just return selection
    const now = Date.now();
    if (cachedInputDevices && Array.isArray(cachedInputDevices) && cachedInputDevices.length > 0 &&
        deviceListCacheTime && (now - deviceListCacheTime) < DEVICE_CACHE_DURATION) {
      const selected = selectBestMicrophone(cachedInputDevices);
      console.log(`🎤 [RECORDER] (prep) Using cached microphone: [${selected.index}] ${selected.name}`);
      return selected;
    }
    // Query and cache devices
    console.log('🎤 [RECORDER] (prep) Warming up microphone device list...');
    const devices = await getInputDevices();
    cachedInputDevices = devices;
    deviceListCacheTime = Date.now();
    const selected = selectBestMicrophone(devices);
    console.log(`🎤 [RECORDER] (prep) Selected microphone: [${selected.index}] ${selected.name}`);
    return selected;
  } catch (e) {
    console.warn('🎤 [RECORDER] (prep) Failed to prepare microphone, defaulting to :0:', e.message);
    return { index: 0, name: 'System Default (:0)' };
  }
}

let userRecordingProcess = null;
let userChunkInterval = null;
let prospectRecordingProcess = null;
let prospectChunkInterval = null;
let processedUserChunks = new Set();
let processedProspectChunks = new Set();
// Track the precise time the microphone actually started writing audio
global.userActualStartMs = null;

// --- Configuration ---
// Audio recording approach - will be reimplemented with native ScreenCaptureKit
const AUDIO_SAMPLE_RATE = '48000';  // Match system audio (48kHz)
const AUDIO_CHANNELS = '2'; // Stereo for consistency
const AUDIO_BITRATE = '64k';  // Reduced from default
const AUDIO_COMPRESSION = 'libmp3lame';  // Using MP3 compression

// Helper: Wait for file to exist before processing (max 2 seconds)
function waitForFile(filePath, timeout = 2000) {
  return new Promise((resolve, reject) => {
    const start = Date.now();
    (function check() {
      if (fs.existsSync(filePath)) return resolve();
      if (Date.now() - start > timeout) return reject(new Error('File not found in time: ' + filePath));
      setTimeout(check, 100);
    })();
  });
}

// Add this helper function at the top of the file, after the imports
const handleAudioResponse = async (filePath, speaker, metadata) => {
  try {
    console.log(`[${speaker} Recording] Starting handleAudioResponse for: ${filePath}`);
    const response = await uploadService.uploadAudioChunk(filePath, speaker, metadata);
    
    console.log(`[${speaker} Recording] Response received:`, {
      hasResponse: !!response,
      hasTranscript: !!(response && response.transcript),
      hasText: !!(response && response.transcript && response.transcript.text),
      textLength: response?.transcript?.text?.length || 0,
      text: response?.transcript?.text?.substring(0, 50) + '...' || 'none'
    });
    
    // Send transcription data to renderer if we have valid transcript
    if (response && response.transcript && response.transcript.text && response.transcript.text.trim()) {
      console.log(`[${speaker} Recording] Global coachWindow exists:`, !!global.coachWindow);
      
      if (global.coachWindow && !global.coachWindow.isDestroyed()) {
        const transcriptionData = {
          text: response.transcript.text,
          speaker: speaker,
          timestamp: response.timestamp || Date.now()
        };
        
        console.log(`[${speaker} Recording] Sending transcription data to coach window:`, transcriptionData);
        global.coachWindow.webContents.send('transcription-data', transcriptionData);
        console.log(`[${speaker} Recording] Sent transcription to coach window:`, response.transcript.text.substring(0, 50) + '...');
      } else {
        console.log(`[${speaker} Recording] Coach window not available or destroyed - skipping transcription`);
      }
    } else {
      console.log(`[${speaker} Recording] Skipping transcription - no valid text`);
    }
    
    return response;
  } catch (error) {
    console.error(`[${speaker} Recording] Error in handleAudioResponse:`, error.message);
    throw error;
  }
};

// Helper function to get writable temp directories
function getWritableTempDir(folderName) {
  const isDev = process.env.NODE_ENV !== 'production';
  
  if (isDev) {
    // Development: use local directory
    return path.join(__dirname, folderName);
  } else {
    // Production: use app's user data directory
    return path.join(app.getPath('userData'), 'temp', folderName);
  }
}

// Audio recording functions - will be reimplemented with native ScreenCaptureKit
async function startUserRecording({ duration = 8, onChunk, metadata = {} }) {
  console.log('🎤 [RECORDER] === STARTING USER RECORDING (MICROPHONE) ===');
  console.log('🎤 [RECORDER] Duration:', duration);
  console.log('🎤 [RECORDER] Metadata:', JSON.stringify(metadata, null, 2));
  
  cleanupTempFolders();
  const tempDir = getWritableTempDir('chunks_user');
  
  // Ensure the directory exists
  if (!fs.existsSync(tempDir)) {
    fs.mkdirSync(tempDir, { recursive: true });
    console.log(`🎤 [RECORDER] Created temp directory: ${tempDir}`);
  } else {
    console.log(`🎤 [RECORDER] Using existing temp directory: ${tempDir}`);
  }
  
  processedUserChunks.clear();

  // Clean temp dir (only if it exists and has files)
  if (fs.existsSync(tempDir)) {
    try {
      const files = fs.readdirSync(tempDir);
      console.log(`🎤 [RECORDER] Found ${files.length} existing files in temp dir`);
      files.forEach(file => {
        try {
          fs.unlinkSync(path.join(tempDir, file));
          console.log(`🎤 [RECORDER] Deleted leftover chunk file: ${file}`);
        } catch (err) {
          console.warn(`🎤 [RECORDER] Could not delete user chunk file: ${file}`, err.message);
        }
      });
    } catch (err) {
      console.warn(`🎤 [RECORDER] Could not read user temp directory: ${err.message}`);
    }
  }

  console.log(`🎤 [RECORDER] Starting microphone recording with FFmpeg`);
  console.log(`🎤 [RECORDER] Chunk duration: ${duration} seconds`);
  console.log(`🎤 [RECORDER] Temp directory: ${tempDir}`);
  
  // Detect active microphone
  const micInfo = await detectActiveMicrophone();
  console.log(`🎤 [RECORDER] Using microphone: [${micInfo.index}] ${micInfo.name}`);
  
  const recordingStartTime = new Date();
  
  // Use detected microphone device
  const ffmpegArgs = [
    '-f', 'avfoundation',
    '-i', `:${micInfo.index}`, // Use detected microphone device
    '-ar', AUDIO_SAMPLE_RATE,
    '-ac', AUDIO_CHANNELS,
    '-c:a', AUDIO_COMPRESSION,
    '-b:a', AUDIO_BITRATE,
    '-flush_packets', '1',
    '-f', 'segment',
    '-segment_time', duration.toString(),
    '-segment_time_delta', '1',
    '-segment_format', 'mp3',
    '-segment_list_size', '0',
    '-segment_wrap', '0',
    `${tempDir}/user-chunk-%03d.mp3`,
  ];
  
  console.log('🎤 [RECORDER] FFmpeg args:', ffmpegArgs.join(' '));
  
  userRecordingProcess = spawn(getFfmpegPath(), ffmpegArgs);
  
  console.log('🎤 [RECORDER] FFmpeg process spawned, PID:', userRecordingProcess.pid);

  userRecordingProcess.stderr.on('data', (data) => {
    const output = data.toString();
    if (output.includes('Error')) {
      console.error('🎤 [RECORDER] FFmpeg Error detected:', output);
    } else {
      console.log('🎤 [RECORDER] FFmpeg output:', output);
    }
  });

  userRecordingProcess.on('error', (error) => {
    console.error('🎤 [RECORDER] FFmpeg process error:', error);
  });

  userRecordingProcess.on('exit', (code, signal) => {
    console.log(`🎤 [RECORDER] FFmpeg process exited with code ${code}, signal ${signal}`);
  });

  // Check more frequently than the chunk duration
  userChunkInterval = setInterval(async () => {
    console.log('[User Recording] Interval running...');
    try {
      const files = await fs.promises.readdir(tempDir);
      console.log('[User Recording] Files in tempDir:', files);
      const wavFiles = files.filter(f => f.endsWith('.mp3'));
      console.log('[User Recording] MP3 files found:', wavFiles);
      if (!wavFiles.length) return;

      // Sort files by chunk number
      wavFiles.sort((a, b) => {
        const numA = parseInt(a.match(/\d+/)[0]);
        const numB = parseInt(b.match(/\d+/)[0]);
        return numA - numB;
      });

      for (const fileName of wavFiles) {
        const filePath = path.join(tempDir, fileName);
        if (processedUserChunks.has(filePath)) {
          continue;
        }

        try {
          // Add a small delay to ensure file is fully written
          await new Promise(resolve => setTimeout(resolve, 500));
          await waitForFile(filePath, 2000);
          const stats = fs.statSync(filePath);
          if (stats.size > 0) {
            console.log(`[User Recording] Processing file: ${filePath}, size: ${stats.size} bytes`);
            console.log(`[User Recording] About to call handleAudioResponse for: ${filePath}`);
            try {
              const chunkIndex = parseInt(fileName.match(/\d+/)[0]);
              const chunkStartTime = recordingStartTime.getTime() + (chunkIndex * duration * 1000);
              await handleAudioResponse(filePath, 'user', { ...metadata, recording_start: chunkStartTime });
              console.log(`[User Recording] handleAudioResponse completed successfully`);
            } catch (error) {
              console.error(`[User Recording] handleAudioResponse failed:`, error);
            }
            fs.unlinkSync(filePath);
            console.log(`[User Recording] Successfully processed and deleted: ${filePath}`);
            processedUserChunks.add(filePath);
          }
        } catch (err) {
          console.error(`[User Recording] Error processing file ${filePath}:`, err.message);
        }
      }
    } catch (err) {
      console.error("[User Recording] Error reading user chunks directory:", err);
    }
  }, 2000); // Check every 2 seconds instead of chunk duration
}

// startProspectRecording function removed - will be reimplemented with native ScreenCaptureKit

async function stopRecording() {
  console.log('[Recording Control] Stopping all recordings...');
  
  // Stop user recording (microphone)
  if (userRecordingProcess) {
    userRecordingProcess.kill('SIGINT');
    userRecordingProcess = null;
  }
  if (userChunkInterval) {
    clearInterval(userChunkInterval);
    userChunkInterval = null;
  }

  // Stop prospect recording - will be reimplemented with native ScreenCaptureKit
  if (prospectChunkInterval) {
    clearInterval(prospectChunkInterval);
    prospectChunkInterval = null;
  }
  
  // Clear processed files tracking
  processedUserChunks.clear();
  processedProspectChunks.clear();
  
  // Clear the audio queue
  audioQueue.clear();
  
  // Clean up any remaining files
  const userTempDir = getWritableTempDir('chunks_user');
  const prospectTempDir = getWritableTempDir('chunks_prospect');
  
  [userTempDir, prospectTempDir].forEach(dir => {
    if (fs.existsSync(dir)) {
      fs.readdirSync(dir).forEach(file => {
        try {
          fs.unlinkSync(path.join(dir, file));
        } catch (err) {
          console.warn(`Could not delete file ${file} during cleanup:`, err.message);
        }
      });
    }
  });
  
  console.log('[Recording Control] ✅ All recordings stopped and cleaned up');
}

// Update cleanupTempFolders function
const cleanupTempFolders = () => {
  const folders = ['chunks_user', 'chunks_prospect', 'full_recordings'];
  
  folders.forEach(folderName => {
    const tempDir = getWritableTempDir(folderName);
    if (fs.existsSync(tempDir)) {
      try {
        const files = fs.readdirSync(tempDir);
        files.forEach(file => {
          try {
            fs.unlinkSync(path.join(tempDir, file));
            console.log(`[Cleanup] Deleted: ${file}`);
          } catch (err) {
            console.warn(`[Cleanup] Could not delete ${file}:`, err.message);
          }
        });
      } catch (err) {
        console.warn(`[Cleanup] Could not read ${tempDir}:`, err.message);
      }
    }
  });
};

process.on('unhandledRejection', (reason, promise) => {
  console.error('Unhandled Rejection at:', promise, 'reason:', reason);
});

// Store streaming callback for user audio
let userStreamingCallback = null;

/**
 * Set streaming callback for user audio
 * @param {Function} callback - Callback function(buffer, format)
 *   - buffer: Buffer containing raw audio data
 *   - format: Object { sampleRate, channels, bitDepth, isFloat }
 */
function setUserStreamingCallback(callback) {
  if (callback && typeof callback !== 'function') {
    throw new Error('[RECORDER] Streaming callback must be a function');
  }
  userStreamingCallback = callback;
  console.log(`🎤 [RECORDER] User streaming callback ${callback ? 'set' : 'cleared'}`);
}

// New full recording functions for post-meeting processing
async function startUserFullRecording({ metadata = {}, streamingCallback = null } = {}) {
  console.log('🎤 [RECORDER] === STARTING USER FULL RECORDING (MICROPHONE) ===');
  console.log('🎤 [RECORDER] Metadata:', JSON.stringify(metadata, null, 2));
  
  // Set streaming callback if provided
  if (streamingCallback) {
    setUserStreamingCallback(streamingCallback);
  }
  
  // Get the output directory - use user subfolder
  const baseDir = getWritableTempDir('full_recordings');
  
  // Ensure the directory exists
  if (!fs.existsSync(baseDir)) {
    fs.mkdirSync(baseDir, { recursive: true });
    console.log(`🎤 [RECORDER] Created recording directory: ${baseDir}`);
  } else {
    console.log(`🎤 [RECORDER] Using existing recording directory: ${baseDir}`);
  }
  
  // Optional: clean previous user files in base directory (disabled to avoid touching system files)
  // If needed, implement targeted cleanup based on filename prefix
  /* if (fs.existsSync(baseDir)) {
    try {
      const files = fs.readdirSync(baseDir).filter(f => f.startsWith('user-') && f.endsWith('.caf'));
      files.forEach(file => {
        try { fs.unlinkSync(path.join(baseDir, file)); console.log(`🎤 [RECORDER] Deleted leftover user file: ${file}`); } catch (err) { console.warn(`🎤 [RECORDER] Could not delete user file: ${file}`, err.message); }
      });
    } catch (err) {
      console.warn(`🎤 [RECORDER] Could not scan recording directory: ${err.message}`);
    }
  } */

  // Generate filename including start timestamp in milliseconds for alignment
  const sessionId = metadata.sessionId || 'unknown-session';
  const startMs = metadata.userStartMs || Date.now();
  const userAudioFile = path.join(baseDir, `user-${sessionId}-${startMs}.caf`);
  
  // Detect active microphone
  const micInfo = await detectActiveMicrophone();
  
  console.log(`🎤 [RECORDER] Starting full microphone recording with FFmpeg`);
  console.log(`🎤 [RECORDER] Using microphone: [${micInfo.index}] ${micInfo.name}`);
  console.log(`🎤 [RECORDER] Output directory: ${baseDir}`);
  console.log(`🎤 [RECORDER] Output file: ${userAudioFile}`);
  console.log(`🎤 [RECORDER] Streaming: ${userStreamingCallback ? 'enabled' : 'disabled'}`);
  
  try {
    // Use FFmpeg to record directly to CAF format (same as prospect audio)
    // CAF is uncompressed but we'll compress it to MP3 later
    // CRITICAL: Explicitly resample to 48kHz to match system audio and prevent speed issues
    // avfoundation may provide audio at 44.1kHz, so we force resample with proper timing
    const ffmpegArgs = [
      '-f', 'avfoundation',
      '-i', `:${micInfo.index}`, // Use detected microphone device
      '-af', `aresample=${AUDIO_SAMPLE_RATE}:async=1`, // Explicit resample to 48kHz with async correction for timing
      '-ac', AUDIO_CHANNELS,
      '-c:a', 'pcm_s16le', // PCM 16-bit (uncompressed for now, will compress later)
      '-f', 'caf', // Core Audio Format
      '-y', // Overwrite output file if exists
      userAudioFile
    ];
    
    console.log('🎤 [RECORDER] FFmpeg args:', ffmpegArgs.join(' '));
    
    // Reset actual start timestamp before spawn
    global.userActualStartMs = null;

    // Spawn FFmpeg process for full recording
    global.userFullRecordingProcess = spawn(getFfmpegPath(), ffmpegArgs);
    
    console.log('🎤 [RECORDER] FFmpeg process spawned for full recording, PID:', global.userFullRecordingProcess.pid);
    
    // If streaming is enabled, also spawn a separate FFmpeg process for streaming (raw PCM to stdout)
    let streamingProcess = null;
    if (userStreamingCallback) {
      console.log('🎤 [RECORDER] Starting streaming FFmpeg process...');
      
      // Format info for streaming callback
      const streamFormat = {
        sampleRate: parseInt(AUDIO_SAMPLE_RATE), // 48000
        channels: parseInt(AUDIO_CHANNELS), // 2
        bitDepth: 16, // pcm_s16le
        isFloat: false
      };
      
      // FFmpeg args for streaming (output raw PCM to stdout)
      const streamingArgs = [
        '-f', 'avfoundation',
        '-i', `:${micInfo.index}`,
        '-af', `aresample=${AUDIO_SAMPLE_RATE}:async=1`,
        '-ac', AUDIO_CHANNELS,
        '-c:a', 'pcm_s16le', // PCM 16-bit
        '-f', 's16le', // Raw PCM format
        '-' // Output to stdout
      ];
      
      streamingProcess = spawn(getFfmpegPath(), streamingArgs);
      
      // Handle streaming stdout data
      streamingProcess.stdout.on('data', (chunk) => {
        if (userStreamingCallback && chunk.length > 0) {
          try {
            // Convert to Buffer if needed
            const buffer = Buffer.isBuffer(chunk) ? chunk : Buffer.from(chunk);
            userStreamingCallback(buffer, streamFormat);
          } catch (error) {
            console.error('🎤 [RECORDER] Error in streaming callback:', error);
          }
        }
      });
      
      streamingProcess.stderr.on('data', (data) => {
        // Suppress FFmpeg stderr output for streaming process (it's verbose)
        // Only log errors
        const output = data.toString();
        if (output.includes('Error')) {
          console.error('🎤 [RECORDER] Streaming FFmpeg error:', output);
        }
      });
      
      streamingProcess.on('error', (error) => {
        console.error('🎤 [RECORDER] Streaming FFmpeg process error:', error);
      });
      
      streamingProcess.on('exit', (code, signal) => {
        console.log(`🎤 [RECORDER] Streaming FFmpeg process exited with code ${code}, signal ${signal}`);
      });
      
      // Store streaming process for cleanup
      global.userStreamingProcess = streamingProcess;
      
      console.log('🎤 [RECORDER] ✅ Streaming FFmpeg process started');
    }
    
    // Store listeners so we can remove them later
    const stderrHandler = (data) => {
      const output = data.toString();
      // Set actual start when we observe the first progress from FFmpeg
      if (!global.userActualStartMs) {
        if (output.includes('time=') || output.toLowerCase().includes('size=')) {
          global.userActualStartMs = Date.now();
          console.log(`🎤 [RECORDER] ✅ Detected user actualStartMs=${global.userActualStartMs}`);
        }
      }
      if (output.includes('Error')) {
        console.error('🎤 [RECORDER] FFmpeg Error detected:', output);
      } else if (output.includes('time=')) {
        // Log progress every few seconds (time= indicates progress)
        const timeMatch = output.match(/time=(\d+:\d+:\d+\.\d+)/);
        if (timeMatch) {
          console.log('🎤 [RECORDER] Recording progress:', timeMatch[1]);
        }
      }
    };
    
    const errorHandler = (error) => {
      console.error('🎤 [RECORDER] FFmpeg process error:', error);
      throw error;
    };
    
    const exitHandler = (code, signal) => {
      console.log(`🎤 [RECORDER] FFmpeg process exited with code ${code}, signal ${signal}`);
      // Code 255 can be normal when interrupted with SIGINT, code 0 is successful completion
      if (code === 0) {
        console.log('🎤 [RECORDER] ✅ User full recording completed successfully');
      } else if (code === 255 && signal === null) {
        // SIGINT interruption - this is expected when we stop the recording
        console.log('🎤 [RECORDER] ✅ User recording stopped (SIGINT)');
      } else {
        console.warn(`🎤 [RECORDER] ⚠️ User full recording exited with code ${code}, signal ${signal}`);
      }
      
      // Clean up listeners when process exits
      cleanupFFmpegListeners();
      
      // Clean up streaming process if it exists
      if (global.userStreamingProcess) {
        try {
          global.userStreamingProcess.kill('SIGINT');
          global.userStreamingProcess = null;
        } catch (err) {
          console.warn('🎤 [RECORDER] Error stopping streaming process:', err);
        }
      }
    };
    
    // Store handlers for cleanup
    global.userFullRecordingProcess._stderrHandler = stderrHandler;
    global.userFullRecordingProcess._errorHandler = errorHandler;
    global.userFullRecordingProcess._exitHandler = exitHandler;
    
    // Handle FFmpeg stderr output (FFmpeg logs to stderr)
    global.userFullRecordingProcess.stderr.on('data', stderrHandler);
    
    // Handle process errors
    global.userFullRecordingProcess.on('error', errorHandler);
    
    // Handle process exit
    global.userFullRecordingProcess.on('exit', exitHandler);
    
    // Store the file path for later retrieval
    global.userRecordingFile = userAudioFile;
    
    console.log('🎤 [RECORDER] ✅ User full recording started');
    
    return userAudioFile;
    
  } catch (error) {
    console.error('🎤 [RECORDER] ❌ Error starting microphone recording:', error);
    throw error;
  }
}

async function startProspectFullRecording({ metadata = {} }) {
  console.log('🎤 [RECORDER] === STARTING PROSPECT FULL RECORDING (SYSTEM AUDIO) ===');
  console.log('🎤 [RECORDER] Metadata:', JSON.stringify(metadata, null, 2));
  
  cleanupTempFolders();
  const tempDir = getWritableTempDir('full_recordings');
  
  // Ensure the directory exists
  if (!fs.existsSync(tempDir)) {
    fs.mkdirSync(tempDir, { recursive: true });
    console.log(`🎤 [RECORDER] Created temp directory: ${tempDir}`);
  }
  
  // Clean temp dir
  if (fs.existsSync(tempDir)) {
    try {
      const files = fs.readdirSync(tempDir);
      files.forEach(file => {
        try {
          fs.unlinkSync(path.join(tempDir, file));
          console.log(`🎤 [RECORDER] Deleted leftover file: ${file}`);
        } catch (err) {
          console.warn(`🎤 [RECORDER] Could not delete file: ${file}`, err.message);
        }
      });
    } catch (err) {
      console.warn(`🎤 [RECORDER] Could not read temp directory: ${err.message}`);
    }
  }

  console.log(`🎤 [RECORDER] Starting full system audio recording with ScreenCaptureKit + MediaRecorder`);
  console.log(`🎤 [RECORDER] Temp directory: ${tempDir}`);
  
  try {
    // Initialize ScreenCaptureKit
    const screenCapture = new ScreenAudioCapture();
    
    // Start system audio capture
    console.log('🎤 [RECORDER] Starting ScreenCaptureKit system audio capture...');
    const systemAudioStream = await screenCapture.startSystemAudioCapture();
    
    // Store the stream for cleanup later
    global.prospectAudioStream = systemAudioStream;
    global.screenCapture = screenCapture;
    
    console.log('🎤 [RECORDER] ✅ System audio capture started successfully');
    
    // Generate unique filename with timestamp
    const timestamp = new Date().toISOString().replace(/[:.]/g, '-');
    const prospectAudioFile = path.join(tempDir, `prospect-recording-${timestamp}.webm`);
    
    console.log('🎤 [RECORDER] Starting MediaRecorder for system audio...');
    console.log('🎤 [RECORDER] Will record to file:', prospectAudioFile);
    
    // Create MediaRecorder for system audio
    const mediaRecorder = new MediaRecorder(systemAudioStream, {
      mimeType: 'audio/webm;codecs=opus'
    });
    
    const audioChunks = [];
    
    // Handle data available event
    mediaRecorder.ondataavailable = (event) => {
      if (event.data.size > 0) {
        audioChunks.push(event.data);
        console.log('🎤 [RECORDER] Received audio chunk:', event.data.size, 'bytes');
      }
    };
    
    // Handle recording stop event
    mediaRecorder.onstop = async () => {
      try {
        console.log('🎤 [RECORDER] MediaRecorder stopped, processing audio chunks...');
        
        // Create blob from chunks
        const audioBlob = new Blob(audioChunks, { type: 'audio/webm;codecs=opus' });
        console.log('🎤 [RECORDER] Created audio blob:', audioBlob.size, 'bytes');
        
        // Convert blob to buffer and save to file
        const arrayBuffer = await audioBlob.arrayBuffer();
        const buffer = Buffer.from(arrayBuffer);
        
        fs.writeFileSync(prospectAudioFile, buffer);
        console.log('🎤 [RECORDER] ✅ System audio saved to:', prospectAudioFile);
        
        // Update global file path
        global.prospectRecordingFile = prospectAudioFile;
        
      } catch (error) {
        console.error('🎤 [RECORDER] ❌ Error saving system audio:', error);
      }
    };
    
    // Handle recording error
    mediaRecorder.onerror = (event) => {
      console.error('🎤 [RECORDER] ❌ MediaRecorder error:', event.error);
    };
    
    // Start recording
    mediaRecorder.start(1000); // Record in 1-second chunks
    console.log('🎤 [RECORDER] ✅ MediaRecorder started');
    
    // Store MediaRecorder for cleanup
    global.mediaRecorder = mediaRecorder;
    
    // Store the file path for later retrieval
    global.prospectRecordingFile = prospectAudioFile;
    
    return prospectAudioFile;
    
  } catch (error) {
    console.error('🎤 [RECORDER] ❌ Error starting system audio capture:', error);
    throw error;
  }
}

/**
 * Stop user audio streaming (separate from file recording)
 * Stops the streaming FFmpeg process and clears the callback
 */
async function stopUserStreaming() {
  console.log('[Recording Control] Stopping user audio streaming...');
  
  // Stop user streaming process if it exists
  if (global.userStreamingProcess) {
    try {
      console.log('[Recording Control] Stopping user streaming FFmpeg process...');
      global.userStreamingProcess.kill('SIGINT');
      await new Promise(resolve => setTimeout(resolve, 200));
      console.log('[Recording Control] ✅ User streaming process stopped');
    } catch (error) {
      console.error('[Recording Control] ❌ Error stopping user streaming process:', error);
    }
    global.userStreamingProcess = null;
  }
  
  // Clear streaming callback
  setUserStreamingCallback(null);
  
  console.log('[Recording Control] ✅ User audio streaming stopped');
}

async function stopFullRecording() {
  console.log('[Recording Control] Stopping full recordings...');
  
  // Only return user file - prospect file is handled by native module separately
  const userFile = global.userRecordingFile || null;
  const userActualStartMs = global.userActualStartMs || null;
  
  // Stop user recording (microphone via FFmpeg)
  if (global.userFullRecordingProcess) {
    try {
      console.log('[Recording Control] Stopping user FFmpeg process...');
      
      // Clean up listeners before killing to prevent delayed log output
      cleanupFFmpegListeners();
      
      // Send SIGINT to gracefully stop FFmpeg
      global.userFullRecordingProcess.kill('SIGINT');
      console.log('[Recording Control] ✅ User FFmpeg process stopped');
      
      // Wait a bit for FFmpeg to finish writing the file
      await new Promise(resolve => setTimeout(resolve, 500));
    } catch (error) {
      console.error('[Recording Control] ❌ Error stopping user FFmpeg process:', error);
      // Clean up listeners even if kill fails
      cleanupFFmpegListeners();
    }
    global.userFullRecordingProcess = null;
  }

  // Legacy MediaRecorder cleanup (if it exists)
  if (global.userMediaRecorder) {
    try {
      console.log('[Recording Control] Stopping user MediaRecorder (legacy)...');
      global.userMediaRecorder.stop();
      console.log('[Recording Control] ✅ User MediaRecorder stopped');
    } catch (error) {
      console.error('[Recording Control] ❌ Error stopping user MediaRecorder:', error);
    }
    global.userMediaRecorder = null;
  }

  // Legacy audio stream cleanup (if it exists)
  if (global.userAudioStream) {
    try {
      console.log('[Recording Control] Stopping user audio stream (legacy)...');
      global.userAudioStream.getTracks().forEach(track => track.stop());
      console.log('[Recording Control] ✅ User audio stream stopped');
    } catch (error) {
      console.error('[Recording Control] ❌ Error stopping user audio stream:', error);
    }
    global.userAudioStream = null;
  }

  // Stop prospect recording (system audio via ScreenCaptureKit)
  if (global.mediaRecorder) {
    try {
      console.log('[Recording Control] Stopping prospect MediaRecorder...');
      global.mediaRecorder.stop();
      console.log('[Recording Control] ✅ Prospect MediaRecorder stopped');
    } catch (error) {
      console.error('[Recording Control] ❌ Error stopping prospect MediaRecorder:', error);
    }
    global.mediaRecorder = null;
  }
  
  // Stop prospect audio stream
  if (global.prospectAudioStream) {
    try {
      console.log('[Recording Control] Stopping prospect audio stream...');
      global.prospectAudioStream.getTracks().forEach(track => track.stop());
      console.log('[Recording Control] ✅ Prospect audio stream stopped');
    } catch (error) {
      console.error('[Recording Control] ❌ Error stopping prospect audio stream:', error);
    }
    global.prospectAudioStream = null;
  }
  
  // Stop ScreenCaptureKit if it exists
  if (global.screenCapture) {
    try {
      console.log('[Recording Control] Stopping ScreenCaptureKit...');
      await global.screenCapture.stopSystemAudioCapture();
      console.log('[Recording Control] ✅ ScreenCaptureKit stopped');
    } catch (error) {
      console.error('[Recording Control] ❌ Error stopping ScreenCaptureKit:', error);
    }
    global.screenCapture = null;
  }
  
  // Clear global file paths
  global.userRecordingFile = null;
  global.prospectRecordingFile = null;
  
  console.log('[Recording Control] ✅ Full recordings stopped');
  console.log('[Recording Control] User file:', userFile);
  
  // Clear globals after returning
  const result = { userFile, actualStartMs: userActualStartMs };
  global.userActualStartMs = null;
  
  return result;
}

// Compress audio file from CAF to MP3 using FFmpeg
async function compressAudioFile({ inputPath, outputPath, format = 'mp3', bitrate = '128k', sampleRate = 48000 }) {
  return new Promise((resolve, reject) => {
    console.log('🎵 [COMPRESS] Starting compression:', { inputPath, outputPath, format, bitrate, sampleRate });
    
    if (!fs.existsSync(inputPath)) {
      return reject(new Error(`Input file not found: ${inputPath}`));
    }
    
    const ffmpegPath = getFfmpegPath();
    const ffmpegArgs = [
      '-i', inputPath,
      '-codec:a', 'libmp3lame',
      '-b:a', bitrate,
      '-ar', sampleRate.toString(),
      '-y', // Overwrite output file if exists
      outputPath
    ];
    
    console.log('🎵 [COMPRESS] FFmpeg args:', ffmpegArgs.join(' '));
    
    const compressProcess = spawn(ffmpegPath, ffmpegArgs);
    
    let stderrOutput = '';
    
    compressProcess.stderr.on('data', (data) => {
      const output = data.toString();
      stderrOutput += output;
      // Log progress if available
      if (output.includes('time=')) {
        const timeMatch = output.match(/time=(\d+:\d+:\d+\.\d+)/);
        if (timeMatch) {
          console.log('🎵 [COMPRESS] Progress:', timeMatch[1]);
        }
      }
    });
    
    compressProcess.on('error', (error) => {
      console.error('🎵 [COMPRESS] FFmpeg process error:', error);
      reject(error);
    });
    
    compressProcess.on('exit', (code, signal) => {
      if (code === 0) {
        if (fs.existsSync(outputPath)) {
          const stats = fs.statSync(outputPath);
          console.log('🎵 [COMPRESS] ✅ Compression successful:', {
            outputPath,
            size: stats.size,
            originalSize: fs.statSync(inputPath).size
          });
          resolve({ outputPath, size: stats.size });
        } else {
          reject(new Error('Compression completed but output file not found'));
        }
      } else {
        console.error('🎵 [COMPRESS] FFmpeg exited with code:', code, 'signal:', signal);
        console.error('🎵 [COMPRESS] FFmpeg stderr:', stderrOutput);
        reject(new Error(`FFmpeg compression failed with code ${code}: ${stderrOutput}`));
      }
    });
  });
}

// Updated module exports - includes both chunked and full recording functions
module.exports = {
  setUserStreamingCallback,
  stopUserStreaming,
  // Original chunked recording functions (for backward compatibility)
  startUserRecording, 
  stopRecording,
  
  // New full recording functions (for post-meeting processing)
  startUserFullRecording,
  startProspectFullRecording,
  stopFullRecording,
  prepareUserMicrophone,
  
  // Audio compression function
  compressAudioFile,
};
