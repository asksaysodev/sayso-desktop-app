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

let userRecordingProcess = null;
let userChunkInterval = null;
let prospectRecordingProcess = null;
let prospectChunkInterval = null;
let processedUserChunks = new Set();
let processedProspectChunks = new Set();

// --- Configuration ---
// For avfoundation, use ':audio_index' format (e.g., ':0' for first audio device)
function getDeviceIndexByName(targetDevices) {
  return new Promise((resolve, reject) => {
    const { exec } = require('child_process');
    const ffmpegPath = getFfmpegPath();
    console.log(`[DEBUG] Running FFmpeg command: ${ffmpegPath} -f avfoundation -list_devices true -i ""`);
    
    exec(`"${ffmpegPath}" -f avfoundation -list_devices true -i ""`, (err, stdout, stderr) => {
      console.log(`[DEBUG] FFmpeg stdout:`, stdout);
      console.log(`[DEBUG] FFmpeg stderr:`, stderr);
      console.log(`[DEBUG] FFmpeg error:`, err);
      
      const output = stderr.toString();
      console.log(`[DEBUG] Parsing stderr output:`, output);
      
      const lines = output.split('\n');
      console.log(`[DEBUG] Split lines:`, lines);
      
      // Separate video and audio devices
      const videoDevices = [];
      const audioDevices = [];
      let currentSection = null;
      
      lines.forEach((line, index) => {
        console.log(`[DEBUG] Processing line ${index}:`, line);
        
        // Detect which section we're in
        if (line.includes('AVFoundation video devices:')) {
          currentSection = 'video';
          return;
        } else if (line.includes('AVFoundation audio devices:')) {
          currentSection = 'audio';
          return;
        }
        
        // Look for lines that contain [number] followed by a device name
        const match = line.match(/\[(\d+)\] (.+)$/);
        if (match && currentSection) {
          console.log(`[DEBUG] Found device match:`, match);
          const device = { index: match[1], name: match[2] };
          
          if (currentSection === 'video') {
            videoDevices.push(device);
          } else if (currentSection === 'audio') {
            audioDevices.push(device);
          }
        }
      });

      console.log(`[DEBUG] Video devices:`, videoDevices);
      console.log(`[DEBUG] Audio devices:`, audioDevices);
      
      // Log all found devices
      console.log('[Device List] AVFoundation video devices:');
      videoDevices.forEach(dev => console.log(`  [${dev.index}] ${dev.name}`));
      console.log('[Device List] AVFoundation audio devices:');
      audioDevices.forEach(dev => console.log(`  [${dev.index}] ${dev.name}`));

      // Ensure targetDevices is an array
      const deviceArray = Array.isArray(targetDevices) ? targetDevices : [targetDevices];
      
      // Try to find the device by name in order (case-insensitive, partial match)
      // ONLY search in audio devices, not video devices
      for (const deviceName of deviceArray) {
        const found = audioDevices.find(dev =>
          dev.name.toLowerCase().includes(deviceName.toLowerCase())
        );
        if (found) {
          console.log(`[Device Resolver] Found device "${deviceName}" at index ${found.index}`);
          return resolve(found.index); // Return just the index number
        }
      }

      // If none found, suggest available audio devices only
      const audioDeviceNames = audioDevices.map(dev => dev.name).join(', ');
      const targetDeviceNames = deviceArray.join(', ');
      return reject(
        new Error(
          `None of the target devices [${targetDeviceNames}] were found. Available audio devices: ${audioDeviceNames}`
        )
      );
    });
  });
}

// Initialize device IDs
let USER_MIC_DEVICE_ID = null;
let PROSPECT_AUDIO_DEVICE_ID = null;

// Initialize devices
async function initializeDevices() {
  try {
    USER_MIC_DEVICE_ID = await getDeviceIndexByName(['macbook', 'mac','macbook pro', 'macbook air', 'imac', 'microphone', 'mic']);
    PROSPECT_AUDIO_DEVICE_ID = await getDeviceIndexByName(['sayso speaker', 'sayso', 'blackhole']);
    console.log('[Device Resolver] Successfully initialized devices:');
    console.log('[Device Resolver] User Mic:', USER_MIC_DEVICE_ID);
    console.log('[Device Resolver] Prospect Audio:', PROSPECT_AUDIO_DEVICE_ID);
  } catch (err) {
    console.error('[Device Resolver] Error initializing devices:', err.message);
    throw err;
  }
}

