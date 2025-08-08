/**
 * @file ZoomSDKWebinarController.h
 * @brief Interfaces for managing webinar features in Zoom SDK.
 */


#import <Foundation/Foundation.h>
#import <ZoomSDK/ZoomSDKErrors.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * @class ZoomSDKWebinarMeetingStatus
 * @brief Status information of the current webinar.
 */
@interface ZoomSDKWebinarMeetingStatus : NSObject
{
    BOOL _allowPanellistStartVideo;
    BOOL _allowAttendeeChat;
    BOOL _allowRaiseHand;
    BOOL _isSupportAttendeeTalk;
}
/**
 * @brief Query if panelist is allowed to start video.
 * @return YES means that panelist is allowed to enable video, otherwise not.
 */
- (BOOL)isAllowPanellistStartVideo;
/**
 * @brief Query if attendee is allowed to chat.
 * @return YES means that attendee is allowed to chat, otherwise not.
 */
- (BOOL)isAllowAttendeeChat;
/**
 * @brief Query if attendee raise hand status.
 * @return If it is enabled, the return value is YES. Otherwise, the return value is NO.
 */
- (BOOL)isAllowRaiseHand;
/**
 * @brief Query if webinar supports attendee to talk.
 * @return YES means supporting the feature of allowing attendee to talk, otherwise not.
 * @deprecated This method is no longer used.
 */
- (BOOL)isSupportAttendeeTalk DEPRECATED_MSG_ATTRIBUTE("No longer used");
/**
 * @brief Determine if current webinar supports emoji reactions.
 * @return YES means the current webinar supports emoji reactions, NO means the feature is not supported.
 */
- (BOOL)isWebinarEmojiReactionSupported;
/**
 * @brief Query if emoji reactions status.
 * @return If it is enabled, the return value is YES. If not, the return value is NO.
 */
- (BOOL)isWebinarEmojiReactionAllowed;

/**
 * @brief Query if attendee to view the participant count status.
 * @return If this is enabled, the return value is YES. Otherwise the return value is NO.
 */
- (BOOL)canAttendeesViewParticipantCount;
@end


/**
 * @class ZoomSDKWebinarRegisterHelper
 * @brief Helper class for webinar registration.
 */
@interface ZoomSDKWebinarRegisterHelper :NSObject
{
    WebinarRegisterType   _webinarRegisterType;
}
/**
 * @brief Get type of user registers webinar.
 * @return The type of user to register webinar.
 */
- (WebinarRegisterType)getWebinarRegisterType;
/**
 * @brief Get URL of user registers webinar.
 * @return The url for user to register webinar.	
 */
- (NSURL*_Nullable)getWebinarRegisterURL;
/**
 * @brief Input email and screen name to register webinar. 
 * @param email The email to register webinar
 * @param name The display name to register webinar.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)inputEmail:(NSString*)email screenName:(NSString*)name;
/**
 * @brief Cancel to register webinar.
 */
- (ZoomSDKError)cancel;
@end


/**
 * @class ZoomSDKWebinarInputScreenNameHandler
 * @brief Handler for entering screen name when joining a webinar.
 */
@interface ZoomSDKWebinarInputScreenNameHandler :NSObject
/**
 * @brief Input screen name to join the webinar.
 * @param name The display name for the webinar.
 * @return If the function succeeds, it returns ZoomSDKError_Success. Otherwise the function fails.
 */
- (ZoomSDKError)inputName:(NSString*)name;
/**
 * @brief Cancel to join webinar.
 */
- (ZoomSDKError)cancel;
@end


/**
 * @protocol ZoomSDKWebinarControllerDelegate
 * @brief Protocol for receiving webinar events and callbacks.
 */
@protocol ZoomSDKWebinarControllerDelegate <NSObject>
@optional
/**
 * @brief Callback of users are allowed to talk in webinar. User needs to enable the mic in the view-only mode of webinar if he wants to talk.
 */
- (void)onSelfAllowTalkNoti;
/**
 * @brief Callback of users are disallowed to talk in webinar. 
 */
- (void)onSelfDisallowTalkNoti;
/**
 * @brief Callback to promote attendee to panelist.
 * @param result The result of promotion. If the promotion is successful, the result is zero(0). Otherwise it is an error code.
 */
