#include <nan.h>
#include <node.h>
#include <v8.h>
#include <uv.h>
#include <CoreAudio/CoreAudio.h>
#include <CoreFoundation/CoreFoundation.h>
#include <Foundation/Foundation.h>
#include <ScreenCaptureKit/ScreenCaptureKit.h>
#include <AVFoundation/AVFoundation.h>
#include <CoreMedia/CoreMedia.h>
#include <AudioToolbox/AudioToolbox.h>
#include <dispatch/dispatch.h>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <vector>

using namespace v8;

// Global variables for ScreenCaptureKit
static SCStream* g_stream = nullptr;
static SCStreamConfiguration* g_config = nullptr;
static SCContentFilter* g_filter = nullptr;
static dispatch_queue_t g_audioQueue = nullptr;
static bool g_isCapturing = false;
static std::string g_audioFilePath = "";

// Global variables for audio file writing
static AudioFileID g_audioFile = nullptr;
static AudioStreamBasicDescription g_audioFormat = {0};
static bool g_audioFileInitialized = false;
static std::string g_outputDirectory = "";
static bool g_formatDetected = false;
static AudioStreamBasicDescription g_sourceFormat = {0};
static AudioStreamBasicDescription g_fileFormat = {0};
// Track the precise moment when the first audio buffer is received (ms since epoch)
static double g_actualStartMs = 0.0;

// Global streaming callback for real-time audio chunks
static Nan::Persistent<v8::Function> g_streamingCallback;
static uv_async_t* g_streamingAsyncHandle = nullptr;

// Structure to pass audio data to async callback
struct StreamingData {
    std::vector<char> audioData;
    double sampleRate;
    int channels;
    int bitDepth;
    bool isFloat;
};

// Async callback handler (runs on main thread)
static void StreamingAsyncCallback(uv_async_t* handle) {
    Nan::HandleScope scope;
    
    // Get the data from the handle
    StreamingData* data = static_cast<StreamingData*>(handle->data);
    if (!data || g_streamingCallback.IsEmpty()) {
        return;
    }
    
    // Create Buffer from audio data
    Local<Object> buffer = Nan::CopyBuffer(data->audioData.data(), data->audioData.size()).ToLocalChecked();
    
    // Create format object
    Local<Object> format = Nan::New<Object>();
    Nan::Set(format, Nan::New("sampleRate").ToLocalChecked(), Nan::New<Number>(data->sampleRate));
    Nan::Set(format, Nan::New("channels").ToLocalChecked(), Nan::New<Integer>(data->channels));
    Nan::Set(format, Nan::New("bitDepth").ToLocalChecked(), Nan::New<Integer>(data->bitDepth));
    Nan::Set(format, Nan::New("isFloat").ToLocalChecked(), Nan::New<v8::Boolean>(data->isFloat));
    
    // Invoke callback
    Local<Function> callback = Nan::New(g_streamingCallback);
    Local<Value> argv[2] = {buffer, format};
    Nan::Call(callback, Nan::GetCurrentContext()->Global(), 2, argv);
    
    // Clean up
    delete data;
    handle->data = nullptr;
}

// Audio conversion function for 32-bit integer to float
static bool convertInt32ToFloat(const char* sourceData, size_t sourceSize, char* destData, size_t& destSize) {
    if (!sourceData || !destData) return false;
    
    // Ensure we have enough space for conversion (same size for 32-bit to 32-bit)
    if (destSize < sourceSize) return false;
    
    const int32_t* intSamples = reinterpret_cast<const int32_t*>(sourceData);
    float* floatSamples = reinterpret_cast<float*>(destData);
    size_t sampleCount = sourceSize / sizeof(int32_t);
    
    // Convert from 32-bit signed integer to 32-bit float
    // Scale by 1.0f / INT32_MAX to normalize to [-1.0, 1.0]
    const float scale = 1.0f / (float)INT32_MAX;
    
    // Debug: log first few samples to check conversion
    static int conversionCount = 0;
    conversionCount++;
    if (conversionCount <= 3) {
        NSLog(@"🎵 [CONVERSION] Sample %d: First few int32 values: %d, %d, %d, %d", 
              conversionCount, intSamples[0], intSamples[1], intSamples[2], intSamples[3]);
        
        // Check the actual range of values
        int32_t minVal = INT32_MAX, maxVal = INT32_MIN;
        for (size_t i = 0; i < sampleCount && i < 1000; i++) {
            if (intSamples[i] < minVal) minVal = intSamples[i];
            if (intSamples[i] > maxVal) maxVal = intSamples[i];
        }
        NSLog(@"🎵 [CONVERSION] Sample %d: Actual range of int32 values: [%d, %d]", 
              conversionCount, minVal, maxVal);
    }
    
    for (size_t i = 0; i < sampleCount; i++) {
        floatSamples[i] = (float)intSamples[i] * scale;
    }
    
    if (conversionCount <= 3) {
        NSLog(@"🎵 [CONVERSION] Sample %d: First few float values: %.6f, %.6f, %.6f, %.6f", 
              conversionCount, floatSamples[0], floatSamples[1], floatSamples[2], floatSamples[3]);
    }
    
    destSize = sampleCount * sizeof(float);
    return true;
}

// Audio conversion function for 32-bit integer to 16-bit integer
static bool interleaveAudio(const char* sourceData, size_t sourceSize, char* destData, size_t& destSize, int channels, int bytesPerSample) {
    if (!sourceData || !destData || channels <= 0) return false;
    
    size_t totalSamples = sourceSize / bytesPerSample;
    size_t samplesPerChannel = totalSamples / channels;
    
    if (destSize < sourceSize) return false;
    
    if (bytesPerSample == 4) { // 32-bit
        const int32_t* source = reinterpret_cast<const int32_t*>(sourceData);
        int32_t* dest = reinterpret_cast<int32_t*>(destData);
        
        // Non-interleaved input: [L0 L1 L2... R0 R1 R2...]
        // Interleaved output: [L0 R0 L1 R1 L2 R2...]
        for (size_t i = 0; i < samplesPerChannel; i++) {
            for (int ch = 0; ch < channels; ch++) {
                dest[i * channels + ch] = source[ch * samplesPerChannel + i];
            }
        }
    } else if (bytesPerSample == 2) { // 16-bit
        const int16_t* source = reinterpret_cast<const int16_t*>(sourceData);
        int16_t* dest = reinterpret_cast<int16_t*>(destData);
        
        for (size_t i = 0; i < samplesPerChannel; i++) {
            for (int ch = 0; ch < channels; ch++) {
                dest[i * channels + ch] = source[ch * samplesPerChannel + i];
            }
        }
    }
    
    destSize = sourceSize;
    return true;
}

