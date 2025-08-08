/**
 * @file ZoomSDKMeetingConfiguration.h
 * @brief Meeting configuration interface for controlling UI behavior and meeting features.
 */

#import <ZoomSDK/ZoomSDKErrors.h>
NS_ASSUME_NONNULL_BEGIN

/**
 * @class ZoomSDKMeetingConfiguration
 * @brief Meeting configuration class to control various meeting UI and behavior settings.
 */
@interface ZoomSDKMeetingConfiguration :NSObject
{
    CGDirectDisplayID  _displayAppID;
    CGDirectDisplayID  _monitorID;
    NSPoint            _floatVideoPoint;
    BOOL               _shareToolBarVisible;
    NSPoint            _mainVideoPoint;
    BOOL               _jbhWindowVisible;
    BOOL               _enableMuteOnEntry;
    BOOL               _enableChime;
    BOOL               _isDirectShareDesktop;
    BOOL               _enableAutoAdjustSpeakerVolume;
    BOOL                _enableAutoAdjustMicVolume;
    BOOL                _disablePopupWrongPasswordWindow;
    BOOL                _autoAdjustSpeakerVolumeWhenJoinAudio;
    BOOL                _disableEndOtherMeetingAlert;
    BOOL                _hideEndOtherMeetingAlert;
    BOOL                _enableAutoEndOtherMeeting;
    BOOL                _disableInputPasswordWindow;
    BOOL                _disableDoubleClickToFullScreen;
    BOOL                _hideThumbnailVideoWindow;
    BOOL                _disableRenameInMeeting;
    BOOL                _disableShareButtonClickOriginAction;
    BOOL                _disableToolbarInviteButtonClickOriginAction;
    BOOL                _needPrefillWebinarJoinInfo;
    BOOL                _hideRegisterWebinarInfoWindow;
    BOOL                _hideWebinarNameInputWindow;
    BOOL                _disableParticipantButtonClickOriginAction;
    BOOL                _hideTelephoneInAudioWindow;
    BOOL                _hideCallMeInAudioWindow;
    BOOL                _forceDisableMultiShare;
    BOOL                _disableCustomLiveStreamAction;
    BOOL                _disableFreeUserOriginAction;
    BOOL                _disableFreeMeetingRemainTimeNotify;
    BOOL                _hideInviteInMeetingH323CallInTab;
    BOOL                _hideInviteInMeetingH323CallOutTab;
    BOOL                _hideMeetingStaticBadNetWorkWaring;
    BOOL                _hideMeetingStaticSystemBusyWaring;
    BOOL                _hideSwitchCameraButton;
    BOOL                _disableOpenRecordFileWhenMeetingEnd;
    BOOL                _hideShowKeypadButton;
    BOOL                _hideCopyURLButtonWhenInviteOthers;
    BOOL                _hideCopyInvitationButtonWhenInviteOthers;
    BOOL                _hideChatItemInMeeting;
    BOOL                _hideRemoteControlItemOnMoreMenu;
    BOOL                _hideChooseSaveRecordingFilePathWindow;
    BOOL                _disableAudioButtonClickOriginAction;
    BOOL                _disableAudioSettingMenuButtonClickOriginAction;
    BOOL                _hideLoadingWindow;
    BOOL                _disableBreakoutRoomsButtonClickOriginAction;
    BOOL                _hideMeetingInfoButtonOnVideo;
    BOOL                _hideInviteButtonOnHCWindow;
    BOOL                _disableToolbarLeaveButtonClickOriginAction;
    BOOL                _disableToolbarCloseCaptionButtonClickOriginAction;
    BOOL                _hideInviteLinkOnMeetingUI;
    BOOL                _hideCloudWhiteboardFeedbackButton;
    BOOL                _hideCloudWhiteboardShareButton;
    BOOL                _hideCloudWhiteboardAboutButton;
    BOOL                _hideCloudWhiteboardHelperCenterButton;
    BOOL                _hideCloudWhiteboardOpenInBrowserButton;
    BOOL                _hideConfirmStartArchiveWindow;
    BOOL                _hideRecoverMeetingWindow;
}
/**
 * @brief The APP ID to be shared.
 */
