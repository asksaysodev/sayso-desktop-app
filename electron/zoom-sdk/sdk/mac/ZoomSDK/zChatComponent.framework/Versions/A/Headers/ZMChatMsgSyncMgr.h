//
//  ZMChatMsgSyncMgr.h
//  zChatComponent
//
//  Created by Yong Zhou on 7/5/24.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMChatMsgSyncMgr : NSObject

+ (ZMChatMsgSyncMgr*)sharedInstance;

+ (void)releaseInstance;

- (void)addSyncLinkUnfurlingMsgInSession:(NSString*)sessionID messageID:(NSString*)messageID;

- (void)addVisibleMessagesInSession:(NSString*)sessionID messageID:(NSString*)messageID;

@end

NS_ASSUME_NONNULL_END
