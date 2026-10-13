const path = require('path');
const fs = require('fs');

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
    this.streamingCallback = null;
  }

  async initialize() {
    if (this.isInitialized) return;
    
    try {
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

  checkScreenRecordingGranted() {
    return nativeAudio.checkScreenRecordingGranted();
  }

  async checkAudioCaptureCapability() {
    if (typeof nativeAudio.checkAudioCaptureCapability !== 'function') return null;
    await this.initialize();
    return nativeAudio.checkAudioCaptureCapability();
  }

  setStreamingCallback(callback) {
    if (callback && typeof callback !== 'function') {
      throw new Error('[AUDIO MANAGER] Streaming callback must be a function');
    }
    this.streamingCallback = callback;
    
    if (nativeAudio.setStreamingCallback) {
      nativeAudio.setStreamingCallback(callback);
    }
  }

  async startSystemAudioCapture(options = {}) {
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

  async startProspectStreaming({ streamingCallback } = {}) {
    await this.initialize();
    
    if (!streamingCallback || typeof streamingCallback !== 'function') {
      throw new Error('[AUDIO MANAGER] streamingCallback is required for startProspectStreaming');
    }
    
    this.setStreamingCallback(streamingCallback);
    
    const started = await this.startSystemAudioCapture({ streamingOnly: true });
    if (!started.success) {
      throw new Error(started.error || '[AUDIO MANAGER] System audio capture failed to start');
    }
    return { success: true };
  }

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

  setLifecycleEventCallback(callback) {
    if (nativeAudio.setLifecycleEventCallback) {
      nativeAudio.setLifecycleEventCallback(callback);
    }
  }

  async startMicrophoneCapture(options = {}) {
    await this.initialize();
    
    const { streamingCallback, ...captureOptions } = options;
    
    if (streamingCallback) {
      if (nativeAudio.setMicrophoneStreamingCallback) {
        nativeAudio.setMicrophoneStreamingCallback(streamingCallback);
      } else {
        this.setStreamingCallback(streamingCallback);
      }
    }
    
    return nativeAudio.startMicrophoneCapture(captureOptions);
  }

  async stopMicrophoneCapture() {
    await this.initialize();

    if (nativeAudio.setMicrophoneStreamingCallback) {
      nativeAudio.setMicrophoneStreamingCallback(null);
    } else {
      this.setStreamingCallback(null);
    }

    return nativeAudio.stopMicrophoneCapture();
  }

  async isMicrophoneCaptureActive() {
    await this.initialize();
    return nativeAudio.isMicrophoneCaptureActive();
  }

  async isMicRouteRecovering() {
    await this.initialize();
    if (typeof nativeAudio.isMicRouteRecovering !== 'function') return false;
    return nativeAudio.isMicRouteRecovering();
  }

  async getMicInputDiagnostics() {
    if (typeof nativeAudio.getMicInputDiagnostics !== 'function') return null;
    return nativeAudio.getMicInputDiagnostics();
  }
}

module.exports = new AudioDeviceManager();
