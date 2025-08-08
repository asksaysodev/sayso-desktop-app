//
//  ZMImageOperateModule.h
//  ChatUI
//
//  Created by groot Ding on 2021/11/12.
//  Copyright © 2021 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <ZoomKit/ZMCallbackImpl.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMImageOperateModule : NSObject

+ (instancetype)sharedInstance;

- (nonnull NSString *)compressImage:(nonnull NSString *)imagePath toFile:(nullable NSString *)toFile compressSize:(NSSize)compressSize identifier:(NSUInteger)identifier completion:(nullable ZMCallbackBlock)completion;

- (void)removeCallbackByImagePath:(nonnull NSString *)toFilePath identifier:(NSUInteger)identifier;

@end

NS_ASSUME_NONNULL_END
