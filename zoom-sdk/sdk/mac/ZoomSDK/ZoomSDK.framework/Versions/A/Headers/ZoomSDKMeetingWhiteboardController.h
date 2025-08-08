/**
 * @file ZoomSDKMeetingWhiteboardController.h
 * @brief Defines the interface for controlling and managing the whiteboard feature in Zoom meetings.
 *
 * This file declares the ZoomSDKMeetingWhiteboardController class, which provides APIs for
 * managing whiteboard sharing, creation, view display, and user permissions during meetings.
 * It also defines the ZoomSDKMeetingWhiteboardControllerDelegate protocol to receive
 * whiteboard status updates and settings changes.
 *
 * This feature enhances collaboration by enabling dynamic whiteboard interaction within
 * Zoom meetings.
 */

#import <Foundation/Foundation.h>
#import <ZoomSDK/ZoomSDKErrors.h>

NS_ASSUME_NONNULL_BEGIN
/**
 * @protocol ZoomSDKMeetingWhiteboardControllerDelegate
 * @brief Delegate protocol to receive whiteboard status and settings changes.
 */
@protocol ZoomSDKMeetingWhiteboardControllerDelegate <NSObject>

/**
 * @brief Whiteboard status changed callback. Use this function to inform the user that the whiteboard has been started or stopped, and all users in the meeting can get the event.
 * @param status Specify current whiteboard status.
 */
- (void)onWhiteboardStatusChanged:(ZoomSDKWhiteboardStatus)status;

/**
 * @brief Callback event of whiteboard setting type changed.
 * @param shareOption Who can share their whiteboard.
 * @param createOption Who can create a new whiteboard.
 * @param bEnable Whether to enable the participants' ability to create a whiteboard without a host in the meeting.
 */
- (void)onWhiteboardSettingsChanged:(ZoomSDKWhiteboardShareOption)shareOption createOption:(ZoomSDKWhiteboardCreateOption)createOption enable:(BOOL)bEnable;

@end


/**
 * @class ZoomSDKMeetingWhiteboardController
 * @brief Controller to manage whiteboard functionality during Zoom meetings.
 *
 * This class provides interfaces to control whiteboard display, settings,
 * permissions, and status in a meeting environment.
 */
@interface ZoomSDKMeetingWhiteboardController : NSObject
@property(nonatomic,assign,nullable) id<ZoomSDKMeetingWhiteboardControllerDelegate> delegate;

/**
 * @brief Set the whiteboard web view and dashboard web view owner window.
 * @param window Specify the owner window. If the window is nil, the desktop window will be the owner window.
 * @return If the function succeeds, it will return @c ZoomSDKError_Success.
 * @warning This function should be called before call ShowDashboardView. Call this function before calling showDashboardView. Call this function when receiving the meeting status ZoomSDKMeetingStatus_InMeeting. Valid only for user custom interface mode.
 */
- (ZoomSDKError)setParentWindow:(NSWindow *)window;

/**
 * @brief Determine whether the current meeting supports the whiteboard or not.
 * @return YES indicates to support.
 */
- (BOOL)isSupportWhiteBoard;

/**
 * @brief Determine whether the current meeting can start sharing the whiteboard.
 * @param reason The reason no one can start sharing the whiteboard.
 * @return YES indicates you can start sharing the whiteboard.
 */
- (BOOL)canStartShareWhiteboard:(ZoomSDKCannotShareReasonType*)reason;

/**
 * @brief Show the dashboard web view window.
 * @param point The original point to display dashboard web view window. If the point is (-1,-1) the window positon will be in the center of the screen where the owner window is located.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise the function fails.
 */
- (ZoomSDKError)showDashboardView:(NSPoint)point;

/**
 * @brief Set the dashboard web view window size.
 * @param size The size of display dashboard web view window. If the size is (0,0) the window size will be default size.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise the function fails.
 */
- (ZoomSDKError)setDashboardViewSize:(NSSize)size;

/**
 * @brief Set the whiteboard web view window position.
 * @param point The original point to display dashboard web view window. If the point is (-1,-1) the window positon will be in the center of the screen where the owner window is located.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise the function fails.
 */
- (ZoomSDKError)setWhiteboardViewPos:(NSPoint)point;

/**
 * @brief Set the whiteboard web view window size.
 * @param size The size of display whiteboard web view window. If the size is (0,0) the window size will be default size.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise the function fails.
 */
- (ZoomSDKError)setWhiteboardViewSize:(NSSize)size;
/**
 * @brief Set the option for who can share a whiteboard.
 * @param option New setting for who can share a whiteboard.
 * @return If the function succeeds, it will return ZoomSDKError_Success.
 */
- (ZoomSDKError)setWhiteboardShareOption:(ZoomSDKWhiteboardShareOption)option;

/**
 * @brief Get the option for who can share whiteboards.
 * @return The return value is the setting option for who can share a whiteboard.
 */
- (ZoomSDKWhiteboardShareOption)getWhiteboardShareOption;

/**
 * @brief Set the option for whiteboard who can initiate a new whiteboard.
 * @param option Setting option for who can initiate a new whiteboard.
 * @return If the function succeeds, it will return ZoomSDKError_Success.
 */
- (ZoomSDKError)setWhiteboardCreateOption:(ZoomSDKWhiteboardCreateOption)option;

/**
 * @brief Get the option for whiteboard who can initiate a new whiteboard.
 * @return The return value is the setting option for whiteboard who can initiate a new whiteboard.
 */
- (ZoomSDKWhiteboardCreateOption)getWhiteboardCreateOption;

/**
 * @brief Enable the participants create a new whiteboard without the host in the meeting.
 * @param bEnable YES indicates to enable. NO is not.
 * @return If the function succeeds, it will return ZoomSDKError_Success.
 */
- (ZoomSDKError)enableParticipantsCreateWithoutHost:(BOOL)bEnable;

/**
 * @brief Determine whether to enable the participants create a new whiteboard without the host in the meeting.
 * @return YES indicates that they have these permissions.
 */
- (BOOL)isParticipantsCreateWithoutHostEnabled;

/**
 * @brief Query if other user is sharing a whiteboard.
 * @return YES means that they are sharing, NO means that they are not.
 */
- (BOOL)isOtherSharingWhiteboard;

/**
 * @brief if the current user is successfully sharing a whiteboard.
 * @return YES means that they are sharing, NO means that they are not.
 */
- (BOOL)isSharingWhiteboardOut;
@end

NS_ASSUME_NONNULL_END
