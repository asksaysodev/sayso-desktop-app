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
#include <atomic>
#include <mutex>
#include <cstdlib>
#include <cstdio>
#include <list>
#include <vector>
#include <string>
#include <unistd.h>

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

// Global flag to skip file creation when streaming-only
static bool g_streamingOnly = false;

// Global streaming callback for real-time audio chunks
static Nan::Persistent<v8::Function> g_streamingCallback;
static uv_async_t* g_streamingAsyncHandle = nullptr;

// Separate callback for microphone audio
static Nan::Persistent<v8::Function> g_micStreamingCallback;
static uv_async_t* g_micStreamingAsyncHandle = nullptr;

// Native→JS lifecycle diagnostics channel (SAYSO-355; also the transport for SAYSO-353's mic
// route events). Deliberately NOT the handle->data pattern: producers append strings to a
// mutex-guarded queue from any thread and the JS-thread drain swaps it out, so there is no
// ownership race. The uv_async_t is allocated once and never freed (uv handles are cheap;
// freeing on callback-clear is the exact UAF class SAYSO-349 documents).
static std::mutex g_lifecycleMutex;
static std::vector<std::string> g_lifecycleQueue;    // guarded by g_lifecycleMutex
static uv_async_t* g_lifecycleAsyncHandle = nullptr; // allocated once, never freed
static Nan::Persistent<v8::Function> g_lifecycleCallback;

// Global variables for microphone capture
static AVAudioEngine* g_micEngine = nullptr;
static AVAudioInputNode* g_micInputNode = nullptr;
static bool g_isMicCapturing = false;

// Core Audio: default input changes — log + debounced mic engine restart while capture is active.
static bool g_defaultInputListenerRegistered = false;
static AudioDeviceID g_lastKnownDefaultInputDevice = kAudioObjectUnknown;

// Last HAL default input device id the running AVAudioEngine was started against (follow-OS-default mode).
static AudioDeviceID g_micOpenedInputDeviceId = kAudioObjectUnknown;
static std::mutex g_micEngineLifecycleMutex;
// Bumped on each new default-input notification or Stop — invalidates in-flight debounce/stability delays.
static std::atomic<uint64_t> g_micRouteRestartGeneration{0};

// Set from the realtime mic tap when the first buffer is enqueued (Bluetooth / cold-start can delay taps).
static std::atomic<bool> g_micFirstTapSeen{false};

static void SaysoScheduleMicRouteDebouncedRestart();

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

