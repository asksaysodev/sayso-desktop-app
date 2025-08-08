//
//  ZMPSAsyncRecordingStatusProtocol.h
//  ZoomSetting
//
//  Created by Michael Lin on 2024/1/15.
//

#ifndef ZMPSAsyncRecordingStatusProtocol_h
#define ZMPSAsyncRecordingStatusProtocol_h

@class ZMPSActionToken;
@protocol ZMPSAsyncRecordingStatusProtocol <NSObject>

@optional
- (void)psOnAsyncRecordingActionTokenReceived:(ZMPSActionToken *_Nullable)token;
- (void)psRecordingFinishedWithRecordingID:(NSUInteger)recordingID;
- (void)psRecordingStartedWithRecordingID:(NSUInteger)recordingID;
- (void)psRecordingCanceledWithRecordingID:(NSUInteger)recordingID;
- (void)psRecordingSegmentGeneratedWithRecordingID:(NSUInteger)recordingID duration:(NSUInteger)duration;
- (void)psRecordingPausedWithRecordingID:(NSUInteger)recordingID;
- (void)psRecordingResumedWithRecordingID:(NSUInteger)recordingID;
- (void)psRecordingRestartedWithRecordingID:(NSUInteger)recordingID;
- (void)psUploadFinishedWithRecordingID:(NSUInteger)recordingID webRecordingID:(nullable NSString *)webRecordingID status:(NSInteger)status errorReason:(NSInteger)errorReason retryable:(BOOL)retryable;
- (void)psRecordingCreatedOnWebWithRecordingID:(NSUInteger)recordingID;
- (void)psChecedLimitationFinishedWithSuccess:(BOOL)success canCreate:(BOOL)canCreate errorCode:(NSUInteger)errorCode maxDuration:(NSInteger)maxDuration canNotCreateMsg:(nullable NSString *)canNotCreateMsg moreURL:(nullable NSString *)moreURL upgradeURL:(nullable NSString *)upgradeURL;
- (void)psOnRecvCustomMessage:(nullable NSString *)message;
- (void)psOnAsyncRecordingResolutionChanged;
- (void)psOnThirdPartyVirtualCameraConfigChanged;

@end

#endif /* ZMPSAsyncRecordingStatusProtocol_h */
