/**
 * @file ZoomSDKMeetingRecordController.h
 * @brief Interfaces for controlling meeting recording features in Zoom SDK.
 */

#import <ZoomSDK/ZoomSDKErrors.h>
NS_ASSUME_NONNULL_BEGIN

/**
 * @brief Enumeration of recording layout mode.
 */
typedef enum
{
	/** For initialization. */
    RecordingLayoutMode_None = 0,
    /** Only record active speaker's video. */
    RecordingLayoutMode_ActiveVideoOnly = 1,
    /** Video wall mode. */
    RecordingLayoutMode_VideoWall = (1<<1),
    /** Record shared content with participants' video. */
    RecordingLayoutMode_VideoShare = (1<<2),
    /** Record only the audio. */
    RecordingLayoutMode_OnlyAudio = (1<<3),
    /** Record only the shared content. */
    RecordingLayoutMode_OnlyShare = (1<<4),
}RecordingLayoutMode;



/**
 * @class CustomizedRecordingLayoutHelper
 * @brief Helper class to customize recording layout during meetings.
 */
@interface CustomizedRecordingLayoutHelper : NSObject
/**
 * @brief Get the layout mode supported by the current meeting.
 * @return If the function succeeds, it will return the layout mode. The value is the 'bitwise OR' of each supported layout mode.
 */
- (int)getSupportLayoutMode;

/**
 * @brief Get the list of users whose video source is available.
 * @return The list of users. ZERO(0) indicates that there is no available video source of users. 
 */
- (NSArray*)getValidVideoSource;

/**
 * @brief Get available shared source received by users. 
 * @return If the function succeeds, it will return a NSArray including all valid shared source received by users.
 */
- (NSArray*)getValidReceivedShareSource;

/**
 * @brief Query if sending shared source is available. 
 * @return YES means available, otherwise not.
 */
- (BOOL)isSendingShareSourceAvailable;

/**
 * @brief Determine if there exists the active video source.
 * @return YES means existing, otherwise not.
 */
- (BOOL)haveActiveVideoSource;

/**
 * @brief Select layout mode for recording.
 * @param mode The layout mode for recording.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise not.
 */
- (ZoomSDKError)selectRecordingLayoutMode:(RecordingLayoutMode)mode;

/**
 * @brief Add the video source of specified user to the list of recorded videos.
 * @param userid The ID of specified user.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise not.
 */
- (ZoomSDKError)addVideoSourceToResArray:(unsigned int)userid;

/**
 * @brief Add active video source to the array.
 * @return If the function succeeds, it will return the @c ZoomSDKError_Success, otherwise failed.
 * @note It works only when RecordingLayoutMode is  or RecordingLayoutMode_VideoShare.
 */
- (ZoomSDKError)selectActiveVideoSource;

/**
 * @brief Select the shared source of the specified user.
 * @param shareSourceID The share source ID of specified user.
 * @return If the function succeeds, it will return the @c ZoomSDKError_Success, otherwise failed.
 * @note It works only when RecordingLayoutMode is RecordingLayoutMode_OnlyShare.
 */
- (ZoomSDKError)selectShareSource:(unsigned int)shareSourceID;
/**
 * @brief Select shared source of the current user.
 * @return If the function succeeds, it will return the @c ZoomSDKError_Success, otherwise failed.
 * @note It works only when RecordingLayoutMode is RecordingLayoutMode_OnlyShare.
 */
- (ZoomSDKError)selectSendShareSource;
@end


/**
 * @class ZoomSDKRequestLocalRecordingPrivilegeHandler
 * @brief Handler for managing requests for local recording privileges.
 */
@interface ZoomSDKRequestLocalRecordingPrivilegeHandler : NSObject
/**
 * @brief Get the request ID.
 */
@property(nonatomic, copy, readonly, nullable)NSString* requestId;
/**
 * @brief Get the user ID who requested privilege.
 */
@property(nonatomic, assign, readonly)int requesterId;
/**
 * @brief Get the user name who requested privileges.
 */
@property(nonatomic, copy, readonly, nullable)NSString* requesterName;
/**
 * @brief Allows the user to start local recording.
 * @return If the function succeeds, it will return the @c ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)grantLocalRecordingPrivilege;
/**
 * @brief Denies the user permission to start local recording.
 * @return If the function succeeds, it will return the @c ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)denyLocalRecordingPrivilege;
@end


/**
 * @class ZoomSDKRequestEnableAndStartSmartRecordingHandler
 * @brief Handler for managing a user's request to enable and start smart recording.
 */
@interface ZoomSDKRequestEnableAndStartSmartRecordingHandler : NSObject

/**
 * @brief The user who requests to enable and start smart cloud recording.
 */
@property(nonatomic, assign, readonly)unsigned int requestUserID;

/**
 * @brief Get legal tip that you should agree to handle the user request.
 */
@property(nonatomic, copy, readonly, nullable)NSString* tipString;

/**
 * @brief Start normal cloud recording without enabling smart recording.
 */
