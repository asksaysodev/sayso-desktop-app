import type { IAudioProvider, StreamingCallback } from './audio/IAudioProvider';
const nativeAudio: IAudioProvider = require('./audio').default;
import * as Sentry from '@sentry/electron/main';

// Store streaming callback for user audio
let userStreamingCallback: StreamingCallback | null = null;

/**
 * Set streaming callback for user audio
 * @param {Function} callback - Callback function(buffer, format)
 *   - buffer: Buffer containing raw audio data
 *   - format: AudioFormat { sampleRate, channels, bitDepth, isFloat }
 */
function setUserStreamingCallback(callback: StreamingCallback | null) {
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
async function startUserStreaming({ streamingCallback }: { streamingCallback: StreamingCallback }) {
  if (!streamingCallback || typeof streamingCallback !== 'function') {
    throw new Error('[RECORDER] streamingCallback is required for startUserStreaming');
  }
  
  // Set streaming callback (both local and native module)
  setUserStreamingCallback(streamingCallback);
  
  let micStartReason: string | undefined;
  try {
    // Use native module for microphone capture. SAYSO-347: native returns plain `true` on success
    // (unchanged) but on failure now returns { ok: false, reason } instead of a bare `false` — the
    // reason distinguishes a wiring bug, a teardown race, and four genuinely different ways the
    // audio engine itself can fail (see IAudioProvider's MicStartFailureReason), previously all
    // indistinguishable in Sentry. Still tolerate a bare `false` for a stale native build that
    // predates this change (falls back to the old generic message).
    const result = await nativeAudio.startMicrophoneCapture({ streamingCallback });

    if (result !== true) {
      micStartReason = result && typeof result === 'object' ? result.reason : undefined;
      const error: any = new Error(micStartReason ? `mic_start_failed reason=${micStartReason}` : 'Failed to start microphone capture');
      // Not `error.code` — that's conventionally a Node system error code (ENOENT, …); a collision
      // there could confuse an upstream `err.code === '…'` check that has nothing to do with mic start.
      if (micStartReason) error.micStartReason = micStartReason;
      throw error;
    }

    return { success: true };

  } catch (error: any) {
    console.error('🎤 [RECORDER] ❌ Error starting user streaming:', error);
    // Tag rather than rely on the message substring alone — Sentry doesn't serialize arbitrary
    // error properties by default (no extraErrorDataIntegration configured), so `error.micStartReason`
    // itself never reaches the event; a tag makes the reason queryable/facetable instead of only
    // grep-in-message searchable.
    Sentry.captureException(error, micStartReason ? { tags: { mic_start_reason: micStartReason } } : undefined);
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
