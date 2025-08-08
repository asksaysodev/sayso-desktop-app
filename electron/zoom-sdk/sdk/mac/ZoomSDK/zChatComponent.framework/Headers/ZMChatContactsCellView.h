//
//  ZMChatContactsCellView.h
//  zChatUI
//
//  Created by groot.ding on 2018/5/3.
//  Copyright © 2018 Zoom. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <zChatComponent/ZMIMAvatarView.h>
#import <ZoomUnit/ZMSipContactItemProtocol.h>
#import <Prism/Prism-Swift.h>

@class ZMButton;
@class ZMStackView;
@class ZMBuddyAdapter;
@class ZMSubscribeRequestAdapter;
@class ZMLabelIconTextView;

typedef enum : NSUInteger {
    ZMChatContactsCellTypeDefault = 0,
    ZMChatContactsCellTypeSessionInfo = 1 << 0,
    ZMChatContactsCellTypeContactsChannelMemberInfo = 1 << 1,
} ZMChatContactsCellType;

@interface ZMChatContactsCellView :ZMBaseTableCellView <ZMHMenuDelegate>

@property (nonatomic,retain)  ZMIMAvatarView *avatarImageView;

@property (nonatomic,retain)  NSTextField *nameTextField;
@property (nonatomic,retain)  NSTextField *signTextField;
@property (nonatomic,retain)  PZMTextBadge *externalBadge;

@property (nonatomic,retain)  NSTextField *namePendingTextField;

@property (nonatomic,retain)  ZMStackView *buttonsContainarView;
@property (nonatomic,retain)  ZMButton *chatButton;
@property (nonatomic,retain)  ZMButton *meetButton;
@property (nonatomic,retain)  ZMButton *callButton;
@property (nonatomic,retain)  ZMButton *moreButton;
@property (nonatomic,retain)  ZMButton *smsButton;
@property (nonatomic,retain)  ZMButton *addContactButton;
@property (nonatomic,retain)  ZMButton *BTransferButton;
@property (nonatomic,retain)  ZMButton *WTransferButton;
@property (nonatomic,retain)  ZMButton *VoicemailButton;

@property (nonatomic,retain)  NSImageView *reminderBellView;

@property (nonatomic,retain)  NSColor *nameNormalTextColor;
@property (nonatomic,retain)  NSColor *nameSelectedTextColor;
@property (nonatomic,retain)  NSColor *nameInactiveSelectedTextColor;

@property (nonatomic,retain)  NSColor *signNormalTextColor;
@property (nonatomic,retain)  NSColor *signSelectedTextColor;
@property (nonatomic,retain)  NSColor *signInactiveSelectedTextColor;

@property (nonatomic,assign) BOOL needShowToolButtons;

@property (nonatomic,assign) BOOL needShowSign;

@property (nonatomic,assign) BOOL needShowAvatarHighlight;

@property (nonatomic,assign) BOOL needShowAvatarPresence;

@property (nonatomic,assign) BOOL needShowBlocked;

@property (nonatomic,assign) BOOL needShowHuddleBorder;

@property (nonatomic, assign) ZMChatContactsCellType cellType;

@property (nonatomic,copy) void(^chatToBuddy)(ZMBuddyAdapter *buddy);

@property (nonatomic,copy) void(^meetToBuddy)(ZMBuddyAdapter *buddy);

@property (nonatomic,copy) void(^callToBuddy)(ZMBuddyAdapter *buddy);

@property (nonatomic,copy) void(^smsToBuddy)(ZMBuddyAdapter *buddy);

@property (nonatomic,copy) void(^acceptToBuddy)(ZMBuddyAdapter *buddy);

@property (nonatomic,copy) void(^ignoreToBuddy)(ZMBuddyAdapter *buddy);

@property (nonatomic,copy) BOOL(^addContactToBuddy)(ZMBuddyAdapter *buddy);

@property (nonatomic,copy) void(^moreButtonAction)(ZMChatContactsCellView *cellView,NSButton *button);

@property (nonatomic, assign, readonly) BOOL isBuddyReallyExternalContact;

@property (nonatomic,retain) NSLayoutConstraint *avatarConstraintLeading;
@property (nonatomic,retain) NSLayoutConstraint *namePendingTextFieldLeading;
@property (nonatomic, retain) NSLayoutConstraint *nameConstraintTrailing;

@property (nonatomic,assign) BOOL needShowChatIgnoDeavtivated;
@property (nonatomic,assign) BOOL needShowTransferButtons;
@property (nonatomic,assign) BOOL needShowreminderBell;


- (void)updateUI;
- (void)updateButtonStateWhenHover;
- (void)updateToolButtons;
- (void)updateTextColor;
- (void)updateButtonState;
- (void)updateButtonWhenSelected;

- (ZMBuddyAdapter *)getBuddy;
- (id <ZMSipContactItem>)getCloudBuddy;

- (CGSize)avatarSize;
- (CGFloat)avatarLeadingConstant;
- (CGFloat)title2AvatarSpace;
- (CGFloat)toolButtonTrailingConstant;
- (CGFloat)toolButtonHeightConstant;
- (CGFloat)toolButtonSpace;
- (CGSize)toolButtonSize;
- (CGFloat)toolButtonRadius;

- (CGFloat)nameTrailingConstraint;

- (ZMButton *)getToolButton NS_RETURNS_RETAINED;
- (void)addToolButtons:(ZMStackView *)containarView;

- (void)setup;

- (void)onMouseEntered;

@end

