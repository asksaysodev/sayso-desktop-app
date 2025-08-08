//
//  ZMMessageListViewController.h
//  zChatComponent
//
//  Created by groot Ding on 4/30/25.
//

#import <ZoomKit/ZoomKit.h>
#import <zChatComponent/ZMChatMessageDataSource.h>
#import <zChatComponent/ZMMessageCellViewDelegate.h>
#import <zChatComponent/ZMMessageInputProtocol.h>
#import <zChatComponent/ZMIMListView.h>
#import <zChatComponent/ZMIMCommonDefine.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMFilePickerChatApp;
@class ZMChatMsgItemInfo;
@class ZMChatFileItem;
@class ZMIMMessageLayoutConfig;
@class ZMBaseMessageCellView;
@class ZMToastCoordinator;
@class ZMIMMessageLayoutConfig;
@class ZMChatAppShortcutInfo;
@class ZMInputTextView;
@class ZMChatsInputContainerViewCtrl;
@class ZMAddReplyMessageCellView;
@class ZMToast;

@interface ZMStashTableViewVisableInfo : NSObject

@property (nonatomic,retain) id targetNode;
@property (nonatomic,assign) NSPoint offset;

@property (nonatomic,assign) BOOL isBottomWhenStash;
@property (nonatomic,readonly) BOOL isEditingWhenStash;

@end

@interface ZMMessageListViewController : ZMBaseViewController <ZMChatMessageDataSource,
ZMChatMessageLoadDelegate,ZMListViewDataSource,ZMListViewDelegate,ZMMessageCellViewDelegate>

@property (nonatomic,strong) ZMIMListView *messagesOutlineView;

@property (nonatomic,copy) NSString* sessionID;

@property (nonatomic,strong) ZMChatMessageDataSourceImpl *mainDataSourceImpl;

@property (nonatomic,readonly) NSString *sessionName;

@property (nonatomic,readonly) NSString *sessionNameWithoutGroup;

@property (nonatomic,copy) NSString *loadingReplyThread;

@property (nonatomic,readonly) BOOL isVirtualSession;

@property (nonatomic,readonly) BOOL isMySelfSession;

@property (nonatomic,readonly) BOOL isMeetingSession;

@property (nonatomic,readonly) BOOL isPrivateSession;

@property (nonatomic,readonly) BOOL isE2EEnabled;

@property (nonatomic,readonly) BOOL needLoadMoreRecent;

@property (nonatomic,assign) BOOL isManualScroll;

@property (nonatomic,assign) BOOL isSyncDoneWhenInit;

@property (nonatomic,assign) BOOL automaticLoadMessageWhenOnline;

@property (nonatomic,assign) BOOL isNeedShowLoadFail;

@property (nonatomic,strong,nullable) ZMIMMessageLayoutConfig *layoutConfig;

@property (nonatomic,copy) NSArray<ZMChatAppShortcutInfo *> *threadShortcuts;

// layoutManagerClass, works when tableView reloads, nullable (ZOOM-401850)
@property (nonatomic,assign) Class layoutManagerClass;

@property (nonatomic,strong) ZMToastCoordinator *toastCoordinator;

@property (nonatomic,strong) ZMStashTableViewVisableInfo *stashVisableInfo;

@property (nonatomic,strong) ZMFilePickerChatApp *filePickerChatApp;

- (instancetype)initWithSessionID:(NSString*)sessionID;

- (void)setupDataSource;
- (void)setupSubViews;
- (void)setupMessageOutlineView;

- (ZMChatMessageDataSourceImpl *)dataSourceImpl;
- (ZMChatMessageDataSourceImpl *)getDataSourceBySessoinId:(NSString *)sessionId;

- (ZMChatsInputContainerViewCtrl *)inputViewCtrl;

- (Class)fetchCellLayoutManager;

- (void)registerNotification;

- (void)resetCellHeight:(BOOL)fontSizeChanged;

- (void)updateUIForFontSizeRate:(NSInteger)sizeOffset;

- (BOOL)checkNeedUpdateBuddies:(NSArray<NSString *> *)jids;

