//
//  ZMSettingAudioShareMgr.h
//  SaasBeeConfUIModule
//
//  Created by zoom on 13-7-27.
//  Copyright (c) 2013 zoom.us. All rights reserved.
//

#import <Foundation/Foundation.h>
typedef NS_ENUM(NSInteger,ZMInstallReturn){
    ZMInstallReturnSuccess,
    ZMInstallReturnFileNotExist,
    ZMInstallReturnCancel,
    ZMInstallReturnSystemPolicy,
    ZMInstallReturnError,
};
@class ZMAudioMgr;
@interface ZMSettingAudioShareMgr : NSObject

+ (instancetype)shared;

- (BOOL)isSharingComputerAudio;
- (BOOL)isAudioShareEnabled;

- (BOOL)isAudioDeviceInstalled;
- (ZMInstallReturn)installAudioDevice;
- (BOOL)isCoreAudioRunning;//[Zoom-43850]
- (BOOL)isCoreAudioJustKilled;//[Zoom-43850]
- (NSString *)getCoreAudioProcessID;
@end
