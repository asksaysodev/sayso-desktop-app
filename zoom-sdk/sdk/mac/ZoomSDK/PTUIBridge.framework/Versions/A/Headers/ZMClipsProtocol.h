//
//  ZMClipsProtocol.h
//  ChatUI
//
//  Created by Michael Lin on 2023/1/16.
//  Copyright © 2023 Zoom. All rights reserved.
//

#ifndef ZMClipsProtocol_h
#define ZMClipsProtocol_h

#import <Foundation/Foundation.h>
#ifdef __cplusplus
#include "zPSApp/define/PSDefine.h"
#endif

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, ZMClipsLaunchPosition) {
    ZMClipsLaunchPositionExternal,
    ZMClipsLaunchPositionStatusBarMenu,
    ZMClipsLaunchPositionClipsWebView,
    ZMClipsLaunchPositionShortcut,
};
@class ZMClipsTabMgr;
@protocol ZMSwitchAccountModelProtocol;
@protocol ZMClipsProtocol <NSObject>

+ (nonnull instancetype)shared;
- (void)showZoomClips:(ZMClipsLaunchPosition)position;
- (void)showZoomClipsForAIAvatar;
- (void)showZoomClipsWithRecordInfo:(nullable NSString *)recordInfo position:(ZMClipsLaunchPosition)position;
- (void)internalLaunchZoomClipsForPreload:(BOOL)preload type:(NSUInteger)type;

- (void)onZoomLoginFinished;

- (BOOL)isZoomClipsRecorderAvailable;
- (BOOL)isZoomClipsTabAvailable;
- (BOOL)isZoomClipsRunning;

- (void)clipsEnableChanged;
- (void)psOnRecvCustomMessage:(nullable NSString *)message;
- (void)sendCustomMessage:(nullable NSString *)message;

- (void)showAvatarSetting;
#ifdef __cplusplus
- (void)terminateZoomClipsWithReason:(ps::PSTerminateReason)reason;
- (void)terminateZoomClipsWithReason:(ps::PSTerminateReason)reason switchAccountModel:(nullable id<ZMSwitchAccountModelProtocol>)model isSignOutAll:(BOOL)isSignOutAll;
- (void)openClipsContentInfoWithResult:(ps::PSAsyncRecordingUploadResultToPT)result;
- (void)clipsTerminateWithResponse:(ps::PSTerminateResponse)response;
- (BOOL)showZoomClipsWithActionToken:(ps::PSAsyncRecordingActionToken)actionToken;
#endif
@end

NS_ASSUME_NONNULL_END

#endif /* ZMClipsProtocol_h */
