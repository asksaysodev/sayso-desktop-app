//
//  ZMPSAudioMgr.h
//  ZoomClips
//
//  Created by Michael Lin on 2022/12/30.
//

#import <Foundation/Foundation.h>
#import <ZoomSetting/ZMPSObject.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMPSAudioDevice;

typedef NS_ENUM(NSUInteger, ZMBGNoiseSuppress) {
    ZMBGNoiseSuppressAuto,
    ZMBGNoiseSuppressLow,
    ZMBGNoiseSuppressMedium,
    ZMBGNoiseSuppressHigh,
};

@interface ZMPSAudioMgr : ZMPSObject

@property (nonatomic, weak) ZMPSAudioDevice *microphoneDevice;
- (NSArray<ZMPSAudioDevice *> *)microphoneDevices;
- (BOOL)isDefaultMicDeviceNotExisted;
- (void)micDeviceAddedWithDeviceID:(nullable NSString *)deviceID;
- (void)micDeviceRemovedWithDeviceID:(nullable NSString *)deviceID;
- (void)unregisterSink;
- (void)registerSink;
- (BOOL)isConfigReady;
@property (nonatomic, assign) BOOL enableMicKeepOriInput;
@property (nonatomic, assign) BOOL originalSoundChangable;
@property (nonatomic, assign) BOOL enableAGCMode;
@property (nonatomic, assign) BOOL enableHighFideMusicMode;
@property (nonatomic, assign) BOOL enableEchoCancellation;
@property (nonatomic, assign) BOOL enableStereo;
@property (nonatomic, assign, readonly) BOOL isAudioDeviceInstalled;
@property (nonatomic, assign) ZMBGNoiseSuppress bgNoiseSuppress;

- (BOOL)isStereoMicRecordingEnabled;
- (void)setStereoMicRecording:(BOOL)enable;
- (void)startMicForIdle;
- (void)notifyShareAudioDriverInstalled;
- (void)startComputerAudioShare;
- (void)stopComputerAudioShare;
- (void)stopMicForIdle;

@end

NS_ASSUME_NONNULL_END