static bool convertInt32ToInt16(const char* sourceData, size_t sourceSize, char* destData, size_t& destSize) {
    if (!sourceData || !destData) return false;
    
    const int32_t* int32Samples = reinterpret_cast<const int32_t*>(sourceData);
    int16_t* int16Samples = reinterpret_cast<int16_t*>(destData);
    size_t sampleCount = sourceSize / sizeof(int32_t);
    
    // Calculate required destination size
    size_t requiredDestSize = sampleCount * sizeof(int16_t);
    if (destSize < requiredDestSize) return false;
    
    // Convert from 32-bit signed integer to 16-bit signed integer
    // Use proper scaling to maintain audio quality and reduce noise
    for (size_t i = 0; i < sampleCount; i++) {
        int32_t sample = int32Samples[i];
        
        // Convert to float for proper scaling, then back to int16
        // This provides better quality than simple bit-shifting
        float normalizedSample = (float)sample / (float)INT32_MAX;
        
        // Clamp to [-1.0, 1.0] range
        if (normalizedSample > 1.0f) normalizedSample = 1.0f;
        else if (normalizedSample < -1.0f) normalizedSample = -1.0f;
        
        // Scale to 16-bit range with proper rounding
        int32_t scaledSample = (int32_t)(normalizedSample * (float)INT16_MAX);
        
        // Final clamp to int16 range
        if (scaledSample > INT16_MAX) {
            int16Samples[i] = INT16_MAX;
        } else if (scaledSample < INT16_MIN) {
            int16Samples[i] = INT16_MIN;
        } else {
            int16Samples[i] = (int16_t)scaledSample;
        }
    }
    
    destSize = requiredDestSize;
    return true;
}

