/**
 * @file ZoomSDKMeetingService.h
 * @brief Define parameter structures for joining and starting Zoom meetings using the Zoom SDK.
 */


#import <Foundation/Foundation.h>
#import <ZoomSDK/ZoomSDKErrors.h>
#import <ZoomSDK/ZoomSDKH323Helper.h>
#import <ZoomSDK/ZoomSDKPhoneHelper.h>
#import <ZoomSDK/ZoomSDKWaitingRoomController.h>
#import <ZoomSDK/ZoomSDKMeetingUIController.h>
#import <ZoomSDK/ZoomSDKMeetingConfiguration.h>
#import <ZoomSDK/ZoomSDKASController.h>
#import <ZoomSDK/ZoomSDKMeetingActionController.h>
#import <ZoomSDK/ZoomSDKLiveStreamHelper.h>
#import <ZoomSDK/ZoomSDKVideoContainer.h>
#import <ZoomSDK/ZoomSDKMeetingRecordController.h>
#import <ZoomSDK/ZoomSDKWebinarController.h>
#import <ZoomSDK/ZoomSDKCloseCaptionController.h>
#import <ZoomSDK/ZoomSDKRealNameAuthenticationController.h>
#import <ZoomSDK/ZoomSDKQAController.h>
#import <ZoomSDK/ZoomSDKNewBreakoutRoomController.h>
#import <ZoomSDK/ZoomSDKInterpretationController.h>
#import <ZoomSDK/ZoomSDKReactionController.h>
#import <ZoomSDK/ZoomSDKAppSignalController.h>
#import <ZoomSDK/ZoomSDKRawArchivingController.h>
#import <ZoomSDK/ZoomSDKSignInterpretationController.h>
#import <ZoomSDK/ZoomSDKCustomImmersiveController.h>
#import <ZoomSDK/ZoomSDKMeetingChatController.h>
#import <ZoomSDK/ZoomSDKMeetingWhiteboardController.h>
#import <ZoomSDK/ZoomSDKMeetingEncryptionController.h>
#import <ZoomSDK/ZoomSDKPollingController.h>
#import <ZoomSDK/ZoomSDKMeetingRemoteSupportController.h>
#import <ZoomSDK/ZoomSDKMeetingAICompanionController.h>
#import <ZoomSDK/ZoomSDKCameraControlHelper.h>
#import <ZoomSDK/ZoomSDKMeetingIndicatorController.h>
#import <ZoomSDK/ZoomSDKMeetingProductionStudioController.h>
#import <ZoomSDK/ZoomSDKMeetingDocsController.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * @class ZoomSDKMeetingParameter
 * @brief Meeting parameter information.
 */
@interface ZoomSDKMeetingParameter : NSObject
/**
 * @brief meetingType type of the meeting.
 */
@property (assign,nonatomic) MeetingType meetingType;
/**
 * @brief isViewOnly YES indicates to view only meeting,otherwise not.
 */
@property (assign,nonatomic) BOOL isViewOnly;
/**
 * @brief isAutoRecordingLocal Auto local recording or not. YES indicates to auto local recording.
 */
@property (assign,nonatomic) BOOL isAutoRecordingLocal;
/**
 * @brief isAutoRecordingCloud Auto cloud recording or not. YES indicates to auto cloud recording.
 */
@property (assign,nonatomic) BOOL isAutoRecordingCloud;
/**
 * @brief meetingNumber unique number of the meeting.
 */
@property (assign,nonatomic) long long meetingNumber;
/**
 * @brief meetingTopic topic of the meeting.
 */
@property (copy,nonatomic, nullable) NSString *meetingTopic;
/**
 * @brief meetingHost host of the meeting.
 */
@property (copy,nonatomic, nullable) NSString *meetingHost;
@end


/**
 * @class ZoomSDKStartMeetingElements
 * @brief Meeting start options when using SDK credentials (no ZAK).
 */
@interface ZoomSDKStartMeetingElements : NSObject
/**
 * @brief Set meetingNumber to 0 if you want to start a meeting with vanityID.
 */
