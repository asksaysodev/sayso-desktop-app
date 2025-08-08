//
//  ZMMessageUIMgrProtocol.h
//  zChatComponent
//
//  Created by Kevin Li on 2022/8/5.
//

#ifndef ZMMessageUIMgrProtocol_h
#define ZMMessageUIMgrProtocol_h

#import <zChatComponent/ZMIMCommonDefine.h>
#import <zChatComponent/ZMChatsSessionViewProxy.h>

@class ZMFileEntity, ZMMessageAdapter, ZPZAppHeader, ZMChatAppShortcutInfo, ZMChatAppContext,ZMSendMessageParams, ZMShowCalendarEventParams, ZMScreenShotItem;
@protocol ZMMessageUIMgrProtocol

- (void)tryToCreateDocWithType:(NSInteger)type sessionID:(nullable NSString*)sessionID threadID:(nullable NSString*)threadID nodeID:(nullable NSString*)nodeID;
- (BOOL)tryToShareWithMessage:(nullable ZMMessageAdapter *)message;
- (void)uploadLocalSharePointFileInSession:(nullable NSString*)sessionID threadID:(nullable NSString*)threadID nodeID:(nullable NSString*)nodeID;
- (void)didGetRootNodeInfoWithSessionID:(nullable NSString*)sessionID rootNodeID:(nullable NSString*)nodeID requestID:(nullable NSString*)requestID;

- (nullable NSWindow *)mainIMResponsibleWindow;

- (BOOL)isChatWindow:(nonnull NSWindow *)window;

- (nullable id<ZMChatsSessionViewProxy>)viewProxyForChatsSessionWithSessionID:(nonnull NSString *)sessionId;

- (nullable NSView *)chatTabView;

- (void)checkSessionForEmitterFeature:(nonnull NSString *)sessionId onView:(nonnull NSView *)onView;
- (void)removeEmitterWith:(nonnull NSString*)sessionId;
- (void)removeAllHitEmitter;

- (void)showFileSharePannelByFileItem:(nonnull ZMFileEntity *)fileItem sessionID:(nullable NSString*)sessionID onWindow:(nonnull NSWindow*)inWindow;

- (void)showFileSharePannelBySessionID:(nonnull NSString*)sessionID andMsgID:(nonnull NSString*)MsgId onWindow:(nonnull NSWindow*)inWindow;

- (void)showDisbandChannelPanel:(nonnull NSString *)channelId onWindow:(nullable NSWindow*)inWindow;

- (void)showEventCardWithParams:(nonnull ZMShowCalendarEventParams *)params rect:(NSRect)rect positioningView:(nonnull NSView *)positioningView preferredEdge:(NSRectEdge)preferredEdge;

- (void)showEventDetailWithParams:(nonnull ZMShowCalendarEventParams *)params;

- (void)editMeetingInChatSession:(nonnull NSString *)sessionId meetingNumber:(long)meetingNumber occurrenceTime:(long)occurrenceTime meetingMasterEventId:(nonnull NSString *)meetingMasterEventId;
- (void)editMeetingInMeetingList:(nonnull NSString *)sessionId meetingNumber:(long)meetingNumber occurrenceTime:(long)occurrenceTime meetingMasterEventId:(nonnull NSString *)meetingMasterEventId;
- (void)deleteMeetingInChatSession:(nonnull NSString *)sessionId meetingNumber:(long)meetingNumber occurrenceTime:(long)occurrenceTime meetingMasterEventId:(nonnull NSString *)meetingMasterEventId onlyAllOccurrence:(BOOL)onlyAllOccurrence;
- (BOOL)isUserWithJID:(nullable NSString *)jid inMeetingWithMeetingNumber:(long)meetingNumber;

- (void)showSendMultiplePannel:(nonnull ZMSendMessageParams *)sendMessageParams onWindow:(nullable NSWindow *)onWindow completion:(nonnull ZMCallbackBlock)completion;

- (BOOL)isCreateZoomTasksEnabled;

- (BOOL)notifyScreenshotByHotkey;

// DL app bridge
- (void)notifyEventName:(nonnull NSString *)eventName service:(NSUInteger)service result:(BOOL)result params:(nonnull NSDictionary *)params;

- (void)notifyEventName:(nonnull NSString *)eventName params:(nonnull NSDictionary *)params;

- (NSString *)getTargetLanguage:(nullable BOOL *)isMandatory;

@end


#ifndef ZMShareMessageUIMgrProtocol
#define ZMShareMessageUIMgrProtocol (id <ZMMessageUIMgrProtocol>)ZMSharedFor(ZMMessageUIMgrProtocol)
#endif

#endif /* ZMMessageUIMgrProtocol_h */
