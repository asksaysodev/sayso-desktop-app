/**
 * @file ZoomSDKASController.h
 * @brief Interface for managing share and Zoom UI annotation functionalities.
 */

#import <ZoomSDK/ZoomSDKErrors.h>
#import <ZoomSDK/ZoomSDKCustomizedAnnotationCtr.h>
#import <ZoomSDK/ZoomSDKShareContainer.h>
#import <ZoomSDK/ZoomSDKRemoteControllerHelper.h>

NS_ASSUME_NONNULL_BEGIN
/**
 * @class ZoomSDKAnnotationController
 * @brief Controls Zoom UI annotation features in Zoom meetings, including setting tools, clearing annotations, and managing annotation states.
 */
@interface ZoomSDKAnnotationController :NSObject

/**
 * @brief Determine if annotation disabled.
 * @return YES if annotation is disabled, otherwise NO.
 */
- (BOOL)isAnnotationDisable;

/**
 * @brief Set annotation tool type.
 * @param type The specify annotation tool type.
 * @param screen Select the screen where you want to operate on.
 * @return If the function succeeds, the return value is @c ZoomSDKError_Success. Otherwise failed.
 * @warning The tool type AnnotationToolType_Picker and AnnotationToolType_SpotLight are not support for viewer.
 */
- (ZoomSDKError)setTool:(AnnotationToolType)type onScreen:(ScreenType)screen;

/**
 * @brief Clear the annotation of share screen.
 * @param type Specify the type of clear annotation.
 * @param screen Select the screen where you want to operate on.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)clear:(AnnotationClearType)type onScreen:(ScreenType)screen;

/**
 * @brief Set annotation color.
 * @param red The value of red.
 * @param green The value of green.
 * @param black The value of black.
 * @param screen Select the screen where you want to operate on.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)setColor:(float)red Green:(float)green Black:(float)black onScreen:(ScreenType)screen;

/**
 * @brief Set the width of the line for the annotation.
 * @param lineWidth The line width.
 * @param screen Select the screen where you want to operate on.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)setLineWidth:(long)lineWidth onScreen:(ScreenType)screen;

/**
 * @brief Undo annotation.
 * @param screen Select the screen where you want to operate on.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)undo:(ScreenType)screen;

/**
 * @brief Redo annotation.
 * @param screen Select the screen where you want to operate on.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)redo:(ScreenType)screen;

/**
 * @brief Determine whether the legal notice for annotation is available.
 * @return YES indicates the legal notice for annotation transcript is available. Otherwise not.
 */
- (BOOL)isAnnotationLegalNoticeAvailable;

/**
 * @brief Get the annotation legal notices prompt.
 * @return If the function succeeds, it will return the annotation legal notices prompt. Otherwise nil.
 */
- (NSString *)getAnnotationLegalNoticesPrompt;

/**
 * @brief Get the annotation legal notices explained.
 * @return If the function succeeds, it will return the annotation legal notices explained. Otherwise nil.
 */
- (NSString *)getAnnotationLegalNoticesExplained;
@end


/**
 * @class ZoomSDKSharingSourceInfo
 * @brief Represents information about a sharing source.
 */
@interface ZoomSDKSharingSourceInfo: NSObject
/**
 * @brief Unique ID of the sharing source.
 */
@property(nonatomic, assign, readonly) unsigned int              shareSourceID;
/**
 * @brief User ID of the sharer.
 */
@property(nonatomic, assign, readonly) unsigned int              userID;
/**
 * @brief Content type of the shared source.
 */
@property(nonatomic, assign, readonly) ZoomSDKShareContentType   contentType;
/**
 * @brief Current status of the sharing.
 */
@property(nonatomic, assign, readonly) ZoomSDKShareStatus        status;
/**
 * @brief Window ID of the shared application.
 */
@property(nonatomic, assign, readonly) CGWindowID                windowID;
/**
 * @brief Display ID of the shared desktop.
 */
