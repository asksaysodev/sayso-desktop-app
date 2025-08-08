//
//  ZMMessageContentViewController.h
//  ChatUI
//
//  Created by Groot Ding on 2022/4/7.
//  Copyright © 2022 Zoom. All rights reserved.
//

#import <ZoomKit/ZoomKit.h>
#import <zChatComponent/ZMChatMessageDataSource.h>
#import <zChatComponent/ZMMessageCellViewDelegate.h>
#import <zChatComponent/ZMMessageInputProtocol.h>
#import <zChatComponent/ZMIMListView.h>
#import <zChatComponent/ZMIMCommonDefine.h>
#import <zChatComponent/ZMMessageListViewController.h>
#import <ZoomUnit/ZoomUnit-Swift.h>

NS_ASSUME_NONNULL_BEGIN

#if __cplusplus
static constexpr float kZMMessageContentViewController_reminderStackViewOffset = -16.f;
#else
static const float kZMMessageContentViewController_reminderStackViewOffset = -16.f;
#endif

extern NSString *kIM_ChatSessionWillDisappear;

@class ZMChatMessageItemInfo;
@class ZMAddReplyMessageCellView;
@class ZMInputTextView;
@class ZMIMMessageLayoutConfig;
@class ZMChatMsgItemInfo;
@class ZMBaseMessageCellView;
@class ZMChatsInputContainerViewCtrl;
@class ZMChatAppContext;
@class ZMEditMessageInfoResult;
@class ZMIMMessageDraft;
@class ZMMessageTipView;

/// UI Configs Model
/// Suggest adding UI configs instead of business params for generality
@interface ZMMessageContentViewControllerModel : NSObject

@property (nonatomic, copy) NSString *sessionId;
@property (nonatomic, assign) BOOL needLoadHistoryWhenInit; // Determine invoke `initMessages` when `viewDidLoad`
@property (nonatomic, assign) BOOL needKeepUnFold;

@end

@interface ZMMessageContentViewController : ZMMessageListViewController

@property (nonatomic,strong) NSMutableArray *newAddedMarkUnreadMsgArray NS_RETURNS_NOT_RETAINED;

@property (nonatomic,copy) NSString *selfMarkReadMsgId;

@property (nonatomic,assign) BOOL shouldScrollToEnd;

@property (nonatomic,assign) NSInteger readedMsgTime;

@property (nonatomic,assign) BOOL jumpToBtnCanVisible;

@property (nonatomic,assign) BOOL isShowPinHightlight;

@property (nonatomic,assign) BOOL isNeedShowRemindButton;

@property (nonatomic,copy) NSString *highlightBackgroundSearchMessageId;

@property (nonatomic,copy) NSString *highlightBubbleMessageId;

@property (nonatomic,assign) BOOL shouldCleanUnread;

@property (nonatomic,strong) ZMStackView * reminderBtnStackViews;

@property (nonatomic,strong) ZMMessageTipView *reminderButton;

@property (nonatomic,strong) ZMMessageTipView *markUnreadReminderView;

@property (nonatomic, copy) void (^updateUnreadMentionMe)(NSString *msgId);

@property (nonatomic, strong) NSLayoutConstraint *scrollViewTopConstraint;
@property (nonatomic, strong) NSLayoutConstraint *scrollViewBottomConstraint;
@property (nonatomic, strong) NSLayoutConstraint *reminderStackViewBottomConstraint;

@property (nonatomic, readonly, assign) NSEdgeInsets scrollViewContentInsets;

@property (nonatomic,weak) ZMChatsInputContainerViewCtrl *inputViewCtrl;
@property (nonatomic, weak) ZMChatsInputContainerViewCtrl *inputViewCtrlForNewChats;

- (instancetype)initWithModel:(ZMMessageContentViewControllerModel *)model;

- (void)setupSubViews;
- (void)viewDidShow;

