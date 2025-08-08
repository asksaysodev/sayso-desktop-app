//
//  ZMTabControlMenu.h
//  ChatUI
//
//  Created by Huxley Yang on 8/4/23.
//  Copyright © 2023 Zoom. All rights reserved.
//

#import <ZoomKit/ZoomKit.h>

@class ZMTabControlItem;

typedef void(^ZMTabControlMenuDropCallback)(ZMTabControlItem * _Nullable itemFrom, ZMTabControlItem * _Nullable itemTo);

NS_ASSUME_NONNULL_BEGIN

@interface ZMTabControlMenu : ZMHMenu

@property (copy, nullable) ZMTabControlMenuDropCallback dropCallback;

@end

@interface ZMTabControlMenuItem : ZMHMenuItem

@property BOOL asDraggingImage;
@property (weak, nonatomic) ZMTabControlItem *tabItem;

@end

@interface ZMTabControlMenuItemView : ZMHMenuItemView

@property (weak) ZMTabControlMenuItem *representedItem;
- (ZMTabControlItem *)tabItem;

@end

NS_ASSUME_NONNULL_END
