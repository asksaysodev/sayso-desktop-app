// Audio Streamer - Main Orchestrator
// Coordinates audio buffering, conversion, and WebSocket streaming for both user and prospect streams

const { AudioChunkBuffer } = require('./audioBuffer');
const { WebSocketClient } = require('./websocketClient');
const { convertToAssemblyAIFormat } = require('./audioConverter');
const { STREAMING_ENDPOINTS, AUDIO_SOURCE_FORMATS } = require('./streamingConfig');
const axios = require('axios');

/**
 * AudioStreamer - Manages audio streaming for both user and prospect streams
 * 
 * Flow:
 * 1. Raw audio arrives → addUserAudio() / addProspectAudio()
 * 2. Audio added to buffer → AudioChunkBuffer accumulates
 * 3. Check for ready chunks → getReadyChunks()
 * 4. Convert each chunk → convertToAssemblyAIFormat()
 * 5. Send via WebSocket → WebSocketClient.send()
 */
class AudioStreamer {
  /**
   * @param {Object} options - Configuration options
   * @param {string} options.token - Supabase JWT token (can be set later)
   * @param {string} options.sessionId - Session ID (can be set later)
   * @param {Function} options.onUserConnected - Callback when user stream connects
   * @param {Function} options.onProspectConnected - Callback when prospect stream connects
   * @param {Function} options.onError - Callback on error
   */
  constructor(options = {}) {
    this.token = options.token || null;
    this.sessionId = options.sessionId || null;
    this.isStreaming = false;
    
    // User stream components
    this.userBuffer = new AudioChunkBuffer('user');
    this.userWebSocket = null;
    
    // Prospect stream components
    this.prospectBuffer = new AudioChunkBuffer('prospect');
    this.prospectWebSocket = null;
    
    // Callbacks
    this.onUserConnected = options.onUserConnected || null;
    this.onProspectConnected = options.onProspectConnected || null;
    this.onError = options.onError || null;
    this.onMessage = options.onMessage || null;
    
    // Error tracking
    this.userSendFailures = 0;
    this.prospectSendFailures = 0;
    this.maxSendFailures = 5; // Max failures before giving up
  }

  /**
   * Start streaming - Get sessionId and connect both WebSockets
   * @param {string} token - Supabase JWT token
   * @returns {Promise<void>}
   */
  async start(token) {
    if (this.isStreaming) {
      console.warn('⚠️ [AudioStreamer] Already streaming, skipping start');
      return;
    }

    if (!token) {
      throw new Error('[AudioStreamer] Token is required to start streaming');
    }

    this.token = token;

    try {
      // Get sessionId from backend endpoint
      // TODO: Replace with actual endpoint when backend is ready
      // For now, generate a temporary sessionId
      if (!this.sessionId) {
        this.sessionId = await this._getSessionId(token);
      }

      // Create WebSocket clients
      this.userWebSocket = new WebSocketClient('user', STREAMING_ENDPOINTS.userStream, {
        token: this.token,
        sessionId: this.sessionId,
        onConnected: () => {
          if (this.onUserConnected) this.onUserConnected();
        },
        onError: (error) => {
          console.error('❌ [AudioStreamer] User stream error:', error);
          if (this.onError) this.onError('user', error);
        }
      });

      this.prospectWebSocket = new WebSocketClient('prospect', STREAMING_ENDPOINTS.prospectStream, {
        token: this.token,
        sessionId: this.sessionId,
        onConnected: () => {
          if (this.onProspectConnected) this.onProspectConnected();
        },
        onError: (error) => {
          console.error('❌ [AudioStreamer] Prospect stream error:', error);
          if (this.onError) this.onError('prospect', error);
        }
      });

      // Listen for messages from prospect websocket (insights come through here)
      this.prospectWebSocket.on('message', (message) => {
        // Check if it's an insight message
        if (message && typeof message === 'object' && message.type === 'insight') {
          if (this.onMessage) {
            this.onMessage(message);
          }
        }
      });

      // Connect both WebSockets
      await Promise.all([
        this.userWebSocket.connect(),
        this.prospectWebSocket.connect()
      ]);

      this.isStreaming = true;
      this.userSendFailures = 0;
      this.prospectSendFailures = 0;

    } catch (error) {
      console.error('❌ [AudioStreamer] Failed to start streaming:', error);
      this.isStreaming = false;
      throw error;
    }
  }