- (void)onSessionActiveChanged:(BOOL)isActived;
- (void)updateMessageWhenActiveChanged:(BOOL)isActived;
- (void)cleanUpUnreadMessages;
- (void)cleanUpUnreadMessagesFromMenu:(BOOL)cleanTotal;
- (void)cleanUnreadCommentsForThread:(NSInteger)svrSideTime;
- (void)resetUnreadHint;
- (void)cleanAllMessages;
- (void)checkAutoScroll;
- (BOOL)isStartAtFirstUnread;
- (BOOL)needUnreadHint;
- (BOOL)isDropMode;
- (BOOL)needDragFiles;
- (CGFloat)messageTopMargin;
- (NSInteger)getUnreadMessageCount;
- (NSInteger)getReadedMessageTime;
- (NSString *)mySelfJid;
- (void)checkReminderViewWhenReceiveMessage:(ZMMessageAdapter*)message;


- (BOOL)jumpToMessageID:(NSString*)messageId;
- (BOOL)jumpToMessageIfShowed:(ZMChatMessageItemInfo *)msgInfo;
- (BOOL)shouldJump2MessageOnBottom:(NSTreeNode *)msgNode;
- (void)jumpToUnreadHintMsg;
- (void)jumpToUnreadMessage;
- (void)jumpStartOfUnreadMessage;
- (void)scrollToEndOfDocument;

- (void)initMessages;

- (void)onMarkUnreadUpdate:(NSString *)sessionId messageIds:(NSArray <NSString *> *)messageIds;
- (void)resetMarkUnreadWhenViewDidShow;
- (void)schedule2CleanVisibleAreaMarkUnreadMsg;
- (void)markUnreadOrReadChanged;
- (NSArray*)getSortedMarkUnreadMessageArray:(BOOL)filterNewMarkUnread;

- (void)sessionUnreadCountReset:(BOOL)isFromOtherPlatform;

- (void)cleanNoDropAllUnreadComments;

- (BOOL)requestNewPageAfterOnlineAgain;

- (void)tryToHighlightMessageBubble;
- (void)cancelHighlightMessageBubble:(NSString *)messageId;
- (BOOL)tryToHighlightBackground;


- (void)setReminderButtonToVisible:(BOOL)isVisible;
- (void)setMarkUnreadReminderBtnVisible:(BOOL)isVisible;
- (void)scheduleCheckReminderButtonVisible;
- (BOOL)unread_isMainWindow;

- (void)didUpdateUnreadMentionMeMessageByMsgId:(NSString *)msgId;

- (void)quoteMessage:(ZMMessageAdapter *)message; //jeffery add for meeting chat
- (BOOL)canReplyToMessge:(ZMMessageAdapter *)message;

- (void)hideAllMenu;
- (void)closeMessageAlerts;

- (void)markAllMsgRead;

- (void)invalidateScrollViewContentInsets;

- (void)switchMessageContentNormalType;

- (BOOL)jumpToSearch:(id)param;
- (BOOL)jumpToSearch:(id)param needHighlightBackground:(BOOL)needHighlightBackground;

- (void)editScheduledMessage:(nonnull ZMIMMessageDraft *)draft;
- (void)finishEditingScheduledMessage;

- (void)updateE2EStatus:(BOOL)isEnableE2E;
- (void)checkShowUserGuideInE2EChannelAfterSendHybridMsg;

- (BOOL)isLastMessageVisible;

- (BOOL)isScheduledMessageBannerShow;


#pragma mark - Chat App
- (void)openChatApp:(ZMChatAppContext *)context referenceView:(nullable NSView *)view;

#pragma mark Smart Reply
- (void)requestComposeSmartReplyIfNeed;
- (void)hideComposeSmartReply;

- (void)onWindowResizing;

- (BOOL)isFirstOpenSession;

#pragma mark - AIC+
- (NSDictionary *)getVisibleMessagesInfo;

@end

NS_ASSUME_NONNULL_END
