//
//  ZMAvatarView.h
//  ZCommonUI
//
//  Created by groot.ding on 21/07/2020.
//  Copyright © 2020 zoom. All rights reserved.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMAvatarView : ZMAccessibleView

@property (nonatomic,retain,nullable) NSImage *image;

@property (nonatomic,copy,nullable) NSString *userName;

@property (nonatomic,assign) NSInteger radius;

@property (nonatomic,assign) NSSize inset;

@property (nonatomic, retain,nullable) NSColor *avatarFillColor;

@property (nonatomic,assign) NSSize zmIntrinsicContentSize;

@property (nonatomic,retain,nullable) NSBezierPath *maskPath;

@property (nonatomic,weak,nullable) id target;

@property (nullable) SEL action;

@property (nullable) SEL keyPressAction;

@property (nullable) SEL hoverAction;

@property (nonatomic,assign) BOOL enabled;

@property (nullable,retain) id representedObject;

@property (nonatomic,assign) BOOL needShowNameTip;

@end

NS_ASSUME_NONNULL_END
