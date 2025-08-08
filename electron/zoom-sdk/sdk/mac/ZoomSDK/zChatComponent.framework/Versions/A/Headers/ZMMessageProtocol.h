//
//  ZMMessageProtocol.h
//  SaasBeePTUIModule
//
//  Created by groot.ding on 2019/7/18.
//  Copyright © 2019 Zoom. All rights reserved.
//

#ifndef ZMMessageProtocol_h
#define ZMMessageProtocol_h

#if defined(__cplusplus)
#import <dataModule/zDataConstants.h>
#endif

#import <zChatComponent/ZMIMCommonDefine.h>

#if defined(__cplusplus)
#include <zChatApp/IZoomMessenger.h>
#endif

#import <zChatComponent/ZMMessageDraftHelper.h>
#import <zChatComponent/ZMReminderMessageMgrProtocol.h>
#import <zChatComponent/ZMIMInputCustomButtonMgrProtocol.h>
#import <zChatComponent/ZMEmojiModel.h>

@class ZMSyncDataResult, ZMMTBuddyModel;
@class ZMMessageAdapter;
@class ZMSyncMessageFilter;
@class ZMStickerInfo;
@class ZMFileIntegrationShareInfo;
@class ZMPinMessageCallback;
@class ZMPinMessageInfo;
@class ZMUserInfo;
@class ZMMessageRevokeInfo;
@class ZMThreadCommentState;
@class ZMFileEntity;
@class ZMBuddyAdapter;
@class ZMDLPPolicyResult;
@class ZMToastCoordinator;
@class ZMChatsCommandData;
@class ZMSendMessageSessionInfo;
@class ZMChannelMeetingEvent;
@class ZMChannelMeetingStatus;
@class ZMScheduleMeetingCallback;
@class ZMChannelSpotMeetingCallBack;
@class ZMCustomMessageInfo;
@class ZMMeetingChatCardEntity;
@class ZMMeetingChatCardReqParticipantsPara;
@class ZMMCCSyncMessageResult;
@class ZMMCCSyncParticipantsResult;
@class ZMShareEmailInfo;
@class ZMChatAppInfo;
@class ZMChatAppShortcutInfo;
@class ZMChatAppContext;
@class ZMChatAppMessagePreviewInfo;
@class ZMChatAppComposeMessageInfo;
@class ZMSendMessageParams;
@class ZMEditMessageInfoResult;
@class ZMFileCallbackParam;
@class ZMChannelMeetingAttendeeEvent;
@class ZMMeetingAssetsSharingResult;
@class ZMShareMessageInfo;
@class ZMAddExternalUserInfo;
@class ZMChannelOwnerSpecialSettingInfo;
@class ZMSmartSchedulerRspInfo;
@class ZMInputBoxLinkInfoSyncedResult;
@class ZMAccountAdminWhoInviteExternalToGroupSettingInfo;
@class ZMVoicemailStatusChangedInfo;
@class ZMMarkUnreadMessageMentionInfo;

NS_ASSUME_NONNULL_BEGIN

#define kChatMessageSessionId       @"kChatMessageSessionId"
#define kChatMessageMsgType         @"kChatMessageMsgType"
#define kChatMessageUserGuid        @"kChatMessageUserGuid"
#define kChatMessageUserConfId      @"kChatMessageUserConfId"
#define kChatMessageUserName        @"kChatMessageUserName"

#define kChannelBelongTo            @"kChannelBelongTo"
#define kChannelViewHistory         @"kChannelViewHistory"

static NSString * _Nonnull const kZMSendMessageProtocol_CustomEmojiPlaceholderMsgID = @"##kZMSendMessageProtocol_CustomEmojiPlaceholderMsgID##";

typedef NS_ENUM(NSUInteger, ZMThreadSortType){
    ZMThreadSortTypeVisableTime,
    ZMThreadSortTypeThreadTime
};

typedef NS_ENUM(NSUInteger, ERR_CODE){
    ERR_CODE_TIMEOUT = 1001,
    ERR_CODE_REDIRECTED,
    ERR_CODE_DataStatePreviewNoPermission,
};

typedef NS_ENUM(NSUInteger, ZMJumpMessageSourceType){
    ZMJumpMessageFromUnknow,
    ZMJumpMessageFromPinMessage,
    ZMJumpMessageFromSearch,
};

typedef NS_ENUM(NSUInteger, InputBoxMsgType) {
    InputBoxMsgType_TextOnly = 0,
    InputBoxMsgType_SingleFileOnly = 1 << 0,
    InputBoxMsgType_HybridMsg = 1 << 1,
};

typedef NS_ENUM(NSUInteger, ZMReminderMessageStatus) {
    ZMReminderMessageStatus_OK,
    ZMReminderMessageStatus_Unknown,
    ZMReminderMessageStatus_Fail,
    ZMReminderMessageStatus_Fail_Session_Not_Found,
    ZMReminderMessageStatus_Probing_In_Progress,
    ZMReminderMessageStatus_Sync,
    ZMReminderMessageStatus_Sync_Expire,
    ZMReminderMessageStatus_Sync_Set,
    ZMReminderMessageStatus_Sync_Edit,
    ZMReminderMessageStatus_Sync_Reschedule,
};

typedef NS_ENUM(NSUInteger, ZMReminderMessageEvent) {
    ZMReminderMessageEvent_None,
    ZMReminderMessageEvent_Set,
    ZMReminderMessageEvent_Edited,
    ZMReminderMessageEvent_Rescheduled,
};

typedef NS_ENUM(NSUInteger, ZMReminderMessageError) {
    ZMReminderMessageError_OK,
    ZMReminderMessageError_Unknown,
    ZMReminderMessageError_System,
    ZMReminderMessageError_NotImplemented,
    ZMReminderMessageError_MessageNotFound,
    ZMReminderMessageError_ReminderNotFound,
    ZMReminderMessageError_E2E,
    ZMReminderMessageError_OpDisabled,
    ZMReminderMessageError_UnsupportedMessageType,
    ZMReminderMessageError_TooMany,
    ZMReminderMessageError_TimeoutTooSmall,
    ZMReminderMessageError_TimeoutTooBig,
    ZMReminderMessageError_NoteTooLong,
    
    ZMReminderMessageError_NoSyncPerformed = UINT_MAX
};

typedef NS_ENUM(NSUInteger, ZMSummaryLocationType) {
    ZMSummaryLocation_FloatButton,
    ZMSummaryLocation_FileSummary,
    ZMSummaryLocation_FirstMessageHint,
    ZMSummaryLocation_LatestMessageHint,
    ZMSummaryLocation_Sidebar,
};

typedef NS_ENUM(NSUInteger, ZMBotMessageUpdateTriggerType) {
    ZMBotMessageUpdateTriggerTypeUnspecified = 0,
    ZMBotMessageUpdateTriggerTypeStopScrolling,
    ZMBotMessageUpdateTriggerTypeUserInteraction,
};

@protocol ZMSyncMessageProtocal <NSObject>

- (nullable ZMSyncDataResult *)syncThreadsByFilter:(nonnull ZMSyncMessageFilter *)filter completion:(nullable ZMCallbackBlock)completion;

- (nullable ZMSyncDataResult *)syncCommentsByFilter:(nonnull ZMSyncMessageFilter *)filter completion:(nullable ZMCallbackBlock)completion;

- (nullable ZMSyncDataResult *)syncSingleThread:(nonnull ZMSyncMessageFilter *)filter completion:(nullable ZMCallbackBlock)completion;

- (BOOL)cancelDBRequest:(nonnull NSString *)requestId sessionId:(nonnull NSString *)sessionId;

- (BOOL)cancelXMSRequest:(nonnull NSString *)requestId sessionId:(nonnull NSString *)sessionId;

- (BOOL)hasSyncMessageCallBackByRequestId:(nonnull NSString *)requestId;

- (void)addSyncMessageCallBackByRequestId:(nonnull NSString *)requestId sessionId:(nonnull NSString *)sessionId threadId:(nullable NSString *)threadId syncWeb:(BOOL)syncWeb block:(nullable ZMCallbackBlock)completion;
@end

@protocol ZMSyncMessageCallback <NSObject>

- (void)didSyncThreadDataByDataResult:(nonnull ZMSyncDataResult *)syncDataResult;

- (void)didSyncCommentDataByDataResult:(nonnull ZMSyncDataResult *)syncDataResult;

- (void)didSyncSignleThread:(nonnull NSString *)threadId sessionId:(nonnull NSString *)sessionId requestId:(nonnull NSString *)requestId fromAsync:(BOOL)fromAsync success:(BOOL)success;

@end

@protocol ZMSyncSessionUnreadCallback <NSObject>

- (void)didSessionUnreadCountReady:(nonnull NSArray <NSString *> *)sessionIds;

@end

@protocol ZMCheckMessageProtocal <NSObject>

