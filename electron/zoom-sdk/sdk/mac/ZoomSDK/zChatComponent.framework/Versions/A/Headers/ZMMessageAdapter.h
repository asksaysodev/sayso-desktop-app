//
//  ZMMessageAdapter.h
//  Zoom
//
//  Created by Deal Shao on 9/25/13.
//  Copyright (c) 2013 Zoom Video Communications, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <ZoomKit/zoom_client_conf.h>
#import <zChatComponent/ZMIMCommonDefine.h>
#import <ZoomUnit/ZPZappAdapter.h>
#import <zChatComponent/ZMMessageFontInfo.h>
#import <zChatComponent/ZMMessageMentionInfo.h>

#if defined(__cplusplus)
#import <zPTApp/SaasBeePTAppInterface.h>
#import <zChatApp/IZoomMessenger.h>
#endif

@class ZMPinMessageInfo;
@class ZMFileEntity;
@class ZMChatFileItem;
@class ZMScheduleMeetingInfo;
@class ZMShareEmailInfo;
@class ZMReminderMessageInfo;
@class ZMMTBuddyModel;
@class ZMMeetingChatCardEntity;
@class ZMChannelInviteRequestEntity;
@class ZMDeepLinkEntity;
@class ZMSharedSpaceOpenChannelInfo;
@class ZMUserSignatureData;

typedef NS_ENUM(NSUInteger, ZMZAppToastViewType);

typedef NS_ENUM(NSUInteger, ZMDisplayMsgTypeCheckResult) {
    ZMDisplayMsgTypeCheckResult_None = 0,
    ZMDisplayMsgTypeCheckResult_Checked = 1,
    ZMDisplayMsgTypeCheckResult_DisabledFileFromOthers = 2,
};


typedef NS_ENUM(NSUInteger, ZMMessageAdapterSenderType)
{
    ZMMessageAdapterSenderType_unknow = 0,
    ZMMessageAdapterSenderType_me,
    ZMMessageAdapterSenderType_other,
};

typedef NS_ENUM(NSUInteger, ZMCloudStoreState)
{
    ZMCloudStoreState_unknow = 0,
    ZMCloudStoreState_stored,
    ZMCloudStoreState_NotStored,
};

typedef NS_ENUM(NSUInteger, ZMThreadHasCommentType)
{
    ZMThreadHasCommentType_unknow = 0,
    ZMThreadHasCommentType_has,
    ZMThreadHasCommentType_no,
};

typedef NS_ENUM(NSUInteger, ZMPMCUnsupportMessageType)
{
    ZMPMCUnsupportMessageType_None, //supported message
    ZMPMCUnsupportMessageType_MeetChatToTeamChat, // message from Meet chat to Team chat，only support text message.
    ZMPMCUnsupportMessageType_TeamChatToMeetChat, // message from Team chat to Meet chat，only support text message.
    ZMPMCUnsupportMessageType_MeetChatToMeetChat, //message from Meet chat to Meet char，only support MessageType_JPG，MessageType_PNG，MessageType_GIF，MessageType_OtherFile，MessageType_ZappMessage，MessageType_MeetFileIntegration
};

typedef NS_ENUM(NSUInteger, ZMMarkUnreadMessageMentionType) {
    ZMMarkUnreadMessageMentionTypeNone,
    ZMMarkUnreadMessageMentionTypeMentionMe,
    ZMMarkUnreadMessageMentionTypeMentionGroup,
    ZMMarkUnreadMessageMentionTypeBoth
};

#if defined(__cplusplus)
typedef MessageState_CMKState ZMMessageCMKStatus;
typedef MessageErrorCode_CMK ZMMessageCMKErrorCode;

@interface ZMMessageBackwardCompatibilityInfo : NSObject

@property (nonatomic,copy,nullable) NSString *content;
@property (nonatomic,copy,nullable) NSString *linkText;
@property (nonatomic,copy,nullable) NSString *link;
@property (nonatomic,assign)        ns_zoom_messager::BCLinkType linkType;
@end
#endif

@interface ZMOriginalMessageInfo : NSObject

@property (nonatomic, copy, nullable) NSString *sessionId;
@property (nonatomic, copy, nullable) NSString *msgId;
@property (nonatomic, assign) NSInteger timestamp;

@property (nonatomic, assign) BOOL showAttribute;
@property (nonatomic, assign) BOOL isGroup;

@property (nonatomic, assign) BOOL isFromPublicChannel;

@end

