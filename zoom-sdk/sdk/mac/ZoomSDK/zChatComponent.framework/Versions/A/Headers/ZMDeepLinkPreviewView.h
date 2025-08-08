//
//  ZMDeepLinkPreviewView.h
//  zChatComponent
//
//  Created by Felipe Bastos on 6/8/22.
//

#import <ZoomKit/ZoomKit.h>
#import <zChatComponent/ZMDeeplinkItemInfo.h>

#if defined(__cplusplus)
static constexpr float kZMDeepLinkPreviewViewCollapsedHeight = 35.;
static constexpr float kZMDeepLinkPreviewViewMinWidth = 240.;
#else
static const float kZMDeepLinkPreviewViewCollapsedHeight = 35.;
static const float kZMDeepLinkPreviewViewMinWidth = 240.;
#endif

@protocol ZMDeepLinkPreviewViewDelegate <NSObject>
- (void)notifyDeepLinkPreviewExpandedOrCollapsed:(id _Nonnull)sender;

@end

@interface ZMDeepLinkPreviewCollapseButton : ZMButton

@property (nonatomic, assign) BOOL isCollapsed;

@end

@interface ZMDeepLinkPreviewView : ZMBaseView

@property (nonatomic, weak) id<ZMDeepLinkPreviewViewDelegate> _Nullable delegate;

@property (nonatomic, nullable, retain) ZMDeeplinkItemInfo *entity;
@property (nullable, retain) NSView *linkTypeView; // icon + text, private/public/muc
@property (nullable, retain) NSView *titleView;
@property (nullable, retain) NSTextView *descriptionView;
@property (nullable, retain) NSTextView *optionalDetailView;
@property (nullable, retain) NSImageView *imageView;
@property (nullable, retain) NSProgressIndicator *progressIndicator;

@property (nullable, retain) NSLayoutConstraint *minWidthConstraint;
@property (nullable, retain) NSLayoutConstraint *maxWidthConstraint;
@property (nullable, retain) NSLayoutConstraint *preferredWidthConstraint;
@property (nullable, retain) NSLayoutConstraint *heightConstraint;
@property (nonatomic, assign) NSUInteger maximumSuperviewWidth;

@property (nonatomic, assign) BOOL isHovered;

// is the message whose link we're previewing a response in a thread? If so, we need to shrink by 20px
@property (assign) BOOL isAttachedToReplyMessage;

@property (nullable, retain) ZMDeepLinkPreviewCollapseButton *collapseButton;
@property (nonatomic, assign) BOOL isCollapsed;

@property (nonatomic, assign) BOOL hasContentAbove;
@property (nonatomic, assign) BOOL hasContentBelow;

@property (nonatomic, retain, nullable) CAShapeLayer *borderLayer;

- (instancetype _Nullable)initWithDeeplinkInfo:(ZMDeeplinkItemInfo *_Nonnull)deeplinkInfo;
- (void)setEntity:(ZMDeeplinkItemInfo *_Nonnull)entity;

- (void)updateUI;

@end
