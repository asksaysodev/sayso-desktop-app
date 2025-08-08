/**
 * @file ZoomSDKRawDataShareSourceController.h
 * @brief Interfaces for external share source and audio source in Zoom SDK.
 */

#import <Foundation/Foundation.h>


NS_ASSUME_NONNULL_BEGIN
/**
 * @class ZoomSDKShareSender
 * @brief Send external share content into the meeting.
 */
@interface ZoomSDKShareSender : NSObject

/**
 * @brief Send share raw data in meeting.
 * @param frameBuffer The buffer of the data to send.
 * @param width The width of the data to send.
 * @param height The height of the data to send.
 * @param frameLength The length of the data to send.
 * @param format The format of frame data.
 * @return If the function succeeds, it will return ZoomSDKError_Success.
 */
- (ZoomSDKError)sendShareFrame:(char* _Nonnull)frameBuffer width:(unsigned int)width height:(unsigned int)height frameLength:(unsigned int)frameLength format:(ZoomSDKFrameDataFormat)format;
@end


/**
 * @protocol ZoomSDKShareSourceDelegate
 * @brief Callback interface for external share source.
 */
@protocol ZoomSDKShareSourceDelegate <NSObject>
/**
 * @brief Notify to start send share source.
 * @param sender The object of ZoomSDKShareSender to send share source.
 */
- (void)onStartSend:(ZoomSDKShareSender*_Nullable)sender;

/**
 * @brief Notify to stop send share source.
 */
- (void)onStopSend;
@end

/**
 * @class ZoomSDKShareAudioSender
 * @brief Send external share audio content into the meeting.
 */
@interface ZoomSDKShareAudioSender : NSObject

/**
 * @brief Send audio raw data.
 * @param data The audio data's address.
 * @param length The audio data's length, in even numbers.
 * @param rate The audio data's sampling rate.
 * @param channel The channel type, default is ZoomSDKAudioChannel_Mono.
 * @return If the function succeeds, it will return @c ZoomSDKError_Success.
 * @note Support sample rate: 44100 / 48000 / 50000 / 50400.
 */
- (ZoomSDKError)sendShareAudio:(char* _Nonnull)data dataLength:(unsigned int)length sampleRate:(int)rate audioChannel:(ZoomSDKAudioChannel)channel;
@end

/**
 * @protocol ZoomSDKShareAudioSourceDelegate
 * @brief Callback interface for external share audio source.
 */
@protocol ZoomSDKShareAudioSourceDelegate <NSObject>

/**
 * @brief Callback for audio source to start sending raw data.
 * @param shareAudioSender The object of ZoomSDKShareAudioSender to send share audio source.
 */
- (void)onStartSendAudio:(ZoomSDKShareAudioSender*_Nullable)shareAudioSender;

/**
 * @brief Callback for audio source to stop sending audio raw data.
 */
- (void)onStopSendAudio;
@end


/**
 * @class ZoomSDKRawDataShareSourceController
 * @brief Manage external source for share video and share audio in meeting.
 */
@interface ZoomSDKRawDataShareSourceController : NSObject

/**
 * @brief Start sharing external source.
 * @param shareSource The external source object pointer.
 * @param audioSource The pointer of external audio source object.
 * @return If the function succeeds, it will return @c ZoomSDKError_Success.
 * @note If audioSource is non-null, it indicates sharing user-defined audio at the same time.
 */
- (ZoomSDKError)setExternalShareSource:(id<ZoomSDKShareSourceDelegate> _Nonnull)shareSource shareAudioSource:(id<ZoomSDKShareAudioSourceDelegate> _Nullable)audioSource;

/**
 * @brief Start sharing pure external audio source.
 * @param audioSource The pointer of external audio source object.
 * @return If the function succeeds, it will return @c ZoomSDKError_Success.
 * @note The feature is similar to sharing computer audio, except that the shared content is user-defined.
 */
- (ZoomSDKError)setSharePureAudioSource:(id<ZoomSDKShareAudioSourceDelegate> _Nonnull)audioSource;
@end
NS_ASSUME_NONNULL_END
