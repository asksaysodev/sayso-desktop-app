//
//  ZMPSAsyncRecording.h
//  ZoomClips
//
//  Created by Michael Lin on 2023/1/5.
//

#import <Foundation/Foundation.h>
#ifdef __cplusplus
#import <ZoomSetting/ZMPSDefine.h>
#endif
#import <ZoomSetting/ZMPSObject.h>
#import <ZoomSetting/ZMPSMediaClient.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMPSRootCanvas, ZMPSCanvas, ZMPSRender, ZMClipsSharingSendingParam;
@class ZMPSActionToken, ZMPSRecordingStatistics;
@interface ZMPSAsyncRecording : ZMPSObject

@property (nonatomic, assign, readonly) NSUInteger recordingID;

- (void)configWithSize:(NSSize)size title:(nullable NSString *)title webRecordInfo:(nullable NSString *)webRecordInfo layoutInfoStr:(nullable NSString *)layoutInfoStr;
- (nullable ZMPSRootCanvas *)rootCanvas;
- (void)startNote;
- (void)restartRecording;
- (void)stopNote;
- (void)cancelNote;
- (void)pauseNote;
- (void)resumeNote;
- (void)startPreviewWithRender:(ZMPSRender *)render;
- (void)startPreviewBeforeStopWithRender:(ZMPSRender *)render;
- (void)stopPreviewWithRender:(ZMPSRender *)render;
- (void)stopPreview;
- (BOOL)isPreviewingWithRender:(ZMPSRender *)render;
- (BOOL)isPreviewing;

- (nullable NSString *)webRecordingID;
- (nullable ZMPSRecordingStatistics *)recordingStatistics;

- (void)bindSoundWithDefaultMic;
- (void)unbindSoundWithMic;
- (BOOL)isSoundWithMicBound;
- (void)muteSoundWithMic;
- (void)unmuteSoundWithMic;
- (BOOL)isSoundWithMicMuted;

- (void)bindVideoWithDefaultDeviceWithCanvas:(ZMPSCanvas *)canvas immersive:(BOOL)immersive expectedResolutionType:(ZMPSResolutionType)resolutionType;
- (void)unbindVideoWithDefaultDeviceWithCanvas:(ZMPSCanvas *)canvas;
- (BOOL)isVideoWithDeviceBound:(ZMPSCanvas *)canvas;

#ifdef __cplusplus
- (void)bindShareWithDefaultCaptureWithCanvas:(ZMPSCanvas *)canvas captureParam:(const ps::PSCaptureParam&)captureParam;
#endif
- (void)unbindShareWithDefaultCaptureWithCanvas:(ZMPSCanvas *)canvas;

- (NSInteger)getRecordingStreamDuration;

- (void)retryUpload;
- (void)cancelUpload;

- (void)setLayoutInfo:(NSString *)info;

@end

NS_ASSUME_NONNULL_END
