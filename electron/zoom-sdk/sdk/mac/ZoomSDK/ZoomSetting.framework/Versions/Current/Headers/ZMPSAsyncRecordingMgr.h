//
//  ZMPSAsyncRecordingMgr.h
//  ZoomClips
//
//  Created by Michael Lin on 2023/5/6.
//

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import <ZoomSetting/ZMPSObject.h>
#import <ZoomSetting/ZMPSMediaClient.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, ZMPSVideoBorderShape) {
    ZMPSVideoBorderShapeRectangle,
    ZMPSVideoBorderShapeRoundedRectangle,
    ZMPSVideoBorderShapeCircle,
};

@class ZMPSDevice;
@interface ZMPSAsyncRecordingMgr : ZMPSObject

- (NSUInteger)generateAsyncRecording;
- (void)destoryAsyncRecordingWithID:(NSUInteger)recordingID;
- (void)registerSink;
- (void)unregisterSink;
- (nullable NSString *)clipsCommonConfiguration;
- (void)retryUploadAsyncRecordingWithID:(NSUInteger)recordingID;
- (void)cancelUploadAsyncRecordingWithID:(NSUInteger)recordingID;
- (void)notifyUserUseStart;
- (void)openDefaultVBSetting;
- (void)openPTLibrary;
- (void)setClipsIsRunning:(BOOL)isRunning;
- (BOOL)needCheckAsyncRecordingLimitation;
- (void)queryAsyncRecordingLimitation;

- (BOOL)isThirdPartyVirtualCamera:(ZMPSDevice *)device;

- (void)sendCustomMessageToPT:(NSString *)message;

@property (nonatomic, strong, nullable) NSColor *videoBorderColor;
@property (nonatomic, assign) ZMPSResolutionType recordResolution;
@property (nonatomic, assign) ZMPSVideoBorderShape videoBorderShape;

@end

NS_ASSUME_NONNULL_END