@property(nonatomic, assign)CGDirectDisplayID displayAppID;
/**
 * @brief The monitor ID to be shared.
 */
@property(nonatomic, assign)CGDirectDisplayID monitorID;
/**
 * @brief The location of float video.
 */
@property(nonatomic, assign)NSPoint floatVideoPoint;
/**
 * @brief The location of main interface of meeting.
 */
@property(nonatomic, assign)NSPoint mainVideoPoint;
/**
 * @brief The visibility of toolbar when sharing.
 */
@property(nonatomic, assign)BOOL shareToolBarVisible;
/**
 * @brief The visibility of the window of waiting for the host.
 */
@property(nonatomic, assign)BOOL jbhWindowVisible;
/**
 * @brief Attendees join the meeting with audio muted.
 */
@property(nonatomic, assign)BOOL enableMuteOnEntry;
/**
 * @brief Query whether to share screen or not.
 */
@property(nonatomic, assign)BOOL isDirectShareDesktop;
/**
 * @brief Play chime when user joins/leaves meeting.
 */
@property(nonatomic, assign)BOOL enableChime;
/**
 * @brief Hide the prompt dialog of wrong password.
 */
@property(nonatomic, assign)BOOL disablePopupWrongPasswordWindow;
/**
 * @brief Enable auto-adjust the speaker volume when joining meeting.
 * @deprecated This method is no longer used.
 */
@property(nonatomic, assign)BOOL autoAdjustSpeakerVolumeWhenJoinAudio DEPRECATED_MSG_ATTRIBUTE("No longer used");
/**
 * @brief Enable auto-adjust the microphone volume.
 * @deprecated This method is no longer used.
 */
@property(nonatomic, assign)BOOL autoAdjustMicVolumeWhenJoinAudio DEPRECATED_MSG_ATTRIBUTE("No longer used");
/**
 * @brief Disable the alert to end another ongoing meeting.
 * @deprecated This method is no longer used.
 */
@property(nonatomic, assign)BOOL disableEndOtherMeetingAlert DEPRECATED_MSG_ATTRIBUTE("No longer used");
/**
 * @brief Hide the alert to end another ongoing meeting.
 */
@property(nonatomic, assign)BOOL hideEndOtherMeetingAlert;
/**
 * @brief Auto end another ongoing meeting.
 */
@property(nonatomic, assign)BOOL enableAutoEndOtherMeeting;
/**
 * @brief Disable input password window.
 */
@property(nonatomic, assign)BOOL disableInputPasswordWindow;
/**
 * @brief Disable the feature to enter full screen by double click.
 */
@property(nonatomic, assign)BOOL disableDoubleClickToFullScreen;
/**
 * @brief Hide the window of thumbnail video.
 */
@property(nonatomic, assign)BOOL hideThumbnailVideoWindow;
/**
 * @brief Disable to rename in meeting.
 */
@property(nonatomic, assign)BOOL disableRenameInMeeting;
/**
 * @brief Disable ZOOM original actions of clicking share button.
 */
@property(nonatomic, assign)BOOL disableShareButtonClickOriginAction;
/**
 * @brief Disable ZOOM original actions of clicking toolbar invite button.
 */
@property(nonatomic, assign)BOOL disableToolbarInviteButtonClickOriginAction;
/**
 * @brief Input meeting information in advance when user joins webinar.
 */
@property(nonatomic, assign)BOOL needPrefillWebinarJoinInfo;
/**
 * @brief Hide register webinar window when join webinar.
 */
@property(nonatomic, assign)BOOL hideRegisterWebinarInfoWindow;
/**
 * @brief Hide input name window when join webinar.
 */
@property(nonatomic, assign)BOOL hideWebinarNameInputWindow;
/**
 * @brief Disable ZOOM original actions of clicking button participants.
 */
