//
//  ZMChannelMemberCellView.h
//  zChatUI
//
//  Created by groot.ding on 2018/5/7.
//  Copyright © 2018 Zoom. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <zChatComponent/ZMChatContactsCellView.h>

@interface ZMChannelMemberCellView : ZMChatContactsCellView

@property (nonatomic,copy) NSString *channelId;
@property (nonatomic, copy) NSString* sharedSpaceId;

@property (nonatomic,retain) NSTextField *roleTextField;

@property (nonatomic,assign) BOOL needShowRole;

@property (nonatomic,retain) NSLayoutConstraint *roleTextFieldLeadingLayoutConstraint;

- (BOOL)isPendingJoinChannel;

@end
