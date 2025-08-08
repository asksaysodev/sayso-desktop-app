//
//  ZMPromptMessageCellView.h
//  ZCommonUI
//
//  Created by groot.ding on 28/07/2020.
//  Copyright © 2020 zoom. All rights reserved.
//

#import <zChatComponent/ZMBaseMessageCellView.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMChatMsgItemTextView;

@interface ZMPromptMessageCellView : ZMBaseMessageCellView

@property (nonatomic,retain) ZMChatMsgItemTextView *promptTextView;

@end

NS_ASSUME_NONNULL_END