@interface ZMMarkUnreadMessageMentionInfo : NSObject
@property (nonatomic, assign) NSInteger markUnreadCount;
@property (nonatomic, assign) NSInteger markUnreadMentionCount;
@property (nonatomic, assign) ZMMarkUnreadMessageMentionType mentionType;

@end

@interface ZMMessageAdapter : NSObject

/**message ids**/
@property (nonatomic,copy,nonnull)      NSString *m_msgID;

@property (nonatomic,copy,nullable)     NSString *serverGuid;

@property (nonatomic,copy,nullable)     NSString * m_XMPPGuid;

@property (nonatomic,copy,nullable)     NSString *m_msgBody;

@property (nonatomic,copy,nullable)     NSString *sessionID;

@property (nonatomic,copy,nullable)     NSString *m_senderID;

@property (nonatomic,copy,nullable)     NSString *m_senderName;

@property (nonatomic,copy,nullable)     NSString *m_receiverID;

@property (nonatomic,copy,nullable)     NSString *threadID;

@property (nonatomic,copy,nullable)     NSString *m_giphyID;

@property (nonatomic,copy,nullable)     NSString *deleteThreadOperator;

/**message times**/

@property (nonatomic,assign)            unsigned long long threadServerTime;

@property (nonatomic,assign)            unsigned long long svrSideTime;

@property (nonatomic,assign)            unsigned long long prevMsgSvrSideTime;

@property (nonatomic,assign)            unsigned long long lastCommentTime;

@property (nonatomic,assign)            unsigned long long lastEmojiTime;

@property (nonatomic,assign)            unsigned long long internalTimeValue;

//deprecated
//@property (nonatomic,readonly)          unsigned long long meetingMessageTime;

@property (nonatomic,retain,nullable)   NSDate *m_timeStamp;

@property (nonatomic,retain,nullable)   NSDate *editTime;

/**message type**/

#if defined(__cplusplus)
@property (nonatomic,assign)            MessageType       m_msgType;
#endif

@property (nonatomic,assign)            ZMFileStorageType fileStorageType;

@property (nonatomic,readonly)          ZMPMCUnsupportMessageType PMCUnsupportMessageType;

@property (nonatomic,assign)            NSInteger filterType;

@property (nonatomic,assign)            BOOL isStickerMessage;

@property (nonatomic,assign)            BOOL isTempMessage;

@property (nonatomic,assign)            BOOL is3rdFileStorageMsg;

@property (nonatomic,assign)            BOOL isPMCSystemMsg;

@property (nonatomic,assign)            BOOL isSharedMessage;

@property (nonatomic, assign)           BOOL isForwardedMessage;

@property (nonatomic, assign)           BOOL isNoteWithForwardMsg;

@property (nonatomic,assign)            BOOL isSentByZoomRoom;

@property (nonatomic,readonly)          BOOL isMyNoteMessage;

// deprecated
//@property (nonatomic,readonly)          BOOL isMeetingMessage;
//@property (nonatomic,readonly)          BOOL isMeetingMessagePostByHost;

@property (nonatomic,readonly)          BOOL isSystemMessage;

@property (nonatomic,assign,readonly)   BOOL isPhoneCallMsgHasVoiceMail;

@property (nonatomic,assign)            BOOL isMCCMessage; // ZOOM-377225 Flag, meeting chat card message

@property (nonatomic,readonly)          BOOL isMeetChatAceTrans;

@property (nonatomic,assign)            BOOL isCurrentMeetingMsg;

@property (nonatomic,readonly)          BOOL isPhoneCallAction;
@property (nonatomic,readonly,nullable) NSString *phoneCallID;

/**message status**/

#if defined(__cplusplus)
@property (nonatomic,assign)            MessageState m_msgState;
#endif

@property (nonatomic,assign)            ZMCloudStoreState msgCloudStoreState;

@property (nonatomic,assign)            ZMCloudStoreState commentThreadCloudStoreState;

//e2e
@property (nonatomic,assign)            BOOL isE2EMessage;

@property (nonatomic,assign)            BOOL m_isPending;

@property (nonatomic,assign)            BOOL m_isSendingFailed;

@property (nonatomic,assign)            BOOL m_isHistorySyncMessage;

@property (nonatomic,assign)            BOOL m_isUnread;

@property (nonatomic,readonly)          BOOL isUnread;

@property (nonatomic,assign)            BOOL m_isOffline;

@property (nonatomic,assign)            BOOL isDeletedThread;

@property (nonatomic,assign)            BOOL isNotExistThread;

@property (nonatomic,assign)            BOOL isDeletedByChannelAdmin;

@property (nonatomic,assign)            BOOL isTimeout;

