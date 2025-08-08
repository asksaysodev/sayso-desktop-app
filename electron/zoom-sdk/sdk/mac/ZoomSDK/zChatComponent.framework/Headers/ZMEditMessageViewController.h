//
//  ZMEditMessageViewController.h
//  zChatUI
//
//  Created by groot.ding on 2019/5/29.
//  Copyright © 2019 Zoom. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <zChatComponent/ZMIMInputButtonsContainerView.h>
#import <zChatComponent/ZMMessageDraftHelper.h>

@class ZMInputTextView;
@class ZMStickerInfo;
@class ZMChatsInputFileView;
@class ZMEditMessageInfo;
@class ZMFileEntity;
@class ZMIMOnlyImageButton;
@class ZMEditMessageViewController;
@class ZMChatAppContext;
@class ZMSendMessageParams;

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, ZMEditMessageUIStyle)
{
    ZMEditMessageUIStyle_AddReply     = 0,
    ZMEditMessageUIStyle_EditMessage  = 1<<0,
    ZMEditMessageUIStyle_ShareNote    = 1<<1,
};

typedef NS_ENUM(NSUInteger, ZMRecordMessageType)
{
    ZMRecordMessageVoiceType,
    ZMRecordMessageVideoType
};

@protocol ZMEditMessageViewControllerProtocol <NSObject>

- (void)editViewController:(ZMEditMessageViewController *)vc addReplyDidClickFileButton:(id)sender;
- (BOOL)editViewController:(ZMEditMessageViewController *)vc checkScreenshotPath:(NSString *)imagePath;
- (void)editViewController:(ZMEditMessageViewController *)vc textDidChange:(NSNotification *)notification;
// fetch a flag: should enable parsing deeplinks
- (BOOL)editViewControllerFetchEnableParseDeeplink:(ZMEditMessageViewController *)vc;
// invoke when deeplink parsing is disabled
- (void)editViewControllerShowDisableDeeplink:(ZMEditMessageViewController *)vc;
- (BOOL)editViewControllerCheckDLP:(ZMEditMessageViewController *)vc;
- (BOOL)editViewControllerIsSupportMentions:(ZMEditMessageViewController *)vc;
@optional
- (void)finishEditingScheduledMessage;

//screenshot
- (void)meetingChatReplyStartScreenshot:(BOOL)needHideWindow;
- (void)meetingChatReplyScreenshotCompleted:(BOOL)successed;

- (BOOL)editViewControllerCheckCanSendFile:(ZMEditMessageViewController *)vc;
- (BOOL)editViewControllerCheckCanSendMessage:(ZMEditMessageViewController *)vc;

- (void)editViewController:(ZMEditMessageViewController *)vc openChatApp:(ZMChatAppContext *)context;

@end

@interface ZMEditMessageViewController : ZMBaseViewController

@property (nonatomic,retain) ZMIMInputButtonsContainerView *menusContainerView;

@property (nonatomic,copy,nullable) NSString *sessionID;

@property (nonatomic,copy,nullable) NSString *msgID;

@property (nonatomic,copy) NSString *threadId;

@property (nonatomic,retain) ZMInputTextView *inputTextView;
@property (nonatomic, assign) BOOL isReplacingZoomLinks;

@property (nonatomic,retain) ZMChatsInputFileView *inputFileView;

@property (nonatomic,readonly) NSEdgeInsets inputFileViewEdgeinset;

@property (nonatomic,assign) BOOL isContainAt;

@property (nonatomic,assign) BOOL hasAppPreviewCard;

@property (nonatomic,assign) BOOL couldBeEmpty;

/**
 default YES
 */
@property (nonatomic,assign) BOOL needEmojiButton;

/**
 default NO
 */
@property (nonatomic,assign) BOOL needFileButton;

/**
 default NO
 */
@property (nonatomic,assign) BOOL needRecordButton;

/**
 default NO
 */
@property (nonatomic,assign) BOOL needRecordVideoButton;

/**
 default NO
 */
@property (nonatomic,assign) BOOL needScreenShotButton;

