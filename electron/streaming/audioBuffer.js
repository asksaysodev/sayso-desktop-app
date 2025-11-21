// Audio Chunk Buffer
// Accumulates raw audio data and outputs chunks that meet AssemblyAI's 50-1000ms requirement
// Format conversion happens AFTER buffering (in audioStreamer.js)

const { CHUNK_CONFIG } = require('./streamingConfig');

/**
 * Calculate duration in milliseconds from buffer size and format
 * @param {number} bufferSize - Buffer size in bytes
 * @param {Object} format - Format info { sampleRate, channels, bitDepth, isFloat }
 * @returns {number} - Duration in milliseconds
 */
function calculateDurationMs(bufferSize, format) {
  if (!format || bufferSize === 0) return 0;
  
  const bytesPerSample = format.bitDepth / 8;
  const bytesPerSecond = format.sampleRate * format.channels * bytesPerSample;
  return (bufferSize / bytesPerSecond) * 1000;
}

/**
 * Calculate buffer size in bytes for a given duration
 * @param {number} durationMs - Duration in milliseconds
 * @param {Object} format - Format info { sampleRate, channels, bitDepth }
 * @returns {number} - Buffer size in bytes
 */
function calculateBufferSize(durationMs, format) {
  if (!format) return 0;
  
  const bytesPerSample = format.bitDepth / 8;
  const bytesPerSecond = format.sampleRate * format.channels * bytesPerSample;
  return Math.floor((bytesPerSecond * durationMs) / 1000);
}

/**
 * AudioChunkBuffer - Accumulates raw audio and outputs ready chunks
 * 
 * Stores raw audio data and returns chunks when:
 * - Buffer has >= targetSizeMs worth of audio, OR
 * - Buffer has >= 50ms AND no new data for 300ms (timeout)
 */
class AudioChunkBuffer {
  /**
   * @param {string} speaker - 'user' or 'prospect' (for logging)
   * @param {number} targetChunkSizeMs - Target chunk size in ms (default: 100ms)
   */
  constructor(speaker, targetChunkSizeMs = CHUNK_CONFIG.targetSizeMs) {
    this.speaker = speaker;
    this.buffer = Buffer.alloc(0); // Internal buffer for raw audio
    this.format = null; // Set on first chunk: { sampleRate, channels, bitDepth, isFloat }
    this.targetChunkSizeMs = targetChunkSizeMs;
    this.lastDataTime = null; // Timestamp of last data arrival
    this.timeoutMs = 300; // Flush timeout (300ms)
    this.maxBufferSizeMs = 5000; // Safety limit: 5 seconds
    
    // Validate target chunk size
    if (this.targetChunkSizeMs < CHUNK_CONFIG.minSizeMs || 
        this.targetChunkSizeMs > CHUNK_CONFIG.maxSizeMs) {
      console.warn(`⚠️ [AudioBuffer:${speaker}] Invalid target chunk size ${this.targetChunkSizeMs}ms, using default ${CHUNK_CONFIG.targetSizeMs}ms`);
      this.targetChunkSizeMs = CHUNK_CONFIG.targetSizeMs;
    }
  }

  /**
   * Add raw audio data to the buffer
   * @param {Buffer} audioBuffer - Raw audio buffer
   * @param {Object} format - Format info { sampleRate, channels, bitDepth, isFloat }
   */
  addAudioData(audioBuffer, format) {
    if (!audioBuffer || audioBuffer.length === 0) {
      return;
    }

    // Set format on first chunk
    if (!this.format) {
      this.format = { ...format };
      console.log(`🎵 [AudioBuffer:${this.speaker}] Format set: ${format.sampleRate}Hz, ${format.channels}ch, ${format.bitDepth}bit`);
    } else {
      // Warn if format changes (but keep using first format)
      const formatChanged = 
        this.format.sampleRate !== format.sampleRate ||
        this.format.channels !== format.channels ||
        this.format.bitDepth !== format.bitDepth ||
        this.format.isFloat !== format.isFloat;
      
      if (formatChanged) {
        console.warn(`⚠️ [AudioBuffer:${this.speaker}] Format changed but keeping original format. New: ${format.sampleRate}Hz/${format.channels}ch/${format.bitDepth}bit, Using: ${this.format.sampleRate}Hz/${this.format.channels}ch/${this.format.bitDepth}bit`);
      }
    }

    // Append to buffer
    this.buffer = Buffer.concat([this.buffer, audioBuffer]);
    this.lastDataTime = Date.now();

    // Check safety limit (5 seconds max)
    const currentSizeMs = this.getCurrentSize();
    if (currentSizeMs > this.maxBufferSizeMs) {
      console.warn(`⚠️ [AudioBuffer:${this.speaker}] Buffer exceeded ${this.maxBufferSizeMs}ms (${currentSizeMs.toFixed(0)}ms), flushing and resetting`);
      this.buffer = Buffer.alloc(0);
      this.lastDataTime = null;
      return;
    }
  }

