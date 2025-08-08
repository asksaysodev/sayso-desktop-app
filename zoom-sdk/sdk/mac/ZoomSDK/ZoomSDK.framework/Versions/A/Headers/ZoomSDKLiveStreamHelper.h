/**
 * @file ZoomSDKLiveStreamHelper.h
 * @brief Interfaces for managing standard and raw live streaming in Zoom meetings.
 */


#import <ZoomSDK/ZoomSDKErrors.h>

NS_ASSUME_NONNULL_BEGIN
/**
 * @class ZoomSDKLiveStreamItem
 * @brief Represents a standard live stream target with URL and description.
 */
@interface ZoomSDKLiveStreamItem : NSObject
/**
 * @brief Get live stream URL.
 * @return The live stream URL.							   
 */
- (NSString*)getLiveStreamURL;

/**
 * @brief Get the viewer URL of the live stream meeting.
 * @return The viewer URL of the live stream meeting.
 */
- (NSString*)getLiveStreamViewerURL;

/**
 * @brief Get description of live stream URL.
 * @return The descriptions of live stream URL.
 */
- (NSString*)getLiveStreamURLDescription;
@end

/**
 * @class ZoomSDKRawLiveStreamInfo
 * @brief Information of an active raw live stream.
 */
@interface ZoomSDKRawLiveStreamInfo : NSObject
/**
 * @brief Get the user ID.
 */
@property(nonatomic, assign, readonly) unsigned int userID;
/**
 * @brief Get the broadcast URL.
 */
@property(nonatomic, copy, readonly) NSString *broadcastUrl;
/**
 * @brief Get the broadcast name.
 */
@property(nonatomic, copy, readonly) NSString *broadcastName;
@end


/**
 * @class ZoomSDKRequestRawLiveStreamPrivilegeHandler
 * @brief Process after the host receives the requirement from the user to give the raw live stream privilege.
 */
@interface ZoomSDKRequestRawLiveStreamPrivilegeHandler : NSObject
/**
 * @brief Get the request ID.
 */
@property(nonatomic, copy, readonly, nullable) NSString *requestId;
/**
 * @brief Get the user ID who requested privilege.
 */
@property(nonatomic, assign, readonly) unsigned int requesterId;
/**
 * @brief Get the user name who requested privileges.
 */
@property(nonatomic, copy, readonly, nullable) NSString *requesterName;
/**
 * @brief Get the broadcast URL.
 */
@property(nonatomic, copy, readonly, nullable) NSString *broadcastUrl;
/**
 * @brief Get the broadcast name.
 */
@property(nonatomic, copy, readonly, nullable) NSString *broadcastName;

/**
 * @brief Allows the user to start raw live stream and finally self-destroy.
 * @return If the function succeeds, the return value ZoomSDKErrors_Success.
 */
- (ZoomSDKError)grantRawLiveStreamPrivilege;

/**
 * @brief Denies the user permission to start raw live stream and finally self-destroy.
 * @return If the function succeeds, the return value ZoomSDKError_Success.
 */
- (ZoomSDKError)denyRawLiveStreamPrivilege;
@end

/**
 * @protocol ZoomSDKLiveStreamHelperDelegate
 * @brief Protocol for receiving live stream-related events and updates.
 */
@protocol ZoomSDKLiveStreamHelperDelegate <NSObject>
/**
 * @brief Callback event when live stream status changes.
 * @param status The live streaming status.
 */
- (void)onLiveStreamStatusChange:(LiveStreamStatus)status;

/**
 * @brief Callback event when the current user's raw live streaming privilege changes.
 * @param bHasPrivilege Specify whether or not the user has privilege.
 */
- (void)onRawLiveStreamPrivilegeChanged:(BOOL)bHasPrivilege;

/**
 * @brief Callback event when the current user's request has times out.
 */
- (void)onRawLiveStreamPrivilegeRequestTimeout;

/**
 * @brief Callback event when another user's raw live streaming privilege changes.
 * @param userID The ID of the user whose privilege changed.
 * @param bHasPrivilege Specify whether or not the user has privilege.
 */
- (void)onUserRawLiveStreamPrivilegeChanged:(unsigned int)userID hasPrivilege:(BOOL)bHasPrivilege;

/**
 * @brief Callback event when a user requests raw live streaming privilege.
 * @param handler A pointer to the ZoomSDKRequestRawLiveStreamPrivilegeHandler.
 */
- (void)onRawLiveStreamPrivilegeRequested:(ZoomSDKRequestRawLiveStreamPrivilegeHandler*_Nullable)handler;

/**
 * @brief Callback event when users start or stop raw live streaming.
 * @param liveStreamList A list of users with an active raw live stream.
 */
- (void)onUserRawLiveStreamingStatusChanged:(NSArray<ZoomSDKRawLiveStreamInfo*>*_Nullable)liveStreamList;

/**
 * @brief Callback event when the live stream reminder enable status changes.
 * @param enable YES means the live stream reminder is enabled.
 */
- (void)onLiveStreamReminderStatusChanged:(BOOL)enable;

/**
 * @brief Callback event when the live stream reminder enable status change fails.
 */
- (void)onLiveStreamReminderStatusChangeFailed;

/**
 * @brief Callback event when the meeting or webinar user has nearly reached the meeting capacity, like 80% or 100% for the meeting or webinar capacity.
 * The host can start live stream to let unjoined users to watch live stream.
 * @param percent Proportion of joined users to the total capacity.
 */
- (void)onUserThresholdReachedForLiveStream:(int)percent;
@end


