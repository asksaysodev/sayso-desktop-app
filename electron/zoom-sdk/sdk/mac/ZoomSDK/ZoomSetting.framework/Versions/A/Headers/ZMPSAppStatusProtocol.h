//
//  ZMPSAppStatusProtocol.h
//  ZoomSetting
//
//  Created by Michael Lin on 2024/1/11.
//

#ifndef ZMPSAppStatusProtocol_h
#define ZMPSAppStatusProtocol_h
#import <zPSApp/define/PSDefine.h>

@protocol ZMPSAppStatusProtocol <NSObject>

- (void)psOnPTRequestActiveApp:(ps::PSAsyncRecordingActionType)actionType;
- (void)psRequestToTermWithReason:(ps::PSTerminateReason)reason;
- (void)psAllSceneConfigReady;
- (void)psOnUserLoginChanged;
- (void)psOnIPCDisconnected;

@end

#endif /* ZMPSAppStatusProtocol_h */