- (ZoomSDKError)startCloudRecordingWithoutEnableSmartRecording;

/**
 * @brief Agree to the legal notice to enable and start smart cloud recording.
 * @param allMeetings NO means means only enable smart recording for the current meeting. YES means enable smart recording for all future meetings including the current meeting,
 */
- (ZoomSDKError)agreeToEnableAndStart:(BOOL)allMeetings;

/**
 * @brief Decline the request to start cloud recording.
 * @param denyAll YES indicates to deny all attendees' requests for the host to start cloud recording. Participants can't send these types of requests again until the host changes the setting.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)deny:(BOOL)denyAll;
@end

/**
 * @class ZoomSDKSmartRecordingEnableActionHandler
 * @brief Object to handle enable and start the smart recording.
 */
@interface ZoomSDKSmartRecordingEnableActionHandler : NSObject
/**
 * @brief Get the legal tip to enable smart recording.
 */
@property(nonatomic, copy, readonly)NSString* tipString;
/**
 * @brief Confirm enabling and starting the smart recording.
 * @param allMeetings NO means means only enable smart recording for the current meeting. YES means enable smart recording for all future meetings including the current meeting,
 */
- (ZoomSDKError)actionConfirm:(BOOL)allMeetings;

/**
 * @brief Cancel enabling and starting the smart recording.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)actionCancel;
@end

/**
 * @class ZoomSDKRequestStartCloudRecordingHandler
 * @brief Object to handle a user's request to start cloud recording. If the current user can control web setting for smart recording, they will get ZoomSDKRequestEnableAndStartSmartRecordingHandler when an attendee requests to start cloud recording or start cloud recording by self.
 */
@interface ZoomSDKRequestStartCloudRecordingHandler : NSObject

/**
 * @brief Get the user ID who requested that the host start cloud recording.
 */
@property(nonatomic, assign, readonly)unsigned int requesterId;

/**
 * @brief Get the user name who requested that the host start cloud recording.
 */
@property(nonatomic, copy, readonly, nullable)NSString* requesterName;

/**
 * @brief Allows to start cloud recording.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)start;

/**
 * @brief Deny the request to start cloud recording.
 * @param denyAll  YES indicates to deny all attendees' requests for the host to start cloud recording. Participants can't send these types of requests again until the host changes the setting.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)deny:(BOOL)denyAll;
@end



/**
 * @brief Delegate protocol to receive recording-related events.
 */
@protocol ZoomSDKMeetingRecordDelegate <NSObject>

/**
 * @brief Callback event of ending the conversion to MP4 format.
 * @param success YES means converting successfully, otherwise not. 
 * @param recordPath The path of saving the recording file.
 */
- (void)onRecord2MP4Done:(BOOL)success Path:(NSString*)recordPath;

/**
 * @brief Callback event of the process to convert the recording file to MP4 format. 
 * @param percentage Percentage of conversion process. Range from ZERO(0) to ONE HUNDREAD(100).
 */
- (void)onRecord2MP4Progressing:(int)percentage;

/**
 * @brief Callback event that the status of Cloud recording changes.
 * @param status Value of recording status.
 */
- (void)onCloudRecordingStatus:(ZoomSDKRecordingStatus)status;

/**
 * @brief Callback event that the recording authority changes.
 * @param canRec YES means that it is able to record, otherwise not.
 */
- (void)onRecordPrivilegeChange:(BOOL)canRec;

/**
 * @brief Callback event that the local recording source changes in the custom UI mode.
 * @param helper A CustomizedRecordingLayoutHelper pointer.
 */
- (void)onCustomizedRecordingSourceReceived:(CustomizedRecordingLayoutHelper*)helper;

/**
 * @brief Callback event that the status of the specified user local recording changes.
 * @param status Value of recording status.
 * @param userID The ID of the specified user.
 */
- (void)onLocalRecordStatus:(ZoomSDKRecordingStatus)status userID:(unsigned int)userID;

/**
 * @brief Callback event that the status of request local recording privilege.
 * @param status Value of request local recording privilege status.
 */
- (void)onLocalRecordingPrivilegeRequestStatus:(ZoomSDKRequestLocalRecordingStatus)status;

/**
 * @brief Callback event when a user requests local recording privilege.
 * @param handler A pointer to the ZoomSDKRequestLocalRecordingPrivilegeHandler.
 */
- (void)onLocalRecordingPrivilegeRequested:(ZoomSDKRequestLocalRecordingPrivilegeHandler *_Nullable)handler;

/**
 * @brief An event sink that the cloud recording storage is full.
 * @param gracePeriodDate A point in time in time, in milliseconds, in UTC.  You can use the cloud recording storage until the gracePeriodDate.
 */
- (void)onCloudRecordingStorageFull:(time_t)gracePeriodDate;

/**
 * @brief Callback event for when the host responds to a cloud recording permission request.
 * @param status Value of request host to start cloud recording response status.
 */
- (void)onRequestCloudRecordingResponse:(ZoomSDKRequestStartCloudRecordingStatus)status;

