//
//  ZMIMMessageLayoutManager.h
//  zChatUI
//
//  Created by groot.ding on 2019/6/18.
//  Copyright © 2019 Zoom. All rights reserved.
//

#import <AppKit/AppKit.h>

typedef enum : NSUInteger {
    ZMMessageToolButtonsAlignmentMessageBubbleTop,
    ZMMessageToolButtonsAlignmentRightTop,
    ZMMessageToolButtonsAlignmentRightFloating, // ZOOM-401850
} ZMMessageToolButtonsAlignment;

typedef enum : NSUInteger {
    ZMMessageDateAlignmentTrailing,
    ZMMessageDateAlignmentLeadingToName,
    ZMMessageDateAlignmentLeadingToAvatar,
} ZMMessageDateAlignment;

typedef enum : NSUInteger {
    ZMViewMoreCommentsButtonBule,
    ZMViewMoreCommentsButtonGrey
} ZMViewMoreCommentsButtonStyle;

typedef enum : NSUInteger {
    ZMMoreButtonNormal,
    ZMMoreButtonBorde,
    ZMMoreButtonShowJump,
} ZMMoreButtonStyle;

typedef enum : NSUInteger {
    ZMMessagePinTipAlignmentLeadingToBubble,
    ZMMessagePinTipAlignmentLeadingToReplyCount,
} ZMMessagePinTipAlignment;

typedef NS_ENUM(NSUInteger, ZMMessageSendFailedViewStyle) {
    ZMMessageSendFailedViewStyleNone = 0,
    ZMMessageSendFailedViewStyleRetryAndDelete = 1,
    ZMMessageSendFailedViewStyleRetryOnly = 2,
};

@class ZMFileEntity;

@protocol ZMMessageLayoutDelegate <NSObject>

@required

- (NSEdgeInsets)contentToCellViewEdgeInsets;

- (NSEdgeInsets)bubbleToCellViewEdgeInsets;

- (NSEdgeInsets)messageToBubbleEdgeInsets;

- (NSEdgeInsets)accessoryViewToBubbleEdgeInsets;

- (NSEdgeInsets)shortcutViewToBubbleEdgeInsets;
/*!
 @abstract The space inbetween message elements
 @discussion Some message elements are link previews, the text bubble, files, and images
 */
- (CGFloat)messageElementSpacing;

- (NSEdgeInsets)nameToCellViewEdgeInsets;

- (NSColor *_Nonnull)nameColor;

- (NSFont *_Nonnull)nameFont;

- (NSSize)avatarSize;

- (CGSize)threadAvatarSize;

- (CGSize)replyAvatarSize;

- (CGFloat)avatarRadius;

- (NSColor *_Nullable)avatarFillColor;

- (BOOL)avatarEnable;

- (CGFloat)bubbleToAvatarSpacing;

- (CGFloat)threadBubbleToAvatarSpacing;

- (CGFloat)commentAvatarToReadLineSpacing;

- (NSEdgeInsets)avatarToCellViewEdgeInsets;

- (NSEdgeInsets)dateToCellViewEdgeInsets;

- (ZMMessageDateAlignment)dateAlignment;

- (CGFloat)bubbleCornerRadius;

- (NSColor *_Nullable)bubbleColor;

- (NSColor *_Nullable)backgroundColor;

- (NSEdgeInsets)reactionToCellEdgeInsets;

- (NSEdgeInsets)pinTipToCellEdgeInsets;

- (NSEdgeInsets)linkPreviewToCellViewEdgeInsets;

- (CGFloat)sendFailedViewToBubbleBottomSpace;

- (CGFloat)sendFailedViewHeight;

- (CGFloat)viewMoreCommentsButtonHeight;

- (ZMViewMoreCommentsButtonStyle)viewMoreCommentsButtonStyle;

- (ZMMessageToolButtonsAlignment)toolButtonsAlignment;

- (CGFloat)toolButtonSpacing;

- (ZMMessagePinTipAlignment)pinTipAlignment;

- (ZMMoreButtonStyle)moreButtonStyle;

- (BOOL)isMutableSelected;

- (BOOL)validateMenuItem:(NSMenuItem *_Nonnull)menuItem;

- (BOOL)isNeedFitFontSizeRate;

- (CGFloat)currentFontSizeRate;

- (NSLayoutAttribute)promptContentLayout;

@end

@protocol ZMMessageShowElementDelegate <NSObject>

@required

- (BOOL)isNeedFileValidationBtn;

- (BOOL)isNeedShowAvatar;

- (BOOL)isNeedShowName;

- (BOOL)isNeedShowUnread;

- (BOOL)isNeedShowDate;

- (BOOL)isNeedShowReadReceipts;

- (BOOL)isNeedShowToolButtons;

- (BOOL)isNeedShowMarkButton;

- (BOOL)isNeedShowAddReactionButton;

- (BOOL)isNeedShowAddReplyButton;

- (BOOL)isNeedShowExtendReplyButton;

- (BOOL)isNeedShowReactionEmoji;

- (BOOL)isNeedShowReactionView;

- (BOOL)isNeedShowCommentVisibleButton;

- (BOOL)isNeedShowViewMoreCommentView;

- (BOOL)isNeedShowDecodeFailedButton;

- (BOOL)isNeedShowDecodingProgressIndicator;

- (BOOL)isNeedShowMessageSendingSignView;

- (BOOL)isNeedShowMoreButtonWhenSendingMessage;

- (BOOL)isNeedShowMessageSendingMaskView;

- (ZMMessageSendFailedViewStyle)sendFailedViewStyle;

- (BOOL)isNeedShowCommonErrorMessageLabel;

- (BOOL)isNeedShowReadLine;

- (BOOL)isNeedShowLastMsgBottomReactButtons;

- (BOOL)isNeedshowReactionHoverButton;

- (BOOL)isNeedShowTopPinTip;

- (BOOL)isNeedShowPin;

- (BOOL)isNeedShowLinkPreview;

- (BOOL)isNeedShortcutPreview;

- (BOOL)isNeedShowAccessoryListView;

- (BOOL)isNeedShowSaveAsButton4ImageViewWithFile:(ZMFileEntity*_Nonnull)fileItem;

- (BOOL)isNeedShowScreenshot;

- (BOOL)isNeedShowMoreButton;

- (BOOL)isNeedShowFileButton;
- (BOOL)isNeedShowFileButtonForEdit;

- (BOOL)isNeedShowGIFPauseIcon;
- (BOOL)isNeedShowTranslationUI;

- (BOOL)isNeedShowSmartTools;
- (BOOL)isNeedShowVoiceMail;

- (BOOL)isNeedShowPresenceOnAvatar;

@end

@protocol ZMMessageElementClassDelegate <NSObject>

- (Class _Nonnull )nameTextViewClass;

- (Class _Nonnull )textMessageTextViewClass;

@end


@class ZMChatMsgItemInfo;

@class ZMIMMessageLayoutConfig;


@interface ZMIMMessageLayoutManager : NSObject <ZMMessageLayoutDelegate,ZMMessageShowElementDelegate,ZMMessageElementClassDelegate>

@property (nonatomic,retain,nullable) ZMIMMessageLayoutConfig *config;

@property (nonatomic,retain,nullable) ZMChatMsgItemInfo *messageItem;

@property (nonatomic,weak,nullable) NSView *messageContentView;

- (ZMChatMsgItemInfo *_Nullable)item;

@end