- (BOOL)checkHasMoreCommentsByChannelID:(nonnull NSString*)channelID threadID:(nonnull NSString*)threadID startCommentID:(nonnull NSString*)startCommentID;

- (BOOL)checkHasRecentCommentsByChannelID:(nonnull NSString*)channelID threadID:(nonnull NSString*)threadID startCommentID:(nonnull NSString*)startCommentID;

- (BOOL)checkHasMoreHistoricThreadsWithChannelID:(nonnull NSString*)sessionID startThreadID:(nonnull NSString*)threadID;

- (BOOL)checkHasMoreRecentThreadsWithChannelID:(nonnull NSString*)sessionID startThreadID:(nonnull NSString*)threadID;

- (BOOL)checkMessageIsDirtyByChannelID:(nonnull NSString *)channelID threadID:(nullable NSString*)threadID commnetID:(nonnull NSString *)commnetID;

- (BOOL)checkThreadHasReply:(nonnull NSString *)threadId sessionId:(nonnull NSString *)sessionId;

- (NSInteger)checkThreadHasReplyOdds:(nonnull NSString *)threadId sessionId:(nonnull NSString *)sessionId;

// v5.12.0-persistent chat: original messages are from old meeting-chat senders
- (BOOL)checkThreadIsOriginalMeetingMessage:(nonnull NSString *)threadId sessionId:(nonnull NSString *)sessionId;

- (BOOL)checkSessionHaveLastMessage:(nonnull NSString *)sessionId;

- (void)checkSHowSmartScheduleButtonForMsg:(nonnull NSArray *)msgs sessionId:(nonnull NSString *)sessionId requestId:(NSString *_Nullable*_Nullable)requestId;

- (BOOL)checkMailZipValidWithFilePath:(nonnull NSString *)filePath messageId:(nonnull NSString *)messageId sessionId:(nonnull NSString *)sessionId;
@end

@protocol ZMOperateMessageProtocol <NSObject>

- (nullable ZMMessageAdapter *)getMessageById:(nonnull NSString *)msgId sessionId:(nonnull NSString *)sessionId;

- (nullable ZMMessageAdapter *)getMessageWithoutStyleById:(nonnull NSString *)msgId sessionId:(nonnull NSString *)sessionId;

- (nullable ZMMessageAdapter *)getMessageByServerTime:(long long)serverTime sessionId:(nonnull NSString *)sessionId;

- (nullable ZMMessageAdapter *)getMessageByServerTime:(long long)serverTime sessionId:(nonnull NSString *)sessionId useDb:(BOOL)useDB;

- (nullable ZMMessageAdapter*)getLastMessageBySessionId:(nonnull NSString *)sessionId;

- (nullable ZMMessageAdapter*)GetLastMessageFromMemoryOrDBBySessionId:(nonnull NSString *)sessionId;

- (nullable NSString *)getMessageIDByServerTime:(long long)serverTime sessionId:(nonnull NSString *)sessionId;

#if defined(__cplusplus)
- (MessageType)getMessageType:(nonnull NSString *)messageId sessionId:(nonnull NSString *)sessionId;
#endif

- (BOOL)checkMessageExistById:(nonnull NSString *)msgId sessionId:(nonnull NSString *)sessionId;

- (nullable NSDate *)getThreadVisibleTimeByChannelID:(nonnull NSString*)channelID threadID:(nonnull NSString*)threadID;

- (long long)getVisibleTimeWithSessionID:(nonnull NSString *)sessionId threadID:(nonnull NSString *)messageId;

- (long long)getServerVisibleTimeWithSessionID:(nonnull NSString*)sessionID threadID:(nonnull NSString*)messageId;

- (long long)getSessionLocalStorageEraseTimeWithSessionID:(nonnull NSString *)sessionID;

- (nullable NSDate *)getMessageTimeStampById:(nonnull NSString *)msgId sessionId:(nonnull NSString *)sessionId;

- (void)readMessageById:(nonnull NSString *)msgId sessionId:(nonnull NSString *)sessionId;

- (BOOL)updateBotMessageByIds:(nonnull NSArray<NSString *> *)msgIds sessionId:(nonnull NSString *)sessionId triggerType:(ZMBotMessageUpdateTriggerType)triggerType;

- (void)deleteMessage:(nonnull ZMMessageAdapter *)message;

- (void)deleteLocalMessage:(nonnull ZMMessageAdapter *)message;

- (BOOL)canDeleteBotMessage:(nonnull NSString *)msgId sessionId:(nonnull NSString *)sessionId;

- (BOOL)canDeleteMessageWithSessionId:(nullable NSString *)sessionId;

- (BOOL)canEditMessageWithSessionId:(nullable NSString *)sessionId;

- (BOOL)isWithinDeleteMsgLimitForTimestamp:(NSDate *)timeStamp;

- (BOOL)isWithinEditMsgLimitForTimestamp:(NSDate *)timeStamp;

- (BOOL)markMessage:(nonnull ZMMessageAdapter *)message asUnread:(BOOL)asUnread;

- (BOOL)markMessage:(NSInteger)msgSvrTime sessionId:(nonnull NSString *)sessionId asUnread:(BOOL)asUnread;

- (BOOL)markLastMessageUnreadBySessionId:(nonnull NSString *)sessionId;

- (BOOL)isMarkUnreadMessage:(nonnull NSString *)messageId sessionId:(nonnull NSString *)sessionId;

- (BOOL)clearAllMarkedUnreadMessagesBySessionId:(nonnull NSString *)sessionId;

- (nullable NSArray <ZMCustomMessageInfo *>*)getMarkUnreadMessagesBySessionId:(nonnull NSString *)sessionId;

- (BOOL)isUnreadReply:(nonnull NSString *)replyMessageId sessionId:(nonnull NSString *)sessionId;

- (BOOL)tryDecodeMessageById:(nonnull NSString *)messageId sessionId:(nonnull NSString *)sessionId;

- (BOOL)decodeCMKMessagesWithIDs:(nonnull NSArray<NSString *> *)messageIDs inSessionWithID:(nonnull NSString *)sessionID;

- (BOOL)isThreadCommentInfoAccurate:(nonnull NSString *)messageId sessionId:(nonnull NSString *)sessionId;

- (void)syncMessageEmojiCountInfoWithChanelID:(nonnull NSString*)channelID msgArray:(nonnull NSArray<NSString*>*)msgArray;

- (void)addTempMessageWithBody:(nonnull NSString *)msgBody sessionID:(nonnull NSString *)sessionID;

- (void)searchUnsupportMessages:(nonnull NSArray <ZMMessageAdapter*> *)messages;

#if defined(__cplusplus)
- (nullable NSString *)insertSystemMessage:(nonnull NSString *)msgBody inMsgId:(nonnull NSString *)inMsgId sessionId:(nonnull NSString *)sessionId sendId:(nullable NSString *)sendId messageType:(MessageType)type isRead:(BOOL)isRead timeStamp:(nullable NSDate*)timeStamp serverSideTime:(NSUInteger)svrSideTm prvMsgTime:(NSUInteger)prvMsgTm outTimeStamp:(nullable NSUInteger*)pOutTimestamp;
#endif

- (NSArray <ZMChatAppMessagePreviewInfo *>*)beginEditMessageWithMessageId:(NSString *)messageId sessionId:(NSString *)sessionId threadId:(NSString *)threadId;

- (BOOL)editMessageWithBody:(nonnull NSString *)msgBody sessionId:(nonnull NSString *)sessionId guid:(nonnull NSString *)guid atList:(nullable NSArray *)atList emojiInfo:(nullable void *)emojiInfo fontStyleInfo:(nullable NSArray*)fontStyleList fileList:(nullable NSArray *)fileList isE2E:(BOOL)isE2E  isAutoEditWBPreviewMsg:(BOOL)isAutoEditWBPreviewMsg hasAppPreviewCard:(BOOL)hasAppPreviewCard isSharedMessage:(BOOL)isSharedMessage editedNote:(nullable NSString *)editedNote dismissLinks:(nullable NSArray *)dismissLinks appPreviewInfoArray:(nullable NSArray *)appPreviewsInfoArray
     appMessagePreviewField:(nullable NSDictionary *)appMessagePreviewField;

- (BOOL)syncThreadCommentCountWithChannelID:(nonnull NSString*)channelID msgSvrTimes:(nonnull NSArray<NSNumber*> *)msgSvrTimeArray;

- (BOOL)isThreadCommentCountSyncedWithChannelID:(nonnull NSString*)channelID msgSvrTime:(long long)svrTime;

- (ZMThreadSortType)getThreadSortTypeWithSessionId:(nullable NSString *)sessionId;

- (ZMThreadSortType)getThreadSortTypeIfHavePending;

- (void)setThreadSortType:(ZMThreadSortType)sortType;

- (BOOL)isDMOPFlagDisabled;
- (BOOL)isDMDisabledSession:(NSString *)sessionID;

