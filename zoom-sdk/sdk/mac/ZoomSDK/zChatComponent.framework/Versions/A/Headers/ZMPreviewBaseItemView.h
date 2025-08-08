//
//  ZMPreviewBaseView.h
//  MessageListDemo
//
//  Created by groot.ding on 2/7/20.
//  Copyright © 2020 groot.ding. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <zChatComponent/ZMFilePreViewItem.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMPreviewBaseItemView;

@protocol ZMPreviewItemViewDelegate <NSObject>

- (void)didMouseUpOutside:(ZMPreviewBaseItemView *)itemView;

- (void)didContentScaling:(ZMPreviewBaseItemView *)itemView scaling:(float)scaling;

- (void)didReloadFile:(ZMPreviewBaseItemView *)itemView;

@end

@interface ZMPreviewBaseItemView : NSView

@property (nonatomic,retain,nullable) ZMFilePreViewItem *item;

@property (nonatomic,weak) id <ZMPreviewItemViewDelegate> delegate;

- (void)showContent;
- (void)showDownloading;
- (void)showDownloadFail;

- (void)updateItem;
- (void)updateContent;
- (void)cleanContent;
- (void)contentBoundsDidChange;

- (float)contentCurrentScaling;
- (float)contentMinScaling;

- (void)forceRefresh;
@end

NS_ASSUME_NONNULL_END