@property(nonatomic, assign, readonly) CGDirectDisplayID         displayID;
/**
 * @brief Indicates if the content is shown on the first screen (Zoom UI mode only).
 */
@property(nonatomic, assign, readonly) BOOL                      isShowInFirstScreen;
/**
 * @brief Indicates if the content is shown on the second screen (Zoom UI mode only).
 */
@property(nonatomic, assign, readonly) BOOL                      isShowInSecondScreen;
/**
 * @brief Indicates whether the shared content can be remote controlled.
 */
@property(nonatomic, assign, readonly) BOOL                      canBeRemoteControl;
/**
 * @brief Determines if optimizing video sharing is enabled.
 */
@property(nonatomic, assign, readonly) BOOL                      bEnableOptimizingVideoSharing;

/**
 * @brief Get the window ID of the shared APP.
 * @param windowID A pointer to CGWindowID.
 */
- (ZoomSDKError)getWindowID:(CGWindowID*)windowID;

/**
* @brief Get the display ID on which that user wants to share the content.
* @param displayID A pointer of CGDirectDisplayID.
*/
- (ZoomSDKError)getDisplayID:(CGDirectDisplayID*)displayID;
@end


/**
 * @class ZoomSDKSplitScreenInfo
 * @brief Provides information about Zoom SDK split screen mode support and status.
 */
@interface ZoomSDKSplitScreenInfo : NSObject
{
    BOOL _isInSplitScreenMode;
    BOOL _isSupportSplitScreenMode;
}
/**
 * @brief Determine whether the current mode is split screen.
 * @return YES if in split screen mode. Otherwise NO.
 */
- (BOOL)isInSplitScreenMode;

/**
 * @brief Determine whether split screen mode is supported.
 * @return YES if split screen mode is supported. Otherwise NO.
 */
- (BOOL)isSupportSplitScreenMode;
@end

/**
 * @protocol ZoomSDKASControllerDelegate
 * @brief Delegate protocol for annotation and sharing events.
 */
@protocol ZoomSDKASControllerDelegate <NSObject>
@optional
/**
 * @brief Notification of the sharing status in the meeting.
 * @param shareInfo The object of ZoomSDKSharingSourceInfo.
 */
- (void)onSharingStatusChanged:(ZoomSDKSharingSourceInfo*)shareInfo;

/**
 * @brief Notification of failure to start sharing.
 */
- (void)onFailedToStartShare;

/**
 * @brief Notification if the share is locked by host/co-host.
 * @param shareLocked YES means the share is locked, otherwise not.
 * @deprecated This method is no longer used.
 */
- (void)onShareStatusLocked:(BOOL)shareLocked DEPRECATED_MSG_ATTRIBUTE("No longer used");

/**
 * @brief Notification of shared content is changed.
 * @param shareInfo The shared content, including window ID and monitor ID.
 */
- (void)onShareContentChanged:(ZoomSDKSharingSourceInfo*)shareInfo;

/**
 * @brief Designated for Zoom Meeting notify the sharing user has changed the viewer's annotation privilage.
 * @param isSupportAnnotation YES means the share source user enable the viewer do annotations. Otherwise not.
 * @param shareSourceID The share source ID that is sharing.
 */
- (void)onAnnotationSupportPropertyChanged:(BOOL)isSupportAnnotation shareSourceID:(unsigned int)shareSourceID;

/**
 * @brief Designated for Zoom Meeting notify the share Settings type changes.
 * @param type The share setting type.
 */
- (void)onShareSettingTypeChanged:(ZoomSDKShareSettingType)type;

/**
 * @brief The shared video’s playback has completed.
 */
- (void)onSharedVideoEnded;

/**
 * @brief Designated for the video file playback error.
 * @param error The error type.
 */
- (void)onVideoFileSharePlayError:(ZoomSDKVideoFileSharePlayError)error;