- (BOOL)isCreateMUCDisabled;

- (BOOL)isDisableReply;

- (BOOL)isDisableReplyWithSessionId:(nullable NSString *)sessionId;

- (BOOL)isDisableReactionWithSessionId:(nullable NSString *)sessionId;

- (nullable NSArray <NSString *> *)getUnreadAllMentionedMessagesBySessionId:(nonnull NSString *)sessionId;

- (long long)getReadedMsgTimeBySessionId:(nonnull NSString *)sessionId;

- (NSInteger)getUnreadCountBySessionId:(nonnull NSString *)sessionId;

- (void)cleanUnreadMessageCountBySessoinId:(nonnull NSString *)sessionId cleanTotal:(BOOL)bCleanTotal;

- (NSInteger)getUnreadCommentCount:(long long)threadServerTime sessionId:(nonnull NSString *)sessionId;

- (BOOL)cleanUnreadCommentsForThread:(long long)threadServerTime sessionId:(nonnull NSString *)sessionId;

- (BOOL)setMessageAsRead:(BOOL)isRead messageId:(nonnull NSString *)messageId sessionId:(nonnull NSString *)sessionId;

- (BOOL)isUnreadMessage:(nonnull NSString *)messageId sessionId:(nonnull NSString *)sessionId;

- (NSInteger)getMarkUnreadMessageCount:(nonnull NSString *)sessionId;

- (ZMMarkUnreadMessageMentionInfo *)getMarkUnreadMentionMessageInfo:(nonnull NSString *)sessionId;

- (nullable NSArray <ZMThreadCommentState *> *)getUnreadReplysInfoBySessionId:(nonnull NSString *)sessionId;

- (nullable NSArray *)getSendErrorMessagesBySessionId:(nonnull NSString *)sessionId;

- (BOOL)isSubCMCHasSendErrorMessageBySessionId:(nonnull NSString *)sessionId;

- (BOOL)checkSessionCanChat:(nonnull NSString *)sessionId;

- (BOOL)checkSessionCanAddMsgReply:(NSString *)sessionId;

- (BOOL)checkSessionCanAddReaction:(nonnull NSString *)sessionId;

- (BOOL)isEnableE2EWithSessionID:(nonnull NSString*)sessionId;

- (BOOL)isUnifySearchChannelAndContactEnabled;

#if defined(__cplusplus)
- (E2EChatAbility)checkE2EAbility:(E2EChatOption)e2eChatOption withJID:(NSString*)jid;
#endif

- (BOOL)isHybridMessageEnabledWithSessionId:(NSString*)sessionId;

- (BOOL)isFileTransferDisabled;
- (BOOL)isFileViewDisabled;
- (BOOL)isFileViewDisabled:(NSString *)sessionId;

- (BOOL)isHyperlinkDisabledWithSessionID:(nullable NSString *)sessionID;

- (BOOL)isCodeblockEnabledWithSessionID:(nullable NSString *)sessionID;

- (BOOL)isDisableInternalPresence;

- (BOOL)isAllowWhiteboardPreviewShareToChat:(nonnull NSString*)sessionId;

- (nullable NSMutableArray<ZMFileEntity*> *)filterWhiteboardURLWithArray:(nullable NSMutableArray<NSString*> *)urlMutableArray andSessionId:(nonnull NSString *)sessionId;

- (BOOL)canSendFileInSession:(nonnull NSString*)sessionID;

- (BOOL)isRichTextFormatEnabledWithSessionId:(nullable NSString *)sessionId;
- (BOOL)isRichTextFormatEnabledOnWebWithSessionId:(nullable NSString *)sessionId;

- (BOOL)isChatAppsShortcutsEnabled;

- (BOOL)isChatAppEnabledInSession:(NSString *)sessionId;
- (BOOL)isChatAppEnabledInEditReplySession:(NSString *)sessionId;
- (BOOL)isChatAIComposeRedesignEnabled:(NSString *)sessionId;

- (long long)getMaxFileSizeInSession:(nonnull NSString*)sessionID;

- (long long)getMaxFileSize4InternalInByte;

- (long long)getMaxFileSize4ExternalInByte;

// ZOOM-786530
- (long long)getMaxFileViewSize4InternalInByte;
- (long long)getMaxFileViewSize4ExternalInByte;

- (ZMFTPermissionType)getUploadFilePermissionInSession:(nonnull NSString*)sessionID fileSuffix:(nonnull NSString*)fileSuffix;

- (ZMFTPermissionType)getViewFilePermissionInSession:(nonnull NSString*)sessionID sourceType:(NSInteger)sourceType fileSuffix:(nonnull NSString*)fileSuffix senderJid:(nonnull NSString*)senderJid;

- (BOOL)isValidInsertLink:(nonnull NSString*)inputLink outLink:(NSString *_Nullable *_Nullable)outLink sessionId:(nullable NSString *)sessionId;

- (BOOL)isRecordMessageEnabledWithSessionId:(nullable NSString *)sessionId;

- (BOOL)isRecordVideoMessageEnabled;

- (BOOL)needShowSendVideoMorethan30MTip;

- (BOOL)isEnablePlayableVideoMsgWithSessionId:(nullable NSString *)sessionId;

- (BOOL)isNoMeetingLicenseUser;

- (nullable NSString*)getRootNodeInfoFromCacheWithSessionID:(nonnull NSString*)sessionID;

- (BOOL)checkConnectionGood:(nonnull NSString *)sessionId;

- (nullable NSString *)getJoinMeetingURLByCallHistoryMessageId:(nonnull NSString *)messageId sessionId:(nonnull NSString *)sessionId;

- (BOOL)isSessionUnreadReady;

- (nullable NSString *)getDataFolderInSession:(nonnull NSString *)sessionId;

- (BOOL)isBroadcastAnnouncementsSession:(nonnull NSString *)sessionID;

- (BOOL)isScreenCapturePolicyEnabled;

- (BOOL)isScreenCaptureEnabled;

- (BOOL)isSmartSchedulerEnabled;

- (BOOL)is3rdPartStorageEnabledInChannel:(nonnull NSString*)groupID;

- (BOOL)isBoxEnabledInChannel:(nonnull NSString*)groupID;

- (nullable NSArray*)getFileStorageSupportedTypesFromCache;

- (nullable NSString*)getFileStorageSupportedTypes;

- (nullable NSArray*)getArrayForFileIntegrationShare;

- (nullable NSString*)getOpusFilePathWithSessionId:(nonnull NSString*)sessionID;

- (nullable NSArray*)getAllRobotCmdBySessionID:(nonnull NSString*)inSessionId;

- (BOOL)set1V1Session:(nonnull NSString*)sessionId inputStatus:(NSInteger)inStatus;

- (BOOL)isE2EGetCanSendMessageCipher:(NSUInteger *_Nonnull)outReason sessionID:(nonnull NSString *)sessionID;

- (NSUInteger)getBuddyCountInSession:(nonnull NSString*)sessionID;

- (NSUInteger)getMucUserLimit;

- (BOOL)isDisableCreatePrivateChannel;

- (BOOL)IsEnableChannelIcon;

- (BOOL)isDLPEnabled;

- (nullable ZMDLPPolicyResult*)checkDLPPolicyWithConent:(nullable NSString*)content;

- (void)setAsTrustedSenderForDownloadFileWithSenderJID:(nonnull NSString*)senderJID trust:(BOOL)trust;

- (BOOL)setLastUsedRobotCommand:(nullable ZMChatsCommandData*)commandData sessionId:(nonnull NSString *)sessionId;

- (nullable ZMChatsCommandData*)getLastUsedRobotCommand;

- (nullable NSString*)sendAddonMessage:(nonnull NSString*)message sessionID:(nonnull NSString *)sessionID jid:(nonnull NSString*)jid;

- (nullable NSString*)getCorrectLink:(nullable NSString*)relativeURL;

- (BOOL)requestSaveOrOpenUrlWithType:(NSInteger)type isSaved:(BOOL)isSaved messageInfo:(nonnull ZMMessageAdapter *)msg;

- (BOOL)isStarredMessage:(nonnull NSString*)sessionID serverTime:(long long)time;
- (void)starMessage:(nonnull ZMMessageAdapter *)inMsgInfo;
- (void)unStarMessage:(nonnull ZMMessageAdapter *)inMsgInfo;

- (void)unStarMessageBySessionId:(nonnull NSString *)sessionID serverTime:(long long)time;

- (nullable NSArray *)getIntegrationFileManagerListWithMessage:(nonnull ZMMessageAdapter *)message;

- (BOOL)isThreadFollowedWithSessionID:(nonnull NSString*)sessionID threadID:(nonnull NSString*)msgID;
- (BOOL)followThreadWithSessionID:(nonnull NSString*)sessionID threadID:(nonnull NSString*)msgID;
- (BOOL)discardFollowThreadWithSessionID:(nonnull NSString*)sessionID threadID:(nonnull NSString*)msgID;