@property (nonatomic,assign)            BOOL m_isSupportedMessage;

@property (nonatomic,assign)            BOOL isNeedTriggerUpdate;

@property (nonatomic,assign)            BOOL isContainCommentFeature;

@property (nonatomic,assign)            BOOL isCouldReallySupport;

@property (nonatomic,assign)            BOOL isSaveInDatabase;

@property (nonatomic,assign)            BOOL isWaring;

@property (nonatomic,assign)            BOOL isBotMessageCanReply;

@property (nonatomic,assign)            BOOL hasReplyPending;

@property (nonatomic,assign)            BOOL isFollowedThread;

@property (nonatomic,readonly)          BOOL isSendOrReceived;

@property (nonatomic,readonly)          BOOL isSending;

@property (nonatomic,readonly)          BOOL isSendByMe;

@property (nonatomic,readonly)          BOOL isSendSuccessMessage;

@property (nonatomic,readonly)          BOOL isSendErrorMessage;

@property (nonatomic,readonly)          BOOL isSendFailMessage;

@property (nonatomic,readonly)          BOOL isMarkUnread;

@property (nonatomic,readonly)          BOOL isE2EDecodeFailed;

@property (nonatomic,readonly)          BOOL isE2EMessageDecoded;

@property (nonatomic,readonly)          BOOL hasReaction;

@property (nonatomic, assign, readonly) BOOL isSingleFileMessage;

@property (nonatomic, assign, readonly) BOOL isPlainTextMessage;

// maintain by SDK. (only contain texts, not contain files, custom emojis and other elements)
@property (nonatomic, assign) BOOL isPureTextMessage;

@property (nonatomic, readonly)         BOOL isSubCMCMessage;
@property (nonatomic, readonly)         BOOL isSubCMCBotMessage;

/** Some type of image (like .png, .jpg, or .jif) would be deleted from file-server and not sync with xmpp server, which can no longer be download anymore. This "isNotExistAtServerImage" serves as a flag of those invalid image. Those image-msg would convert to text-msg */
@property (nonatomic,assign)            BOOL isNotExistAtServerImage;

/**message reply**/
@property (nonatomic,assign)            BOOL isComment;

@property (nonatomic,assign)            BOOL isThread;

@property (nonatomic,assign)            BOOL hasComments;

@property (nonatomic,assign)            NSInteger iCommentsCount;

/**message mentioin**/
@property (nonatomic,assign)            BOOL m_atEveryone;

@property (nonatomic,assign)            BOOL m_atMe;

@property (nonatomic,assign)            BOOL m_atMentionGroup;

@property (nonatomic,retain,nullable)   NSMutableArray*   m_atList;

@property (nonatomic,retain,nullable)   NSMutableArray*   m_mentionInfoList;

/**message pin**/
@property (nonatomic,copy,nullable)     NSString *pinner;

@property (nonatomic,assign)            BOOL isTopPin;

/**message font style**/
@property (nonatomic,retain,nullable)   NSMutableArray*   m_fontStyleInfoList;

/**message image/file status**/
@property (nonatomic,readonly)          BOOL hasFile;

@property (nonatomic,assign)            BOOL m_isFileDownloaded;

@property (nonatomic,assign)            BOOL m_isPlayed;

@property (nonatomic,assign)            BOOL m_isPreviewPicDownloaded;

@property (nonatomic,assign)            BOOL m_isFileUploaded;

@property (nonatomic,assign)            NSUInteger m_uploadPercentage;
@property (nonatomic,assign)            NSUInteger m_uplaodBitsPerSecond;
@property (nonatomic,assign)            NSUInteger m_uploadTransferredSize;
@property (nonatomic,assign)            NSInteger  m_uploadErrorID;
@property (nonatomic,assign)            NSUInteger m_audioLength;
@property (nonatomic,assign)            NSUInteger m_videoLength;
@property (nonatomic,assign)            NSUInteger m_fileSize;

@property (nonatomic,copy,nullable)     NSString *m_picturePreviewPath;
@property (nonatomic,copy,nullable)     NSString *m_localFilePath;
@property (nonatomic,copy,nullable)     NSString *m_fileName;

@property (nonatomic,readonly)          NSArray <ZMChatFileItem *> * _Nullable allFiles;

@property (nonatomic,readonly)          NSArray <NSString *> * _Nullable allFileIDs;

@property (nonatomic,retain,nullable)   ZMFileEntity *fileModel;

/**message apps card**/
@property (nonatomic,assign)            BOOL isAppPreviewCardMsgWithNoTextAndFile;

