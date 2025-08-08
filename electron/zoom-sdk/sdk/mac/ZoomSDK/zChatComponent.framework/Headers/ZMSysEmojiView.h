//
//  ZMSysEmojiView.h
//  ZoomUnit
//
//  Created by Mario Rao on 2023/7/16.
//

#import <Cocoa/Cocoa.h>


NS_ASSUME_NONNULL_BEGIN

@class ZMEmojiDisplayView;
@class ZMSysEmojiMgr;

///This is NOT a new class, just move it form ZMSysEmojiMgr.

@interface ZMSysEmojiView : NSView

@property (nonatomic, assign) BOOL drawBorder;
@property (nonatomic, assign) BOOL forceDarkMode;

@property (nonatomic, retain) NSColor *backgroundColor;
@property (nonatomic, retain) NSColor *borderColor;

@property (nonatomic, assign, readonly) BOOL isNewEmojiMode;

@property (nonatomic, strong) ZMEmojiDisplayView *emojiDisplayView;

- (instancetype)initWithFrame:(NSRect)frameRect emojiMgr:(ZMSysEmojiMgr *)emojiMgr;
- (instancetype)initWithFrame:(NSRect)frameRect emojiMgr:(ZMSysEmojiMgr *)emojiMgr itemClass:(Class)kls;
- (void)reconstructToolbar;
- (void)resetSearchField;
- (void)resetEmojiToolbar;
- (void)switchToTabViewByIndex:(NSUInteger)index;
- (void)makeSearchFiledFirstResponder;

@end

NS_ASSUME_NONNULL_END
