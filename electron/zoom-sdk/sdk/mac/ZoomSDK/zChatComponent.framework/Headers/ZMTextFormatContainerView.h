//
//  ZMTextFormatContainerView.h
//  zChatComponent
//
//  Created by Yong Zhou on 6/1/21.
//  Copyright © 2021 Zoom. All rights reserved.
//

#import <zChatComponent/ZMTextFormatEntity.h>
#import <zChatComponent/ZMRichFormatTextView.h>
#import <ZoomKit/ZMGroupedAccessibilityView.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM (NSInteger, ZMTextFormatContainerViewType) {
    ZMTextFormatContainerView_MainBox = 0,
    ZMTextFormatContainerView_ReplyBox,
};

typedef NS_OPTIONS(NSUInteger, ZMTextFormatContainerViewOptions) {
    ZMTextFormatContainerViewOptionsInsertLinkDisabled = 1 << 0,
    ZMTextFormatContainerViewOptionsCodeblockEnabled = 1 << 1,
};

@protocol ZMTextFormatDelegate <NSObject>
/*
 All of the following methosd have a default Implementation.
 onCheckValidUrlLink:,
 onTextFormatLinkUrlConfirmed:selectedRange:contentString:,
 onTextFormatLinkUrlRemoved
 If you need some custom behaviour for the insert-link, then just implement these delegates.
 */
@optional
- (void)onCheckValidUrlLink:(NSString*)urlString;
- (void)onTextFormatLinkUrlConfirmed:(NSString *)linkUrlString contentString:(NSAttributedString *)contentAttrStr selectedRange:(NSRange)selectedRange;
- (void)onTextFormatLinkUrlRemoved:(NSRange)range;
- (BOOL)zmTextFormatCheckValidLink:(NSString *)inputLink outLink:(NSString *_Nullable*_Nullable)outLink;
@end

@interface ZMTextFormatContainerView : ZMGroupedAccessibilityView

@property (nonatomic, weak) id<ZMTextFormatDelegate> delegate;
@property (nonatomic, retain) ZMTextFormatInfo *formatInfo;
@property (nonatomic, assign) ZMTextFormatContainerViewType sourceType;
@property (nonatomic, copy) void (^checkLinkURLCompletion)(BOOL isValidUrl, BOOL needsClose);
@property (nonatomic, copy) void(^onCodeblockClicked)(void);

/* The designated initializer.
 */
- (instancetype)initWithFrame:(NSRect)frameRect textView:(nonnull ZMRichFormatTextView*)textView options:(ZMTextFormatContainerViewOptions)options NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithFrame:(NSRect)frameRect NS_UNAVAILABLE; // Use the designated initializer initWithFrame: textView:
- (instancetype)initWithCoder:(NSCoder *)coder NS_UNAVAILABLE; // Use the designated initializer initWithFrame: textView:

- (void)updateFormatsBtnStatusWithEnabledFormats:(NSUInteger)format;
- (void)showAddLinkViewForSelection;
- (void)showLinkViewWithUrl:(nullable NSString*)urlString linkRange:(NSRange)range relatedView:(NSView *)relatedView relatedRect:(NSRect)relatedRect;
- (void)closeAllPopoverView;
- (void)clearFormatInfo;

@end

NS_ASSUME_NONNULL_END
