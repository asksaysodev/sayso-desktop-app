//
//  ZMIMInputButtonsContainerView.h
//  zChatComponent
//
//  Created by Yong Zhou on 10/31/22.
//

#import <ZoomKit/ZoomKit.h>
#import <Prism/Prism-Swift.h>
#import <zChatComponent/ZMIMCommonDefine.h>
#import <zChatComponent/ZMIMInputCustomButtonItem.h>
#import <zChatComponent/ZMMessageAdapter.h>

@class ZMChatsInputContainerViewCtrlModel;
@class ZMChatAppContext;

typedef NS_ENUM(NSUInteger, ZMIMInputContainerSourceType)
{
    ZMIMInputContainerSource_ReplyInput = 0,
    ZMIMInputContainerSource_ReplyEdit,
    ZMIMInputContainerSource_MainInput,
    ZMIMInputContainerSource_ShareNote,
};

typedef NS_ENUM(NSUInteger, ZMChatSourceType)
{
    ZMChatSourceType_IM = 0,
    ZMChatSourceType_Meeting
};

typedef NS_ENUM(NSUInteger, ZMChatsInputContainerViewType)
{
    ZMChatsInputContainerViewType_Normal                = 0,
    ZMChatsInputContainerViewType_Robot                 = 1 << 0,
    ZMChatsInputContainerViewType_Create                = 1 << 1,
    ZMChatsInputContainerViewType_FileDisabled          = 1 << 2,
    ZMChatsInputContainerViewType_ScreenCaptureEnable   = 1 << 3,
    ZMChatsInputContainerViewType_CodeSnippetEnable     = 1 << 4,
    ZMChatsInputContainerViewType_Encrypted             = 1 << 5,
    ZMChatsInputContainerViewType_MyNotes               = 1 << 6,
    ZMChatsInputContainerViewType_BroadcastAnnouncement = 1 << 7, //ZOOM-108962
    ZMChatsInputContainerViewType_NotAnnouncer          = 1 << 8
};


NS_ASSUME_NONNULL_BEGIN

@protocol ZMIMInputButtonsContainerViewProtocol <NSObject>

- (void)menusContainerViewDidClickMoreButton:(nullable ZMGroupedElementOnlyImageButton*)sender;
- (void)menusContainerViewDidClickFileButton:(nullable ZMGroupedElementOnlyImageButton*)sender;
- (void)menusContainerViewDidClickFormatButton:(nullable ZMGroupedElementOnlyImageButton*)sender;
- (void)menusContainerViewDidClickScreenShotButton:(nullable ZMGroupedElementOnlyImageButton*)sender;
- (void)menusContainerViewDidClickAudioButton:(nullable ZMGroupedElementOnlyImageButton*)sender;
- (void)menusContainerViewDidClickVideoButton:(nullable ZMGroupedElementOnlyImageButton*)sender;
- (void)menusContainerViewDidClickCodeSnippetButton:(nullable ZMGroupedElementOnlyImageButton*)sender;
- (void)menusContainerViewDidClickEmojiButton:(nullable ZMGroupedElementOnlyImageButton*)sender;
- (void)menusContainerViewDidClickGiphyButton:(nullable ZMGroupedElementOnlyImageButton*)sender;
- (void)menusContainerViewDidClickSendButton:(nullable ZMGroupedElementOnlyImageButton*)sender;

@optional
- (void)menusContainerViewDidClickScheduleDropdownButton:(nullable ZMGroupedElementOnlyImageButton *)sender;
- (void)menusContainerViewDidClickScreenShotOptionButton:(nullable ZMGroupedElementOnlyImageButton*)sender;
- (void)menusContainerViewDidClickSaveEditButton:(nullable ZMGroupedElementOnlyImageButton*)sender;
- (void)menusContainerViewDidClickCancelEditButton:(nullable ZMGroupedElementOnlyImageButton*)sender;

- (void)menusContainerViewDidClickSaveScheduledButton:(nullable ZMGroupedElementOnlyImageButton *)sender;
- (void)menusContainerViewDidClickCancelScheduledButton:(nullable ZMGroupedElementOnlyImageButton *)sender;
- (void)menusContainerViewDidClickChatApp:(ZMChatAppContext *)chatAppContext;

