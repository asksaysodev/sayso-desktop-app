//
//  ZMChatsInputContainerViewCtrl.h
//  ZoomSDKChatUI
//
//  Created by ryan on 25/05/2018.
//  Copyright © 2018 Zoom. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <Prism/Prism-Swift.h>
#import <zChatComponent/ZMIMCommonDefine.h>
#import <zChatComponent/ZMStickerMgr.h>
#import <zChatComponent/ZMRTFShortcutWindowCtrl.h>
#import <zChatComponent/ZMIMScreenShotMgr.h>
#import <zChatComponent/ZMChatsInputContainerViewCtrlModel.h>
#import <zChatComponent/ZMIMInputButtonsContainerView.h>
#import <zChatComponent/ZMInputTextView.h>
#import <ZoomUnit/ZoomUnit-Swift.h>
@class ZMChatsInputFileView;
@class ZMChatsSessionViewCtrl;
@class ZMScreenShotItem;
@class ZMInputTextView;
@class ZMGroupedElementOnlyImageButton;
@class ZMInputContainerViewCtrl;
@class ZMGroupedAccessibilityView;
@class ZMTextFormatContainerView;
@class ZMFileEntity;
@class ZMSendMessageParams;

typedef NS_ENUM(NSUInteger, ZMChatsInputSendButtonToolTipType)
{
    ZMChatsInputSendButtonToolTip_Default     = 0,
    ZMChatsInputSendButtonToolTip_CanNotSendSignleChatApp
};

@protocol ZMChatsInputProtocol <NSObject>

- (void)popupNewChatNoContactTipWithSessionId:(NSString*)sessionId;
- (BOOL)isSessionHasContactWithSessionId:(NSString*)sessionId;
@end

@protocol ZMChatsInputContainerViewCtrlDelegate <NSObject>

@optional

- (NSArray <PZMMenuItem *> *)chatInputViewController:(__kindof NSViewController *)viewController getMoreButtonExtendMenuItems:(__kindof NSButton *)button;
- (void)chatInputViewController:(__kindof NSViewController *)viewController didClickMenuMoreButton:(__kindof NSButton *)button;
- (void)chatInputViewController:(__kindof NSViewController *)viewController didClickMenuFileButton:(__kindof NSButton *)button;
- (BOOL)chatInputViewControllerCheckCanSendFile:(__kindof NSViewController *)viewController;
- (BOOL)chatInputViewControllerCheckCanSendMessage:(__kindof NSViewController *)viewController;
- (BOOL)chatInputViewControllerCheckCanEnableSendButton:(__kindof NSViewController *)viewController;
- (void)chatInputViewController:(__kindof NSViewController *)viewController textDidChange:(NSNotification *)aNotification;
- (BOOL)chatInputViewControllerCheckDLP:(__kindof NSViewController *)viewController;
- (CGFloat)chatInputViewControllerMaxInputHeight:(__kindof NSViewController *)viewController;

- (void)finishEditingScheduledMessage;

- (void)chatInputViewControllerDidOpenExternalMembersList:(__kindof NSViewController *)viewController;

@end

@protocol ZMChatsSessionViewCtrlDelegate <NSObject>

@optional

- (void)sessionNewChatsViewCtrlDidShow;
- (void)checkShowUserGuideInE2EChannelAfterSendHybridMsg;
- (NSDictionary *)getNewChatMembersInfoInSession;
- (void)sendMessageToNewChat:(NSDictionary *)data;
- (void)didSendMessageInputViewController:(__kindof NSViewController *)viewController;
- (void)didFormatModeChange:(__kindof NSViewController *)viewController;
- (void)didContentHeightChange:(__kindof NSViewController *)viewController;
@end

@interface ZMChatsInputContainerViewCtrl : ZMBaseViewController
<
ZMRTFShortcutWindowCtrlDelegate,
ZMChatEmojiOperationDelegate,
ZMScreenshotCallback,
ZMInputTextViewDelegate,
ZMToastCoordinatorProvider,
ZMIMInputButtonsContainerViewProtocol
>
@property (nonatomic,weak) id<ZMChatsSessionViewCtrlDelegate> sessionViewCtrlDelegate;
@property (nonatomic,weak) id<ZMChatsInputContainerViewCtrlDelegate> delegate;
@property (nonatomic,retain) ZMIMInputButtonsContainerView *menusContainerView;
@property (nonatomic,retain) ZMTextFormatContainerView *formatsContainerView;
@property (nonatomic,retain) ZMInputTextView* inputTextView;
@property (nonatomic, assign) BOOL isReplacingZoomLinks;
@property (nonatomic,retain) ZMChatsInputFileView *inputFileView;
@property (nonatomic,retain) ZMPopupAlertViewController* alertMentinAll;
@property (nonatomic,assign) NSInteger style;
@property (nonatomic, retain) NSString* sessionID;
@property (assign) BOOL isFromHoverOrSelect;
@property (nonatomic, assign) NSUInteger iBtnFlag;
@property (nonatomic,copy) void (^heightChange)(float height);
@property (nonatomic, copy) void (^inputTextViewFirstRespondereChanged)(BOOL isFirstResponder);
@property (nonatomic, assign) BOOL isChannelEnabled3rdPartStorage;
@property (nonatomic, assign) BOOL showPrivacyExternalWarning;
@property (nonatomic, assign) NSInteger largeChannelWarningMemberCount;

