/**
 * @file ZoomSDKMeetingRemoteSupportController.h
 * @brief Zoom SDK meeting remote support controller interface.
 *
 * This file defines the interfaces for managing remote support features in a meeting,
 * including requesting remote support, handling requests, and controlling remote support screens.
 */

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
NS_ASSUME_NONNULL_BEGIN

/**
 * @class ZoomSDKMeetingRemoteSupportRequestHandler
 * @brief Remote support request handler class.
 */
@interface ZoomSDKMeetingRemoteSupportRequestHandler : NSObject

/**
 * The user ID of the user who requested privilege.
 */
@property(nonatomic, assign, readonly)unsigned int requesterId;

/**
 * The user name of the user who requested privileges.
 */
@property(nonatomic, copy, readonly)NSString* requesterName;

/**
 * The remote support type.
 */
@property(nonatomic, assign, readonly)ZoomSDKRemoteSupportType remoteSupportType;

/**
 * @brief Set the specified application.
 * @param windowID Specify the APP that user wants to be remote support. Available only for ZoomSDKRemoteSupportType_Application.
 * @return If the function succeeds, it will return ZoomSDKError_Success.
 */
- (ZoomSDKError)setRemoteSupportApplicationView:(CGWindowID)windowID;

/**
 * @brief Allows the user to remote support .
 * @return If the function succeeds, it will return ZoomSDKError_Success.
 */
- (ZoomSDKError)grant;

/**
 * @brief Denies the user to remote support .
 * @return If the function succeeds, it will return ZoomSDKError_Success.
 */
- (ZoomSDKError)deny;
@end


/**
 * @protocol ZoomSDKMeetingRemoteSupportControllerDelegate
 * @brief Callback events for remote support controller.
 */
@protocol ZoomSDKMeetingRemoteSupportControllerDelegate <NSObject>
/**
 * @brief Callback event of notification of the remote support status.
 * @param status The value of remote support status.
 * @param userId The remote support user ID.
 */
- (void)onRemoteSupportStatus:(ZoomSDKRemoteSupportStatus)status userId:(unsigned int)userId;

/**
 * @brief These ZoomSDKMeetingRemoteSupportRequestHandler have expired.
 * @param requestHandleList The list of request handle.
 */
- (void)onRemoteSupportRequestExpired:(NSArray<ZoomSDKMeetingRemoteSupportRequestHandler*>*)requestHandleList;

/**
 * @brief Callback event when a host requests to support you remotely.
 * @param handler A pointer to the ZoomSDKMeetingRemoteSupportRequestHandler.
 */
- (void)onRemoteSupportRequestReceived:(ZoomSDKMeetingRemoteSupportRequestHandler*)handler;

/**
 * @brief Callback event for update the user's screen list when the request be granted.
 * @param userId The remote support user ID.
 * @param list The monitor index to be supported.
 */
- (void)onRemoteSupportScreenListUpdated:(unsigned int)userId monitorList:(NSArray<NSNumber*>*)list;

/**
 * @brief Callback event when remote support requests are forbidden for guest users.
 * @param userId The remote support user ID.
 */
- (void)onUserRemoteSupportForbidden:(unsigned int)userId;
@end


/**
 * @class ZoomSDKMeetingRemoteSupportController
 * @brief Zoom SDK meeting remote support controller.
 *
 * This class provides APIs to request, grant, and manage remote support sessions during meetings.
 */
@interface ZoomSDKMeetingRemoteSupportController : NSObject

/**
 * @brief The delegate object to receive remote support events.
 */
@property(nonatomic,assign,nullable)id<ZoomSDKMeetingRemoteSupportControllerDelegate> delegate;

/**
 * @brief Query Whether the meeting supports remote support.
 * @return YES  means supported, NO means not supported.
 */
- (BOOL)isSupportRemoteSupport;

/**
 * @brief Whether if the current user can request remote support.
 * @return YES  means can request remote support, otherwise not.
 */
- (BOOL)canRequestRemoteSupport;

/**
 * @brief Get the list of users'IDs that can be supported remotely.
 * @return If the function succeeds, the return value is a pointer to the array object.
 */
- (NSArray*)getRemoteSupportUserList;

/**
 * @brief Send the request of remote support to the specified user.
 * @param userId Specify the user ID that you want to send the request.
 * @param type Specify category for remote support.
 * @return If the function succeeds, it will return ZoomSDKError_Success.
 */
- (ZoomSDKError)requestRemoteSupport:(unsigned int)userId remoteSupportType:(ZoomSDKRemoteSupportType)type;

/**
 * @brief Stop the remote support to the specified user.
 * @return If the function succeeds, it will return ZoomSDKError_Success.
 */
- (ZoomSDKError)stopRemoteSupport;

/**
 * @brief Whether if the current user is able to select remote support screen.
 * @return If the function succeeds, it will return ZoomSDKError_Success.
 */
- (ZoomSDKError)canSelectRemoteSupportScreen;

/**
 * @brief Select the monitor of remote support to the specified user.
 * @param screenIndex Specify the monitor index that you want to control.
 * @return If the function succeeds, it will return ZoomSDKError_Success.
 */
- (ZoomSDKError)selectRemoteSupportScreen:(int)screenIndex;
@end

NS_ASSUME_NONNULL_END