@property(nonatomic, copy, nullable)NSString* vanityID;
/**
 * @brief It depends on the type of client account.
 */
@property(nonatomic, assign)ZoomSDKUserType userType;
/**
 * @brief It may be the number of a scheduled meeting or a Personal Meeting ID. Set it to 0 to start an instant meeting.
 */
@property(nonatomic, assign)long long meetingNumber;
/**
 * @brief Set it to YES to start sharing computer desktop directly when meeting starts.
 */
@property(nonatomic, assign)BOOL isDirectShare;
/**
 * @brief The APP to be shared.
 */
@property(nonatomic, assign)CGDirectDisplayID displayID;
/**
 * @brief Set it to YES to turn off the video when user joins meeting.
 */
@property(nonatomic, assign)BOOL isNoVideo;
/**
 * @brief Set it to YES to turn off the audio when user joins meeting.
 */
@property(nonatomic, assign)BOOL isNoAudio;
/**
 * @brief Customer Key the customer key of user.
 */
@property(nonatomic, copy, nullable)NSString* customerKey;
/**
 * @brief Set it to YES to contain my voice in the mixed audio raw data.
 */
@property(nonatomic, assign)BOOL isMyVoiceInMix;
/**
 * @brief Set the invitation ID for automatic meeting invitation.
 */
@property(nonatomic, copy, nullable)NSString *inviteContactID;
/**
 * @brief Is audio raw data stereo? The default is mono.
 */
@property(nonatomic, assign)BOOL isAudioRawDataStereo;
/**
 * @brief The sampling rate of the acquired raw audio data. The default is AudioRawdataSamplingRate_32K.
 */
@property(nonatomic, assign)ZoomSDKAudioRawdataSamplingRate audioRawdataSamplingRate;
@end


/**
 * @class ZoomSDKStartMeetingUseZakElements
 * @brief Meeting start options using ZAK (Zoom Access Key).
 */
@interface ZoomSDKStartMeetingUseZakElements : NSObject
/**
 * @brief Security session key got from web.
 */
@property(nonatomic, copy, nullable)NSString* zak;
/**
 * @brief User's screen name displayed in the meeting.
 */
@property(nonatomic, copy, nullable)NSString* displayName;
/**
 * @brief Set meetingNumber to 0 if you want to start a meeting with vanityID.
 */
@property(nonatomic, copy, nullable)NSString* vanityID;
/**
 * @brief User type.
 */
@property(nonatomic, assign)SDKUserType userType;
/**
 * @brief It may be the number of a scheduled meeting or a Personal Meeting ID. Set it to 0 to start an instant meeting.
 */
@property(nonatomic, assign)long long meetingNumber;
/**
 * @brief Set it to YES to start sharing computer desktop directly when meeting starts.
 */
@property(nonatomic, assign)BOOL isDirectShare;
/**
 * @brief The APP to be shared.
 */
@property(nonatomic, assign)CGDirectDisplayID displayID;
/**
 * @brief Set it to YES to turn off the video when user joins meeting.
 */
@property(nonatomic, assign)BOOL isNoVideo;
/**
 * @brief Set it to YES to turn off the audio when user joins meeting.
 */
@property(nonatomic, assign)BOOL isNoAudio;
/**
 * @brief Customer Key the customer key of user.
 */
@property(nonatomic, copy, nullable)NSString* customerKey;
/**
 * @brief Set it to YES to contain my voice in the mixed audio raw data.
 */
@property(nonatomic, assign)BOOL isMyVoiceInMix;
/**
 * @brief Set the invitation ID for automatic meeting invitation.
 */
@property(nonatomic, copy, nullable)NSString *inviteContactID;
/**
 * @brief Is audio raw data stereo? The default is mono.
 */
@property(nonatomic, assign)BOOL isAudioRawDataStereo;
/**
 * @brief The sampling rate of the acquired raw audio data. The default is AudioRawdataSamplingRate_32K.
 */