@property (nonatomic, assign) NSTimeInterval editDraftTime;
@property (nonatomic, retain) NSString *activeDraftID;
@property (nonatomic, assign) BOOL isGroupChat;
@property (nonatomic, copy) NSString *searchRequestID;
@property (nonatomic, copy, readonly) NSString* searchKey;

// Custom top view, nullable
@property (nonatomic, retain) __kindof NSView *topView;
@property (nonatomic, assign) NSEdgeInsets topViewInsets;
@property (nonatomic, assign) CGFloat topViewHeight;

@property (nonatomic, assign) CGFloat manualHeight;

@property (nonatomic, assign) BOOL needHideAtList;
- (void)changeShouldShowTopView:(BOOL)shouldShowTopView needUpdate:(BOOL)needUpdate;

// UI configs Model: nullable (ZOOM-401850)
@property (nonatomic, retain) ZMChatsInputContainerViewCtrlModel *model;

@property (nonatomic, retain) ZMToastCoordinator *toastCoordinator;

// Init methods
- (instancetype)initWithModel:(ZMChatsInputContainerViewCtrlModel *)model;
- (instancetype)initWithSessionID:(NSString*)sessionID;
- (void)updateSessionID:(NSString*)sessionID;

/*
 Set placeholder
*/
// IM ssession custom placeholders: A)"What's on your mind" for my note; B) "Message someone" for DM; C) "Announce" for broadcast;
- (void)updateIMSessionPlaceholder:(NSString *)name;
// Text view placeholder
- (void)updatePlaceholder:(NSString *)placeholder;

- (void)cleanUp;
- (void)viewDidShow;
- (void)focusInputView;

- (void)updateButtons;
- (void)viewFrameDidChange:(NSNotification*)notification;

- (BOOL)sendMessageWithMessageParam:(ZMSendMessageParams *)messageParam;

- (void)sendDataForNewChat:(NSDictionary*)data;
- (void)sendFiles:(NSArray<NSString*>*)files;
- (NSRect)emojiButtonFrame;
- (void)try2UpdateEmojiWndPosition;
- (void)selectCommand:(NSString*)command;
- (void)onUploadLocalFile;
- (void)onUploadLocalFileWithNode:(NSString*)nodeID;
- (void)onUploadLocalFileWithTriggerThread;

- (void)sendMessage;
- (void)checkToSendMessage;
- (void)disableSendButton;
- (void)updateSendButtonStatus;

//save draft
- (void)saveDraftToCloud:(BOOL)forceCloudSync;
- (NSString*)currentDraft;
- (NSArray *)currentDraftFileList;
- (NSDate *)draftLastEditTime;
- (void)cleanDraft;

//addon
- (void)addonCopyMsgAction:(NSString*)bodyMessage andType:(NSInteger)type;

- (void)updateE2EStatus:(BOOL)isEnableE2E;

- (void)updateUIForFontSizeRate:(NSInteger)sizeOffset;

- (void)popupNewChatNoContactTip;
- (BOOL)isSessionHasContact;

- (BOOL)closePopWindowIncludeFontMarkWindow:(BOOL)isInclude;

- (BOOL)showCommandListWithKey:(NSString*)key;

- (void)addIntegrationFile:(NSDictionary*)info;

- (BOOL)tryTriggerThreadInputboxActionWith:(NSButton*)btn;
- (BOOL)enterToSelectCommondOrContact;

- (BOOL)isMyNotes;
- (BOOL)isRobot;
- (BOOL)isNewChat;
- (BOOL)isScreenCaptureEabled;
- (BOOL)isCodeSnippetEnabled;
- (BOOL)isBroadcastAnnouncement;
- (BOOL)isEncrypted;
- (BOOL)isInputMode;
- (BOOL)isViewHadShown;

- (void)cleanInputFile;
- (BOOL)hasInputFile;

- (void)cleanContent;
- (BOOL)hasContent;

- (void)updateInputStatus:(NSNumber*)inStatusNumber;

- (void)updateInputArea;
- (ZMChatsInputViewStatus)getInputAreaStatus;

- (BOOL)updateInputAreaWithStatus:(ZMChatsInputViewStatus)status;

- (void)hideAllMenu;

// Capture Image
- (void)screenShotReceivedWithPath:(NSString *)imagePath;

// warp sending params: need to override
- (ZMSendMessageParams *)wrapSendMessageParams;

- (void)recoverInputWithDraft:(ZMIMMessageDraft *)draftItem;

- (void)showScheduledPopoverIfNeeded;

- (CGFloat)minInputHeight;
- (CGFloat)maxInputHeight;
- (CGFloat)bestInputHeight;
- (void)updateNotInChannelMentionWhenUsersAdded:(NSArray<NSString*>*)added removed:(NSArray<NSString*>*)removed;

- (void)updateBannerViewLoginCSEKey;
@end
