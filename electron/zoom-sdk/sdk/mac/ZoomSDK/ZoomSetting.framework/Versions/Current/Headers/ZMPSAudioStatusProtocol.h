//
//  ZMPSAudioStatusProtocol.h
//  ZoomSetting
//
//  Created by Michael Lin on 2024/1/15.
//

#ifndef ZMPSAudioStatusProtocol_h
#define ZMPSAudioStatusProtocol_h

typedef NS_ENUM(NSUInteger, ZMPSMCDeviceSpecialInfoType) {
    ZMPSMCDeviceSpecialInfoTypeNoInput,
    ZMPSMCDeviceSpecialInfoTypeMusicDetected,
    ZMPSMCDeviceSpecialInfoTypeMuteReminder,
};

@class ZMPSDevice;
@protocol ZMPSAudioStatusProtocol <NSObject>

@optional
- (void)psAudioDeviceSpecialInfoChange:(ZMPSMCDeviceSpecialInfoType)type;
- (void)psClipsPoorQualityAlert;
- (void)psMicAudioLevelChanged:(NSUInteger)level;
- (void)psMicDeviceChanged;
- (void)psMicDeviceRemovedForCurrentDevice:(BOOL)isCurrentDevice;
- (void)psMicDeviceAdded:(ZMPSDevice *)device;
- (void)psMicMutedChanged;
- (void)psStereoMicRecordingChanged;
- (void)psOriginalSoundChanged;
- (void)psMicKeepOriInputChanged;

@end

#endif /* ZMPSAudioStatusProtocol_h */
