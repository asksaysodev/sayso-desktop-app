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
#include <CoreAudio/CATapDescription.h>
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
#include <time.h>

using namespace v8;
static SCStream* g_stream = nullptr;
static SCStreamConfiguration* g_config = nullptr;
static SCContentFilter* g_filter = nullptr;
static dispatch_queue_t g_audioQueue = nullptr;
static bool g_isCapturing = false;
static std::string g_audioFilePath = "";
static AudioFileID g_audioFile = nullptr;
static AudioStreamBasicDescription g_audioFormat = {0};
static bool g_audioFileInitialized = false;
static std::string g_outputDirectory = "";
static bool g_formatDetected = false;
static AudioStreamBasicDescription g_sourceFormat = {0};
static AudioStreamBasicDescription g_fileFormat = {0};
static double g_actualStartMs = 0.0;
static bool g_streamingOnly = false;
static Nan::Persistent<v8::Function> g_streamingCallback;
static uv_async_t* g_streamingAsyncHandle = nullptr;
static Nan::Persistent<v8::Function> g_micStreamingCallback;
static uv_async_t* g_micStreamingAsyncHandle = nullptr;
static std::mutex g_lifecycleMutex;
static std::vector<std::string> g_lifecycleQueue;
static uv_async_t* g_lifecycleAsyncHandle = nullptr;
static Nan::Persistent<v8::Function> g_lifecycleCallback;
static AVAudioEngine* g_micEngine = nullptr;
static AVAudioInputNode* g_micInputNode = nullptr;
static id<NSObject> g_micEngineConfigChangeObserver = nil;
static std::atomic<int64_t> g_micEngineObserverAttachedAtMs{0};
static const int64_t kMicEngineConfigChangeCooldownMs = 300;
static std::atomic<bool> g_isMicCapturing{false};
static bool g_defaultInputListenerRegistered = false;
static AudioDeviceID g_lastKnownDefaultInputDevice = kAudioObjectUnknown;
static std::atomic<AudioDeviceID> g_micOpenedInputDeviceId{kAudioObjectUnknown};
static std::atomic<uint64_t> g_micRouteRestartGeneration{0};
static std::atomic<bool> g_micFirstTapSeen{false};
static std::atomic<bool> g_micRouteRecovering{false};
static int64_t g_micRouteRecoveryStartMs = 0;

static std::atomic<int64_t> g_micLastDefaultInputChangeMs{0};
static std::atomic<int64_t> g_micLastEngineConfigChangeMs{0};
static std::atomic<uint64_t> g_micEngineConfigChanges{0};
static std::atomic<uint64_t> g_micEngineBuilds{0};
static std::atomic<int64_t> g_micEngineBuiltAtMs{0};
static std::atomic<bool> g_micEngineRunning{false};
static std::atomic<uint64_t> g_micTapCallbacks{0};
static std::atomic<uint64_t> g_micTapDelivered{0};
static std::atomic<uint64_t> g_micTapDroppedNoHandle{0};
static std::atomic<uint64_t> g_micTapDroppedNoCallback{0};
static std::atomic<uint64_t> g_micTapDroppedEmpty{0};
static std::atomic<uint64_t> g_micTapDroppedLayout{0};
static std::atomic<uint64_t> g_micTapDroppedUnsupported{0};
static std::atomic<int64_t> g_micTapLastCallbackMs{0};
static std::atomic<int64_t> g_micTapLastDeliveredMs{0};
static std::atomic<int> g_micDiagTapSampleRate{0};
static std::atomic<int> g_micDiagTapChannels{0};
static std::atomic<unsigned long> g_micDiagTapCommonFormat{0};

static void SaysoScheduleMicRouteDebouncedRestart();
static int64_t SaysoNowMs();

static int64_t SaysoRealtimeNowMs() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (int64_t)ts.tv_sec * 1000 + (int64_t)(ts.tv_nsec / 1000000);
}

struct StreamingData {
    std::vector<char> audioData;
    double sampleRate;
    int channels;
    int bitDepth;
    bool isFloat;
    float gainDb = 0;
};

static void StreamingAsyncCallback(uv_async_t* handle) {
    Nan::HandleScope scope;
    
    StreamingData* data = static_cast<StreamingData*>(handle->data);
    if (!data || g_streamingCallback.IsEmpty()) {
        return;
    }
    
    Local<Object> buffer = Nan::CopyBuffer(data->audioData.data(), data->audioData.size()).ToLocalChecked();
    
    Local<Object> format = Nan::New<Object>();
    Nan::Set(format, Nan::New("sampleRate").ToLocalChecked(), Nan::New<Number>(data->sampleRate));
    Nan::Set(format, Nan::New("channels").ToLocalChecked(), Nan::New<Integer>(data->channels));
    Nan::Set(format, Nan::New("bitDepth").ToLocalChecked(), Nan::New<Integer>(data->bitDepth));
    Nan::Set(format, Nan::New("isFloat").ToLocalChecked(), Nan::New<v8::Boolean>(data->isFloat));
    
    Local<Function> callback = Nan::New(g_streamingCallback);
    Local<Value> argv[2] = {buffer, format};
    Nan::Call(callback, Nan::GetCurrentContext()->Global(), 2, argv);
    
    delete data;
    handle->data = nullptr;
}

static void MicStreamingAsyncCallback(uv_async_t* handle) {
    Nan::HandleScope scope;
    
    StreamingData* data = static_cast<StreamingData*>(handle->data);
    if (!data || g_micStreamingCallback.IsEmpty()) {
        if (data) delete data;
        return;
    }
    
    Local<Object> buffer = Nan::CopyBuffer(data->audioData.data(), data->audioData.size()).ToLocalChecked();
    
    Local<Object> format = Nan::New<Object>();
    Nan::Set(format, Nan::New("sampleRate").ToLocalChecked(), Nan::New<Number>(data->sampleRate));
    Nan::Set(format, Nan::New("channels").ToLocalChecked(), Nan::New<Integer>(data->channels));
    Nan::Set(format, Nan::New("bitDepth").ToLocalChecked(), Nan::New<Integer>(data->bitDepth));
    Nan::Set(format, Nan::New("isFloat").ToLocalChecked(), Nan::New<v8::Boolean>(data->isFloat));
    
    Nan::Set(format, Nan::New("gainDb").ToLocalChecked(), Nan::New<Number>(data->gainDb));

    Local<Function> callback = Nan::New(g_micStreamingCallback);
    Local<Value> argv[2] = {buffer, format};
    Nan::Call(callback, Nan::GetCurrentContext()->Global(), 2, argv);

    delete data;
    handle->data = nullptr;
}

