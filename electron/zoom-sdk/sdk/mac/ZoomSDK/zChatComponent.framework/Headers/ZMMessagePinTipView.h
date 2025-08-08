//
//  ZMMessagePinTipView.h
//  ZCommonUI
//
//  Created by groot.ding on 06/10/2020.
//  Copyright © 2020 zoom. All rights reserved.
//



typedef void(^ZMViewPinHistoryClicked)(void);

NS_ASSUME_NONNULL_BEGIN

@class ZMChatMsgItemInfo;

@interface ZMMessagePinTipView : ZMBaseView

@property (nonatomic,retain) ZMChatMsgItemInfo *messageInfo;

@property (nonatomic,assign) BOOL hover;

@property (nonatomic,copy) ZMViewPinHistoryClicked viewPinHistoryBlock;

- (void)updateUI;

@end

NS_ASSUME_NONNULL_END
