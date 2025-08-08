//
//  ZMChannelCellView.h
//  zChatUI
//
//  Created by groot.ding on 2018/5/4.
//  Copyright © 2018 Zoom. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class ZMSessionGroupAdapter;
@class ZMIMAvatarView;
@class ZMStackView;
@class ZMButton;
@class ZMAccessibilityTextField;

@interface ZMChannelCellView : ZMBaseTableCellView

@property (nonatomic,retain) ZMIMAvatarView *avatarImageView;
@property (nonatomic,retain) NSImageView *e2eImageView;

@property (nonatomic,retain) NSTextField *nameTextField;
@property (nonatomic,retain) NSTextField *descTextField;
@property (nonatomic,retain) NSTextField *markTextField;
@property (nonatomic,retain) ZMAccessibilityTextField *numberTextField;

@property (nonatomic,retain) NSColor *normalTextColor;
@property (nonatomic,retain) NSColor *selectedTextColor;
@property (nonatomic,retain) NSColor *inactiveSelectedTextColor;

@property (nonatomic,retain) ZMStackView *buttonsContainarView;

@property (nonatomic,retain) ZMButton *chatButton;
@property (nonatomic,retain) ZMButton *meetButton;
@property (nonatomic,retain) ZMButton *moreButton;
@property (nonatomic,retain) ZMButton *joinHuddleButton;

@property (nonatomic,copy) void(^chatToChannel)(ZMSessionGroupAdapter *adapter);

@property (nonatomic,copy) void(^meetToChannel)(ZMSessionGroupAdapter *adapter);

@property (nonatomic,copy) void(^moreButtonAction)(ZMChannelCellView *cellView,NSButton *button);

@property (nonatomic,copy) void(^joinHuddleToChannel)(ZMSessionGroupAdapter *adapter);

@property (nonatomic,assign) BOOL needShowDescTextField;

@property (nonatomic,assign) BOOL needShowToolButtons;

@property (nonatomic,assign) BOOL needShowPrivateIcon;

@property (nonatomic, assign) BOOL needShowPublicIcon;

@property (nonatomic,assign) BOOL needShowMembersCount;

@property (nonatomic,assign) BOOL needShowMembersCountWithBracketsStyle;

@property (nonatomic,assign) BOOL needShowStatueIcon;

@property (nonatomic, retain) NSLayoutConstraint *nameConstraintTrailing;

- (void)updateUI;

- (CGSize)avatarSize;
- (CGFloat)avatarLeadingConstant;
- (CGFloat)title2AvatarSpace;
- (CGFloat)toolButtonTrailingConstant;
- (CGFloat)toolButtonHeightConstant;
- (CGFloat)toolButtonSpace;

- (CGFloat)nameTrailingConstraint;

- (ZMSessionGroupAdapter *)getAdapter;
- (NSString *)getChannelDesc;
- (void)setup;
- (void)updateButtonStateWhenHover;
- (void)updateTextColor;

@end