// Separate callback for microphone audio
static void MicStreamingAsyncCallback(uv_async_t* handle) {
    Nan::HandleScope scope;
    
    // Get the data from the handle
    StreamingData* data = static_cast<StreamingData*>(handle->data);
    if (!data || g_micStreamingCallback.IsEmpty()) {
        if (data) delete data;
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
    Local<Function> callback = Nan::New(g_micStreamingCallback);
    Local<Value> argv[2] = {buffer, format};
    Nan::Call(callback, Nan::GetCurrentContext()->Global(), 2, argv);

    // Clean up
    delete data;
    handle->data = nullptr;
}

// JS-thread drain for the lifecycle queue. Swap-then-invoke so producers never block on V8.
static void LifecycleAsyncCallback(uv_async_t* /*handle*/) {
    std::vector<std::string> events;
    {
        std::lock_guard<std::mutex> lk(g_lifecycleMutex);
        events.swap(g_lifecycleQueue);
    }
    if (g_lifecycleCallback.IsEmpty()) {
        return;
    }
    Nan::HandleScope scope;
    Local<Function> callback = Nan::New(g_lifecycleCallback);
    for (const auto& event : events) {
        Local<Value> argv[1] = { Nan::New<v8::String>(event.c_str()).ToLocalChecked() };
        Nan::Call(callback, Nan::GetCurrentContext()->Global(), 1, argv);
    }
}

// Emit a lifecycle diagnostic. Safe from any thread. Always NSLogs (dev terminal); additionally
// forwards to JS when the channel is wired, so packaged builds get it in the file log / Sentry.
// Message vocabulary: snake_case event token first, then `key=value` pairs; events ending in
// `_failed` are escalated by the JS side.
static void EmitLifecycleEvent(const std::string& message) {
    NSLog(@"🔎 [NATIVE] %s", message.c_str());
    std::lock_guard<std::mutex> lk(g_lifecycleMutex);
    if (!g_lifecycleAsyncHandle) {
        return; // channel not wired (older JS) — NSLog above is the only sink
    }
    g_lifecycleQueue.push_back(message);
    uv_async_send(g_lifecycleAsyncHandle);
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

    for (size_t i = 0; i < sampleCount; i++) {
        floatSamples[i] = (float)intSamples[i] * scale;
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
    
    
    // Initialize audio file only once on first callback, but after format detection
    // Skip file initialization if streaming-only mode
    if (!g_audioFileInitialized) {
        
        // First, detect the format from this sample (needed for both file recording and streaming)
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
        
        // Only initialize audio file if NOT in streaming-only mode
        if (!g_streamingOnly) {
            if (!initializeAudioFile()) {
                NSLog(@"❌ [NATIVE] Failed to initialize audio file");
                return;
            }
            NSLog(@"🎤 [NATIVE] Audio file initialized, starting continuous recording");
        } else {
            NSLog(@"🎤 [NATIVE] Format detected, streaming-only mode - skipping file initialization");
        }
        
        // Mark as initialized to skip this block next time (for both modes)
        g_audioFileInitialized = true;
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
    
    // Write audio data to file (skip if streaming-only mode)
    if (!g_streamingOnly) {
        if (callbackCount <= 5) {
            NSLog(@"📝 [WRITE] Writing %u bytes to file", bytesToWrite);
        }
        
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
            
        } else {
            NSLog(@"❌ [NATIVE] Audio file not initialized");
            return;
        }
        
    } else {
        // Streaming-only mode: skip file writing but continue to streaming callback
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
    if (type == SCStreamOutputTypeAudio) {
        if (stream != g_stream) {
            // Buffer from a stream that isn't the published pipeline (orphaned late start, or a
            // stream mid-teardown) — drop rather than contaminate the active session (SAYSO-355).
            return;
        }
        // Record actual start time at the first audio buffer
        if (g_actualStartMs == 0.0) {
            NSTimeInterval nowMs = [[NSDate date] timeIntervalSince1970] * 1000.0;
            g_actualStartMs = nowMs;
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

// SAYSO-359: what's currently OUTSTANDING in the SCK start sequence. A blanket 10s timeout only ever
// told us THAT the start hung, never WHERE — this distinguishes "getShareableContent never called
// back" (matches the documented external ScreenCaptureKit/replayd daemon hang) from a hang in
// startCapture, or from a stall in our own synchronous pipeline setup between the two.
//
// Every value names the operation IN FLIGHT when a timeout/cancel catches it, set immediately before
// that operation begins — never a "this step just finished" marker. That uniform convention matters:
// a mid-sequence value that means "completed" rather than "outstanding" (an earlier revision had
// several) is easy to misread under time pressure. It also means only points with real,
// watchdog-observable duration get their own value — filter/config allocation between receiving
// content and building the stream is synchronous, in-process, sub-microsecond, and genuinely cannot
// be caught mid-flight, so it doesn't get one; `addStreamOutput` does, since unlike the allocations
// around it, it plausibly performs real work.
enum class SckStartStage {
    not_started,
    getting_shareable_content,  // outstanding: async getShareableContentWithCompletionHandler
    adding_stream_output,       // outstanding: filter/config/SCStream build through addStreamOutput
    starting_capture,           // outstanding: async startCaptureWithCompletionHandler
};

// snake_case to match the lifecycle-event convention (docs/NATIVE_AUDIO_CONTRACT.md) and the existing
// `stage=shareable_content` at the late-callback-ignored site below, which now reuses this directly
// instead of a second, independently-spelled literal.
static const char* SckStartStageName(SckStartStage stage) {
    switch (stage) {
        case SckStartStage::not_started: return "not_started";
        case SckStartStage::getting_shareable_content: return "getting_shareable_content";
        case SckStartStage::adding_stream_output: return "adding_stream_output";
        case SckStartStage::starting_capture: return "starting_capture";
    }
    return "unknown";
}

// ScreenCaptureKit async start: Promise settled on libuv thread when addStreamOutput / startCapture completes.
// Settlement is generation-guarded and settle-once: the SCK completion blocks, the watchdog timeout and
// stop-cancel all race to settle the same start, and every loser must become a harmless no-op — never a
// double-settle and never a write into a freed PendingSckStart (SAYSO-355; UAF discipline per SAYSO-349).
// Blocks capture the generation VALUE, never the pending pointer, so a late callback can outlive the
// struct safely.
struct PendingSckStart {
    uv_async_t async;
    Nan::Persistent<v8::Promise::Resolver> resolver;
    uint64_t generation;
    bool settled;   // guarded by g_sckStartMutex
    bool reject;
    std::string message;
    SckStartStage stage;   // guarded by g_sckStartMutex; SAYSO-359
};

static PendingSckStart* g_sckStartPending = nullptr;
static std::mutex g_sckStartMutex;             // guards g_sckStartPending + settled transitions
static uint64_t g_sckStartNextGeneration = 0;  // bumped per start (JS thread only)

// Async system-audio stop: JS Promise resolves on libuv thread after SCK stopCapture completes.
struct PendingSckStop {
    uv_async_t async;
    std::list<Nan::Persistent<v8::Promise::Resolver>> resolvers;
    std::string savedFilePath;
    bool hasFilePath;
    double actualStartMsCopy;
    bool hasActualStartMs;
    bool stopFailed;
    std::string stopErrorDescription;
};

static PendingSckStop* g_sckStopPending = nullptr;

static Local<Promise> MakeRejectedPromise(Isolate* isolate, const char* msg) {
    Local<Context> context = isolate->GetCurrentContext();
    MaybeLocal<Promise::Resolver> maybe = Promise::Resolver::New(context);
    if (maybe.IsEmpty()) {
        return Local<Promise>();
    }
    Local<Promise::Resolver> resolver = maybe.ToLocalChecked();
    resolver->Reject(context, Nan::Error(msg)).Check();
    return resolver->GetPromise();
}

static void SckStartSettledCb(uv_async_t* handle) {
    PendingSckStart* p = static_cast<PendingSckStart*>(handle->data);
    if (!p) {
        return;
    }
    {
        std::lock_guard<std::mutex> lk(g_sckStartMutex);
        if (g_sckStartPending == p) {
            g_sckStartPending = nullptr;
        }
    }

    Nan::HandleScope scope;
    Isolate* isolate = Isolate::GetCurrent();
    Local<Context> context = isolate->GetCurrentContext();
    Local<Promise::Resolver> res = Nan::New(p->resolver);
    p->resolver.Reset();

    if (p->reject) {
        const char* m = p->message.empty() ? "System audio capture failed" : p->message.c_str();
        res->Reject(context, Nan::Error(m)).Check();
    } else {
        res->Resolve(context, Nan::True()).Check();
    }

    uv_close(reinterpret_cast<uv_handle_t*>(handle), [](uv_handle_t* h) {
        PendingSckStart* self = static_cast<PendingSckStart*>(h->data);
        delete self;
    });
}

// Settle the pending SCK start for `generation` exactly once. Returns false when that start is no
// longer current (already settled by a competing path, superseded, or gone) — callers must treat
// false as "you were abandoned": clean up anything created locally and touch no globals.
static bool SettleSckStartForGeneration(uint64_t generation, bool reject, const std::string& message) {
    PendingSckStart* winner = nullptr;
    {
        std::lock_guard<std::mutex> lk(g_sckStartMutex);
        if (!g_sckStartPending || g_sckStartPending->generation != generation || g_sckStartPending->settled) {
            return false;
        }
        g_sckStartPending->settled = true;
        winner = g_sckStartPending;
        winner->reject = reject;
        winner->message = message;
    }
    // Safe outside the lock: only the single winner ever sends on this handle, and deletion happens
    // strictly after SckStartSettledCb consumes this send on the JS thread.
    uv_async_send(&winner->async);
    return true;
}

// True when the start identified by `generation` has been abandoned (timed out, canceled, superseded).
static bool SckStartAbandoned(uint64_t generation) {
    std::lock_guard<std::mutex> lk(g_sckStartMutex);
    return !g_sckStartPending || g_sckStartPending->generation != generation || g_sckStartPending->settled;
}

// SAYSO-359: advance the stage marker for `generation`. A no-op for an abandoned/superseded/already-
// settled generation — never touches a newer start's stage, mirroring SckStartAbandoned's guard.
static void SckStartAdvanceStage(uint64_t generation, SckStartStage stage) {
    std::lock_guard<std::mutex> lk(g_sckStartMutex);
    if (g_sckStartPending && g_sckStartPending->generation == generation && !g_sckStartPending->settled) {
        g_sckStartPending->stage = stage;
    }
}

// Peek the current stage for `generation` (NotStarted if it's no longer the pending start). Used to
// enrich a message that must be built *before* calling SettleSckStartForGeneration — the watchdog and
// stop-cancel paths, which fire asynchronously and otherwise have no idea which stage was in flight.
static SckStartStage SckStartPeekStage(uint64_t generation) {
    std::lock_guard<std::mutex> lk(g_sckStartMutex);
    if (g_sckStartPending && g_sckStartPending->generation == generation) {
        return g_sckStartPending->stage;
    }
    return SckStartStage::not_started;
}

static void SckStopSettledCb(uv_async_t* handle) {
    PendingSckStop* p = static_cast<PendingSckStop*>(handle->data);
    if (!p) {
        return;
    }
    g_sckStopPending = nullptr;

    Nan::HandleScope scope;
    Isolate* isolate = Isolate::GetCurrent();
    Local<Context> context = isolate->GetCurrentContext();

    Local<Object> result = Nan::New<Object>();
    Nan::Set(result, Nan::New("success").ToLocalChecked(), Nan::New<v8::Boolean>(!p->stopFailed));
    if (p->hasFilePath) {
        Nan::Set(result, Nan::New("filePath").ToLocalChecked(),
                 Nan::New<String>(p->savedFilePath.c_str()).ToLocalChecked());
    } else {
        Nan::Set(result, Nan::New("filePath").ToLocalChecked(), Nan::Null());
    }
    if (p->hasActualStartMs) {
        Nan::Set(result, Nan::New("actualStartMs").ToLocalChecked(), Nan::New<Number>(p->actualStartMsCopy));
    } else {
        Nan::Set(result, Nan::New("actualStartMs").ToLocalChecked(), Nan::Null());
    }
    if (p->stopFailed && !p->stopErrorDescription.empty()) {
        Nan::Set(result, Nan::New("error").ToLocalChecked(),
                 Nan::New<String>(p->stopErrorDescription.c_str()).ToLocalChecked());
    }

    for (auto& pers : p->resolvers) {
        Local<Promise::Resolver> res = Nan::New(pers);
        if (!res.IsEmpty()) {
            res->Resolve(context, result).Check();
        }
        pers.Reset();
    }
    p->resolvers.clear();

    uv_close(reinterpret_cast<uv_handle_t*>(handle), [](uv_handle_t* h) {
        PendingSckStop* self = static_cast<PendingSckStop*>(h->data);
        delete self;
    });
}

static NSString* SaysoCopyAudioDeviceName(AudioDeviceID deviceID) {
    if (deviceID == kAudioObjectUnknown) {
        return @"(unknown)";
    }
    CFStringRef nameRef = nullptr;
    UInt32 dataSize = sizeof(nameRef);
    AudioObjectPropertyAddress pa = {
        kAudioObjectPropertyName,
        kAudioObjectPropertyScopeGlobal,
        kAudioObjectPropertyElementMain
    };
    OSStatus st = AudioObjectGetPropertyData(deviceID, &pa, 0, nullptr, &dataSize, &nameRef);
    if (st != noErr || nameRef == nullptr) {
        return [NSString stringWithFormat:@"device %u", (unsigned)deviceID];
    }
    NSString* s = [NSString stringWithString:(__bridge NSString*)nameRef];
    CFRelease(nameRef);
    return s;
}

static NSString* SaysoCopyAudioDeviceUID(AudioDeviceID deviceID) {
    if (deviceID == kAudioObjectUnknown) {
        return nil;
    }
    CFStringRef uidRef = nullptr;
    UInt32 dataSize = sizeof(uidRef);
    AudioObjectPropertyAddress pa = {
        kAudioDevicePropertyDeviceUID,
        kAudioObjectPropertyScopeGlobal,
        kAudioObjectPropertyElementMain
    };
    OSStatus st = AudioObjectGetPropertyData(deviceID, &pa, 0, nullptr, &dataSize, &uidRef);
    if (st != noErr || uidRef == nullptr) {
        return nil;
    }
    NSString* s = [NSString stringWithString:(__bridge NSString*)uidRef];
    CFRelease(uidRef);
    return s;
}

static AudioDeviceID SaysoGetCurrentDefaultInputDeviceID() {
    AudioDeviceID dev = kAudioObjectUnknown;
    UInt32 sz = sizeof(dev);
    AudioObjectPropertyAddress pa = {
        kAudioHardwarePropertyDefaultInputDevice,
        kAudioObjectPropertyScopeGlobal,
        kAudioObjectPropertyElementMain
    };
    if (AudioObjectGetPropertyData(kAudioObjectSystemObject, &pa, 0, nullptr, &sz, &dev) != noErr) {
        return kAudioObjectUnknown;
    }
    return dev;
}

static OSStatus SaysoDefaultInputDeviceListenerProc(AudioObjectID /*inObjectID*/,
                                                    UInt32 inNumberAddresses,
                                                    const AudioObjectPropertyAddress* inAddresses,
                                                    void* /*inClientData*/) {
    for (UInt32 i = 0; i < inNumberAddresses; i++) {
        if (inAddresses[i].mSelector != kAudioHardwarePropertyDefaultInputDevice) {
            continue;
        }

        AudioDeviceID newDevice = kAudioObjectUnknown;
        UInt32 size = sizeof(newDevice);
        AudioObjectPropertyAddress pa = {
            kAudioHardwarePropertyDefaultInputDevice,
            kAudioObjectPropertyScopeGlobal,
            kAudioObjectPropertyElementMain
        };
        OSStatus err = AudioObjectGetPropertyData(kAudioObjectSystemObject, &pa, 0, nullptr, &size, &newDevice);
        if (err != noErr) {
            NSLog(@"🔊 [NATIVE] Default input listener: failed to read new default device (err=%d)", (int)err);
            continue;
        }

        AudioDeviceID previous = g_lastKnownDefaultInputDevice;
        g_lastKnownDefaultInputDevice = newDevice;

        dispatch_async(dispatch_get_main_queue(), ^{
            NSString* prevName = SaysoCopyAudioDeviceName(previous);
            NSString* newName = SaysoCopyAudioDeviceName(newDevice);
            NSString* newUid = SaysoCopyAudioDeviceUID(newDevice);
            NSLog(@"🔊 [NATIVE] Default INPUT changed: %u \"%@\" -> %u \"%@\" uid=\"%@\" | Sayso micCaptureActive=%s",
                  (unsigned)previous, prevName,
                  (unsigned)newDevice, newName,
                  newUid ? newUid : @"(nil)",
                  g_isMicCapturing ? "YES" : "NO");
            if (g_isMicCapturing) {
                SaysoScheduleMicRouteDebouncedRestart();
            }
        });
    }
    return noErr;
}

static void SaysoRegisterDefaultInputDeviceListener() {
    if (g_defaultInputListenerRegistered) {
        return;
    }
    AudioObjectPropertyAddress pa = {
        kAudioHardwarePropertyDefaultInputDevice,
        kAudioObjectPropertyScopeGlobal,
        kAudioObjectPropertyElementMain
    };
    OSStatus st = AudioObjectAddPropertyListener(kAudioObjectSystemObject, &pa, SaysoDefaultInputDeviceListenerProc, nullptr);
    if (st != noErr) {
        NSLog(@"❌ [NATIVE] Could not register default-input listener: %d", (int)st);
        return;
    }
    g_defaultInputListenerRegistered = true;

    AudioDeviceID current = kAudioObjectUnknown;
    UInt32 sz = sizeof(current);
    OSStatus q = AudioObjectGetPropertyData(kAudioObjectSystemObject, &pa, 0, nullptr, &sz, &current);
    if (q == noErr) {
        g_lastKnownDefaultInputDevice = current;
        NSString* nm = SaysoCopyAudioDeviceName(current);
        NSString* uid = SaysoCopyAudioDeviceUID(current);
        NSLog(@"🔊 [NATIVE] Default INPUT at init: id=%u name=\"%@\" uid=\"%@\"",
              (unsigned)current, nm, uid ? uid : @"(nil)");
    }
    NSLog(@"✅ [NATIVE] CoreAudio default-input device listener registered");
}

// Initialize the native module
NAN_METHOD(Initialize) {
    
    // Create audio processing queue
    g_audioQueue = dispatch_queue_create("com.sayso.audio", DISPATCH_QUEUE_SERIAL);
    
    // Create delegate
    g_delegate = [[AudioCaptureDelegate alloc] init];

    SaysoRegisterDefaultInputDeviceListener();
    
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(true));
}

// Non-destructive check: returns true if screen recording is already granted.
// Uses CGPreflightScreenCaptureAccess() (macOS 12.3+) — no dialog, no side effects.
NAN_METHOD(CheckScreenRecordingGranted) {
    bool granted = CGPreflightScreenCaptureAccess();
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(granted));
}

// Request screen recording permission.
// CGRequestScreenCaptureAccess() shows the native macOS dialog when the permission is
// "not-determined" (its own "Open System Settings" button leads the user to the right pane).
// Returns true if already granted; false if the dialog was shown or the permission was previously denied.
NAN_METHOD(RequestScreenRecordingPermission) {
    bool granted = CGRequestScreenCaptureAccess();
    NSLog(@"🎤 [NATIVE] RequestScreenRecordingPermission → granted=%d", granted);
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(granted));
}

// Start system audio capture
NAN_METHOD(StartSystemAudioCapture) {
    NSLog(@"🎤 [NATIVE] Starting system audio capture");
    
    Isolate* isolate = info.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();
    
    if (g_isCapturing) {
        NSLog(@"⚠️ [NATIVE] System audio capture already active");
        info.GetReturnValue().Set(MakeRejectedPromise(isolate, "System audio capture already active"));
        return;
    }
    {
        std::lock_guard<std::mutex> lk(g_sckStartMutex);
        if (g_sckStartPending != nullptr) {
            NSLog(@"⚠️ [NATIVE] System audio capture start already in progress (gen=%llu)",
                  (unsigned long long)g_sckStartPending->generation);
            info.GetReturnValue().Set(MakeRejectedPromise(isolate, "System audio capture start already in progress"));
            return;
        }
    }
    if (g_sckStopPending != nullptr) {
        NSLog(@"⚠️ [NATIVE] System audio stop still in progress");
        info.GetReturnValue().Set(MakeRejectedPromise(isolate, "System audio stop still in progress"));
        return;
    }
    
    // Reset streaming-only flag
    g_streamingOnly = false;
    
    // Check if streamingOnly option is provided
    if (info.Length() > 0 && info[0]->IsObject()) {
        Local<Object> options = Nan::To<Object>(info[0]).ToLocalChecked();
        Local<Value> streamingOnlyValue = Nan::Get(options, Nan::New("streamingOnly").ToLocalChecked()).ToLocalChecked();
        if (!streamingOnlyValue->IsUndefined() && streamingOnlyValue->IsBoolean()) {
            g_streamingOnly = Nan::To<bool>(streamingOnlyValue).FromJust();
            if (g_streamingOnly) {
                NSLog(@"🎤 [NATIVE] Streaming-only mode: file creation disabled");
            }
        }
    }
    
    // Reset actual start timestamp
    g_actualStartMs = 0.0;

    // Set output directory
    std::string currentPath = std::string([[[NSBundle mainBundle] bundlePath] UTF8String]);
    size_t electronPos = currentPath.find("/node_modules/electron/dist/Electron.app");
    if (electronPos != std::string::npos) {
        currentPath = currentPath.substr(0, electronPos);
        g_outputDirectory = currentPath + "/electron/full_recordings";
        NSLog(@"🎤 [NATIVE] Development mode - using project directory: %s", g_outputDirectory.c_str());
    } else {
        NSArray* paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
        NSString* appSupportDir = [paths firstObject];
        NSString* appDir = [appSupportDir stringByAppendingPathComponent:@"sayso-app"];
        NSString* tempFullRecordingsDir = [appDir stringByAppendingPathComponent:@"temp/full_recordings"];
        g_outputDirectory = std::string([tempFullRecordingsDir UTF8String]);
        NSLog(@"🎤 [NATIVE] Production mode - unified output directory: %s", g_outputDirectory.c_str());
    }
    
    NSLog(@"🎤 [NATIVE] Final output directory: %s", g_outputDirectory.c_str());
    
    MaybeLocal<Promise::Resolver> maybeResolver = Promise::Resolver::New(context);
    if (maybeResolver.IsEmpty()) {
        info.GetReturnValue().Set(MakeRejectedPromise(isolate, "Failed to create promise resolver"));
        return;
    }
    Local<Promise::Resolver> resolver = maybeResolver.ToLocalChecked();
    info.GetReturnValue().Set(resolver->GetPromise());
    
    PendingSckStart* pending = new PendingSckStart();
    pending->async.data = pending;
    pending->settled = false;
    pending->reject = false;
    pending->stage = SckStartStage::not_started;
    int uvErr = uv_async_init(uv_default_loop(), &pending->async, SckStartSettledCb);
    if (uvErr != 0) {
        delete pending;
        resolver->Reject(context, Nan::Error("Failed to initialize async notifier for system audio start")).Check();
        return;
    }
    pending->resolver.Reset(resolver);
    pending->generation = ++g_sckStartNextGeneration;
    const uint64_t startGen = pending->generation;
    {
        std::lock_guard<std::mutex> lk(g_sckStartMutex);
        g_sckStartPending = pending;
    }
    NSLog(@"🎤 [NATIVE] SCK start pending (gen=%llu)", (unsigned long long)startGen);

    // Watchdog: SCK completion handlers are not guaranteed to fire (SAYSO-355 saw a start wedge for
    // the process lifetime). If nothing settles this generation in time, reject it so Cue can retry
    // without an app relaunch. Captures the generation VALUE — safe after the struct is freed.
    const int64_t kSckStartWatchdogNs = 10 * NSEC_PER_SEC;
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, kSckStartWatchdogNs), dispatch_get_main_queue(), ^{
        // SAYSO-359: peek the stage BEFORE settling — the watchdog fires from an independent timer
        // with no idea which async step was in flight, so this is the only chance to capture it.
        SckStartStage stage = SckStartPeekStage(startGen);
        std::string msg = std::string("sck_start_timeout stage=") + SckStartStageName(stage) +
            ": ScreenCaptureKit start did not settle within 10s";
        if (SettleSckStartForGeneration(startGen, true, msg)) {
            EmitLifecycleEvent("sck_start_watchdog_fired gen=" + std::to_string(startGen) +
                               " stage=" + SckStartStageName(stage) + " timeout_ms=10000");
        }
    });

    // The blocks below deliberately build the pipeline in LOCALS and publish to globals only after
    // winning settlement. An abandoned (timed-out/canceled) start must never clobber the globals of a
    // newer start, and must stop any stream it managed to create.
    SckStartAdvanceStage(startGen, SckStartStage::getting_shareable_content);
    [SCShareableContent getShareableContentWithCompletionHandler:^(SCShareableContent *content, NSError *error) {
        if (SckStartAbandoned(startGen)) {
            EmitLifecycleEvent("sck_start_late_callback_ignored gen=" + std::to_string(startGen) +
                               " stage=" + SckStartStageName(SckStartStage::getting_shareable_content));
            return;
        }
        if (error) {
            NSLog(@"❌ [NATIVE] Failed to get shareable content: %@", error.localizedDescription);
            std::string msg([[error localizedDescription] UTF8String]);
            if (msg.empty()) {
                msg = "Failed to get shareable content (screen recording permission?)";
            }
            SettleSckStartForGeneration(startGen, true, msg);
            return;
        }

        if (content.displays.count == 0) {
            NSLog(@"❌ [NATIVE] No displays available");
            SettleSckStartForGeneration(startGen, true, "No displays available for system audio capture");
            return;
        }

        SCDisplay *display = content.displays.firstObject;
        NSLog(@"🎤 [NATIVE] Using display: %u", display.displayID);
        SckStartAdvanceStage(startGen, SckStartStage::adding_stream_output);

        SCContentFilter* filter = [[SCContentFilter alloc] initWithDisplay:display excludingWindows:@[]];

        SCStreamConfiguration* config = [[SCStreamConfiguration alloc] init];

        if (@available(macOS 13.0, *)) {
            config.capturesAudio = YES;
            config.channelCount = 2;
        } else {
            SettleSckStartForGeneration(startGen, true, "System audio capture requires macOS 13 or later");
            return;
        }

        if (@available(macOS 14.0, *)) {
            config.excludesCurrentProcessAudio = YES;
        }

        // SAYSO-337: we only consume SCStreamOutputTypeAudio, but SCK still runs an internal
        // video pipeline under the hood even when nothing subscribes to it. Left at defaults
        // (full display resolution, 60fps, queueDepth 10) that pipeline drove a system-wide
        // WindowServer/GPU slowdown. Shrinking the frame to 2x2, capping to 1fps, and trimming
        // the IOSurface queue eliminates that overhead without affecting audio delivery.
        config.width = 2;
        config.height = 2;
        config.minimumFrameInterval = CMTimeMake(1, 1);
        config.queueDepth = 3;

        NSLog(@"🎤 [NATIVE] Stream configuration: Audio=%@, SampleRate=%ld, Channels=%ld, Video=%ldx%ld, MinFrameInterval=%lld/%d, QueueDepth=%ld",
              config.capturesAudio ? @"YES" : @"NO",
              (long)config.sampleRate,
              (long)config.channelCount,
              (long)config.width,
              (long)config.height,
              config.minimumFrameInterval.value,
              config.minimumFrameInterval.timescale,
              (long)config.queueDepth);

        SCStream* stream = [[SCStream alloc] initWithFilter:filter
                                              configuration:config
                                                   delegate:g_delegate];

        NSError *streamError = nil;
        BOOL outputOk = [stream addStreamOutput:g_delegate
                                           type:SCStreamOutputTypeAudio
                             sampleHandlerQueue:g_audioQueue
                                          error:&streamError];

        if (!outputOk) {
            NSString* desc = streamError ? streamError.localizedDescription : @"Unknown error";
            NSLog(@"❌ [NATIVE] Failed to add stream output: %@", desc);
            std::string msg([desc UTF8String]);
            if (msg.empty()) {
                msg = "Failed to add ScreenCaptureKit audio stream output";
            }
            SettleSckStartForGeneration(startGen, true, msg);
            return;
        }

        NSLog(@"✅ [NATIVE] Stream output added successfully");
        SckStartAdvanceStage(startGen, SckStartStage::starting_capture);
        [stream startCaptureWithCompletionHandler:^(NSError *startErr) {
            if (startErr) {
                NSLog(@"❌ [NATIVE] Failed to start capture: %@", startErr.localizedDescription);
                std::string msg([[startErr localizedDescription] UTF8String]);
                if (msg.empty()) {
                    msg = "Failed to start system audio capture";
                }
                SettleSckStartForGeneration(startGen, true, msg);
                return;
            }
            if (SettleSckStartForGeneration(startGen, false, "")) {
                // We won: publish the pipeline. g_stream non-null ⟺ capture owned + running.
                g_stream = stream;
                g_filter = filter;
                g_config = config;
                g_delegate.audioCallback = ^(CMSampleBufferRef sampleBuffer) {
                    audioCallback(sampleBuffer);
                };
                g_isCapturing = true;
                NSLog(@"✅ [NATIVE] System audio capture started successfully (gen=%llu)",
                      (unsigned long long)startGen);
            } else {
                // Abandoned start that nevertheless reached running: orphaned stream — stop it, touch
                // no globals (they may belong to a newer start by now). If this stop fails we have a
                // zombie SCK capture (purple indicator, no owner): _failed is escalated JS-side.
                EmitLifecycleEvent("sck_orphan_stream_stopping gen=" + std::to_string(startGen));
                [stream stopCaptureWithCompletionHandler:^(NSError *stopErr) {
                    if (stopErr) {
                        std::string desc([[stopErr localizedDescription] UTF8String]);
                        EmitLifecycleEvent("sck_orphan_stream_stop_failed gen=" + std::to_string(startGen) +
                                           " error=" + desc);
                    } else {
                        EmitLifecycleEvent("sck_orphan_stream_stopped gen=" + std::to_string(startGen));
                    }
                }];
            }
        }];
    }];
}

// Stop system audio capture (returns Promise; resolves after SCK stop + pipeline teardown completes)
NAN_METHOD(StopSystemAudioCapture) {
    NSLog(@"🎤 [NATIVE] Stopping system audio capture");
    
    Isolate* isolate = info.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();
    
    // A start may be pending (SCK callbacks not yet settled). Cancel it so a wedged start cannot
    // outlive a stop: reject its promise now; the late completion (if it ever fires) sees the
    // abandoned generation and stops its own orphaned stream (SAYSO-355).
    {
        uint64_t pendingGen = 0;
        bool hasPending = false;
        {
            std::lock_guard<std::mutex> lk(g_sckStartMutex);
            if (g_sckStartPending && !g_sckStartPending->settled) {
                hasPending = true;
                pendingGen = g_sckStartPending->generation;
            }
        }
        if (hasPending) {
            // SAYSO-359: same reasoning as the watchdog — stop-cancel fires from a separate call path
            // with no idea which stage the pending start had reached, so peek it before settling.
            SckStartStage stage = SckStartPeekStage(pendingGen);
            std::string msg = std::string("sck_start_canceled_by_stop stage=") + SckStartStageName(stage) +
                ": stop requested while start still pending";
            if (SettleSckStartForGeneration(pendingGen, true, msg)) {
                EmitLifecycleEvent("sck_start_canceled_by_stop gen=" + std::to_string(pendingGen) +
                                   " stage=" + SckStartStageName(stage));
            }
        }
    }

    // Idle: no stream object (covers "not capturing"; a pending start was just canceled above)
    if (!g_stream) {
        NSLog(@"⚠️ [NATIVE] System audio capture not active (no stream)");
        MaybeLocal<Promise::Resolver> maybeIdle = Promise::Resolver::New(context);
        if (maybeIdle.IsEmpty()) {
            info.GetReturnValue().Set(MakeRejectedPromise(isolate, "Failed to create promise resolver for stop"));
            return;
        }
        Local<Promise::Resolver> idleResolver = maybeIdle.ToLocalChecked();
        Local<Object> idleResult = Nan::New<Object>();
        Nan::Set(idleResult, Nan::New("success").ToLocalChecked(), Nan::New<v8::Boolean>(false));
        Nan::Set(idleResult, Nan::New("filePath").ToLocalChecked(), Nan::Null());
        Nan::Set(idleResult, Nan::New("actualStartMs").ToLocalChecked(), Nan::Null());
        idleResolver->Resolve(context, idleResult).Check();
        info.GetReturnValue().Set(idleResolver->GetPromise());
        return;
    }
    
    // Coalesce concurrent stop calls onto the same SCK completion
    if (g_sckStopPending != nullptr) {
        MaybeLocal<Promise::Resolver> maybeCo = Promise::Resolver::New(context);
        if (maybeCo.IsEmpty()) {
            info.GetReturnValue().Set(MakeRejectedPromise(isolate, "Failed to create promise resolver for stop"));
            return;
        }
        Local<Promise::Resolver> coResolver = maybeCo.ToLocalChecked();
        g_sckStopPending->resolvers.emplace_back();
        g_sckStopPending->resolvers.back().Reset(coResolver);
        info.GetReturnValue().Set(coResolver->GetPromise());
        return;
    }
    
    PendingSckStop* pending = new PendingSckStop();
    pending->async.data = pending;
    pending->savedFilePath = g_audioFilePath;
    pending->hasFilePath = !pending->savedFilePath.empty();
    pending->actualStartMsCopy = g_actualStartMs;
    pending->hasActualStartMs = g_actualStartMs > 0.0;
    pending->stopFailed = false;
    
    MaybeLocal<Promise::Resolver> maybeFirst = Promise::Resolver::New(context);
    if (maybeFirst.IsEmpty()) {
        delete pending;
        info.GetReturnValue().Set(MakeRejectedPromise(isolate, "Failed to create promise resolver for stop"));
        return;
    }
    Local<Promise::Resolver> firstResolver = maybeFirst.ToLocalChecked();
    pending->resolvers.emplace_back();
    pending->resolvers.back().Reset(firstResolver);
    
    int uvErr = uv_async_init(uv_default_loop(), &pending->async, SckStopSettledCb);
    if (uvErr != 0) {
        for (auto& pr : pending->resolvers) {
            pr.Reset();
        }
        pending->resolvers.clear();
        delete pending;
        firstResolver->Reject(context, Nan::Error("Failed to initialize async notifier for system audio stop")).Check();
        info.GetReturnValue().Set(firstResolver->GetPromise());
        return;
    }
    
    g_sckStopPending = pending;
    
    if (pending->hasFilePath) {
        NSLog(@"✅ [NATIVE] Stop will return file path: %s", pending->savedFilePath.c_str());
    } else {
        NSLog(@"⚠️ [NATIVE] No file path available at stop");
    }
    
    [g_stream stopCaptureWithCompletionHandler:^(NSError *error) {
        if (error) {
            NSLog(@"❌ [NATIVE] Failed to stop capture: %@", error.localizedDescription);
            pending->stopFailed = true;
            NSString* desc = error.localizedDescription;
            if (desc) {
                pending->stopErrorDescription = std::string([desc UTF8String]);
            }
        } else {
            NSLog(@"✅ [NATIVE] System audio capture stopped successfully");
        }
        
        if (g_delegate) {
            g_delegate.audioCallback = nil;
        }
        g_isCapturing = false;
        g_streamingOnly = false;
        closeAudioFile();
        g_streamingCallback.Reset();
        
        if (g_streamingAsyncHandle) {
            uv_async_t* handleToDelete = g_streamingAsyncHandle;
            g_streamingAsyncHandle = nullptr;
            uv_close((uv_handle_t*)handleToDelete, [](uv_handle_t* handle) {
                delete reinterpret_cast<uv_async_t*>(handle);
            });
        }
        
        g_stream = nil;
        g_filter = nil;
        g_config = nil;
        
        uv_async_send(&pending->async);
    }];
    
    info.GetReturnValue().Set(firstResolver->GetPromise());
}

// Tear down mic engine + tap without clearing Node callbacks (internal retry path).
static void MicEngineTeardownOnly() {
    if (g_micInputNode) {
        [g_micInputNode removeTapOnBus:0];
        g_micInputNode = nullptr;
    }
    if (g_micEngine) {
        [g_micEngine stop];
        g_micEngine = nullptr;
    }
}

static bool WaitForMicFirstTapMs(int timeoutMs) {
    const useconds_t stepUs = 2000;
    int elapsedMs = 0;
    while (elapsedMs < timeoutMs) {
        if (g_micFirstTapSeen.load(std::memory_order_acquire)) {
            return true;
        }
        usleep(stepUs);
        elapsedMs += 2;
    }
    return g_micFirstTapSeen.load(std::memory_order_acquire);
}

// Starts engine and blocks until the realtime tap enqueues at least one buffer (or timeout).
// SAYSO-347: `failReason`, when non-null, is set to one of "no_input_node" / "tap_install_failed" /
// "engine_start_failed" / "no_tap_buffers" on failure — the same collapsing bug this ticket fixed one
// level up (all four used to report as a single StartMicrophoneCapture-level "no_tap_buffers"), most
// importantly "engine_start_failed" carrying a real NSError that was previously NSLog'd and discarded.
// Default nullptr keeps the SaysoPerformMicRestartIfCapturing call site (which only needs the bool)
// unaffected.
static bool TryStartMicrophoneCaptureOnce(int waitForFirstTapMs, const char** failReason = nullptr) {
    g_micFirstTapSeen.store(false, std::memory_order_release);

    g_micEngine = [[AVAudioEngine alloc] init];
    g_micInputNode = [g_micEngine inputNode];

    if (!g_micInputNode) {
        NSLog(@"❌ [NATIVE] Failed to get input node from audio engine");
        g_micEngine = nullptr;
        if (failReason) *failReason = "no_input_node";
        return false;
    }
    
    // Use nil format so the tap matches the hardware connection. Passing inputFormatForBus:0 can
    // raise NSException "Failed to create tap due to format mismatch" on some routes (e.g. after SCK).
    @try {
        [g_micInputNode installTapOnBus:0 bufferSize:4096 format:nil block:^(AVAudioPCMBuffer* buffer, AVAudioTime* when) {
        if (!buffer) {
            return;
        }
        
        if (!g_micStreamingAsyncHandle) {
            NSLog(@"⚠️ [NATIVE] Mic callback skipped - microphone streaming async handle is null");
            return;
        }
        
        if (g_micStreamingCallback.IsEmpty()) {
            NSLog(@"⚠️ [NATIVE] Mic callback skipped - microphone streaming callback is empty");
            return;
        }
        
        AVAudioFormat* bufferFormat = buffer.format;
        int channels = (int)bufferFormat.channelCount;
        int frameLength = (int)buffer.frameLength;
        if (frameLength <= 0) {
            return;
        }
        
        StreamingData* streamData = new StreamingData();
        size_t dataSize = 0;
        
        if (bufferFormat.commonFormat == AVAudioPCMFormatFloat32) {
            float* floatData = buffer.floatChannelData[0];
            dataSize = frameLength * channels * sizeof(float);
            streamData->audioData.resize(dataSize);
            memcpy(streamData->audioData.data(), floatData, dataSize);
            streamData->bitDepth = 32;
            streamData->isFloat = true;
        } else if (bufferFormat.commonFormat == AVAudioPCMFormatInt16) {
            int16_t* intData = buffer.int16ChannelData[0];
            dataSize = frameLength * channels * sizeof(int16_t);
            streamData->audioData.resize(dataSize);
            memcpy(streamData->audioData.data(), intData, dataSize);
            streamData->bitDepth = 16;
            streamData->isFloat = false;
        } else {
            NSLog(@"⚠️ [NATIVE] Unsupported audio format: %lu", (unsigned long)bufferFormat.commonFormat);
            delete streamData;
            return;
        }
        
        streamData->sampleRate = bufferFormat.sampleRate;
        streamData->channels = channels;
        
        g_micFirstTapSeen.store(true, std::memory_order_release);
        g_micStreamingAsyncHandle->data = streamData;
        uv_async_send(g_micStreamingAsyncHandle);
    }];
    } @catch (NSException *ex) {
        NSLog(@"❌ [NATIVE] installTapOnBus failed: %@ — %@", ex.name, ex.reason);
        MicEngineTeardownOnly();
        if (failReason) *failReason = "tap_install_failed";
        return false;
    }

    NSError* error = nil;
    BOOL ok = [g_micEngine startAndReturnError:&error];

    if (!ok) {
        NSLog(@"❌ [NATIVE] Failed to start microphone capture: %@ (code=%ld)",
              error.localizedDescription, (long)error.code);
        MicEngineTeardownOnly();
        if (failReason) *failReason = "engine_start_failed";
        return false;
    }

    if (!WaitForMicFirstTapMs(waitForFirstTapMs)) {
        NSLog(@"⚠️ [NATIVE] Mic engine started but no tap buffers within %d ms — tearing down for retry",
              waitForFirstTapMs);
        MicEngineTeardownOnly();
        if (failReason) *failReason = "no_tap_buffers";
        return false;
    }

    return true;
}

static dispatch_queue_t SaysoMicRouteRestartQueue() {
    static dispatch_queue_t q;
    static dispatch_once_t once;
    dispatch_once(&once, ^{
        q = dispatch_queue_create("com.sayso.micRouteRestart", DISPATCH_QUEUE_SERIAL);
    });
    return q;
}

// Runs on SaysoMicRouteRestartQueue. Restarts AVAudioEngine so capture follows the new OS default input.
// When `forcedAfterDefaultInputNotification` is YES, always teardown+restart if capture is active: Core Audio
// can fire default-input changes for route/format churn where the HAL default id still matches
// `g_micOpenedInputDeviceId` while AVAudioEngine is stuck on a dead Bluetooth path (e.g. AirPods removed).
static void SaysoPerformMicRestartIfCapturing(BOOL forcedAfterDefaultInputNotification) {
    std::lock_guard<std::mutex> lk(g_micEngineLifecycleMutex);

    if (!g_isMicCapturing) {
        return;
    }
    if (g_micStreamingCallback.IsEmpty() || !g_micStreamingAsyncHandle) {
        return;
    }

    AudioDeviceID currentDefault = SaysoGetCurrentDefaultInputDeviceID();
    if (currentDefault == kAudioObjectUnknown) {
        NSLog(@"🔊 [NATIVE] Mic route restart skipped: could not read default input device");
        return;
    }

    if (!forcedAfterDefaultInputNotification && currentDefault == g_micOpenedInputDeviceId &&
        g_micEngine != nullptr) {
        NSLog(@"🔊 [NATIVE] Mic route restart skipped: default id %u unchanged and engine present",
              (unsigned)currentDefault);
        return;
    }

    if (forcedAfterDefaultInputNotification && currentDefault == g_micOpenedInputDeviceId &&
        g_micEngine != nullptr) {
        NSLog(@"🔊 [NATIVE] Mic route: forced restart after default-input notification (id %u unchanged — "
              @"rebinding engine/tap)",
              (unsigned)currentDefault);
    }

    NSLog(@"🔊 [NATIVE] Mic route: restarting engine for default input id=%u (previous opened id=%u)",
          (unsigned)currentDefault, (unsigned)g_micOpenedInputDeviceId);

    MicEngineTeardownOnly();

    const int kFirstTapWaitMs = 1200;
    const int kRetryTapWaitMs = 1500;
    bool ok = TryStartMicrophoneCaptureOnce(kFirstTapWaitMs);
    if (!ok) {
        NSLog(@"🎤 [NATIVE] Mic route: repeating capture once after default change");
        ok = TryStartMicrophoneCaptureOnce(kRetryTapWaitMs);
    }

    if (ok) {
        g_micOpenedInputDeviceId = currentDefault;
        NSLog(@"✅ [NATIVE] Mic route restart succeeded; now following default input id=%u",
              (unsigned)currentDefault);
    } else {
        g_isMicCapturing = false;
        g_micOpenedInputDeviceId = kAudioObjectUnknown;
        NSLog(@"❌ [NATIVE] Mic route restart FAILED after OS default change — mic capture marked inactive");
    }
}

// Coalesces rapid default-input notifications. Each notification bumps `g_micRouteRestartGeneration`
// so earlier timers no-op.
// Fast path: HAL default id != id we opened the engine against → short delays so we don't leave the
// tap on a dead Bluetooth path for hundreds of ms while the user is already on built-in mic.
// Slow path: same id (stuck engine / spurious notify) → longer debounce + stability to absorb flap.
static void SaysoScheduleMicRouteDebouncedRestart() {
    dispatch_async(SaysoMicRouteRestartQueue(), ^{
        if (!g_isMicCapturing) {
            return;
        }

        AudioDeviceID halNow = SaysoGetCurrentDefaultInputDeviceID();
        const bool fastPath =
            (halNow != kAudioObjectUnknown && g_micOpenedInputDeviceId != kAudioObjectUnknown &&
             halNow != g_micOpenedInputDeviceId);

        const int64_t debounceNs =
            fastPath ? (80 * NSEC_PER_MSEC) : (600 * NSEC_PER_MSEC);
        const int64_t stabilityDelayNs =
            fastPath ? (60 * NSEC_PER_MSEC) : (150 * NSEC_PER_MSEC);

        if (fastPath) {
            NSLog(@"🔊 [NATIVE] Mic route: fast-path schedule (HAL default %u ≠ opened %u) debounce=%lldms "
                  @"stability=%lldms",
                  (unsigned)halNow, (unsigned)g_micOpenedInputDeviceId, (long long)(debounceNs / NSEC_PER_MSEC),
                  (long long)(stabilityDelayNs / NSEC_PER_MSEC));
        }

        const uint64_t wave = g_micRouteRestartGeneration.fetch_add(1, std::memory_order_acq_rel) + 1;

        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, debounceNs), SaysoMicRouteRestartQueue(), ^{
            if (!g_isMicCapturing) {
                return;
            }
            if (wave != g_micRouteRestartGeneration.load(std::memory_order_acquire)) {
                return;
            }

            AudioDeviceID firstDefault = SaysoGetCurrentDefaultInputDeviceID();
            if (firstDefault == kAudioObjectUnknown) {
                NSLog(@"🔊 [NATIVE] Mic route debounce fired: could not read default input — skipping restart");
                return;
            }

            dispatch_after(dispatch_time(DISPATCH_TIME_NOW, stabilityDelayNs), SaysoMicRouteRestartQueue(), ^{
                if (!g_isMicCapturing) {
                    return;
                }
                if (wave != g_micRouteRestartGeneration.load(std::memory_order_acquire)) {
                    return;
                }

                AudioDeviceID secondDefault = SaysoGetCurrentDefaultInputDeviceID();
                if (secondDefault == kAudioObjectUnknown) {
                    return;
                }
                if (firstDefault != secondDefault) {
                    NSLog(@"🔊 [NATIVE] Mic route: default input still changing (%u → %u) — coalescing again",
                          (unsigned)firstDefault, (unsigned)secondDefault);
                    SaysoScheduleMicRouteDebouncedRestart();
                    return;
                }
                SaysoPerformMicRestartIfCapturing(YES);
            });
        });
    });
}

