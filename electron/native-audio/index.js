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
  }

  async initialize() {
    if (this.isInitialized) return;
    
    try {
      // Initialize the native module
      await nativeAudio.initialize();
      this.isInitialized = true;
      console.log('🎤 [AUDIO MANAGER] Native audio module initialized');
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

  async startSystemAudioCapture(options = {}) {
    await this.initialize();
    return nativeAudio.startSystemAudioCapture(options);
  }

  async stopSystemAudioCapture() {
    await this.initialize();
    return nativeAudio.stopSystemAudioCapture();
  }

  async isSystemAudioCaptureActive() {
    await this.initialize();
    return nativeAudio.isSystemAudioCaptureActive();
  }
}

module.exports = new AudioDeviceManager();
