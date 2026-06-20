const path = require('path');
const fs = require('fs');

// Try to load the native module with better error handling
let nativeAudio;
try {
  const modulePath = path.join(__dirname, 'build', 'Release', 'native_audio.node');

  if (!fs.existsSync(modulePath)) {
    throw new Error(`Native module not found at: ${modulePath}`);
  }

  nativeAudio = require(modulePath);
  console.log('🎤 [AUDIO MANAGER] Native module loaded successfully');
} catch (error) {
  console.error('🎤 [AUDIO MANAGER] Failed to load native module:', error.message);
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

  // Non-destructive screen-recording permission probe. Does NOT require initialize().
  // Resolves a diagnostic object: { granted, preflight, displays, errorCode, errorDomain, errorDescription }
  async checkSCKPermission() {
    return nativeAudio.checkSCKPermission();
  }

  // Legacy synchronous CGPreflight check (kept as fallback).
  checkScreenRecordingGranted() {
    return nativeAudio.checkScreenRecordingGranted();
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
   * @returns {Promise<{ success: boolean, error?: string }>} success when SCK audio output is attached and capture started
   */
  async startSystemAudioCapture(options = {}) {
    try {
      await this.initialize();
      const { streamingCallback, ...captureOptions } = options;
      if (streamingCallback) {
        this.setStreamingCallback(streamingCallback);
      }
      const nativeResult = await nativeAudio.startSystemAudioCapture(captureOptions);
      if (nativeResult === true) {
        return { success: true };
      }
      return {
        success: false,
        error: 'System audio capture did not start (unexpected native result)',
      };
    } catch (err) {
      const message = err && err.message ? err.message : String(err);
      return { success: false, error: message };
    }
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
    
    const started = await this.startSystemAudioCapture({ streamingOnly: true });
    if (!started.success) {
      throw new Error(started.error || '[AUDIO MANAGER] System audio capture failed to start');
    }
    return { success: true };
  }

  /**
   * Stops SCK system audio; resolves after native teardown completes (safe before next start).
   * @returns {Promise<{ success: boolean, filePath?: string|null, actualStartMs?: number|null, error?: string }>}
   */
  async stopSystemAudioCapture() {
    await this.initialize();

    this.setStreamingCallback(null);

    return await nativeAudio.stopSystemAudioCapture();
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
