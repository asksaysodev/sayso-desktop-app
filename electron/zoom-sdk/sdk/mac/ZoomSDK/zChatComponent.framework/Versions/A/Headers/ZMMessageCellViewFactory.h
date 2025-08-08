//
//  ZMMessageCellViewFactory.h
//  zChatUI
//
//  Created by groot.ding on 2019/5/17.
//  Copyright © 2019 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <zChatComponent/ZMChatMsgItemInfo.h>
#import <zChatComponent/ZMBaseMessageCellView.h>

@interface ZMMessageCellViewFactory : NSObject

+ (instancetype)sharedInstance;

+ (void)releaseInstance;

- (Class)cellViewClassByMessageItem:(ZMChatMsgItemInfo *)item ;

- (Class)defaultLayoutManagerClass;

- (CGFloat)tableView:(NSView *)tableView cellHeightForMessage:(ZMChatMsgItemInfo *)message;

- (CGFloat)tableView:(NSView *)tableView cellHeightForMessage:(ZMChatMsgItemInfo *)message layoutManager:(Class)layoutManagerClass;

- (CGFloat)tableView:(NSView *)tableView cellHeightForMessage:(ZMChatMsgItemInfo *)message layoutManager:(Class)layoutManagerClass layoutConfig:(ZMIMMessageLayoutConfig *)layoutConfig;

- (NSSize)tableView:(NSView *)tableView cellReactionSizeForMessage:(ZMChatMsgItemInfo *)message layoutManager:(Class)layoutManagerClass layoutConfig:(ZMIMMessageLayoutConfig *)layoutConfig;

- (ZMBaseMessageCellView *)tableView:(NSView *)tableView cellForMessage:(ZMChatMsgItemInfo *)message;

- (ZMBaseMessageCellView *)tableView:(NSView *)tableView cellForMessage:(ZMChatMsgItemInfo *)message layoutManager:(Class)layoutManagerClass;

- (ZMBaseMessageCellView *)tableView:(NSView *)tableView cellForMessage:(ZMChatMsgItemInfo *)message layoutManager:(Class)layoutManagerClass layoutConfig:(ZMIMMessageLayoutConfig *)layoutConfig;

@end