- (ZMStartRecordStatus)checkStartRecordAudioStatus;
- (ZMStartRecordStatus)checkStartRecordVideoStatus;

- (nonnull NSString *)getMeetingJidByUserId:(nonnull NSString *)userID sessionId:(nonnull NSString *)sessionId;
- (nonnull NSString*)getMeetingChatUserIdByJid:(nonnull NSString *)jid sessionId:(nonnull NSString *)sessionId;
- (BOOL)checkMeetingMessageCanBeDeleted:(NSString *)messageId sessionId:(NSString *)sessionId;

- (BOOL)isOnlyVisibleToYouForMessage:(NSString *)messageId inSession:(NSString *)sessionID;
- (BOOL)isAppIconDisabledForMessage:(NSString *)messageID inSession:(NSString *)sessionID;

- (nullable NSString *)GetZoomMeetPMCChannelID:(nullable NSString *)groupID ;
- (BOOL)PMC_CheckInTeamChatFromMeetingChat:(nullable NSString *)accountID meetingGroupID:(nullable NSString *)meetingGroupID PMCChannelID:(nullable NSString *)PMCChannelID;

- (NSArray *)getTodayMeetingChatsList;

/** messageId, threadId, messageSvrTime and ThreadSvrTime can’t be empty if jump to a message */
- (BOOL)PMC_OpenTeamChatFromMeetingChatWithAccountID:(nullable NSString *)accountID meetingGroupID:(nullable NSString *)meetingGroupID PMCChannelID:(nullable NSString *)PMCChannelID messageId:(nullable NSString *)messageId threadId:(nullable NSString *)threadId messageSvrTime:(NSInteger)messageSvrTime threadSvrTime:(NSInteger)threadSvrTime;

#if defined(__cplusplus)
- (void)PMC_NotifyTeamChatUpdated:(PMCTeamChatUpdatedInfo)info;
#endif

- (BOOL)isAdminRemoveChannelMsgEnabled:(nullable NSString *)sessionID;
// ZOOM-484848
- (BOOL)isMeetChatMessageDBReadyBySessionId:(nullable NSString *)sessionId;

// ZOOM-504418
- (nullable NSString*)fetchMTSenderName:(nullable NSString*)sessionId messageId:(nonnull NSString*)messageId;
- (nullable NSString*)fetchMTReceiverName:(nullable NSString*)sessionId messageId:(nonnull NSString*)messageId;

- (BOOL)isMsgSenderMyselfInMeeting:(nonnull NSString *)sessionId
                         messageId:(nonnull NSString *)messageId;
- (BOOL)isMsgReceiverMyselfInMeeting:(nonnull NSString *)sessionId
                           messageId:(nonnull NSString *)messageId;

- (NSString *)getMainMeetingCardMsgId:(long)meetingNumber;

- (BOOL)isLocalStorageEnabledWithSessionId:(NSString *)sessionId;

- (nonnull NSString*)localStorageTimeStringWithSessionId:(nonnull NSString*)sessionId;

#if defined(__cplusplus)
- (nonnull NSString*)localStorageTimeStringWithRetentionType:(ns_zoom_messager::IZoomMessenger::LocalStorageRetentionType)type;

- (BOOL)localStorageLess30DayWithType:(ns_zoom_messager::IZoomMessenger::LocalStorageRetentionType)type;
#endif

- (long long)localStorageEraseTimeStampWithSessionId:(NSString*)sessionId;

- (void)updateBroadcastID;

- (nullable NSString *)getBroadcastID;

- (nullable NSArray<NSString *> *)getBroadcastIDList;

- (void)setLocalizedBroadcastName:(nonnull NSString *)name forChannel:(nonnull NSString *)channelID;

- (BOOL)isMessageAtMe:(nonnull NSString *)msgId sessionId:(nonnull NSString *)sessionId;
- (BOOL)isMessageAtEveryone:(nonnull NSString *)msgId sessionId:(nonnull NSString *)sessionId;
- (BOOL)isMessageMentionGroupAtMe:(nonnull NSString *)msgId sessionId:(nonnull NSString *)sessionId;
- (BOOL)isMessageNotExist:(nonnull NSString *)msgId sessionId:(nonnull NSString *)sessionId;

- (void)notifySummaryForSession:(NSString *)sessionId payloadJson:(NSString *)payload location:(ZMSummaryLocationType)location;

- (BOOL)isForwardMessageEnabledWithSession:(nullable NSString *)sessionId;

- (BOOL)isChatAppDisableAddEmojiInAppSession:(nullable NSString *)sessionId;
- (BOOL)isChatAppDisableReplyInAppSession:(nullable NSString *)sessionId;
- (BOOL)isChatAppOnlyDisplayCopyItemInAppSession:(nullable NSString *)sessionId;

- (void)fetchOriMsgInfoWithForwardedMsg:(ZMMessageAdapter *)forwardedMessage noteMsg:(ZMMessageAdapter *)noteMessage;

@end

@protocol ZMEmojiProtocol <NSObject>

- (BOOL)isEnableChatEmojiWithSessionId:(nullable NSString *)sessionId;
- (BOOL)isEnableSelectedChatEmojiWithSessionId:(NSString *)sessionId;
- (NSString *)code2SDKEmojiID:(NSString *)emojiID withType:(ZMEmojiType)emojiType;
- (NSString *)decodeSDKEmojiID:(NSString *)emojiID emojiType:(ZMEmojiType*)emojiType;

@end

@protocol ZMGiphyProtocol <NSObject>

- (nullable NSArray*)getCachedHotGiphyInfoList:(nullable NSString *)sessionId;

- (BOOL)getHotGiphyInfoWithNum:(NSUInteger)giphyNum sessionId:(nullable NSString *)sessionId requestID:(NSString *_Nullable *_Nullable)requestID;

- (BOOL)getGiphyInfoByStr:(nonnull NSString*)giphyString andGiphyNum:(NSUInteger)giphyNum sessionId:(nullable NSString *)sessionId  requestID:( NSString *_Nullable * _Nullable)requestID;

- (nullable NSDictionary *)getGiphyInfo:(nonnull NSString*)giphyID sessionId:(nullable NSString *)sessionId;

- (nullable NSString*)giphyImgPathByGiphyID:(nonnull NSString*)giphyID sessionId:(nullable NSString *)sessionId;

- (BOOL)downloadGIFFromGiphyByUrl:(nonnull NSDictionary*)downloadInfoDict sessionId:(nullable NSString *)sessionId requestID:(NSString *_Nullable *_Nullable)requestID andIsBigPic:(BOOL)isBigPic;

- (BOOL)cancelGiphyDownload:(nonnull NSString*)reqId sessionId:(nullable NSString *)sessionId;

- (BOOL)getGiphyInfoFromSever:(nonnull NSString*)giphyID sessionId:(nonnull NSString*)sessionId messageId:(nonnull NSString*)msgId;

- (BOOL)isGiphyOptionEnabled:(nullable NSString *)sessionId;

- (BOOL)isGiphyMixedMessageEnabled:(nullable NSString *)sessionId;

@end

@protocol ZMGiphyCallbackProtocol <NSObject>

- (void)didGotGiphyInfo:(nonnull id)param;

- (void)didGiphyDownloaded:(nonnull id)param;

@end

@protocol ZMStickerProtocol <NSObject>

- (BOOL)syncStickerList;

- (nullable NSArray <ZMStickerInfo *> *)getStickerList;

- (NSInteger)getStickersCont;

- (int)downloadSticker:(nonnull ZMStickerInfo *)sticker path:(nonnull NSString *)path reqId:(NSString *_Nullable *_Nullable)reqId;

- (int)makeStickerWithFileId:(nonnull NSString *)fileId;

- (int)uploadFileAndMakeSticker:(nonnull NSString *)filePath reqId:(NSString *_Nullable *_Nullable)reqId;

- (int)discardSticker:(nonnull ZMStickerInfo *)sticker;

@end

@protocol ZMLinkPreviewProtocol <NSObject>

- (BOOL)isHyperlinkPreviewEnabledInE2EWithSessionId:(nullable NSString *)sessionId;

- (BOOL)isEnableLinkPreview;

- (BOOL)checkNeedParseURLBySender:(nonnull NSString *)senderJid sessionId:(nonnull NSString*)sessionId;

- (nullable NSMutableDictionary *)fuzzyGetLinkMetaInfoWithURL:(nonnull NSString *)linkURL sessionId:(nonnull NSString *)sessionId;

- (void)sendLinkMetaInfoWithSessionID:(nonnull NSString *)sessionID messageID:(nonnull NSString *)messageID urls:(nonnull NSArray *)urls;

- (BOOL)crawlLinkMetaInfoWithSessionID:(nonnull NSString *)sessionID messageID:(nonnull NSString *)messageID urls:(nonnull NSArray *)urls;