  /**
   * Get ready chunks that meet size requirements
   * Returns chunks when: >= targetSizeMs OR (>= 50ms AND timeout exceeded)
   * @returns {Array<{buffer: Buffer, format: Object}>} - Array of ready chunks with format info
   */
  getReadyChunks() {
    if (!this.format || this.buffer.length === 0) {
      return [];
    }

    const currentSizeMs = this.getCurrentSize();
    const now = Date.now();
    const timeSinceLastData = this.lastDataTime ? (now - this.lastDataTime) : Infinity;

    // Check if we have enough data OR timeout condition met
    const hasEnoughData = currentSizeMs >= this.targetChunkSizeMs;
    const timeoutCondition = currentSizeMs >= CHUNK_CONFIG.minSizeMs && timeSinceLastData > this.timeoutMs;

    if (!hasEnoughData && !timeoutCondition) {
      return [];
    }

    // Extract ready chunks
    const readyChunks = [];
    let remainingBuffer = this.buffer;

    // Extract chunks of targetSizeMs until we can't extract more
    while (remainingBuffer.length > 0) {
      const remainingSizeMs = calculateDurationMs(remainingBuffer.length, this.format);
      
      // If we have at least targetSizeMs, extract one chunk
      if (remainingSizeMs >= this.targetChunkSizeMs) {
        const chunkSizeBytes = calculateBufferSize(this.targetChunkSizeMs, this.format);
        const chunk = remainingBuffer.slice(0, chunkSizeBytes);
        remainingBuffer = remainingBuffer.slice(chunkSizeBytes);
        
        readyChunks.push({
          buffer: chunk,
          format: { ...this.format }
        });
      } else {
        // If timeout condition and we have >= 50ms, extract what we have
        if (remainingSizeMs >= CHUNK_CONFIG.minSizeMs && timeSinceLastData > this.timeoutMs) {
          readyChunks.push({
            buffer: remainingBuffer,
            format: { ...this.format }
          });
          remainingBuffer = Buffer.alloc(0);
        }
        break;
      }
    }

    // Update buffer with remaining data
    this.buffer = remainingBuffer;
    
    if (readyChunks.length > 0) {
      console.log(`📦 [AudioBuffer:${this.speaker}] Extracted ${readyChunks.length} chunk(s), remaining: ${calculateDurationMs(this.buffer.length, this.format).toFixed(0)}ms`);
    }

    return readyChunks;
  }

  /**
   * Flush remaining data (even if < 50ms)
   * @returns {Array<{buffer: Buffer, format: Object}>} - Remaining chunk with format info, or empty array
   */
  flush() {
    if (!this.format || this.buffer.length === 0) {
      return [];
    }

    const flushed = [{
      buffer: this.buffer,
      format: { ...this.format }
    }];

    console.log(`🚰 [AudioBuffer:${this.speaker}] Flushed ${calculateDurationMs(this.buffer.length, this.format).toFixed(0)}ms of remaining audio`);
    
    this.reset();
    return flushed;
  }

  /**
   * Reset buffer (clear all data and format)
   */
  reset() {
    this.buffer = Buffer.alloc(0);
    this.format = null;
    this.lastDataTime = null;
    console.log(`🔄 [AudioBuffer:${this.speaker}] Buffer reset`);
  }

  /**
   * Get current buffer size in milliseconds
   * @returns {number} - Duration in milliseconds
   */
  getCurrentSize() {
    if (!this.format) {
      return 0;
    }
    return calculateDurationMs(this.buffer.length, this.format);
  }

  /**
   * Get current buffer size in bytes
   * @returns {number} - Buffer size in bytes
   */
  getCurrentSizeBytes() {
    return this.buffer.length;
  }
}

module.exports = {
  AudioChunkBuffer,
  calculateDurationMs,
  calculateBufferSize
};