- (void)onPromoteAttendee2PanelistResultNoti:(unsigned int)result;
/**
 * @brief Callback to demote panelist to attendee.
 * @param result The result of demotion. If the promotion is successful, the result is zero(0). Otherwise it is an error code.
 */
- (void)onDepromotePanelist2AttendeeResultNoti:(unsigned int)result;
/**
 * @brief Callback to enable the panelist to start the video.
 */
- (void)onAllowPanelistStartVideoNoti;
/**
 * @brief Callback to disable the panelist to start the video.
 */
- (void)onDisAllowPanelistStartVideoNoti;
/**
 * @brief Panelist will receive this callback if the attendee's audio status changed. 
 * @param userid The user ID of attendee whose audio status changes.
 * @param is_muted YES means the user is muted, otherwise not.
 */
- (void)onAttendeeAudioStatusNoti:(unsigned int)userid canTalk:(BOOL)can_talk isMuted:(BOOL)is_muted;
/**
 * @brief During the webinar, this callback will be triggered if the user needs to register.
 * @param webinarRegisterHelper An object used by user to complete all the related operations.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)onWebinarNeedRegisterResponse:(ZoomSDKWebinarRegisterHelper*_Nullable)webinarRegisterHelper;
/**
 * @brief Panelist will receive this callback if the attendee's audio status changed.
 * @param userid The user ID of attendee whose promote.
 * @param agree YES means the user is agree join webinar as panelist.
 */
- (void)onAttendeePromoteConfirmResult:(unsigned int)userid isAgree:(BOOL)agree;

/**
 * @brief Callback for when the host enables/disables emoji reactions.
 * @param isAllowed If the reaction is allowed, the value of isAllow is true, otherwise false.
 */
- (void)onAllowWebinarReactionStatusChanged:(BOOL)isAllowed;

/**
 * @brief Callback to attendee raise hand status changed.
 * @param isAllowed If the raise hand is allowed, the result is YES. If not, the result is NO.
 */
- (void)onAllowAttendeeRaiseHandStatusChanged:(BOOL)isAllowed;

/**
 * @brief Callback to attendee view the participant count status changed.
 * @param isAllowed If attendees are allowed to view the participant count, the result is YES. Otherwise, the result is NO.
 */
- (void)onAllowAttendeeViewTheParticipantCountStatusChanged:(BOOL)isAllowed;

/**
 * @brief When join the webinar, this callback is triggered if the user needs to input username.
 * @param handler An object used by user to complete all the related operations.
 */
- (void)onWebinarNeedInputScreenName:(ZoomSDKWebinarInputScreenNameHandler*_Nullable)handler;
@end


/**
 * @class ZoomSDKWebinarController
 * @brief Controller for managing webinar-specific functions.
 */
@interface ZoomSDKWebinarController : NSObject
{
    id<ZoomSDKWebinarControllerDelegate> _delegate;
    ZoomSDKWebinarMeetingStatus* _webinarMeetingStatus;
}

/**
 * @brief Set the webinar controller delegate.
 */
@property(nonatomic, assign, nullable)id<ZoomSDKWebinarControllerDelegate> delegate;
/**
 * @brief Get the webinar meeting status.
 */
@property(nonatomic, retain)ZoomSDKWebinarMeetingStatus* webinarMeetingStatus;

/**
 * @brief Promote the specified attendee to panelist.
 * @param userId The ID of the specified attendee.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed. 
 */
- (ZoomSDKError)PromoteAttendee2Panelist:(unsigned int)userId;

/**
 * @brief Demote the specified panelist to attendee.
 * @param userId The ID of the specified panelist. 
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed. 
 */
- (ZoomSDKError)DepromotePanelist2Attendee:(unsigned int)userId;

/**
 * @brief The attendee is permitted to use the audio device.
 * @param userId Specify the permitted user ID.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed. 
 */
- (ZoomSDKError)AllowAttendeeTalk:(unsigned int)userId;

/**
 * @brief Forbid the attendee to use the audio device.
 * @param userId Specify the forbidden user ID.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed. 
 */
- (ZoomSDKError)DisallowAttendeeTalk:(unsigned int)userId;

/**
 * @brief The panelist is permitted to start the video.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed. 
 */
- (ZoomSDKError)AllowPanelistStartVideo;

/**
 * @brief Forbid the panelist to start video.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed. 
 */
- (ZoomSDKError)DisallowPanelistStartVideo;

