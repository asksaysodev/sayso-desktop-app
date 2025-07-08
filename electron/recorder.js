const fs = require('fs');
const path = require('path');
const { spawn } = require('child_process');
const { app } = require('electron');
const audioQueue = require('./audioQueue');
const uploadService = require('./uploadService');

let userRecordingProcess = null;
let userChunkInterval = null;
let prospectRecordingProcess = null;
let prospectChunkInterval = null;
let processedUserChunks = new Set();
let processedProspectChunks = new Set();

// --- Configuration ---
// For avfoundation, use ':audio_index' format (e.g., ':0' for first audio device)
function getDeviceIndexByName(deviceName) {
  return new Promise((resolve, reject) => {
    const { exec } = require('child_process');
    exec('ffmpeg -f avfoundation -list_devices true -i ""', (err, stdout, stderr) => {
      const output = stderr.toString();
      const lines = output.split('\n');
      const foundDevices = [];
      lines.forEach(line => {
        const match = line.match(/\[(\d+)\] (.*)/);
        if (match) {
          foundDevices.push({ index: match[1], name: match[2] });
        }
      });

      // Log all found devices
      console.log('[Device List] AVFoundation input devices:');
      foundDevices.forEach(dev => console.log(`  [${dev.index}] ${dev.name}`));

      // Try to find the device by name (case-insensitive, partial match)
      const found = foundDevices.find(dev =>
        dev.name.toLowerCase().includes(deviceName.toLowerCase())
      );
      if (found) {
        return resolve(found.index); // Return just the index number
      }

      // If not found, suggest available devices
      const deviceNames = foundDevices.map(dev => dev.name).join(', ');
      return reject(
        new Error(
          `Device "${deviceName}" not found. Available devices: ${deviceNames}`
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
    USER_MIC_DEVICE_ID = await getDeviceIndexByName('MacBook Pro Microphone');
    PROSPECT_AUDIO_DEVICE_ID = await getDeviceIndexByName('blackhole');
    console.log('[Device Resolver] Successfully initialized devices:');
    console.log('[Device Resolver] User Mic:', USER_MIC_DEVICE_ID);
    console.log('[Device Resolver] Prospect Audio:', PROSPECT_AUDIO_DEVICE_ID);
  } catch (err) {
    console.error('[Device Resolver] Error initializing devices:', err.message);
    throw err;
  }
}

// Call initializeDevices when the module loads
initializeDevices().catch(console.error);


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
      console.log(`[${speaker} Recording] Global mainWindow exists:`, !!global.mainWindow);
      
      if (global.mainWindow) {
        const transcriptionData = {
          text: response.transcript.text,
          speaker: speaker,
          timestamp: response.timestamp || Date.now()
        };
        
        console.log(`[${speaker} Recording] Sending transcription data:`, transcriptionData);
        global.mainWindow.webContents.send('transcription-data', transcriptionData);
        console.log(`[${speaker} Recording] Sent transcription to renderer:`, response.transcript.text.substring(0, 50) + '...');
      } else {
        console.error(`[${speaker} Recording] Global mainWindow is not set!`);
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
  cleanupTempFolders();
  const tempDir = getWritableTempDir('chunks_user');
  
  // Ensure the directory exists
  if (!fs.existsSync(tempDir)) {
    fs.mkdirSync(tempDir, { recursive: true });
    console.log(`[User Recording] Created temp directory: ${tempDir}`);
  }
  
  processedUserChunks.clear();

  // Clean temp dir (only if it exists and has files)
  if (fs.existsSync(tempDir)) {
    try {
      const files = fs.readdirSync(tempDir);
      files.forEach(file => {
        try {
          fs.unlinkSync(path.join(tempDir, file));
          console.log(`[User Recording] Deleted leftover chunk file on startup: ${file}`);
        } catch (err) {
          console.warn(`Could not delete user chunk file: ${file}`, err.message);
        }
      });
    } catch (err) {
      console.warn(`Could not read user temp directory: ${err.message}`);
    }
  }

  console.log(`[User Recording] Starting with device ID: ${USER_MIC_DEVICE_ID}`);
  console.log(`[User Recording] Chunk duration: ${duration} seconds`);
  console.log(`[User Recording] Temp directory: ${tempDir}`);
  
  const recordingStartTime = new Date(); // When you start the recording
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
  
  userRecordingProcess = spawn('ffmpeg', ffmpegArgs);

  userRecordingProcess.stderr.on('data', (data) => {
    const output = data.toString();
    if (output.includes('Error')) {
      console.error('[User FFmpeg] Error detected in FFmpeg output');
    }
  });
  userRecordingProcess.on('error', (err) => {
    console.error('[User FFmpeg ERROR]: Failed to start ffmpeg process:', err);
  });
  userRecordingProcess.on('exit', (code, signal) => {
    if (userChunkInterval) clearInterval(userChunkInterval);
    userChunkInterval = null;
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
  
  prospectRecordingProcess = spawn('ffmpeg', ffmpegArgs);

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

module.exports = { startUserRecording, startProspectRecording, stopRecording };

