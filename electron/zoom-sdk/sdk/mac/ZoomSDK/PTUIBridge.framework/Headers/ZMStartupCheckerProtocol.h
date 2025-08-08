//
//  ZMStartupCheckerProtocol.h
//  ChatUI
//
//  Created by Michael Lin on 2023/3/10.
//  Copyright © 2023 Zoom. All rights reserved.
//

#ifndef ZMStartupCheckerProtocol_h
#define ZMStartupCheckerProtocol_h

typedef NS_ENUM(NSUInteger, ZMStartupType) {
    ZMStartupTypeInvalid = 0,
    ZMStartupTypeMeeting,
    ZMStartupTypePhone,
    ZMStartupTypeAudioMessage,
    ZMStartupTypeVideoMessage,
    ZMStartupTypeClips,
    ZMStartupTypeHuddles,
    ZMStartupTypeCCHybridMeeting,
};
@protocol ZMStartupCheckerProtocol <NSObject>

+ (BOOL)checkStartupAvailableWithType:(ZMStartupType)type;
+ (BOOL)checkStartupAvailableWithType:(ZMStartupType)type conflictType:(ZMStartupType *)conflictType;
+ (BOOL)checkStartupAvailableWithAlertViewWithType:(ZMStartupType)type;

@end

#endif /* ZMStartupCheckerProtocol_h */
