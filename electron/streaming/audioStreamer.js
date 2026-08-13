// Audio Streamer - Main Orchestrator
// Coordinates audio buffering, conversion, and WebSocket streaming for both user and prospect streams

const { AudioChunkBuffer } = require('./audioBuffer');
const { WebSocketClient } = require('./websocketClient');
const { convertToAssemblyAIFormat } = require('./audioConverter');
const { STREAMING_ENDPOINTS, AUDIO_SOURCE_FORMATS } = require('./streamingConfig');
const axios = require('axios');
const { reportStreamingError, reportStreamingMessage, streamingErrorCode } = require('./streamingSentry');
const { isTransientNetworkError } = require('../utils/transientErrors');

/**
 * How long a pending connection report waits for the *other* socket to settle
 * before it fires. Promise.all rejects on the first failure, typically a tick
 * before its sibling errors, so reporting immediately would list only one
 * socket. Bounded in case one socket hangs toward the 30s connect timeout while
 * the other fails fast (SAYSO-348).
 */
const REPORT_SETTLE_CAP_MS = 2000;

/**
 * Same idea for a mid-session give-up, where there is no connect promise to
 * wait on. Both sockets exhaust their 1s/2s/4s backoff within milliseconds of
 * each other when the cause is shared.
 */
const REPORT_GRACE_MS = 1000;

