"use strict";
var __createBinding = (this && this.__createBinding) || (Object.create ? (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    var desc = Object.getOwnPropertyDescriptor(m, k);
    if (!desc || ("get" in desc ? !m.__esModule : desc.writable || desc.configurable)) {
      desc = { enumerable: true, get: function() { return m[k]; } };
    }
    Object.defineProperty(o, k2, desc);
}) : (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    o[k2] = m[k];
}));
var __setModuleDefault = (this && this.__setModuleDefault) || (Object.create ? (function(o, v) {
    Object.defineProperty(o, "default", { enumerable: true, value: v });
}) : function(o, v) {
    o["default"] = v;
});
var __importStar = (this && this.__importStar) || (function () {
    var ownKeys = function(o) {
        ownKeys = Object.getOwnPropertyNames || function (o) {
            var ar = [];
            for (var k in o) if (Object.prototype.hasOwnProperty.call(o, k)) ar[ar.length] = k;
            return ar;
        };
        return ownKeys(o);
    };
    return function (mod) {
        if (mod && mod.__esModule) return mod;
        var result = {};
        if (mod != null) for (var k = ownKeys(mod), i = 0; i < k.length; i++) if (k[i] !== "default") __createBinding(result, mod, k[i]);
        __setModuleDefault(result, mod);
        return result;
    };
})();
Object.defineProperty(exports, "__esModule", { value: true });
const nativeAudio = require('./native-audio');
const Sentry = __importStar(require("@sentry/electron/main"));
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
}
/**
 * Start user microphone streaming (for real-time processing like Cue - no file saving)
 * Uses native AVAudioEngine for microphone capture
 * @param {Object} options - Streaming options
 * @param {Function} options.streamingCallback - Callback function(buffer, format)
 * @returns {Promise<Object>} { success: boolean }
 */
async function startUserStreaming({ streamingCallback }) {
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
    }
    catch (error) {
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
    }
    catch (error) {
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
//# sourceMappingURL=recorder.js.map