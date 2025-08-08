//
//  ZMMessageAPIHelperSwiftAdaptor.h
//  zChatComponent
//
//  Created by Cyan.Yang on 2024/6/20.
//

#import <Foundation/Foundation.h>
#import <zChatComponent/ZMMessageUIMgrProtocol.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMChatMsgItemInfo;
@class ZMChatAppContext;

NS_SWIFT_NAME(MessageAPIHelper)
@interface ZMMessageAPIHelperSwiftAdaptor : NSObject

+ (id<ZMMessageUIMgrProtocol>)sharedMessageUIMgr __attribute__((objc_direct));

+ (void)noteMessageDidEditWithMessageID:(NSString *)messageID
                              sessionID:(NSString *)sessionID
                                 result:(BOOL)result __attribute__((objc_direct));

+ (BOOL)isChatAppsShortcutsEnabled:(NSString *)sessionID __attribute__((objc_direct));
+ (void)openChatApp:(ZMChatAppContext *)appContext __attribute__((objc_direct));

+ (BOOL)isCompanionModeEnabled __attribute__((objc_direct));
+ (BOOL)isSchedulingFixedTimeRecurrMeetingEnabled __attribute__((objc_direct));
+ (BOOL)isMeetingScheduledInSessionWithSessionID:(NSString *)sessionID __attribute__((objc_direct));

+ (void)directlyJoinSubCMCWithSessionID:(NSString *)sessionID __attribute__((objc_direct));

+ (BOOL)trackAppActionInteraction:(NSString *)action messageID:(NSString *)messageID sessionID:(NSString *)sessionID;

@end

NS_ASSUME_NONNULL_END