@end

@protocol ZMOperateMessageCallback <NSObject>

- (void)didReceiveMessageById:(nonnull NSString *)messageId sessionId:(nonnull NSString *)sessionId;

- (void)didReceiveMessage:(nonnull ZMMessageAdapter *)message;

- (void)didSendComfirmMessageById:(nonnull NSString *)messageId sessionId:(nonnull NSString *)sessionId;

- (void)didRevokeMessage:(nonnull NSString *)messageId param:(nonnull ZMMessageRevokeInfo *)param;

- (void)didUpdateMessage:(nonnull NSString *)messageId sessionId:(nonnull NSString *)sessionId;

- (void)didEditMessage:(ZMEditMessageInfoResult *)editMessageResult;

- (void)didMarkUnreadMessagesUpdate:(nullable NSArray <NSString *> *)messageIds sessionId:(nullable NSString *)sessionId;

- (void)didMarkUnreadMessagesUpdateFromOtherPlatform:(nullable NSArray <NSString *> *)messageIds sessionId:(nullable NSString *)sessionId;

- (void)didReactionUpdatedWithChannelID:(nonnull NSString*)channelID msgID:(nonnull NSString*)msgID needSync:(BOOL)needSync;

- (void)didE2EMessageUpdateWithChannelID:(nonnull NSString*)channelID msgID:(nonnull NSString*)msgID;

- (void)didCMKMessageUpdateWithChannelID:(nonnull NSString *)channelID msgIDs:(nonnull NSArray<NSString *> *)msgIDs;

- (void)didSyncThreadCommentCountWithChannelID:(nonnull NSString*)channelID msgArray:(nonnull NSArray<NSString*>*)ids requestId:(nonnull NSString *)requestId success:(BOOL)success;

- (void)didPlayAudioMessageStatusChange:(nonnull NSString *)messageId sessionId:(nonnull NSString *)sessionId;

- (void)didReceivedUnsupportMessage:(nonnull NSString*)messageId sessionId:(nonnull NSString*)sessionId;

- (void)didConnectChange:(NSInteger)error;

- (void)PMC_NotifyCheckInTeamChatFromMeetingChatResult:(nonnull NSDictionary*)isBelongTo groupID:(nonnull NSString *)groupID;

#if defined(__cplusplus)
- (void)PMC_NotifyOpenTeamChatFromMeetingChatResult:(PMCOpenTeamChatRespResult)result;
#endif

- (void)didUnreadCountReset:(nonnull NSString *)sessionId isFromOtherPlatform:(BOOL)isFromOtherPlatform;

- (void)didReceiveInputStateChanged:(nonnull NSString *)sessionId state:(NSInteger)state;

- (void)didEditMessageBlockedByContact:(nonnull NSString *)sessionId;

- (void)didOutdatedHistoryRemoved:(nonnull NSArray <NSString *> *)sessionIDs;

- (void)didCommonAppSelectionChanged:(nonnull NSString *)messageId sessionId:(nonnull NSString *)sessionId dict:(nonnull NSDictionary *)dict;

- (void)didNotifyRemovedSessionForOutdatedMsgCheck:(nonnull NSString *)sessionID sessionType:(ZMOutdatedMsgCheckSessionType)sessionType;

- (void)didDelWhiteboardFromMessage:(NSString *  _Nonnull)sessionId msgId:(NSString *  _Nonnull)msgId wbLink:(NSString *  _Nonnull)wbLink;

- (void)didDeeplinkPreviewMessageUpdate:(NSString *  _Nonnull)sessionId deeplinkEntity:(id)deeplinkEntity;

@end

@protocol ZMOperateReactionProtocal <NSObject>

- (void)syncMessageEmojiCountInfoWithChanelID:(nonnull NSString*)channelID msgArray:(nonnull NSArray<NSString*>*)msgArray;

- (nullable NSArray *)getEmojiCountInfoWithChanelID:(nonnull NSString*)channelID messageID:(nonnull NSString*)msgID isLocalOnly:(BOOL)isLocalOnly xmsReqID:( NSString * _Nullable *_Nullable)outXmsReqID;

- (nullable NSArray *)getEmojiDetailInfoWithChanelID:(nonnull NSString*)channelID messageID:(nonnull NSString *)msgID emojiString:(nonnull NSString *)emojiString isLocalOnly:(BOOL)isLocalOnly xmsReqID:( NSString * _Nullable *_Nullable)outXmsReqID;

- (NSArray*)getEmojiDetailInfoWithChanelID:(NSString*)channelID messageID:(NSString*)msgID emojiModel:(ZMCustomEmojiModel *)emojiModel isLocalOnly:(BOOL)isLocalOnly xmsReqID:(NSString* _Nullable *_Nullable)outXmsReqID;

- (BOOL)addEmojiForChanelID:(nonnull NSString *)channelID messageID:(nonnull NSString *)msgID emoji:(nonnull NSString *)emoji xmsReqID:( NSString * _Nullable *_Nullable)outXmsReqID;

- (BOOL)removeEmojiForChanelID:(nonnull NSString *)channelID messageID:(nonnull NSString *)msgID emoji:(nonnull NSString *)emoji xmsReqID:( NSString * _Nullable *_Nullable)outXmsReqID;

- (BOOL)addEmojiForChanelID:(nonnull NSString *)channelID messageID:(nonnull NSString *)msgID emojiModel:(nonnull ZMCustomEmojiModel *)emojiModel xmsReqID:( NSString * _Nullable *_Nullable)outXmsReqID;

- (BOOL)removeEmojiForChanelID:(nonnull NSString *)channelID messageID:(nonnull NSString *)msgID emojiModel:(nonnull ZMCustomEmojiModel *)emojiModel xmsReqID:( NSString * _Nullable *_Nullable)outXmsReqID;

- (BOOL)checkSessionCanAddReaction:(nonnull NSString *)sessionId;

- (void)onFetchEmojiCountInfoWithSessionID:(nonnull NSString*)inSID messages:(nonnull NSArray<NSString*> *)msgArray reqID:(nonnull NSString *)reqID;

- (void)onFetchEmojiDetailInfoWithSessionID:(nonnull NSString *)inSID messageID:(nonnull NSString *)inMsgID emojiCode:(nonnull NSString *)emojiCode reqID:(nonnull NSString *)reqID;

@end

@protocol ZMListenMessageProtocal <NSObject>

- (void)removeAllListenByTarget:(nonnull id)target;

- (void)addListenMessageReceiveByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenMessageSendComfirmByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenSyncSingleThreadReceiveCommentsByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenMessageUpdateByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

/// Note:ZMCallbackBlock param will be (NSString *msgId, ZMEditMessageInfoResult *editMessageInfoResult, NSError *error)
- (void)addListenEditMessageByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenDeleteMessageByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenMarkUnreadMessagesUpdateByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenMarkUnreadMessagesUpdateFromOtherPlatformByTarget:(id)target completion:(ZMCallbackBlock)completion;

- (void)addListenReactionUpdateByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenReactionDetailUpdateByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenE2EMessageUpdateByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenCMKMessageUpdateByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenInputStateUpdateByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

//pin message
//completion param,key is sessionId,param is ZMPinMessageCallback

- (void)addListenTopPinMessageByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenUnTopPinMessageByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenHideTopPinMessageByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenTopPinMessageUpdateByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenRemovePinMessageByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

//meeting chat card
- (void)addListenMCCDetailMessagesSyncByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenMCCMessagesAllSyncDone:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenMCCParticipantsSyncByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;
// the meeting card receives the message from sub cmc
- (void)addListenMoveMessageWithNewReplyToBottomByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

//schedule channel meeting

- (void)addListenReceiveChannelMeetingMemberChangedByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenReceiveChannelMeetingListByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenReceiveChannelMeetingStatusChangedByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenReceiveSearchMeetingAttendeesByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenMeetingAssetsSharingResultByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

// sip voicemail

- (void)addListenReceiveVoicemailAddedByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;
- (void)addListenReceiveVoicemailEnableChanged:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;
- (void)addListenReceiveVoicemailActionChanged:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

// spot channel

- (void)addListenReceiveChannelSpotMeetingMemberChangedByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenReceiveChannelSpotParticipantChangedByTarget:(id)target completion:(ZMCallbackBlock)completion;

- (void)addListenReceiveChannelSpotPresenceChangedByTarget:(id)target completion:(ZMCallbackBlock)completion;

- (void)addListenReceiveChannelSpotReminderListChangedByTarget:(id)target completion:(ZMCallbackBlock)completion;

- (void)addListenReceiveChannelSpotMeetingListByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenReceiveChannelSpotStatusUpdateByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenReceivePublicChannelHuddleCreateByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

// persistent meetings chat

