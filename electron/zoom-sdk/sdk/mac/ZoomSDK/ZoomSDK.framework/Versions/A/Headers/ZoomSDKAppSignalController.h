/**
 * @file ZoomSDKAppSignalController.h
 * @brief Interface for controlling application signal and AI Companion panel UI in Zoom SDK.
 */


#import <Foundation/Foundation.h>
#import <ZoomSDK/ZoomSDKErrors.h>

NS_ASSUME_NONNULL_BEGIN
/**
 * @class ZoomSDKAppSignalController
 * @brief A controller that manages the display of App Signal and AI Companion panels.
 */
@interface ZoomSDKAppSignalController : NSObject

/**
 * @brief Show app signal panel.
 * @param point The original point to display app signal panel.
 * @param parentWindow The parent window to locate the application signal panel.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)showAANPanel:(NSPoint)point parentWindow:(NSWindow*)parentWindow;

/**
 * @brief Close app signal panel.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)hideAANPanel;

/**
 * @brief Show the dynamic notice for AI Companion panel window.
 * @param point The original point to display AICompanion panel.
 * @param parentWindow Show the AI companion panel's dynamic notice in this window.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)showDynamicNoticeForAICompanionPanel:(NSPoint)point parentWindow:(NSWindow*)parentWindow;

/**
 * @brief Hide the dynamic notice for AI Companion panel window.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)hideDynamicNoticeForAICompanionPanel;
@end

NS_ASSUME_NONNULL_END
