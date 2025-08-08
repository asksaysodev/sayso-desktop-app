//
//  ZMChannelInfo.h
//  zChatUI
//
//  Created by groot.ding on 2018/5/4.
//  Copyright © 2018 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>

@class ZMSessionGroupAdapter;

@interface ZMChannelSection : NSObject

@property (class, readonly, nonatomic) ZMChannelSection *starredSection;
@property (class, readonly, nonatomic) ZMChannelSection *nonStarredSection;

@property (assign, readonly) BOOL starred;

@end

@interface ZMChannelInfo : NSObject

@property (nonatomic,retain) ZMSessionGroupAdapter* buddy;

@property (nonatomic,assign) BOOL isHoverSelected;

- (instancetype)initWithBuddy:(ZMSessionGroupAdapter*)buddy;

@end