// SAYSO-347: StartMicrophoneCapture used to collapse three distinct failure modes (wiring bug,
// teardown/lifecycle race, audio-route problem) into a bare `false`, indistinguishable in Sentry.
// Success still returns plain `true` for JS-side backward compatibility with a stale native build;
// failure now returns { ok: false, reason } so callers can propagate a specific, greppable cause.
static Local<Value> MicStartResult(bool ok, const char* reason = nullptr) {
    if (ok) {
        return Nan::New<v8::Boolean>(true);
    }
    Local<Object> result = Nan::New<Object>();
    Nan::Set(result, Nan::New("ok").ToLocalChecked(), Nan::New<v8::Boolean>(false));
    if (reason) {
        Nan::Set(result, Nan::New("reason").ToLocalChecked(), Nan::New<String>(reason).ToLocalChecked());
    }
    return result;
}

// Start microphone capture for streaming
NAN_METHOD(StartMicrophoneCapture) {
    NSLog(@"🎤 [NATIVE] Starting microphone capture");

    if (g_micStreamingCallback.IsEmpty()) {
        NSLog(@"⚠️ [NATIVE] Microphone streaming callback is empty - cannot capture microphone");
        info.GetReturnValue().Set(MicStartResult(false, "callback_empty"));
        return;
    }

    if (!g_micStreamingAsyncHandle) {
        g_micStreamingAsyncHandle = new uv_async_t;
        uv_async_init(uv_default_loop(), g_micStreamingAsyncHandle, MicStreamingAsyncCallback);
    }

    std::lock_guard<std::mutex> lk(g_micEngineLifecycleMutex);

    if (g_isMicCapturing) {
        NSLog(@"⚠️ [NATIVE] Microphone capture already active");
        info.GetReturnValue().Set(MicStartResult(false, "already_active"));
        return;
    }

    const int kFirstTapWaitMs = 1200;
    const int kRetryTapWaitMs = 1500;

    const char* failReason = nullptr;
    if (!TryStartMicrophoneCaptureOnce(kFirstTapWaitMs, &failReason)) {
        NSLog(@"🎤 [NATIVE] Mic: repeating capture once (cold start / Bluetooth input path) — first attempt failed: %s",
              failReason ? failReason : "unknown");
        if (!TryStartMicrophoneCaptureOnce(kRetryTapWaitMs, &failReason)) {
            NSLog(@"❌ [NATIVE] Microphone capture failed after retry: %s", failReason ? failReason : "unknown");
            g_micOpenedInputDeviceId = kAudioObjectUnknown;
            info.GetReturnValue().Set(MicStartResult(false, failReason ? failReason : "no_tap_buffers"));
            return;
        }
    }

    g_isMicCapturing = true;
    g_micOpenedInputDeviceId = SaysoGetCurrentDefaultInputDeviceID();
    NSLog(@"✅ [NATIVE] Microphone capture started successfully (first tap received); default input id=%u",
          (unsigned)g_micOpenedInputDeviceId);
    info.GetReturnValue().Set(MicStartResult(true));
}

