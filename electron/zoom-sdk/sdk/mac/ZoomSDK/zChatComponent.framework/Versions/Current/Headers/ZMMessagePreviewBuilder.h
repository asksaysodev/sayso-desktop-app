//
//  ZMMessagePreviewBuilder.h
//  zChatComponent
//
//  Created by Cyan.Yang on 2022/10/24.
//

#import <Foundation/Foundation.h>

#import <zChatComponent/ZMMessagePreviewContext.h>
#import <zChatComponent/ZMMessagePreview.h>

@class ZMMessageAdapter;

NS_ASSUME_NONNULL_BEGIN

@interface ZMMessagePreviewBuilder : NSObject

@property (nonatomic, strong) ZMMessagePreviewContext *context;

- (nullable ZMMessagePreview *)buildPreviewForMessage:(ZMMessageAdapter *)message;

@end

NS_ASSUME_NONNULL_END