- (void)addListenReceivePMCMeetingInfoChangedByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

//reminder message

- (void)addListenSetReminderByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenRemoveReminderByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenReminderExpireByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenReminderSyncByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenReminderSyncRequiredByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenReminderUnreadCountByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

//audio
- (void)addListenPlayAudioStatusByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenUnreadCountReadyByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenGotGiphyInfoByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenGihpyDownloadReadyByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenUnreadCountResetByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

// chat App
- (void)addListenChatAppMessagePreviewSyncFromWebviewByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenOpenChatAppByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;
- (void)addListenChatAppShowAppListByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

// zoom links formatting
- (void)addListenZoomLinksMatchedByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenPMCCheckInTeamChatFromMeetingChatResultByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;
- (void)addListenPMCNotifyOpenTeamChatFromMeetingChatResultByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;
- (void)addListenPMC_NotifyTeamChatUpdatedByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenConnectStatusChangedByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenEditMessageBlockedByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenOutdatedHistoryRemovedByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenCommonAppSelectionChangedByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenNotifyRemovedSessionForOutdatedMsgCheck:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

// smart schedule meeting intent
- (void)addListenReceiveSmartIntentByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenReceiveChatRestrictExternalByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenReceiveChannelOwnerSpecialSettingByTarget:(id)target completion:(nonnull ZMCallbackBlock)completion;

// backward compatibility
- (void)addListenBackwordCompatibilityInfoReadyByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;

- (void)addListenDeeplinkInfoUpdateByTarget:(nonnull id)target completion:(nonnull ZMCallbackBlock)completion;
@end


@protocol ZMSendMessageProtocal <NSObject>

- (InputBoxMsgType)boxMsgTypeWithSession:(ZMSendMessageSessionInfo*)sessionInfo trimmedString:(nonnull NSString *)string fileList:(nullable NSArray<ZMFileEntity *> *)fileList;

#if defined(__cplusplus)
- (nullable NSString *)shareTextMessage:(nonnull NSString *)text messageType:(MessageType)type toSessionId:(nonnull NSString *)toSessionId fromSessionId:(nonnull NSString *)fromSessionId fromMessageId:(nonnull NSString *)fromMessageId;
#endif

// NOTE: the following declarations without ZMSendMessageParams are deprecated. New code should not invoke
//       these; they are only kept for backwards compatibility purposes, and may be removed in the future.

/// START DEPRECATED SELECTORS
- (nullable NSString*)sendSharepointIntegration:(nonnull ZMFileEntity *)fileInfo sessionId:(nonnull NSString *)sessionId threadId:(nullable NSString *)threadId API_DEPRECATED_WITH_REPLACEMENT("-sendSharepointIntegration:sessionId:threadId:params:", macos(10.10, 13.4));

- (nullable NSString *)sendLocalFileIntegrationMessage:(nonnull NSString *)filePath sessionId:(nonnull NSString *)sessionId threadId:(nullable NSString *)threadId isFromScreenShot:(BOOL)isFromScreenShot API_DEPRECATED_WITH_REPLACEMENT("-sendLocalFileIntegrationMessage:sessionId:threadId:isFromScreenShot:params:", macos(10.10, 13.4));

- (nullable NSString *)sendFileMessage:(nonnull NSString *)filePath sessionId:(nonnull NSString *)sessionId threadId:(nullable NSString *)threadId isFromScreenShot:(BOOL)isFromScreenShot API_DEPRECATED_WITH_REPLACEMENT("-sendFileMessage:sessionId:threadId:isFromScreenShot:params:", macos(10.10, 13.4));

- (nullable NSString *)sendVideoMessage:(nonnull NSString *)videoPath previewPath:(nonnull NSString *)previewPath previewSize:(NSSize)size session:(nonnull NSString *)sessionId threadId:(nullable NSString *)threadId API_DEPRECATED_WITH_REPLACEMENT("-sendVideoMessage:previewPath:previewSize:session:threadId:params:", macos(10.10, 13.4));

- (nullable NSString *)sendCodeSnippetMessage:(nonnull NSString *)filePath sessionId:(nonnull NSString *)sessionId threadId:(nullable NSString *)threadId API_DEPRECATED_WITH_REPLACEMENT("-sendCodeSnippetMessage:sessionId:threadId:params:", macos(10.10, 13.4));

- (nullable NSString *)sendFileIntegrationMessage:(nonnull ZMFileIntegrationShareInfo *)fileInfo sessionId:(nonnull NSString *)sessionId threadId:(nullable NSString *)threadId API_DEPRECATED_WITH_REPLACEMENT("-sendFileIntegrationMessage:sessionId:threadId:params:", macos(10.10, 13.4));

- (nullable NSString *)sendAudioFile:(nonnull NSString*)audioFilePath sessionId:(nonnull NSString*)sessionId threadId:(nullable NSString*)threadId audioLength:(NSUInteger)audioLenInSec API_DEPRECATED_WITH_REPLACEMENT("-sendAudioFile:sessionId:threadId:audioLength:params:", macos(10.10, 13.4));

- (NSString * _Nonnull)sendEmail:(ZMShareEmailInfo * _Nonnull)emailInfo text:(NSAttributedString * _Nonnull)text file:(NSString * _Nonnull)file sessionId:(NSString* _Nonnull)sessionId API_DEPRECATED_WITH_REPLACEMENT("-sendEmail:text:file:sessionId:params:", macos(10.10, 13.4));
/// END DEPRECATED SELECTORS

- (BOOL)sendCustomEmojiMessage:(nonnull ZMStickerInfo *)customEmoji sessionId:(nonnull NSString *)sessionId threadId:(nullable NSString *)threadId;

- (nullable NSString *)sendGiphyMessage:(nonnull NSString *)giphyID sessionId:(nonnull NSString *)sessionId threadId:(nullable NSString *)threadId params:(nullable ZMSendMessageParams *)param;

- (nullable NSString*)editCodeSnippetMsgWithFileId:(nullable NSString*)webFileId andFileName:(nullable NSString*)fileName andLocalPath:(nullable NSString*)localPath andIsOnlyNameChanged:(BOOL)isOnlyNameChanged sessionId:(nullable NSString*)inSessionId;


- (nullable NSArray <NSString *> *)sendMessageWithParam:(nullable ZMSendMessageParams *)param;

- (nullable NSString*)sendSharepointIntegration:(nonnull ZMFileEntity *)fileInfo
                                      sessionId:(nonnull NSString *)sessionId
                                       threadId:(nullable NSString *)threadId
                                         params:(nullable ZMSendMessageParams *)param;

- (nullable NSString *)sendLocalFileIntegrationMessage:(nonnull NSString *)filePath
                                             sessionId:(nonnull NSString *)sessionId
                                              threadId:(nullable NSString *)threadId
                                      isFromScreenShot:(BOOL)isFromScreenShot
                                                params:(nullable ZMSendMessageParams *)param;

- (NSString * _Nonnull)sendLink:(NSString * _Nonnull)link
                           text:(nullable NSAttributedString *)attrStr
                      sessionId:(NSString *_Nonnull)sessionId;

- (nullable NSString *)sendFile:(nonnull ZMFileEntity *)fileEntity
                      sessionId:(nonnull NSString *)sessionId
                       threadId:(nullable NSString *)threadId
                         params:(nullable ZMSendMessageParams *)param;

- (nullable NSString *)sendFileMessage:(nonnull NSString *)filePath
                             sessionId:(nonnull NSString *)sessionId
                              threadId:(nullable NSString *)threadId
                      isFromScreenShot:(BOOL)isFromScreenShot
                                params:(nullable ZMSendMessageParams *)param;

- (nullable NSString *)sendVideoMessage:(nonnull NSString *)videoPath
                            previewPath:(nonnull NSString *)previewPath
                            previewSize:(NSSize)size
                                session:(nonnull NSString *)sessionId
                               threadId:(nullable NSString *)threadId
                                 params:(nullable ZMSendMessageParams *)param;

- (nullable NSString *)sendCodeSnippetMessage:(nonnull NSString *)filePath
                                    sessionId:(nonnull NSString *)sessionId
                                     threadId:(nullable NSString *)threadId
                                       params:(nullable ZMSendMessageParams *)param;

- (nullable NSString *)sendFileIntegrationMessage:(nonnull ZMFileIntegrationShareInfo *)fileInfo
                                        sessionId:(nonnull NSString *)sessionId
                                         threadId:(nullable NSString *)threadId
                                           params:(nullable ZMSendMessageParams *)param;

- (nullable NSString *)sendAudioFile:(nonnull NSString*)audioFilePath
                           sessionId:(nonnull NSString*)sessionId
                            threadId:(nullable NSString*)threadId
                         audioLength:(NSUInteger)audioLenInSec
                              params:(nullable ZMSendMessageParams *)param;

