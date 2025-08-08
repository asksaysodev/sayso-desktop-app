/**
 * @file ZoomSDKMeetingUIController.h
 * @brief Interfaces for customizing the Zoom meeting UI components and handling related user interactions.
 */


NS_ASSUME_NONNULL_BEGIN
/**
 * @protocol ZoomSDKMeetingUIControllerDelegate
 * @brief Protocol for receiving user interaction events in Zoom meeting UI.
 */
@protocol ZoomSDKMeetingUIControllerDelegate <NSObject>
/**
 * @brief Callback of clicking invite button in toolbar. If user wants to customize the invitation UI, set _disableToolbarInviteButtonClickOriginAction to YES in ZoomSDKMeetingConfiguration.
 */
- (void)onToolbarInviteButtonClick;

/**
 * @brief Notification when participant clicks LEAVE MEETING button. 
 * @param participantID The ID of participant who leaves meeting.
 */
- (void)onClickLeaveMeeting:(unsigned int)participantID;

/**
 * @brief Notification when SHARE button in toolbar is clicked.
 */
- (void)onToolbarShareButtonClick;

/**
 * @brief Notification when participant button is clicked.
 */
- (void)onParticipantButtonClicked;

/**
 * @brief Notification when custom LIVE button is clicked. 
 */
- (void)onCustomLiveButtonClicked;

/**
 * @brief Notification when audio menu button is clicked.
 */
- (void)onAudioSettingMenuButtonClicked;

/**
 * @brief Notification when audio button is clicked.
 * @param userid The userid of user unique identifier .
 * @param info The enum of audio action info.
 */
- (void)onAudioButtonClicked:(unsigned int)userid audioSession:(ZoomSDKAudioActionInfo)info;

/**
 * @brief Notification when breakout Rooms button is clicked.
 */
- (void)onBreakoutRoomsButtonClick;

/**
 * @brief Notification when CloseCaption button is clicked.
 */
- (void)onCloseCaptionButtonClick;
@end


/**
 * @class ZoomSDKMeetingUIController
 * @brief Class to control and customize Zoom meeting UI components.
 */
@interface ZoomSDKMeetingUIController :NSObject
{
    id<ZoomSDKMeetingUIControllerDelegate> _delegate;
}
/**
 * @brief Set the delegate to receive meeting UI related events.
 */
@property(nonatomic, assign, nullable) id<ZoomSDKMeetingUIControllerDelegate> delegate;

/**
 * @brief Whether to show components in meeting and meeting controls.
 * @param component The enum of meeting components.
 * @param window The pointer to the window. When the function calls successfully, the 'window' will be the meeting component window.
 * @param show YES means to show the components, NO to hide
 * @param inPanel YES means to show the components in panel, otherwise No.
 * @param componentFrame The frame of the meeting component.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */  
- (ZoomSDKError)showMeetingComponent:(MeetingComponent)component window:(NSWindow*_Nullable*_Nullable)window show:(BOOL)show InPanel:(BOOL)inPanel frame:(NSRect)componentFrame;

/**
 * @brief Enter full screen mode.
 * @param fullScreen Set it to YES to enter full screen, otherwise set it to NO. 
 * @param firstView Set it to YES to enter full screen mode on the first view, otherwise set it to NO.
 * @param secondView Set it to YES to enter full screen mode on the second view, otherwise set it to NO.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */ 
- (ZoomSDKError)enterFullScreen:(BOOL)fullScreen firstMonitor:(BOOL)firstView DualMonitor:(BOOL)secondView;
/**
 * @brief Switch to video wall view.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */ 
- (ZoomSDKError)switchToVideoWallView;
/**
 * @brief Switch to the mode of showing the current speaker.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)switchToActiveSpeakerView;
/**
 * @brief Determine if the meeting is in minimize window mode.
 * @return YES means that it is in minimize window mode, otherwise not.
 */ 
- (BOOL)isInMiniVideoMode;

/**
 * @brief Switch UI between mini video and mini bar(without video), it works only when isInMiniVideoMode = YES.
 * @return If the function succeeds, it will return the ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)switchMiniVideoModeUI;

/**
 * @brief Switch float video display mode to minimize mode.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)switchFloatVideoToMinimizeMode;
/**
 * @brief Switch float video display mode to wall mode.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)switchFloatVideoToWallMode;
/**
 * @brief Switch float video display mode to active speaker.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)switchFloatVideoToActiveSpeakerMode;
/**
 * @brief Switch float video display mode to gallery view mode.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)switchFloatVideoToGalleryMode;

/**
 * @brief Display or not the user who does not turn on the video in the video wall mode.
 * @return If the function succeeds, it will return the ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)hideOrShowNoVideoUserOnVideoWall:(BOOL)hide;

/**
 * @brief Get the page information in video wall mode.
 * @param currentPageNum The number of current page.
 * @param totalPageNum The number of total pages.
 * @return If the function succeeds, it will return the ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)getWallViewPageInfo:(int*)currentPageNum TotalPageNum:(int*)totalPageNum;

/**
 * @brief Display previous or following page of video wall in the meeting.
 * @param nextPage Set it to YES to display following page of video wall, NO to display previous page.
 * @return If the function succeeds, it will return the ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)showPreOrNextPageWallView:(BOOL)nextPage;

/**
 * @brief Active the principal window of meeting and place it on top.
 * @return If the function succeeds, it will return the ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)backToMeeting;

/**
 * @brief Determine when join meeting the video preview is display.
 * @return YES means that it is display video preview, otherwise not.
 */
- (BOOL)showVideoPreviewWhenJoinMeeting;

/**
 * @brief Display video preview when join meeting.
 * @param isShow Set it to YES to display video preview,otherwise not display.
 * @return If the function succeeds, it will return the ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)isShowVideoPreviewWhenJoinMeeting:(BOOL)isShow;

/**
 * @brief Display previous or following page of video view when in sharing.
 * @param nextPage Set it to YES to display following page of video view, NO to display previous page.
 * @return If the function succeeds, it will return the ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)pagingToPreOrNextViewWhenSharing:(BOOL)nextPage;

/**
 * @brief Set the invitation domain.
 * @param invitationDomain Specify the invitation domain.
 * @return If the function succeeds, it will return the ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)setCustomizedInvitationDomain:(NSString*)invitationDomain;

/**
 * @brief Allowing the developer to customize the URL of create/edit the polling.
 * @param urlString customized URL.
 * @param bCreate When bCreate is true, it changes the URL of creating a polling. Otherwise, it changes the URL of editing a polling.
 * @return If the function succeeds, it will return the ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)setCustomizedPollingUrl:(NSString*)urlString bCreate:(BOOL)bCreate;

/**
 * @brief Set the feedback url in the white board page.
 * @param feedbackUrl Specify the feedback url in the the white board page.
 * @return If the function succeeds, it will return the ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)setCloudWhiteboardFeedbackUrl:(NSString*)feedbackUrl;

/**
 * @brief Set the visibility of request local recording privilege dialog when attendee request local recording privilege. Default is displaying.
 * @param bHide YES indicates to hide the tab. NO to display the dialog.
 */
- (void)hideRequestRecordPrivilegeDialog:(BOOL)bHide;
@end
NS_ASSUME_NONNULL_END