function newReportState() {
  return { failures: new Map(), reported: false, pending: false, timer: null };
}

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
    this._userSendReported = false;
    this._prospectSendReported = false;

    this.autoStopping = false;

    // Connection-failure reporting, scoped to one start attempt.
    this._report = newReportState();

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

    // Failure state is per start attempt. Replaced rather than cleared, and not
    // touched in stop(), so a report still in flight — from a previous attempt
    // or from a Stop mid-settle — survives on its own captured state.
    this._report = newReportState();

    // Held so the catch below can wait for *both* sockets to settle before
    // reporting, without the success path paying for it.
    let connectPromises = null;

    try {
      // Get sessionId from backend endpoint
      // TODO: Replace with actual endpoint when backend is ready
      // For now, generate a temporary sessionId
      if (!this.sessionId) {
        this.sessionId = await this._getSessionId(token);
      }

      // Create WebSocket clients. The onError callbacks record rather than
      // report — they are the only place that knows which speaker failed, and
      // a DNS blip fires both of them for one underlying cause (SAYSO-348).
      this.userWebSocket = new WebSocketClient('user', STREAMING_ENDPOINTS.userStream, {
        token: this.token,
        sessionId: this.sessionId,
        onConnected: () => {
          if (this.onUserConnected) this.onUserConnected();
        },
        onError: (error) => {
          console.error('❌ [AudioStreamer] User stream error:', error);
          this._recordSocketFailure('user', STREAMING_ENDPOINTS.userStream, error);
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
          this._recordSocketFailure('prospect', STREAMING_ENDPOINTS.prospectStream, error);
          if (this.onError) this.onError('prospect', error);
        }
      });

      this.userWebSocket.on('give-up', (error) => {
        this._recordSocketFailure('user', STREAMING_ENDPOINTS.userStream, error);
        this._scheduleFailureReport(null, 'reconnect-exhausted', REPORT_GRACE_MS);
      });
      this.prospectWebSocket.on('give-up', (error) => {
        this._recordSocketFailure('prospect', STREAMING_ENDPOINTS.prospectStream, error);
        this._scheduleFailureReport(null, 'reconnect-exhausted', REPORT_GRACE_MS);
      });

      // Listen for messages from prospect websocket (insights come through here)
      this.prospectWebSocket.on('message', (message) => {
        // Check if it's a message we care about forwarding
        if (message && typeof message === 'object' && (message.type === 'insight' || message.type === 'auto_stop' || message.type === 'smart_capture')) {
          if (this.onMessage) {
            if (message.type === 'auto_stop') {
              this.autoStopping = true;
              // Cancel pending reconnect timers synchronously — before the IPC round-trip
              if (this.userWebSocket) {
                this.userWebSocket.shouldReconnect = false;
                if (this.userWebSocket.reconnectTimer) {
                  clearTimeout(this.userWebSocket.reconnectTimer);
                  this.userWebSocket.reconnectTimer = null;
                }
              }
              if (this.prospectWebSocket) {
                this.prospectWebSocket.shouldReconnect = false;
                if (this.prospectWebSocket.reconnectTimer) {
                  clearTimeout(this.prospectWebSocket.reconnectTimer);
                  this.prospectWebSocket.reconnectTimer = null;
                }
              }
            }
            this.onMessage(message);
          }
        }
      });

      connectPromises = [
        this.userWebSocket.connect(),
        this.prospectWebSocket.connect()
      ];
      await Promise.all(connectPromises);

      this.isStreaming = true;
      this.userSendFailures = 0;
      this.prospectSendFailures = 0;
      this._userSendReported = false;
      this._prospectSendReported = false;
      this.autoStopping = false;
      this._routeResetTimer = null;
      this._logEnergyUntilMs = 0;
      
    } catch (error) {
      console.error('❌ [AudioStreamer] Failed to start streaming:', error);
      // Deliberately not awaited: start() must re-throw now or the Start button
      // hangs. The report lands a beat later, once both sockets have settled.
      this._recordUnattributedFailure(error);
      this._scheduleFailureReport(connectPromises, 'connect', REPORT_SETTLE_CAP_MS);
      if (error && typeof error === 'object') {
        error.__cueStreamingReported = true;
      }
      this.isStreaming = false;
      throw error;
    }
  }

  /**
   * Record a socket's failure. First one wins — later errors on the same socket
   * are usually consequences of it, not new information.
   *
   * One exception: a non-transient error always displaces a recorded transient
   * one. A start attempt spans the whole call, so without this a Wi-Fi blip in
   * minute 3 would occupy the slot for good and the auth failure that actually
   * ends the session would never be reported — _flushFailureReport looks for the
   * first non-transient failure and would find nothing (SAYSO-348).
   * @private
   */
  _recordSocketFailure(speaker, endpoint, error) {
    if (!error) {
      return;
    }
    // Synthesized by websocketClient when every attempt closed without an
    // 'error' event. Carries no diagnosis, so it must not become a report.
    if (error.code === 'WS_CLOSED_NO_ERROR') {
      return;
    }
    const failures = this._report.failures;
    const existing = failures.get(speaker);
    if (existing && (isTransientNetworkError(error) || !isTransientNetworkError(existing.error))) {
      return;
    }
    failures.set(speaker, {
      speaker,
      endpoint,
      error,
      code: streamingErrorCode(error)
    });
  }

  /**
   * Backstop for a start() failure that never reached a socket's onError — a
   * bad sessionId, say. Without it such an error would have nothing recorded
   * and would flush to nothing.
   * @private
   */
  _recordUnattributedFailure(error) {
    if (this._report.failures.size === 0) {
      this._recordSocketFailure('session', null, error);
    }
  }

  /**
   * Schedule the single failure report for this start attempt.
   *
   * @param {Array<Promise>|null} connectPromises - awaited (settled, not
   *   resolved) so the report can name every socket that failed; null for
   *   mid-session give-ups, where the cap alone acts as the grace window.
   * @private
   */
  _scheduleFailureReport(connectPromises, stage, capMs) {
    // Captured, not re-read: a restart during the settle window swaps
    // this._report, and this report belongs to the attempt that scheduled it.
    const report = this._report;
    if (report.reported || report.pending) {
      return;
    }
    report.pending = true;

    const settled = connectPromises
      ? Promise.allSettled(connectPromises)
      : new Promise(() => {}); // never settles; the cap below is the trigger

    const capped = new Promise((resolve) => {
      report.timer = setTimeout(resolve, capMs);
      // Don't hold the event loop open on quit just to file a Sentry report.
      if (typeof report.timer.unref === 'function') report.timer.unref();
    });

    Promise.race([settled, capped]).then(() => {
      if (report.timer) {
        clearTimeout(report.timer);
        report.timer = null;
      }
      report.pending = false;
      this._flushFailureReport(report, stage);
    });
  }

  /**
   * Send at most one Sentry row for this start attempt, carrying every socket
   * that failed. The representative is the first *non-transient* failure:
   * picking the first failure outright would let an environmental blip on one
   * socket suppress a real error on the other.
   *
   * @param {Object} report - the attempt's state, captured at schedule time
   * @private
   */
  _flushFailureReport(report, stage) {
    if (report.reported) {
      return;
    }

    const failures = Array.from(report.failures.values());
    const primary = failures.find(f => !isTransientNetworkError(f.error));
    if (!primary) {
      // Every socket failed for environmental reasons — console only.
      return;
    }

    report.reported = reportStreamingError(primary.error, {
      speaker: primary.speaker,
      endpoint: primary.endpoint,
      sessionId: this.sessionId,
      stage,
      failures: failures.map(f => ({
        speaker: f.speaker,
        endpoint: f.endpoint,
        code: f.code
      }))
    });
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
      reportStreamingError(error, { speaker: 'session', sessionId: null, stage: 'session-init' });
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
      reportStreamingError(error, {
        speaker: 'user',
        endpoint: STREAMING_ENDPOINTS.userStream,
        sessionId: this.sessionId,
        stage: 'ingest'
      });
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
      reportStreamingError(error, {
        speaker: 'prospect',
        endpoint: STREAMING_ENDPOINTS.prospectStream,
        sessionId: this.sessionId,
        stage: 'ingest'
      });
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
              // Latched: the counter only resets on a successful send, so once
              // the socket is gone every remaining chunk re-satisfies this. The
              // onError below stays unlatched — the banner is SAYSO-346's, and
              // user-visible behaviour must not change here.
              if (!this._userSendReported) {
                this._userSendReported = true;
                // Suppressed alongside its cause when the socket died to a blip.
                reportStreamingMessage(`User stream: Max send failures reached (${this.maxSendFailures})`, {
                  speaker: 'user',
                  endpoint: STREAMING_ENDPOINTS.userStream,
                  sessionId: this.sessionId,
                  stage: 'send-threshold',
                  cause: this.userWebSocket && this.userWebSocket.lastError
                });
              }
              if (this.onError) {
                this.onError('user', new Error('Max send failures reached'));
              }
            }
          }
        } catch (conversionError) {
          // Log and continue (don't stop streaming)
          console.error('❌ [AudioStreamer] User audio conversion failed:', conversionError.message);
          reportStreamingError(conversionError, {
            speaker: 'user',
            endpoint: STREAMING_ENDPOINTS.userStream,
            sessionId: this.sessionId,
            stage: 'convert'
          });
          // Continue processing other chunks
        }
      }
    } catch (error) {
      console.error('❌ [AudioStreamer] Error processing user chunks:', error);
      reportStreamingError(error, {
        speaker: 'user',
        endpoint: STREAMING_ENDPOINTS.userStream,
        sessionId: this.sessionId,
        stage: 'process'
      });
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
              // Latched — see the user-stream copy above.
              if (!this._prospectSendReported) {
                this._prospectSendReported = true;
                // Suppressed alongside its cause when the socket died to a blip.
                reportStreamingMessage(`Prospect stream: Max send failures reached (${this.maxSendFailures})`, {
                  speaker: 'prospect',
                  endpoint: STREAMING_ENDPOINTS.prospectStream,
                  sessionId: this.sessionId,
                  stage: 'send-threshold',
                  cause: this.prospectWebSocket && this.prospectWebSocket.lastError
                });
              }
              if (this.onError) {
                this.onError('prospect', new Error('Max send failures reached'));
              }
            }
          }
        } catch (conversionError) {
          // Log and continue (don't stop streaming)
          console.error('❌ [AudioStreamer] Prospect audio conversion failed:', conversionError.message);
          reportStreamingError(conversionError, {
            speaker: 'prospect',
            endpoint: STREAMING_ENDPOINTS.prospectStream,
            sessionId: this.sessionId,
            stage: 'convert'
          });
          // Continue processing other chunks
        }
      }
    } catch (error) {
      console.error('❌ [AudioStreamer] Error processing prospect chunks:', error);
      reportStreamingError(error, {
        speaker: 'prospect',
        endpoint: STREAMING_ENDPOINTS.prospectStream,
        sessionId: this.sessionId,
        stage: 'process'
      });
      if (this.onError) this.onError('prospect', error);
    }
  }

  /**
   * Stop streaming - Disconnect WebSockets and optionally send termination messages
   * @param {boolean} sendTermination - Whether to send termination message to backend
   * @returns {Promise<void>}
   */
  async stop(sendTermination = true) {
    if (!this.isStreaming && !this.userWebSocket && !this.prospectWebSocket) {
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
      this._userSendReported = false;
      this._prospectSendReported = false;
      if (this._routeResetTimer) {
        clearTimeout(this._routeResetTimer);
        this._routeResetTimer = null;
      }
      this._logEnergyUntilMs = 0;

    } catch (error) {
      console.error('❌ [AudioStreamer] Error stopping streaming:', error);
      reportStreamingError(error, {
        speaker: 'session',
        sessionId: this.sessionId,
        stage: 'stop'
      });
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