@property(nonatomic, assign)ZoomSDKAudioRawdataSamplingRate audioRawdataSamplingRate;
@end

/**
 * @class ZoomSDKJoinMeetingElements
 * @brief Meeting join options.
 */
@interface ZoomSDKJoinMeetingElements : NSObject
/**
 * @brief Security session key got from web.
 */
@property(nonatomic, copy, nullable)NSString* zak;
/**
 * @brief It is indispensable for a panelist when user joins a webinar.
 */
@property(nonatomic, copy, nullable)NSString* webinarToken;
/**
 * @brief User's screen name displayed in the meeting.
 */
@property(nonatomic, copy, nullable)NSString* displayName;
/**
 * @brief Personal meeting URL, set meetingNumber to 0 if you want to start meeting with vanityID.
 */
@property(nonatomic, copy, nullable)NSString* vanityID;
/**
 * @brief User type.
 */
@property(nonatomic, assign)ZoomSDKUserType userType;
/**
 * @brief Customer Key the customer key of user.
 */
@property(nonatomic, copy, nullable)NSString* customerKey;
/**
 * @brief The number of meeting that you want to join.
 */
@property(nonatomic, assign)long long meetingNumber;
/**
 * @brief Set it to YES to start sharing computer desktop directly when meeting starts.
 */
@property(nonatomic, assign)BOOL isDirectShare;
/**
 * @brief The APP to be shared.
 */
@property(nonatomic, assign)CGDirectDisplayID displayID;
/**
 * @brief Set it to YES to turn off the video when user joins meeting.
 */
@property(nonatomic, assign)BOOL isNoVideo;
/**
 * @brief Set it to YES to turn off the audio when user joins meeting.
 */
@property(nonatomic, assign)BOOL isNoAudio;
/**
 * @brief Meeting password. Set it to nil or @"" to remove the password.
 */
@property(nonatomic, copy, nullable)NSString *password;
/**
 * @brief appPrivilegeToken Token for app privilege.
 */
@property(nonatomic, copy, nullable)NSString *appPrivilegeToken;
/**
 * @brief join_token Meeting join token.
 */
@property(nonatomic, copy, nullable)NSString *join_token;
/**
 * @brief Set it to YES to contain my voice in the mixed audio raw data.
 */
@property(nonatomic, assign)BOOL isMyVoiceInMix;
/**
 * @brief Is audio raw data stereo? The default is mono.
 */
@property(nonatomic, assign)BOOL isAudioRawDataStereo;
/**
 * @brief The sampling rate of the acquired raw audio data. The default is AudioRawdataSamplingRate_32K.
 */
@property(nonatomic, assign)ZoomSDKAudioRawdataSamplingRate audioRawdataSamplingRate;
/**
 * @brief On behalf token.
 */
@property(nonatomic, copy, nullable)NSString* onBehalfToken;
@end


/**
 * @class ZoomSDKMeetingInputUserInfoHandler
 * @brief Interface for handling user input when joining a meeting.
 */
@interface ZoomSDKMeetingInputUserInfoHandler : NSObject

/**
 * @brief Get default display name.
 */
@property(nonatomic, copy, readonly, nullable)NSString *defaultDisplayName;

/**
 * @brief Check whether the user can modify default display name.
 */
@property(nonatomic, assign, readonly)BOOL canModifyDefaultDisplayName;

/**
 * @brief Check whether the inputed email is a valid email format.
 * @param email The email must meet the email format requirements. The email inputed by the logged-in user must be the email.
 * @return YES If the email input is valid, otherwise not.
 * @note The email must meet the email format requirements. The email input by the logged in user must be the same as the user account.
 */
- (BOOL)isValidEmail:(NSString *)email;

/**
 * @brief input user info.
 * @param name The display name to input.
 * @param email The email to input.
 * @return If the function succeeds, it will return ZoomSDKError_Success.
 */
- (ZoomSDKError)inputUserInfo:(NSString *)name email:(NSString *)email;