/**
 * @brief Designated for the optimize video property have changed.
 * @param shareInfo The share source information that has changed.
 */
- (void)onOptimizingShareForVideoClipStatusChanged:(ZoomSDKSharingSourceInfo*)shareInfo;
@end



/**
 * @class ZoomSDKASController
 * @brief The Zoom share controller responsible for managing screen sharing, annotation, and remote control functionalities.
 */
@interface ZoomSDKASController : NSObject
{
    id<ZoomSDKASControllerDelegate> _delegate;
    ZoomSDKShareContainer* _shareContainer;
    ZoomSDKAnnotationController* _annotationController;
    ZoomSDKCustomizedAnnotationCtr* _customizedAnnotationCtr;
    ZoomSDKRemoteControllerHelper*  _remoteControllerHelper;
}
/**
 * @brief The delegate to receive sharing and annotation event callbacks.
 */
@property(nonatomic, assign, nullable)id<ZoomSDKASControllerDelegate> delegate;
/**
 * @brief Enable or disable the shared computer sound, which will be applied when starting the share session.
*/
@property(nonatomic, assign, readwrite) BOOL enableShareComputerSound;
/**
 * @brief Enable or disable optimizing screen sharinge for a video clip, which will be applied when starting the share session.
*/
@property(nonatomic, assign, readwrite) BOOL enableOptimizingScreenShareForVideoClip;

/**
 * @brief Start to share application. 
 * @param windowID The App window id to be shared.
 * @return If the function succeeds, it will return ZoomSDKError_Succuss, otherwise not.
 */
- (ZoomSDKError)startAppShare:(CGWindowID)windowID;

/**
 * @brief Determine if the application's window ID can be shared.
 * @return YES indicates the current application window can be shared, otherwise means the app's window handle can't be shared.
 */
- (BOOL)isShareAppValid:(CGWindowID)windowID;

/**
 * @brief Start to share desktop.
 * @param monitorID The ID of the monitor that you want to display the shared content.
 * @return If the function succeeds, it will return ZoomSDKError_Succuss, otherwise not.
 */
- (ZoomSDKError)startMonitorShare:(CGDirectDisplayID)monitorID;

/**
 * @brief Stop the current share.
 * @return If the function succeeds, it will return ZoomSDKError_Succuss, otherwise not.
 */
- (ZoomSDKError)stopShare;

/**
 * @brief Get the ID of users who are sharing. 
 * @return An NSArray of user ID of all users who are sharing.
 */
- (NSArray<NSNumber *>* _Nullable)getViewableSharingUserList;

/**
 * @brief Get the list of sharing source info.
 * @param userID The ID of the user who is sharing.
 * @return If the function succeeds, the return value is a pointer to the NSArray<ZoomSDKSharingSourceInfo*>. Otherwise, the return value is nil.
 */
- (NSArray<ZoomSDKSharingSourceInfo *>* _Nullable)getSharingSourceInfoList:(unsigned int)userID;

/**
 * @brief View the user's shared content on the screen by the specified user ID.
 * @param shareSourceID The share source ID of user that you want to view the shared content.
 * @param screen Select the screen to display the shared content if you have more than one screen.
 * @return If the function succeeds, it returns @c ZoomSDKError_Succuss. Otherwise not.
 * @deprecated This method is no longer used.
 */
- (ZoomSDKError)viewShare:(unsigned int)shareSourceID onScreen:(ScreenType)screen DEPRECATED_MSG_ATTRIBUTE("No longer used");

/**
 * @brief Determine if it is able for user to start sharing. 
 * @return If the function succeeds, it will return YES, otherwise not.
 * @deprecated Use \link ZoomSDKASController::canStartShare: \endlink instead.
 */
- (BOOL)canStartShare DEPRECATED_MSG_ATTRIBUTE("Use -canStartShare: instead");

