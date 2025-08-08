//
//  ZMImageMessageCellView.h
//  ZCommonUI
//
//  Created by groot.ding on 27/07/2020.
//  Copyright © 2020 zoom. All rights reserved.
//

#import <zChatComponent/ZMBubbleMessageCellView.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMAnimationAndErrorView,ZMGIFImageView;

@interface ZMImageMessageCellView : ZMBubbleMessageCellView

@property (nonatomic,retain) ZMGIFImageView *imageContentView;

@property (nonatomic,retain) ZMAnimationAndErrorView *loadingView;

@end

NS_ASSUME_NONNULL_END
