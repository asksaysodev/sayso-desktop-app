//
//  ZMEmojiHoverableCollectionView.h
//  ZoomUnit
//
//  Created by Mario Rao on 2023/7/20.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMEmojiModel, ZMEmojiHoverableCollectionView, ZMEmojiViewItem;

@protocol ZMEmojiHoverableCollectionViewDelegate <NSCollectionViewDelegate>
@optional
- (BOOL)disableNavigationUpAndDownForEmojiCollectionView:(ZMEmojiHoverableCollectionView *)collectionView;
///default value is 8.
- (NSInteger)numberOfItemsInLineForEmojiCollectionView:(ZMEmojiHoverableCollectionView *)collectionView;
- (void)emojiCollectionView:(ZMEmojiHoverableCollectionView *)collectionView didSelectModelByKeyboard:(ZMEmojiModel *)emojiModel;
@end

@interface ZMEmojiHoverableCollectionView : NSCollectionView

@property (nonatomic, assign) BOOL isTrackingContent;
@property (nonatomic, assign) BOOL forceAcceptFirstResponderAndKeyView;

- (void)resetAllItemHoverStatus;
- (ZMEmojiModel *)currentHoveredEmojiModel;
- (NSIndexPath *)currentHoveredIndexPath;
- (NSArray<ZMEmojiViewItem *> *)visibleItemsInCurrentFrame;
- (ZMEmojiViewItem *)firstVisibleItem;
- (NSIndexPath *)firstVisibleIndexPathAndAllVisibleItems:(NSArray<ZMEmojiViewItem *> **)resultVisibleItems;

@end

NS_ASSUME_NONNULL_END