/**
 * @brief Determine whether the current meeting can start sharing.
 * @param reason The reason that no one can start sharing.
 * @return YES indicates you can start sharing, otherwise not.
 */
- (BOOL)canStartShare:(ZoomSDKCannotShareReasonType*)reason;

/**
 * @brief Determine if the share is locked by the host/co-host.
 * @return If the function succeeds, it will return YES, otherwise not.
 * @deprecated This method is no longer used.
 */
- (BOOL)isShareLocked DEPRECATED_MSG_ATTRIBUTE("No longer used");

/**
 * @brief Determine if it is able to share desktop in the current meeting.
 * @return If the function succeeds, it will return YES, otherwise not.
 */
- (BOOL)isDesktopSharingEnabled;

/**
 * @brief Get the controller of annotation tools.
 * @return The object of ZoomSDKAnnotationController.
 */
- (ZoomSDKAnnotationController*_Nullable)getAnnotationController;

/**
 * @brief Get the controller of annotation tools used in user custom interface mode.
 * @return The object of ZoomSDKCustomizedAnnotationCtr.
 */
- (ZoomSDKCustomizedAnnotationCtr*_Nullable)getCustomizedAnnotationCtr;

/**
 * @brief Get custom share container. 
 * @return If the function succeeds, it will return the object of ZoomSDKShareContainer, otherwise nil.
 */
- (ZoomSDKShareContainer*_Nullable)getShareContainer;

/**
 * @brief Get the class object of ZoomSDKRemoteControllerHelper.
 * @return If the function succeeds, it will return the object of ZoomSDKRemoteControllerHelper, otherwise nil.
 */
- (ZoomSDKRemoteControllerHelper*_Nullable)getRemoteControllerHelper;

/**
 * @brief Start annotation.
 * @param position The position of annotation toolbar. 
 * @param screen Specify the view where you want to place the annotation toolbar.
 * @return If the function succeeds, it will return @c ZoomSDKError_Succuss, otherwise not.
 * @deprecated Use \link ZoomSDKASController::startAnnotation: \endlink instead.
 */
- (ZoomSDKError)startAnnotation:(NSPoint)position onScreen:(ScreenType)screen DEPRECATED_MSG_ATTRIBUTE("Use -startAnnotation: instead");

/**
 * @brief Start annotation.
 * @param screen Specify the view where you want to place the annotation toolbar.
 * @return If the function succeeds, it will return ZoomSDKError_Succuss, otherwise not.
 */
- (ZoomSDKError)startAnnotation:(ScreenType)screen;

/**
 * @brief Stop annotation.
 * @param screen Specify the view on which you want to stop annotating.
 * @return If the function succeeds, it will return ZoomSDKError_Succuss, otherwise not.
 */
- (ZoomSDKError)stopAnnotation:(ScreenType)screen;

/**
 * @brief Get the user ID of current remote controller.
 * @param userID The ID of user who can remotely control others.
 * @return If the function succeeds, it will return @c ZoomSDKError_Succuss, otherwise not.
 * @deprecated This method is no longer used.
 */
- (ZoomSDKError)getCurrentRemoteController:(unsigned int*)userID DEPRECATED_MSG_ATTRIBUTE("No longer used");

/**
 * @brief Get the information of split screen when viewing the share in the meeting.
 * @return If the function succeeds, it will return a pointer to a ZoomSDKSplitScreenInfo object. Otherwise nil.
 */
- (ZoomSDKSplitScreenInfo*_Nullable)getCurrentSplitScreenModeInfo;

/**
 * @brief Switch to split screen mode, which means that the shared content and the video are separated in different column, the video won't cover the shared content.
 * @param switchTo YES means to enable side by side mode, otherwise not.
 * @return If the function succeeds, it will return @c ZoomSDKError_Succuss, otherwise not.
 */
- (ZoomSDKError)switchSplitScreenMode:(BOOL)switchTo;

/**
 * @brief Clean up as-controller object.
 */
