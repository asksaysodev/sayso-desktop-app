//
//  ZMIMScreenShotMgr.h
//  SaasBeePTUIModule
//
//  Created by john on 04/12/2017.
//  Copyright © 2017 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>

static NSString* const zoomIdentifier = @"us.zoom.xos";

@protocol ZMScreenshotConnectionProtocol <NSObject>

- (void)captureDidSucceed:(NSData *)imageData;
- (void)captureFailed;

@end

typedef NS_ENUM(NSUInteger, ScreenShotItemState) {
    ZM_STATE_UNKNOW = 0,
    ZM_STATE_FAIL,
    ZM_STATE_FINISH,
};

@interface ZMScreenShotItem : NSObject
{
    NSString*   _itemId;
    NSImage*    _image;
    id          _video;
    NSUInteger  _itemState;
}
@property(nonatomic, copy) NSString* itemId;
@property(nonatomic, retain) NSImage*   image;
@property(nonatomic, assign) NSUInteger  itemState;
@end

@protocol ZMScreenshotCallback <NSObject>

@required
- (void)didCaptureScreenshot:(ZMScreenShotItem *)item;
- (NSView*)captureScreenSender;

@optional
- (void)willCaptureScreenshot;
- (BOOL)isMeetingSession;
- (void)meetingChatStartScreenshot:(BOOL)needHideWindow;
- (void)meetingChatScreenshotCompleted:(BOOL)successed;
- (void)captureFailed;
- (void)captureFailedTaskIsRunning;
- (void)captureFailedWithNoPermission;
@end

@interface ZMIMScreenShotMgr : NSObject <ZMScreenshotConnectionProtocol>
{
    NSString*           _identifier;
    
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    NSConnection*       _connection;
#pragma clang diagnostic pop
    
    NSString *          _uuid;
}

+ (ZMIMScreenShotMgr *)screenshotMgr;

- (void)cleanUp;

- (BOOL)startCaptureFromHotkey;
- (void)screenshotWithSystemApp;
- (BOOL)startCapture;
- (BOOL)startCaptureWithCallback:(id <ZMScreenshotCallback>)callback;

- (void)addDefaultCallback:(id <ZMScreenshotCallback>)callback;

- (void)removeCallback:(id <ZMScreenshotCallback>)callback;


@end