/**
 * @brief Cancel to join meeting.
 */
- (void)cancel;
@end

/**
 * @class ZoomSDKMeetingArchiveConfirmHandler
 * @brief The interface for the user to confirm whether start archiving after joining the meeting.
 */
@interface ZoomSDKMeetingArchiveConfirmHandler : NSObject
/**
 * @brief The content that notifies the user to confirm starting to archive when joining the meeting.
 */
@property(nonatomic, copy, readonly, nullable)NSString* archiveConfirmContent;

/**
 * @brief Join the meeting.
 * @param startArchive YES means start the archive when joining the meeting, NO means do not start the archive when joining the meeting.
 * @return If the function succeeds, it will return ZoomSDKError_Success.
 */
- (ZoomSDKError)joinWithArchive:(BOOL)startArchive;
@end

/**
 * @class ZoomSDKRecoverMeetingHandler
 * @brief The interface for host user to handle recover meeting or not when start a deleted or expired meeting.
 */
@interface ZoomSDKRecoverMeetingHandler : NSObject
/**
 * @brief The content that notifies the host user to recover the meeting.
 */
@property(nonatomic, copy, readonly, nullable)NSString* recoverMeetingContent;

/**
 * @brief Join the meeting.
 * @param toRecover YES means to recover the meeting and start the meeting. NO means not to recover the meeting and leave the start meeting process.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise the function fails.
 */
- (ZoomSDKError)recoverMeeting:(BOOL)toRecover;
@end


/**
 * @brief Protocol for receiving Zoom meeting events and status changes.
 */
@protocol ZoomSDKMeetingServiceDelegate <NSObject>

@optional

/**
 * @brief Notify if ZOOM meeting status Changes.
 * @param state The status of ZOOM meeting.
 * @param error The enum of ZoomSDKMeetingError.
 * @param reason The enum of EndMeetingReason.
 */
- (void)onMeetingStatusChange:(ZoomSDKMeetingStatus)state meetingError:(ZoomSDKMeetingError)error EndReason:(EndMeetingReason)reason;

/**
 * @brief Notification of statistic warnings of Zoom Meeting.
 * @param type The statistic type.
 */
- (void)onMeetingStatisticWarning:(StatisticWarningType)type;

/**
 * @brief Designated for notify the free meeting need upgrade.
 * @param type The enumeration of FreeMeetingNeedUpgradeType, if the type is FreeMeetingNeedUpgradeType_BY_GIFTURL, user can upgrade free meeting through url. if the type is FreeMeetingNeedUpgradeType_BY_ADMIN, user can ask admin user to upgrade the meeting.
 * @param giftURL User can upgrade the free meeting through the url.
 */
- (void)onFreeMeetingNeedToUpgrade:(FreeMeetingNeedUpgradeType)type giftUpgradeURL:(NSString*_Nullable)giftURL;

/**
 * @brief Designated for notify the free meeting which has been upgraded to free trail meeting has started.
 */
- (void)onFreeMeetingUpgradeToGiftFreeTrialStart;

/**
 * @brief Designated for notify the free meeting which has been upgraded to free trail meeting has stoped.
 */
- (void)onFreeMeetingUpgradeToGiftFreeTrialStop;

/**
 * @brief Designated for notify the free meeting has been upgraded to professional meeting.
 */
- (void)onFreeMeetingUpgradedToProMeeting;

/**
 * @brief Designated for notify the free meeting remain time has been stoped to count down.
 */
- (void)onFreeMeetingRemainTimeStopCountDown;

/**
 * @brief Inform user the remaining time of free meeting.
 * @param seconds The remaining time of the free meeting.
 */
- (void)onFreeMeetingRemainTime:(unsigned int)seconds;
/**
 * @brief Meeting parameter notification callback,The callback will be triggered right before the meeting starts. The meetingParameter will be destroyed once the function calls end.
 * @param meetingParameter Meeting parameter.
 */
- (void)onMeetingParameterNotification:(ZoomSDKMeetingParameter *_Nullable)meetingParameter;

