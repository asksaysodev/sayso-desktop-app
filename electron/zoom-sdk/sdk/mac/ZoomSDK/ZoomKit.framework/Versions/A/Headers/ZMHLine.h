//
//  ZMHLine.h
//  zChatUI
//
//  Created by Huxley Yang on 2018/1/29.
//  Copyright © 2018 Zipow. All rights reserved.
//

#import <ZoomKit/ZMAccessibleView.h>
#import <ZoomKit/ZoomKit.h>

@interface ZMHLine : ZMAccessibleView

+ (instancetype)horizontalLineWithFrame:(NSRect)frame;

@property IBInspectable ZMUserInterfaceOrientation orientation;
@property IBInspectable CGFloat lineWidth;
@property (copy) IBInspectable NSColor *lineColor;

@end