// Helper function to initialize audio file - using CAF for playable files with original format!
static bool initializeAudioFile() {
    if (g_audioFileInitialized) {
        return true;
    }
    
    // Create output directory if it doesn't exist
    NSString* outputDir = [NSString stringWithUTF8String:g_outputDirectory.c_str()];
    NSFileManager* fileManager = [NSFileManager defaultManager];
    
    if (![fileManager fileExistsAtPath:outputDir]) {
        NSError* error;
        [fileManager createDirectoryAtPath:outputDir withIntermediateDirectories:YES attributes:nil error:&error];
        if (error) {
            NSLog(@"❌ [NATIVE] Failed to create output directory: %@", error.localizedDescription);
            return false;
        }
    }
    
    // Create unique filename with timestamp - will try .caf first, fallback to .wav
    NSDateFormatter* formatter = [[NSDateFormatter alloc] init];
    [formatter setDateFormat:@"yyyy-MM-dd_HH-mm-ss"];
    NSString* timestamp = [formatter stringFromDate:[NSDate date]];
    NSString* filename = [NSString stringWithFormat:@"system_audio_%@.caf", timestamp];
    NSString* filePath = [outputDir stringByAppendingPathComponent:filename];
    
    // Ensure we have valid format data before creating the file
    if (g_audioFormat.mSampleRate == 0) {
        NSLog(@"❌ [NATIVE] No valid audio format detected yet");
        return false;
    }
    
    NSLog(@"🎵 [NATIVE] Creating CAF file with original format: %.0f Hz, %d channels, %d bits, flags: 0x%x", 
          g_audioFormat.mSampleRate, g_audioFormat.mChannelsPerFrame, g_audioFormat.mBitsPerChannel, g_audioFormat.mFormatFlags);
    
    // Normalize the format for proper file creation
    AudioStreamBasicDescription cafFormat = g_audioFormat;
    
    // CRITICAL: Use exact original format with minimal changes
    NSLog(@"🎵 [NATIVE] Source format: %.0f Hz, %d channels, %d bits, flags: 0x%x", 
          g_audioFormat.mSampleRate, g_audioFormat.mChannelsPerFrame, g_audioFormat.mBitsPerChannel, g_audioFormat.mFormatFlags);
    
    // CRITICAL: Preserve the original sample rate exactly
    double originalSampleRate = g_audioFormat.mSampleRate;
    NSLog(@"🎵 [NATIVE] PRESERVING original sample rate: %.0f Hz", originalSampleRate);
    
    // Set proper format ID and normalize flags for file compatibility
    cafFormat.mFormatID = kAudioFormatLinearPCM;
    
    // For 32-bit audio, convert to 32-bit float format for file compatibility
    // The original format flags 0x29 indicate 32-bit signed integer, which many file formats reject
    if (cafFormat.mBitsPerChannel == 32) {
        // Convert to 32-bit float format for file compatibility
        cafFormat.mBitsPerChannel = 32;
        cafFormat.mFormatFlags = kAudioFormatFlagIsFloat | kAudioFormatFlagIsPacked;
        
        // CRITICAL: Ensure proper byte calculations for 32-bit float
        cafFormat.mBytesPerFrame = cafFormat.mChannelsPerFrame * (cafFormat.mBitsPerChannel / 8); // 2 * 4 = 8
        cafFormat.mFramesPerPacket = 1;
        cafFormat.mBytesPerPacket = cafFormat.mBytesPerFrame * cafFormat.mFramesPerPacket; // 8 * 1 = 8
        
        NSLog(@"🎵 [NATIVE] Fixed byte calculations: BytesPerFrame=%d, FramesPerPacket=%d, BytesPerPacket=%d", 
              cafFormat.mBytesPerFrame, cafFormat.mFramesPerPacket, cafFormat.mBytesPerPacket);
        
        // Ensure no conflicting flags are set
        cafFormat.mFormatFlags &= ~(kAudioFormatFlagIsNonInterleaved);
        
        NSLog(@"🎵 [NATIVE] Converting 32-bit int to 32-bit float for compatibility (flags: 0x%x -> 0x%x)", 
              g_audioFormat.mFormatFlags, cafFormat.mFormatFlags);
        NSLog(@"🎵 [NATIVE] 32-bit float format details: BytesPerFrame=%d, FramesPerPacket=%d, BytesPerPacket=%d, FormatFlags=0x%x", 
              cafFormat.mBytesPerFrame, cafFormat.mFramesPerPacket, cafFormat.mBytesPerPacket, cafFormat.mFormatFlags);
    } else {
        // For non-32-bit audio, normalize the flags first
        if (cafFormat.mFormatFlags & kAudioFormatFlagIsFloat) {
            // Already float, just ensure packed
            cafFormat.mFormatFlags = kAudioFormatFlagIsFloat | kAudioFormatFlagIsPacked;
        } else if (cafFormat.mBitsPerChannel == 24) {
            // 24-bit: use signed integer
            cafFormat.mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
        } else if (cafFormat.mBitsPerChannel == 16) {
            // 16-bit: use signed integer
            cafFormat.mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
        } else {
            // Fallback: use signed integer for other bit depths
            cafFormat.mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
        }
        
        // Remove any non-interleaved flag which can cause issues for non-32-bit formats
        cafFormat.mFormatFlags &= ~kAudioFormatFlagIsNonInterleaved;
        
        // Ensure proper byte calculations for non-32-bit formats
        if (cafFormat.mBytesPerFrame == 0) {
            cafFormat.mBytesPerFrame = cafFormat.mChannelsPerFrame * (cafFormat.mBitsPerChannel / 8);
        }
        if (cafFormat.mFramesPerPacket == 0) {
            cafFormat.mFramesPerPacket = 1;
        }
        if (cafFormat.mBytesPerPacket == 0) {
            cafFormat.mBytesPerPacket = cafFormat.mBytesPerFrame * cafFormat.mFramesPerPacket;
        }
    }
    
    NSLog(@"🎵 [NATIVE] CAF format (minimal changes): %.0f Hz, %d channels, %d bits, flags: 0x%x, BytesPerFrame: %d", 
          cafFormat.mSampleRate, cafFormat.mChannelsPerFrame, cafFormat.mBitsPerChannel, cafFormat.mFormatFlags, cafFormat.mBytesPerFrame);
    
    // DO NOT override sample rate - use the corrected one from g_audioFormat (which may have been updated based on actual data)
    // cafFormat.mSampleRate is already set to g_audioFormat.mSampleRate from line 147
    
    NSLog(@"🎵 [NATIVE] Creating CAF with normalized format: %.0f Hz, %d channels, %d bits, flags: 0x%x", 
          cafFormat.mSampleRate, cafFormat.mChannelsPerFrame, cafFormat.mBitsPerChannel, cafFormat.mFormatFlags);
    
    // Create CAF file with normalized format
    OSStatus status = AudioFileCreateWithURL((__bridge CFURLRef)[NSURL fileURLWithPath:filePath],
                                           kAudioFileCAFType,
                                           &cafFormat,
                                           kAudioFileFlags_EraseFile,
                                           &g_audioFile);
    
    if (status != noErr) {
        // If CAF fails, try WAV as fallback
        NSLog(@"❌ [NATIVE] CAF failed (%d), trying WAV format", (int)status);
        
        // WAV format - normalize format flags for proper file creation
        AudioStreamBasicDescription wavFormat = g_audioFormat;
        wavFormat.mFormatID = kAudioFormatLinearPCM;
        
        // For 32-bit audio in WAV fallback, convert to 32-bit float for compatibility
        if (wavFormat.mBitsPerChannel == 32) {
            // Convert to 32-bit float format for WAV compatibility
            wavFormat.mBitsPerChannel = 32;
            wavFormat.mFormatFlags = kAudioFormatFlagIsFloat | kAudioFormatFlagIsPacked;
            
            // CRITICAL: Ensure proper byte calculations for 32-bit float in WAV
            wavFormat.mBytesPerFrame = wavFormat.mChannelsPerFrame * (wavFormat.mBitsPerChannel / 8); // 2 * 4 = 8
            wavFormat.mFramesPerPacket = 1;
            wavFormat.mBytesPerPacket = wavFormat.mBytesPerFrame * wavFormat.mFramesPerPacket; // 8 * 1 = 8
            
            // Ensure no conflicting flags are set
            wavFormat.mFormatFlags &= ~(kAudioFormatFlagIsNonInterleaved);
            
            NSLog(@"🎵 [NATIVE] Converting 32-bit int to 32-bit float for WAV compatibility (flags: 0x%x -> 0x%x)", 
                  g_audioFormat.mFormatFlags, wavFormat.mFormatFlags);
            NSLog(@"🎵 [NATIVE] WAV 32-bit float format details: BytesPerFrame=%d, FramesPerPacket=%d, BytesPerPacket=%d, FormatFlags=0x%x", 
                  wavFormat.mBytesPerFrame, wavFormat.mFramesPerPacket, wavFormat.mBytesPerPacket, wavFormat.mFormatFlags);
        } else {
            // For non-32-bit audio, normalize the flags first
            if (wavFormat.mFormatFlags & kAudioFormatFlagIsFloat) {
                // Already float, just ensure packed
                wavFormat.mFormatFlags = kAudioFormatFlagIsFloat | kAudioFormatFlagIsPacked;
            } else if (wavFormat.mBitsPerChannel == 24) {
                // 24-bit: use signed integer
                wavFormat.mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
            } else if (wavFormat.mBitsPerChannel == 16) {
                // 16-bit: use signed integer
                wavFormat.mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
            } else {
                // Fallback: use signed integer for other bit depths
                wavFormat.mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
            }
            
            // Remove any non-interleaved flag which can cause issues with WAV for non-32-bit formats
            wavFormat.mFormatFlags &= ~kAudioFormatFlagIsNonInterleaved;
            
            // Ensure proper byte calculations for non-32-bit formats in WAV
            if (wavFormat.mBytesPerFrame == 0) {
                wavFormat.mBytesPerFrame = wavFormat.mChannelsPerFrame * (wavFormat.mBitsPerChannel / 8);
            }
            if (wavFormat.mFramesPerPacket == 0) {
                wavFormat.mFramesPerPacket = 1;
            }
            if (wavFormat.mBytesPerPacket == 0) {
                wavFormat.mBytesPerPacket = wavFormat.mBytesPerFrame * wavFormat.mFramesPerPacket;
            }
        }
        
        // DO NOT override sample rate - use the corrected one from g_audioFormat (which may have been updated based on actual data)
        // wavFormat.mSampleRate is already set to g_audioFormat.mSampleRate from line 245
        
        NSLog(@"🎵 [NATIVE] WAV fallback normalized format: %.0f Hz, %d channels, %d bits, flags: 0x%x", 
              wavFormat.mSampleRate, wavFormat.mChannelsPerFrame, wavFormat.mBitsPerChannel, wavFormat.mFormatFlags);
        
        // Update filename to .wav
        NSString* wavFilePath = [filePath stringByReplacingOccurrencesOfString:@".caf" withString:@".wav"];
        
        status = AudioFileCreateWithURL((__bridge CFURLRef)[NSURL fileURLWithPath:wavFilePath],
                                      kAudioFileWAVEType,
                                      &wavFormat,
                                      kAudioFileFlags_EraseFile,
                                      &g_audioFile);
        
        if (status == noErr) {
            // Update the file path to the new WAV file
            g_audioFilePath = [wavFilePath UTF8String];
            NSLog(@"✅ [NATIVE] Created WAV fallback file: %@", wavFilePath);
        } else {
            NSLog(@"❌ [NATIVE] Both CAF and WAV failed: %d", (int)status);
            return false;
        }
    } else {
        // CAF succeeded, update file path
        g_audioFilePath = [filePath UTF8String];
        NSLog(@"✅ [NATIVE] CAF audio file created: %@", filePath);
    }
    
    g_audioFileInitialized = true;
    
    return true;
}

