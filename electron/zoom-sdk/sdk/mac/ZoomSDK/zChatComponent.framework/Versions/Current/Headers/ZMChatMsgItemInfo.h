//
//  ZMChatMsgItemInfo.h
//  zChatUI
//
//  Created by groot.ding on 2019/5/16.
//  Copyright © 2019 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import <ZoomUnit/ZoomUnit.h>
#import <zChatComponent/ZMReactionEmojiInfo.h>
#import <zChatComponent/ZMDeeplinkItemInfo.h>
#import <zChatComponent/ZMVoiceItemInfo.h>

typedef NS_ENUM(NSUInteger, ZMChatMsgItemFlag)
{
    ZMChatMsgItemFlag_none              = 0,
    ZMChatMsgItemFlag_session           = 1<<0,
    ZMChatMsgItemFlag_starredSession    = 1<<1,
    ZMChatMsgItemFlag_starredInfoPanel  = 1<<2,
    ZMChatMsgItemFlag_InfoPanel_Pin     = 1<<3,
    ZMChatMsgItemFlag_searchMsg         = 1<<4,
    ZMChatMsgItemFlag_mentionSession    = 1<<5,
    ZMChatMsgItemFlag_previewMsg        = 1<<6,
    ZMChatMsgItemFlag_reminderSession   = 1<<7,
};

typedef NS_ENUM(NSUInteger, ZMChatMsgItemInfoType)
{
    ZMChatMsgItemInfoType_Message = 0,
    ZMChatMsgItemInfoType_Date,
    ZMChatMsgItemInfoType_UnreadSeparator,
    ZMChatMsgItemInfoType_TodayMention,
    ZMChatMsgItemInfoType_AddReply,
    ZMChatMsgItemInfoType_GroupedSystemMessage,
    ZMChatMsgItemInfoType_SummaryNewMessages
};

typedef NS_ENUM(NSUInteger, ZMMessageShowType)
{
    ZMMessageShowType_Text = 0,
    ZMMessageShowType_Prompt,
    ZMMessageShowType_Image,
    ZMMessageShowType_Giphy,
    ZMMessageShowType_File,
    ZMMessageShowType_CodeSnippet,
    ZMMessageShowType_MissCall,
    ZMMessageShowType_AddReply,
    ZMMessageShowType_Audio,
    ZMMessageShowType_App,
    ZMMessageShowType_MeetingChat,
    //zapp
    ZMMessageShowType_Zapp,
    ZMMessageShowType_ScheduleMeeting,
    ZMMessageShowType_Mail,
    ZMMessageShowType_InviteRequest,
    ZMMessageShowType_UnsupportMessage,
    ZMMessageShowType_ShareSpaceOpenChannelCard,
    ZMMessageShowType_SummaryNewMessages,
    ZMMessageShowType_PhoneCall
};

typedef NS_ENUM(NSUInteger, ZMCommentsMessagePostionType){
    ZMCommentsMessagePostionCenter  = 0,
    ZMCommentsMessagePostionTop     = 1 << 0,
    ZMCommentsMessagePostionBottom  = 1 << 1,
};

typedef NS_ENUM(NSUInteger, ZMLoadCommentsType){
    ZMNotLoadComments,
    ZMLoadFirstScreenComments,
    ZMLoadMoreComments,
};

typedef NS_ENUM(NSUInteger, ZMEditStatus){
    ZMEditStatus_None = 0,
    ZMEditStatus_isEditing, /* Will replace property of isEditing in 'ZMMessageItemInfo.h'*/
    ZMEditStatus_isCommittingEditMsg,
    ZMEditStatus_isEditFailed,
};

typedef NS_OPTIONS(NSUInteger, ZMCodeblockPositionOption) {
    ZMCodeblockPositionNone = 0,
    ZMCodeblockPositionTop = 1 << 0,
    ZMCodeblockPositionBottom = 1 << 1,
};

@class ZMMessageAdapter;
@class ZMMessageSwiftAdaptor;
@class ZMCommonAppData;
@class ZMChatXMLMessageBody;
@class ZMPTGiphyData;
@class ZMFileEntity;
@class ZMMessageFontInfo;
@class ZMIMMessageDraft;
@class ZMIMMessageLayoutManager;
@class ZMEditMessageInfo;
@class ZMMessageTranslationInfo;
@class ZMCopyMessageInfo;
@class ZMReadReceiptMessageModel;

@protocol ZMMessageSizeCacheProtocal <NSObject>

- (void)storeMessageSize:(NSSize)messageSize width:(CGFloat)width;
- (void)storeReactionSize:(NSSize)reactionSize width:(CGFloat)width;
- (void)storeReminderSize:(NSSize)reminderSize width:(CGFloat)width;

