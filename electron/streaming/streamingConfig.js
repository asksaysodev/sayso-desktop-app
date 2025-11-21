// CUE Streaming Configuration
// Centralized configuration for audio streaming to AssemblyAI

// AssemblyAI format requirements
const ASSEMBLYAI_FORMAT = {
  encoding: 'pcm_s16le',  
  sampleRate: 16000,      
  channels: 1             
};

// Source audio formats (what we receive from capture)
const AUDIO_SOURCE_FORMATS = {
  screenCapture: {
    sampleRate: 48000,    // 48kHz from ScreenCaptureKit
    channels: 2,          // Stereo
    bitDepth: 32         // 32-bit
  },
  userMic: {
    sampleRate: 48000,    // 48kHz from FFmpeg
    channels: 2,          // Stereo
    bitDepth: 16          // 16-bit
  }
};

// Streaming endpoint paths
const STREAMING_ENDPOINTS = {
  userStream: '/cue/stream/user',
  prospectStream: '/cue/stream/prospect',
  insights: '/cue/insights'
};

// Chunk buffering configuration
const CHUNK_CONFIG = {
  targetSizeMs: 100,  // Default 100ms
  minSizeMs: 50,   // AssemblyAI minimum
  maxSizeMs: 1000  // AssemblyAI maximum
};

// Validate chunk size is within bounds
if (CHUNK_CONFIG.targetSizeMs < CHUNK_CONFIG.minSizeMs || 
    CHUNK_CONFIG.targetSizeMs > CHUNK_CONFIG.maxSizeMs) {
  console.warn(`⚠️ [StreamingConfig] Invalid chunk size ${CHUNK_CONFIG.targetSizeMs}ms, using default 100ms`);
}

// WebSocket connection configuration
const CONNECTION_CONFIG = {
  timeout: 30000,           // 30 seconds
  reconnectAttempts: 3      // Max reconnection attempts
};


function getWebSocketUrl(endpoint) {
  const baseUrl = process.env.VITE_BACKEND_BASE_URL || 'http://localhost:4000';
  
  // Convert http/https to ws/wss
  let wsUrl = baseUrl.replace(/^http:/, 'ws:').replace(/^https:/, 'wss:');
  
  // Remove trailing slash if present
  wsUrl = wsUrl.replace(/\/$/, '');
  
  // Ensure endpoint starts with /
  const path = endpoint.startsWith('/') ? endpoint : `/${endpoint}`;
  
  return `${wsUrl}${path}`;
}

module.exports = {
  ASSEMBLYAI_FORMAT,
  AUDIO_SOURCE_FORMATS,
  STREAMING_ENDPOINTS,
  CHUNK_CONFIG,
  CONNECTION_CONFIG,
  getWebSocketUrl
};