- (void)cleanUp;

/**
 * @brief Share white-board.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)startWhiteBoardShare;

/**
 * @brief Start sharing a portion of screen by a frame. User can resize the shared range during sharing.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)startFrameShare;

/**
 * @brief Share audio.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)startAudioShare;

/**
 * @brief Share camera, only available for custom UI.
 * @param deviceID The ID of the camera to be shared.
 * @param window The view on which you want to show camera content.
 * @return If the function succeeds, it will return ZoomSDKError_Succuss. Oherwise not.
 */
- (ZoomSDKError)startShareCamera:(NSString*)deviceID displayWindow:(NSWindow*)window;

/**
 * @brief Share camera, only available for zoom UI.
 * @return If the function succeeds, it will return ZoomSDKError_Succuss, otherwise not.
 */
- (ZoomSDKError)startShareCamera;

/**
 * @brief Determine if the user can share the next camera.
 * @return If the function succeeds, it will return YES. Otherwise not.
 */
- (BOOL)canSwitchToShareNextCamera;
/**
 * @brief Share the next camera.
 * @return If the function succeeds, it will return ZoomSDKError_Succuss, otherwise not.
 */
- (ZoomSDKError)switchToShareNextCamera;

/**
 * @brief Share video file.
 * @param filePath Specify the video file path. Only supports mov, mp4, or avi format.
 * @return If the function succeeds, it returns ZoomSDKError_Succuss. Otherwise not.
 */
- (ZoomSDKError)startVideoFileShare:(NSString*)filePath;

/**
 * @brief Share video file. Only available for custom UI.
 * @param filePath Specify the video file path. Only supports mov, mp4, or avi format.
 * @param window The view where you want to show share content.
 * @return If the function succeeds, it will return ZoomSDKError_Succuss. Oherwise not.
 */
- (ZoomSDKError)startVideoFileShare:(NSString*)filePath displayWindow:(NSWindow*)window;

/**
 * @brief Determine if user can share white-board.
 * @return If the function succeeds, it will return YES, otherwise not.
 */
- (BOOL)isAbleToShareWhiteBoard;
/**
 * @brief Determine if user can share a potion of screen.
 * @return If the function succeeds, it will return YES, otherwise not.
 */
- (BOOL)isAbleToShareFrame;
/**
 * @brief Determine if user can share computer audio.
 * @return If the function succeeds, it will return YES, otherwise not.
 */
- (BOOL)isAbleToShareComputerAudio;
/**
 * @brief Determine if user can share camera.
 * @return If the function succeeds, it will return YES, otherwise not.
 */
- (BOOL)isAbleToShareCamera;

/**
 * @brief Determine if users can share video files.
 * @return If the function succeeds, it returns YES. Otherwise the function fails and returns NO.
 */
- (BOOL)canShareVideoFile;

/**
 * @brief Determine whether the user can share to the breakout room.
 * @return YES indicates that the user can share to the breakout room. NO indicates that the function fails.
 * @note Valid for user custom interface mode only.
 */
- (BOOL)canEnableShareToBO;

/**
 * @brief Determine if sharing to the breakout room is enabled.
 * @return YES indicates that the sharing is locked.
 * @note Valid for user custom interface mode only.
 */
- (BOOL)isShareToBOEnabled;

/**
 * @brief Set to enable sharing to the breakout room
 * @param bEnable YES indicates to enable. NO indicates that sharing to the breakout room is not enabled.
 * @return If the function succeeds, the return value is @c ZoomSDKError_Success. Otherwise failed.
 * @note Valid for user custom interface mode only.
 */
- (ZoomSDKError)enableShareToBO:(BOOL)bEnable;

/**
 * @brief This method is used for the sharing user to disable/enable viewer's privilege of annotation.
 * @param screenType Select the screen where you want to operate on.
 * @param disable YES means disable viewer's annotation privilege, NO means enable.
 * @return A ZoomSDKError to tell client function call successful or not.
 */
