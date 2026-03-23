const path = require('path');
const fs = require('fs');

// Try to load the native module with better error handling
let nativeAudio;
try {
  console.log('🎤 [AUDIO MANAGER] Starting native module loader...');
  console.log('🎤 [AUDIO MANAGER] __dirname:', __dirname);
  
  const modulePath = path.join(__dirname, 'build', 'Release', 'native_audio.node');
  console.log('🎤 [AUDIO MANAGER] Attempting to load native module from:', modulePath);
  
  // Check if directory exists
  const dirPath = path.dirname(modulePath);
  console.log('🎤 [AUDIO MANAGER] Checking directory:', dirPath);
  console.log('🎤 [AUDIO MANAGER] Directory exists:', fs.existsSync(dirPath));
  
  if (fs.existsSync(dirPath)) {
    const files = fs.readdirSync(dirPath);
    console.log('🎤 [AUDIO MANAGER] Files in directory:', files);
  }
  
  if (!fs.existsSync(modulePath)) {
    throw new Error(`Native module not found at: ${modulePath}`);
  }
  
  console.log('🎤 [AUDIO MANAGER] Module file exists, attempting to require...');
  nativeAudio = require(modulePath);
  console.log('🎤 [AUDIO MANAGER] Native module loaded successfully');
  console.log('🎤 [AUDIO MANAGER] Native module exports:', Object.keys(nativeAudio));
} catch (error) {
  console.error('🎤 [AUDIO MANAGER] Failed to load native module');
  console.error('🎤 [AUDIO MANAGER] Error message:', error.message);
  console.error('🎤 [AUDIO MANAGER] Error code:', error.code);
  console.error('🎤 [AUDIO MANAGER] Error stack:', error.stack);
  console.error('🎤 [AUDIO MANAGER] Full error:', error);
  throw error;
}

class AudioDeviceManager {
  constructor() {
    this.isInitialized = false;
    this.streamingCallback = null; // Callback for streaming audio chunks
  }

  async initialize() {
    if (this.isInitialized) return;
    
    try {
      // Initialize the native module
      await nativeAudio.initialize();
      this.isInitialized = true;
    } catch (error) {
      console.error('🎤 [AUDIO MANAGER] Failed to initialize native audio module:', error);
      throw error;
    }
  }

  async listOutputDevices() {
    await this.initialize();
    return nativeAudio.listOutputDevices();
  }

  async createMultiOutputDevice(name, subDevices) {
    await this.initialize();
    return nativeAudio.createMultiOutputDevice(name, subDevices);
  }

  async deleteMultiOutputDevice(deviceId) {
    await this.initialize();
    return nativeAudio.deleteMultiOutputDevice(deviceId);
  }

  async requestScreenRecordingPermission() {
    await this.initialize();
    return nativeAudio.requestScreenRecordingPermission();
  }

  /**
   * Set streaming callback for real-time audio chunks
   * @param {Function} callback - Callback function(buffer, format)
   *   - buffer: Buffer containing raw audio data
   *   - format: Object { sampleRate, channels, bitDepth, isFloat }
   */
  setStreamingCallback(callback) {
    if (callback && typeof callback !== 'function') {
      throw new Error('[AUDIO MANAGER] Streaming callback must be a function');
    }
    this.streamingCallback = callback;
    
    // Also set in native module if it supports it
    // TODO: Implement native module method to set callback
    // For now, this stores the callback for when native code is updated
    if (nativeAudio.setStreamingCallback) {
      nativeAudio.setStreamingCallback(callback);
    }
  }

  /**
   * Start system audio capture with optional streaming callback
   * @param {Object} options - Capture options
   * @param {Function} options.streamingCallback - Optional callback for audio chunks
   * @returns {Promise<Object>} - Result object with success and filePath
   */
  async startSystemAudioCapture(options = {}) {
    await this.initialize();
    
    // Extract streaming callback from options if provided
    const { streamingCallback, ...captureOptions } = options;
    
    // Set streaming callback if provided
    if (streamingCallback) {
      this.setStreamingCallback(streamingCallback);
    }
    
    // Start capture with remaining options
    return nativeAudio.startSystemAudioCapture(captureOptions);
  }

  /**
   * Start prospect audio streaming (streaming only, no file saving)
   * @param {Function} streamingCallback - Required callback for audio chunks
   * @returns {Promise<Object>} - Result object with success
   */
  async startProspectStreaming({ streamingCallback } = {}) {
    await this.initialize();
    
    if (!streamingCallback || typeof streamingCallback !== 'function') {
      throw new Error('[AUDIO MANAGER] streamingCallback is required for startProspectStreaming');
    }
    
    // Set streaming callback
    this.setStreamingCallback(streamingCallback);
    
    // Start capture with streamingOnly flag to skip file creation
    const result = await nativeAudio.startSystemAudioCapture({ streamingOnly: true });
    
    return { success: true };
  }

  async stopSystemAudioCapture() {
    await this.initialize();
    
    // Clear streaming callback when stopping
    this.setStreamingCallback(null);
    
    return nativeAudio.stopSystemAudioCapture();
  }

  async isSystemAudioCaptureActive() {
    await this.initialize();
    return nativeAudio.isSystemAudioCaptureActive();
  }

  /**
   * Start microphone capture for streaming
   * @param {Object} options - Capture options
   * @param {Function} options.streamingCallback - Optional callback for audio chunks
   * @returns {Promise<boolean>} - Success status
   */
  async startMicrophoneCapture(options = {}) {
    await this.initialize();
    
    const { streamingCallback, ...captureOptions } = options;
    
    // Use microphone-specific callback method
    if (streamingCallback) {
      if (nativeAudio.setMicrophoneStreamingCallback) {
        nativeAudio.setMicrophoneStreamingCallback(streamingCallback);
      } else {
        // Fallback to regular callback if method doesn't exist
        this.setStreamingCallback(streamingCallback);
      }
    }
    
    return nativeAudio.startMicrophoneCapture(captureOptions);
  }

  /**
   * Stop microphone capture
   * @returns {Promise<boolean>} - Success status
   */
  async stopMicrophoneCapture() {
    await this.initialize();

    // Clear *microphone* callback only. setStreamingCallback(null) would also clear the
    // native SCK/prospect path and break prospect streaming if user mic stops first.
    if (nativeAudio.setMicrophoneStreamingCallback) {
      nativeAudio.setMicrophoneStreamingCallback(null);
    } else {
      // Legacy native without mic-specific API (used setStreamingCallback for mic)
      this.setStreamingCallback(null);
    }

    return nativeAudio.stopMicrophoneCapture();
  }

  /**
   * Check if microphone capture is active
   * @returns {Promise<boolean>} - Active status
   */
  async isMicrophoneCaptureActive() {
    await this.initialize();
    return nativeAudio.isMicrophoneCaptureActive();
  }
}

module.exports = new AudioDeviceManager();
