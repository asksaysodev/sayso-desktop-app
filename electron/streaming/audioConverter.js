// Audio Format Converter
// Converts audio buffers to AssemblyAI streaming format (PCM16, 16kHz, mono)

const { ASSEMBLYAI_FORMAT } = require('./streamingConfig');

/**
 * Convert 32-bit audio samples to 16-bit PCM
 * Handles both integer and float formats
 * @param {Buffer} audioBuffer - Source audio buffer
 * @param {Object} sourceFormat - Format info { bitDepth: 32, isFloat: boolean }
 * @returns {Buffer} - PCM16 buffer
 */
function convertToPCM16(audioBuffer, sourceFormat) {
  if (!audioBuffer || audioBuffer.length === 0) {
    throw new Error('Invalid audio buffer');
  }

  const { bitDepth = 32, isFloat = false } = sourceFormat;
  
  // If already 16-bit, return as-is
  if (bitDepth === 16) {
    return audioBuffer;
  }

  const sourceSampleCount = audioBuffer.length / (bitDepth / 8);
  const outputBuffer = Buffer.alloc(sourceSampleCount * 2); // 2 bytes per 16-bit sample

  if (bitDepth === 32) {
    if (isFloat) {
      // 32-bit float → 16-bit int
      const floatSamples = new Float32Array(audioBuffer.buffer, audioBuffer.byteOffset, sourceSampleCount);
      for (let i = 0; i < sourceSampleCount; i++) {
        // Clamp to [-1.0, 1.0] and scale to int16 range
        const clamped = Math.max(-1.0, Math.min(1.0, floatSamples[i]));
        const int16Sample = Math.round(clamped * 32767);
        outputBuffer.writeInt16LE(int16Sample, i * 2);
      }
    } else {
      // 32-bit integer → 16-bit int
      const int32Samples = new Int32Array(audioBuffer.buffer, audioBuffer.byteOffset, sourceSampleCount);
      for (let i = 0; i < sourceSampleCount; i++) {
        // Normalize to [-1.0, 1.0] then scale to int16
        const normalized = int32Samples[i] / 2147483647.0; // INT32_MAX
        const clamped = Math.max(-1.0, Math.min(1.0, normalized));
        const int16Sample = Math.round(clamped * 32767);
        outputBuffer.writeInt16LE(int16Sample, i * 2);
      }
    }
  } else {
    throw new Error(`Unsupported bit depth: ${bitDepth}`);
  }

  return outputBuffer;
}

/**
 * Resample audio buffer from one sample rate to another
 * Uses linear interpolation (simple but effective)
 * @param {Buffer} audioBuffer - Source audio buffer (PCM16)
 * @param {number} fromRate - Source sample rate (e.g., 48000)
 * @param {number} toRate - Target sample rate (e.g., 16000)
 * @returns {Buffer} - Resampled buffer
 */
function resampleAudio(audioBuffer, fromRate, toRate) {
  if (fromRate === toRate) {
    return audioBuffer;
  }

  if (!audioBuffer || audioBuffer.length === 0) {
    throw new Error('Invalid audio buffer for resampling');
  }

  const ratio = fromRate / toRate;
  const sourceSampleCount = audioBuffer.length / 2; // 2 bytes per 16-bit sample
  const targetSampleCount = Math.floor(sourceSampleCount / ratio);
  const outputBuffer = Buffer.alloc(targetSampleCount * 2);

  for (let i = 0; i < targetSampleCount; i++) {
    const sourceIndex = i * ratio;
    const index1 = Math.floor(sourceIndex);
    const index2 = Math.min(index1 + 1, sourceSampleCount - 1);
    const fraction = sourceIndex - index1;

    // Linear interpolation
    const sample1 = audioBuffer.readInt16LE(index1 * 2);
    const sample2 = audioBuffer.readInt16LE(index2 * 2);
    const interpolated = sample1 + (sample2 - sample1) * fraction;

    outputBuffer.writeInt16LE(Math.round(interpolated), i * 2);
  }

  return outputBuffer;
}

