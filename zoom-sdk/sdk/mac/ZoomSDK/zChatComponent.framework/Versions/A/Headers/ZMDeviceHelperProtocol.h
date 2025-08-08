//
//  ZMDeviceHelperProtocol.h
//  zChatComponent
//
//  Created by groot.ding on 7/18/23.
//

#ifndef ZMDeviceInfoProtocol_h
#define ZMDeviceInfoProtocol_h

@protocol ZMDeviceHelperProtocol <NSObject>

+ (BOOL)hasMicDeviceAvailable;

+ (BOOL)hasCameraDeviceAvailable;

+ (NSInteger)checkAudioDeviceAuthorized;

+ (NSInteger)checkVideoDeviceAuthorized;

@end

#endif /* ZMDeviceInfoProtocol_h */