// Helper function to close audio file
static void closeAudioFile() {
    if (g_audioFile) {
        AudioFileClose(g_audioFile);
        g_audioFile = nullptr;
    }
    
    g_audioFileInitialized = false;
    g_formatDetected = false;  // Reset format detection for next recording
    
    // Reset format structures
    memset(&g_sourceFormat, 0, sizeof(AudioStreamBasicDescription));
    memset(&g_fileFormat, 0, sizeof(AudioStreamBasicDescription));
    memset(&g_audioFormat, 0, sizeof(AudioStreamBasicDescription));
    
    NSLog(@"✅ [NATIVE] Audio file closed: %s", g_audioFilePath.c_str());
}

// Audio callback function
static void audioCallback(CMSampleBufferRef sampleBuffer) {
    static int callbackCount = 0;
    callbackCount++;
    
    // Only log every 100th callback to reduce noise
    if (callbackCount % 100 == 1) {
        NSLog(@"🎤 [NATIVE] Audio callback #%d received", callbackCount);
    }
    
    // Initialize audio file only once on first callback, but after format detection
    if (!g_audioFileInitialized) {
        NSLog(@"🎤 [NATIVE] First callback - detecting audio format...");
        
        // First, detect the format from this sample
        CMFormatDescriptionRef formatDesc = CMSampleBufferGetFormatDescription(sampleBuffer);
        if (formatDesc) {
            const AudioStreamBasicDescription* sourceFormat = CMAudioFormatDescriptionGetStreamBasicDescription(formatDesc);
            if (sourceFormat) {
                // Store the source format and use it directly - no conversion!
                g_sourceFormat = *sourceFormat;
                g_fileFormat = *sourceFormat;  // Same as source - no conversion
                g_audioFormat = *sourceFormat; // Use original format directly
                
                NSLog(@"🎵 [NATIVE] ✅ DETECTED FORMAT: %.0f Hz, %d channels, %d bits, flags: 0x%x", 
                      g_audioFormat.mSampleRate, g_audioFormat.mChannelsPerFrame, g_audioFormat.mBitsPerChannel, g_audioFormat.mFormatFlags);
                NSLog(@"🎵 [NATIVE] Original byte calculations: BytesPerFrame=%d, FramesPerPacket=%d, BytesPerPacket=%d", 
                      g_audioFormat.mBytesPerFrame, g_audioFormat.mFramesPerPacket, g_audioFormat.mBytesPerPacket);
                
                // CRITICAL FIX: ScreenCaptureKit sometimes reports wrong byte calculations for 32-bit audio
                // Override with correct calculations for 32-bit stereo
                if (g_audioFormat.mBitsPerChannel == 32 && g_audioFormat.mChannelsPerFrame == 2) {
                    g_audioFormat.mBytesPerFrame = 8;  // 2 channels * 4 bytes = 8 bytes per frame
                    g_audioFormat.mFramesPerPacket = 1;
                    g_audioFormat.mBytesPerPacket = 8; // 8 bytes per frame * 1 frame per packet = 8 bytes per packet
                    NSLog(@"🎵 [NATIVE] FIXED byte calculations for 32-bit stereo: BytesPerFrame=%d, FramesPerPacket=%d, BytesPerPacket=%d", 
                          g_audioFormat.mBytesPerFrame, g_audioFormat.mFramesPerPacket, g_audioFormat.mBytesPerPacket);
                }
                
                NSLog(@"🎵 [NATIVE] CRITICAL: Sample rate being preserved as %.0f Hz", g_audioFormat.mSampleRate);
                
                // Also log timing information
                CMTime presentationTime = CMSampleBufferGetPresentationTimeStamp(sampleBuffer);
                CMTime duration = CMSampleBufferGetDuration(sampleBuffer);
                NSLog(@"🎵 [NATIVE] First sample timing: Presentation=%.3f, Duration=%.6f", 
                      CMTimeGetSeconds(presentationTime), CMTimeGetSeconds(duration));
                
                // === TIMING ANALYSIS DIAGNOSTIC TEST ===
                // This is the key diagnostic from the other Claude's analysis!
                CMBlockBufferRef blockBuffer = CMSampleBufferGetDataBuffer(sampleBuffer);
                if (blockBuffer) {
                    size_t dataLength = CMBlockBufferGetDataLength(blockBuffer);
                    double durationSeconds = CMTimeGetSeconds(duration);
                    
                    // Calculate actual sample rate from the actual data
                    // For stereo audio: 7680 bytes / 4 bytes per sample = 1920 samples
                    // 1920 samples / 0.020 seconds = 96000 Hz
                    size_t bytesPerSample = 4; // 32-bit
                    size_t numSamples = dataLength / bytesPerSample;
                    double calculatedRate = numSamples / durationSeconds;
                    
                    NSLog(@"=== TIMING ANALYSIS ===");
                    NSLog(@"Data length: %zu bytes", dataLength);
                    NSLog(@"Duration: %.6f seconds", durationSeconds);
                    NSLog(@"Samples: %zu (not frames!)", numSamples);
                    NSLog(@"Calculated sample rate: %.2f Hz", calculatedRate);
                    NSLog(@"Format says: %.0f Hz", g_audioFormat.mSampleRate);
                    NSLog(@"Ratio: %.4f", calculatedRate / g_audioFormat.mSampleRate);
                    NSLog(@"======================");
                    
                    // DO NOT update the sample rate - the reported rate is correct!
                    // The calculated rate is per-sample (not per-channel), so it's double the actual rate
                    NSLog(@"ℹ️ [NATIVE] Sample rate is correct: %.0f Hz (calculated %.2f Hz includes all channels)", 
                          g_audioFormat.mSampleRate, calculatedRate);
                }
                
                g_formatDetected = true;
            } else {
                NSLog(@"❌ [NATIVE] Failed to get stream description from format descriptor");
            }
        } else {
            NSLog(@"❌ [NATIVE] Failed to get format description from sample buffer");
        }
        
        if (!initializeAudioFile()) {
            NSLog(@"❌ [NATIVE] Failed to initialize audio file");
            return;
        }
        NSLog(@"🎤 [NATIVE] Audio file initialized, starting continuous recording");
    }
    
    // Get audio buffer list from sample buffer
    CMBlockBufferRef blockBuffer = CMSampleBufferGetDataBuffer(sampleBuffer);
    if (!blockBuffer) {
        if (callbackCount <= 5) {
            NSLog(@"❌ [NATIVE] No data buffer in sample buffer");
        }
        return;
    }
    
    size_t dataLength = CMBlockBufferGetDataLength(blockBuffer);
    if (dataLength == 0) {
        if (callbackCount <= 5) {
            NSLog(@"🔇 [NATIVE] Empty audio sample buffer");
        }
        return; // No audio data
    }
    
    
    // Log audio format info for debugging (only first few callbacks)
    if (callbackCount <= 5) {
        NSLog(@"🎵 [NATIVE] Audio data length: %zu bytes", dataLength);
    }
    
    // Get audio data
    char* sourceAudioData = nullptr;
    OSStatus status = CMBlockBufferGetDataPointer(blockBuffer, 0, nullptr, nullptr, &sourceAudioData);
    if (status != noErr || !sourceAudioData) {
        if (callbackCount <= 5) {
            NSLog(@"❌ [NATIVE] Failed to get audio data pointer: %d", (int)status);
        }
        return;
    }
    
    // Check if source format is non-interleaved
    bool isNonInterleaved = (g_sourceFormat.mFormatFlags & kAudioFormatFlagIsNonInterleaved) != 0;
    bool is32BitInt = (g_sourceFormat.mBitsPerChannel == 32) && 
                      (g_sourceFormat.mFormatFlags & kAudioFormatFlagIsSignedInteger);
    
    const char* audioDataToWrite = sourceAudioData;
    UInt32 bytesToWrite = (UInt32)dataLength;
    static char conversionBuffer[1024 * 1024]; // 1MB buffer
    
    if (callbackCount <= 5) {
        NSLog(@"🔍 [PROCESS] NonInterleaved=%d, 32BitInt=%d", isNonInterleaved, is32BitInt);
    }
    
    // STEP 1: Interleave if needed
    if (isNonInterleaved) {
        size_t bufferSize = sizeof(conversionBuffer);
        if (interleaveAudio(sourceAudioData, dataLength, conversionBuffer, bufferSize, 
                           g_sourceFormat.mChannelsPerFrame, 4)) {
            audioDataToWrite = conversionBuffer;
            bytesToWrite = (UInt32)bufferSize;
            if (callbackCount <= 5) {
                NSLog(@"✅ [INTERLEAVE] Converted non-interleaved to interleaved");
            }
        } else {
            NSLog(@"❌ [INTERLEAVE] Failed to interleave audio");
            return;
        }
    }
    
    // STEP 2: Convert 32-bit int to 32-bit float if needed
    if (is32BitInt) {
        static char floatBuffer[1024 * 1024];
        size_t floatBufferSize = sizeof(floatBuffer);
        if (convertInt32ToFloat(audioDataToWrite, bytesToWrite, floatBuffer, floatBufferSize)) {
            audioDataToWrite = floatBuffer;
            bytesToWrite = (UInt32)floatBufferSize;
            if (callbackCount <= 5) {
                NSLog(@"✅ [CONVERT] Converted 32-bit int to 32-bit float");
            }
        } else {
            NSLog(@"❌ [CONVERT] Failed to convert to float");
            return;
        }
    }
    
    if (callbackCount <= 5) {
        NSLog(@"📝 [WRITE] Writing %u bytes to file", bytesToWrite);
    }
    
    // Write audio data to file
    if (g_audioFile) {
        // Get current file position for proper sequential writing
        SInt64 filePosition = 0;
        UInt32 size = sizeof(filePosition);
        OSStatus status = AudioFileGetProperty(g_audioFile, kAudioFilePropertyAudioDataByteCount, &size, &filePosition);
        if (status != noErr) {
            NSLog(@"❌ [NATIVE] Failed to get audio file position: %d", (int)status);
            return;
        }
        
        // Write the audio data (converted if necessary)
        status = AudioFileWriteBytes(g_audioFile, false, filePosition, &bytesToWrite, audioDataToWrite);
        
        if (status != noErr) {
            // Get file format for debugging
            AudioStreamBasicDescription fileFormat;
            UInt32 formatSize = sizeof(fileFormat);
            AudioFileGetProperty(g_audioFile, kAudioFilePropertyDataFormat, &formatSize, &fileFormat);
            NSLog(@"❌ [NATIVE] Failed to write audio bytes: %d (bytes: %u, format: %.0fHz, %dch, %d bits)", (int)status, bytesToWrite, fileFormat.mSampleRate, fileFormat.mChannelsPerFrame, fileFormat.mBitsPerChannel);
            return;
        }
        
        // Log writing progress for first few samples only
        if (callbackCount <= 5) {
            // Get current file format for logging
            AudioStreamBasicDescription fileFormat;
            UInt32 formatSize = sizeof(fileFormat);
            AudioFileGetProperty(g_audioFile, kAudioFilePropertyDataFormat, &formatSize, &fileFormat);
            NSLog(@"🎵 [NATIVE] Wrote %u bytes to file - Format: %.0fHz, %dch, %d bits", bytesToWrite, fileFormat.mSampleRate, fileFormat.mChannelsPerFrame, fileFormat.mBitsPerChannel);
        }
    } else {
        NSLog(@"❌ [NATIVE] Audio file not initialized");
        return;
    }
    
    // Log progress every 1000 samples (less verbose)
    static int sampleCount = 0;
    if (++sampleCount % 1000 == 0) {
        NSLog(@"🎤 [NATIVE] Recording... (%d samples written)", sampleCount);
    }
    
    // Invoke streaming callback if set (for real-time streaming)
    if (!g_streamingCallback.IsEmpty() && g_streamingAsyncHandle) {
        // Create data structure with audio data
        StreamingData* data = new StreamingData();
        data->audioData.assign(audioDataToWrite, audioDataToWrite + bytesToWrite);
        data->sampleRate = g_audioFormat.mSampleRate;
        data->channels = g_audioFormat.mChannelsPerFrame;
        data->bitDepth = g_audioFormat.mBitsPerChannel;
        data->isFloat = (g_audioFormat.mFormatFlags & kAudioFormatFlagIsFloat) != 0;
        
        // Set data and trigger async callback (will run on main thread)
        g_streamingAsyncHandle->data = data;
        uv_async_send(g_streamingAsyncHandle);
        
        // Log first few invocations for debugging
        if (callbackCount <= 5) {
            NSLog(@"📡 [NATIVE] Streaming callback queued: %u bytes, %.0fHz, %dch, %dbit, float=%d", 
                  bytesToWrite, g_audioFormat.mSampleRate, g_audioFormat.mChannelsPerFrame, 
                  g_audioFormat.mBitsPerChannel, data->isFloat ? 1 : 0);
        }
    }
}

