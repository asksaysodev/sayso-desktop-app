//
//  ZMGridViewItemProperty.h
//  mac-app
//
//  Created by likevin on 9/28/16.
//  Copyright © 2016 zoom.us. All rights reserved.
//

#import <Cocoa/Cocoa.h>

typedef enum
{
    ZMGridViewItemVisibleContentNothing = 0,
    ZMGridViewItemVisibleContentImage = 1 << 0,
    ZMGridViewItemVisibleContentTitle = 1 << 1,
    
}ZMGridViewItemVisibleContent;

@interface ZMGridViewItemProperty : NSObject
{
    NSColor*       _backgroundColor;
    NSColor*       _selectedRingColor;
    CGFloat        _selectedRingLineWidth;
    NSUInteger     _itemBorderRadius;
    ZMGridViewItemVisibleContent _visibleContentMask;
}
@property(nonatomic, readwrite, retain)NSColor*     backgroundColor;
@property(nonatomic, readwrite, retain)NSColor*     selectedRingColor;
@property(nonatomic, readwrite, assign)CGFloat      selectedRingLineWidth;
@property(nonatomic, readwrite, assign)NSUInteger   itemBorderRadius;

+ (ZMGridViewItemProperty*)defaultProperty;
+ (ZMGridViewItemProperty*)hoveredProperty;
+ (ZMGridViewItemProperty*)selectedProperty;
@end