/**
 * @brief The function is invoked when a user joins a meeting that needs their username and email.
 * @param handler Configure information or leave meeting.
 */
- (void)onJoinMeetingNeedUserInfo:(ZoomSDKMeetingInputUserInfoHandler *_Nullable)handler;

/**
 * @brief Callback event when joining a meeting if the admin allows the user to choose to archive the meeting.
 * @param handler The handler for the user to choose whether to archive the meeting when joining the meeting.
 */
- (void)onUserConfirmToStartArchive:(ZoomSDKMeetingArchiveConfirmHandler *_Nullable)handler;

/**
 * @brief Callback event when join a meeting while the meeting is ongoing.
 * @param actionEndOtherMeeting block to end other meeting.
 * @param actionCancel block to cancel join meeting.
 */
- (void)onEndOtherMeetingToJoinMeetingNotification:(ZoomSDKError (^)(void))actionEndOtherMeeting actionCancel:(void (^)(void))actionCancel;

/**
 * @brief Calback event that the meeting users have reached the meeting capacity.
 * The new join user can not join the meeting, but they can watch the meeting live stream.
 * @param liveStreamUrl the live stream URL to watch the meeting live stream.
 */
- (void)onMeetingFullToWatchLiveStream:(NSString*)liveStreamUrl;

/**
 * @brief Callback event when host starts a deleted or expired meeting (not a PMI meeting).
 * @param handler The handler for the host user to handle recover or not.
 */
- (void)onUserConfirmRecoverMeeting:(ZoomSDKRecoverMeetingHandler * _Nullable)handler;
@end

/**
 * @class ZoomSDKMeetingService
 * @brief It is an implementation for client to start/join a Meeting.
 * @note The meeting service allows only one concurrent operation at a time, which means, only one API call is in progress at any given time.		 
 */
@interface ZoomSDKMeetingService : NSObject
{
    id<ZoomSDKMeetingServiceDelegate> _delegate;
    ZoomSDKMeetingUIController* _meetingUIController;
    ZoomSDKMeetingConfiguration* _meetingConfiguration;
    ZoomSDKH323Helper*           _h323Helper;
    ZoomSDKWaitingRoomController* _waitingRoomController;
    ZoomSDKPhoneHelper*           _phoneHelper;
    ZoomSDKASController*          _asController;
    ZoomSDKMeetingActionController*  _actionController;
    ZoomSDKLiveStreamHelper*         _liveStreamHelper;
    //customized UI
    ZoomSDKVideoContainer*           _videoContainer;
    ZoomSDKMeetingRecordController*  _recordController;
    ZoomSDKWebinarController*        _webinarController;
    ZoomSDKCloseCaptionController*   _closeCaptionController;
    ZoomSDKRealNameAuthenticationController*       _realNameController;
    ZoomSDKQAController*             _QAController;
    ZoomSDKNewBreakoutRoomController*  _newBOController;
    ZoomSDKInterpretationController *  _InterpretationController;
    ZoomSDKReactionController*         _reactionController;
    ZoomSDKAppSignalController*        _appSignalController;
    ZoomSDKRawArchivingController*     _rawArchivingController;
    ZoomSDKSignInterpretationController*     _signInterpretationController;
    ZoomSDKCustomImmersiveController* _customImmersiveController;
    ZoomSDKMeetingChatController* _chatController;
    ZoomSDKMeetingWhiteboardController* _whiteboardController;
    ZoomSDKMeetingEncryptionController* _encryptionController;
    ZoomSDKPollingController*   _pollingController;
    ZoomSDKMeetingRemoteSupportController* _remoteSupportController;
    ZoomSDKMeetingAICompanionController* _AICompanionController;
    ZoomSDKMeetingIndicatorController * _meetingIndicatorController;
    ZoomSDKMeetingProductionStudioController* _productionStudioController;
    ZoomSDKMeetingDocsController * _meetingDocsController;
}
/**
 * @brief Callback of receiving meeting events.
 */
