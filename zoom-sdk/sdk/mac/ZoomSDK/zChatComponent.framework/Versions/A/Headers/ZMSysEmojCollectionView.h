//
//  ZMSysEmojCollectionView.h
//  ZoomUnit
//
//  Created by Mario Rao on 2023/7/16.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMEmojiModel;

@protocol ZMSysEmojiActionDelegate <NSObject>
- (void)emojiItemDidClicked:(ZMEmojiModel *)emojiModel;
@end

///This is NOT a new class, just move it form ZMSysEmojiMgr.

@interface ZMSysEmojCollectionView : NSCollectionView {
    NSInteger      _hoverIndex;
}

@property (nonatomic, assign) BOOL respondToKeyboard;
@property (nonatomic, strong) NSNumber *zm_canBecomeKeyView;
@property (nonatomic, strong) NSNumber *zm_canBecomeFirstResponder;

@property (weak) id myController;
@property (nonatomic, weak) id<ZMSysEmojiActionDelegate> actionDelegate;

- (id)initWithFrame:(NSRect)frameRect itemViewClass:(Class)itemViewClass;
- (NSString*)selectedEmojiCode;
- (void)hoverItemViewByIndex:(NSInteger)idx andIsHovered:(BOOL)isHovered;
- (void)resetAllItemHoverStatus;
- (void)mouseIsMoving;
- (void)mouseExisted;
- (void)produceTrackingBySelf;
- (void)stopProducesTracking;

@end

NS_ASSUME_NONNULL_END