  /**
   * Get session ID from backend endpoint
   * @private
   * @param {string} token - Supabase JWT token
   * @returns {Promise<string>} - Session ID
   */
  async _getSessionId(token) {
    const baseUrl = process.env.VITE_BACKEND_BASE_URL || 'http://localhost:4000';
    
    try {
      // TODO: Replace with actual endpoint when backend is ready
      // For now, return a temporary sessionId
      return `temp-session-${Date.now()}`;
      
      // When backend is ready, uncomment this:
      /*
      const response = await axios.post(
        `${baseUrl}/cue/session`,
        {},
        {
          headers: {
            'Authorization': `Bearer ${token}`,
            'Content-Type': 'application/json'
          }
        }
      );
      return response.data.sessionId;
      */
    } catch (error) {
      console.error('❌ [AudioStreamer] Failed to get sessionId:', error);
      // Fallback to temporary sessionId
      return `temp-session-${Date.now()}`;
    }
  }

  /**
   * Add user audio chunk
   * @param {Buffer} buffer - Raw audio buffer
   * @param {Object} format - Format info { sampleRate, channels, bitDepth, isFloat }
   */
  addUserAudio(buffer, format) {
    if (!this.isStreaming) {
      console.warn('⚠️ [AudioStreamer] User audio received but streaming is not active');
      return; // Silently ignore if not streaming
    }

    try {
      // Add to buffer
      this.userBuffer.addAudioData(buffer, format);
      
      // Process ready chunks
      this._processUserChunks();
    } catch (error) {
      console.error('❌ [AudioStreamer] Error adding user audio:', error);
      if (this.onError) this.onError('user', error);
    }
  }

  /**
   * Add prospect audio chunk
   * @param {Buffer} buffer - Raw audio buffer
   * @param {Object} format - Format info { sampleRate, channels, bitDepth, isFloat }
   */
  addProspectAudio(buffer, format) {
    if (!this.isStreaming) {
      return; // Silently ignore if not streaming
    }

    try {
      // Add to buffer
      this.prospectBuffer.addAudioData(buffer, format);
      
      // Process ready chunks
      this._processProspectChunks();
    } catch (error) {
      console.error('❌ [AudioStreamer] Error adding prospect audio:', error);
      if (this.onError) this.onError('prospect', error);
    }
  }

  /**
   * Process ready chunks from user buffer
   * @private
   */
  _processUserChunks() {
    if (!this.userWebSocket || !this.userWebSocket.isConnected()) {
      console.warn('⚠️ [AudioStreamer] User chunks ready but WebSocket not connected');
      return; // Can't send if not connected
    }

    try {
      const readyChunks = this.userBuffer.getReadyChunks();
      
      for (const chunk of readyChunks) {
        try {
          // Convert to AssemblyAI format
          const convertedBuffer = convertToAssemblyAIFormat(chunk.buffer, chunk.format);
          
          // Send via WebSocket
          const sent = this.userWebSocket.send(convertedBuffer);
          
          if (sent) {
            this.userSendFailures = 0; // Reset failure count on success
          } else {
            this.userSendFailures++;
            console.warn(`⚠️ [AudioStreamer] User audio send failed (${this.userSendFailures}/${this.maxSendFailures})`);
            if (this.userSendFailures >= this.maxSendFailures) {
              console.error(`❌ [AudioStreamer] User stream: Max send failures reached (${this.maxSendFailures})`);
              if (this.onError) {
                this.onError('user', new Error('Max send failures reached'));
              }
            }
          }
        } catch (conversionError) {
          // Log and continue (don't stop streaming)
          console.error('❌ [AudioStreamer] User audio conversion failed:', conversionError.message);
          // Continue processing other chunks
        }
      }
    } catch (error) {
      console.error('❌ [AudioStreamer] Error processing user chunks:', error);
      if (this.onError) this.onError('user', error);
    }
  }