@property (assign, nonatomic, nullable) id<ZoomSDKMeetingServiceDelegate> delegate;

/**
 * @brief Get the meeting UI controller interface.
 * @return If the function succeeds, the return value is an object of ZoomSDKMeetingUIController. Otherwise returns nil.
 */
- (ZoomSDKMeetingUIController*)getMeetingUIController;

/**
 * @brief Get the configuration of the meeting.
 * @return If the function succeeds, the return value is an object of ZoomSDKMeetingConfiguration. Otherwise returns nil.
 */
- (ZoomSDKMeetingConfiguration*)getMeetingConfiguration;

/**
 * @brief Get the default H.323 helper of ZOOM meeting service. 
 * @return If the function succeeds, the return value is a ZoomSDKH323Helper object of H.323 Helper.
 */
- (ZoomSDKH323Helper*_Nullable)getH323Helper;

/**
 * @brief Get default Waiting Room Controller of ZOOM meeting service.
 * @return If the function succeeds, the return value is an object of ZoomSDKWaitingRoomController. Otherwise returns nil.
 */
- (ZoomSDKWaitingRoomController*)getWaitingRoomController;

/**
 * @brief Get the default AS(APP share) Controller of ZOOM meeting service.
 * @return If the function succeeds, the return value is an object of ZoomSDKASController. Otherwise returns nil.
 */
- (ZoomSDKASController*)getASController;

/**
 * @brief Get the  default Phone Callout Helper of Zoom meeting service.
 * @return If the function succeeds, the return value is an object of ZoomSDKPhoneHelper. Otherwise returns nil.
 */
- (ZoomSDKPhoneHelper*)getPhoneHelper;

/**
 * @brief Get the default action controller(mute audio/video etc) of ZOOM meeting service.
 * @return If the function succeeds, the return value is an object of ZoomSDKMeetingActionController. Otherwise returns nil.
 */
- (ZoomSDKMeetingActionController*)getMeetingActionController;

/**
 * @brief Get the default live stream helper of ZOOM meeting service.
 * @return If the function succeeds, the return value is an object of ZoomSDKLiveStreamHelper. Otherwise returns nil.
 */
- (ZoomSDKLiveStreamHelper*)getLiveStreamHelper;

/**
 * @brief Get the custom video container of ZOOM SDK.
 * @return If the function succeeds, the return value is an object of ZoomSDKVideoContainer which allows user to customize in-meeting UI. Otherwise returns nil.
 */
- (ZoomSDKVideoContainer*_Nullable)getVideoContainer;

/**
 * @brief Get the custom recording object of ZOOM SDK.
 * @return If the function succeeds, the return value is an object of ZoomSDKMeetingRecordController which allows user to customize meeting recording. Otherwise returns nil.
 */
- (ZoomSDKMeetingRecordController*)getRecordController;
/**
 * @brief Get the custom webinar controller.
 * @return If the function succeeds, the return value is an object of ZoomSDKWebinarController which allows you to customize webinar. Otherwise returns nil.
 */
- (ZoomSDKWebinarController*)getWebinarController;

/**
 * @brief Get controller of close caption in Zoom meeting.
 * @return If the function succeeds, it will return a ZoomSDKCloseCaptionController object which you can use to handle close caption in meeting.
 */
- (ZoomSDKCloseCaptionController*)getCloseCaptionController;

/**
 * @brief Get object of controller ZoomSDKRealNameAuthenticationController.
 * @return If the function succeeds, it will return a ZoomSDKRealNameAuthenticationController object which you can use to Real-name authentication.
 */
- (ZoomSDKRealNameAuthenticationController *)getRealNameController;

/**
 * @brief Get object of ZoomSDKQAController.
 * @return If the function succeeds, it will return a ZoomSDKQAController object.
 */
- (ZoomSDKQAController *)getQAController;

/**
 * @brief Get object of ZoomSDKNewBreakoutRoomController.
 * @return If the function succeeds, it will return a ZoomSDKNewBreakoutRoomController object.
 */
