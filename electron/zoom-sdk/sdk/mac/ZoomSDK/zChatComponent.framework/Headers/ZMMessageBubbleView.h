//
//  ZMMessageBubbleView.h
//  ZCommonUI
//
//  Created by groot.ding on 21/07/2020.
//  Copyright © 2020 zoom. All rights reserved.
//

#import <ZoomKit/ZMBaseView.h>

typedef NS_ENUM(NSUInteger, ZMMessageBubbleBorderStyle) {
    ZMMessageBubbleBorderStylePlain = 0,
    ZMMessageBubbleBorderStyleHoverable,
    
    // Special styles for premium experience.
    ZMMessageBubbleBorderStyleInMeeting,
};

NS_ASSUME_NONNULL_BEGIN

@interface ZMMessageBubbleView : ZMBaseView

@property (nonatomic,weak) id target;

@property (nonatomic,assign) SEL mouseDownAction;

@property (nonatomic,assign) NSInteger headLocation;
@property (nonatomic,assign) BOOL hasContent;

@property (nonatomic,assign) float offset;

@property (nonatomic, assign) ZMMessageBubbleBorderStyle borderStyle;

@end

NS_ASSUME_NONNULL_END