  /**
   * Process ready chunks from prospect buffer
   * @private
   */
  _processProspectChunks() {
    if (!this.prospectWebSocket || !this.prospectWebSocket.isConnected()) {
      return; // Can't send if not connected
    }

    try {
      const readyChunks = this.prospectBuffer.getReadyChunks();
      
      for (const chunk of readyChunks) {
        try {
          // Convert to AssemblyAI format
          const convertedBuffer = convertToAssemblyAIFormat(chunk.buffer, chunk.format);
          
          // Send via WebSocket
          const sent = this.prospectWebSocket.send(convertedBuffer);
          
          if (sent) {
            this.prospectSendFailures = 0; // Reset failure count on success
          } else {
            this.prospectSendFailures++;
            if (this.prospectSendFailures >= this.maxSendFailures) {
              console.error(`❌ [AudioStreamer] Prospect stream: Max send failures reached (${this.maxSendFailures})`);
              if (this.onError) {
                this.onError('prospect', new Error('Max send failures reached'));
              }
            }
          }
        } catch (conversionError) {
          // Log and continue (don't stop streaming)
          console.error('❌ [AudioStreamer] Prospect audio conversion failed:', conversionError.message);
          // Continue processing other chunks
        }
      }
    } catch (error) {
      console.error('❌ [AudioStreamer] Error processing prospect chunks:', error);
      if (this.onError) this.onError('prospect', error);
    }
  }

  /**
   * Stop streaming - Disconnect WebSockets and optionally send termination messages
   * @param {boolean} sendTermination - Whether to send termination message to backend
   * @returns {Promise<void>}
   */
  async stop(sendTermination = true) {
    if (!this.isStreaming) {
      return;
    }

    try {
      // Send termination messages if requested
      if (sendTermination) {
        // TODO: Send termination message to backend when supported
        // For now, just disconnect
      }

      // Disconnect WebSockets (don't flush buffers per user request)
      const disconnectPromises = [];
      
      if (this.userWebSocket) {
        disconnectPromises.push(this.userWebSocket.disconnect());
      }
      
      if (this.prospectWebSocket) {
        disconnectPromises.push(this.prospectWebSocket.disconnect());
      }

      await Promise.all(disconnectPromises);

      // Reset state
      this.isStreaming = false;
      this.userSendFailures = 0;
      this.prospectSendFailures = 0;

    } catch (error) {
      console.error('❌ [AudioStreamer] Error stopping streaming:', error);
      // Force reset state even on error
      this.isStreaming = false;
      throw error;
    }
  }

  /**
   * Check if streaming is active
   * @returns {boolean}
   */
  isStreamingActive() {
    return this.isStreaming;
  }

  /**
   * Get user stream state
   * @returns {string} - Connection state
   */
  getUserState() {
    return this.userWebSocket ? this.userWebSocket.getState() : 'disconnected';
  }

  /**
   * Get prospect stream state
   * @returns {string} - Connection state
   */
  getProspectState() {
    return this.prospectWebSocket ? this.prospectWebSocket.getState() : 'disconnected';
  }

  /**
   * Update token (useful if token expires)
   * @param {string} token - New token
   */
  updateToken(token) {
    this.token = token;
    if (this.userWebSocket) {
      this.userWebSocket.updateToken(token);
    }
    if (this.prospectWebSocket) {
      this.prospectWebSocket.updateToken(token);
    }
  }

  /**
   * Update session ID
   * @param {string} sessionId - New session ID
   */
  updateSessionId(sessionId) {
    this.sessionId = sessionId;
    if (this.userWebSocket) {
      this.userWebSocket.updateSessionId(sessionId);
    }
    if (this.prospectWebSocket) {
      this.prospectWebSocket.updateSessionId(sessionId);
    }
  }
}

module.exports = {
  AudioStreamer
};