- (ZoomSDKNewBreakoutRoomController *)getNewBreakoutRoomController;

/**
 * @brief Get object of ZoomSDKInterpretationController.
 * @return If the function succeeds, it will return a ZoomSDKInterpretationController object.
 */
- (ZoomSDKInterpretationController*)getInterpretationController;

/**
 * @brief Get object of ZoomSDKSignInterpretationController.
 * @return If the function succeeds, it will return a ZoomSDKSignInterpretationController object.
 */
- (ZoomSDKSignInterpretationController*)getSignInterpretationController;
/**
 * @brief Get object of ZoomSDKReactionController.
 * @return If the function succeeds, it will return a ZoomSDKReactionController object.
 */
- (ZoomSDKReactionController*)getReactionController;

/**
 * @brief Get object of ZoomSDKAppSignalController.
 * @return If the function succeeds, it will return a ZoomSDKAppSignalController object.
 */
- (ZoomSDKAppSignalController*_Nullable)getAppSignalController;

/**
 * @brief Get object of ZoomSDKRawArchivingController.
 * @return If the function succeeds, it will return a ZoomSDKRawArchivingController object.
 */
- (ZoomSDKRawArchivingController*)getRawArchivingController;


/**
 * @brief Get the immersive controller.
 * @return If the function succeeds, the return value is a pointer to ZoomSDKCustomImmersiveController. Otherwise, the function returns nil.
 */
- (ZoomSDKCustomImmersiveController*_Nullable)getMeetingImmersiveController;

/**
 * @brief Get the whiteboard controller.
 * @return If the function succeeds, the return value is a pointer to ZoomSDKMeetingWhiteboardController. Otherwise, the function returns nil.
 */
- (ZoomSDKMeetingWhiteboardController*)getMeetingWhiteboardController;
/**
 * @brief Get the meeting chat controller.
 * @return If the function succeeds, the return value is a pointer to ZoomSDKMeetingChatController. Otherwise, the function returns nil.
 */
- (ZoomSDKMeetingChatController*)getMeetingChatController;

/**
 * @brief Get the encryption controller.
 * @return If the function succeeds, the return value is a pointer to ZoomSDKMeetingEncryptionController. Otherwise, the function returns nil.
 */
- (ZoomSDKMeetingEncryptionController*)getInMeetingEncryptionController;

/**
 * @brief Get the remote support controller.
 * @return If the function succeeds, the return value is a pointer to ZoomSDKMeetingRemoteSupportController. Otherwise, the function returns nil.
 */
- (ZoomSDKMeetingRemoteSupportController*)getMeetingRemoteSupportController;

/**
 * @brief Get data center info.
 * @return If the function succeeds, it will return data center details. Otherwise failed.
 */
- (NSString*)getInMeetingDataCenterInfo;

/**
 * @brief Get the meeting polling controller.
 * @return If the function succeeds, the return value is a pointer to ZoomSDKPollingController. Otherwise, the function returns nil.
 */
- (ZoomSDKPollingController*)getMeetingPollingController;

/**
 * @brief Get the meeting AI companion controller.
 * @return If the function succeeds, the return value is a pointer to ZoomSDKMeetingAICompanionController. Otherwise, the function returns nil.
 */
- (ZoomSDKMeetingAICompanionController*)getInMeetingAICompanionController;

/**
 * @brief Get camera controller object, can control any user's cameras.
 * @param userId Controls user's own camera.
 * @return If the function succeeds, the return value is a pointer to ZoomSDKCameraControlHelper. Otherwise, the function returns nil.
 */
- (ZoomSDKCameraControlHelper*_Nullable)getCameraControlHelper:(unsigned int)userId;

/**
 * @brief Get the production studio controller object.
 * @return If the function succeeds, the return value is a pointer to ZoomSDKMeetingProductionStudioController. Otherwise, the function returns nil.
 */
- (ZoomSDKMeetingProductionStudioController*)getMeetingProductionStudioController;

