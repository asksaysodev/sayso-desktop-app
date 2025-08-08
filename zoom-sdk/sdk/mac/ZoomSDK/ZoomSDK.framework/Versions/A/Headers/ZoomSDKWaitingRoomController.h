/**
 * @file ZoomSDKWaitingRoomController.h
 * @brief Interfaces for managing and customizing the waiting room in Zoom SDK.
 */


NS_ASSUME_NONNULL_BEGIN
@class ZoomSDKUserInfo;

/**
 * @class ZoomSDKWaitingRoomBrandingPageColor
 * @brief Branding background color configuration for the waiting room.
 */
@interface ZoomSDKWaitingRoomBrandingPageColor : NSObject
/**
 * @brief The background color.
 */
@property(retain, nonatomic, readonly,nullable)NSColor* backgroundColor;
@end


/**
 * @class ZoomSDKWaitingRoomBrandingTextColor
 * @brief Branding text color configuration for the waiting room.
 */
@interface ZoomSDKWaitingRoomBrandingTextColor : NSObject
/**
 * @brief The primary color.
 */
@property(retain, nonatomic, readonly, nullable)NSColor* primaryColor;
/**
 * @brief The secondary color.
 */
@property(retain, nonatomic, readonly,nullable)NSColor* secondaryColor;
/**
 * @brief The paragraph color.
 */
@property(retain, nonatomic, readonly,nullable)NSColor* paragraphColor;
/**
 * @brief The hyper link color.
 */
@property(retain, nonatomic, readonly,nullable)NSColor* hyperlinkColor;
@end


/**
 * @class ZoomSDKWaitingRoomBrandingButtonColor
 * @brief Branding button color configuration for the waiting room.
 */
@interface ZoomSDKWaitingRoomBrandingButtonColor : NSObject
/**
 * @brief The primary Button color.
 */
@property(retain, nonatomic, readonly,nullable)NSColor* primaryButtonColor;
@end


/**
 * @class ZoomSDKCustomWaitingRoomData
 * @brief Custom data configuration for the waiting room.
 */
@interface ZoomSDKCustomWaitingRoomData : NSObject
/**
 * @brief The title.
 */
@property (copy, nonatomic, readonly, nullable) NSString* title;
/**
 * @brief The description.
 */
@property (copy, nonatomic, readonly, nullable) NSString* dataDescription;
/**
 * @brief The path of logo image.
 */
@property (copy, nonatomic, readonly, nullable) NSString* logoPath;
/**
 * @brief The path of video file.
 */
@property (copy, nonatomic, readonly, nullable) NSString* videoPath;
/**
 * @brief The path of image file.
 */
@property (copy, nonatomic, readonly, nullable) NSString* imagePath;
/**
 * @brief The type.
 */
@property (assign, nonatomic, readonly) ZoomSDKWaitingRoomLayoutType type;
/**
 * @brief The status.
 */
@property (assign, nonatomic, readonly) ZoomSDKCustomWaitingRoomDataStatus status;
/**
 * @brief The object of waiting room branding page color.
 */
@property (retain, nonatomic, readonly) ZoomSDKWaitingRoomBrandingPageColor* pageColor;
/**
 * @brief The object of waiting room branding text color.
 */
@property (retain, nonatomic, readonly) ZoomSDKWaitingRoomBrandingTextColor* textColor;
/**
 * @brief The object of waiting room branding button color.
 */
@property (retain, nonatomic, readonly) ZoomSDKWaitingRoomBrandingButtonColor* buttonColor;
@end


/**
 * @class ZoomSDKWaitingRoomDataDownloadHandler
 * @brief Handler for retrying or ignoring waiting room custom data download failures.
 */
@interface ZoomSDKWaitingRoomDataDownloadHandler : NSObject
/**
 * @brief Retry to download the waiting room customize data information in the waiting room.
 */
- (void)retry;
/**
 * @brief Ignore to downloade the waiting room customize data information in the waiting room.
 */
- (void)ignore;
@end


/**
 * @protocol ZoomSDKWaitingRoomDelegate
 * @brief Protocol for receiving events related to the waiting room.
 */
@protocol ZoomSDKWaitingRoomDelegate <NSObject>
/**
 * @brief Callback of that user joins waiting room.
 * @param userid The ID of user who joins waiting room.
 */
- (void)onUserJoinWaitingRoom:(unsigned int)userid;
/**
 * @brief Callback of that user leaves waiting room.
 * @param userid The ID of user who leaves waiting room.
 */
- (void)onUserLeftWaitingRoom:(unsigned int)userid;

/**
 * @brief During the waiting room, this callback event will be triggered when host change audio status.
 * @param audioCanTurnOn YES means audio can be turned on. Otherwise not.
 */
- (void)onWaitingRoomPresetAudioStatusChanged:(BOOL)audioCanTurnOn;

/**
 * @brief During the waiting room, this callback event will be triggered when host change video status.
 * @param videoCanTurnOn YES means video can be turned on. Otherwise not.
 */
- (void)onWaitingRoomPresetVideoStatusChanged:(BOOL)videoCanTurnOn;

/**
 * @brief Callback of that customize data info updated.
 * @param bData The object of ZoomSDKCustomWaitingRoomData.
 * @param handle The object of ZoomSDKWaitingRoomDataDownloadHandler to handle download failed.
 */
