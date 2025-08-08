//
//  ZMJoinChannelViewController.h
//  zChatUI
//
//  Created by groot.ding on 2018/5/9.
//  Copyright © 2018 Zoom. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class ZMPublicRoomInfo;

@interface ZMJoinChannelViewController : ZMBaseViewController

@property (nonatomic,copy) NSString *shareSpaceId;

- (void)startInitView;

@property (nonatomic, copy) void(^onPreview)(ZMPublicRoomInfo* info);
@end