- (ZoomSDKError)disableViewerAnnotation:(ScreenType)screenType disable:(BOOL)disable;

/**
 * @brief Determine whether the viewer's annotate privilege is locked.
 * @param screenType Select the screen where you want to operate on.
 * @param locked A point to A BOOL, if function call successfully, the value of 'locked' means whether viewer's annotate privilege is locked, YES means viewer's annotate privilege is locked.
 * @return A ZoomSDKError to tell client function call successful or not.
 */
- (ZoomSDKError)isViewerAnnotationLocked:(ScreenType)screenType isLocked:(BOOL*)locked;

/**
 * @brief Determine if it is able for user to disable viewer's annotation privilege.
 * @param screenType Select the screen where you want to operate on.
 * @param canDisable A point to A BOOL, if function call successfully, the value of 'canDisable' means whether the user can disable viewer's annotation, YES means can disable, NO means cannot.
 * @return A ZoomSDKError to tell client function call successful or not.
 */
- (ZoomSDKError)canDisableViewerAnnotation:(ScreenType)screenType canDisabled:(BOOL*)canDisable;

/**
 * @brief Determine if it is able for user to do annotation.
 * @param screenType Select the screen where you want to operate on.
 * @param canAnnotate A point to A BOOL, if function call successfully, the value of 'canAnnotate' means whether the user can do annotation, YES means can do annotation, NO means cannot.
 * @return A ZoomSDKError to tell client function call successful or not.
 */
- (ZoomSDKError)canDoAnnotation:(ScreenType)screenType canAnnotate:(BOOL*)canAnnotate;

/**
 * @brief Determine if support enable or disable optimizing for full screen video clip.
 * @return If supported, this returns YES. Otherwise returns NO.
 */
- (BOOL)isSupportEnableOptimizeForFullScreenVideoClip;

/**
 * @brief Determine if sharing computer sound option is on or off.
 * @return If enabled, this returns YES. Otherwise returns NO.
 * @deprecated Use \link ZoomSDKASController::isEnableShareComputerSoundOnWhenSharing \endlink instead.
 */
- (BOOL)isEnableShareComputerSoundOn DEPRECATED_MSG_ATTRIBUTE("Use -isEnableShareComputerSoundOnWhenSharing instead");

/**
 * @brief Determine if sharing computer sound option is on or off for the current sharing session.
 * @return If enabled, this returns YES. Otherwise returns NO.
 */
- (BOOL)isEnableShareComputerSoundOnWhenSharing;

/**
 * @brief Determine if optimizing for full screen video clip option is on or off.
 * @return If enabled, this returns YES. Otherwise returns NO.
 * @deprecated Use \link ZoomSDKASController::isEnableOptimizeForFullScreenVideoClipOnWhenSharing \endlink instead.
 */
- (BOOL)isEnableOptimizeForFullScreenVideoClipOn DEPRECATED_MSG_ATTRIBUTE("Use -isEnableOptimizeForFullScreenVideoClipOnWhenSharing instead");

/**
 * @brief Determine if optimizing for full screen video clip option is on or off for the current sharing session.
 * @return If enabled, this returns YES. Otherwise returns NO.
 */
- (BOOL)isEnableOptimizeForFullScreenVideoClipOnWhenSharing;

/**
 * @brief Determine if the specified share type supports sharing with computer sound.
 * @param type The type of shring content.
 * @return If supported, this return YES. Otherwise return NO.
 */
- (BOOL)isSupportShareWithComputerSound:(ZoomSDKShareContentType)type;

/**
 * @brief Determine if current share support sharing with computer sound.
 * @return If supported, this return YES. Otherwise return NO.
 */
- (BOOL)isCurrentSharingSupportShareWithComputerSound;

/**
 * @brief Determine enable share computer sound.
 * @param enable Enable or disable share computer sound.
 * @return A ZoomSDKError to tell client function call successful or not.
 */