/**
 * @brief Get the webinar status.
 * @return If the function succeeds, it will return an object of ZoomSDKWebinarMeetingStatus.
 */
- (ZoomSDKWebinarMeetingStatus*_Nullable)getZoomSDKWebinarMeetingStatus;

/**
 * @brief Get the list of attendees in the meeting.
 * @return If the function succeeds, it will return an NSArray of attendees.
 */
- (NSArray*_Nullable)getAttendeeList;

/**
 * @brief Set the chat privilege of the panelist.
 * @param privilege The chat privilege of the panelist.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)setPanelistChatPrivilege:(ZoomSDKPanelistChatPrivilege)privilege;

/**
 * @brief Get the chat privilege of the panelist.
 * @param privilege The chat privilege of the panelist. It validates only when the return value is ZoomSDKError_Success.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)getPanelistChatPrivilege:(ZoomSDKPanelistChatPrivilege*)privilege;

/**
 * @brief Set the view mode of the attendee.
 * @param mode The view mode of the attendee.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise the function returns an error.
 */
- (ZoomSDKError)setAttendeeViewMode:(ZoomSDKAttendeeViewMode)mode;

/**
 * @brief Get the view mode of the attendee.
 * @return If the function succeeds, it will return the attendee's view mode.
 */
- (ZoomSDKAttendeeViewMode)getAttendeeViewMode;

/**
 * @brief Permitted to use emoji reactions.
 * @return If the function succeeds, it will return the @c ZoomSDKError_Success, otherwise failed.
 * @note If the function succeeds, the user receives the '- (void)onAllowWebinarReactionStatusChanged:(BOOL)isAllowed' callback event. Available only for the host.
 */
- (ZoomSDKError)allowWebinarEmojiReaction;

/**
 * @brief Forbid use of emoji reactions.
 * @return If the function succeeds, it will return the @c ZoomSDKError_Success, otherwise failed.
 * @note If the function succeeds, the user receives the '- (void)onAllowWebinarReactionStatusChanged:(BOOL)isAllowed' callback event. Available only for the host.
 */
- (ZoomSDKError)disallowWebinarEmojiReaction;

/**
 * @brief The attendees can raise their hands.
 * @return If the function succeeds, it will return the @c ZoomSDKError_Success, otherwise failed.
 * @note If the function succeeds, the user receives the '- (void)onAllowAttendeeRaiseHandStatusChanged:(BOOL)isAllowed' callback event. Available only for the host.
 */
- (ZoomSDKError)allowAttendeeRaiseHand;

/**
 * @brief Do not let the attendee to raise their hand.
 * @return If the function succeeds, it will return the @c ZoomSDKError_Success, otherwise failed.
 * @note If the function succeeds, the user receives the '- (void)onAllowAttendeeRaiseHandStatusChanged:(BOOL)isAllowed' callback event. Available only for the host.
 */
- (ZoomSDKError)disallowAttendeeRaiseHand;

/**
 * @brief The attendee is permitted to view the participant count.
 * @return If the function succeeds, it will return the @c ZoomSDKError_Success, otherwise failed.
 * @note If the function succeeds, the user receives the '- (void)onAllowAttendeeViewTheParticipantCountStatusChanged:(BOOL)isAllowed' callback event. Available only for the host.
 */
- (ZoomSDKError)allowAttendeeViewTheParticipantCount;

/**
 * @brief Forbid the attendee to view the participant count.
 * @return If the function succeeds, it will return the @c ZoomSDKError_Success, otherwise failed.
 * @note If the function succeeds, the user receives the '- (void)onAllowAttendeeViewTheParticipantCountStatusChanged:(BOOL)isAllowed' callback event. Available only for the host.
 */
- (ZoomSDKError)disallowAttendeeViewTheParticipantCount;

/**
 * @brief Get the participant count.
 * @return The participant count of.
 */
- (int)getParticipantCount;

/**
 * @brief Get the webinal legal notices prompt.
 * @return If the function succeeds, it will return the webinal legal notices prompt. Otherwise nil.
 */
- (NSString *)getWebinarRegistrationLegalNoticesPrompt;

/**
 * @brief Get the webinal legal notices explained.
 * @return If the function succeeds, it will return the webinal legal notices explained.
 */
- (ZoomSDKWebinarRegistrationExplainInfo *)getWebinarRegistrationLegalNoticesExplained;
@end
NS_ASSUME_NONNULL_END
