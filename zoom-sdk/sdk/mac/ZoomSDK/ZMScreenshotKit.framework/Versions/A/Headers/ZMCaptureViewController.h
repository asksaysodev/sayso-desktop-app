//
//  ZMCaptureViewController.h
//  ZMScreenshotKit
//
//  Created by Javenlee.Li on 2024/8/6.
//  Copyright © 2024 zoom. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "ZMSCImageModel.h"

NS_ASSUME_NONNULL_BEGIN

@protocol ZMCaptureViewDelegate <NSObject>
@optional
- (void)captureFinish:(NSData *)imageData;
- (void)downloadCapture:(NSData *)imageData;
- (void)showCaptureToolPalette:(NSViewController *)viewCtrl;
- (void)captureCancel;
- (NSString *)captureTitle;
@end

@interface ZMCaptureViewController : NSViewController

@property (nonatomic, assign) BOOL                           drawSelectInfo;
@property (nonatomic, strong) NSMutableArray                *windowInfo;
@property (nonatomic, assign, nullable) id <ZMCaptureViewDelegate>     delegate;
- (instancetype)initWithModel:(ZMSCImageModel *)model;
- (void)startCaptureModel;
- (NSData *)getSelectedCaptureImage;
- (void)closePopup;
- (void)cleanUp;
@end

NS_ASSUME_NONNULL_END
