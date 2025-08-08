/**
 * @file ZoomSDKNotificationServiceController.h
 * @brief Interfaces for meeting transfer and presence notification in Zoom SDK.
 */

#import <Foundation/Foundation.h>
#import <ZoomSDK/ZoomSDKErrors.h>
#import <ZoomSDK/ZoomSDKPresenceHelper.h>

NS_ASSUME_NONNULL_BEGIN
/**
 * @class ZoomSDKInMeetingDeviceInfo
 * @brief Provides information about meetings on other signed-in devices.
 */
@interface ZoomSDKInMeetingDeviceInfo : NSObject
/**
 * @brief Index. Deprecated this param.
 */
@property (assign, nonatomic,readonly)int index;

/**
 * @brief Device name.
 */
@property (copy, nonatomic,readonly, nullable) NSString *deviceName;

/**
 * @brief Meeting topic.
 */
@property (copy, nonatomic,readonly, nullable) NSString *meetingTopic;

/**
 * @brief Meeting number.
 */
@property (assign, nonatomic,readonly) long long meetingNumber;

/**
 * @brief Meeting ID.
 */
@property (copy, nonatomic,readonly, nullable) NSString *meetingID;

/**
 * @brief YES indicates this meeting supports joining in companion mode.
 */
@property (assign, nonatomic,readonly) BOOL isSupportCompanionMode;
@end


/**
 * @class ZoomSDKTransferMeetingHandler
 * @brief Provides information and operation interface for the result of meeting transfer.
 */
@interface ZoomSDKTransferMeetingHandler : NSObject

/**
 * @brief Determine current transfer meeting mode.
 */
@property(nonatomic, assign, readonly)ZoomSDKTransferMeetingMode transferMeetingMode;

/**
 * @brief Determine transfer meeting is success.
 */
@property(nonatomic, assign, readonly)BOOL isTransferMeetingSuccess;

/**
 * @brief Try to leave companion mode and rejoin the meeting in normal mode.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)leaveCompanionToJoinMeeting;
@end


/**
 * @protocol ZoomSDKNotificationServiceDelegate
 * @brief Callback protocol for meeting transfer and device presence notifications.
 */
@protocol ZoomSDKNotificationServiceDelegate <NSObject>

/**
 * @brief Transfer meeting status changed or join companion mode meeting callback.
 * @param bSuccess The result of transfer meeting.
 * @deprecated Use \link ZoomSDKNotificationServiceDelegate::onTransferMeetingResult: \endlink instead.
 */
- (void)onTransferMeetingStatus:(BOOL)bSuccess DEPRECATED_MSG_ATTRIBUTE("Use -onTransferMeetingResult:instead");

/**
 * @brief Transfer meeting status changed callback or join companion mode meeting callback.
 * @param handler A  pointer of ZoomSDKTransferMeetingHandler.
 */
- (void)onTransferMeetingResult:(ZoomSDKTransferMeetingHandler* _Nullable)handler;

/**
 * @brief Callback event that the device for users to attend the meeting has changed.
 * @param deviceList A list of meeting info with active meetings.
 * @note If the user use \link ZoomSDKAuthService#registerNotificationService \endlink. interface to connect to the service, they can't get webinar meeting list in this callback.
 */
- (void)onMeetingDeviceListChanged:(NSArray <ZoomSDKInMeetingDeviceInfo *>*)deviceList;

@end


/**
 * @class ZoomSDKNotificationServiceController
 * @brief Controller class to manage meeting transfer and presence features.
 */
@interface ZoomSDKNotificationServiceController : NSObject
/**
 * @brief Delegate to receive meeting transfer and presence-related callbacks.
 */
@property(nonatomic, assign, nullable)id<ZoomSDKNotificationServiceDelegate> delegate;

/**
 * @brief Determine whether the transfer meeting feature is enabled.
 */
- (BOOL)isTransferMeetingEnabled;

/**
 * @brief Determine whether the presence feature is enabled.
 * @return YES means enabled, otherwise not.
 */
- (BOOL)isPresenceFeatureEnabled;

/**
 * @brief Try to transfer meeting to current device.
 * @param index Specifies the index.
 * @return If the function succeeds, it will return @c ZoomSDKError_Success, otherwise not.
 * @deprecated Use \link ZoomSDKNotificationServiceController::transferMeeting:transferMeetingMode: \endlink instead.
 */
- (ZoomSDKError)transferMeeting:(int)index DEPRECATED_MSG_ATTRIBUTE("Use -transferMeeting:transferMeetingMode: instead");

/**
 * @brief Get object of ZoomSDKPresenceHelper.
 * @return If the function succeeds, it will return a ZoomSDKPresenceHelper object.
 */
- (ZoomSDKPresenceHelper *_Nullable)getPresenceHelper;

/**
 * @brief Try to transfer meeting to current device or join meeting with companion mode.
 * @param meetingId Specifies the meeting list's meeting ID.
 * @param mode Specifies the transfer meeting mode. If the mode is ZoomSDKTransferMeetingMode_Companion, you will join meeting as a companion,Otherwise will transfer meeting to current device.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)transferMeeting:(NSString *)meetingId transferMeetingMode:(ZoomSDKTransferMeetingMode)mode;
@end

NS_ASSUME_NONNULL_END