- (void)updateVisableBuddies;

- (void)updateVisableBuddies:(NSArray<NSString *>*)jids;

- (void)updateNames;

- (void)resetVisibleCell;

- (NSInteger)findStableRowForStashingScroll;

- (void)scrolledToTheEnd:(NSScrollView *)scrollView;

- (ZMInputTextView *)mainInputTextView;

- (void)checkBlockStatus;
- (void)updateWhenBlockStatusChanged:(BOOL)isBlocked;

- (void)updateReadReceiptsMessageWithForceUpdate:(BOOL)forceUpdate __attribute__((objc_direct));

- (BOOL)updateMessageWithID:(NSString*)msgID;

- (void)scheduleDecodeVisableUndecodedMessages;

- (void)onResendItem:(ZMChatMsgItemInfo*)item forCellView:(ZMBaseMessageCellView*)cellView;

- (BOOL)shouldJump2Message:(NSTreeNode *)msgNode;

- (void)addInputMessageReply:(NSTreeNode *)msgNode;
- (ZMAddReplyMessageCellView*)getReplyMessageCellViewForThreadID:(NSString*)threadID createIfNeed:(BOOL)createIfNeed;

- (void)addTimeChatIfNeed;
- (void)updateTimedChatMessage;

- (void)loadMorePullDownMessages;
- (void)loadMorePullUpMessages;
- (void)loadMoreCommentMessageByIds:(NSDictionary *)threadIds;
- (void)updateCanLoadMorePullDownMessage;
- (void)updateCanLoadMorePullUpMessage;
- (void)updateLoadMorePullDownFinish;
- (void)updateLoadMorePullUpFinish;
- (void)reloadMoreMessageWithThreadCount:(NSInteger)threadCount;

- (void)onSyncDone;

- (void)updateFromMessageCellViewWithNode:(NSTreeNode *)item;
- (void)updateMessageUIByNode:(NSTreeNode *)msgNode;
- (void)noteHeightOfRowsWithIndexesChangedWithoutAnimation:(NSIndexSet*)indexSet;

- (void)tryStashCurrentVisibleScroll;
- (void)tryRecoverCurrentVisibleScroll;

- (void)stashCurrentVisableScroll;
- (void)recoverLastVisableScroll;

- (ZMStashTableViewVisableInfo *)stashCurrentVisableListView;
- (BOOL)recoverVisableListView:(ZMStashTableViewVisableInfo *)info;

- (void)resetLastVisableScroll;

- (BOOL)scrollToEditMyFirstThread;
- (BOOL)scrollToEditMyPreviewCommentUnderThread:(NSString*)threadID;

- (BOOL)legalToEditMessageByMsgId:(NSString *)msgId;
- (void)beginEditMessageByNode:(NSTreeNode *)node;
- (void)onCodeSnippetEdit:(NSString*)inMsgId;
- (BOOL)isPullDownToLoadMore;

#pragma mark - tipview
- (void)showTipView:(ZMTipView_Type)type;
- (void)showTipView:(ZMTipView_Type)type withData:(NSDictionary* _Nullable)extraData;
- (void)showBroadcastAnnouncementPlaceholderView:(ZMTipView_Type)type;
- (void)closeTipView;

- (void)onMessageCountChanged;
- (void)onMessageCountChanged:(NSError *)error;
- (void)onMessageCountChanged:(NSError *)error isFromServer:(BOOL)isFromServer isSyncHistory:(BOOL)isSyncHistory;

- (void)showToast:(ZMToast*)toast;
- (void)removeToast:(ZMToast*)toast;

- (void)openImagePreviewByItem:(ZMChatMsgItemInfo *)item fileItem:(ZMChatFileItem *)fileItem;

#pragma mark - loading & failed

- (void)showLoadingFailedUI;
- (void)hideLoadingFaileddUI;
- (BOOL)isShowLoadingIndicator;
- (BOOL)isShowLoadFailed;
- (void)startLoadingIndicator;
- (void)stopLoadingIndicator;

@end

NS_ASSUME_NONNULL_END
