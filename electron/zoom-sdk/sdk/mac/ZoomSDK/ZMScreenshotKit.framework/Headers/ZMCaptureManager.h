//
//  CaptureManager.h
//  ScreenCapture
//
//  Created by Sky on 2017/11/6.
//  Copyright © 2017 ZOOM. All rights reserved.
//

#import <Cocoa/Cocoa.h>

typedef enum : NSUInteger {
    ZMScreenCaptureErrorCancelled,
} ZMScreenCaptureError;

@class ZMCaptureManager;

@protocol ZMCaptureManagerDelegate <NSObject>

@optional
- (void)captureDidSucceed:(NSData *)imageData;
- (void)captureFailedWithError:(NSError *)error;

@end

@interface ZMCaptureManager : NSObject

@property (nonatomic, weak) id<ZMCaptureManagerDelegate> delegate;

+ (instancetype)sharedManager;
- (void)startCapture;

@end
