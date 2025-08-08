//
//  ZMJumpHelper.h
//  zChatComponent
//
//  Created by groot.ding on 7/13/23.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMMessageAdapter;

@interface ZMJumpHelper : NSObject

+ (void)jump2Session:(NSString *)sessionId;

+ (void)jump2Message:(NSString *)msgId sessionId:(NSString *)sessionId highlightBackground:(BOOL)highlightBackground highlightBubble:(BOOL)highlightBubble;

+ (void)jump2Message:(ZMMessageAdapter *)message highlightBackground:(BOOL)highlightBackground highlightBubble:(BOOL)highlightBubble shouldCleanUnread:(BOOL)shouldCleanUnread;

+ (void)jump2Message:(NSString *)msgId sessionId:(NSString *)sessionId svrTime:(unsigned long long)svrTime isComment:(BOOL)isComment threadId:(NSString *)threadId threadSvrTime:(unsigned long long)threadSvrTime highlightBackground:(BOOL)highlightBackground highlightBubble:(BOOL)highlightBubble shouldCleanUnread:(BOOL)shouldCleanUnread;

+ (void)jump2Session:(NSString *)sessionId expandWithSession:(NSString *)expandSessionId;
+ (void)jump2Session:(NSString *)sessionId expandOrCloseWithSession:(NSString *)expandOrCloseSessionId;// if expandOrCloseSessionId closed, then expand it, if expandOrCloseSessionId opened, then close it.
+ (void)jump2Session:(NSString *)sessionId expandWithSession:(NSString *)expandSessionId jump2ExpandSessionMessage:(NSString *)messageId;// discarded, please use (void)jumpToChatBySessionId:(NSString *)sessionID andSearchedMsgID:(NSString*)msgID andSearchedItemInfo:(id)searchedItemInfo;

+ (void)pushSubCMCSession:(NSString *)sessionId inSession:(NSString *)inSessionId;

+ (void)jump2PinHistory:(NSString *)sessionId;

+ (void)jump2CheckUpgrade;

+ (void)jump2Zapp:(NSString *)zappId appUuid:(nullable NSString*)appUuid;

+ (BOOL)jump2WhiteBoardWithUrl:(NSString *)wbUrl;

+ (BOOL)jump2NoteWithUrl:(NSString *)noteUrl;

+ (BOOL)tryToOpenZoomInternalLink:(nonnull NSString*)shareLinkURL sessionID:(nullable NSString*)sessionID;

+ (BOOL)jump2Setting:(NSInteger)type;

@end

NS_ASSUME_NONNULL_END