/**
 default NO
 */
@property (nonatomic,assign) BOOL needCodeSinppetButton;

/**
 default NO
 */
@property (nonatomic,assign) BOOL needGiphy;
/**
 default NO
 */
@property (nonatomic,assign) BOOL needCustomEmoji;
/**
 default NO
 For in meeting chat
 */
@property (nonatomic,assign) BOOL needHideAtList;

@property (nonatomic,assign) BOOL needHideFormat;
/**
 default NO
  1. If return NO, when clicked the screenshot button, ZMIMScreenShotMgr will read 'kCaptureWithoutChatWindow' from userDefalt to determine whether or not it will capture current window.
  2. If return YES, ZMIMScreenShotMgr will capture current window.
 */
@property (nonatomic,assign) BOOL needForceCaptureWindow;
/**
 default YES
 */
@property (nonatomic,assign) BOOL isVideoPlayable;

@property (nonatomic,assign) ZMEditMessageUIStyle style;

@property (nonatomic,assign,getter = isFormatMenusExpanded) BOOL formatMenusExpanded;

@property (nonatomic,assign) BOOL hasCodeblockOnTop;

@property (nonatomic,copy) void (^commitMessage)(ZMSendMessageParams *params);

@property (nonatomic,copy) BOOL (^recordBtnClicked)(ZMRecordMessageType recordMessageType);

@property (nonatomic,copy) void (^selectFileIntegration)(NSInteger fileType);

@property (nonatomic,copy) void (^cancelEditMessage)();

@property (nonatomic,copy) void (^updateMessage)(NSAttributedString *message, NSArray<ZMFileEntity *> *fileListArr);

@property (nonatomic,copy) void (^updateMessageHeight)();

@property (nonatomic,copy) void (^updateFirstResponse)(BOOL firstResponse);

@property (nonatomic,copy) void (^saveDraftToCloud)(BOOL forceCloudSync);

@property (nonatomic, copy) void (^endScheduledMessageEdit)();

@property (nonatomic, copy, nullable) void (^didInputAt)();

@property (nonatomic, weak) id<ZMEditMessageViewControllerProtocol> delegate;

@property (nonatomic, retain, nullable) ZMIMMessageDraft *scheduledMessage;

- (void)makeInputViewFirstResponse;

- (void)cleanContent;
- (void)updateUIWithStyle:(ZMEditMessageUIStyle)style;
- (void)updateTextViewLayout;
- (void)updateButtonsLayout;

- (NSAttributedString *)messageString;

- (void)removeAllMentions;

- (void)setEditMessageInfo:(ZMEditMessageInfo *)editMessageInfo;
- (void)addAttributedStringToDismissLinks:(ZMEditMessageInfo *)editMessageInfo;

- (void)setInputTextViewFontSize:(NSInteger)fontSize;

- (void)selectUserWithJid:(NSString*)jid name:(NSString*)name isCheckAtRange:(BOOL)isCheckAt;

- (CGFloat)calculateToolMenusHeight:(BOOL)formatsExpanded;
- (NSSize)calculateSizeWithAttributeStirng:(NSAttributedString *)string width:(CGFloat)width;
- (NSSize)calculateSizeWithFileLists:(NSArray <ZMFileEntity *> *)fileLists width:(CGFloat)width;

- (void)updateFileInputViewHeight:(CGFloat)height scrollViewHeight:(CGFloat)scrollViewH;

- (void)uploadLocalFile;
- (void)addIntegrationFile:(NSDictionary*)info;
- (BOOL)isFileButtonEnable;

- (nullable NSArray *)fontStyleInfoWithAttachment:(BOOL)parseAttachment;
- (NSArray *)getDismissLinks;
- (NSDictionary *)getAppMessagePreviewField;
- (void)recoverLinksWithDraft:(NSAttributedString *)attributedString;
- (void)commitReplyMessage;
- (void)checkToCommitReplyMessage;

- (void)updateAttributedPlaceholder:(NSAttributedString *)attributedPlaceholder;

@end

NS_ASSUME_NONNULL_END