@property (nonatomic,assign)            NSUInteger appPreviewCardCount;

@property (nonatomic,copy,nullable)     NSString *pinStringForAppPreviewCardMsg;

@property (nonatomic,retain,nullable)   ZMMeetingChatCardEntity *meetingChatModel;

/**link unfurling**/
@property (nonatomic,copy,nullable)     NSString *linkMessageID;
@property (nonatomic,copy,nullable)     NSString *linkURL;
@property (nonatomic,retain,nullable)   NSMutableArray *linkUnfurlingMessages;
@property (nonatomic,readonly)          BOOL isShortcutUnfurlingMsg;

/**zapps**/
@property (nonatomic,copy,nullable)     NSString *appId;//zapp
@property (nonatomic,copy,nullable)     NSString *zappName;
@property (nonatomic,copy,nullable)     NSString *iconPath;
@property (nonatomic,copy,nullable)     NSString *thumbnailPath;
@property (nonatomic,copy,nullable)     NSString *appUuid;

//sms
@property (nonatomic,assign)            BOOL isSMSMessage;
@property (nonatomic,assign)            NSInteger smsFileCount;

//zapp meeting chat
@property (nonatomic,retain,nullable)   ZMZAppConfShareInfo *zappShareInfo;
@property (nonatomic,copy,nullable)     NSString *appCompanyName;
@property (nonatomic,assign)            ZMZAppToastViewType   viewType;
@property (nonatomic,retain,nullable)   NSImage *betaIcon;
@property (nonatomic,assign)            BOOL isZappMessage;

// CMK Related
#if defined(__cplusplus)
@property (nonatomic,assign)            ZMMessageCMKStatus CMKStatus;
@property (nonatomic,assign)            ZMMessageCMKErrorCode CMKErrorCode;
#endif
@property (nonatomic,readonly)          NSString * _Nullable CMKErrorCodeString;
@property (nonatomic,assign)            BOOL CMKUnavailableHistoryMessage;

/**meeting chat**/
//jeffery add for meeting chat
@property (nonatomic,assign)            BOOL needMeetingStyle;

@property (nonatomic,copy,nullable)     NSString *m_receiverName;

@property (nonatomic,assign)            NSInteger m_MeetingChatmsgType;

@property (nonatomic,readonly,nullable) NSString *senderConfId;
@property (nonatomic,readonly,nullable) NSString *receiverConfId;

// If the message's sent in the meeting, the senderGuid will not be nil.
@property (nonatomic,copy,nullable)     NSString *senderGuid;
@property (nonatomic,copy,nullable)     NSString *receiverGuid;

@property (nonatomic,assign)            BOOL m_isSendToMe;

@property (nonatomic,copy,nullable)     NSString *m_meetingMsgId;

@property (nonatomic,assign)            BOOL enablePersistentChannel;



@property (nonatomic,assign)            BOOL needShowGuestLabel;

/**channel invite **/
@property (nonatomic, retain,nullable)  ZMChannelInviteRequestEntity *channelInviteRequest;

/**email info **/
@property (nonatomic,retain,nullable)   ZMShareEmailInfo *shareEmailInfo;

/**shared message **/
@property (nonatomic,copy,nullable)     NSString *sharedMessageNote;

/**schedule meeting info **/
@property (nonatomic,retain,nullable)   ZMScheduleMeetingInfo *scheduleMeetingInfo;

/** Forwarded message Info */
@property (nonatomic, retain, nullable) ZMOriginalMessageInfo *originalMsgInfo;

/// Parsed URLs in message body.
///
/// If you access this property when its value is not populated, the receiver will
/// load it in the calling thread and returns the resulting value.
@property (nonatomic, readonly, nullable) NSArray<NSString *> *parsedURLs;

@property (nonatomic, assign) BOOL hasParsedHighlightInfo;

@property (nonatomic,readonly)          BOOL isContinueBundleBotMsg;

/**backward compatibility**/
#ifdef __cplusplus
@property (nonatomic, retain,nullable) ZMMessageBackwardCompatibilityInfo *backwardCompatibilityInfo;
#endif

@property (nonatomic,retain,nullable)  ZMSharedSpaceOpenChannelInfo *shareSpaceOpenChannelInfo;

- (id _Nonnull )initWithMessage:(void *_Nonnull)zmMessage;

- (id _Nonnull )initWithMessage:(void *_Nonnull)zmMessage ignoreDeeplinks:(BOOL)ignoreDeeplinks;

