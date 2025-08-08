/**
 * @file ZoomSDKRenderer.h
 * @brief Interface for rendering and receiving raw video data in Zoom SDK.
 */

#import <Foundation/Foundation.h>
#import <ZoomSDK/ZoomSDKErrors.h>

NS_ASSUME_NONNULL_BEGIN
@class ZoomSDKYUVRawDataI420;

/**
 * @class ZoomSDKYUVRawDataI420
 * @brief Represents raw YUV I420 format data received from subscribed video stream.
 */
@interface ZoomSDKYUVRawDataI420 : NSObject
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
 * @brief Get the Y data.
 * @return If the function succeeds, it will return the Y data.
 */
- (char*_Nullable)getYBuffer;
/**
 * @brief Get the U data.
 * @return If the function succeeds, it will return the U data.
 */
- (char*_Nullable)getUBuffer;
/**
 * @brief Get the V data.
 * @return If the function succeeds, it will return the V data.
 */
- (char*_Nullable)getVBuffer;
/**
 * @brief Get the buffer data.
 * @return If the function succeeds, it will return the buffer data.
 */
- (char*_Nullable)getBuffer;
/**
 * @brief Get video alpha mask data buffer.
 * @return buffer address if alpha data exists. Otherwise nil.
 */
- (char*_Nullable)getAlphaBuffer;
/**
 * @brief Get the buffer length of this data.
 * @return If the function succeeds, it will return the buffer length of this data.
 */
- (unsigned int)getBufferLen;
/**
 * @brief Get the alpha buffer length.
  * @return The length of alpha data.
 */
- (unsigned int)getAlphaBufferLen;
/**
 * @brief Get if this data is limited I420 format.
 * @return If is limited I420 format, it will return YES. Otherwise NO.
 */
- (BOOL)isLimitedI420;
/**
 * @brief Get the stream width of this data.
 * @return If the function succeeds, it will return the stream width of this data.
 */
- (unsigned int)getStreamWidth;
/**
 * @brief Get the stream height of this data.
 * @return If the function succeeds, it will return the stream height of this data.
 */
- (unsigned int)getStreamHeight;
/**
 * @brief Get the rotation of this data.
 * @return If the function succeeds, it will return the rotation of this data.
 */
- (unsigned int)getRotation;
/**
 * @brief Get the source id of this data.
 * @return If the function succeeds, it will return the source id of this data.
 */
- (unsigned int)getSourceID;
/**
 * @brief Get the timestamp of the raw data.
 * @return If the function succeeds, it will return the millisecond timestamp.
 */
- (long long)getTimeStamp;
@end


/**
 * @protocol ZoomSDKRendererDelegate
 * @brief Callback interface for receiving video render events.
 */
@protocol ZoomSDKRendererDelegate <NSObject>
/**
 * @brief Notify if subscribed user's video data becomes available.
 */
- (void)onSubscribedUserDataOn;
/**
 * @brief Notify if subscribed user's video data becomes unavailable.
 */
- (void)onSubscribedUserDataOff;
/**
 * @brief Notify if subscribed user has left.
 * @deprecated This method is no longer used.
 */
- (void)onSubscribedUserLeft DEPRECATED_MSG_ATTRIBUTE("No longer used");
/**
 * @brief Notify if raw data is received for rendering.
 * @param data The rawData status.
 */
- (void)onRawDataReceived:(ZoomSDKYUVRawDataI420*_Nullable)data;
/**
 * @brief Notify if the renderer is being destroyed.
 */
- (void)onRendererBeDestroyed;
@end


/**
 * @class ZoomSDKRenderer
 * @brief Subscribe to raw video or share data and handle rendering.
 */
@interface ZoomSDKRenderer : NSObject
{
    unsigned int                  _subscribeID;
    ZoomSDKRawDataType            _rawDataType;
    ZoomSDKResolution             _resolution;
    id<ZoomSDKRendererDelegate>   _delegate;
}
/**
 * @brief The delegate to receive rendering events.
 */
@property(nonatomic, assign, nullable)id<ZoomSDKRendererDelegate> delegate;

/**
 * @brief Subscribe to receive raw data.
 * @param subscribeID The subscribe ID of the raw data user want to receive. If subscribe video, the subscribeID is user ID. If subscribe the sharing, the subscribeID is share source ID.
 * @param rawDataType The type of raw data user want to receive.
 * @return If the function succeeds, it returns ZoomSDKError_Success.
 */
- (ZoomSDKError)subscribe:(unsigned int)subscribeID rawDataType:(ZoomSDKRawDataType)rawDataType;

/**
 * @brief Unsubscribe to receive raw data.
 * @return If the function succeeds, it will return ZoomSDKError_Success.
 */
- (ZoomSDKError)unSubscribe;

/**
 * @brief Get the type of raw data.
 * @return If the function succeeds, it will return the type of raw data.
 */
- (ZoomSDKRawDataType)getRawDataType;

/**
 * @brief Get the subscribe ID of raw data user is subscribing to.
 * @return If the function succeeds, it returns the subscribe ID.
 */
- (unsigned int)getSubscribeID;

/**
 * @brief Get the resolution of raw data.
 * @return If the function succeeds, it will return the resolution.
 */
- (ZoomSDKResolution)getResolution;

/**
 * @brief Set the resolution of raw data.
 * @param resolution The resolution of raw data user want to receive.
 * @return If the function succeeds, it will return ZoomSDKError_Success.
 */
- (ZoomSDKError)setResolution:(ZoomSDKResolution)resolution;
@end

NS_ASSUME_NONNULL_END
