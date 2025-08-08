//
//  ZMMailMessageCardView.h
//  ChatUI
//
//  Created by Groot Ding on 2022/6/23.
//  Copyright © 2022 Zoom. All rights reserved.
//

#import <ZoomKit/ZoomKit.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMMailMessageCardView;
@protocol ZMMailMessageCardViewDelegate <NSObject>

@optional

- (void)previewEmail:(ZMMailMessageCardView *)cardView htmlPath:(NSString *)htmlPath;

@end

const CGFloat ZMMaxMailCardWidth =400;
const CGFloat ZMMaxMailCardHeight =197;

@class ZMShareEmailInfo;

@interface ZMMailMessageCardView : ZMBaseView

@property (nonatomic,retain) ZMShareEmailInfo *emailInfo;

@property (nonatomic,weak) id <ZMMailMessageCardViewDelegate> delegate;

- (void)updateUI;

- (void)sizeToFit;

@end

NS_ASSUME_NONNULL_END
