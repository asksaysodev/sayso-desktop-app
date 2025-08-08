//
//  ZMSysEmojiItemView.h
//  ZoomUnit
//
//  Created by Mario Rao on 2023/7/16.
//

#import <Cocoa/Cocoa.h>
#import <zChatComponent/ZMCustomEmojiView.h>

NS_ASSUME_NONNULL_BEGIN

///This is NOT a new class, just move it form ZMSysEmojiMgr.

@interface ZMSysEmojiItemView : NSView

@property(nonatomic, assign) BOOL isHovered;
@property(nonatomic, assign) BOOL isCurrentSelected;
@property(nonatomic, strong) ZMEmojiModel *emoji;
@property(nonatomic, assign) NSInteger toneIndex;

- (void)updateEmojiModel:(ZMEmojiModel *)emojiModel showMode:(ZMCustomEmojiShowMode)showMode;
- (void)downloadEmojiIfNeeded;

- (void)hideAllContents;
- (void)updateContentHidden;

@end

NS_ASSUME_NONNULL_END
