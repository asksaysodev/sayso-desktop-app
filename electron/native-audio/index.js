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

  async requestScreenRecordingPermission() {
    await this.initialize();
    return nativeAudio.requestScreenRecordingPermission();
  }

  // Non-destructive screen-recording permission check (CGPreflight). Does NOT prompt or require initialize().
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
   * INTERNAL — start system audio capture. Not part of IAudioProvider; the public
   * entry point is startProspectStreaming(), which calls this with streamingOnly.
   * @param {Object} options - Capture options
   * @param {Function} options.streamingCallback - Optional callback for audio chunks
   * @returns {Promise<{ success: boolean, error?: string }>} success when SCK audio output is attached and capture started
   */
  async startSystemAudioCapture(options = {}) {
    // Lifecycle logs are deliberately unconditional: in packaged builds main's console is wrapped
    // into the file logger, and SCK start/settle timing is the evidence SAYSO-355 lacked.
    const startedAt = Date.now();
    console.log('🎤 [AUDIO MANAGER] SCK start requested');
    try {
      await this.initialize();
      const { streamingCallback, ...captureOptions } = options;
      if (streamingCallback) {
        this.setStreamingCallback(streamingCallback);
      }
      const nativeResult = await nativeAudio.startSystemAudioCapture(captureOptions);
      if (nativeResult) {
        console.log(`🎤 [AUDIO MANAGER] SCK start settled ok in ${Date.now() - startedAt}ms`);
        return { success: true };
      }
      return {
        success: false,
        error: 'System audio capture did not start (unexpected native result)',
      };
    } catch (err) {
      const message = err && err.message ? err.message : String(err);
      console.warn(`🎤 [AUDIO MANAGER] SCK start failed after ${Date.now() - startedAt}ms: ${message}`);
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
    const startedAt = Date.now();
    console.log('🎤 [AUDIO MANAGER] SCK stop requested');
    await this.initialize();

    this.setStreamingCallback(null);

    const result = await nativeAudio.stopSystemAudioCapture();
    console.log(`🎤 [AUDIO MANAGER] SCK stop settled in ${Date.now() - startedAt}ms (success=${result && result.success})`);
    return result;
  }

  async isSystemAudioCaptureActive() {
    await this.initialize();
    return nativeAudio.isSystemAudioCaptureActive();
  }

  /**
   * Register a callback for native lifecycle diagnostics (SAYSO-355). Events are short snake_case
   * strings, e.g. "sck_start_watchdog_fired gen=3 timeout_ms=10000"; events ending in `_failed`
   * should be escalated by the caller. Pass null to clear. No-op on native builds that predate
   * the method.
   * @param {((event: string) => void)|null} callback
   */
  setLifecycleEventCallback(callback) {
    if (nativeAudio.setLifecycleEventCallback) {
      nativeAudio.setLifecycleEventCallback(callback);
    }
  }

  /**
   * Start microphone capture for streaming
   * @param {Object} options - Capture options
   * @param {Function} options.streamingCallback - Optional callback for audio chunks
   * @returns {Promise<boolean|{ok: false, reason?: string}>} - `true` on success; on failure,
   *   { ok: false, reason } (SAYSO-347) or a bare `false` from a native build predating that change.
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