- (NSString * _Nonnull)sendEmail:(ZMShareEmailInfo * _Nonnull)emailInfo
                            text:(NSAttributedString * _Nonnull)text
                            file:(NSString * _Nonnull)file
                       sessionId:(NSString* _Nonnull)sessionId
                          params:(nullable ZMSendMessageParams *)param;

- (nullable NSString *)sendTextMessage:(nonnull NSString *)text
                             sessionId:(nonnull NSString *)sessionId
                              threadId:(nullable NSString *)threadId
                                atList:(nullable NSArray*)atList
                             extension:(nullable void*)ext
                         fontStyleInfo:(nullable NSArray*)fontStyleArray
                              fileList:(nullable NSArray *)fileList
                                params:(nullable ZMSendMessageParams *)param;

- (nullable NSString *)sendVideoCallDeclineMessage:(nonnull NSString *)text sessionId:(nonnull NSString *)sessionId meetingId:(nonnull NSString *)meetingId;

- (nullable NSString *)shareTextMessage:(nonnull ZMShareMessageInfo *)shareMessageInfo;

- (nullable NSString *)sendMessageToMultipleSession:(NSArray *)sessions params:(nullable ZMSendMessageParams *)param;

- (nullable NSString *)shareFile:(ZMFileEntity *)file withNoteMessage:(NSString *)noteMessage fontStyleList:(NSArray *)fontStyles atList:(NSArray *)atList toSession:(NSString *)sessionId;

@end

@protocol ZMOperatePinMessageProtocal <NSObject>

- (nullable ZMPinMessageInfo *)getTopPinInfo:(nonnull NSString *)sessionId;

- (nullable ZMMessageAdapter *)getTopPinMessage:(nonnull NSString *)sessionId;

- (BOOL)isDisablePinMessage;

- (BOOL)checkIfNeedRefreshTopPinMessage:(nonnull NSString *)sessionId;

- (BOOL)topPinMessage:(long long)msgSvrTime sessionId:(nonnull NSString *)sessionId requestId:( NSString * _Nullable *_Nullable)requestId;

- (BOOL)unTopPinMessage:(long long)msgSvrTime sessionId:(nonnull NSString *)sessionId requestId:( NSString * _Nullable *_Nullable)requestId;

- (BOOL)removePinMessage:(long long)msgSvrTime sessionId:(nonnull NSString *)sessionId requestId:( NSString * _Nullable *_Nullable)requestId;

- (BOOL)hiddenTipPinMessage:(nonnull NSString *)sessionId;

- (BOOL)isNeedHideTopPinMessage:(nonnull NSString *)sessionId;

- (BOOL)isNeedHidePinPoppedTips;

- (void)setHidePinPopedTips;

- (BOOL)syncTopPinMessages:(nonnull NSArray <NSString *> *)sessionIds requestId:( NSString * _Nullable *_Nullable)requestId completion:(nullable ZMCallbackBlock)completion;

- (nullable NSArray <ZMPinMessageInfo *> *)queryPinMessageHistory:(nonnull NSString *)sessionId startMsgTime:(long long)startMsgTime count:(NSInteger)count requestId:( NSString * _Nullable *_Nullable)requestId hasMore:(BOOL *_Nullable)hasMore error:(NSError *_Nullable *_Nullable)error completion:(nullable ZMCallbackBlock)completion;

- (void)cancelPinOperateByRequestId:(nonnull NSString *)requestId;

@end

@protocol ZMOperatePinMessageCallbackProtocal <NSObject>

- (void)didTopPinMessage:(nonnull ZMPinMessageCallback *)callback;

- (void)didUnTopPinMessage:(nonnull ZMPinMessageCallback *)callback;

- (void)didRemovePinMessage:(nonnull ZMPinMessageCallback *)callback;

- (void)didSyncTopPinMessage:(nonnull ZMPinMessageCallback *)callback;

- (void)didQueryPinMessageHistory:(nonnull ZMPinMessageCallback *)callback;

@end

@protocol ZMScheduleChannelMeetingProtocol <NSObject>

- (void)didReceiveChannelMeetingList:(ZMScheduleMeetingCallback * _Nonnull)callback;
- (void)didReceiveMeetingMemberChanged:(ZMChannelMeetingEvent * _Nonnull)channelMeetingEvent;
- (void)didReceiveMeetingStatusChanged:(ZMChannelMeetingStatus * _Nonnull)channelMeetingStatus;
- (void)didReceiveSearchMeetingAttendees:(ZMChannelMeetingAttendeeEvent *_Nonnull)attendeeEvent;
- (void)didReceiveMeetingAssetsSharingResult:(ZMMeetingAssetsSharingResult * _Nonnull)sharingResult;
- (void)showAppList;

@end

@protocol ZMSipVoiceMailProtocol <NSObject>

- (void)didReceiveVoicemailAdded:(nonnull NSString *)callId refreshAll:(BOOL)refreshAll;
- (void)didReceiveVoicemailEnableChanged:(BOOL)enable;
- (void)didReceiveVoicemailActionChanged:(nonnull ZMVoicemailStatusChangedInfo *)info;

@end

@protocol ZMSpotChannelProtocol <NSObject>

- (void)didReceiveSpotMeetingMemberChanged:(ZMChannelMeetingEvent * _Nonnull)channelMeetingEvent;
- (void)didReceiveSpotParticipantChanged:(ZMChannelMeetingEvent * _Nonnull)channelMeetingEvent;
- (void)didReceiveSpotPresenceChanged:(NSArray <NSString *> * _Nonnull)channels;
- (void)didReceiveSpotReminderListChanged:(NSArray <NSString *> * _Nonnull)channels;
- (void)didReceiveSpotActiveMemberList:(ZMChannelSpotMeetingCallBack * _Nonnull)spotMeetingCallback;
- (void)didReceiveSpotStatusUpdate:(NSString *_Nonnull)channelId;
- (void)didReceivePublicChannelHuddleCreate:(NSString *_Nonnull)spotId requestId:(NSString *_Nullable)reqID;

@end

@protocol ZMPersistentMeetingsChatProtocol <NSObject>

- (void)didReceivePMCMeetingInfoChanged:(nonnull NSString *)groupId;

@end

@protocol ZMMessageSmartIntentProtocol <NSObject>

- (void)didReceiveMessageSmartIntent:(nonnull ZMSmartSchedulerRspInfo *)rspInfo sessionId:(nonnull NSString *)sessionId;

@end

@protocol ZMMessageMeetingCardSubCMCProtocol <NSObject>

- (void)didReceiveMeetingCardSubCMCMessage:(nonnull NSString *)sessionId cardThreadId:(nonnull NSString *)threadId msgSSTime:(long long)msgSSTime;

@end

@protocol ZMMessageBackwardCompatibilityProtocol <NSObject>

- (void)didBackwardCompatibilityInfoReady;

@end

@protocol ZMBotProtocol<NSObject>

- (BOOL)chatAppsGetBotsList:(nonnull NSString *)key req:(NSString *_Nullable*_Nullable)reqID;
- (BOOL)chatAppsAddBotsToChannel:(nonnull NSArray<NSString*>*)jids channel:(nonnull NSString*)channelID req:(NSString *_Nullable*_Nullable)reqID;
- (BOOL)chatAppsRemoveBotsFromChannel:(nonnull NSArray<NSString*>*)jids channel:(nonnull NSString*)channelID req:(NSString *_Nullable*_Nullable)reqID;

@end

@protocol ZMIMSinkProtocol <NSObject>

- (void)zmIMMessageHelper_registerMeetingUISink;
- (void)zmIMMessageHelper_unregisterMeetingUISink;

- (void)zmIMMessageHelper_registerPhoneUISink;
- (void)zmIMMessageHelper_unregisterPhoneUISink;

- (void)zmIMMessageHelper_registerPreMeetingUISink;
- (void)zmIMMessageHelper_unregisterPreMeetingUISink;

- (void)registerPTWebserviceSink;
- (void)unregisterPTWebserviceSink;

- (void)zmIMMessageHelper_registerSipVoiceMailChatIntegrationSink;


@end

@protocol ZMMeetingChatCardMessageProtocal <NSObject>

- (BOOL)syncMCCMessageWithPara:(nonnull ZMMeetingChatCardEntity*)para needsLastvalue:(BOOL)needsLastvalue;

- (void)didSyncMCCMessages:(nonnull ZMMCCSyncMessageResult*)syncResult;

- (nullable NSArray<ZMMessageAdapter*>*)getMCCLocalMessagesWithSessionID:(nonnull NSString *)sessionID messageID:(nonnull NSString *)messageID;

- (BOOL)syncMCCParticipantWithPara:(nonnull ZMMeetingChatCardReqParticipantsPara*)para;

- (nullable NSArray<ZMBuddyAdapter*>*)getMCCLocalParticipantsWithSessionID:(nonnull NSString *)sessionID messageID:(nonnull NSString *)messageID;

