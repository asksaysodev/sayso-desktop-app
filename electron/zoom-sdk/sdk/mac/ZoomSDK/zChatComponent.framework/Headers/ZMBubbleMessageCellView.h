//
//  ZMBubbleMessageCellView.h
//  ZCommonUI
//
//  Created by groot.ding on 21/07/2020.
//  Copyright © 2020 zoom. All rights reserved.
//

#import <zChatComponent/ZMBaseMessageCellView.h>
#import <zChatComponent/ZMMessageBubbleView.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMAvatarView;
@class ZMMessageBubbleView;
@class ZMReactionContainarView;
@class ZMStackView;
@class ZMButton;
@class ZMIMImageButton;
@class ZMIMOnlyImageButton;
@class ZMCopyMessageInfo;
@class ZMReminderContainerView;

static const NSInteger kDefaultReminderBubbleTopToBottom = 2;

@interface ZMBubbleMessageCellView : ZMBaseMessageCellView

@property (nonatomic,retain) ZMIMAvatarView* avatarView;

@property (nonatomic,retain) NSTextView *nameTextView;

@property (nonatomic,retain) NSTextField *unreadTextField;

@property (nonatomic,retain) NSTextField* dateField;

@property (nonatomic,strong) ZMButton *readReceiptsBtn;

@property (nonatomic,retain) ZMMessageBubbleView *bubbleView;

@property (nonatomic,retain) ZMReactionContainarView* reactionView;

@property (nonatomic,retain) ZMReminderContainerView* reminderView;

@property (nonatomic, readonly, retain) NSTextField *commonErrorMessageLabel;

@property (nonatomic,assign,getter=isSelectedBubble) BOOL selectedBubble;

@property (nonatomic,readonly) BOOL isShowDecodeFail;

@property (nonatomic,readonly, retain) ZMStackView* buttonsView;

@property (readonly, assign) CGFloat bubbleTop;
@property (readonly, assign) CGFloat bubbleLeading;
@property (readonly, assign) CGFloat bubbleHeight;
@property (readonly, assign) CGFloat bubbleWidth;

- (BOOL)isUnreadViewVisible;

- (void)updateNameView;
- (void)updateAvatarView;
- (void)updateBubbleView;
- (void)updateReactionView;
- (void)updateToolButtonsStatus;
- (void)updateCommonErrorMessageLabel;
- (void)updateDateField;

- (CGFloat)buttonsViewLeadingConstraintConstant;

- (void)mouseDownInBubbleView;

- (ZMCopyMessageInfo *)getCopyMessageInfo;

- (nullable NSString *)commonErrorMessage;

- (void)onMouseEntered;
- (void)onMouseExited;
@end

NS_ASSUME_NONNULL_END