// ScreenCaptureKit delegate
@interface AudioCaptureDelegate : NSObject <SCStreamDelegate, SCStreamOutput>
@property (nonatomic, copy) void (^audioCallback)(CMSampleBufferRef);
@end

@implementation AudioCaptureDelegate

- (void)stream:(SCStream *)stream 
    didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer 
    ofType:(SCStreamOutputType)type {
    static int delegateCallCount = 0;
    delegateCallCount++;
    
    // Only log every 100th call to reduce noise
    if (delegateCallCount % 100 == 1) {
        NSLog(@"🎤 [NATIVE] Delegate received sample buffer #%d of type: %ld", delegateCallCount, (long)type);
    }
    
    if (type == SCStreamOutputTypeAudio) {
        // Record actual start time at the first audio buffer
        if (g_actualStartMs == 0.0) {
            NSTimeInterval nowMs = [[NSDate date] timeIntervalSince1970] * 1000.0;
            g_actualStartMs = nowMs;
            NSLog(@"🎤 [NATIVE] ✅ actualStartMs set: %.0f", g_actualStartMs);
        }
        if (delegateCallCount % 100 == 1) {
            NSLog(@"🎤 [NATIVE] Processing audio sample buffer #%d", delegateCallCount);
        }
        if (self.audioCallback) {
            self.audioCallback(sampleBuffer);
        }
    } else {
        // Non-audio buffers are less common, so log them all
        NSLog(@"🎤 [NATIVE] Received non-audio sample buffer (type: %ld)", (long)type);
    }
}

