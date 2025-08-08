/**
 * @file ZoomSDKRawDataVideoSourceController.h
 * @brief Interfaces for managing virtual video source and raw video preprocessing in Zoom SDK.
 */

#import <Foundation/Foundation.h>
#import <ZoomSDK/ZoomSDKRawDataController.h>

NS_ASSUME_NONNULL_BEGIN
/**
 * @class ZoomSDKYUVProcessDataI420
 * @brief Represents I420 YUV video frame data.
 */
@interface ZoomSDKYUVProcessDataI420 : NSObject
/**
 * @brief Get the width of this data.
 * @return If the function succeeds, it will return the width of this data.
 */
- (unsigned int)getWidth;

/**
 * @brief Get the height of this data.
 * @return If the function succeeds, it will return the height of this data.
 */
- (unsigned int)getHeight;

/**
 * @brief Get the Y buffer of this data.
 * @return If the function succeeds, it will return the Y buffer of this data.
 */
- (char*_Nullable)getYBuffer:(unsigned int)lineNum;

/**
 * @brief Get the U buffer of this data.
 * @return If the function succeeds, it will return the U buffer of this data.
 */
- (char*_Nullable)getUBuffer:(unsigned int)lineNum;

/**
 * @brief Get the V buffer of this data.
 * @return If the function succeeds, it will return the V buffer of this data.
 */
- (char*_Nullable)getVBuffer:(unsigned int)lineNum;

/**
 * @brief Get the V stride of this data.
 * @return If the function succeeds, it will return the V stride of this data.
 */
- (unsigned int)getYStride;

/**
 * @brief Get the U stride of this data.
 * @return If the function succeeds, it will return the U stride of this data.
 */
- (unsigned int)getUStride;

/**
 * @brief Get the V stride of this data.
 * @return If the function succeeds, it will return the V stride of this data.
 */
- (unsigned int)getVStride;

/**
 * @brief Get the rotation of this data.
 * @return If the function succeeds, it will return the rotation of this data.
 */
- (unsigned int)getRotation;

/**
 * @brief Get if this data is limited I420.
 * @return If it is limited I420, it will return YES. Otherwise NO.
 */
- (BOOL)isLimitedI420;
@end


/**
 * @class ZoomSDKVideoCapabilityItem
 * @brief Represents video capability information.
 */
@interface ZoomSDKVideoCapabilityItem : NSObject
/**
 * @brief Get the width of this data.
 * @return If the function succeeds, it will return the width of this data.
 */
- (unsigned int)getWidth;

/**
 * @brief Get the height of this data.
 * @return If the function succeeds, it will return the height of this data.
 */
- (unsigned int)getHeight;

/**
 * @brief Get the frame of this data.
 * @return If the function succeeds, it will return the frame of this data.
 */
- (unsigned int)getFrame;
@end


/**
 * @class ZoomSDKRawDataSender
 * @brief Send raw video frame data into the meeting.
 */
@interface ZoomSDKRawDataSender : NSObject

/**
 * @brief Send raw video data in meeting.
 * @param data The data to send.
 * @param width The width of the data to send.
 * @param height The height of the data to send.
 * @param length The length of the data to send.
 * @param rotation The rotation of the data to send.
 * @param format The format of frame data.
 */
- (void)sendRawData:(char*)data width:(unsigned int)width height:(unsigned int)height dataLength:(unsigned int)length rotation:(ZoomSDKLocalVideoDeviceRotation)rotation format:(ZoomSDKFrameDataFormat)format;
@end

/**
 * @protocol ZoomSDKVirtualVideoSourceDelegate
 * @brief Callback interface for managing virtual video source.
 */
@protocol ZoomSDKVirtualVideoSourceDelegate <NSObject>
/**
 * @brief Callback when the virtual video source is initialized.
 * @param sender The sender object used to send raw video data.
 * @param capabilityList List of supported video capabilities.
 * @param suggestCap The suggested video capability for the current device.
 */
- (void)onInitialize:(ZoomSDKRawDataSender*_Nullable)sender supportedCapabilityList:(NSArray*_Nullable)capabilityList suggestCapability:(ZoomSDKVideoCapabilityItem*_Nullable)suggestCap;

/**
 * @brief Callback when the virtual video source property changes.
 * @param supportedCapabilityList List of updated supported video capabilities.
 * @param suggestCap The updated suggested video capability.
 */
- (void)onPropertyChange:(NSArray*_Nullable)supportedCapabilityList suggestCapability:(ZoomSDKVideoCapabilityItem*_Nullable)suggestCap;

/**
 * @brief Callback to indicate the start of video data sending.
 */
- (void)onStartSend;

/**
 * @brief Callback to indicate the stop of video data sending.
 */
- (void)onStopSend;

/**
 * @brief Callback when the virtual video source is uninitialized.
 */
- (void)onUninitialize;
@end

/**
 * @protocol ZoomSDKRawDataSendDelegate
 * @brief Callback for preprocessing raw video frames before rendering or transmission.
 */
@protocol ZoomSDKRawDataSendDelegate <NSObject>
/**
 * @brief Notify the delegate to preprocess a raw data.
 * @param data The raw YUV data.
 */
- (void)onPreProcessRawData:(ZoomSDKYUVProcessDataI420*_Nullable)data;
@end

@interface ZoomSDKRawDataVideoSourceController : NSObject
/**
 * @brief Register the delegate of raw data preprocessor.
 * @param delegate The delegate to receive callback.
 * @return If the function succeeds, it will return ZoomSDKError_Success.
 */
- (ZoomSDKError)registerRawDataPreProcessor:(id<ZoomSDKRawDataSendDelegate>)delegate;

/**
 * @brief unRegister the delegate of raw data preprocessor.
 * @param delegate The delegate to receive callback.
 * @return If the function succeeds, it will return ZoomSDKError_Success.
 */
- (ZoomSDKError)unRegisterRawDataPreProcessor:(id<ZoomSDKRawDataSendDelegate>)delegate;

/**
 * @brief Set the delegate of virtual video source.
 * @param videoSource The delegate to receive callback.
 * @return If the function succeeds, it will return ZoomSDKError_Success.
 */
- (ZoomSDKError)setExternalVideoSource:(id <ZoomSDKVirtualVideoSourceDelegate>)videoSource;
@end
NS_ASSUME_NONNULL_END