- (ZoomSDKError)enableShareComputerSound:(BOOL)enable;

/**
 * @brief Determine enable optimizing for full screen video clip.
 * @param enable Enable or disable optimizing for full screen video clip.
 * @return A ZoomSDKError to tell client function call successful or not.
 */
- (ZoomSDKError)enableOptimizingScreenShareForVideoClip:(BOOL)enable;

/**
 * @brief Set the audio share mode.
 * @param mode The mode for audio share.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)setAudioShareMode:(ZoomSDKAudioShareMode)mode;

/**
 * @brief Get the audio share mode.
 * @param mode The mode for audio share.
 * @return If the function succeeds, it will return ZoomSDKError_Success, the param mode will be the current audio share mode.
 */
- (ZoomSDKError)getAudioShareMode:(ZoomSDKAudioShareMode*)mode;

/**
 * @brief Determine enable share computer sound when shaing.
 * @param enable Enable or disable share computer sound.
 * @return A ZoomSDKError to tell client function call successful or not.
 */
- (ZoomSDKError)enableShareComputerSoundWhenSharing:(BOOL)enable;

/**
 * @brief Determine enable optimizing for full screen video clip when shaing.
 * @param enable Enable or disable optimizing for full screen video clip.
 * @return A ZoomSDKError to tell client function call successful or not.
 */
- (ZoomSDKError)enableOptimizingScreenShareForVideoClipWhenSharing:(BOOL)enable;

/**
 * @brief Determine whether the legal notice for whiteboard is available.
 * @return YES indicates the legal notice for whiteboard is available. Otherwise not.
 */
- (BOOL)isWhiteboardLegalNoticeAvailable;

/**
 * @brief Get the whiteboard legal notices prompt.
 * @return If the function succeeds, it will return the whiteboard legal notices prompt. Otherwise nil.
 */
- (NSString *)getWhiteboardLegalNoticesPrompt;

/**
 * @brief Get the whiteboard legal notices explained.
 * @return If the function succeeds, it will return the whiteboard legal notices explained. Otherwise nil.
 */
- (NSString *)getWhiteboardLegalNoticesExplained;

/**
 * @brief Switch the window size by the function when watching the share on the specified view.
 * @param shareSourceID Specify the sharing source ID that you want to switch the zoom ratio.
 * @param zoomRatio Specify the size you want to set. The default is ZoomSDKShareViewZoomRatio_50, which means share content will zoom-out to 50%.
 * @return If the function succeeds, it returns ZoomSDKError_Success. Otherwise the function fails.
 */
- (ZoomSDKError)switchZoomRatioWhenViewShare:(unsigned int)shareSourceID zoomRatio:(ZoomSDKShareViewZoomRatio)zoomRatio;

/**
 * @brief Enable following the presenter's pointer by the function when watching the shared content on the specified view.
 * @param shareSourceID Specify the sharing source ID that you want to follow the presenter's pointer.
 * @param enable YES indicates to enable following the pointer. Otherwise not.
 * @return If the function succeeds, it returns ZoomSDKError_Success. Otherwise the function fails.
 */
- (ZoomSDKError)enableFollowPresenterPointerWhenViewShare:(unsigned int)shareSourceID enable:(BOOL)enable;

/**
 * @brief Determine if you can enable following the presenter's pointer when watching the share on the specified view.
 * @param shareSourceID Specify the sharing source ID that you want to follow the presenter's pointer.
 * @param bCan YES indicates that following the pointer can be enabled. NO indicates that it can’t.
 * @return If the function succeeds, it returns ZoomSDKError_Success. Otherwise the function fails.
*/
- (ZoomSDKError)canEnableFollowPresenterPointerWhenViewShare:(unsigned int)shareSourceID bCan:(BOOL*)bCan;
@end
NS_ASSUME_NONNULL_END

