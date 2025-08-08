//
//  ZMPreviewImageView.h
//  MessageListDemo
//
//  Created by groot.ding on 2/7/20.
//  Copyright © 2020 groot.ding. All rights reserved.
//

#import <zChatComponent/ZMPreviewBaseItemView.h>
#import <ZoomUnit/ZMGIFImageView.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMPreviewImageItemView : ZMPreviewBaseItemView

@property (nonatomic,retain) ZMGIFImageView *imageView;

@property (nonatomic,assign) NSRect imageRect;

- (void)rotateLeft;
- (void)rotateRight;
- (void)resetToOriginalOrientation;

- (void)updateSliderForAnnotationMode:(BOOL)isAnnotationMode;


@end

NS_ASSUME_NONNULL_END
