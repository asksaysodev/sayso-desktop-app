// WebSocket Client for Audio Streaming
// Handles WebSocket connections to backend for audio streaming

const WebSocket = require('ws');
const EventEmitter = require('events');
const { getWebSocketUrl, CONNECTION_CONFIG, STREAMING_ENDPOINTS } = require('./streamingConfig');
const Sentry = require("@sentry/electron/main");

/**
 * WebSocketClient - Manages WebSocket connection for audio streaming
 * 
 * Handles:
 * - Connection lifecycle (connect, disconnect, reconnect)
 * - Authentication via query params (token + sessionId)
 * - Binary audio data transmission
 * - Connection state management
 * - Event emission for connection events
 */
class WebSocketClient extends EventEmitter {
  /**
   * @param {string} speaker - 'user' | 'prospect' (for logging)
   * @param {string} endpoint - WebSocket endpoint path (e.g., '/cue/stream/user')
   * @param {Object} options - Configuration options
   * @param {string} options.token - Supabase JWT token for authentication
   * @param {string} options.sessionId - Session ID from backend endpoint
   * @param {Function} options.onConnected - Callback when connected
   * @param {Function} options.onDisconnected - Callback when disconnected
   * @param {Function} options.onError - Callback on error
   * @param {Function} options.onReconnecting - Callback when reconnecting
   */
  constructor(speaker, endpoint, options = {}) {
    super();
    
    this.speaker = speaker;
    this.endpoint = endpoint;
    this.token = options.token || null;
    this.sessionId = options.sessionId || null;
    
    // Connection state
    this.ws = null;
    this.state = 'disconnected'; // 'disconnected' | 'connecting' | 'connected' | 'error'
    this.reconnectAttempts = 0;
    this.reconnectTimer = null;
    this.shouldReconnect = false;
    
    // Callbacks (also emit events)
    if (options.onConnected) {
      this.on('connected', options.onConnected);
    }
    if (options.onDisconnected) {
      this.on('disconnected', options.onDisconnected);
    }
    if (options.onError) {
      this.on('error', options.onError);
    }
    if (options.onReconnecting) {
      this.on('reconnecting', options.onReconnecting);
    }
  }

  /**
   * Connect to WebSocket server
   * @param {string} token - Optional token override
   * @param {string} sessionId - Optional sessionId override
   * @returns {Promise<void>}
   */
  async connect(token = null, sessionId = null) {
    // Use provided values or fall back to constructor options
    const authToken = token || this.token;
    const authSessionId = sessionId || this.sessionId;
    
    if (!authToken) {
      throw new Error(`[WebSocketClient:${this.speaker}] Token is required for connection`);
    }
    
    if (!authSessionId) {
      throw new Error(`[WebSocketClient:${this.speaker}] Session ID is required for connection`);
    }
    
    // Update stored values
    this.token = authToken;
    this.sessionId = authSessionId;
    
    // Don't connect if already connected or connecting
    if (this.state === 'connected' || this.state === 'connecting') {
      console.log(`⚠️ [WebSocketClient:${this.speaker}] Already ${this.state}, skipping connect`);
      return;
    }
    
    // Close existing connection if any
    if (this.ws) {
      this.ws.removeAllListeners();
      this.ws.close();
      this.ws = null;
    }
    
    this.shouldReconnect = true;
    this.reconnectAttempts = 0;
    return this._connect();
  }

  /**
   * Internal connection method
   * @private
   */
  _connect() {
    return new Promise((resolve, reject) => {
      this.state = 'connecting';
      this.reconnectAttempts++;
      
      // Build WebSocket URL with query params
      const baseUrl = getWebSocketUrl(this.endpoint);
      const url = new URL(baseUrl);
      url.searchParams.set('token', this.token);
      url.searchParams.set('sessionId', this.sessionId);
      
      const wsUrl = url.toString();
      
      try {
        this.ws = new WebSocket(wsUrl);
        
        // Connection timeout
        const timeout = setTimeout(() => {
          if (this.state === 'connecting') {
            console.error(`⏱️ [WebSocketClient:${this.speaker}] Connection timeout`);
            this.ws.close();
            this._handleDisconnect();
            reject(new Error('Connection timeout'));
          }
        }, CONNECTION_CONFIG.timeout);
        
        // Connection opened
        this.ws.on('open', () => {
          clearTimeout(timeout);
          this.state = 'connected';
          this.emit('connected');
          resolve();
        });
        
        // Message received (for future use - transcripts, etc.)
        this.ws.on('message', (data) => {
          // Backend might send JSON messages (transcripts, errors, etc.)
          try {
            const message = JSON.parse(data.toString());
            console.log(`🔴 [WebSocketClient:${this.speaker}] Message received:`, message);
            this.emit('message', message);
          } catch (e) {
            // Binary message or non-JSON
            this.emit('message', data);
          }
        });
        
        // Connection error
        this.ws.on('error', (error) => {
          clearTimeout(timeout);
          console.error(`❌ [WebSocketClient:${this.speaker}] WebSocket error:`, error.message);
          Sentry.captureException(error);
          this.state = 'error';
          this.emit('error', error);
          reject(error);
        });
        
        // Connection closed
        this.ws.on('close', (code, reason) => {
          clearTimeout(timeout);
          this._handleDisconnect(code);
        });
        
      } catch (error) {
        clearTimeout(timeout);
        console.error(`❌ [WebSocketClient:${this.speaker}] Failed to create WebSocket:`, error);
        Sentry.captureException(error);
        this.state = 'error';
        this.emit('error', error);
        reject(error);
      }
    });
  }