static void LifecycleAsyncCallback(uv_async_t* ) {
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

static void EmitLifecycleEvent(const std::string& message) {
    NSLog(@"🔎 [NATIVE] %s", message.c_str());
    std::lock_guard<std::mutex> lk(g_lifecycleMutex);
    if (!g_lifecycleAsyncHandle) {
        return;
    }
    g_lifecycleQueue.push_back(message);
    uv_async_send(g_lifecycleAsyncHandle);
}

static bool convertInt32ToFloat(const char* sourceData, size_t sourceSize, char* destData, size_t& destSize) {
    if (!sourceData || !destData) return false;
    
    if (destSize < sourceSize) return false;
    
    const int32_t* intSamples = reinterpret_cast<const int32_t*>(sourceData);
    float* floatSamples = reinterpret_cast<float*>(destData);
    size_t sampleCount = sourceSize / sizeof(int32_t);
    
    const float scale = 1.0f / (float)INT32_MAX;

    for (size_t i = 0; i < sampleCount; i++) {
        floatSamples[i] = (float)intSamples[i] * scale;
    }

    destSize = sampleCount * sizeof(float);
    return true;
}

static bool interleaveAudio(const char* sourceData, size_t sourceSize, char* destData, size_t& destSize, int channels, int bytesPerSample) {
    if (!sourceData || !destData || channels <= 0) return false;
    
    size_t totalSamples = sourceSize / bytesPerSample;
    size_t samplesPerChannel = totalSamples / channels;
    
    if (destSize < sourceSize) return false;
    
    if (bytesPerSample == 4) {
        const int32_t* source = reinterpret_cast<const int32_t*>(sourceData);
        int32_t* dest = reinterpret_cast<int32_t*>(destData);
        
        for (size_t i = 0; i < samplesPerChannel; i++) {
            for (int ch = 0; ch < channels; ch++) {
                dest[i * channels + ch] = source[ch * samplesPerChannel + i];
            }
        }
    } else if (bytesPerSample == 2) {
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
    
    size_t requiredDestSize = sampleCount * sizeof(int16_t);
    if (destSize < requiredDestSize) return false;
    
    for (size_t i = 0; i < sampleCount; i++) {
        int32_t sample = int32Samples[i];
        
        float normalizedSample = (float)sample / (float)INT32_MAX;
        
        if (normalizedSample > 1.0f) normalizedSample = 1.0f;
        else if (normalizedSample < -1.0f) normalizedSample = -1.0f;
        
        int32_t scaledSample = (int32_t)(normalizedSample * (float)INT16_MAX);
        
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

static bool initializeAudioFile() {
    if (g_audioFileInitialized) {
        return true;
    }
    
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
    
    NSDateFormatter* formatter = [[NSDateFormatter alloc] init];
    [formatter setDateFormat:@"yyyy-MM-dd_HH-mm-ss"];
    NSString* timestamp = [formatter stringFromDate:[NSDate date]];
    NSString* filename = [NSString stringWithFormat:@"system_audio_%@.caf", timestamp];
    NSString* filePath = [outputDir stringByAppendingPathComponent:filename];
    
    if (g_audioFormat.mSampleRate == 0) {
        NSLog(@"❌ [NATIVE] No valid audio format detected yet");
        return false;
    }
    
    NSLog(@"🎵 [NATIVE] Creating CAF file with original format: %.0f Hz, %d channels, %d bits, flags: 0x%x", 
          g_audioFormat.mSampleRate, g_audioFormat.mChannelsPerFrame, g_audioFormat.mBitsPerChannel, g_audioFormat.mFormatFlags);
    
    AudioStreamBasicDescription cafFormat = g_audioFormat;
    
    NSLog(@"🎵 [NATIVE] Source format: %.0f Hz, %d channels, %d bits, flags: 0x%x", 
          g_audioFormat.mSampleRate, g_audioFormat.mChannelsPerFrame, g_audioFormat.mBitsPerChannel, g_audioFormat.mFormatFlags);
    
    double originalSampleRate = g_audioFormat.mSampleRate;
    NSLog(@"🎵 [NATIVE] PRESERVING original sample rate: %.0f Hz", originalSampleRate);
    
    cafFormat.mFormatID = kAudioFormatLinearPCM;
    
    if (cafFormat.mBitsPerChannel == 32) {
        cafFormat.mBitsPerChannel = 32;
        cafFormat.mFormatFlags = kAudioFormatFlagIsFloat | kAudioFormatFlagIsPacked;
        
        cafFormat.mBytesPerFrame = cafFormat.mChannelsPerFrame * (cafFormat.mBitsPerChannel / 8);
        cafFormat.mFramesPerPacket = 1;
        cafFormat.mBytesPerPacket = cafFormat.mBytesPerFrame * cafFormat.mFramesPerPacket;
        
        NSLog(@"🎵 [NATIVE] Fixed byte calculations: BytesPerFrame=%d, FramesPerPacket=%d, BytesPerPacket=%d", 
              cafFormat.mBytesPerFrame, cafFormat.mFramesPerPacket, cafFormat.mBytesPerPacket);
        
        cafFormat.mFormatFlags &= ~(kAudioFormatFlagIsNonInterleaved);
        
        NSLog(@"🎵 [NATIVE] Converting 32-bit int to 32-bit float for compatibility (flags: 0x%x -> 0x%x)", 
              g_audioFormat.mFormatFlags, cafFormat.mFormatFlags);
        NSLog(@"🎵 [NATIVE] 32-bit float format details: BytesPerFrame=%d, FramesPerPacket=%d, BytesPerPacket=%d, FormatFlags=0x%x", 
              cafFormat.mBytesPerFrame, cafFormat.mFramesPerPacket, cafFormat.mBytesPerPacket, cafFormat.mFormatFlags);
    } else {
        if (cafFormat.mFormatFlags & kAudioFormatFlagIsFloat) {
            cafFormat.mFormatFlags = kAudioFormatFlagIsFloat | kAudioFormatFlagIsPacked;
        } else if (cafFormat.mBitsPerChannel == 24) {
            cafFormat.mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
        } else if (cafFormat.mBitsPerChannel == 16) {
            cafFormat.mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
        } else {
            cafFormat.mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
        }
        
        cafFormat.mFormatFlags &= ~kAudioFormatFlagIsNonInterleaved;
        
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
    
    
    NSLog(@"🎵 [NATIVE] Creating CAF with normalized format: %.0f Hz, %d channels, %d bits, flags: 0x%x", 
          cafFormat.mSampleRate, cafFormat.mChannelsPerFrame, cafFormat.mBitsPerChannel, cafFormat.mFormatFlags);
    
    OSStatus status = AudioFileCreateWithURL((__bridge CFURLRef)[NSURL fileURLWithPath:filePath],
                                           kAudioFileCAFType,
                                           &cafFormat,
                                           kAudioFileFlags_EraseFile,
                                           &g_audioFile);
    
    if (status != noErr) {
        NSLog(@"❌ [NATIVE] CAF failed (%d), trying WAV format", (int)status);
        
        AudioStreamBasicDescription wavFormat = g_audioFormat;
        wavFormat.mFormatID = kAudioFormatLinearPCM;
        
        if (wavFormat.mBitsPerChannel == 32) {
            wavFormat.mBitsPerChannel = 32;
            wavFormat.mFormatFlags = kAudioFormatFlagIsFloat | kAudioFormatFlagIsPacked;
            
            wavFormat.mBytesPerFrame = wavFormat.mChannelsPerFrame * (wavFormat.mBitsPerChannel / 8);
            wavFormat.mFramesPerPacket = 1;
            wavFormat.mBytesPerPacket = wavFormat.mBytesPerFrame * wavFormat.mFramesPerPacket;
            
            wavFormat.mFormatFlags &= ~(kAudioFormatFlagIsNonInterleaved);
            
            NSLog(@"🎵 [NATIVE] Converting 32-bit int to 32-bit float for WAV compatibility (flags: 0x%x -> 0x%x)", 
                  g_audioFormat.mFormatFlags, wavFormat.mFormatFlags);
            NSLog(@"🎵 [NATIVE] WAV 32-bit float format details: BytesPerFrame=%d, FramesPerPacket=%d, BytesPerPacket=%d, FormatFlags=0x%x", 
                  wavFormat.mBytesPerFrame, wavFormat.mFramesPerPacket, wavFormat.mBytesPerPacket, wavFormat.mFormatFlags);
        } else {
            if (wavFormat.mFormatFlags & kAudioFormatFlagIsFloat) {
                wavFormat.mFormatFlags = kAudioFormatFlagIsFloat | kAudioFormatFlagIsPacked;
            } else if (wavFormat.mBitsPerChannel == 24) {
                wavFormat.mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
            } else if (wavFormat.mBitsPerChannel == 16) {
                wavFormat.mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
            } else {
                wavFormat.mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
            }
            
            wavFormat.mFormatFlags &= ~kAudioFormatFlagIsNonInterleaved;
            
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
        
        
        NSLog(@"🎵 [NATIVE] WAV fallback normalized format: %.0f Hz, %d channels, %d bits, flags: 0x%x", 
              wavFormat.mSampleRate, wavFormat.mChannelsPerFrame, wavFormat.mBitsPerChannel, wavFormat.mFormatFlags);
        
        NSString* wavFilePath = [filePath stringByReplacingOccurrencesOfString:@".caf" withString:@".wav"];
        
        status = AudioFileCreateWithURL((__bridge CFURLRef)[NSURL fileURLWithPath:wavFilePath],
                                      kAudioFileWAVEType,
                                      &wavFormat,
                                      kAudioFileFlags_EraseFile,
                                      &g_audioFile);
        
        if (status == noErr) {
            g_audioFilePath = [wavFilePath UTF8String];
            NSLog(@"✅ [NATIVE] Created WAV fallback file: %@", wavFilePath);
        } else {
            NSLog(@"❌ [NATIVE] Both CAF and WAV failed: %d", (int)status);
            return false;
        }
    } else {
        g_audioFilePath = [filePath UTF8String];
        NSLog(@"✅ [NATIVE] CAF audio file created: %@", filePath);
    }
    
    g_audioFileInitialized = true;
    
    return true;
}

static void closeAudioFile() {
    if (g_audioFile) {
        AudioFileClose(g_audioFile);
        g_audioFile = nullptr;
    }
    
    g_audioFileInitialized = false;
    g_formatDetected = false;
    
    memset(&g_sourceFormat, 0, sizeof(AudioStreamBasicDescription));
    memset(&g_fileFormat, 0, sizeof(AudioStreamBasicDescription));
    memset(&g_audioFormat, 0, sizeof(AudioStreamBasicDescription));
    
    NSLog(@"✅ [NATIVE] Audio file closed: %s", g_audioFilePath.c_str());
}

static void audioCallback(CMSampleBufferRef sampleBuffer) {
    static int callbackCount = 0;
    callbackCount++;
    
    
    if (!g_audioFileInitialized) {
        
        CMFormatDescriptionRef formatDesc = CMSampleBufferGetFormatDescription(sampleBuffer);
        if (formatDesc) {
            const AudioStreamBasicDescription* sourceFormat = CMAudioFormatDescriptionGetStreamBasicDescription(formatDesc);
            if (sourceFormat) {
                g_sourceFormat = *sourceFormat;
                g_fileFormat = *sourceFormat;
                g_audioFormat = *sourceFormat;
                
                NSLog(@"🎵 [NATIVE] ✅ DETECTED FORMAT: %.0f Hz, %d channels, %d bits, flags: 0x%x", 
                      g_audioFormat.mSampleRate, g_audioFormat.mChannelsPerFrame, g_audioFormat.mBitsPerChannel, g_audioFormat.mFormatFlags);
                NSLog(@"🎵 [NATIVE] Original byte calculations: BytesPerFrame=%d, FramesPerPacket=%d, BytesPerPacket=%d", 
                      g_audioFormat.mBytesPerFrame, g_audioFormat.mFramesPerPacket, g_audioFormat.mBytesPerPacket);
                
                if (g_audioFormat.mBitsPerChannel == 32 && g_audioFormat.mChannelsPerFrame == 2) {
                    g_audioFormat.mBytesPerFrame = 8;
                    g_audioFormat.mFramesPerPacket = 1;
                    g_audioFormat.mBytesPerPacket = 8;
                    NSLog(@"🎵 [NATIVE] FIXED byte calculations for 32-bit stereo: BytesPerFrame=%d, FramesPerPacket=%d, BytesPerPacket=%d", 
                          g_audioFormat.mBytesPerFrame, g_audioFormat.mFramesPerPacket, g_audioFormat.mBytesPerPacket);
                }
                
                NSLog(@"🎵 [NATIVE] CRITICAL: Sample rate being preserved as %.0f Hz", g_audioFormat.mSampleRate);
                
                CMTime presentationTime = CMSampleBufferGetPresentationTimeStamp(sampleBuffer);
                CMTime duration = CMSampleBufferGetDuration(sampleBuffer);
                NSLog(@"🎵 [NATIVE] First sample timing: Presentation=%.3f, Duration=%.6f", 
                      CMTimeGetSeconds(presentationTime), CMTimeGetSeconds(duration));
                
                CMBlockBufferRef blockBuffer = CMSampleBufferGetDataBuffer(sampleBuffer);
                if (blockBuffer) {
                    size_t dataLength = CMBlockBufferGetDataLength(blockBuffer);
                    double durationSeconds = CMTimeGetSeconds(duration);
                    
                    size_t bytesPerSample = 4;
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
        
        if (!g_streamingOnly) {
            if (!initializeAudioFile()) {
                NSLog(@"❌ [NATIVE] Failed to initialize audio file");
                return;
            }
            NSLog(@"🎤 [NATIVE] Audio file initialized, starting continuous recording");
        } else {
            NSLog(@"🎤 [NATIVE] Format detected, streaming-only mode - skipping file initialization");
        }
        
        g_audioFileInitialized = true;
    }
    
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
        return;
    }
    
    
    if (callbackCount <= 5) {
        NSLog(@"🎵 [NATIVE] Audio data length: %zu bytes", dataLength);
    }
    
    char* sourceAudioData = nullptr;
    OSStatus status = CMBlockBufferGetDataPointer(blockBuffer, 0, nullptr, nullptr, &sourceAudioData);
    if (status != noErr || !sourceAudioData) {
        if (callbackCount <= 5) {
            NSLog(@"❌ [NATIVE] Failed to get audio data pointer: %d", (int)status);
        }
        return;
    }
    
    bool isNonInterleaved = (g_sourceFormat.mFormatFlags & kAudioFormatFlagIsNonInterleaved) != 0;
    bool is32BitInt = (g_sourceFormat.mBitsPerChannel == 32) && 
                      (g_sourceFormat.mFormatFlags & kAudioFormatFlagIsSignedInteger);
    
    const char* audioDataToWrite = sourceAudioData;
    UInt32 bytesToWrite = (UInt32)dataLength;
    static char conversionBuffer[1024 * 1024];
    
    if (callbackCount <= 5) {
        NSLog(@"🔍 [PROCESS] NonInterleaved=%d, 32BitInt=%d", isNonInterleaved, is32BitInt);
    }
    
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
    
    if (!g_streamingOnly) {
        if (callbackCount <= 5) {
            NSLog(@"📝 [WRITE] Writing %u bytes to file", bytesToWrite);
        }
        
        if (g_audioFile) {
            SInt64 filePosition = 0;
            UInt32 size = sizeof(filePosition);
            OSStatus status = AudioFileGetProperty(g_audioFile, kAudioFilePropertyAudioDataByteCount, &size, &filePosition);
            if (status != noErr) {
                NSLog(@"❌ [NATIVE] Failed to get audio file position: %d", (int)status);
                return;
            }
            
            status = AudioFileWriteBytes(g_audioFile, false, filePosition, &bytesToWrite, audioDataToWrite);
            
            if (status != noErr) {
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
    }
    
    if (!g_streamingCallback.IsEmpty() && g_streamingAsyncHandle) {
        StreamingData* data = new StreamingData();
        data->audioData.assign(audioDataToWrite, audioDataToWrite + bytesToWrite);
        data->sampleRate = g_audioFormat.mSampleRate;
        data->channels = g_audioFormat.mChannelsPerFrame;
        data->bitDepth = g_audioFormat.mBitsPerChannel;
        data->isFloat = (g_audioFormat.mFormatFlags & kAudioFormatFlagIsFloat) != 0;
        
        g_streamingAsyncHandle->data = data;
        uv_async_send(g_streamingAsyncHandle);
    }
}

@interface AudioCaptureDelegate : NSObject <SCStreamDelegate, SCStreamOutput>
@property (nonatomic, copy) void (^audioCallback)(CMSampleBufferRef);
@end

@implementation AudioCaptureDelegate

- (void)stream:(SCStream *)stream 
    didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer 
    ofType:(SCStreamOutputType)type {
    if (type == SCStreamOutputTypeAudio) {
        if (stream != g_stream) {
            return;
        }
        if (g_actualStartMs == 0.0) {
            g_actualStartMs = (double)SaysoNowMs();
        }
        if (self.audioCallback) {
            self.audioCallback(sampleBuffer);
        }
    } else {
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

enum class SckStartStage {
    not_started,
    getting_shareable_content,
    adding_stream_output,
    starting_capture,
};

static const char* SckStartStageName(SckStartStage stage) {
    switch (stage) {
        case SckStartStage::not_started: return "not_started";
        case SckStartStage::getting_shareable_content: return "getting_shareable_content";
        case SckStartStage::adding_stream_output: return "adding_stream_output";
        case SckStartStage::starting_capture: return "starting_capture";
    }
    return "unknown";
}

struct PendingSckStart {
    uv_async_t async;
    Nan::Persistent<v8::Promise::Resolver> resolver;
    uint64_t generation;
    bool settled;
    bool reject;
    std::string message;
    SckStartStage stage;
};

static PendingSckStart* g_sckStartPending = nullptr;
static std::mutex g_sckStartMutex;
static uint64_t g_sckStartNextGeneration = 0;

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
    uv_async_send(&winner->async);
    return true;
}

static bool SckStartAbandoned(uint64_t generation) {
    std::lock_guard<std::mutex> lk(g_sckStartMutex);
    return !g_sckStartPending || g_sckStartPending->generation != generation || g_sckStartPending->settled;
}

static void SckStartAdvanceStage(uint64_t generation, SckStartStage stage) {
    std::lock_guard<std::mutex> lk(g_sckStartMutex);
    if (g_sckStartPending && g_sckStartPending->generation == generation && !g_sckStartPending->settled) {
        g_sckStartPending->stage = stage;
    }
}

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

static int64_t SaysoNowMs() {
    return (int64_t)([[NSDate date] timeIntervalSince1970] * 1000.0);
}

static bool SaysoIsBluetoothInputDevice(AudioDeviceID deviceID) {
    if (deviceID == kAudioObjectUnknown) {
        return false;
    }
    UInt32 transportType = 0;
    UInt32 sz = sizeof(transportType);
    AudioObjectPropertyAddress pa = {
        kAudioDevicePropertyTransportType,
        kAudioObjectPropertyScopeGlobal,
        kAudioObjectPropertyElementMain
    };
    if (AudioObjectGetPropertyData(deviceID, &pa, 0, nullptr, &sz, &transportType) != noErr) {
        return true;
    }
    return transportType == kAudioDeviceTransportTypeBluetooth ||
           transportType == kAudioDeviceTransportTypeBluetoothLE;
}


static const char* SaysoTransportTypeName(UInt32 transport) {
    switch (transport) {
        case 'bltn': return "built_in";
        case 'blue': return "bluetooth";
        case 'blea': return "bluetooth_le";
        case 'usb ': return "usb";
        case 'virt': return "virtual";
        case 'grup': return "aggregate";
        case 'fgrp': return "auto_aggregate";
        case 'hdmi': return "hdmi";
        case 'dprt': return "displayport";
        case 'airp': return "airplay";
        case 'thun': return "thunderbolt";
        case 'pci ': return "pci";
        case '1394': return "firewire";
        case 'eavb': return "avb";
        case 'ccwd': return "continuity_wired";
        case 'ccwl': return "continuity_wireless";
        case 0: return "unknown";
        default: return "other";
    }
}

static bool SaysoReadHalPod(AudioObjectID object, AudioObjectPropertySelector selector,
                            AudioObjectPropertyScope scope, AudioObjectPropertyElement element,
                            void* out, UInt32 size) {
    if (object == kAudioObjectUnknown) {
        return false;
    }
    AudioObjectPropertyAddress pa = { selector, scope, element };
    if (!AudioObjectHasProperty(object, &pa)) {
        return false;
    }
    UInt32 sz = size;
    return AudioObjectGetPropertyData(object, &pa, 0, nullptr, &sz, out) == noErr && sz == size;
}

static bool SaysoReadHalString(AudioObjectID object, AudioObjectPropertySelector selector, std::string& out) {
    CFStringRef ref = nullptr;
    if (!SaysoReadHalPod(object, selector, kAudioObjectPropertyScopeGlobal, kAudioObjectPropertyElementMain,
                         &ref, sizeof(ref)) || !ref) {
        return false;
    }
    char buf[512];
    bool ok = CFStringGetCString(ref, buf, sizeof(buf), kCFStringEncodingUTF8);
    CFRelease(ref);
    if (ok) {
        out = buf;
    }
    return ok;
}

static int SaysoReadInputChannelCount(AudioObjectID device) {
    AudioObjectPropertyAddress pa = {
        kAudioDevicePropertyStreamConfiguration, kAudioObjectPropertyScopeInput, kAudioObjectPropertyElementMain
    };
    if (device == kAudioObjectUnknown || !AudioObjectHasProperty(device, &pa)) {
        return -1;
    }
    UInt32 size = 0;
    if (AudioObjectGetPropertyDataSize(device, &pa, 0, nullptr, &size) != noErr || size < sizeof(AudioBufferList)) {
        return -1;
    }
    std::vector<uint8_t> storage(size);
    AudioBufferList* abl = reinterpret_cast<AudioBufferList*>(storage.data());
    if (AudioObjectGetPropertyData(device, &pa, 0, nullptr, &size, abl) != noErr) {
        return -1;
    }
    int channels = 0;
    for (UInt32 i = 0; i < abl->mNumberBuffers; i++) {
        channels += (int)abl->mBuffers[i].mNumberChannels;
    }
    return channels;
}

static bool SaysoReadInputScalar(AudioObjectID device, AudioObjectPropertySelector selector, void* out, UInt32 size) {
    return SaysoReadHalPod(device, selector, kAudioObjectPropertyScopeInput, kAudioObjectPropertyElementMain, out, size) ||
           SaysoReadHalPod(device, selector, kAudioObjectPropertyScopeInput, 1, out, size);
}

struct SaysoMicDeviceSnapshot {
    AudioDeviceID id = kAudioObjectUnknown;
    bool hasName = false;
    std::string name;
    bool hasUid = false;
    std::string uid;
    UInt32 transport = 0;
    Float64 nominalSampleRate = -1;
    int inputChannels = -1;
    int isAlive = -1;
    int isRunningSomewhere = -1;
    bool hasHogPid = false;
    pid_t hogPid = -1;
    int muted = -1;
    Float32 volume = -1;
};

static SaysoMicDeviceSnapshot SaysoSnapshotMicDevice(AudioDeviceID device) {
    SaysoMicDeviceSnapshot snap;
    snap.id = device;
    if (device == kAudioObjectUnknown) {
        return snap;
    }
    snap.hasName = SaysoReadHalString(device, kAudioObjectPropertyName, snap.name);
    snap.hasUid = SaysoReadHalString(device, kAudioDevicePropertyDeviceUID, snap.uid);
    SaysoReadHalPod(device, kAudioDevicePropertyTransportType, kAudioObjectPropertyScopeGlobal,
                    kAudioObjectPropertyElementMain, &snap.transport, sizeof(snap.transport));
    SaysoReadHalPod(device, kAudioDevicePropertyNominalSampleRate, kAudioObjectPropertyScopeGlobal,
                    kAudioObjectPropertyElementMain, &snap.nominalSampleRate, sizeof(snap.nominalSampleRate));
    snap.inputChannels = SaysoReadInputChannelCount(device);
    UInt32 flag = 0;
    if (SaysoReadHalPod(device, kAudioDevicePropertyDeviceIsAlive, kAudioObjectPropertyScopeGlobal,
                        kAudioObjectPropertyElementMain, &flag, sizeof(flag))) {
        snap.isAlive = flag ? 1 : 0;
    }
    if (SaysoReadHalPod(device, kAudioDevicePropertyDeviceIsRunningSomewhere, kAudioObjectPropertyScopeGlobal,
                        kAudioObjectPropertyElementMain, &flag, sizeof(flag))) {
        snap.isRunningSomewhere = flag ? 1 : 0;
    }
    snap.hasHogPid = SaysoReadHalPod(device, kAudioDevicePropertyHogMode, kAudioObjectPropertyScopeGlobal,
                                     kAudioObjectPropertyElementMain, &snap.hogPid, sizeof(snap.hogPid));
    if (SaysoReadInputScalar(device, kAudioDevicePropertyMute, &flag, sizeof(flag))) {
        snap.muted = flag ? 1 : 0;
    }
    SaysoReadInputScalar(device, kAudioDevicePropertyVolumeScalar, &snap.volume, sizeof(snap.volume));
    return snap;
}

static std::string SaysoEventSafe(const char* value) {
    std::string out = value ? value : "";
    for (char& c : out) {
        if (c == '"' || c == '\n' || c == '\r') {
            c = '\'';
        }
    }
    return out;
}

static OSStatus SaysoDefaultInputDeviceListenerProc(AudioObjectID ,
                                                    UInt32 inNumberAddresses,
                                                    const AudioObjectPropertyAddress* inAddresses,
                                                    void* ) {
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
        g_micLastDefaultInputChangeMs.store(SaysoRealtimeNowMs());

        dispatch_async(dispatch_get_main_queue(), ^{
            NSString* prevName = SaysoCopyAudioDeviceName(previous);
            NSString* newName = SaysoCopyAudioDeviceName(newDevice);
            NSString* newUid = SaysoCopyAudioDeviceUID(newDevice);
            NSLog(@"🔊 [NATIVE] Default INPUT changed: %u \"%@\" -> %u \"%@\" uid=\"%@\" | Sayso micCaptureActive=%s",
                  (unsigned)previous, prevName,
                  (unsigned)newDevice, newName,
                  newUid ? newUid : @"(nil)",
                  g_isMicCapturing ? "YES" : "NO");
            UInt32 prevTransport = 0;
            UInt32 newTransport = 0;
            SaysoReadHalPod(previous, kAudioDevicePropertyTransportType, kAudioObjectPropertyScopeGlobal,
                            kAudioObjectPropertyElementMain, &prevTransport, sizeof(prevTransport));
            SaysoReadHalPod(newDevice, kAudioDevicePropertyTransportType, kAudioObjectPropertyScopeGlobal,
                            kAudioObjectPropertyElementMain, &newTransport, sizeof(newTransport));
            EmitLifecycleEvent(std::string("mic_default_input_changed from=\"") + SaysoEventSafe([prevName UTF8String]) +
                               "\" from_transport=" + SaysoTransportTypeName(prevTransport) +
                               " to=\"" + SaysoEventSafe([newName UTF8String]) +
                               "\" to_transport=" + SaysoTransportTypeName(newTransport) +
                               " capturing=" + (g_isMicCapturing ? "1" : "0"));
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

NAN_METHOD(Initialize) {
    
    g_audioQueue = dispatch_queue_create("com.sayso.audio", DISPATCH_QUEUE_SERIAL);
    
    g_delegate = [[AudioCaptureDelegate alloc] init];

    SaysoRegisterDefaultInputDeviceListener();
    
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(true));
}

NAN_METHOD(CheckScreenRecordingGranted) {
    bool granted = CGPreflightScreenCaptureAccess();
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(granted));
}

NAN_METHOD(RequestScreenRecordingPermission) {
    bool granted = CGRequestScreenCaptureAccess();
    NSLog(@"🎤 [NATIVE] RequestScreenRecordingPermission → granted=%d", granted);
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(granted));
}

NAN_METHOD(CheckAudioCaptureCapability) {
    Local<Object> result = Nan::New<Object>();

    if (@available(macOS 14.4, *)) {
        Nan::Set(result, Nan::New("macOS14_4Plus").ToLocalChecked(), Nan::New<v8::Boolean>(true));

        CATapDescription* description =
            [[CATapDescription alloc] initStereoGlobalTapButExcludeProcesses:@[]];
        description.name = @"Sayso System Audio Tap Probe";

        AudioObjectID tapID = kAudioObjectUnknown;
        OSStatus status = AudioHardwareCreateProcessTap(description, &tapID);

        if (status == noErr && tapID != kAudioObjectUnknown) {
            AudioHardwareDestroyProcessTap(tapID);
            NSLog(@"✅ [NATIVE] Process-tap capability probe succeeded");
            Nan::Set(result, Nan::New("available").ToLocalChecked(), Nan::New<v8::Boolean>(true));
            Nan::Set(result, Nan::New("error").ToLocalChecked(), Nan::Null());
        } else {
            NSLog(@"❌ [NATIVE] Process-tap capability probe failed: OSStatus=%d", (int)status);
            Nan::Set(result, Nan::New("available").ToLocalChecked(), Nan::New<v8::Boolean>(false));
            char errBuf[48];
            snprintf(errBuf, sizeof(errBuf), "OSStatus %d", (int)status);
            Nan::Set(result, Nan::New("error").ToLocalChecked(),
                     Nan::New<v8::String>(errBuf).ToLocalChecked());
        }
    } else {
        NSLog(@"ℹ️ [NATIVE] Process-tap capability probe skipped — macOS < 14.4, SCK fallback applies");
        Nan::Set(result, Nan::New("macOS14_4Plus").ToLocalChecked(), Nan::New<v8::Boolean>(false));
        Nan::Set(result, Nan::New("available").ToLocalChecked(), Nan::New<v8::Boolean>(false));
        Nan::Set(result, Nan::New("error").ToLocalChecked(),
                 Nan::New<v8::String>("macOS version below 14.4").ToLocalChecked());
    }

    info.GetReturnValue().Set(result);
}

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
    
    g_streamingOnly = false;
    
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
    
    g_actualStartMs = 0.0;

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

    const int64_t kSckStartWatchdogNs = 10 * NSEC_PER_SEC;
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, kSckStartWatchdogNs), dispatch_get_main_queue(), ^{
        SckStartStage stage = SckStartPeekStage(startGen);
        std::string msg = std::string("sck_start_timeout stage=") + SckStartStageName(stage) +
            ": ScreenCaptureKit start did not settle within 10s";
        if (SettleSckStartForGeneration(startGen, true, msg)) {
            EmitLifecycleEvent("sck_start_watchdog_fired gen=" + std::to_string(startGen) +
                               " stage=" + SckStartStageName(stage) + " timeout_ms=10000");
        }
    });

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

NAN_METHOD(StopSystemAudioCapture) {
    NSLog(@"🎤 [NATIVE] Stopping system audio capture");
    
    Isolate* isolate = info.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();
    
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
            SckStartStage stage = SckStartPeekStage(pendingGen);
            std::string msg = std::string("sck_start_canceled_by_stop stage=") + SckStartStageName(stage) +
                ": stop requested while start still pending";
            if (SettleSckStartForGeneration(pendingGen, true, msg)) {
                EmitLifecycleEvent("sck_start_canceled_by_stop gen=" + std::to_string(pendingGen) +
                                   " stage=" + SckStartStageName(stage));
            }
        }
    }

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

static void SaysoObserveMicEngineConfigChange(AVAudioEngine* engine) {
    if (!engine) {
        return;
    }
    g_micEngineObserverAttachedAtMs = SaysoNowMs();
    g_micEngineConfigChangeObserver = [[[NSNotificationCenter defaultCenter]
        addObserverForName:AVAudioEngineConfigurationChangeNotification
                    object:engine
                     queue:nil
                usingBlock:^(NSNotification* _Nonnull note) {
                    int64_t sinceAttachMs = SaysoNowMs() - g_micEngineObserverAttachedAtMs;
                    const bool ignored = sinceAttachMs < kMicEngineConfigChangeCooldownMs;
                    g_micLastEngineConfigChangeMs.store(SaysoRealtimeNowMs());
                    g_micEngineConfigChanges.fetch_add(1);
                    g_micEngineRunning.store(false);
                    EmitLifecycleEvent(std::string("mic_engine_config_change ignored=") + (ignored ? "1" : "0") +
                                       " since_attach_ms=" + std::to_string(sinceAttachMs));
                    if (ignored) {
                        NSLog(@"🔊 [NATIVE] AVAudioEngineConfigurationChangeNotification ignored — %lldms "
                              @"after engine attach (cooldown %lldms, likely our own rebuild)",
                              (long long)sinceAttachMs, (long long)kMicEngineConfigChangeCooldownMs);
                        return;
                    }
                    NSLog(@"🔊 [NATIVE] AVAudioEngineConfigurationChangeNotification received");
                    SaysoScheduleMicRouteDebouncedRestart();
                }] retain];
}

static void SaysoRemoveMicEngineConfigChangeObserver() {
    if (g_micEngineConfigChangeObserver) {
        [[NSNotificationCenter defaultCenter] removeObserver:g_micEngineConfigChangeObserver];
        [g_micEngineConfigChangeObserver release];
        g_micEngineConfigChangeObserver = nil;
    }
}

static void MicEngineTeardownOnly() {
    g_micEngineRunning.store(false);
    SaysoRemoveMicEngineConfigChangeObserver();
    if (g_micInputNode) {
        [g_micInputNode removeTapOnBus:0];
        g_micInputNode = nullptr;
    }
    if (g_micEngine) {
        [g_micEngine stop];
        [g_micEngine release];
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


static const float kMicAgcTargetRms = 0.025f;
static const float kMicAgcMaxGain = 63.1f;
static const float kMicAgcGainRisePerBlock = 1.059f;
static const float kMicLimiterKnee = 0.9f;
static const float kMicAgcSpeechOverNoise = 4.0f;
static const float kMicAgcNoiseFloorRise = 1.02f;
static const float kMicAgcMinRms = 1e-5f;
static const float kMicAgcFloorMinRms = 3e-5f;
static const int kMicAgcWarmupBlocks = 5;
static const int kMicAgcSpeechBlocksToLock = 2;
static const int kMicAgcStatusFirstBlocks = 50;
static const int kMicAgcStatusEveryBlocks = 300;

static std::atomic<bool> g_micTapStateResetRequested{true};
static int g_micTapLastChannels = 0;
static double g_micTapLastSampleRate = 0;
static float g_micAgcGain = 1.0f;
static float g_micAgcSpeechLevel = 0;
static float g_micAgcNoiseFloor = 0;
static int g_micAgcBlocks = 0;
static int g_micAgcSpeechRun = 0;
static bool g_micLayoutMismatchReported = false;

static std::string SaysoFormatDb(float linear) {
    char buf[16];
    snprintf(buf, sizeof(buf), "%.1f", linear > 0 ? 20.0 * log10(linear) : -120.0);
    return std::string(buf);
}

static int SaysoMicReadableFrames(AVAudioPCMBuffer* buffer, bool interleaved, int channels, size_t bytesPerSample) {
    const AudioBufferList* abl = buffer.audioBufferList;
    int frames = (int)buffer.frameLength;
    if (!abl || abl->mNumberBuffers == 0) return 0;
    if (interleaved) {
        size_t avail = abl->mBuffers[0].mDataByteSize / (bytesPerSample * (size_t)channels);
        return std::min(frames, (int)avail);
    }
    if ((int)abl->mNumberBuffers < channels) return 0;
    for (int c = 0; c < channels; c++) {
        size_t avail = abl->mBuffers[c].mDataByteSize / bytesPerSample;
        frames = std::min(frames, (int)avail);
    }
    return frames;
}

static void SaysoMicDownmixWithGain(AVAudioPCMBuffer* buffer, bool isFloat32, bool interleaved, int channels,
                                    int frames, std::vector<float>& out) {
    out.resize(frames);
    const float invChannels = 1.0f / (float)channels;
    double sumSq = 0;
    float* const* floatChannels = isFloat32 ? buffer.floatChannelData : nullptr;
    int16_t* const* int16Channels = isFloat32 ? nullptr : buffer.int16ChannelData;
    if ((isFloat32 && !floatChannels) || (!isFloat32 && !int16Channels)) {
        out.clear();
        return;
    }
    for (int i = 0; i < frames; i++) {
        float sum = 0;
        for (int c = 0; c < channels; c++) {
            if (isFloat32) {
                sum += interleaved ? floatChannels[0][i * channels + c] : floatChannels[c][i];
            } else {
                const int16_t v = interleaved ? int16Channels[0][i * channels + c] : int16Channels[c][i];
                sum += (float)v / 32768.0f;
            }
        }
        const float mono = sum * invChannels;
        out[i] = mono;
        sumSq += (double)mono * mono;
    }

    const float blockRms = frames > 0 ? (float)sqrt(sumSq / frames) : 0;
    const float prevGain = g_micAgcGain;

    if (blockRms >= kMicAgcFloorMinRms) {
        if (g_micAgcNoiseFloor <= 0 || blockRms < g_micAgcNoiseFloor) {
            g_micAgcNoiseFloor = blockRms;
        } else {
            g_micAgcNoiseFloor = std::min(g_micAgcNoiseFloor * kMicAgcNoiseFloorRise, blockRms);
        }
    }

    const bool isSpeech = g_micAgcNoiseFloor > 0 && blockRms > kMicAgcMinRms &&
                          blockRms > g_micAgcNoiseFloor * kMicAgcSpeechOverNoise;
    g_micAgcSpeechRun = isSpeech ? g_micAgcSpeechRun + 1 : 0;

    if (isSpeech && g_micAgcBlocks >= kMicAgcWarmupBlocks) {
        if (g_micAgcSpeechLevel <= 0) {
            if (g_micAgcSpeechRun >= kMicAgcSpeechBlocksToLock) {
                g_micAgcSpeechLevel = blockRms;
                g_micAgcGain = std::min(kMicAgcMaxGain, std::max(1.0f, kMicAgcTargetRms / blockRms));
            }
        } else {
            const float coeff = blockRms > g_micAgcSpeechLevel ? 0.5f : 0.05f;
            g_micAgcSpeechLevel += coeff * (blockRms - g_micAgcSpeechLevel);
            const float desired = std::min(kMicAgcMaxGain, std::max(1.0f, kMicAgcTargetRms / g_micAgcSpeechLevel));
            g_micAgcGain = desired < g_micAgcGain ? desired : std::min(desired, g_micAgcGain * kMicAgcGainRisePerBlock);
        }
    }

    for (int i = 0; i < frames; i++) {
        const float g = prevGain + (g_micAgcGain - prevGain) * ((float)i / (float)frames);
        float y = out[i] * g;
        const float a = fabs(y);
        if (a > kMicLimiterKnee) {
            const float headroom = 1.0f - kMicLimiterKnee;
            y = copysign(kMicLimiterKnee + headroom * (float)tanh((a - kMicLimiterKnee) / headroom), y);
        }
        out[i] = y;
    }
}

static bool TryStartMicrophoneCaptureOnce(int waitForFirstTapMs, const char** failReason = nullptr) {
    g_micFirstTapSeen.store(false, std::memory_order_release);
    g_micTapStateResetRequested.store(true);
    g_micEngineBuilds.fetch_add(1);
    g_micEngineBuiltAtMs.store(SaysoRealtimeNowMs());
    g_micTapCallbacks.store(0);
    g_micTapDelivered.store(0);
    g_micTapDroppedNoHandle.store(0);
    g_micTapDroppedNoCallback.store(0);
    g_micTapDroppedEmpty.store(0);
    g_micTapDroppedLayout.store(0);
    g_micTapDroppedUnsupported.store(0);

    g_micEngine = [[AVAudioEngine alloc] init];
    g_micInputNode = [g_micEngine inputNode];

    if (!g_micInputNode) {
        NSLog(@"❌ [NATIVE] Failed to get input node from audio engine");
        [g_micEngine release];
        g_micEngine = nullptr;
        if (failReason) *failReason = "no_input_node";
        return false;
    }

    SaysoObserveMicEngineConfigChange(g_micEngine);

    @try {
        [g_micInputNode installTapOnBus:0 bufferSize:4096 format:nil block:^(AVAudioPCMBuffer* buffer, AVAudioTime* when) {
        if (!buffer) {
            return;
        }
        g_micTapCallbacks.fetch_add(1, std::memory_order_relaxed);
        g_micTapLastCallbackMs.store(SaysoRealtimeNowMs(), std::memory_order_relaxed);
        
        if (!g_micStreamingAsyncHandle) {
            g_micTapDroppedNoHandle.fetch_add(1, std::memory_order_relaxed);
            NSLog(@"⚠️ [NATIVE] Mic callback skipped - microphone streaming async handle is null");
            return;
        }
        
        if (g_micStreamingCallback.IsEmpty()) {
            g_micTapDroppedNoCallback.fetch_add(1, std::memory_order_relaxed);
            NSLog(@"⚠️ [NATIVE] Mic callback skipped - microphone streaming callback is empty");
            return;
        }
        
        AVAudioFormat* bufferFormat = buffer.format;
        int channels = (int)bufferFormat.channelCount;
        int frameLength = (int)buffer.frameLength;
        g_micDiagTapSampleRate.store((int)bufferFormat.sampleRate, std::memory_order_relaxed);
        g_micDiagTapChannels.store(channels, std::memory_order_relaxed);
        g_micDiagTapCommonFormat.store((unsigned long)bufferFormat.commonFormat, std::memory_order_relaxed);
        if (frameLength <= 0 || channels <= 0) {
            g_micTapDroppedEmpty.fetch_add(1, std::memory_order_relaxed);
            return;
        }

        const bool isFloat32 = bufferFormat.commonFormat == AVAudioPCMFormatFloat32;
        const bool isInt16 = bufferFormat.commonFormat == AVAudioPCMFormatInt16;
        const bool interleaved = bufferFormat.isInterleaved;

        if (g_micTapStateResetRequested.exchange(false) ||
            channels != g_micTapLastChannels || bufferFormat.sampleRate != g_micTapLastSampleRate) {
            g_micTapLastChannels = channels;
            g_micTapLastSampleRate = bufferFormat.sampleRate;
            g_micAgcGain = 1.0f;
            g_micAgcSpeechLevel = 0;
            g_micAgcNoiseFloor = 0;
            g_micAgcBlocks = 0;
            g_micAgcSpeechRun = 0;
            g_micLayoutMismatchReported = false;
            const bool downmix = channels > 1 && (isFloat32 || isInt16);
            EmitLifecycleEvent("mic_input_format sample_rate=" + std::to_string((int)bufferFormat.sampleRate) +
                               " channels=" + std::to_string(channels) +
                               " interleaved=" + (interleaved ? "1" : "0") +
                               " common_format=" + std::to_string((unsigned long)bufferFormat.commonFormat) +
                               " downmix=" + (downmix ? "1" : "0") + " agc=" + (downmix ? "1" : "0"));
        }

        StreamingData* streamData = new StreamingData();
        size_t dataSize = 0;

        if (channels > 1 && (isFloat32 || isInt16)) {
            const int frames = SaysoMicReadableFrames(buffer, interleaved, channels, isFloat32 ? sizeof(float) : sizeof(int16_t));
            if (frames <= 0) {
                g_micTapDroppedLayout.fetch_add(1, std::memory_order_relaxed);
                if (!g_micLayoutMismatchReported) {
                    g_micLayoutMismatchReported = true;
                    const AudioBufferList* abl = buffer.audioBufferList;
                    EmitLifecycleEvent("mic_downmix_layout_mismatch_failed channels=" + std::to_string(channels) +
                                       " buffers=" + std::to_string(abl ? (int)abl->mNumberBuffers : -1) +
                                       " interleaved=" + (interleaved ? "1" : "0"));
                }
                delete streamData;
                return;
            }
            std::vector<float> mono;
            SaysoMicDownmixWithGain(buffer, isFloat32, interleaved, channels, frames, mono);
            dataSize = (size_t)frames * sizeof(float);
            streamData->audioData.resize(dataSize);
            memcpy(streamData->audioData.data(), mono.data(), dataSize);
            streamData->bitDepth = 32;
            streamData->isFloat = true;
            streamData->gainDb = g_micAgcGain > 0 ? (float)(20.0 * log10(g_micAgcGain)) : 0.0f;
            channels = 1;

            g_micAgcBlocks++;
            if (g_micAgcBlocks == kMicAgcStatusFirstBlocks ||
                (g_micAgcBlocks > kMicAgcStatusFirstBlocks &&
                 (g_micAgcBlocks - kMicAgcStatusFirstBlocks) % kMicAgcStatusEveryBlocks == 0)) {
                EmitLifecycleEvent("mic_agc_status gain_db=" + SaysoFormatDb(g_micAgcGain) +
                                   " speech_db=" + (g_micAgcSpeechLevel > 0 ? SaysoFormatDb(g_micAgcSpeechLevel) : std::string("none")) +
                                   " noise_db=" + SaysoFormatDb(g_micAgcNoiseFloor) +
                                   " source_channels=" + std::to_string(g_micTapLastChannels));
            }
        } else if (isFloat32) {
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
            g_micTapDroppedUnsupported.fetch_add(1, std::memory_order_relaxed);
            NSLog(@"⚠️ [NATIVE] Unsupported audio format: %lu", (unsigned long)bufferFormat.commonFormat);
            delete streamData;
            return;
        }
        
        streamData->sampleRate = bufferFormat.sampleRate;
        streamData->channels = channels;
        
        g_micFirstTapSeen.store(true, std::memory_order_release);
        g_micTapDelivered.fetch_add(1, std::memory_order_relaxed);
        g_micTapLastDeliveredMs.store(SaysoRealtimeNowMs(), std::memory_order_relaxed);
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
    g_micEngineRunning.store(true);
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

static const int64_t kMicRouteRecoveryBackoffMs[] = {250, 500, 1000, 2000, 4000};
static const size_t kMicRouteRecoveryBackoffSteps =
    sizeof(kMicRouteRecoveryBackoffMs) / sizeof(kMicRouteRecoveryBackoffMs[0]);
static const int64_t kMicRouteRecoveryCeilingMs = 30000;

static void SaysoAttemptMicRouteRecovery(AudioDeviceID targetDevice, size_t stepIndex, uint64_t wave,
                                          bool leavingBluetooth);

static bool SaysoMicRouteStepStillValid(uint64_t wave) {
    return g_isMicCapturing.load() && wave == g_micRouteRestartGeneration.load(std::memory_order_acquire);
}

static void SaysoScheduleMicRouteRecoveryRetry(AudioDeviceID targetDevice, size_t stepIndex, uint64_t wave,
                                                bool leavingBluetooth) {
    int64_t elapsedMs = SaysoNowMs() - g_micRouteRecoveryStartMs;
    if (elapsedMs >= kMicRouteRecoveryCeilingMs) {
        g_micRouteRecovering = false;
        NSLog(@"❌ [NATIVE] Mic route recovery FAILED after %lldms (ceiling reached) — mic capture remains "
              @"logically active but not producing audio; a future device change will retry",
              (long long)elapsedMs);
        EmitLifecycleEvent("mic_route_recovery_failed elapsedMs=" + std::to_string(elapsedMs));
        return;
    }

    int64_t delayMs = kMicRouteRecoveryBackoffMs[std::min(stepIndex, kMicRouteRecoveryBackoffSteps - 1)];
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, delayMs * NSEC_PER_MSEC), SaysoMicRouteRestartQueue(), ^{
        if (!SaysoMicRouteStepStillValid(wave)) {
            return;
        }
        SaysoAttemptMicRouteRecovery(targetDevice, stepIndex, wave, leavingBluetooth);
    });
}

static void SaysoAttemptMicRouteRecovery(AudioDeviceID targetDevice, size_t stepIndex, uint64_t wave,
                                          bool leavingBluetooth) {
    NSLog(@"🔁 [NATIVE] Mic route recovery: attempt %zu for default input id=%u (elapsed=%lldms)%s",
          stepIndex + 1, (unsigned)targetDevice, (long long)(SaysoNowMs() - g_micRouteRecoveryStartMs),
          leavingBluetooth ? " [leaving Bluetooth — widened]" : "");

    MicEngineTeardownOnly();
    bool ok = TryStartMicrophoneCaptureOnce(leavingBluetooth ? 2500 : 1200);

    if (ok) {
        g_micOpenedInputDeviceId = targetDevice;
        g_micRouteRecovering = false;
        int64_t elapsedMs = SaysoNowMs() - g_micRouteRecoveryStartMs;
        NSLog(@"✅ [NATIVE] Mic route recovery succeeded on attempt %zu (%lldms); now following default input "
              @"id=%u", stepIndex + 1, (long long)elapsedMs, (unsigned)targetDevice);
        EmitLifecycleEvent("mic_route_recovery_succeeded elapsedMs=" + std::to_string(elapsedMs) +
                            " attempts=" + std::to_string(stepIndex + 1));
        return;
    }

    SaysoScheduleMicRouteRecoveryRetry(targetDevice, stepIndex + 1, wave, leavingBluetooth);
}

static void SaysoPerformMicRestartIfCapturing(BOOL forcedAfterDefaultInputNotification, bool leavingBluetoothHint) {
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

    const bool leavingBluetooth = leavingBluetoothHint;

    NSLog(@"🔊 [NATIVE] Mic route: restarting engine for default input id=%u (previous opened id=%u)%s",
          (unsigned)currentDefault, (unsigned)g_micOpenedInputDeviceId,
          leavingBluetooth ? " [leaving Bluetooth — widened first attempt]" : "");

    MicEngineTeardownOnly();

    const int kFirstTapWaitMs = leavingBluetooth ? 2500 : 1200;
    const int kRetryTapWaitMs = 1500;
    bool ok = TryStartMicrophoneCaptureOnce(kFirstTapWaitMs);
    if (!ok) {
        NSLog(@"🎤 [NATIVE] Mic route: repeating capture once after default change");
        ok = TryStartMicrophoneCaptureOnce(kRetryTapWaitMs);
    }

    if (ok) {
        g_micOpenedInputDeviceId = currentDefault;
        g_micRouteRecovering = false;
        NSLog(@"✅ [NATIVE] Mic route restart succeeded; now following default input id=%u",
              (unsigned)currentDefault);
    } else {
        NSLog(@"⚠️ [NATIVE] Mic route: immediate attempts failed for default input id=%u — entering backoff "
              @"recovery", (unsigned)currentDefault);
        if (!g_micRouteRecovering.exchange(true)) {
            g_micRouteRecoveryStartMs = SaysoNowMs();
            EmitLifecycleEvent("mic_route_recovery_started");
        }
        SaysoScheduleMicRouteRecoveryRetry(currentDefault, 0,
                                            g_micRouteRestartGeneration.load(std::memory_order_acquire),
                                            leavingBluetooth);
    }
}

static void SaysoScheduleMicRouteDebouncedRestart() {
    dispatch_async(SaysoMicRouteRestartQueue(), ^{
        if (!g_isMicCapturing) {
            return;
        }

        AudioDeviceID halNow = SaysoGetCurrentDefaultInputDeviceID();
        const bool changed =
            (halNow != kAudioObjectUnknown && g_micOpenedInputDeviceId != kAudioObjectUnknown &&
             halNow != g_micOpenedInputDeviceId);
        const bool leavingBluetooth = changed && SaysoIsBluetoothInputDevice(g_micOpenedInputDeviceId);
        const bool fastPath = changed && !leavingBluetooth;

        const int64_t debounceNs =
            fastPath ? (80 * NSEC_PER_MSEC) : leavingBluetooth ? (450 * NSEC_PER_MSEC) : (600 * NSEC_PER_MSEC);
        const int64_t stabilityDelayNs =
            fastPath ? (60 * NSEC_PER_MSEC) : (150 * NSEC_PER_MSEC);

        if (fastPath) {
            NSLog(@"🔊 [NATIVE] Mic route: fast-path schedule (HAL default %u ≠ opened %u) debounce=%lldms "
                  @"stability=%lldms",
                  (unsigned)halNow, (unsigned)g_micOpenedInputDeviceId, (long long)(debounceNs / NSEC_PER_MSEC),
                  (long long)(stabilityDelayNs / NSEC_PER_MSEC));
        } else if (leavingBluetooth) {
            NSLog(@"🔊 [NATIVE] Mic route: leaving-Bluetooth schedule (HAL default %u ≠ opened %u, opened was "
                  @"Bluetooth) debounce=%lldms stability=%lldms",
                  (unsigned)halNow, (unsigned)g_micOpenedInputDeviceId, (long long)(debounceNs / NSEC_PER_MSEC),
                  (long long)(stabilityDelayNs / NSEC_PER_MSEC));
        }

        const uint64_t wave = g_micRouteRestartGeneration.fetch_add(1, std::memory_order_acq_rel) + 1;

        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, debounceNs), SaysoMicRouteRestartQueue(), ^{
            if (!SaysoMicRouteStepStillValid(wave)) {
                return;
            }

            AudioDeviceID firstDefault = SaysoGetCurrentDefaultInputDeviceID();
            if (firstDefault == kAudioObjectUnknown) {
                NSLog(@"🔊 [NATIVE] Mic route debounce fired: could not read default input — skipping restart");
                return;
            }

            dispatch_after(dispatch_time(DISPATCH_TIME_NOW, stabilityDelayNs), SaysoMicRouteRestartQueue(), ^{
                if (!SaysoMicRouteStepStillValid(wave)) {
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
                SaysoPerformMicRestartIfCapturing(YES, leavingBluetooth);
            });
        });
    });
}

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

    __block bool alreadyActive = false;
    __block bool started = false;
    __block const char* failReason = nullptr;

    dispatch_sync(SaysoMicRouteRestartQueue(), ^{
        g_micRouteRecovering = false;

        if (g_isMicCapturing) {
            alreadyActive = true;
            return;
        }

        const int kFirstTapWaitMs = 1200;
        const int kRetryTapWaitMs = 1500;

        if (!TryStartMicrophoneCaptureOnce(kFirstTapWaitMs, &failReason)) {
            NSLog(@"🎤 [NATIVE] Mic: repeating capture once (cold start / Bluetooth input path) — first attempt failed: %s",
                  failReason ? failReason : "unknown");
            if (!TryStartMicrophoneCaptureOnce(kRetryTapWaitMs, &failReason)) {
                NSLog(@"❌ [NATIVE] Microphone capture failed after retry: %s", failReason ? failReason : "unknown");
                g_micOpenedInputDeviceId = kAudioObjectUnknown;
                return;
            }
        }

        g_isMicCapturing = true;
        g_micOpenedInputDeviceId = SaysoGetCurrentDefaultInputDeviceID();
        NSLog(@"✅ [NATIVE] Microphone capture started successfully (first tap received); default input id=%u",
              (unsigned)g_micOpenedInputDeviceId);
        started = true;
    });

    if (alreadyActive) {
        NSLog(@"⚠️ [NATIVE] Microphone capture already active");
        info.GetReturnValue().Set(MicStartResult(false, "already_active"));
        return;
    }

    if (!started) {
        info.GetReturnValue().Set(MicStartResult(false, failReason ? failReason : "no_tap_buffers"));
        return;
    }

    info.GetReturnValue().Set(MicStartResult(true));
}

NAN_METHOD(StopMicrophoneCapture) {
    NSLog(@"🎤 [NATIVE] Stopping microphone capture");

    g_micRouteRestartGeneration.fetch_add(1, std::memory_order_acq_rel);

    dispatch_sync(SaysoMicRouteRestartQueue(), ^{
        MicEngineTeardownOnly();
        g_isMicCapturing = false;
        g_micOpenedInputDeviceId = kAudioObjectUnknown;
        g_micRouteRecovering = false;
    });

    NSLog(@"✅ [NATIVE] Microphone capture stopped");
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(true));
}

NAN_METHOD(IsMicrophoneCaptureActive) {
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(g_isMicCapturing.load()));
}

NAN_METHOD(IsMicRouteRecovering) {
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(g_micRouteRecovering.load()));
}

struct PendingMicDiagnostics {
    uv_async_t async;
    Nan::Persistent<v8::Promise::Resolver> resolver;
    int64_t takenAtMs = 0;
    SaysoMicDeviceSnapshot defaultInput;
    bool hasOpened = false;
    SaysoMicDeviceSnapshot openedInput;
    AudioDeviceID openedId = kAudioObjectUnknown;
    bool capturing = false;
    bool routeRecovering = false;
    bool engineRunning = false;
    uint64_t engineBuilds = 0;
    int64_t engineBuiltAtMs = 0;
    int64_t lastDefaultInputChangeMs = 0;
    int64_t lastEngineConfigChangeMs = 0;
    uint64_t engineConfigChanges = 0;
    uint64_t tapCallbacks = 0;
    uint64_t tapDelivered = 0;
    uint64_t tapDroppedNoHandle = 0;
    uint64_t tapDroppedNoCallback = 0;
    uint64_t tapDroppedEmpty = 0;
    uint64_t tapDroppedLayout = 0;
    uint64_t tapDroppedUnsupported = 0;
    int64_t tapLastCallbackMs = 0;
    int64_t tapLastDeliveredMs = 0;
    int tapSampleRate = 0;
    int tapChannels = 0;
    unsigned long tapCommonFormat = 0;
};

static std::atomic<bool> g_micDiagnosticsInFlight{false};

static Local<Value> SaysoMsSinceOrNull(int64_t nowMs, int64_t thenMs) {
    if (thenMs <= 0) {
        return Nan::Null();
    }
    return Nan::New<Number>((double)std::max<int64_t>(0, nowMs - thenMs));
}

static Local<Object> SaysoMicDeviceSnapshotToJs(const SaysoMicDeviceSnapshot& d) {
    Local<Object> o = Nan::New<Object>();
    auto str = [](bool has, const std::string& v) -> Local<Value> {
        return has ? Local<Value>(Nan::New<String>(v.c_str()).ToLocalChecked()) : Local<Value>(Nan::Null());
    };
    auto tri = [](int v) -> Local<Value> {
        return v < 0 ? Local<Value>(Nan::Null()) : Local<Value>(Nan::New<v8::Boolean>(v == 1));
    };
    Nan::Set(o, Nan::New("id").ToLocalChecked(), Nan::New<Number>((double)d.id));
    Nan::Set(o, Nan::New("name").ToLocalChecked(), str(d.hasName, d.name));
    Nan::Set(o, Nan::New("uid").ToLocalChecked(), str(d.hasUid, d.uid));
    Nan::Set(o, Nan::New("transport").ToLocalChecked(),
             Nan::New<String>(SaysoTransportTypeName(d.transport)).ToLocalChecked());
    Nan::Set(o, Nan::New("nominalSampleRate").ToLocalChecked(),
             d.nominalSampleRate > 0 ? Local<Value>(Nan::New<Number>(d.nominalSampleRate)) : Local<Value>(Nan::Null()));
    Nan::Set(o, Nan::New("inputChannels").ToLocalChecked(),
             d.inputChannels >= 0 ? Local<Value>(Nan::New<Number>(d.inputChannels)) : Local<Value>(Nan::Null()));
    Nan::Set(o, Nan::New("isAlive").ToLocalChecked(), tri(d.isAlive));
    Nan::Set(o, Nan::New("isRunningSomewhere").ToLocalChecked(), tri(d.isRunningSomewhere));
    Nan::Set(o, Nan::New("hogModePid").ToLocalChecked(),
             d.hasHogPid ? Local<Value>(Nan::New<Number>((double)d.hogPid)) : Local<Value>(Nan::Null()));
    Nan::Set(o, Nan::New("inputMuted").ToLocalChecked(), tri(d.muted));
    Nan::Set(o, Nan::New("inputVolume").ToLocalChecked(),
             d.volume >= 0 ? Local<Value>(Nan::New<Number>(d.volume)) : Local<Value>(Nan::Null()));
    return o;
}

static void MicDiagnosticsSettledCb(uv_async_t* handle) {
    PendingMicDiagnostics* p = static_cast<PendingMicDiagnostics*>(handle->data);
    if (!p) {
        return;
    }
    Nan::HandleScope scope;
    Isolate* isolate = Isolate::GetCurrent();
    Local<Context> context = isolate->GetCurrentContext();
    Local<Promise::Resolver> res = Nan::New(p->resolver);
    p->resolver.Reset();

    const int64_t now = p->takenAtMs;
    Local<Object> result = Nan::New<Object>();
    Nan::Set(result, Nan::New("defaultInput").ToLocalChecked(),
             p->defaultInput.id != kAudioObjectUnknown ? Local<Value>(SaysoMicDeviceSnapshotToJs(p->defaultInput))
                                                       : Local<Value>(Nan::Null()));
    Nan::Set(result, Nan::New("openedInputId").ToLocalChecked(),
             p->openedId != kAudioObjectUnknown ? Local<Value>(Nan::New<Number>((double)p->openedId))
                                                : Local<Value>(Nan::Null()));
    Nan::Set(result, Nan::New("openedInput").ToLocalChecked(),
             p->hasOpened ? Local<Value>(SaysoMicDeviceSnapshotToJs(p->openedInput)) : Local<Value>(Nan::Null()));
    Nan::Set(result, Nan::New("capturing").ToLocalChecked(), Nan::New<v8::Boolean>(p->capturing));
    Nan::Set(result, Nan::New("routeRecovering").ToLocalChecked(), Nan::New<v8::Boolean>(p->routeRecovering));
    Nan::Set(result, Nan::New("engineRunning").ToLocalChecked(), Nan::New<v8::Boolean>(p->engineRunning));
    Nan::Set(result, Nan::New("engineBuilds").ToLocalChecked(), Nan::New<Number>((double)p->engineBuilds));
    Nan::Set(result, Nan::New("msSinceEngineBuilt").ToLocalChecked(), SaysoMsSinceOrNull(now, p->engineBuiltAtMs));
    Nan::Set(result, Nan::New("msSinceDefaultInputChange").ToLocalChecked(),
             SaysoMsSinceOrNull(now, p->lastDefaultInputChangeMs));
    Nan::Set(result, Nan::New("msSinceEngineConfigChange").ToLocalChecked(),
             SaysoMsSinceOrNull(now, p->lastEngineConfigChangeMs));
    Nan::Set(result, Nan::New("engineConfigChanges").ToLocalChecked(), Nan::New<Number>((double)p->engineConfigChanges));

    Local<Object> tap = Nan::New<Object>();
    Nan::Set(tap, Nan::New("callbacks").ToLocalChecked(), Nan::New<Number>((double)p->tapCallbacks));
    Nan::Set(tap, Nan::New("delivered").ToLocalChecked(), Nan::New<Number>((double)p->tapDelivered));
    Nan::Set(tap, Nan::New("droppedNoHandle").ToLocalChecked(), Nan::New<Number>((double)p->tapDroppedNoHandle));
    Nan::Set(tap, Nan::New("droppedNoCallback").ToLocalChecked(), Nan::New<Number>((double)p->tapDroppedNoCallback));
    Nan::Set(tap, Nan::New("droppedEmpty").ToLocalChecked(), Nan::New<Number>((double)p->tapDroppedEmpty));
    Nan::Set(tap, Nan::New("droppedLayout").ToLocalChecked(), Nan::New<Number>((double)p->tapDroppedLayout));
    Nan::Set(tap, Nan::New("droppedUnsupported").ToLocalChecked(), Nan::New<Number>((double)p->tapDroppedUnsupported));
    Nan::Set(tap, Nan::New("msSinceLastCallback").ToLocalChecked(), SaysoMsSinceOrNull(now, p->tapLastCallbackMs));
    Nan::Set(tap, Nan::New("msSinceLastDelivered").ToLocalChecked(), SaysoMsSinceOrNull(now, p->tapLastDeliveredMs));
    Nan::Set(tap, Nan::New("lastSampleRate").ToLocalChecked(),
             p->tapSampleRate > 0 ? Local<Value>(Nan::New<Number>(p->tapSampleRate)) : Local<Value>(Nan::Null()));
    Nan::Set(tap, Nan::New("lastChannels").ToLocalChecked(),
             p->tapChannels > 0 ? Local<Value>(Nan::New<Number>(p->tapChannels)) : Local<Value>(Nan::Null()));
    Nan::Set(tap, Nan::New("lastCommonFormat").ToLocalChecked(),
             p->tapCommonFormat > 0 ? Local<Value>(Nan::New<Number>((double)p->tapCommonFormat)) : Local<Value>(Nan::Null()));
    Nan::Set(result, Nan::New("tap").ToLocalChecked(), tap);

    res->Resolve(context, result).Check();
    g_micDiagnosticsInFlight.store(false);

    uv_close(reinterpret_cast<uv_handle_t*>(handle), [](uv_handle_t* h) {
        PendingMicDiagnostics* self = static_cast<PendingMicDiagnostics*>(h->data);
        delete self;
    });
}

NAN_METHOD(GetMicInputDiagnostics) {
    Isolate* isolate = info.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();
    MaybeLocal<Promise::Resolver> maybeResolver = Promise::Resolver::New(context);
    if (maybeResolver.IsEmpty()) {
        return;
    }
    Local<Promise::Resolver> resolver = maybeResolver.ToLocalChecked();

    if (g_micDiagnosticsInFlight.exchange(true)) {
        resolver->Resolve(context, Nan::Null()).Check();
        info.GetReturnValue().Set(resolver->GetPromise());
        return;
    }

    PendingMicDiagnostics* p = new PendingMicDiagnostics();
    p->resolver.Reset(resolver);
    p->async.data = p;
    uv_async_init(uv_default_loop(), &p->async, MicDiagnosticsSettledCb);

    dispatch_async(dispatch_get_global_queue(QOS_CLASS_UTILITY, 0), ^{
        p->takenAtMs = SaysoRealtimeNowMs();
        p->openedId = g_micOpenedInputDeviceId.load();
        p->capturing = g_isMicCapturing.load();
        p->routeRecovering = g_micRouteRecovering.load();
        p->engineRunning = g_micEngineRunning.load();
        p->engineBuilds = g_micEngineBuilds.load();
        p->engineBuiltAtMs = g_micEngineBuiltAtMs.load();
        p->lastDefaultInputChangeMs = g_micLastDefaultInputChangeMs.load();
        p->lastEngineConfigChangeMs = g_micLastEngineConfigChangeMs.load();
        p->engineConfigChanges = g_micEngineConfigChanges.load();
        p->tapCallbacks = g_micTapCallbacks.load();
        p->tapDelivered = g_micTapDelivered.load();
        p->tapDroppedNoHandle = g_micTapDroppedNoHandle.load();
        p->tapDroppedNoCallback = g_micTapDroppedNoCallback.load();
        p->tapDroppedEmpty = g_micTapDroppedEmpty.load();
        p->tapDroppedLayout = g_micTapDroppedLayout.load();
        p->tapDroppedUnsupported = g_micTapDroppedUnsupported.load();
        p->tapLastCallbackMs = g_micTapLastCallbackMs.load();
        p->tapLastDeliveredMs = g_micTapLastDeliveredMs.load();
        p->tapSampleRate = g_micDiagTapSampleRate.load();
        p->tapChannels = g_micDiagTapChannels.load();
        p->tapCommonFormat = g_micDiagTapCommonFormat.load();

        const AudioDeviceID defaultId = SaysoGetCurrentDefaultInputDeviceID();
        p->defaultInput = SaysoSnapshotMicDevice(defaultId);
        if (p->openedId != kAudioObjectUnknown && p->openedId != defaultId) {
            p->hasOpened = true;
            p->openedInput = SaysoSnapshotMicDevice(p->openedId);
        }
        uv_async_send(&p->async);
    });

    info.GetReturnValue().Set(resolver->GetPromise());
}

NAN_METHOD(SetStreamingCallback) {
    if (info.Length() < 1 || info[0]->IsNull() || info[0]->IsUndefined()) {
        g_streamingCallback.Reset();
        
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
    
    g_streamingCallback.Reset(Nan::To<Function>(info[0]).ToLocalChecked());
    
    if (!g_streamingAsyncHandle) {
        g_streamingAsyncHandle = new uv_async_t();
        uv_async_init(uv_default_loop(), g_streamingAsyncHandle, StreamingAsyncCallback);
    }
    
    NSLog(@"🎤 [NATIVE] Streaming callback set");
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(true));
}

NAN_METHOD(SetMicrophoneStreamingCallback) {
    if (info.Length() < 1 || info[0]->IsNull() || info[0]->IsUndefined()) {
        g_micStreamingCallback.Reset();
        
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
    
    g_micStreamingCallback.Reset(Nan::To<Function>(info[0]).ToLocalChecked());
    
    if (!g_micStreamingAsyncHandle) {
        g_micStreamingAsyncHandle = new uv_async_t();
        uv_async_init(uv_default_loop(), g_micStreamingAsyncHandle, MicStreamingAsyncCallback);
    }
    
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(true));
}

NAN_METHOD(IsSystemAudioCaptureActive) {
    bool pendingStart = false;
    {
        std::lock_guard<std::mutex> lk(g_sckStartMutex);
        pendingStart = (g_sckStartPending != nullptr && !g_sckStartPending->settled);
    }
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(g_isCapturing || pendingStart));
}

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
        
        propertyAddress.mScope = kAudioDevicePropertyScopeOutput;
        propertyAddress.mSelector = kAudioDevicePropertyStreams;
        
        dataSize = 0;
        status = AudioObjectGetPropertyDataSize(deviceID, 
                                              &propertyAddress, 
                                              0, 
                                              NULL, 
                                              &dataSize);
        
        if (status == noErr && dataSize > 0) {
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

NAN_METHOD(CreateMultiOutputDevice) {
    if (info.Length() < 2) {
        Nan::ThrowError("Expected 2 arguments: name and subDevices");
        return;
    }
    
    String::Utf8Value name(Nan::GetCurrentContext()->GetIsolate(), info[0]);
    Local<Array> subDevices = Local<Array>::Cast(info[1]);
    
    NSLog(@"🎤 [NATIVE] Creating multi-output device: %s", *name);
    
    CFMutableDictionaryRef description = CFDictionaryCreateMutable(NULL, 0, 
                                                                &kCFTypeDictionaryKeyCallBacks, 
                                                                &kCFTypeDictionaryValueCallBacks);
    
    CFStringRef deviceName = CFStringCreateWithCString(NULL, *name, kCFStringEncodingUTF8);
    CFDictionarySetValue(description, CFSTR(kAudioAggregateDeviceNameKey), deviceName);
    
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
    
    AudioDeviceID aggregateDeviceID;
    OSStatus status = AudioHardwareCreateAggregateDevice(description, &aggregateDeviceID);
    
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

NAN_MODULE_INIT(Init) {
    Nan::Set(target, Nan::New("initialize").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(Initialize)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("checkScreenRecordingGranted").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(CheckScreenRecordingGranted)).ToLocalChecked());

    Nan::Set(target, Nan::New("requestScreenRecordingPermission").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(RequestScreenRecordingPermission)).ToLocalChecked());

    Nan::Set(target, Nan::New("checkAudioCaptureCapability").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(CheckAudioCaptureCapability)).ToLocalChecked());

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

    Nan::Set(target, Nan::New("isMicRouteRecovering").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(IsMicRouteRecovering)).ToLocalChecked());

    Nan::Set(target, Nan::New("getMicInputDiagnostics").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(GetMicInputDiagnostics)).ToLocalChecked());

    Nan::Set(target, Nan::New("setLifecycleEventCallback").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(SetLifecycleEventCallback)).ToLocalChecked());
}

NODE_MODULE(native_audio, Init)