// Stop microphone capture
NAN_METHOD(StopMicrophoneCapture) {
    NSLog(@"🎤 [NATIVE] Stopping microphone capture");

    g_micRouteRestartGeneration.fetch_add(1, std::memory_order_acq_rel);

    std::lock_guard<std::mutex> lk(g_micEngineLifecycleMutex);

    if (g_micInputNode) {
        [g_micInputNode removeTapOnBus:0];
        g_micInputNode = nullptr;
    }

    if (g_micEngine) {
        [g_micEngine stop];
        g_micEngine = nullptr;
    }

    // Note: On macOS, no audio session to deactivate

    g_isMicCapturing = false;
    g_micOpenedInputDeviceId = kAudioObjectUnknown;
    NSLog(@"✅ [NATIVE] Microphone capture stopped");
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(true));
}

// Check if mic capture is active
NAN_METHOD(IsMicrophoneCaptureActive) {
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(g_isMicCapturing));
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

// Set microphone-specific streaming callback
NAN_METHOD(SetMicrophoneStreamingCallback) {
    if (info.Length() < 1 || info[0]->IsNull() || info[0]->IsUndefined()) {
        // Clear callback
        g_micStreamingCallback.Reset();
        
        // Clean up async handle
        if (g_micStreamingAsyncHandle) {
            uv_async_t* handleToDelete = g_micStreamingAsyncHandle;
            g_micStreamingAsyncHandle = nullptr;
            uv_close((uv_handle_t*)handleToDelete, [](uv_handle_t* handle) {
                delete reinterpret_cast<uv_async_t*>(handle);
            });
        }
        
        info.GetReturnValue().Set(Nan::New<v8::Boolean>(true));
        return;
    }
    
    if (!info[0]->IsFunction()) {
        Nan::ThrowTypeError("Callback must be a function");
        return;
    }
    
    // Store persistent reference to callback
    g_micStreamingCallback.Reset(Nan::To<Function>(info[0]).ToLocalChecked());
    
    // Create async handle if it doesn't exist
    if (!g_micStreamingAsyncHandle) {
        g_micStreamingAsyncHandle = new uv_async_t();
        uv_async_init(uv_default_loop(), g_micStreamingAsyncHandle, MicStreamingAsyncCallback);
    }
    
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(true));
}