/**
 * Convert stereo audio to mono
 * Averages left and right channels
 * @param {Buffer} audioBuffer - Stereo PCM16 buffer
 * @param {number} channels - Number of channels (default: 2)
 * @returns {Buffer} - Mono PCM16 buffer
 */
function stereoToMono(audioBuffer, channels = 2) {
  if (channels === 1) {
    return audioBuffer; // Already mono
  }

  if (!audioBuffer || audioBuffer.length === 0) {
    throw new Error('Invalid audio buffer for mono conversion');
  }

  const samplesPerChannel = audioBuffer.length / (2 * channels); // 2 bytes per sample, N channels
  const outputBuffer = Buffer.alloc(samplesPerChannel * 2); // 2 bytes per mono sample

  for (let i = 0; i < samplesPerChannel; i++) {
    let sum = 0;
    
    // Sum all channels
    for (let ch = 0; ch < channels; ch++) {
      const sampleIndex = (i * channels + ch) * 2;
      sum += audioBuffer.readInt16LE(sampleIndex);
    }
    
    // Average and write mono sample
    const monoSample = Math.round(sum / channels);
    outputBuffer.writeInt16LE(monoSample, i * 2);
  }

  return outputBuffer;
}

/**
 * Calculate buffer size in bytes for a given duration
 * @param {number} sampleRate - Sample rate in Hz
 * @param {number} durationMs - Duration in milliseconds
 * @param {number} bytesPerSample - Bytes per sample (2 for PCM16)
 * @param {number} channels - Number of channels (1 for mono)
 * @returns {number} - Buffer size in bytes
 */
function calculateChunkSize(sampleRate, durationMs, bytesPerSample = 2, channels = 1) {
  const samplesPerSecond = sampleRate * channels;
  const samplesPerMs = samplesPerSecond / 1000;
  const samplesForDuration = Math.floor(samplesPerMs * durationMs);
  return samplesForDuration * bytesPerSample;
}

/**
 * Master conversion function - converts audio to AssemblyAI format
 * Orchestrates: bit depth → resample → mono conversion
 * @param {Buffer} audioBuffer - Raw audio buffer
 * @param {Object} sourceFormat - Source format { sampleRate, channels, bitDepth, isFloat }
 * @param {Object} targetFormat - Target format (defaults to ASSEMBLYAI_FORMAT)
 * @returns {Buffer} - Converted PCM16, 16kHz, mono buffer
 */
function convertToAssemblyAIFormat(audioBuffer, sourceFormat, targetFormat = ASSEMBLYAI_FORMAT) {
  if (!audioBuffer || audioBuffer.length === 0) {
    throw new Error('Invalid audio buffer');
  }

  let convertedBuffer = audioBuffer;
  let currentFormat = { ...sourceFormat };

  // Step 1: Convert bit depth (32-bit → 16-bit if needed)
  // This must happen first because stereoToMono and resampleAudio expect PCM16
  if (sourceFormat.bitDepth !== 16) {
    convertedBuffer = convertToPCM16(convertedBuffer, {
      bitDepth: sourceFormat.bitDepth,
      isFloat: sourceFormat.isFloat || false
    });
    currentFormat.bitDepth = 16;
    currentFormat.isFloat = false;
  }

  // Step 2: Convert stereo to mono (if needed)
  // This happens before resampling because resampling mono is simpler
  if (sourceFormat.channels > 1) {
    convertedBuffer = stereoToMono(convertedBuffer, sourceFormat.channels);
    currentFormat.channels = 1;
  }

  // Step 3: Resample (48kHz → 16kHz if needed)
  // Resample after mono conversion (buffer is now PCM16 mono)
  if (sourceFormat.sampleRate !== targetFormat.sampleRate) {
    convertedBuffer = resampleAudio(
      convertedBuffer,
      currentFormat.sampleRate, // Use current format (might be mono now)
      targetFormat.sampleRate
    );
    currentFormat.sampleRate = targetFormat.sampleRate;
  }

  return convertedBuffer;
}

module.exports = {
  convertToPCM16,
  resampleAudio,
  stereoToMono,
  calculateChunkSize,
  convertToAssemblyAIFormat
};