- (void)onCustomWaitingRoomDataUpdated:(ZoomSDKCustomWaitingRoomData*_Nullable)bData handle:(ZoomSDKWaitingRoomDataDownloadHandler*_Nullable)handle;

/**
 * @brief Callback indicating that the name of a user in the waiting room has changed.
 * @param userID The ID of the user whose user name has changed.
 * @param userName The new user name.
 */
- (void)onWaitingRoomUserNameChanged:(unsigned int)userID userName:(NSString *)userName;

/**
 * @brief This callback event will be triggered when host or cohost enables or disables waiting room entrance.
 * @param enabled True means enables waiting room entrance, false means disables waiting room entrance.
 */
- (void)onWaitingRoomEntranceEnabled:(BOOL)enabled;
@end


/**
 * @class ZoomSDKWaitingRoomController
 * @brief Interface for managing the waiting room during a meeting.
 */
@interface ZoomSDKWaitingRoomController : NSObject
{
    id<ZoomSDKWaitingRoomDelegate> _delegate;
}
/**
 * @brief Set the delegate to receive waiting room events.
 */
@property (assign, nonatomic, nullable) id<ZoomSDKWaitingRoomDelegate> delegate;
/**
 * @brief Query if the meeting supports waiting room.
 * @return YES means supported, otherwise not.
 */
- (BOOL)isSupportWaitingRoom;

/**
 * @brief Query if \link ZoomSDKWaitingRoomController::enableWaitingRoomOnEntry: \endlink feature locked.
 * @return YES means enabled, otherwise not.
 */
- (BOOL)isWaitingRoomOnEntryLocked;

/**
 * @brief Query if waiting room is enabled in current meeting.
 * @return YES means enabled, otherwise not.	
 */
- (BOOL)isEnableWaitingRoomOnEntry;
/**
 * @brief Set to enable/disable waiting room.
 * @param enable YES means enabled, NO disabled.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)enableWaitingRoomOnEntry:(BOOL)enable;
/**
 * @brief Get the list of users in the waiting room.
 * @return If the function succeeds, it will return a NSArray.
 */
- (NSArray*_Nullable)getWaitRoomUserList;
/**
 * @brief Get the information of users in the waiting room.
 * @param userid The ID of user who is in the waiting room.
 * @return If the function succeeds, it will return the object of ZoomSDKUserInfo for the specified user.
 */
- (ZoomSDKUserInfo*_Nullable)getWaitingRoomUserInfo:(unsigned int)userid;
/**
 * @brief Admit user to join meeting.
 * @param userid The ID of user who joins meeting.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)admitToMeeting:(unsigned int)userid;

/**
 * @brief  Permit all of the users currently in the waiting room to join the meeting.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)admitAllToMeeting;

/**
 * @brief Put user into waiting room.
 * @param userid The ID of user who is put into waiting room by host/co-host.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)putIntoWaitingRoom:(unsigned int)userid;

/**
 * @brief Determine if the attendee is enabled to turn on audio when joining the meeting.
 * @return True indicates to enable to turn on.
 */
- (BOOL)isAudioEnabledInWaitingRoom;

/**
 @brief Pre-set audio mute or unmute status in waiting room.
 @param muteAudio Pre-set audio mute or unmute status.
 @return @c ZoomSDKError_Success means the pre-set success, otherwise failed.
 @warning Only worked out of meeting.
 */
- (ZoomSDKError)presetAudioInWaitingRoom:(BOOL)muteAudio;

/**
 @brief Get the audio pre-set mute or unmute status in waiting room.
 @return YES means pre-set unmute, NO means pre-set mute.
 @warning Only worked out of meeting.
 */
- (BOOL)isPresetAudioUnmuteInWaitingRoom;

/**
 * @brief Determine if the attendee is enabled to turn on video when joining the meeting.
 * @return True indicates to enable to turn on.
 */
- (BOOL)isVideoEnabledInWaitingRoom;

/**
 @brief Pre-set video mute or unmute status in waiting room.
 @param muteVideo Pre-set video mute or unmute status.
 @return @c ZoomSDKError_Success means the pre-set succeeded, otherwise failed.
 @warning Only worked out of meeting.
 */
- (ZoomSDKError)presetVideoInWaitingRoom:(BOOL)muteVideo;

/**
 @brief Get the video pre-set mute or unmute status in waiting room.
 @return YES means pre-set unmute, NO means pre-set mute.
 @warning Only worked out of meeting.
 */
- (BOOL)isPresetVideoUnmuteInWaitingRoom;

/**
 * @brief Get the WaitingRoom CustomizeData information in the waiting room.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)requestCustomWaitingRoomData;

/**
 * @brief Determine if the host or cohost can rename users in the waiting room.
 * @return YES means the host or cohost can rename users in the waiting room. Otherwise they can’t.
 */
- (BOOL)canRenameUser;

/**
 * @brief Change a user's screen name in the waiting room.
 * @param userID The ID of users put into the waiting room by a host or cohost.
 * @param userName The new user name.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)renameUser:(unsigned int)userID newUserName:(NSString *)userName;

/**
 * @brief Determine if a host or cohost can expel users in the waiting room.
 * @return YES means that a host or cohost can expel users in the waiting room. Otherwise they may not.
 */
- (BOOL)canExpelUser;

/**
 * @brief Remove a specified user from the waiting room.
 * @param userID The ID of the user  removed from the waiting room by a host or cohost.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)expelUser:(unsigned int)userID;
@end
NS_ASSUME_NONNULL_END