@property(nonatomic, assign)BOOL disableParticipantButtonClickOriginAction;
/**
 * @brief Hide the window of phone dialing in.
 */
@property(nonatomic, assign)BOOL hideTelephoneInAudioWindow;
/**
 * @brief Hide the window of CALL ME.
 */
@property(nonatomic, assign)BOOL hideCallMeInAudioWindow;
/**
 * @brief Forbid multi-participants sharing at the same time.
 */
@property(nonatomic, assign)BOOL forceDisableMultiShare;
/**
 * @brief Disable custom live stream.
 */
@property(nonatomic, assign)BOOL disableCustomLiveStreamAction;
/**
 * @brief Set whether to disable ZOOM original reminder action for free user.
 */
@property(nonatomic, assign)BOOL disableFreeUserOriginAction;
/**
 * @brief Disable the ZOOM original notification of remaining time for meeting organized by free user.
 */
@property(nonatomic, assign)BOOL disableFreeMeetingRemainTimeNotify;
/**
 * @brief Hide the h323 call in tab on invite window.
 */
@property(nonatomic, assign)BOOL hideInviteInMeetingH323CallInTab;
/**
 * @brief Hide the h323 call out tab on invite window.
 */
@property(nonatomic, assign)BOOL hideInviteInMeetingH323CallOutTab;
/**
 * @brief Hide meeting static warning of bad network.
 */
@property(nonatomic, assign)BOOL hideMeetingStaticBadNetWorkWaring;
/**
 * @brief Hide meeting static warning of system busy.
 */
@property(nonatomic, assign)BOOL hideMeetingStaticSystemBusyWaring;
/**
 * @brief Hide switch camera button when sharing camera.
 */
@property(nonatomic, assign)BOOL hideSwitchCameraButton;
/**
 * @brief Disable opening recording file when meeting end.
 */
@property(nonatomic, assign)BOOL disableOpenRecordFileWhenMeetingEnd;
/**
 * @brief Hide 'show keypad' button on meeting window.
 */
@property(nonatomic, assign)BOOL hideShowKeypadButton;
/**
 * @brief Hide copy URL button when invite others join meeting.
 */
@property(nonatomic, assign)BOOL hideCopyURLButtonWhenInviteOthers;
/**
 * @brief Hide copy Invitation button when invite others join meeting.
 */
@property(nonatomic, assign)BOOL hideCopyInvitationButtonWhenInviteOthers;
/**
 * @brief Hide chat menu item in-meeting.
 */
@property(nonatomic, assign)BOOL hideChatItemInMeeting;
/**
 * @brief Hide remote control item on more menu.
 */
@property(nonatomic, assign)BOOL hideRemoteControlItemOnMoreMenu;
/**
 * @brief Hide choose save recording file path window.
 */
@property(nonatomic, assign)BOOL hideChooseSaveRecordingFilePathWindow;
/**
 * @brief Disable ZOOM original actions of clicking Audio button.
 */
@property(nonatomic, assign)BOOL disableAudioButtonClickOriginAction;
/**
 * @brief Disable audio menu item original action in-meeting.
 */
@property(nonatomic, assign)BOOL disableAudioSettingMenuButtonClickOriginAction;
/**
 * @brief Hide loading window when start meeting without login.
 */
@property(nonatomic, assign)BOOL hideLoadingWindow;
/**
 * @brief Disable ZOOM original actions of clicking button Breakout Rooms.
 */
@property(nonatomic, assign)BOOL disableBreakoutRoomsButtonClickOriginAction;
/**
 * @brief Hide meeting info button on video UI.
 */
@property(nonatomic, assign)BOOL hideMeetingInfoButtonOnVideo;
/**
 * @brief Hide invited button on participants window.
 */
@property(nonatomic, assign)BOOL hideInviteButtonOnHCWindow;
/**
 * @brief Disable ZOOM original actions of clicking toolbar Leave button.
 */