  /**
   * Handle disconnection and attempt reconnection if needed
   * @private
   */
  _handleDisconnect(closeCode = 1006) {
	console.log(`[WSClient:${this.speaker}] _handleDisconnect code=${closeCode} attempts=${this.reconnectAttempts} shouldReconnect=${this.shouldReconnect}`);
    this.state = 'disconnected';
    this.ws = null;
    this.emit('disconnected');

    if (closeCode === 1000) {
      this.shouldReconnect = false;
      return;
    }

    // Attempt reconnection if enabled and within retry limit
    if (this.shouldReconnect && this.reconnectAttempts < CONNECTION_CONFIG.reconnectAttempts) {
      const delay = Math.min(1000 * Math.pow(2, this.reconnectAttempts), 10000); // Exponential backoff, max 10s
      console.log(`🔄 [WebSocketClient:${this.speaker}] Reconnecting in ${delay}ms (attempt ${this.reconnectAttempts}/${CONNECTION_CONFIG.reconnectAttempts})...`);
      
      this.emit('reconnecting', {
        attempt: this.reconnectAttempts,
        maxAttempts: CONNECTION_CONFIG.reconnectAttempts,
        delay
      });
      
      this.reconnectTimer = setTimeout(() => {
        this._connect().catch((error) => {
          console.error(`❌ [WebSocketClient:${this.speaker}] Reconnection failed:`, error.message);
          Sentry.captureException(error);
        });
      }, delay);
    } else if (this.reconnectAttempts >= CONNECTION_CONFIG.reconnectAttempts) {
      console.error(`❌ [WebSocketClient:${this.speaker}] Max reconnection attempts reached`);
      this.shouldReconnect = false;
    }
  }

  /**
   * Send binary audio data
   * @param {Buffer} buffer - Audio buffer to send (PCM16, 16kHz, mono)
   * @returns {boolean} - True if sent successfully, false otherwise
   */
  send(buffer) {
    if (!this.isConnected()) {
      console.warn(`⚠️ [WebSocketClient:${this.speaker}] Cannot send data - not connected (state: ${this.state})`);
      return false;
    }
    
    if (!Buffer.isBuffer(buffer)) {
      console.error(`❌ [WebSocketClient:${this.speaker}] Invalid buffer - must be a Buffer`);
      return false;
    }
    
    try {
      this.ws.send(buffer, { binary: true });
      return true;
    } catch (error) {
      console.error(`❌ [WebSocketClient:${this.speaker}] Failed to send data:`, error.message);
      Sentry.captureException(error);
      this.emit('error', error);
      return false;
    }
  }

  /**
   * Send a JSON control message (e.g. force_endpoint after mic route change)
   * @param {Object} payload - Serializable object
   * @returns {boolean}
   */
  sendJson(payload) {
    if (!this.isConnected()) {
      console.warn(`⚠️ [WebSocketClient:${this.speaker}] Cannot send JSON — not connected (state: ${this.state})`);
      return false;
    }
    try {
      this.ws.send(JSON.stringify(payload));
      return true;
    } catch (error) {
      console.error(`❌ [WebSocketClient:${this.speaker}] Failed to send JSON:`, error.message);
      Sentry.captureException(error);
      this.emit('error', error);
      return false;
    }
  }

  /**
   * Disconnect from WebSocket server
   * @param {boolean} shouldReconnect - Whether to allow reconnection after disconnect
   */
  async disconnect(shouldReconnect = false) {
    this.shouldReconnect = shouldReconnect;
    
    if (this.reconnectTimer) {
      clearTimeout(this.reconnectTimer);
      this.reconnectTimer = null;
    }
    
    if (this.ws) {
      this.ws.removeAllListeners();
      this.ws.close();
      this.ws = null;
    }
    
    this.state = 'disconnected';
    this.reconnectAttempts = 0;
    this.emit('disconnected');
  }

  /**
   * Check if WebSocket is connected
   * @returns {boolean}
   */
  isConnected() {
    return this.state === 'connected' && this.ws && this.ws.readyState === WebSocket.OPEN;
  }

  /**
   * Get current connection state
   * @returns {string} - 'disconnected' | 'connecting' | 'connected' | 'error'
   */
  getState() {
    return this.state;
  }

  /**
   * Update token (useful if token expires)
   * @param {string} token - New token
   */
  updateToken(token) {
    this.token = token;
  }

  /**
   * Update session ID
   * @param {string} sessionId - New session ID
   */
  updateSessionId(sessionId) {
    this.sessionId = sessionId;
  }
}

module.exports = {
  WebSocketClient
};