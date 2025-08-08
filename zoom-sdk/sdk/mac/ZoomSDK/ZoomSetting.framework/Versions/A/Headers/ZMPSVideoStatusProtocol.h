//
//  ZMPSVideoStatusProtocol.h
//  ZoomSetting
//
//  Created by Michael Lin on 2024/1/15.
//

#ifndef ZMPSVideoStatusProtocol_h
#define ZMPSVideoStatusProtocol_h

#import <ZoomSetting/ZMPSDevice.h>
@class ZMPSBioInfoVerifyStatus;
@protocol ZMPSVideoStatusProtocol <NSObject>

@optional
- (void)psOnRecordLayoutChanged;
- (void)psVideoDeviceChanged;
- (void)psVideoDeviceRemovedForCurrentDevice:(BOOL)isCurrentDevice;
- (void)psVideoDeviceAdded:(nonnull ZMPSDevice *)device;

- (void)psBioInfoVerifyStatusChanged:(nonnull ZMPSBioInfoVerifyStatus *)result;
- (void)psBioInfoVerifyInternalError:(NSInteger)errorCode deviceID:(nullable NSString *)deviceID;

- (void)psMirrorMyVideoEnabledStatusChanged;
- (void)psVideoBoundChanged;
- (void)psFaceBeautyEnabledStatusChanged;
- (void)psFaceBeautyStrengthValueChanged;
- (void)psLightAdaptionEnabledStatusChanged;
- (void)psAutoLightAdaptionEnabledStatusChanged;
- (void)psLightAdaptionManualValueChanged;

@end

#endif /* ZMPSVideoStatusProtocol_h */
