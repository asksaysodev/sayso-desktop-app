//
//  ZMContentCollectionView.h
//  ChatUI
//
//  Created by Michael Lin on 2023/6/25.
//  Copyright © 2023 Zoom. All rights reserved.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMContentCollectionView;
@protocol ZMContentCollectionViewDelegate <NSCollectionViewDelegate>

@optional
- (void)collectionView:(NSCollectionView *)collectionView changeZMSelected:(BOOL)zmSelected forItemAtIndexPath:(NSIndexPath *)indexPath;

- (BOOL)collectionView:(NSCollectionView *)collectionView shouldChangeZMSelect:(BOOL)zmSelect forItemAtIndexPath:(NSIndexPath *)indexPath;

@end

@interface ZMContentCollectionView : NSCollectionView

@property (nonatomic, strong, nullable) NSIndexPath *lastSelectedIndexPath;

@property (nullable, weak) id<ZMContentCollectionViewDelegate> delegate;

@property (nullable, nonatomic, strong) NSColor *zmBackgroundColor;

@property (nonatomic, assign) BOOL allowArrowKeyToMakeFirstResponder;
@property (nonatomic, assign) BOOL allowTabKeyToSwitchSection;

@end

NS_ASSUME_NONNULL_END
