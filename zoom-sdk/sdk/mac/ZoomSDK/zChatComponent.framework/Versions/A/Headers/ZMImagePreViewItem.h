//
//  ZMImagePreViewItem.h
//  FilePreviewDemo
//
//  Created by groot.ding on 2/6/20.
//  Copyright © 2020 groot.ding. All rights reserved.
//

#import <zChatComponent/ZMFilePreViewItem.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMImagePreViewItem : ZMFilePreViewItem

@property (nonatomic,retain,nullable) NSImage *image;
@property (nonatomic,retain,nullable) NSImage *thumbnailImage;
@end

NS_ASSUME_NONNULL_END
