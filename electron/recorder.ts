const nativeAudio = require('./native-audio');
import * as Sentry from '@sentry/electron/main';

// Store streaming callback for user audio
let userStreamingCallback = null;

/**
 * Set streaming callback for user audio
 * @param {Function} callback - Callback function(buffer, format)
 *   - buffer: Buffer containing raw audio data
 *   - format: Object { sampleRate, channels, bitDepth, isFloat }
 */
function setUserStreamingCallback(callback: ((audioData: Buffer, sampleRate: number) => void) | null) {
  if (callback && typeof callback !== 'function') {
    throw new Error('[RECORDER] Streaming callback must be a function');
  }
  userStreamingCallback = callback;
}

/**
 * Start user microphone streaming (for real-time processing like Cue - no file saving)
 * Uses native AVAudioEngine for microphone capture
 * @param {Object} options - Streaming options
 * @param {Function} options.streamingCallback - Callback function(buffer, format)
 * @returns {Promise<Object>} { success: boolean }
 */
async function startUserStreaming({ streamingCallback }: { streamingCallback: (audioData: Buffer, sampleRate: number) => void }) {
  if (!streamingCallback || typeof streamingCallback !== 'function') {
    throw new Error('[RECORDER] streamingCallback is required for startUserStreaming');
  }
  
  // Set streaming callback (both local and native module)
  setUserStreamingCallback(streamingCallback);
  
  try {
    // Use native module for microphone capture
    const success = await nativeAudio.startMicrophoneCapture({ streamingCallback });
    
    if (!success) {
      throw new Error('Failed to start microphone capture');
    }
      
      return { success: true };
    
  } catch (error: any) {
    console.error('🎤 [RECORDER] ❌ Error starting user streaming:', error);
    Sentry.captureException(error);
    // Clear callback on error
    setUserStreamingCallback(null);
    throw error;
  }
}

/**
 * Stop user microphone streaming
 * Stops the native AVAudioEngine microphone capture
 * @returns {Promise<void>}
 */
async function stopUserStreaming() {
  console.log('[Recording Control] Stopping user audio streaming...');
  
  // Stop microphone capture using native module
  try {
    await nativeAudio.stopMicrophoneCapture();
    console.log('[Recording Control] ✅ User microphone capture stopped');
    } catch (error: any) {
    console.error('[Recording Control] ❌ Error stopping microphone capture:', error);
    Sentry.captureException(error);
  }
  
  // Clear streaming callback
  setUserStreamingCallback(null);
  console.log('[Recording Control] ✅ User streaming callback cleared');
}

module.exports = {
  setUserStreamingCallback,
  stopUserStreaming,
  startUserStreaming,
};
