//
//  ZMLinkPreviewMgr.h
//  zChatUI
//
//  Created by likevin on 2018/8/4.
//  Copyright © 2018 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>

@class ZMChatMsgItemInfo;

@interface ZMLinkPreviewMgr : NSObject

+ (ZMLinkPreviewMgr*)sharedInstance;
+ (void)releaseInstance;
- (BOOL)isNeedShowLinkPreview:(NSString*)url andSessionID:(NSString*)sessionID MsgGuid:(NSString*)msgGuid;
- (void)removeUnusedLink2MSGItems:(NSDate*)cmpMsgTimeStamp andSessionID:(NSString*)sessionID;
- (void)removeLink2MsgItemBy:(NSString*)xmppGuid andSessionID:(NSString*)sessionID;
- (void)removeAllLink2MSGItemsWithSessionID:(NSString*)sessionID;

- (void)checkForLinkPreview:(ZMChatMsgItemInfo*)messageInfo;

/** This is a time-consuming function,  cache the result of 'getMessageLinks' if is possible */
- (NSArray*)getMessageLinks:(ZMChatMsgItemInfo*)messageInfo;

- (void)clearLinkPreviewRecord:(NSString*)sessionID;

@property (readonly) BOOL isEnableLinkPreview;

@end

@interface ZMLinkPreviewSessionMgr :NSObject
- (id)initWithSessionID:(NSString*)sessionID;
- (void)cleanUp;
- (BOOL)isNeedShowLinkPreview:(NSString*)url andMsgGuid:(NSString*)msgGuid;
- (void)removeUnusedLink2MSGItems:(NSDate*)cmpMsgTimeStamp;
- (void)removeLink2MsgItemBy:(NSString*)xmppGuid;
- (void)removeAllLink2MSGItems;
@end