- (void)didSyncMCCParticipants:(nonnull ZMMCCSyncParticipantsResult*)syncResult;

- (nullable NSArray<ZMBuddyAdapter*>*)syncMCCLocalSearchParticipantWithSessionID:(nonnull NSString* )sessionID messageID:(nonnull NSString *)messageID key:(nonnull NSString *)key;

@end

@protocol ZMReminderMessageProtocol<NSObject>

- (BOOL)setReminderMessage:(ZMMessageAdapter* _Nonnull)message timeout:(NSInteger)timeout note:(NSString* _Nullable)note
                   context:(ZMReminderMessageContext)context;

- (BOOL)setReminderMessage:(ZMMessageAdapter* _Nonnull)message timeout:(NSInteger)timeout note:(NSString* _Nullable)note
                   context:(ZMReminderMessageContext)context
                completion:(ZMCallbackBlock _Nonnull)block;

- (BOOL)removeReminderMessage:(ZMMessageAdapter* _Nonnull)message
                      context:(ZMReminderMessageContext)context;

- (BOOL)removeReminderMessage:(ZMMessageAdapter* _Nonnull)message
                      context:(ZMReminderMessageContext)context
                   completion:(ZMCallbackBlock _Nonnull)block;

- (BOOL)removeReminderMessageByServerTime:(long long)serverTime session:(NSString* _Nonnull)sessionId
                                  context:(ZMReminderMessageContext)context;

- (BOOL)removeReminderMessageByServerTime:(long long)serverTime session:(NSString* _Nonnull)sessionId
                                  context:(ZMReminderMessageContext)context
                               completion:(ZMCallbackBlock _Nonnull)block;

- (NSString* _Nullable)syncReminderMessages;
- (NSString* _Nullable)syncReminderMessages:(ZMCallbackBlock _Nonnull)block;
/// True if the SDK has accepted a reminder sync request. False if no sync requests have been made/accepted.
/// @note True doesn't mean the request has completed, only that it was sent to the server.
@property (nonatomic, readonly, assign) BOOL hasHadSuccessfulReminderSyncRequest;

- (BOOL)isReminderMessageEnabled;
- (BOOL)isReminderMessage:(ZMMessageAdapter* _Nonnull)message;

- (BOOL)reminderizableByServerTime:(long long)message sessionId:(NSString* _Nonnull)sessionId;

- (NSInteger)reminderMessageUnreadCount;
- (NSInteger)reminderMessageUnreadCountReset:(long long)serverTime;
- (NSInteger)reminderMessageCount;

- (NSArray* _Nonnull)reminderMessageArray;

- (NSInteger)reminderMessageExpiryTimeoutMin;
- (NSInteger)reminderMessageExpiryTimeoutMax;
- (NSInteger)reminderMessageCountMax;
- (NSInteger)reminderMessageNoteLengthMax;

- (NSArray<NSString*> * _Nonnull)reminderMessageBodyByServerTime:(long long)serverTime sessionId:(NSString* _Nonnull)sessionId;

#if defined(__cplusplus)
- (ZMReminderMessageStatus)reminderStatusForCode:(NS_ZOOM_MESSAGER::ReminderMessageStatus)status;
- (ZMReminderMessageEvent)reminderEventForCode:(NS_ZOOM_MESSAGER::ReminderEvent)event;
#endif

@end

@protocol ZMGradientLabelProtocol<NSObject>

- (NSString* _Nonnull)gradientLabelInitializeKey;
- (NSString* _Nonnull)gradientLabelClickedKey;

- (BOOL)setGradientLabelTimestamp:(NSString* _Nonnull)key;
- (NSInteger)getGradientLabelTimestamp:(NSString* _Nonnull)key;

@end

@protocol ZMReminderMessageCallbackProtocol <NSObject>

- (void)didSetReminderMessage:(ZMReminderMessageInfo * _Nonnull)reminder;
- (void)didRemoveReminderMessage:(ZMReminderMessageInfo * _Nonnull)reminder;
- (void)didExpireReminderMessage:(ZMReminderMessageInfo * _Nonnull)reminder;
- (void)didSyncReminderMessages:(NSString * _Nonnull)requestId;
- (void)didUpdateReminderUnreadCount:(NSInteger)count;
- (void)needsReminderMessagesSync;

@end

@protocol ZMChatRestrictExternalUserProtocol <NSObject>

- (void)didReceiveChatRestrictExternalUserSetting:(nonnull ZMAddExternalUserInfo *)userInfo;
/// Gets the web external settings for the specified user's account.
/// `block` is called with `nil` if the settings can't be fetched.
/// Calls `block` synchronously if no web request is needed, asynchronously if a web request is needed.
- (void)fetchWebExternalSettingsByOwner:(NSString*)owner block:(void(^)(ZMAccountAdminWhoInviteExternalToGroupSettingInfo* _Nullable))block;
@end

@protocol ZMChatMessengerGroupPropertyUIProtocol <NSObject>

- (void)didReceiveChannelOwnerSpecialSetting:(nonnull ZMChannelOwnerSpecialSettingInfo *)settingInfo;

@end

@protocol ZMChatAppProtocol

- (nullable NSArray<ZMChatAppInfo*>*)getChatAppComposeShortcuts;
- (nullable NSArray<ZMChatAppInfo*>*)getChatAppComposeShortcutsWithSessionId:(NSString *_Nullable)sessionId;
- (nullable NSArray<ZMChatAppInfo*>*)getChatAppThreadShortcutsWithSessionId:(NSString *_Nullable)sessionId;
- (nullable NSArray<ZMChatAppShortcutInfo*>*)getChatMessageShortcuts;
- (nullable NSArray<ZMChatAppShortcutInfo*>*)getChatMessageShortcutsWithSessionId:(NSString *_Nullable)sessionId;
- (nullable NSArray<ZMChatAppShortcutInfo*>*)getChatMessageShortcutsInSystemGroup:(BOOL)systemGroup;
- (nullable NSArray<ZMChatAppShortcutInfo*>*)getChatMessageShortcutsWithSessionId:(NSString *_Nullable)sessionId systemGroup:(BOOL)systemGroup;

- (nullable NSArray<ZMChatAppInfo*> *)getChatAppShortcutsByKey:(NSString *)key;

- (nullable NSArray<ZMChatAppInfo*>*)getChatSessionShortcuts;

- (nullable NSArray<ZMChatAppShortcutInfo*>*)getChatAppFormatShortcutsWithSessionId:(NSString *_Nullable)sessionId;

- (void)setChatAppContextWithSessionID:(NSString * _Nullable)sessionID threadID:(NSString * _Nullable)threadID;

//call back from webView
- (void)didReceiveChatAppMessagePreviewInfoFromWebView:(nonnull ZMChatAppMessagePreviewInfo*)syncResult;
- (void)openChatApp:(nonnull ZMChatAppContext *)context;
- (void)showAppList;
- (BOOL)sendShortcutCommand:(ZMChatAppContext *)context;
- (void)clearWebhookCallbackID;
@end

@protocol ZMInputboxSyncInfoProtocol

- (void)didFetchLinkInfoResult:(ZMInputBoxLinkInfoSyncedResult*)linkInfo;

@end

@protocol ZMMessageProtocol <ZMSyncMessageProtocal,ZMSyncMessageCallback,ZMCheckMessageProtocal,ZMOperateMessageProtocol,ZMOperateMessageCallback,ZMOperateReactionProtocal,ZMListenMessageProtocal,ZMSendMessageProtocal,ZMOperatePinMessageProtocal,ZMOperatePinMessageCallbackProtocal,ZMSyncSessionUnreadCallback,ZMEmojiProtocol,ZMGiphyProtocol,ZMGiphyCallbackProtocol,ZMStickerProtocol,ZMLinkPreviewProtocol,ZMBotProtocol,ZMScheduleChannelMeetingProtocol,ZMReminderMessageProtocol,ZMReminderMessageCallbackProtocol,ZMGradientLabelProtocol,ZMMeetingChatCardMessageProtocal,ZMChatAppProtocol,ZMPersistentMeetingsChatProtocol,ZMSpotChannelProtocol,ZMIMInputCustomButtonMgrProtocol,ZMMessageSmartIntentProtocol,ZMChatRestrictExternalUserProtocol,ZMMessageBackwardCompatibilityProtocol,ZMInputboxSyncInfoProtocol,ZMChatMessengerGroupPropertyUIProtocol, ZMMessageMeetingCardSubCMCProtocol,ZMSipVoiceMailProtocol>
@end

#ifndef ZMShareMessageProtocol
#define ZMShareMessageProtocol (id <ZMMessageProtocol>)ZMSharedFor(ZMMessageProtocol)
#endif

NS_ASSUME_NONNULL_END

#endif /* ZMMessageProtocol_h */

