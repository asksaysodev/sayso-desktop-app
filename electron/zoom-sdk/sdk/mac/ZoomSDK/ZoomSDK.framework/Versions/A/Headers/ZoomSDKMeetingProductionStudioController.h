/**
 * @file ZoomSDKMeetingProductionStudioController.h
 * @brief Interface for managing Zoom Meeting Production Studio mode and sending video/audio data.
 */

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN
/**
 * @class ZoomSDKPSSender
 * @brief Provides APIs to send video and audio data to the production studio.
 */
@interface ZoomSDKPSSender : NSObject

/**
 * @brief Send video to production studio.
 * @param frameBuffer The data to send.
 * @param width The width of the data to send.
 * @param height The height of the data to send.
 * @param frameLength The length of the data to send.
 * @param format The format of frame data.
 * @return If the function succeeds, it will return the @c ZoomSDKError_Success, otherwise failed.
 * @note The width and height of the video data sent are same with the width and height in \link ZoomSDKPSVideoSourceCapability \endlink, which is passed in when calling \link startPSMode \endlink. If they are not same, returns ZoomSDKError_InvalidParameter.
 */
- (ZoomSDKError)sendVideoFrame:(char*)frameBuffer width:(unsigned int)width height:(unsigned int)height frameLength:(unsigned int)frameLength format:(ZoomSDKFrameDataFormat)format;

/**
 * @brief Send audio to production studio.
 * @param data The audio data's address.
 * @param length The audio data's length, in even numbers.
 * @param rate The audio data's sampling rate. 
 * @param channel The channel type, default is ZoomSDKAudioChannel_Mono.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise the function fails.
 */
- (ZoomSDKError)sendAudio:(char*)data dataLength:(unsigned int)length sampleRate:(int)rate channel:(ZoomSDKAudioChannel)channel;

@end


/**
 * @class ZoomSDKPSVideoSourceCapability
 * @brief Describes video source capabilities for production studio mode.
 */
@interface ZoomSDKPSVideoSourceCapability : NSObject

/**
 * @brief The width of send video data.
 */
@property (nonatomic, assign) int width;

/**
 * @brief The height of send video data.
 */
@property (nonatomic, assign) int height;

/**
 * @brief The format of send video data.
 */
@property (nonatomic, assign) ZoomSDKFrameDataFormat format;
@end


/**
 * @protocol ZoomSDKMeetingProductionStudioControllerDelegate
 * @brief Delegate to receive callbacks related to production studio mode events.
 */
@protocol ZoomSDKMeetingProductionStudioControllerDelegate <NSObject>

/**
 * @brief Notify the result of starting production studio mode.
 * @param success YES  if PS mode is started successfully. Otherwise, return NO.
 */
- (void)onStartPSModeResult:(BOOL)success;

/**
 * @brief This callback is received when production studio starts.
 * @param sender The object of send video or audio data.
 */
- (void)onStartSend:(ZoomSDKPSSender* _Nullable)sender;

/**
 * @brief This callback is received when production studio stops.
 */
- (void)onStopSend;

@optional
/**
 * @brief This callback is received when the production studio user starts and stops.
 * @param userId the production studio user's user ID.
 * @param bStart YES means that means the production studio user starts sending video and audio. NO means the production studio user stopped sending video and audio.
 */
- (void)onPSUserStatusChanged:(unsigned int)userId isStart:(BOOL)start;
@end



/**
 * @class ZoomSDKMeetingProductionStudioController
 * @brief Provides control and status APIs for Zoom meeting production studio mode.
 */
@interface ZoomSDKMeetingProductionStudioController : NSObject

@property(nonatomic, assign, nullable)id<ZoomSDKMeetingProductionStudioControllerDelegate> delegate;

/**
 * @brief Returns if production studio mode is supported.
 * @return YES means production studio mode is supported.
 */
- (BOOL)isSupportPSMode;

/**
 * @brief Determine if you have permission to start production studio mode. Only the host or cohost can start production studio mode.
 * @return If you have permission to start production studio mode, return YES. Otherwise return NO.
 */
- (BOOL)canStartPSMode;

/**
 * @brief Start production studio mode. After starting, send data to video and audio publication to production studio through the \link ZoomSDKPSSender \endlink interface.
 * @param capability Format description of the video data sent.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise the function fails.
 */
- (ZoomSDKError)startPSMode:(ZoomSDKPSVideoSourceCapability*)capability;

/**
 * @brief Stop production studio mode.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise the function fails.
 */
- (ZoomSDKError)stopPSMode;

/**
 * @brief Determine if production studio mode has started.
 * @return YES means production studio mode is started. Otherwise not.
 */
- (BOOL)isPSModeStarted;

/**
 * @brief Returns the current production studio user's user ID.
 * @return The user ID of current production studio user.
 */
- (unsigned int)getPSUserID;
@end

NS_ASSUME_NONNULL_END