- (void)stream:(SCStream *)stream didStopWithError:(NSError *)error {
    if (error) {
        NSLog(@"❌ [NATIVE] Stream stopped with error: %@", error.localizedDescription);
    } else {
        NSLog(@"✅ [NATIVE] Stream stopped successfully");
    }
    g_isCapturing = false;
}

@end

static AudioCaptureDelegate* g_delegate = nil;

// Initialize the native module
NAN_METHOD(Initialize) {
    NSLog(@"🎤 [NATIVE] Initializing native audio module");
    
    // Create audio processing queue
    g_audioQueue = dispatch_queue_create("com.sayso.audio", DISPATCH_QUEUE_SERIAL);
    
    // Create delegate
    g_delegate = [[AudioCaptureDelegate alloc] init];
    
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(true));
}

// Request screen recording permission
NAN_METHOD(RequestScreenRecordingPermission) {
    NSLog(@"🎤 [NATIVE] Requesting screen recording permission");
    
    // Check current permission status by trying to get shareable content
    [SCShareableContent getShareableContentWithCompletionHandler:^(SCShareableContent *content, NSError *error) {
        if (error) {
            NSLog(@"❌ [NATIVE] Permission check failed: %@", error.localizedDescription);
            // Return false to JavaScript
            return;
        }
        
        NSLog(@"✅ [NATIVE] Screen recording permission granted");
        // Return true to JavaScript
    }];
    
    // For now, return true - the actual permission check happens asynchronously
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(true));
}

// Start system audio capture
NAN_METHOD(StartSystemAudioCapture) {
    NSLog(@"🎤 [NATIVE] Starting system audio capture");
    
    if (g_isCapturing) {
        NSLog(@"⚠️ [NATIVE] System audio capture already active");
        info.GetReturnValue().Set(Nan::New<v8::Boolean>(false));
        return;
    }
    
    // Reset actual start timestamp
    g_actualStartMs = 0.0;

    // Set output directory
    // Dev: keep existing project directory behavior
    // Prod: unify with mic path at ~/Library/Application Support/sayso-app/temp/full_recordings
    std::string currentPath = std::string([[[NSBundle mainBundle] bundlePath] UTF8String]);
    size_t electronPos = currentPath.find("/node_modules/electron/dist/Electron.app");
    if (electronPos != std::string::npos) {
        // Development mode - use project directory (unchanged)
        currentPath = currentPath.substr(0, electronPos);
        g_outputDirectory = currentPath + "/electron/full_recordings";
        NSLog(@"🎤 [NATIVE] Development mode - using project directory: %s", g_outputDirectory.c_str());
    } else {
        // Production mode - use unified temp path under sayso-app
        NSArray* paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
        NSString* appSupportDir = [paths firstObject];
        NSString* appDir = [appSupportDir stringByAppendingPathComponent:@"sayso-app"]; // match mic path app name
        NSString* tempFullRecordingsDir = [appDir stringByAppendingPathComponent:@"temp/full_recordings"];
        g_outputDirectory = std::string([tempFullRecordingsDir UTF8String]);
        NSLog(@"🎤 [NATIVE] Production mode - unified output directory: %s", g_outputDirectory.c_str());
    }
    
    NSLog(@"🎤 [NATIVE] Final output directory: %s", g_outputDirectory.c_str());
    
    // Get shareable content
    [SCShareableContent getShareableContentWithCompletionHandler:^(SCShareableContent *content, NSError *error) {
        if (error) {
            NSLog(@"❌ [NATIVE] Failed to get shareable content: %@", error.localizedDescription);
            return;
        }
        
        if (content.displays.count == 0) {
            NSLog(@"❌ [NATIVE] No displays available");
            return;
        }
        
        // Get the main display
        SCDisplay *display = content.displays.firstObject;
        NSLog(@"🎤 [NATIVE] Using display: %u", display.displayID);
        
                    // Create content filter for system audio (not display)
        g_filter = [[SCContentFilter alloc] initWithDisplay:display excludingWindows:@[]];
        
                    // Create stream configuration with optimal audio settings
        g_config = [[SCStreamConfiguration alloc] init];
        g_config.capturesAudio = YES;
        g_config.excludesCurrentProcessAudio = YES;
                    
                    // Let ScreenCaptureKit use the actual system audio rate - don't force any rate
                    // This avoids sample rate conversion and pitch issues
                    g_config.channelCount = 2;    // Stereo
                    
                    NSLog(@"🎤 [NATIVE] Stream configuration: Audio=YES, Channels=%ld (system will decide sample rate)", (long)g_config.channelCount);
                    
                    // Optimize for audio processing (reduce frame rate since we only need audio)
                    g_config.minimumFrameInterval = CMTimeMake(1, 60); // 60 FPS minimum but we'll ignore video
                    g_config.queueDepth = 10;     // Larger buffer for smoother audio
                    
                    // Additional audio-specific optimizations
                    g_config.capturesAudio = YES;
                    g_config.excludesCurrentProcessAudio = YES;
        
        NSLog(@"🎤 [NATIVE] Stream configuration: Audio=%@, SampleRate=%ld, Channels=%ld", 
              g_config.capturesAudio ? @"YES" : @"NO", 
              (long)g_config.sampleRate, 
              (long)g_config.channelCount);
        
        // Create stream
        g_stream = [[SCStream alloc] initWithFilter:g_filter 
                                       configuration:g_config 
                                             delegate:g_delegate];
        
        // Set up audio callback
        g_delegate.audioCallback = ^(CMSampleBufferRef sampleBuffer) {
            audioCallback(sampleBuffer);
        };
        
                    // Add stream output with correct method signature (includes error parameter)
                    NSError *streamError = nil;
                    BOOL success = [g_stream addStreamOutput:g_delegate 
                             type:SCStreamOutputTypeAudio 
                                          sampleHandlerQueue:g_audioQueue 
                                                       error:&streamError];
                    
                    if (success) {
                        NSLog(@"✅ [NATIVE] Stream output added successfully");
                    } else {
                        NSLog(@"❌ [NATIVE] Failed to add stream output: %@", streamError.localizedDescription);
                        // Continue anyway - we'll start the stream and see what happens
                    }
        
        // Start capture
        [g_stream startCaptureWithCompletionHandler:^(NSError *error) {
            if (error) {
                NSLog(@"❌ [NATIVE] Failed to start capture: %@", error.localizedDescription);
                g_isCapturing = false;
            } else {
                NSLog(@"✅ [NATIVE] System audio capture started successfully");
                g_isCapturing = true;
            }
        }];
    }];
    
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(true));
}

