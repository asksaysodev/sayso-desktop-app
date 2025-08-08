/**
 * @file ZoomSDKRawDataController.h
 * @brief Interface definitions for managing Zoom SDK raw data.
 *
 * This file provides interfaces for accessing and controlling audio, video,
 * and share raw data streams, including subscription control and rendering integration.
 */


#import <Foundation/Foundation.h>
#import <ZoomSDK/ZoomSDKErrors.h>
#import <ZoomSDK/ZoomSDKRenderer.h>

@class ZoomSDKRawDataVideoSourceController;
@class ZoomSDKRawDataShareSourceController;
@class ZoomSDKRawDataAudioSourceController;

NS_ASSUME_NONNULL_BEGIN
/**
 * @class ZoomSDKAudioRawData
 * @brief Represents audio raw data received from the SDK.
 */
@interface ZoomSDKAudioRawData : NSObject
/**
 * @brief Get if this object can add ref.
 * @return If can add ref, it will return YES. Otherwise NO.
 */
- (BOOL)canAddRef;
/**
 * @brief Add reference for this object, if you doesn't add ref, this object will be released when the callback response ends.
 * @return If the function succeeds, it will return YES. Otherwise NO.
 */
- (BOOL)addRef;
/**
 * @brief Release the object, if you has add ref, remeber to call this api to release when you wantn't to use this object.
 * @return If the function succeeds, it will return reference count of this object.
 */
- (int)releaseData;
/**
 * @brief Get the buffer data.
 * @return If the function succeeds, it will return the buffer data.
 */
- (char*_Nullable)getBuffer;
/**
 * @brief Get the buffer length of this data.
 * @return If the function succeeds, it will return the buffer length of this data.
 */
- (unsigned int)getBufferLen;
/**
 * @brief Get the sample rate of this data.
 * @return If the function succeeds, it will return the sample rate of this data.
 */
- (unsigned int)getSampleRate;
/**
 * @brief Get the channel number of this data.
 * @return If the function succeeds, it will return the channel number of this data.
 */
- (unsigned int)getChannelNum;
/**
 * @brief Get the timestamp of the raw data.
 * @return If the function succeeds, it will return millisecond timestamp.
 */
- (long long)getTimeStamp;
@end

/**
 * @protocol ZoomSDKAudioRawDataDelegate
 * @brief Delegate to handle raw audio data.
 */
@protocol ZoomSDKAudioRawDataDelegate <NSObject>
/**
 * @brief Notify to receive the mixed audio raw data.
 * @param data The received audio raw data.
 */
- (void)onMixedAudioRawDataReceived:(ZoomSDKAudioRawData*_Nullable)data;
/**
 * @brief Notify to receive the one way audio raw data.
 * @param data The received audio raw data.
 * @param nodeID The user id of received user's data.
 * @deprecated Use \link ZoomSDKAudioRawDataDelegate::onOneWayAudioRawDataReceived:userID: \endlink instead.
 */
- (void)onOneWayAudioRawDataReceived:(ZoomSDKAudioRawData*_Nullable)data nodeID:(unsigned int)nodeID DEPRECATED_MSG_ATTRIBUTE("Use -onOneWayAudioRawDataReceived: userID: instead");
/**
 * @brief Notify to receive the one way audio raw data.
 * @param data The received audio raw data.
 * @param userID The user id of received user's data.
 */
- (void)onOneWayAudioRawDataReceived:(ZoomSDKAudioRawData*_Nullable)data userID:(unsigned int)userID;
/**
 * @brief Notify to receive the share audio raw data.
 * @param data The received audio raw data.
 */
- (void)onShareAudioRawDataReceived:(ZoomSDKAudioRawData*_Nullable)data DEPRECATED_MSG_ATTRIBUTE("Use -onShareAudioRawDataReceived: userID: instead");
/**
 * @brief Notify to receive the share audio raw data.
 * @param data The received audio raw data.
 * @param userID The user id of received user's data.
 */