@property(nonatomic, assign)BOOL disableToolbarLeaveButtonClickOriginAction;
/**
 * @brief Disable ZOOM original actions of clicking toolbar CloseCaption button.
 */
@property(nonatomic, assign)BOOL disableToolbarCloseCaptionButtonClickOriginAction;
/**
 * @brief hide invited link on meeting info window.
 */
@property(nonatomic, assign)BOOL hideInviteLinkOnMeetingUI;
/**
 * @brief Set the feedback button’s visibility on cloud Whiteboard. Default is displaying.
 */
@property(nonatomic, assign)BOOL hideCloudWhiteboardFeedbackButton;
/**
 * @brief Set the share button’s visibility on cloud Whiteboard. Default is displaying.
 */
@property(nonatomic, assign)BOOL hideCloudWhiteboardShareButton;
/**
 * @brief Set the about button’s visibility on the cloud whiteboard. Default is displaying.
 */
@property(nonatomic, assign)BOOL hideCloudWhiteboardAboutButton;
/**
 * @brief Set the Helper center button's visibility on cloud whiteboard. Default is displaying.
 */
@property(nonatomic, assign)BOOL hideCloudWhiteboardHelperCenterButton;
/**
 * @brief Set the Open in browser button's visibility on cloud whiteboard. Default is displaying.
 */
@property(nonatomic, assign)BOOL hideCloudWhiteboardOpenInBrowserButton;
/**
 * @brief Set the confirm start archive dialog to hide or display. Default is displaying.
 */
@property(nonatomic, assign)BOOL hideConfirmStartArchiveWindow;
/**
 * @brief Enable or disable hiding the recover meeting window in Zoom UI mode.
 */
@property(nonatomic, assign)BOOL hideRecoverMeetingWindow;

/**
 * @brief Prefill the user name and email info when joining webinar.
 * @param userName The user name to prefill.
 * @param email The email address to prefill.
 * @return If the function succeeds, it returns ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)prefillWebinarUserName:(NSString*)userName Email:(NSString*)email;

/**
 * @brief Hide or show specified SDK buttons.
 * @param hide YES to hide the button, NO to show.
 * @param button The SDK button type to be hidden or shown.
 * @return If the function succeeds, it returns ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)hideSDKButtons:(BOOL)hide ButtonType:(SDKButton)button;

/**
 * @brief Modify the DSCP of audio and video.
 * @param videoDSCP Video values in the meeting.
 * @param audioDSCP Audio values in the meeting.
 * @return If the function succeeds, it will return the @c ZoomSDKError_Success, otherwise failed.
 * @note It is necessary to input both values of the videoDSCP and audioDSCP if you want to modify.
 */
- (ZoomSDKError)modifyVideoDSCP:(int)videoDSCP AudioDSCP:(int)audioDSCP;

/**
 * @brief Reset all properties in this class.
 */
- (void)reset;

/**
 * @brief Enable or disable the new Zoom Whiteboard feature (different from Classic Whiteboard). This is enabled by default.
 * @param enable YES means enable the cloud whiteboard feature,otherwise not.
 * @return If the function succeeds, it will return the ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)enableCloudWhiteboard:(BOOL)enable;

/**
 * @brief Set whether to close the current sharing of another user without prompt and directly beginning a new sharing content by the closer.
 * @param enable YES indicates no prompt, otherwise not.
 * @return If the function succeeds, it will return the ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)enableGrabShareWithoutReminder:(BOOL)enable;

/**
 * @brief Set the meeting input user info dialog to hide or display.
 * @param hide YES means hide the dialog. Otherwise, do not hide the dialog.
 */
- (void)setHideMeetingInputUserInfoWindow:(BOOL)hide;

/**
 * @brief Get the meeting input user info dialog to hide or display.
 * @return YES if the input user info dialog is hidden, otherwise NO.
 */
- (BOOL)isHideMeetingInputUserInfoWindow;
@end
NS_ASSUME_NONNULL_END