// Call initializeDevices when the module loads


// const USER_MIC_DEVICE_ID = 'none:"1"';  // MacBook Pro Microphone (index 1)
// const PROSPECT_AUDIO_DEVICE_ID = 'none:"0"';  // Multi-Output Device for system audio
const AUDIO_SAMPLE_RATE = '44100';  // Changed from 16000 to 44100 (44.1kHz)
const AUDIO_CHANNELS = '1'; // Mono
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

function startUserRecording({ duration = 8, onChunk, metadata = {} }) {
  console.log('🎤 [RECORDER] === STARTING USER RECORDING ===');
  console.log('🎤 [RECORDER] Device ID:', USER_MIC_DEVICE_ID);
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

  console.log(`🎤 [RECORDER] Starting FFmpeg with device ID: ${USER_MIC_DEVICE_ID}`);
  console.log(`🎤 [RECORDER] Chunk duration: ${duration} seconds`);
  console.log(`🎤 [RECORDER] Temp directory: ${tempDir}`);
  
  const recordingStartTime = new Date();
  const ffmpegArgs = [
    '-f', 'avfoundation',
    '-i', `:${USER_MIC_DEVICE_ID}`,
    '-ar', AUDIO_SAMPLE_RATE,
    '-ac', AUDIO_CHANNELS,
    '-c:a', AUDIO_COMPRESSION,
    '-b:a', AUDIO_BITRATE,
    '-af', 'aresample=44100',
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

  let lastProcessedChunk = -1;

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

function startProspectRecording({ duration = 8, onChunk, metadata = {} }) {
  cleanupTempFolders();
  const tempDir = getWritableTempDir('chunks_prospect');
  
  // Ensure the directory exists
  if (!fs.existsSync(tempDir)) {
    fs.mkdirSync(tempDir, { recursive: true });
    console.log(`[Prospect Recording] Created temp directory: ${tempDir}`);
  }
  
  processedProspectChunks.clear();

  // Clean temp dir (only if it exists and has files)
  if (fs.existsSync(tempDir)) {
    try {
      const files = fs.readdirSync(tempDir);
      files.forEach(file => {
        try {
          fs.unlinkSync(path.join(tempDir, file));
          console.log(`[Prospect Recording] Deleted leftover chunk file on startup: ${file}`);
        } catch (err) {
          console.warn(`Could not delete prospect chunk file: ${file}`, err.message);
        }
      });
    } catch (err) {
      console.warn(`Could not read prospect temp directory: ${err.message}`);
    }
  }

  console.log(`[Prospect Recording] Starting with device ID: ${PROSPECT_AUDIO_DEVICE_ID}`);
  console.log(`[Prospect Recording] Chunk duration: ${duration} seconds`);
  console.log(`[Prospect Recording] Temp directory: ${tempDir}`);
  
  const recordingStartTime = new Date(); // When you start the recording
  const ffmpegArgs = [
    '-f', 'avfoundation',
    '-i', `:${PROSPECT_AUDIO_DEVICE_ID}`,
    '-ar', AUDIO_SAMPLE_RATE,
    '-ac', AUDIO_CHANNELS,
    '-c:a', AUDIO_COMPRESSION,
    '-b:a', AUDIO_BITRATE,
    '-af', 'aresample=44100',
    '-flush_packets', '1',
    '-f', 'segment',
    '-segment_time', duration.toString(),
    '-segment_time_delta', '1',
    '-segment_format', 'mp3',
    '-segment_list_size', '0',
    '-segment_wrap', '0',
    `${tempDir}/prospect-chunk-%03d.mp3`,
  ];
  
  prospectRecordingProcess = spawn(getFfmpegPath(), ffmpegArgs);

  prospectRecordingProcess.stderr.on('data', (data) => {
    const output = data.toString();
    if (output.includes('Error')) {
      console.error('[Prospect FFmpeg] Error detected in FFmpeg output');
    }
  });
  prospectRecordingProcess.on('error', (err) => {
    console.error('[Prospect FFmpeg ERROR]: Failed to start ffmpeg process:', err);
  });
  prospectRecordingProcess.on('exit', (code, signal) => {
    if (prospectChunkInterval) clearInterval(prospectChunkInterval);
    prospectChunkInterval = null;
  });

  // Check more frequently than the chunk duration
  prospectChunkInterval = setInterval(async () => {
    console.log('[Prospect Recording] Interval running...');
    try {
      const files = await fs.promises.readdir(tempDir);
      const wavFiles = files.filter(f => f.endsWith('.mp3'));
      if (!wavFiles.length) return;

      // Sort files by chunk number
      wavFiles.sort((a, b) => {
        const numA = parseInt(a.match(/\d+/)[0]);
        const numB = parseInt(b.match(/\d+/)[0]);
        return numA - numB;
      });

      for (const fileName of wavFiles) {
        const filePath = path.join(tempDir, fileName);
        if (processedProspectChunks.has(filePath)) {
          continue;
        }

        try {
          // Add a small delay to ensure file is fully written
          await new Promise(resolve => setTimeout(resolve, 500));
          await waitForFile(filePath, 2000);
          const stats = fs.statSync(filePath);
          if (stats.size > 0) {
            console.log(`[Prospect Recording] Processing file: ${filePath}, size: ${stats.size} bytes`);
            console.log(`[Prospect Recording] About to call handleAudioResponse for: ${filePath}`);
            try {
              const chunkIndex = parseInt(fileName.match(/\d+/)[0]);
              const chunkStartTime = recordingStartTime.getTime() + (chunkIndex * duration * 1000);
              await handleAudioResponse(filePath, 'prospect', { ...metadata, recording_start: chunkStartTime });
              console.log(`[Prospect Recording] handleAudioResponse completed successfully`);
            } catch (error) {
              console.error(`[Prospect Recording] handleAudioResponse failed:`, error);
            }
            fs.unlinkSync(filePath);
            console.log(`[Prospect Recording] Successfully processed and deleted: ${filePath}`);
            processedProspectChunks.add(filePath);
          }
        } catch (err) {
          console.error(`[Prospect Recording] Error processing file ${filePath}:`, err.message);
        }
      }
    } catch (err) {
      console.error("[Prospect Recording] Error reading prospect chunks directory:", err);
    }
  }, 2000); // Check every 2 seconds instead of chunk duration
}

function stopRecording() {
  console.log('[Recording Control] Stopping all recordings...');
  
  if (userRecordingProcess) {
    userRecordingProcess.kill('SIGINT');
    userRecordingProcess = null;
  }
  if (userChunkInterval) {
    clearInterval(userChunkInterval);
    userChunkInterval = null;
  }

  if (prospectRecordingProcess) {
    prospectRecordingProcess.kill('SIGINT');
    prospectRecordingProcess = null;
  }
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
  const userTempDir = path.join(__dirname, 'chunks_user');
  const prospectTempDir = path.join(__dirname, 'chunks_prospect');
  
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
}

// Update cleanupTempFolders function
const cleanupTempFolders = () => {
  const folders = ['chunks_user', 'chunks_prospect'];
  
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

// Place this function near the top of the file
function getSwitchAudioSourcePath() {
  // Try extraResources path first
  let binPath = path.join(process.resourcesPath, 'bin', 'SwitchAudioSource');
  if (fs.existsSync(binPath)) return binPath;

  // Fallback to asarUnpack path
  binPath = path.join(process.resourcesPath, 'app.asar.unpacked', 'electron', 'bin', 'SwitchAudioSource');
  if (fs.existsSync(binPath)) return binPath;

  // Fallback to dev path
  binPath = path.join(__dirname, 'bin', 'SwitchAudioSource');
  return binPath;
}

class AudioDeviceManager {
  constructor() {
    this.previousDevice = null;
    this.binPath = getSwitchAudioSourcePath(); // Add this line!
    console.log('🎤 [AUDIO DEVICE MANAGER] Initialized');
    console.log('🎤 [AUDIO DEVICE MANAGER] SwitchAudioSource path:', this.binPath);
  }

  // Get current output device
  async getCurrentOutputDevice() {
    return new Promise((resolve, reject) => {
      exec(`"${this.binPath}" -t output -c`, (err, stdout, stderr) => {
        if (err) {
          console.error('[AudioDeviceManager] ❌ Error getting current output device:', err);
          return reject(err);
        }
        const deviceName = stdout.toString().trim();
        console.log('[AudioDeviceManager] ✅ Current output device:', deviceName);
        resolve(deviceName);
      });
    });
  }

  // Get list of available audio output devices
  async getAvailableOutputDevices() {
    return new Promise((resolve, reject) => {
      exec(`"${this.binPath}" -t output -a`, (err, stdout, stderr) => {
        if (err) {
          console.error('[AudioDeviceManager] ❌ Error getting available output devices:', err);
          return reject(err);
        }
        const devices = stdout.toString().trim().split('\n');
        console.log('[AudioDeviceManager] ✅ Available output devices:', devices);
        resolve(devices);
      });
    });
  }

  // Switch to Sayso Speaker device
  async switchToSaysoSpeaker() {
    console.log('🎤 [AUDIO DEVICE MANAGER] === SWITCHING TO SAYSO SPEAKER ===');
    
    try {
      // Get current device first
      console.log('🎤 [AUDIO DEVICE MANAGER] Step 1: Getting current output device...');
      this.previousDevice = await this.getCurrentOutputDevice();
      console.log('🎤 [AUDIO DEVICE MANAGER] Current device:', this.previousDevice);
      
      // Get available devices
      console.log('🎤 [AUDIO DEVICE MANAGER] Step 2: Getting available devices...');
      const devices = await this.getAvailableOutputDevices();
      console.log('🎤 [AUDIO DEVICE MANAGER] Available devices:', devices);
      
      // Find Sayso Speaker
      const saysoDevice = devices.find(device => 
        device.toLowerCase().includes('sayso speaker')
      );
      
      if (!saysoDevice) {
        console.error('🎤 [AUDIO DEVICE MANAGER] ❌ Sayso Speaker not found in available devices');
        throw new Error('Sayso Speaker device not found');
      }
      
      console.log('🎤 [AUDIO DEVICE MANAGER] Found Sayso Speaker:', saysoDevice);
      
      // Switch to Sayso Speaker
      console.log('🎤 [AUDIO DEVICE MANAGER] Step 3: Switching to Sayso Speaker...');
      const switchPath = getSwitchAudioSourcePath();
      console.log('🎤 [AUDIO DEVICE MANAGER] SwitchAudioSource path:', switchPath);
      
      return new Promise((resolve, reject) => {
        exec(`"${this.binPath}" -t output -s "${saysoDevice}"`, (err, stdout, stderr) => {
          if (err) {
            console.error('[AudioDeviceManager] ❌ Failed to switch audio device:', err);
            reject(err);
          } else {
            console.log('[AudioDeviceManager] ✅ Successfully switched to Sayso Speaker');
            resolve();
          }
        });
      });
    } catch (error) {
      console.error('🎤 [AUDIO DEVICE MANAGER] ❌ Error switching to Sayso Speaker:', error);
      throw error;
    }
  }

  // Restore previous device
  async restorePreviousDevice() {
    if (!this.previousDevice) {
      console.log('[AudioDeviceManager] No previous device to restore');
      return;
    }

    try {
      console.log('[AudioDeviceManager] Restoring to previous device:', this.previousDevice);

      return new Promise((resolve, reject) => {
        exec(`"${this.binPath}" -t output -s "${this.previousDevice}"`, (err, stdout, stderr) => {
          if (err) {
            console.error('[AudioDeviceManager] ❌ Failed to restore audio device:', err);
            reject(err);
          } else {
            console.log('[AudioDeviceManager] ✅ Successfully restored previous audio device');
            resolve();
          }
        });
      });
    } catch (error) {
      console.error('[AudioDeviceManager] ❌ Error restoring audio device:', error);
      throw error;
    }
  }
}

// Create a singleton instance
const audioDeviceManager = new AudioDeviceManager();

// Update the module exports to include the device manager
module.exports = { 
  startUserRecording, 
  startProspectRecording, 
  stopRecording,
  initializeDevices, // Add this
  audioDeviceManager 
};