- (void)onShareAudioRawDataReceived:(ZoomSDKAudioRawData*_Nullable)data userID:(unsigned int)userID;
/**
 * @brief Invoked when individual interpreter's raw audio data received.
 * @param data Raw audio data.
 * @param languageName The interpreter language name of the audio raw data.
 */
- (void)onOneWayInterpreterAudioRawDataReceived:(ZoomSDKAudioRawData*)data strLanguageName:(NSString*)languageName;
@end


/**
 * @class ZoomSDKAudioRawDataHelper
 * @brief Helper to subscribe or unsubscribe audio raw data.
 */
@interface ZoomSDKAudioRawDataHelper : NSObject
{
    id<ZoomSDKAudioRawDataDelegate> _delegate;
}
/**
 * @brief Delegate for receiving audio raw data events.
 */
@property(nonatomic, assign, nullable)id<ZoomSDKAudioRawDataDelegate> delegate;
/**
 * @brief if audioWithInterpreters is YES, it means that you want to get the audio data of interpreters, otherwise not.
 * @note if audioWithInterpreters is YES, it will cause your local interpreter related functions to be unavailable.
 */
@property(nonatomic, assign)BOOL audioWithInterpreters;
/**
 * @brief Start the audio raw data process.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)subscribe;
/**
 * @brief Stop the audio raw data process.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)unSubscribe;
@end

/**
 * @class ZoomSDKRawDataController
 * @brief Controller for managing raw data helpers and sources.
 */
@interface ZoomSDKRawDataController : NSObject
{
    ZoomSDKAudioRawDataHelper*              _audioRawDataHelper;
    ZoomSDKRawDataVideoSourceController*    _rawDataVideoSourceHelper;
    ZoomSDKRawDataShareSourceController*    _rawDataShareSourceHelper;
    ZoomSDKRawDataAudioSourceController*    _rawDataAudioSourceHelper;
}
/**
 * @brief Query if the user has raw data license.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not ZoomSDKError_NoPermission.
 */
- (ZoomSDKError)hasRawDataLicense;
/**
 * @brief Get the object of ZoomSDKAudioRawDataHelper.
 * @param audioRawDataHelper The point to the object of ZoomSDKAudioRawDataHelper.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)getAudioRawDataHelper:(ZoomSDKAudioRawDataHelper*_Nullable* _Nonnull)audioRawDataHelper;

/**
 * @brief Get the object of ZoomSDKRawDataVideoSourceController.
 * @param videoRawDataSendHelper The point to the object of ZoomSDKRawDataVideoSourceController.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)getRawDataVideoSourceHelper:(ZoomSDKRawDataVideoSourceController*_Nullable*_Nonnull)videoRawDataSendHelper;

/**
 * @brief Get the object of ZoomSDKRawDatShareSourceController.
 * @param shareRawDataSendHelper The point to the object of ZoomSDKRawDatShareSourceController.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)getRawDataShareSourceHelper:(ZoomSDKRawDataShareSourceController*_Nullable*_Nonnull)shareRawDataSendHelper;

/**
 * @brief Get the object of ZoomSDKRawDataAudioSourceController.
 * @param audioRawDataSendHelper The point to the object of ZoomSDKRawDataAudioSourceController.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)getRawDataAudioSourceHelper:(ZoomSDKRawDataAudioSourceController*_Nullable*_Nonnull)audioRawDataSendHelper;

/**
 * @brief Creat the object of ZoomSDKRenderer.
 * @param render The point to the object of ZoomSDKRenderer.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)createRender:(ZoomSDKRenderer*_Nullable*_Nonnull)render;

/**
 * @brief Destory the object of ZoomSDKRenderer.
 * @param render The point to the object of ZoomSDKRenderer.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)destroyRender:(ZoomSDKRenderer*_Nullable)render;
@end
NS_ASSUME_NONNULL_END
