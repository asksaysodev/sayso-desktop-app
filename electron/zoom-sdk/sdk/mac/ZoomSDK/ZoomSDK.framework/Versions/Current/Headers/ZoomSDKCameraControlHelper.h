/**
 * @file ZoomSDKCameraControlHelper.h
 * @brief Interface for controlling remote camera during Zoom meetings.
 */

#import <Foundation/Foundation.h>
#import <ZoomSDK/ZoomSDKErrors.h>
NS_ASSUME_NONNULL_BEGIN

/**
 * @class ZoomSDKCameraControlHelper
 * @brief Provides APIs to control the remote camera, including pan, tilt, and zoom operations.
 */
@interface ZoomSDKCameraControlHelper : NSObject
/**
 * @brief  Gets the controlled user ID.
 * @return If the function succeeds, it will return the user ID. Otherwise return 0.
 */
- (unsigned int)getUserId;

/**
 * @brief  Determine whether the camera can be controlled or not.
 * @return YES means that the camera can be controlled.
 */
- (BOOL)canControlCamera;

/**
 * @brief Request to control remote camera.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise the function fails.
 */
- (ZoomSDKError)requestControlRemoteCamera;

/**
 * @brief Give up control of the remote camera.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise the function fails.
 */
- (ZoomSDKError)giveUpControlRemoteCamera;

/**
 * @brief Turn the camera to the left.
 * @param range Rotation range, between 10 to 100.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise the function fails.
 */
- (ZoomSDKError)turnLeft:(int)range;

/**
 * @brief Turn the camera to the right.
 * @param range Rotation range, between 10 to 100.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise the function fails.
 */
- (ZoomSDKError)turnRight:(int)range;

/**
 * @brief Turn the camera up.
 * @param range Rotation range, between 10 to 100.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise the function fails.
 */
- (ZoomSDKError)turnUp:(int)range;

/**
 * @brief Turn the camera down.
 * @param range Rotation range, between 10 to 100.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise the function fails.
 */
- (ZoomSDKError)turnDown:(int)range;

/**
 * @brief Zoom the camera in.
 * @param range Rotation range, between 10 to 100.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise the function fails.
 */
- (ZoomSDKError)zoomIn:(int)range;

/**
 * @brief Zoom the camera out.
 * @param range Rotation range, between 10 to 100.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise the function fails.
 */
- (ZoomSDKError)zoomOut:(int)range;
@end

NS_ASSUME_NONNULL_END
