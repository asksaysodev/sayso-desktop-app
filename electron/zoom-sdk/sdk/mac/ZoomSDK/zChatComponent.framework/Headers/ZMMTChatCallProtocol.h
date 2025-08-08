//
//  ZMMTChatCallProtocol.h
//  zChatComponent
//
//  Created by Devl on 1/24/24.
//

#ifndef ZMMTChatCallProtocol_h
#define ZMMTChatCallProtocol_h

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, ZMMTChatShortcutType) {
    ZMMTChatShortcut_NewChat,
    ZMMTChatShortcut_FocusLists,
    ZMMTChatShortcut_FoucsLatestMessage,
    ZMMTChatShortcut_FocusInput,
};

@class ZMChatAppContext;
@class ZMMessageAdapter, ZMToast;
@protocol ZMMTChatCallProtocol <ZMRoutableObject>

- (BOOL)mtChatCanChat:(NSString*)sessionID;
- (BOOL)mtChatCanSendFile;
- (BOOL)mtChatAmIHostCohost;
- (BOOL)mtChatInSilentMode;
- (BOOL)mtChatIsTeamChatEnabled;
- (BOOL)mtChatIsChannelActive:(NSString*)channelID;
- (BOOL)mtChatIsValidMessage:(ZMMessageAdapter*)adapter;
- (BOOL)mtChatNeedShowReceiver:(ZMMessageAdapter*)adapter;
- (BOOL)mtChatCanPreviewHoverLink:(NSString*)sessionID;

- (BOOL)mtChatCheckError:(NSString*)sessionID threadID:(NSString*)threadID;
- (BOOL)mtChatCanSendChatTo:(NSDictionary*)info;
- (void)mtChatSelectChatTo:(NSDictionary*)info;

- (BOOL)mtChatCheckFileSizeFailed:(unsigned long long)size isScreenshot:(BOOL)isScreenshot;
- (BOOL)mtChatCheckFileTypeFailed:(NSString*)path isScreenshot:(BOOL)isScreenshot;
- (unsigned long long)mtChatGetMaxFileSize;

- (NSString*)mtChatGetAccountID;
- (NSString*)mtChatGetCurrentSessionID;
- (NSWindow*)mtChatGetMainWindow;

- (NSInteger)mtChatGetPlistCount:(NSString*)sessionID;
- (NSString*)mtChatGetSendUserID:(NSString*)sessionID threadID:(NSString*)threadID;
- (NSInteger)mtChatGetSendMsgType:(NSString*)sessionID threadID:(NSString*)threadID;
- (NSInteger)mtChatGetMsgNodeID:(NSString*)sessionID threadID:(NSString*)threadID;

- (NSImage*)mtChatGetAvatarByJID:(NSString*)jid name:(NSString*)name msgType:(NSInteger)msgType;
- (NSImage*)mtChatGetAvatarByGuid:(NSString*)guid name:(NSString*)name msgType:(NSInteger)msgType;
- (NSImage*)mtChatGetAvatarByConfId:(NSString*)confId name:(NSString*)name msgType:(NSInteger)msgType;
- (NSImage*)mtChatGetMyAvatar;

#ifdef __cplusplus
- (std::set<CString>)mtChatGetUserList:(NSString*)sessionID;
#endif
- (void *)mtChatGetZAppModuleAPI;

- (void)mtChatLoadChatDBReady;
- (void)mtChatLoadChatDataReady;
- (void)mtChatRefreshChatDataReady;

- (void)mtChatShowDeletedByDlpAlert;
- (void)mtChatOpenAppInvitation:(ZMChatAppContext*)context;

//shortCut
- (BOOL)mtChatRunShortcut:(ZMMTChatShortcutType)type;

//meeting toast
- (void)mtChatDeliverToast:(ZMToast*)toast;
@end

NS_ASSUME_NONNULL_END

#endif /* ZMMTChatCallProtocol_h */