- (NSSize)getMessageSizeByWidth:(CGFloat)width hitCache:(BOOL *_Nullable)hitCache;
- (NSSize)getReactionSizeByWidth:(CGFloat)width hitCache:(BOOL *_Nullable)hitCache;
- (NSSize)getReminderSizeByWidth:(CGFloat)width hitCache:(BOOL *_Nullable)hitCache;

- (void)cleanMessageSizeCache;
- (void)cleanReactionSizeCache;

@end

@protocol ZMMessageInfoUpdate <NSObject>
- (void)didMessageInfoUpdate;
- (void)didMessageExtensionUpdate;
@end


@interface ZMChatMsgItemInfo : NSObject <ZMMessageSizeCacheProtocal,ZMMessageInfoUpdate>

@property (nonatomic,weak,nullable) NSTreeNode *zm_TreeNode;

@property (nonatomic,retain,nullable) ZMMessageAdapter * message;

@property (nonatomic,assign) NSSize inputFileViewSize;
@property (nonatomic,assign) NSSize inputFileScrollViewSize;

@property (nonatomic,assign) NSSize accessoryViewSize;
@property (nonatomic,assign) BOOL accessoryListViewSelected;
@property (nonatomic,assign) CGFloat messagePaneWidth;

@property (nonatomic, assign, readonly) BOOL needsShowForwardedMsg;
@property (nonatomic, assign) BOOL isForwardedOriginalMsg;
@property (nonatomic, assign) NSSize forwardedMessageSize;

@property (nonatomic, assign) NSSize unsupportViewSize;

/// A width the item users use to display the text view contents.
///
/// This property is used to coordinate with the attributed string cache.
/// When the value is changed, it means the cached string is no longer
/// valid. For example, app cards can display collapsed paragraphs, which
/// depends on the number of rendered lines.
@property (nonatomic,assign) CGFloat textLayoutWidth;

@property (nonatomic,assign) NSSize linkPreviewSize;

@property (nonatomic,readonly) BOOL threadNotLoad;
@property (nonatomic,assign) BOOL isDownloading;
@property (nonatomic,assign) BOOL isDecoding;
@property (nonatomic,assign) BOOL isLinkPreviewExpanded;
@property (nonatomic,assign) BOOL isDeepLinkForceDisabled;
@property (nonatomic,assign) NSUInteger  iInfoFlag;
@property (nonatomic,retain,nullable) NSArray* previewLinksInfo;

@property (nonatomic,copy,nullable) NSURL* selectedLinkRUL;
@property (nonatomic,retain,nullable) ZMCommonAppData* commonAppData;

@property (nonatomic,weak,nullable) ZMChatMsgItemInfo *parentMessage;
@property (nonatomic,retain,nullable) NSMutableArray<ZMChatMsgItemInfo*>* childMessages;
@property (nonatomic,retain,nullable) NSMutableArray<NSString*>* childURLs;

@property (nonatomic, retain, nullable) ZMChatMsgItemInfo *forwardedMessage;

@property (nonatomic,retain,nullable) NSMutableArray<ZMChatMsgItemInfo*>* shortcutChildMessages;

@property (nonatomic,retain,nullable) NSMutableArray<ZMDeeplinkItemInfo*>* deeplinkItemsInfo;

@property (nonatomic, retain, nullable) ZMVoiceItemInfo *voiceItemInfo;

@property (nonatomic,retain,nullable) ZMChatXMLMessageBody *xmlMessageBody;

/// When the message is edited, update the messageLinks, default YES.
@property (nonatomic, assign) BOOL needsUpdateMessageLinks;
@property (nonatomic,retain,nullable) NSArray *messageLinks;

@property (nonatomic, assign) BOOL needsUpdatePreviewLinks;

@property (nonatomic,retain,nullable) NSDate *visibleTime;

@property (nonatomic,assign) BOOL isCommentsCountReady;
@property (nonatomic,assign) BOOL isRetryMsg;
@property (nonatomic,assign) BOOL isHasReplyItem;
@property (nonatomic,assign) BOOL isSearchMessage;
//@property (nonatomic,assign) BOOL isFestivalType;
@property (nonatomic,assign) BOOL isExpandChildsMessage;
@property (nonatomic,assign) BOOL isNewReceiveMessage;
@property (nonatomic,assign) BOOL isPartSelected;

@property (nonatomic,assign) BOOL isEditByUpKey;

@property (nonatomic,assign) NSUInteger codeblockPosition;

@property (nonatomic,assign) BOOL hasCodeblock;

@property (nonatomic,retain,nullable) NSCursor *messageCursor;

@property (nonatomic,retain,nullable) AVPlayer *avPlayer;

