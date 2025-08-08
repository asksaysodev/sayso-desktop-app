/**
 * @file ZoomSDKMeetingIndicatorController.h
 * @brief Interfaces for managing meeting indicator items in Zoom SDK.
 */

#import <Foundation/Foundation.h>
#import <ZoomSDK/ZoomSDKErrors.h>

NS_ASSUME_NONNULL_BEGIN
@class ZoomSDKMeetingIndicatorHandle;
/**
 * @protocol ZoomSDKMeetingIndicatorControllerDelegate
 * @brief Protocol to receive callbacks about meeting indicator lifecycle events.
 */
@protocol ZoomSDKMeetingIndicatorControllerDelegate <NSObject>
/**
 * @brief When a new indicator item is received.
 * @param handler The handle to show or hide the indicator panel.
 */
- (void)onIndicatorItemReceived:(ZoomSDKMeetingIndicatorHandle *)handler;

/**
 * @brief When the indicator item is removed.
 * @param handler The handle will be removed. Do not use it any more.
 */
- (void)onIndicatorItemRemoved:(ZoomSDKMeetingIndicatorHandle *)handler;
@end

/**
 * @class ZoomSDKMeetingIndicatorHandle
 * @brief A handle that represents a meeting indicator item and allows control of its visibility.
 */
@interface ZoomSDKMeetingIndicatorHandle : NSObject

/**
 * @brief The indicator item ID.
 */
@property(nonatomic, copy, readonly, nullable) NSString* indicatorItemId;
/**
 * @brief The indicator name.
 */
@property(nonatomic, copy, readonly, nullable) NSString* indicatorName;
/**
 * @brief The indicator icon URL.
 */
@property(nonatomic, copy, readonly, nullable) NSString* indicatorIcon;

/**
 * @brief Show the indicator panel window.
 * @param point The original point to display indicator panel.
 * @param parentWindow Show the indicator panel's dynamic notice in this window.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)showIndicatorPanel:(NSPoint)point parentWindow:(NSWindow*)parentWindow;

/**
 * @brief Hide the indicator panel window.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)hideIndicatorPanel;
@end


/**
 * @class ZoomSDKMeetingIndicatorController
 * @brief Controller that manages meeting indicator items and dispatches updates via delegate.
 */
@interface ZoomSDKMeetingIndicatorController : NSObject
/**
 * @brief Set or get the delegate to receive indicator item callbacks.
 */
@property (assign, nonatomic, nullable) id<ZoomSDKMeetingIndicatorControllerDelegate> delegate;
@end

NS_ASSUME_NONNULL_END