/**
 * @class ZoomSDKLiveStreamHelper
 * @brief Interface for managing standard and raw live streams in a meeting.
 */
@interface ZoomSDKLiveStreamHelper : NSObject
{
    id<ZoomSDKLiveStreamHelperDelegate> _delegate;
}
/**
 * @brief Set or get the delegate to receive live stream event callbacks.
 */
@property(nonatomic, assign, nullable)id<ZoomSDKLiveStreamHelperDelegate> delegate;
/**
 * @brief Query if it is able for the user to enable live stream.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */

- (ZoomSDKError)canStartLiveStream;

/**
 * @brief Get the items of live stream supported by the SDK.
 * @return If the function succeeds, it will return the items, otherwise failed.
 * @deprecated Use \link ZoomSDKASController::getSupportLiveStreamItems \endlink instead.
 */
- (NSArray*)getSupportLiveStreamItem DEPRECATED_MSG_ATTRIBUTE("Use -getSupportLiveStreamItems instead");

/**
 * @brief Get the list of live stream information items in the current meeting.
 * @return If the function succeeds, it will return the the live stream item list, otherwise failed.
 */
- (NSArray<ZoomSDKLiveStreamItem*>*_Nullable)getSupportLiveStreamItems;

/**
 * @brief Get the current live stream object.
 * @return If the function succeeds, it will return the current live stream object, otherwise failed.
 */
- (ZoomSDKLiveStreamItem*_Nullable)getCurrentLiveStreamItem;

/**
 * @brief Start a live stream.
 * @param item The item of live stream supported by the SDK.
 * @return If the function succeeds, it will return the ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)startLiveStream:(ZoomSDKLiveStreamItem*)item;

/**
 * @brief Start a live stream with the URL customized by user.
 * @param streamURL The URL of customized live stream.
 * @param key The key of customized stream stream.
 * @param broadcastURL Everyone who uses this link can watch the live broadcast.
 * @return If the function succeeds, it will return the ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)startLiveStreamByURL:(NSString*)streamURL StreamKey:(NSString*)key BroadcastURL:(NSString*)broadcastURL;

/**
 * @brief Stop a live stream.
 * @return If the function succeeds, it will return the ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)stopLiveStream;

/**
 * @brief Get the status of current live stream.
 * @return If the function succeeds, it will return the LiveStreamStatus_InProgress, otherwise failed.
 */
- (LiveStreamStatus)getLiveStreamStatus;

/**
 * @brief Query Whether the meeting supports raw live streams.
 * @return Yes if supported, NO if not supported.
 */
- (BOOL)isRawLiveStreamSupported;

/**
 * @brief Whether if the current user is able to start raw live streaming.
 * @return If the current user is able to start raw live streaming, the return value is ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)canStartRawLiveStream;

/**
 * @brief Send a request to enable the SDK to start a raw live stream.
 * @param broadcastURL The broadcast URL of the live-stream.
 * @param broadcastName The broadcast name of the live-stream.
 * @return If the function succeeds, the return value is ZoomSDKError_Success and the SDK will send the request, otherwise it fails and the request will not be sent.
 */
- (ZoomSDKError)requestRawLiveStreaming:(NSString*)broadcastURL broadcastName:(NSString*)broadcastName;

/**
 * @brief Start raw live streaming.
 * @param broadcastURL The broadcast URL of the live-stream.
 * @param broadcastName The broadcast name of the live-stream.
 * @return If the function succeeds, it will return the ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)startRawLiveStreaming:(NSString *)broadcastURL broadcastName:(NSString*)broadcastName;

/**
 * @brief Stop raw live streaming.
 * @return If the function succeeds, it will return the ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)stopRawLiveStream;

/**
 * @brief Remove the raw live stream privilege.
 * @param userID Specify the ID of the user whose privilege will be removed.
 * @return If the function succeeds, the return value is ZoomSDKError_Success, otherwise it fails.
 */
- (ZoomSDKError)removeRawLiveStreamPrivilege:(unsigned int)userID;

/**
 * @brief Get a list of current active raw live streams.
 * @return If the function succeeds, the return value is a pointer to the NSArray object, otherwise nil.
 */
- (NSArray<ZoomSDKRawLiveStreamInfo*>*_Nullable)getRawLiveStreamingInfoList;

/**
 * @brief Get the list of users’ IDs who have raw live stream privileges.
 * @return If the function succeeds, the return value is a pointer to the NSArray object.
 */
- (NSArray<NSNumber*>*_Nullable)getRawLiveStreamPrivilegeUserList;
/**
 * @brief Check if the live stream reminder is enabled.
 * When the live stream reminder is enabled, the new join user is notified that the meeting is at capacity but that they can watch the meeting live stream with the callback \link ZoomSDKMeetingServiceDelegate::onMeetingFullToWatchLiveStream: \endlink when the meeting user has reached the meeting capacity.
 * @return YES means the live stream reminder is enabled.
 */
- (BOOL)isLiveStreamReminderEnabled;

/**
 * @brief Check if the current user can enable or disable the live stream reminder.
 * @return YES means the current user can enable or disable the live stream reminder.
 */
- (BOOL)canEnableLiveStreamReminder;

/**
 * @brief Enable or disable the live stream reminder.
 * @param enble YES means enable the live stream reminder. False means disable the live stream reminder.
 * @return If the function succeeds, the return value is ZoomSDKError_Success, otherwise it fails.
 */
- (ZoomSDKError)enableLiveStreamReminder:(BOOL)enable;
@end
NS_ASSUME_NONNULL_END