@property (nonatomic,assign) BOOL isNeedHighlightBubble;

@property (nonatomic,assign) NSSize messageSize;

@property (nonatomic,assign) NSSize reactionSize;

@property (nonatomic,assign) NSSize reminderSize;

@property (nonatomic,assign) NSSize linkStackSize;

@property (nonatomic,assign) NSSize shortcutStackSize;

@property (nonatomic,assign) NSSize AIToolsStackSize;

@property (nonatomic,assign) NSSize voiceMailStackSize;

@property (nonatomic,assign) NSSize referenceMessageSize;

@property (nonatomic,copy,nullable) NSString *sessionId;//Attention: Not reliable, may be nil. Check it before using

@property (nonatomic,readonly) NSTimeInterval timeInterval;

@property (nonatomic, readonly) ZMMessageShowType showType;

@property (nonatomic, readonly) BOOL isSharedMessageWithSingleFile;

@property (nonatomic, assign) ZMChatMsgItemInfoType infoType;

@property (nonatomic,retain,nullable) ZMEditMessageInfo *editMessageInfo;

@property (nonatomic,assign) BOOL isEditMessageInfoAssigned;

@property (nonatomic,assign) BOOL isLastReadReceiptMessage;

@property (nonatomic, strong) ZMReadReceiptMessageModel *readReceiptModel;

/// Whether this message should hide head or not regardless of last read receipt.
@property (nonatomic,assign) BOOL shouldHideHead;

/// Whether this message hide head or not.
@property (nonatomic,readonly) BOOL isHideHead;


@property (nonatomic,assign) BOOL isEditing;

@property (nonatomic,assign) ZMEditStatus editStatus;

@property (nonatomic,assign) BOOL isE2EDecoding;

@property (nonatomic,assign) BOOL isUnreadState;

@property (nonatomic,setter=setMessageDirty:) BOOL isMessageDirty;

@property (nonatomic,getter=isHoverSelected) BOOL hoverSelected;

@property (nonatomic,assign) BOOL convertToTextMessage;

@property (nonatomic,readonly) BOOL isAddReplyItem;

//star
@property (nonatomic,readonly) BOOL isStarStyleMessage;

@property (nonatomic,readonly) BOOL isStarredMessagesSession;

@property (nonatomic,readonly) BOOL isStarred;

//pin
@property (nonatomic,readonly) BOOL isPinStyleMessage;

//link preview
@property (nonatomic,readonly) BOOL hasLinkPreview;
@property (nonatomic,readonly) BOOL isShortcutChildMessagesEqualToEditMessageInfos;

//Mention
@property (nonatomic,readonly) BOOL isMentionMessage;

//Reminder Messsage
@property (nonatomic,readonly) BOOL isShowingInReminderSession;
@property (nonatomic,readonly) BOOL hasReminderMessageNote;

//thread & comments
@property (nonatomic,copy,nullable) NSString *threadId;

@property (nonatomic,assign) ZMLoadCommentsType loadCommentsType;

@property (nonatomic,assign) BOOL isLastThreadMsg;

@property (nonatomic,assign) NSInteger commentsPostionType;

@property (nonatomic,assign) BOOL isLoadCommentsFromNewest;

@property (nonatomic,readonly) NSInteger currentCommentCount;
@property (nonatomic,assign) NSInteger displayCommentCount;
@property (nonatomic,assign) NSInteger displaySuccessCommentCount;
@property (nonatomic,assign) BOOL hasMoreHistoryComments;
@property (nonatomic,assign) BOOL hasMoreRecentComments;

@property (nonatomic,assign) BOOL hasUnreadComments;

//collapsed
@property (nonatomic,assign) BOOL isCollapsed;

@property (nonatomic,assign) BOOL isFormatMenusExpanded;

@property (nonatomic,assign) BOOL hasCodeblockOnTop;

@property (nonatomic,assign, readonly) BOOL usingDefaultMinHeightForReplyBox;

@property (nonatomic,assign) BOOL isShowPinHightlight;

@property (nonatomic,assign) BOOL isNeedHighlightBackground;

@property (nonatomic,assign) BOOL enableMentionGroupLink;

@property (nonatomic,assign) BOOL isUseMessageSizeCache;

//giphy
@property (nonatomic,retain,nullable) ZMPTGiphyData *giphyData;
@property (nonatomic,assign) BOOL isGiphyMixedMessageEnabled;
//emoji
@property (nonatomic,assign) BOOL isEnableEmoji;

@property (nonatomic,assign) BOOL isShowSmartIntent;
@property (nonatomic,copy,nullable) NSString *smartIntentTitle;
@property (nonatomic,assign) BOOL isSmartIntentChecked;