/**
 * @brief Revoke camera control privilege
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)revokeCameraControlPrivilege;

/**
 * @brief Get  indicator controller object.
 * @return If the function succeeds, the return value is a pointer to ZoomSDKMeetingIndicatorController. Otherwise, the function returns nil.
 */
- (ZoomSDKMeetingIndicatorController*_Nullable)getMeetingIndicatorController;

/**
 * @brief Get docs controller object.
 * @return If the function succeeds, the return value is a pointer to ZoomSDKMeetingDocsController. Otherwise, the function returns nil.
 */
- (ZoomSDKMeetingDocsController*_Nullable)getMeetingDocsController;

/**
 * @brief Start a ZOOM meeting with meeting number for login user.
 * @param context It is a ZoomSDKStartMeetingElements class,contain all params to start meeting.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)startMeeting:(ZoomSDKStartMeetingElements *)context;
/**
 * @brief Start a ZOOM meeting with ZAK.
 * @param context It is a ZoomSDKStartMeetingUseZakElements class,contain all params to start meeting with zak.
 * @return If the function succeeds, it will return the @c ZoomSDKError_Success, otherwise failed.
 * @note It is just for non-logged-in user.
 */
- (ZoomSDKError)startMeetingWithZAK:(ZoomSDKStartMeetingUseZakElements *)context;
/**
 * @brief Join a Zoom meeting.
 * @param context It is a ZoomSDKJoinMeetingElements class,contain all params to join meeting.
 * @return If the function succeeds, it will return the @c ZoomSDKError_Success, otherwise failed.
 * @note toke4enfrocelogin/customerKey is for API user.
 */
- (ZoomSDKError)joinMeeting:(ZoomSDKJoinMeetingElements *)context;
/**
 * @brief End/Leave the current meeting.
 * @param cmd The command for leaving the current meeting. Only host can end the meeting.
 */
- (void)leaveMeetingWithCmd:(LeaveMeetingCmd)cmd;

/**
 * @brief Get the status of meeting.
 * @return The status of meeting.
 */
- (ZoomSDKMeetingStatus)getMeetingStatus;

/**
 * @brief Get the property of meeting.
 * @param command Commands for user to get different properties.
 * @return If the function succeeds, it will return an NSString of meeting property, otherwise failed.
 */
- (NSString*_Nullable)getMeetingProperty:(MeetingPropertyCmd)command;

/**
 * @brief Get the audio type supported by the current meeting.
 * @return If the function succeeds, it will return the audio type supported by the current meeting. The value is the 'bitwise OR' of each supported audio type.
 */
- (int)getSupportedMeetingAudioType;

/**
 * @brief Get the network quality of meeting connection.
 * @param component Video/audio/share.
 * @param sending Set it to YES to get the status of sending data, NO to get the status of receiving data.
 * @return If the function succeeds, it will return an enumeration of network connection quality, otherwise failed.
 */
- (ZoomSDKConnectionQuality)getConnectionQuality:(ConnectionComponent)component Sending:(BOOL)sending;
/**
 * @brief Get the type of current meeting.
 * @return If the function succeeds, it will return the type of meeting, otherwise failed.
 */
- (MeetingType)getMeetingType;

/**
 * @brief Determine whether the meeting is failover or not.
 * @return YES means the current meeting is failover, otherwise not. 
 */

- (BOOL)isFailoverMeeting;

/**
 * @brief Handle the event that user joins meeting from web or meeting URL.
 * @param urlAction The URL string got from web.
 * @return If the function succeeds, it will return ZoomSDKError_Succuss, otherwise failed.
 */
- (ZoomSDKError)handleZoomWebUrlAction:(NSString*)urlAction;

/**
 * @brief Get the size of user's video.
 * @param userID The ID of user in the meeting. UserID should be 0 when not in meeting.
 * @return The size of user's video.
 */
- (CGSize)getUserVideoSize:(unsigned int)userID;
@end
NS_ASSUME_NONNULL_END