/**
 * @brief Callback event received only by the host when a user requests to start cloud recording.
 * @param handler A pointer to the ZoomSDKRequestStartCloudRecordingHandler.
 */
- (void)onStartCloudRecordingRequested:(ZoomSDKRequestStartCloudRecordingHandler *_Nullable)handler;

/**
 * @brief Callback event received only by the host when a user requests to enable and start smart cloud recording.
 * @param handler The handler to agree or decline the request.
 */
- (void)onEnableAndStartSmartRecordingRequested:(ZoomSDKRequestEnableAndStartSmartRecordingHandler *_Nullable)handler;

/**
 * @brief Callback event received when you call enableSmartRecording.
 * @param handler The handler to confirm or cancel enabling and starting the smart recording.
 */
- (void)onSmartRecordingEnableActionCallback:(ZoomSDKSmartRecordingEnableActionHandler *_Nullable)handler;
@end


/**
 * @brief Controller to manage meeting recording features.
 */
@interface ZoomSDKMeetingRecordController : NSObject
{
    id<ZoomSDKMeetingRecordDelegate> _delegate;
}
/**
 * @brief Delegate to receive recording events.
 */
@property(nonatomic, assign, nullable)id<ZoomSDKMeetingRecordDelegate> delegate;
/**
 * @brief Determine if the current user is enabled to start recording.
 * @param isCloud YES means to determine whether to enable the cloud recording. NO local recording.
 * @return If the value of cloud_recording is set to TRUE and the cloud recording is enabled, the return value is ZoomSDKError_Success. If the value of cloud_recording is set to FALSE and the local recording is enabled, the return value is ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)canStartRecording:(BOOL)isCloud;
/**
 * @brief Determine if the current user owns the authority to change the recording permission of the others.
 * @return If the user own the authority, the return value is ZoomSDKError_Success. Otherwise failed. 
 */
- (ZoomSDKError)canAllowDisallowRecording;

/**
 * @brief Start cloud recording.
 * @param start Set it to YES to start cloud recording, NO to stop recording.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise failed. 
 */
- (ZoomSDKError)startCloudRecording:(BOOL)start;

/**
 * @brief Start recording on the local computer.
 * @param startTimestamp The timestamps when start recording.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise failed.  
 */
- (ZoomSDKError)startRecording:(time_t*)startTimestamp;

/**
 * @brief Stop recording on the local computer.
 * @param stopTimestamp The timestamps when stop recording.
 * @return If the function succeeds, the return value is SDKErr_Success. Otherwise failed.
 */
- (ZoomSDKError)stopRecording:(time_t*)stopTimestamp;

/**
 * @brief Determine if the user owns the authority to enable the local recording.
 * @param userid Specify the user ID.
 * @return If the specified user is enabled to start local recording, the return value is ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)isSupportLocalRecording:(unsigned int)userid;

/**
 * @brief Give the specified user authority for local recording.
 * @param allow YES means allowing user to record on the local computer, otherwise not.
 * @param userid Specify the user ID.
 * @return If the specified user is enabled to start local recording, the return value is ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)allowLocalRecording:(BOOL)allow User:(unsigned int)userid;

/**
 * @brief Set whether to enable custom local recording notification.
 * @param request Set it to YES to receive callback of onCustomizedRecordingSourceReceived.	
 * @return If the specified user is enabled to start local recording, the return value is ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)requestCustomizedLocalRecordingNotification:(BOOL)request;

/**
 * @brief Pause cloud recording.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)pauseCloudRecording;
/**
 * @brief Resume cloud recording.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)resumeCloudRecording;
/**
 * @brief Pause local recording
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)pauseLocalRecording;
/**
 * @brief Resume local recording.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)resumeLocalRecording;

/**
 * @brief Start rawdata recording.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)startRawRecording;

/**
 * @brief Stop rawdata recording.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)stopRawRecording;

/**
 * @brief Determine if the specified user is enabled to start raw recording.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise the function failed.
 */
- (ZoomSDKError)canStartRawRecording;

/**
 * @brief Get current cloud recording status.
 * @return If the function succeeds, the return value is recording status.
 */
- (ZoomSDKRecordingStatus)getCloudRecordingStatus;

/**
 * @brief Determine if the user owns the authority to enable the local recording.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)isSupportRequestLocalRecordingPrivilege;

/**
 * @brief Send a request to ask the host to start cloud recording.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)requestStartCloudRecording;

/**
 * @brief Send a request to enable the SDK to start local recording.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)requestLocalRecordingPrivilege;

/**
 * @brief Determine if the smart recording feature is enabled in the meeting.
 * @return YES means that the feature enabled, NO means that the feature isn't enabled.
 */
- (BOOL)isSmartRecordingEnabled;

/**
 * @brief Whether the current user can enable the smart recording feature.
 * @return YES means the current user can enable the smart recording feature.
 */
- (BOOL)canEnableSmartRecordingFeature;

/**
 * @brief Enable the smart recording feature.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)enableSmartRecording;
@end
NS_ASSUME_NONNULL_END