- (void *_Nullable)messageExtention;
- (BOOL)isReminderMessage;
- (BOOL)supportsReminderMessage;
- (BOOL)isE2EMessageDecoded;
- (BOOL)isPicMessage;
- (BOOL)isVoiceMessge;
/**Old file message type with single file**/
- (BOOL)isFileMessge;
- (BOOL)isGiphyMessage;
- (BOOL)isDisabledFileFromOthers;
- (BOOL)isHybridMessage;
- (BOOL)isCodeSnippetMessage;
- (BOOL)isHTMLCodeSinnpetBody;
- (BOOL)isRobotMessage;
- (BOOL)isCallMessage;
- (BOOL)isVideoMessage;
#ifdef __cplusplus
- (NS_ZOOM_MESSAGER::FileIntegrationType)fileIntegrationType;
#endif
- (void)update;
- (void)updateComments;
- (void)updatePinInfo;
- (BOOL)isNotOrderGroupSysmentMessage;
- (BOOL)isCountdownMeetingCard;
- (BOOL)isScheduleMeetingMessage;
- (NSString *_Nonnull)unsupportedText;

// meeting
- (NSString *_Nullable)fetchMeetSenderName;
- (NSString *_Nullable)fetchMeetReceiverName;
- (void)requestToShareMeetingAssets:(ZMMeetingAssetsSharingCategory)categories meetingID:(NSString * _Nonnull)meetingID;
//todo-
//- (BOOL)getFileIntegrationDetailWithFileIntegrations:(std::vector<NS_ZOOM_MESSAGER::FileIntegrationData>)fileIntegrations;
- (BOOL)isSupportMarkUnread;
- (ZMThreadHasCommentType)hasCommentsType;
- (BOOL)isNeedSyncComments;
//ZOOM-108962
- (BOOL)isBroadcastAnnouncement;
- (BOOL)isBroadcastAnnouncementReceiver;
//ZOOM-164281
/**Server verify, avoid customers to get around by renaming the extension to the account's approved file types */
- (BOOL)validateFailedOfApprovedFileInServer:(NSUInteger)fileIndex;
- (NSString *_Nullable)getFileNameInServer:(NSUInteger)fileIndex;
- (BOOL)isDirectShareScreenMessage;
- (BOOL)isHuddleMessage;
- (NSString* _Nonnull)getHuddleInvitesHuddleName;
- (BOOL)getHuddleInvitesHuddleNameSearched;

#ifdef __cplusplus
- (BOOL)getFileIntegrationDetailWithFileIntegrations:(std::vector<NS_ZOOM_MESSAGER::FileIntegrationData>)fileIntegrations;

+ (ZMPinMessageInfo *_Nonnull)getPinMessageInfo:(const ns_zoom_messager::PinMessageInfo &)pinMsgInfo;

+ (ZMReminderMessageInfo *_Nonnull)getReminderMessageInfo:(const NS_ZOOM_MESSAGER::ReminderInfo &)reminderMsgInfo
     event:(const NS_ZOOM_MESSAGER::ReminderEvent)event;
#endif

// Compare senders and receivers between two messages to know if they are the same, when merging the cell header
+ (BOOL)isSameUserByAdapter:(ZMMessageAdapter *_Nonnull)firstAdapter comparedTo:(ZMMessageAdapter *_Nonnull)secondAdapter;


- (void)clearAccessoryDeepLinkList;

/*hybrid message*/
- (BOOL)onlyHasWhiteboardPreviewFlieList;
- (BOOL)hasAccessoryLists;
- (BOOL)hasAccessoryListsWithoutDeepLink;
- (BOOL)hasAccessoryListsWithoutWhiteboarePreview;
- (BOOL)hasAccessoryListsWithoutWhiteboardOrDeepLink;

- (NSArray<ZMFileEntity *> *_Nullable)getAccessoryWhiteboardPreviewList;
- (NSArray<ZMFileEntity *> *_Nullable)getAccessoryFileList;
- (NSArray<ZMFileEntity *> *_Nullable)getAccessoryImageList;
- (NSArray<ZMDeepLinkEntity *> *_Nullable)getAccessoryDeepLinkList;
- (NSArray<ZMFileEntity *> *)getDisableAccessoryFileList;

/* combined with accessoryFileList & accessoryImageList, sorted with [ZMFileEntity positionIndex] */
- (NSArray<ZMFileEntity *> *_Nullable)getSortedAccessoryList;
- (NSArray<ZMFileEntity *> *_Nullable)getSortedAccessoryListWithoutWhiteboard;

/* RTF */
- (NSArray<ZMMessageFontInfo *> *_Nullable)getFontStyleInfoList;

@end