// Stop system audio capture
NAN_METHOD(StopSystemAudioCapture) {
    NSLog(@"🎤 [NATIVE] Stopping system audio capture");
    
    if (!g_isCapturing || !g_stream) {
        NSLog(@"⚠️ [NATIVE] System audio capture not active");
        Local<Object> result = Nan::New<Object>();
        Nan::Set(result, Nan::New("success").ToLocalChecked(), Nan::New<v8::Boolean>(false));
        Nan::Set(result, Nan::New("filePath").ToLocalChecked(), Nan::Null());
        info.GetReturnValue().Set(result);
        return;
    }
    
    // Save file path before closing (g_audioFilePath is set during recording)
    std::string savedFilePath = g_audioFilePath;
    bool hasFilePath = !savedFilePath.empty();
    
    [g_stream stopCaptureWithCompletionHandler:^(NSError *error) {
        if (error) {
            NSLog(@"❌ [NATIVE] Failed to stop capture: %@", error.localizedDescription);
        } else {
            NSLog(@"✅ [NATIVE] System audio capture stopped successfully");
        }
        g_isCapturing = false;
        
    // Close audio file when stopping capture
    closeAudioFile();
    
    // Clear streaming callback when stopping
    g_streamingCallback.Reset();
    
    // Clean up async handle
    if (g_streamingAsyncHandle) {
        uv_async_t* handleToDelete = g_streamingAsyncHandle;
        g_streamingAsyncHandle = nullptr;
        uv_close((uv_handle_t*)handleToDelete, [](uv_handle_t* handle) {
            delete reinterpret_cast<uv_async_t*>(handle);
        });
    }
    }];
    
    // Return result object with success, file path and actual start time
    Local<Object> result = Nan::New<Object>();
    Nan::Set(result, Nan::New("success").ToLocalChecked(), Nan::New<v8::Boolean>(true));
    
    if (hasFilePath) {
        Nan::Set(result, Nan::New("filePath").ToLocalChecked(), 
                Nan::New<String>(savedFilePath.c_str()).ToLocalChecked());
        NSLog(@"✅ [NATIVE] Returning file path: %s", savedFilePath.c_str());
    } else {
        Nan::Set(result, Nan::New("filePath").ToLocalChecked(), Nan::Null());
        NSLog(@"⚠️ [NATIVE] No file path available");
    }
    // Include actualStartMs if available
    if (g_actualStartMs > 0.0) {
        Nan::Set(result, Nan::New("actualStartMs").ToLocalChecked(), Nan::New<Number>(g_actualStartMs));
    } else {
        Nan::Set(result, Nan::New("actualStartMs").ToLocalChecked(), Nan::Null());
    }
    
    info.GetReturnValue().Set(result);
}

// Set streaming callback for real-time audio chunks
NAN_METHOD(SetStreamingCallback) {
    if (info.Length() < 1 || info[0]->IsNull() || info[0]->IsUndefined()) {
        // Clear callback
        g_streamingCallback.Reset();
        
        // Clean up async handle
        if (g_streamingAsyncHandle) {
            uv_async_t* handleToDelete = g_streamingAsyncHandle;
            g_streamingAsyncHandle = nullptr;
            uv_close((uv_handle_t*)handleToDelete, [](uv_handle_t* handle) {
                delete reinterpret_cast<uv_async_t*>(handle);
            });
        }
        
        NSLog(@"🎤 [NATIVE] Streaming callback cleared");
        info.GetReturnValue().Set(Nan::New<v8::Boolean>(true));
        return;
    }
    
    if (!info[0]->IsFunction()) {
        Nan::ThrowTypeError("Callback must be a function");
        return;
    }
    
    // Store persistent reference to callback
    g_streamingCallback.Reset(Nan::To<Function>(info[0]).ToLocalChecked());
    
    // Create async handle if it doesn't exist
    if (!g_streamingAsyncHandle) {
        g_streamingAsyncHandle = new uv_async_t();
        uv_async_init(uv_default_loop(), g_streamingAsyncHandle, StreamingAsyncCallback);
    }
    
    NSLog(@"🎤 [NATIVE] Streaming callback set");
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(true));
}

// Check if system audio capture is active
NAN_METHOD(IsSystemAudioCaptureActive) {
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(g_isCapturing));
}