// Check if system audio capture is active
NAN_METHOD(IsSystemAudioCaptureActive) {
    // "Active" includes a start still settling: callers use this probe to decide whether teardown
    // is needed before a fresh start, and a pending start absolutely needs teardown (SAYSO-355 —
    // the wedge was invisible precisely because this returned false while a start was stuck).
    bool pendingStart = false;
    {
        std::lock_guard<std::mutex> lk(g_sckStartMutex);
        pendingStart = (g_sckStartPending != nullptr && !g_sckStartPending->settled);
    }
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(g_isCapturing || pendingStart));
}

// Register the native→JS lifecycle diagnostics callback (pass null to clear).
// The uv_async_t is created once on first registration and intentionally never closed — freeing
// it on callback-clear is the UAF class documented in SAYSO-349.
NAN_METHOD(SetLifecycleEventCallback) {
    if (info.Length() < 1 || info[0]->IsNull() || info[0]->IsUndefined()) {
        g_lifecycleCallback.Reset();
        info.GetReturnValue().Set(Nan::New<v8::Boolean>(true));
        return;
    }
    if (!info[0]->IsFunction()) {
        Nan::ThrowTypeError("Callback must be a function");
        return;
    }
    g_lifecycleCallback.Reset(Nan::To<Function>(info[0]).ToLocalChecked());
    {
        std::lock_guard<std::mutex> lk(g_lifecycleMutex);
        if (!g_lifecycleAsyncHandle) {
            g_lifecycleAsyncHandle = new uv_async_t();
            uv_async_init(uv_default_loop(), g_lifecycleAsyncHandle, LifecycleAsyncCallback);
        }
    }
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(true));
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
    
    Nan::Set(target, Nan::New("checkScreenRecordingGranted").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(CheckScreenRecordingGranted)).ToLocalChecked());

    Nan::Set(target, Nan::New("requestScreenRecordingPermission").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(RequestScreenRecordingPermission)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("startSystemAudioCapture").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(StartSystemAudioCapture)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("stopSystemAudioCapture").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(StopSystemAudioCapture)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("setStreamingCallback").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(SetStreamingCallback)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("setMicrophoneStreamingCallback").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(SetMicrophoneStreamingCallback)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("isSystemAudioCaptureActive").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(IsSystemAudioCaptureActive)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("listOutputDevices").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(ListOutputDevices)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("createMultiOutputDevice").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(CreateMultiOutputDevice)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("deleteMultiOutputDevice").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(DeleteMultiOutputDevice)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("startMicrophoneCapture").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(StartMicrophoneCapture)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("stopMicrophoneCapture").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(StopMicrophoneCapture)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("isMicrophoneCaptureActive").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(IsMicrophoneCaptureActive)).ToLocalChecked());

    Nan::Set(target, Nan::New("setLifecycleEventCallback").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(SetLifecycleEventCallback)).ToLocalChecked());
}

NODE_MODULE(native_audio, Init)