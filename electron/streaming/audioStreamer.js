// Audio Streamer - Main Orchestrator
// Coordinates audio buffering, conversion, and WebSocket streaming for both user and prospect streams

const { AudioChunkBuffer } = require('./audioBuffer');
const { WebSocketClient } = require('./websocketClient');
const { convertToAssemblyAIFormat } = require('./audioConverter');
const { STREAMING_ENDPOINTS, AUDIO_SOURCE_FORMATS } = require('./streamingConfig');
const axios = require('axios');
const Sentry = require("@sentry/electron/main");

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

    this.autoStopping = false;
    /** Debounce timer for reset_transcription (fresh STT session) after route stabilizes. */
    this._routeResetTimer = null;
    /** Log PCM energy for N ms after a route change to confirm audio is non-silent. */
    this._logEnergyUntilMs = 0;
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
          Sentry.captureException(error);
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
          Sentry.captureException(error);
          if (this.onError) this.onError('prospect', error);
        }
      });

      // Listen for messages from prospect websocket (insights come through here)
      this.prospectWebSocket.on('message', (message) => {
        // Check if it's an insight message
        if (message && typeof message === 'object' && (message.type === 'insight' || message.type === 'auto_stop')) {
          if (this.onMessage) {
            if (message.type === 'auto_stop') {
              this.autoStopping = true;
            }
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
      this.autoStopping = false;
      this._routeResetTimer = null;
      this._logEnergyUntilMs = 0;
      
    } catch (error) {
      console.error('❌ [AudioStreamer] Failed to start streaming:', error);
      Sentry.captureException(error);
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
      Sentry.captureException(error);
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
      const routeFormatChanged = this.userBuffer.addAudioData(buffer, format);
      if (routeFormatChanged && this.userWebSocket && this.userWebSocket.isConnected()) {
        const now = Date.now();

        // Debounced: request a fresh STT session (non-destructive swap on server) after route settles.
        // Do NOT send force_endpoint — AssemblyAI treats ForceEndpoint as a session terminator.
        if (this._routeResetTimer) {
          clearTimeout(this._routeResetTimer);
        }
        this._routeResetTimer = setTimeout(() => {
          this._routeResetTimer = null;
          if (this.userWebSocket && this.userWebSocket.isConnected()) {
            this.userWebSocket.sendJson({ type: 'reset_transcription' });
            if (process.env.NODE_ENV === 'development') {
              console.log('[AudioStreamer] Sent reset_transcription after route stabilize');
            }
          }
        }, 1500);

        // Gate near-silent chunks and log energy for 5s after the route change
        this._logEnergyUntilMs = now + 5000;
      }

      // Process ready chunks
      this._processUserChunks();
    } catch (error) {
      console.error('❌ [AudioStreamer] Error adding user audio:', error);
      Sentry.captureException(error);
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
      Sentry.captureException(error);
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

          // Within 3 seconds of a route change, compute peak energy and gate near-silent chunks.
          // AirPods take ~0.5-1s to initialize their mic on reconnect, emitting noise/silence
          // that can cause AssemblyAI to close the active session. Drop those bad chunks early.
          const inRouteChangeWindow = Date.now() < this._logEnergyUntilMs;
          if (inRouteChangeWindow) {
            const samples = new Int16Array(convertedBuffer.buffer, convertedBuffer.byteOffset, convertedBuffer.length / 2);
            let maxAbs = 0;
            for (let i = 0; i < samples.length; i++) {
              const v = Math.abs(samples[i]);
              if (v > maxAbs) maxAbs = v;
            }
            if (maxAbs < 500) {
              if (process.env.NODE_ENV === 'development') {
                console.warn(`⚠️ [AudioStreamer] User chunk SILENT after route change (maxSample=${maxAbs}) — dropping chunk`);
              }
              continue; // Don't send garbage audio to the server during mic init
            }
            if (process.env.NODE_ENV === 'development') {
              if (maxAbs < 1500) {
                console.log(`🔉 [AudioStreamer] User chunk low-energy after route change (maxSample=${maxAbs})`);
              } else {
                console.log(`✅ [AudioStreamer] User chunk energy ok after route change (maxSample=${maxAbs})`);
              }
            }
          }

          // Send via WebSocket
          const sent = this.userWebSocket.send(convertedBuffer);
          
          if (sent) {
            this.userSendFailures = 0; // Reset failure count on success
          } else {
            this.userSendFailures++;
            console.warn(`⚠️ [AudioStreamer] User audio send failed (${this.userSendFailures}/${this.maxSendFailures})`);
            if (this.userSendFailures >= this.maxSendFailures) {
              console.error(`❌ [AudioStreamer] User stream: Max send failures reached (${this.maxSendFailures})`);
              Sentry.captureMessage(`User stream: Max send failures reached (${this.maxSendFailures})`, 'error');
              if (this.onError) {
                this.onError('user', new Error('Max send failures reached'));
              }
            }
          }
        } catch (conversionError) {
          // Log and continue (don't stop streaming)
          console.error('❌ [AudioStreamer] User audio conversion failed:', conversionError.message);
          Sentry.captureException(conversionError);
          // Continue processing other chunks
        }
      }
    } catch (error) {
      console.error('❌ [AudioStreamer] Error processing user chunks:', error);
      Sentry.captureException(error);
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
              Sentry.captureMessage(`Prospect stream: Max send failures reached (${this.maxSendFailures})`, 'error');
              if (this.onError) {
                this.onError('prospect', new Error('Max send failures reached'));
              }
            }
          }
        } catch (conversionError) {
          // Log and continue (don't stop streaming)
          console.error('❌ [AudioStreamer] Prospect audio conversion failed:', conversionError.message);
          Sentry.captureException(conversionError);
          // Continue processing other chunks
        }
      }
    } catch (error) {
      console.error('❌ [AudioStreamer] Error processing prospect chunks:', error);
      Sentry.captureException(error);
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
      if (this._routeResetTimer) {
        clearTimeout(this._routeResetTimer);
        this._routeResetTimer = null;
      }
      this._logEnergyUntilMs = 0;

    } catch (error) {
      console.error('❌ [AudioStreamer] Error stopping streaming:', error);
      Sentry.captureException(error);
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