// List output devices (CoreAudio implementation)
NAN_METHOD(ListOutputDevices) {
    NSLog(@"🎤 [NATIVE] Listing output devices");
    
    AudioObjectPropertyAddress propertyAddress = {
        kAudioHardwarePropertyDevices,
        kAudioObjectPropertyScopeGlobal,
        kAudioObjectPropertyElementMain
    };
    
    UInt32 dataSize = 0;
    OSStatus status = AudioObjectGetPropertyDataSize(kAudioObjectSystemObject, 
                                                    &propertyAddress, 
                                                    0, 
                                                    NULL, 
                                                    &dataSize);
    
    if (status != noErr) {
        NSLog(@"❌ [NATIVE] Failed to get device list size: %d", status);
        info.GetReturnValue().Set(Nan::New<Array>(0));
        return;
    }
    
    UInt32 deviceCount = dataSize / sizeof(AudioDeviceID);
    AudioDeviceID* devices = new AudioDeviceID[deviceCount];
    
    status = AudioObjectGetPropertyData(kAudioObjectSystemObject, 
                                      &propertyAddress, 
                                      0, 
                                      NULL, 
                                      &dataSize, 
                                      devices);
    
    if (status != noErr) {
        NSLog(@"❌ [NATIVE] Failed to get device list: %d", status);
        delete[] devices;
        info.GetReturnValue().Set(Nan::New<Array>(0));
        return;
    }
    
    Local<Array> result = Nan::New<Array>();
    UInt32 outputDeviceCount = 0;
    
    for (UInt32 i = 0; i < deviceCount; i++) {
        AudioDeviceID deviceID = devices[i];
        
        // Check if it's an output device
        propertyAddress.mScope = kAudioDevicePropertyScopeOutput;
        propertyAddress.mSelector = kAudioDevicePropertyStreams;
        
        dataSize = 0;
        status = AudioObjectGetPropertyDataSize(deviceID, 
                                              &propertyAddress, 
                                              0, 
                                              NULL, 
                                              &dataSize);
        
        if (status == noErr && dataSize > 0) {
            // Get device name
            propertyAddress.mSelector = kAudioDevicePropertyDeviceNameCFString;
            propertyAddress.mScope = kAudioObjectPropertyScopeGlobal;
            
            CFStringRef deviceName = NULL;
            dataSize = sizeof(deviceName);
            status = AudioObjectGetPropertyData(deviceID, 
                                              &propertyAddress, 
                                              0, 
                                              NULL, 
                                              &dataSize, 
                                              &deviceName);
            
            if (status == noErr && deviceName) {
                char nameBuffer[256];
                CFStringGetCString(deviceName, nameBuffer, sizeof(nameBuffer), kCFStringEncodingUTF8);
                
                Local<Object> device = Nan::New<Object>();
                device->Set(Nan::GetCurrentContext(), Nan::New("id").ToLocalChecked(), Nan::New<Number>(deviceID));
                device->Set(Nan::GetCurrentContext(), Nan::New("name").ToLocalChecked(), Nan::New(nameBuffer).ToLocalChecked());
                
                result->Set(Nan::GetCurrentContext(), outputDeviceCount++, device);
                
                CFRelease(deviceName);
            }
        }
    }
    
    delete[] devices;
    info.GetReturnValue().Set(result);
}

// Create multi-output device (CoreAudio implementation)
NAN_METHOD(CreateMultiOutputDevice) {
    if (info.Length() < 2) {
        Nan::ThrowError("Expected 2 arguments: name and subDevices");
        return;
    }
    
    String::Utf8Value name(Nan::GetCurrentContext()->GetIsolate(), info[0]);
    Local<Array> subDevices = Local<Array>::Cast(info[1]);
    
    NSLog(@"🎤 [NATIVE] Creating multi-output device: %s", *name);
    
    // Create aggregate device description
    CFMutableDictionaryRef description = CFDictionaryCreateMutable(NULL, 0, 
                                                                &kCFTypeDictionaryKeyCallBacks, 
                                                                &kCFTypeDictionaryValueCallBacks);
    
    // Set device name
    CFStringRef deviceName = CFStringCreateWithCString(NULL, *name, kCFStringEncodingUTF8);
    CFDictionarySetValue(description, CFSTR(kAudioAggregateDeviceNameKey), deviceName);
    
    // Set sub-devices
    CFMutableArrayRef subDevicesArray = CFArrayCreateMutable(NULL, 0, &kCFTypeArrayCallBacks);
    
    for (uint32_t i = 0; i < subDevices->Length(); i++) {
        Local<Value> subDevice = subDevices->Get(Nan::GetCurrentContext(), i).ToLocalChecked();
        if (subDevice->IsString()) {
            String::Utf8Value subDeviceName(Nan::GetCurrentContext()->GetIsolate(), subDevice);
            CFStringRef subDeviceCFString = CFStringCreateWithCString(NULL, *subDeviceName, kCFStringEncodingUTF8);
            CFArrayAppendValue(subDevicesArray, subDeviceCFString);
            CFRelease(subDeviceCFString);
        }
    }
    
    CFDictionarySetValue(description, CFSTR(kAudioAggregateDeviceSubDeviceListKey), subDevicesArray);
    
    // Create the aggregate device
    AudioDeviceID aggregateDeviceID;
    OSStatus status = AudioHardwareCreateAggregateDevice(description, &aggregateDeviceID);
    
    // Clean up
    CFRelease(subDevicesArray);
    CFRelease(deviceName);
    CFRelease(description);
    
    if (status != noErr) {
        NSLog(@"❌ [NATIVE] Failed to create aggregate device: %d", status);
        info.GetReturnValue().Set(Nan::New<v8::Boolean>(false));
        return;
    }
    
    NSLog(@"✅ [NATIVE] Created aggregate device with ID: %u", aggregateDeviceID);
    info.GetReturnValue().Set(Nan::New<Number>(aggregateDeviceID));
}

// Delete multi-output device
NAN_METHOD(DeleteMultiOutputDevice) {
    if (info.Length() < 1) {
        Nan::ThrowError("Expected 1 argument: deviceID");
        return;
    }
    
    AudioDeviceID deviceID = info[0]->Uint32Value(Nan::GetCurrentContext()).FromJust();
    NSLog(@"🎤 [NATIVE] Deleting multi-output device: %u", deviceID);
    
    OSStatus status = AudioHardwareDestroyAggregateDevice(deviceID);
    
    if (status != noErr) {
        NSLog(@"❌ [NATIVE] Failed to delete aggregate device: %d", status);
        info.GetReturnValue().Set(Nan::New<v8::Boolean>(false));
        return;
    }
    
    NSLog(@"✅ [NATIVE] Deleted aggregate device successfully");
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(true));
}

// Module initialization
NAN_MODULE_INIT(Init) {
    Nan::Set(target, Nan::New("initialize").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(Initialize)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("requestScreenRecordingPermission").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(RequestScreenRecordingPermission)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("startSystemAudioCapture").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(StartSystemAudioCapture)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("stopSystemAudioCapture").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(StopSystemAudioCapture)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("setStreamingCallback").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(SetStreamingCallback)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("isSystemAudioCaptureActive").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(IsSystemAudioCaptureActive)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("listOutputDevices").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(ListOutputDevices)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("createMultiOutputDevice").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(CreateMultiOutputDevice)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("deleteMultiOutputDevice").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(DeleteMultiOutputDevice)).ToLocalChecked());
}

NODE_MODULE(native_audio, Init)