//search ranges
@property (nonatomic,retain,nullable) NSArray <NSValue *> *searchedRangeArray;

@property (nonatomic,weak,nullable) ZMIMMessageLayoutManager *layoutManager;

//attributedString
@property (nonatomic,readonly) NSAttributedString * _Nullable messageAttributedString;

@property (nonatomic,readonly) NSAttributedString *_Nullable editMessageAttributedString;

@property (nonatomic,readonly) NSAttributedString *_Nullable sharedMessageAttributedString;

@property (nonatomic,readonly) NSAttributedString *_Nullable nameAttributedString;

@property (nonatomic,readonly) NSAttributedString *_Nullable unsupportAttributeString;

/** ZOOM-354983 OP flag copied here so it's reachable outside of the ChatUI project */
@property (nonatomic, assign) BOOL isImageFileSharingImprovementEnabled;
/// The translation info for this message. nil if the user has not translated this message.
@property (nullable, nonatomic, retain) ZMMessageTranslationInfo* translationInfo;
/// The translation of this message with the proper attributes for display. nil if translationInfo.state != ZMTranslationState_Translated
@property (nullable, nonatomic, readonly, strong) NSAttributedString* translationAttributedString;

@property (nonatomic,readonly) BOOL isPMCUnsupportMessage;

@property (nonatomic,assign) BOOL needsRedraw;

@property (nonatomic, assign) NSInteger unreadCount;

/**
 A localized string of the reason for a send failure.
 
 Note:
 Introduced in ZOOM-484331. This is the source of truth that UI may use,
 further reasons should also be added here.
 */
@property (nonatomic, readonly) NSString * _Nullable readableSendFailureReason;

@property (nonatomic, copy) NSArray *_Nullable threadShortcuts;


@property (nonatomic, retain) NSColor * _Nullable textColor;

@property (nonatomic, retain) NSFont * _Nullable font;

//regular message default alignment left
//system message default alignment center
@property (nonatomic,assign) NSTextAlignment textAlignment;

+ (instancetype _Nonnull )addReplyItemWithSessionId:(NSString *_Nonnull)sessionId threadId:(NSString *_Nonnull)threadId;

- (ZMMessageAdapter *_Nullable)messageInfo;

- (ZMMessageSwiftAdaptor * _Nullable)messageSwiftAdaptor __attribute__((objc_direct));

- (BOOL)isNeedShowShareFile;

- (BOOL)hasLinkPreview;

- (BOOL)isMentionMessage;

- (BOOL)isSidebarMessage;

- (BOOL)isImageFile;

- (void)updateGiphyData;

- (void)checkDownloadGiphy;

- (void)updateMessageInfo;
- (void)updateCommentsInfo;

- (void)updateVisableTime;

- (void)updateFestivalType;

- (void)updateLinkPreviewLinksInfo;

- (NSArray <ZMChatAppMessagePreviewInfo *>*_Nullable)getEditReplyPreviewCards NS_RETURNS_RETAINED;

- (void)updateReactionInfo;
- (void)updateOtherExtesionInfo;
- (void)didUpdateTranslationString;

/**
 Invalidates the attributed string that represents this message, without
 updating any other data.
 
 This method is useful when the client wants to update the UI due to some
 user interactions that only affect local UI state. For example, calling
 `updateMessageInfo` method will reset the `commonAppData` object, which
 also contains some local UI state.
 */
- (void)invalidateMessageAttributedString;

- (NSAttributedString*_Nullable)linkPreviewAttributedString;
// ZOOM-354983: functions for new link preview UI
- (NSAttributedString*_Nullable)linkPreviewAttributedStringWithSpacing:(CGFloat)spacing;
- (NSAttributedString*_Nullable)linkPreviewAttributedStringWithSpacing:(CGFloat)spacing
                                              hasElementAbove:(BOOL)elementAbove
                                              hasElementBelow:(BOOL)elementBelow;

- (NSAttributedString *_Nullable)getNameAttributedString;
- (NSAttributedString *_Nullable)getNameAttributedString:(CGFloat)pixels;

- (NSString *_Nullable)getDateString;

- (NSString *_Nullable)getFileName;

- (NSImage *_Nullable)getAvatarImage;
- (NSString *_Nullable)getSessionName;



- (NSArray <ZMReactionEmojiInfo *> *_Nullable)getReactionEmojiList;

- (BOOL)startDownloadFileByManual:(BOOL)isManual;

//copy
- (ZMCopyMessageInfo *_Nullable)copyMessageByRange:(NSRange)range NS_RETURNS_NOT_RETAINED;

- (NSString *_Nullable)getCopyFilePath;

- (BOOL)updateReadReceiptsWithModel:(ZMReadReceiptMessageModel *)model;

@end

