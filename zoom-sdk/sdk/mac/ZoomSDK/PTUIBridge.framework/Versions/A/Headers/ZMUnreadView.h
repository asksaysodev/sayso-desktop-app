//
//  ZMUnreadView.h
//  zChatUI
//
//  Created by groot.ding on 2018/7/7.
//  Copyright © 2018 Zoom. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <ZoomKit/ZoomKit.h>

typedef enum : NSUInteger {
    ZMUnreadStyleNone = 0,
    ZMUnreadStyleShowNumber,
    ZMUnreadStyleRedPoint,
    ZMUnreadStyleTipNumber,
    ZMUnreadStyleNew,
    ZMUnreadStyleBeta,
    ZMUnreadStyleFree,
    ZMUnreadStyleError
} ZMUnreadStyle;

@interface ZMUnreadView : ZMAccessibleView

@property (nonatomic,assign) NSUInteger unreadCount;

@property (nonatomic,assign) CGFloat cornerRadius;

@property (nonatomic,assign) ZMUnreadStyle style;

@property (nonatomic,assign) CGSize insetSize;

@property (nonatomic,assign) CGFloat minWidth;

@property (nonatomic,assign) CGFloat intrinsicWidth;

@property (nonatomic,assign) CGFloat intrinsicHeight;

@property (nonatomic,strong) NSColor *borderColor;

@property (nonatomic,assign) CGFloat borderWidth;

@property (nonatomic,strong) NSColor *bgColor;
@property (nonatomic,strong) NSColor *badgeColor;

@property (nonatomic,assign) CGFloat fontSize;

@end
