//
//  NSCollectionView+ZoomKit.h
//  ZCommonUI
//
//  Created by Huxley on 2018/6/27.
//  Copyright © 2018 zoom. All rights reserved.
//

#import <ZoomKit/ZMBase.h>

@interface NSCollectionView (ZoomKit)

@property (nullable, nonatomic) __kindof NSCollectionViewItem *latestHoveredItem;

- (__kindof NSCollectionViewItem * __nullable)itemAtPoint:(NSPoint)point;

@end