- (NSArray <PZMMenuItem *> *)menusContainerExtendMoreButtonMenu:(nullable ZMGroupedElementOnlyImageButton*)sender;

- (NSInteger)getCurrentTextLength;

- (NSInteger)getMaximunTextLength;

@end

@interface ZMIMInputButtonsContainerView : ZMBaseView

@property (nonatomic, weak) id<ZMIMInputButtonsContainerViewProtocol> delegate;

/**ZMChatsInputContainerViewType*/
@property (nonatomic, assign) NSUInteger containerViewStyle;

/**This flag  tells if we should show & hide a button with the specified  state.
 Example, In ZMEditMessageViewController,  add/edit reply using the specified rule */
@property (nonatomic, assign) BOOL usingSpecifiedButtonDisplayFlag;

/** SpecifiedDisplayFlag will make effect when  usingSpecifiedButtonDisplayFlag  is YES */
@property (nonatomic, assign) NSUInteger specifiedDisplayFlag;

@property (nonatomic, readonly, retain) ZMGroupedElementOnlyImageButton *formatButton;
@property (nonatomic, readonly, retain) ZMGroupedElementOnlyImageButton *emojiButton;
@property (nonatomic, readonly, retain) ZMGroupedElementOnlyImageButton *giphyButton;
@property (nonatomic, readonly, retain) ZMGroupedElementOnlyImageButton *fileButton;
@property (nonatomic, readonly, retain) ZMGroupedElementOnlyImageButton *screenShotButton;
@property (nonatomic, readonly, retain) ZMGroupedElementOnlyImageButton *screenShotOptionButton;
@property (nonatomic, readonly, retain) ZMGroupedElementOnlyImageButton *recordButton;
@property (nonatomic, readonly, retain) ZMGroupedElementOnlyImageButton *videoButton;
@property (nonatomic, readonly, retain) ZMGroupedElementOnlyImageButton *codeButton;
@property (nonatomic, readonly, retain) ZMGroupedElementOnlyImageButton *moreButton; /* ZOOM-401850-Meeting */
@property (nonatomic, readonly, retain) ZMGroupedElementOnlyImageButton *sendButton;
@property (nonatomic, readonly, retain) ZMGroupedElementOnlyImageButton *saveEditButton;
@property (nonatomic, readonly, retain) ZMGroupedElementOnlyImageButton *cancelEditButton;
@property (nonatomic, readonly, retain) ZMGroupedElementOnlyImageButton *scheduleDropdownButton;

/* send app card from Reply should have a thread **/
@property (nonatomic, nullable, retain) NSString *threadID;
@property (nonatomic, nullable, retain) NSString *sessionID;

/* ZOOM-779723 **/
@property (nonatomic, nullable, retain) NSString *messageID;

/* current for ZMEditMessageViewController only **/
@property (nonatomic, assign) MessageType messageType;

// UI configs Model: nullable (ZOOM-401850)
@property (nonatomic, readonly, nullable, retain) ZMChatsInputContainerViewCtrlModel *model; // @Cain

@property (nonatomic, assign) BOOL needShowCharacterLimit;

/*
 The designated initializer.
 */
- (instancetype)initWithSourceType:(ZMIMInputContainerSourceType)sourceType
                         sessionID:(nonnull NSString *)sessionID
                             model:(nullable ZMChatsInputContainerViewCtrlModel*)model;

/*
 The designated initializer for ZMEditMessageViewController
 */
- (instancetype)initWithSourceType:(ZMIMInputContainerSourceType)sourceType
                         sessionID:(nonnull NSString *)sessionID
                             model:(nullable ZMChatsInputContainerViewCtrlModel*)model
                       messageType:(MessageType)messageType;

- (void)hideAllMenu;
- (void)updateButtons;

- (void)startScheduledEdit;
- (void)endScheduledEdit;

/** Update File button  state */
- (void)setBeyondMaxFileCount:(BOOL)beyond;
- (void)updateFileButtonState2Enable:(BOOL)enable eventType:(ZMFileButtonDisabledType)type;

/** Update Send button state */
- (void)setSendButtonEnabled:(BOOL)enable;

- (void)updateCharacterLimitsLabel;

@end

NS_ASSUME_NONNULL_END
