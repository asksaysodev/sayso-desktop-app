/**
 * @file ZoomSDKMeetingActionController.h
 * @brief Interfaces for managing meeting actions  in Zoom meetings.
 */


#import <ZoomSDK/ZoomSDKErrors.h>
#import <ZoomSDK/ZoomSDKMeetingChatController.h>
NS_ASSUME_NONNULL_BEGIN

/**
 * @brief This class provides properties to specify and manage the privileges associated with different types of assets when assigning roles such as co-host or host in a meeting.
 *
 * @note \link ZoomSDKGrantCoOwnerAssetsInfo \endlink objects must be obtained through the \link getGrantCoOwnerAssetsInfo \endlink interface provided by the SDK. Manual creation is not allowed.
 */
@interface ZoomSDKGrantCoOwnerAssetsInfo : NSObject
/**
 * @brief Indicates whether the specified privilege is granted for managing the asset. This is a writable property that can be set to YES or NO.
 */
@property (assign, nonatomic) BOOL isGranted;
/**
 * @brief Specifies the type of asset (such as smart summary, cloud recording) for which the privilege applies. This is a read-only property.
 */
@property (assign,nonatomic, readonly) ZoomSDKGrantCoOwnerAssetsType assetType;
/**
 * @brief Indicates whether the asset is locked, preventing any modifications. This is a read-only property.
 */
@property (assign,nonatomic, readonly) BOOL isAssetsLocked;
@end

/**
 * @class ZoomSDKMultiToSingleShareConfirmHandler
 * @brief Handles confirmation for switching from multi-share to single-share mode during screen sharing.
 */
@interface ZoomSDKMultiToSingleShareConfirmHandler : NSObject
/**
 * @brief Cancel to switch to single share from multi-share. All the shares are remained.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)cancelSwitch;
/**
 * @brief Confirm to switch to single share from multi-share. All the shares are stopped.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)confirmSwitch;
@end

/**
 * @class ZoomSDKChatInfo
 * @brief Represents a chat message in the meeting.
 */
@interface ZoomSDKChatInfo : NSObject
{
    unsigned int                      _sendID;
    unsigned int                      _receiverID;
    NSString*                         _sendName;
    NSString*                         _receiverName;
    NSString*                         _content;
    time_t                            _timestamp;
    BOOL                              _isChatToWaitingRoom;
    ZoomSDKChatMessageType            _chatMessageType;
}
/**
 * @brief Get the user ID of whom sending message.
 * @return If the function succeeds, the return value is the user ID of sender.
 */
- (unsigned int)getSenderUserID;
/**
 * @brief Get the screen name of the sender.
 * @return If the function succeeds, the return value is the screen name.
 */
- (NSString*)getSenderDisplayName;
/**
 * @brief Get the user ID of whom receiving the message.
 * @return If the function succeeds, the return value is the user ID.
 */
- (unsigned int)getReceiverUserID;
/**
 * @brief Get the screen name of receiver.
 * @return If the function succeeds, the return value is the screen name.
 */
- (NSString*)getReceiverDisplayName;
/**
 * @brief Get the content of message.
 * @return If the function succeeds, the return value is the content of message.
 */
- (NSString*)getMsgContent;
/**
 * @brief Get the timestamps of the current message.
 * @return If the function succeeds, the return value is the timestamps of the current message. 
 */
- (time_t)getTimeStamp;
/**
 * @brief The current message is send to waiting room.
 * @return If return YES means the message is send to waiting room, otherwise not.
 */
- (BOOL)isChatToWaitingRoom;
/**
 * @brief Get the type of the current message.
 * @return If the function succeeds, the return value is the enum of ZoomSDKChatMessageType.
 */
- (ZoomSDKChatMessageType)getChatMessageType;
/**
 * @brief Get chat message ID.
 * @return If the function succeeds, the return value is the ID of chat message.
 */
- (NSString*)getMessageID;

/**
 * @brief Determine if the current message is a reply to another message.
 * @return YES indicates that the current message is a reply to another message. Otherwise the function fails and the message is a standalone message.
 */
- (BOOL)isComment;

/**
 * @brief Determine if the current message is part of a message thread, and can be directly replied to.
 * @return YES indicates that the current message is a part of a message thread. Otherwise the function fails and return NO.
 */
- (BOOL)isThread;

/**
 * @brief Get the current message’s chat message font style list.
 * @deprecated This method is no longer used.
 */
- (NSArray<ZoomSDKRichTextStyleItem *> *_Nullable)getTextStyleItemList DEPRECATED_MSG_ATTRIBUTE("No longer used");

/**
 * @brief Get the list of segment details in the current message.
 * @return If the function succeeds, the return value is an array of ZoomSDKChatMsgSegmentDetails objects representing rich text segments in the message. Otherwise nil.
 */
- (NSArray<ZoomSDKChatMsgSegmentDetails *> *_Nullable)getSegmentDetails;

/**
 * @brief Get the current message’s thread ID.
 * @return If the function succeeds, the return value is the current message’s thread ID. Otherwise the function fails and the return value is the string of length zero(0).
 */
- (NSString*)getThreadID;
@end

/**
 * @class ZoomSDKNormalMeetingChatPrivilege
 * @brief Represents the chat privileges of a participant in a normal meeting.
 */
@interface ZoomSDKNormalMeetingChatPrivilege : NSObject
/**
 * YES indicates that the user owns the authority to send message to chat.
 */
@property (assign,nonatomic,readonly) BOOL canChat;
/**
 * YES indicates that the user owns the authority to send message to all.
 */
@property (assign,nonatomic,readonly) BOOL canChatToAll;
/**
 * YES indicates that the user owns the authority to send message to an individual attendee in the meeting.
 */
@property (assign,nonatomic,readonly) BOOL canChatToIndividual;
/**
 * YES indicates that the user owns the authority to send message only to the host.
 */
@property (assign,nonatomic,readonly) BOOL isOnlyCanChatToHost;
@end

/**
 * @class ZoomSDKWebinarAttendeeChatPrivilege
 * @brief Represents the chat privileges of a webinar attendee in a meeting.
 */
@interface ZoomSDKWebinarAttendeeChatPrivilege : NSObject
/**
 * YES indicates that the attendee can send message to chat.
 */
@property (assign,nonatomic,readonly) BOOL canChat;
/**
 * YES indicates that the user owns the authority to send message to all the panelists and attendees.
 */
@property (assign,nonatomic,readonly) BOOL canChatToAllPanellistAndAttendee;
/**
 * YES indicates that the user owns the authority to send message to all the panelists.
 */
@property (assign,nonatomic,readonly) BOOL canChatToAllPanellist;
@end

/**
 * @class ZoomSDKWebinarPanelistChatPrivilege
 * @brief Represents the chat privileges of a webinar panelist in a meeting.
 */
@interface ZoomSDKWebinarPanelistChatPrivilege : NSObject
/**
 * YES indicates that the user owns the authority to send message to all the panelists.
 */
@property (assign,nonatomic,readonly) BOOL canChatToAllPanellist;
/**
 * YES indicates that the user owns the authority to send message to all.
 */
@property (assign,nonatomic,readonly) BOOL canChatToAllPanellistAndAttendee;
/**
 * YES indicates that the user owns the authority to send message to individual attendee.
 */
@property (assign,nonatomic,readonly) BOOL canChatToIndividual;
@end

/**
 * @class ZoomSDKChatStatus
 * @brief Provides access to the chat privileges for different user roles in a meeting or webinar.
 */
@interface ZoomSDKChatStatus : NSObject
/**
 * @brief Get the chat privilege of the meeting participant.
 * @return The chat privilege of the meeting participant.
 */
- (ZoomSDKNormalMeetingChatPrivilege *)getNormalMeetingPrivilege;
/**
 * @brief Get the chat privilege of the webinar attendee.
 * @return The chat privilege of the webinar attendee.
 */
- (ZoomSDKWebinarAttendeeChatPrivilege *)getWebinarAttendeePrivilege;
/**
 * @brief Get the chat privilege of the webinar panelist.
 * @return The chat privilege of the webinar panelist.
 */
- (ZoomSDKWebinarPanelistChatPrivilege *)getWebinarPanelistPrivilege;
/**
 * @brief current meeting support chat.
 * @return If return YES means current meeting support chat feature, otherwise not.
 */
- (BOOL)isSupportChat;
/**
 * @brief current role is a webinar attendee.
 * @return If return YES means current role is a webinar attendee, otherwise not.
 */
- (BOOL)isWebinarAttendee;
/**
 * @brief current meeting is a webinar meeting.
 * @return If return YES means current meeting is a webinar meeting, otherwise not.
 */
- (BOOL)isWebinarMeeting;
@end

/**
 * @class ZoomSDKUserAudioStatus
 * @brief Represents a user's audio connection status and audio type in the meeting.
 */
@interface ZoomSDKUserAudioStatus : NSObject
{
    unsigned int _userID;
    ZoomSDKAudioStatus _status;
    ZoomSDKAudioType _type;
}
/**
 * @brief Get the user ID.
 * @return If the function succeeds, it will return user ID.
 */
- (unsigned int)getUserID;
/**
 * @brief Get the status of the audio.
 * @return The audio status.
 */
- (ZoomSDKAudioStatus)getStatus;
/**
 * @brief Get the audio type of the user.
 * @return The audio type.
 */
- (ZoomSDKAudioType)getType;
@end


/**
 * @class ZoomSDKWebinarAttendeeStatus
 * @brief Represents the webinar attendee status in a meeting.
 */
@interface ZoomSDKWebinarAttendeeStatus : NSObject
{
    BOOL _isAttendeeCanTalk;
}
/**
 * @brief Indicates whether the webinar attendee is allowed to talk.
 */
@property(nonatomic, assign)BOOL  isAttendeeCanTalk;
@end


/**
 * @class ZoomSDKVirtualNameTag
 * @brief Represents a virtual name tag.
 */
@interface ZoomSDKVirtualNameTag : NSObject
/**
 * @brief Tag ID. tagID is the unique identifier. The range of tagID is 0-1024.
 */
@property (nonatomic, assign) int tagID;

/**
 * @brief Tag name.
 */
@property (nonatomic, copy, nullable) NSString *tagName;
@end


/**
 * @class ZoomSDKUserInfo
 * @brief Provides detailed information about a user in a  meeting.
 */
@interface ZoomSDKUserInfo :NSObject
{
    unsigned int _userID;
}
/**
 * @brief Determine if the information corresponds to the current user.
 * @return YES means that the information corresponds to the current user, otherwise not.
 */
- (BOOL)isMySelf;
/**
 * @brief Get the username matched with the current user information.
 * @return The username.
 */
- (NSString*_Nullable)getUserName;
/**
 * @brief Get the user ID matched with the current user information.
 * @return The user ID. 
 */
- (unsigned int)getUserID;

/**
 * @brief Get the storage path of avatar.
 * @return The path to store the head portrait.
 */
- (NSString*_Nullable)getAvatarPath;

/**
 * @brief Determine whether the member corresponding with the current information is the host or not.
 * @return YES means host.
 */
- (BOOL)isHost;
/**
 * @brief Determine the video status of the user specified by the current information.
 * @return YES means that the video is turned on.
 */
- (BOOL)isVideoOn;

/**
 * @brief Get the audio status of user.
 * @return The audio status of user.
 */
- (ZoomSDKAudioStatus)getAudioStatus;
/**
 * @brief Get the audio type of user.
 * @return The audio type of user.
 */
- (ZoomSDKAudioType)getAudioType;
/**
 * @brief Get the type of role of the user specified by the current information.
 * @return The role of the user.
 */
- (UserRole)getUserRole;
/**
 * @brief Determine whether the user corresponding to the current information joins the meeting by telephone or not.
 * @return YES indicates that the user joins the meeting by telephone.
 */
- (BOOL)isPurePhoneUser;
/**
 * @brief Determine whether the user corresponding to the current information joins the meeting by h323 or not.
 * @return YES indicates that the user joins the meeting by h323.
 */
- (BOOL)isH323User;
/**
 * @brief Determine if it is able to change the specified user role as the co-host.
 * @return If the specified user can be the co-host, the return value is YES. Otherwise failed.
 */
- (BOOL)canBeCoHost;

/**
 * @brief Query if the user can be assigned as co-owner in meeting. Co-owner can be grant privilege to manage some assets after the meeting.
 * @return YES means that the user can be assigned as co-owner. Otherwise cannot.
 */
- (BOOL)canBeCoOwner;

/**
 * @brief Get the webinar status of the user specified by the current information.
 * @return The object of ZoomSDKWebinarAttendeeStatus.
 */
- (ZoomSDKWebinarAttendeeStatus*_Nullable)GetWebinarAttendeeStatus;

/**
 * @brief Get the user is talking.
 * @return YES means that the user is talking.
 */
- (BOOL)isTalking;

/**
 * @brief Get the customer Key matched with the current user information. The max length of customer Key is 35.
 * @return The user customer Key.
 */
- (NSString *_Nullable)getCustomerKey;

/**
 * @brief Determine if user is interpreter.
 * @return YES means that the user is interpreter.
 */
- (BOOL)isInterpreter;

/**
 * @brief Determine whether the user specified by the current information is a sign language interpreter or not.
 * @return YES means that the user is sign interpreter.
 */
- (BOOL)isSignLanguageInterpreter;

/**
 * @brief Get interpreter active language.
 * @return Value of language id.
 */
- (NSString *_Nullable)getInterpreterActiveLanguage;

/**
 * @brief Get the raising hand status.
 * @return YES means that the user is raising hand.
 */
- (BOOL)isRaisingHand;

/**
 * @brief Get the local recording status.
 * @return The status of the local recording status.
 */
- (ZoomSDKRecordingStatus)getLocalRecordingStatus;

/**
 * @brief Get the user persistent ID matched with the current user information. This ID persists for the duration of the main meeting. Once the main meeting ends, the ID will be discarded.
 * @return If the function succeeds, the return value is the user persistent ID. Otherwise nil.
 */
- (NSString *_Nullable)getPersistentId;

/**
 * @brief Determine whether the user has started a raw live stream.
 * @return YES indicates that the specified user has started a raw live stream, otherwise false.
 */
- (BOOL)isRawLiveStreaming;

/**
 * @brief Determine whether the user has raw live stream privilege.
 * @return YES indicates that the specified user has raw live stream privilege, otherwise false.
 */
- (BOOL)hasRawLiveStreamPrivilege;

/**
 * @brief Get the emoji feedback type of the user.
 * @return The emoji feedback type.
 */
- (ZoomSDKEmojiFeedbackType)getEmojiFeedbackType;

/**
 * @brief Query if the participant has a camera.
 * @return YES means the user has a camera.
 */
- (BOOL)hasCamera;

/**
 * @brief Query if the participant is in waiting room.
 * @return YES means the user in waiting room.
 */
- (BOOL)isInWaitingRoom;

/**
 * @brief Determine whether the user specified by the current information is in the webinar backstage or not.
 * @return YES indicates that the specified user is in the webinar backstage.
 */
- (BOOL)isInWebinarBackstage;

/**
 * @brief Query if the participant is closedCaption sender.
 * @return YES means the user is closedCaption sender.
 */
- (BOOL)isClosedCaptionSender;

/**
 * @brief Returns whether the user is production studio user.
 * @return YES means the user is production studio user.
 */
- (BOOL)isProductionStudioUser;

/**
 * @brief Returns the ID of the parent user of this production user.
 * @return The  userid  of the parent user of this production user.
 */
- (unsigned int)getProductionStudioParent;

/**
 * @brief Determine whether the user specified by the current information is bot user or not.
 * @return YES indicates that the specified user is bot user.
 */
- (BOOL)isBotUser;

/**
 * @brief Get the bot app name.
 * @return If the function succeeds, the return value is the bot app name. Otherwise the function fails, the return value is nil.
 */
- (NSString * _Nullable)getBotAppName;

/**
 * @brief Query if the participant enabled virtual name tag.
 * @return YES means enabled. Otherwise it isn't enabled.
 */
- (BOOL)isVirtualNameTagEnabled;

/**
 * @brief Query the virtual name tag roster infomation.
 * @return If the function succeeds, it returns the list of user's virtual name tag roster info.
 */
- (nullable NSArray<ZoomSDKVirtualNameTag*> *)getVirtualNameTagArray;

/**
 * @brief Determine whether the user specified by the current information in companion mode or not.
 * @return YES indicates that the specified user in companion mode.
 */
- (BOOL)isCompanionModeUser;

/**
 * @brief Query the granted assets info when assign a co-owner.
 * @return If the function succeeds, it returns the list of user's grant assets info.
 * @note If not granted any assets privilege, the default configuration of the web will be queried. If has granted assets privilege, the result after granting will be queried.
 */
- (NSArray <ZoomSDKGrantCoOwnerAssetsInfo *>* _Nullable)getGrantCoOwnerAssetsInfo;
@end



/**
 * @class ZoomSDKJoinMeetingHelper
 * @brief Helper interface for handling the process of joining a meeting, such as inputting password or display name.
 */
@interface ZoomSDKJoinMeetingHelper :NSObject
{
    JoinMeetingReqInfoType   _reqInfoType;
}
/**
 * @brief Get the type of registration information required to join the meeting.
 * @return The type of the registration information.
 */
- (JoinMeetingReqInfoType)getReqInfoType;
/**
 * @brief Input the password to join meeting.
 * @param password The meeting password of the meeting.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)inputPassword:(NSString*)password;

/**
 * @brief Input the screen name to join meeting.
 * @param screenName The username of the meeting.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)inputMeetingScreenName:(NSString*)screenName;

/**
 * @brief Cancel to join meeting.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)cancel;
@end


/**
 * @protocol ZoomSDKMeetingActionControllerDelegate
 * @brief Protocol for receiving meeting events such as user join, leave, and audio status changes.
 */
@protocol ZoomSDKMeetingActionControllerDelegate <NSObject>

/**
 * @brief Notification of user's audio status changes. 
 * @param userAudioStatusArray An array contains ZoomSDKUserAudioStatus elements of each user's audio status.
 */
- (void)onUserAudioStatusChange:(NSArray*)userAudioStatusArray;

/**
 * @brief Notification of user joins meeting.
 * @param array Array of users who join meeting. 
 *
 */
- (void)onUserJoin:(NSArray*)array;

/**
 * @brief Notification of user leaves meeting.
 * @param array Array of users leave meeting.
 */
- (void)onUserLeft:(NSArray*)array;

/**
 * @brief Upgrade the information of the specified user.
 * @param userID The ID of the specified user.
 * @deprecated This method is no longer used.
 */
- (void)onUserInfoUpdate:(unsigned int)userID DEPRECATED_MSG_ATTRIBUTE("No longer used");

/**
 * @brief Notification of virtual name tag status change.
 * @param bOn YES means that virtual name tag is turned on. Otherwise it isn't.
 * @param userID The ID of user who virtual name tag status changed.
 */
- (void)onVirtualNameTagStatusChanged:(BOOL)bOn userID:(unsigned int)userID;

/**
 * @brief Notification of virtual name tag roster info update.
 * @param userID The ID of user who virtual name tag status changed.
 */
- (void)onVirtualNameTagRosterInfoUpdated:(unsigned int)userID;

/**
 * @brief Notification of host changes.
 * @param userID User ID of new host.
 *
 */
- (void)onHostChange:(unsigned int)userID;

/**
 * @brief Notification of co-host changes.
 * @param userID User ID with coHost privilege changes.
 * @param isCoHost YES indicates that the specified user is co-host.
 */
- (void)onMeetingCoHostChanged:(unsigned int)userID isCoHost:(BOOL)isCoHost;

/**
 * @brief Callback event for when the video spotlight user list changes.Spotlight user means that the view will show only the specified user and won't change the view even other users speak.
 * @param spotlightedUserList Spotlight user list.
 */
- (void)onSpotlightVideoUserChange:(NSArray*_Nullable)spotlightedUserList;

/**
 * @brief Notify that user's video status changes.
 * @param videoStatus The status of the video.
 * @param userID The ID of user who video status changes.
 *
 */
- (void)onVideoStatusChange:(ZoomSDKVideoStatus)videoStatus UserID:(unsigned int)userID;
/**
 * @brief Notification of user's hand status changes.
 * @param raise YES means that the specified user raises hand, otherwise, puts hand down.  
 * @param userID The ID of user whose hand status changes.
 */
- (void)onLowOrRaiseHandStatusChange:(BOOL)raise UserID:(unsigned int)userID;

/**
 * @brief Callback event of user joins meeting. 
 * @param joinMeetingHelper An object for inputing password or canceling to join meeting.
 */
- (void)onJoinMeetingResponse:(ZoomSDKJoinMeetingHelper*_Nullable)joinMeetingHelper;

/**
 * @brief Notify user to confirm or cancel to switch to single share from multi-share.
 * @param confirmHandle An object used to handle the action to switch to single share from multi-share.
 */
- (void)onMultiToSingleShareNeedConfirm:(ZoomSDKMultiToSingleShareConfirmHandler*_Nullable)confirmHandle;
/**
 * @brief Notify that video of active user changes.
 * @param userID The user's user ID.						  
 */
- (void)onActiveVideoUserChanged:(unsigned int)userID;
/**
 * @brief Notify that video of active speaker changes.
 * @param userID The user's user ID.
 */
- (void)onActiveSpeakerVideoUserChanged:(unsigned int)userID;
/**
 * @brief Notify that host ask you to unmute yourself.
 */
- (void)onHostAskUnmute;
/**
 * @brief Notify that host ask you to start video.
 */
- (void)onHostAskStartVideo;
/**
 * @brief Notification of in-meeting active speakers.
 * @param useridArray The array contain userid of the active speakers.
 */
- (void)onUserActiveAudioChange:(NSArray *)useridArray;

/**
 * @brief Notification of user name changed.
 * @param userList The list of user whose user name have changed.
 * @note The old interface - (void)onUserNameChanged:(unsigned int)userid userName:(NSString *)userName; will be marked as deprecated, and all platforms will be using this new callbacks. This is because in a webinar, when the host renamed an attendee, only the attendee could receive the old callback, and the host/cohost/panlist is not able to receive it, which leads to the developer not being able to update the UI.
 */
- (void)onUserNamesChanged:(NSArray<NSNumber*>*)userList;

/**
 * @brief Notification of reclaim host key is invalid.
 */
- (void)onInvalidReclaimHostKey;

/**
 * @brief Notification the video order updated.
 * @param orderList The order list contains the user ID of listed users.
 */
- (void)onHostVideoOrderUpdated:(NSArray*)orderList;

/**
 * @brief Notification the local video order updated.
 * @param localOrderList The lcoal vidoe order list contains the user ID of listed users.
 */
- (void)onLocalVideoOrderUpdated:(NSArray*)localOrderList;

/**
 * @brief Notification the status of following host's video order changed.
 * @param follow Yes means the option of following host's video order is on, otherwise not.
 */
- (void)onFollowHostVideoOrderChanged:(BOOL)follow;

/**
 * @brief When the host calls the lower all hands interface, the host/cohost/panelist will receive this callback.
 */
- (void)onAllHandsLowered;

/**
 * @brief Notify that user's video quality changes.
 * @param quality The quality of the Video.
 * @param userID The ID of user whose video quality changed.
 */
- (void)onUserVideoQualityChanged:(ZoomSDKVideoQuality)quality userID:(unsigned int)userID;

/**
 * @brief Notify that the chat message deleted.
 * @param msgID The ID of deleted chat message.
 * @param deleteBy Indicates the message was deleted by whom.
 */
- (void)onChatMsgDeleteNotification:(NSString*)msgID messageDeleteType:(ZoomSDKChatMessageDeleteType)deleteBy;

/**
 * @brief Callback to notif that the privilege of participants/webinar chat has changed.
 * @param chatStatus The info of the privilege.
 */
- (void)onChatStatusChangedNotification:(ZoomSDKChatStatus *)chatStatus;

/**
 * @brief Notify that the share meeting chat status.
 * @param isStart YES means share meeting chat is started, otherwise not.
 */
- (void)onShareMeetingChatStatusChanged:(BOOL)isStart;

/**
 * @brief Callback event when a meeting is suspended.
 */
- (void)onSuspendParticipantsActivities;

/**
 * @brief Callback event that lets participants start a video.
 * @param allow True allow. If false, disallow.
 */
- (void)onAllowParticipantsStartVideoNotification:(BOOL)allow;

/**
 * @brief Callback event that lets participants rename themself.
 * @param allow True allow. If false, participants may not rename themselves.
 */
- (void)onAllowParticipantsRenameNotification:(BOOL)allow;

/**
 * @brief Callback event that lets participants unmute themself.
 * @param allow True allow. If false, participants may not unmute themselves.
 */
- (void)onAllowParticipantsUnmuteSelfNotification:(BOOL)allow;

/**
 * @brief Callback event that lets participants share a new whiteboard.
 * @param allow True allow. If false, participants may not share new whiteboard.
 */
- (void)onAllowParticipantsShareWhiteBoardNotification:(BOOL)allow;

/**
 * @brief Callback event that allows a meeting lock status change.
 * @param isLock If true, the status is locked. If false, the status is unlocked.
 */
- (void)onMeetingLockStatus:(BOOL)isLock;

/**
 * @brief Callback event that requests local recording privilege changes.
 * @param status Value of request for local recording privilege status.
 */
- (void)onRequestLocalRecordingPrivilegeChanged:(ZoomSDKLocalRecordingRequestPrivilegeStatus)status;

/**
 * @brief Callback event that lets participants request that the host starts cloud recording.
 * @param allow YES  allow. If NO, disallow.
 */
- (void)onAllowParticipantsRequestCloudRecording:(BOOL)allow;
/**
 * @brief Callback event that user's avatar path updated when in the meeting.
 * @param userID The ID of user whose avatar path updated.
 */
- (void)onInMeetingUserAvatarPathUpdated:(unsigned int)userID;

/**
 * @brief Sink the event that AI Companion active status changed.
 * @param active YES means AI Companion is active.
 */
- (void)onAICompanionActiveChangeNotice:(BOOL)active;

/**
 * @brief Sink the event that participant profile status change.
 * @param hidden YES means hide participant profile picture, NO means show participant profile picture.
 */
- (void)onParticipantProfilePictureStatusChange:(BOOL)hidden;

/**
 * @brief Callback event of alpha channel mode changes.
 * @param isAlphaModeOn YES means it's in alpha channel mode. Otherwise, it's not.
 */
- (void)onVideoAlphaChannelStatusChanged:(BOOL)isAlphaModeOn;

/**
 * @brief Sink the event that focus mode changed by Host/Co-Host.
 * @param on YES means the focus mode change to on, otherwise NO.
 */
- (void)onFocusModeStateChanged:(BOOL)on;

/**
 * @brief The focus mode share type was changed by the host or co-host.
 * @param shareType Share type change.
 */
- (void)onFocusModeShareTypeChanged:(ZoomSDKFocusModeShareType)shareType;

/**
 * @brief Callback event of meeting QA feature status changes.
 * @param isMeetingQAFeatureOn YES means meeting QA feature is on, otherwise not.
 */
- (void)onMeetingQAStatusChanged:(BOOL)isMeetingQAFeatureOn;

/**
 * @brief Callback event that requests to join third party telephony audio.
 * @param audioInfo Instruction on how to join the meeting with third party audio.
 */
 - (void)notifyToJoin3rdPartyTelephonyAudio:(NSString*)audioInfo;

/**
 * @brief Callback for when the current user receives a camera control request. This callback will be triggered when another user requests control of the current user’s camera.
 * @param userId The user ID that sent the request.
 * @param requestType The request type.
 * @param actionApprove Execute this block to approve.
 * @param actionDecline Execute this block to decline.
 */
- (void)onCameraControlRequestReceived:(unsigned int)userId requestType:(ZoomSDKCameraControlRequestType)requestType actionApprove:(nullable ZoomSDKError(^)(void))actionApprove actionDecline:(nullable ZoomSDKError(^)(void))actionDecline;

/**
 * @brief Callback for when the current user is granted camera control access. Once the current user sends the camera control request, this callback will be triggered when they receive a request result.
 * @param userId The user ID that response the request.
 * @param resultType The result type.
 */
- (void)onCameraControlRequestResult:(unsigned int)userId resultType:(ZoomSDKCameraControlRequestResult)resultType;

/**
 * @brief Callback event for the mute on entry status change.
 * @param enable Specify whether mute on entry is enabled or not.
 */
- (void)onMuteOnEntryStatusChange:(BOOL)enable;

/**
 * @brief Callback event for the meeting topic changed.
 * @param topic The new meeting topic.
 */
- (void)onMeetingTopicChanged:(NSString *)topic;

/**
 * @brief Callback event that the bot relationship changed in the meeting.
 * @param authorizeUserID Specify the authorizer user ID.
 */
- (void)onBotAuthorizerRelationChanged:(unsigned int)authorizeUserID;

/**
 * @brief Callback event that the companion relationship created in the meeting.
 * @param parentUserID Specify the parent user ID.
 * @param childUserID Specify the child user ID.
 */
- (void)onCreateCompanionRelation:(unsigned int)parentUserID childUserID:(unsigned int)childUserID;

/**
 * @brief Callback event that the companion relationship removed in the meeting.
 * @param childUserID Specify the child user ID.
 */
- (void)onRemoveCompanionRelation:(unsigned int)childUserID;

/**
 * @brief Callback event when the user's grant co-owner permission changed.
 * @param canGrantOther YES means can grant others, otherwise not.
 */
- (void)onGrantCoOwnerPrivilegeChanged:(BOOL)canGrantOther;
@end


/**
 * @class ZoomSDKMeetingActionController
 * @brief Interface for managing participant-related actions in a meeting, such as retrieving participant list, muting audio, or controlling roles.
 */
@interface ZoomSDKMeetingActionController : NSObject
{
    id<ZoomSDKMeetingActionControllerDelegate> _delegate;
}
/**
 * @brief Set or get the delegate to receive meeting action events.
 */
@property(nonatomic, assign, nullable) id<ZoomSDKMeetingActionControllerDelegate> delegate;

/**
 * @brief Get the list of participants.
 * @return An array of participant ID.
 */
- (NSArray*_Nullable)getParticipantsList;
/**
 * @brief Commands in the meeting.
 * @param cmd The commands in the meeting.
 * @param userID The ID of user. Zero(0) means that the current user can control the commands. If it is other participant, it will return the corresponding user ID.
 * @param screen Specify the screen on which you want to do action.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)actionMeetingWithCmd:(ActionMeetingCmd)cmd userID:(unsigned int)userID onScreen:(ScreenType)screen;

/**
 * @brief Get the information of the specified user. 
 * @param userID The ID of the specified user. 
 * @return If the function succeeds, it will return the object of ZoomSDKUserInfo, otherwise nil.
 */
- (ZoomSDKUserInfo*_Nullable)getUserByUserID:(unsigned int)userID;

/**
 * @brief Get the information of myself.
 * @return If the function succeeds, it will return the object of ZoomSDKUserInfo, otherwise nil.
 */
- (ZoomSDKUserInfo*_Nullable)getMyself;

/**
 * @brief Change user's screen name in the meeting.
 * @param userID The ID of user whose screen name will be changed. Normal participants can change only their personal screen name while the host/co-host can change all participants' names. 
 * @param name The new screen name. 
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)changeUserName:(unsigned int)userID newName:(NSString*)name;

/**
 * @brief Assign a participant to be a new host, or original host who loose the privilege reclaims to be the host.
 * @param userID User ID of new host. Zero(0) means that the original host takes back the privilege.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)makeHost:(unsigned int)userID;

/**
 * @brief Make the specified user to raise hand or lower hand.
 * @param raise YES means to raise hand, NO means lower hand for this specified user.
 * @param userid User ID of the user to be modified.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)raiseHand:(BOOL)raise UserID:(unsigned int)userid;

/**
 * @brief Remove the specified user from meeting.
 * @param userid User ID of the user to be modified.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)expelUser:(unsigned int)userid;

/**
 * @brief Allow or disallow the specified user to local recording.
 * @param allow YES means allow, NO means disallow.
 * @param userid User ID of the user to be modified.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)allowLocalRecord:(BOOL)allow UserID:(unsigned int)userid;

/**
 * @brief Query whether the current user is the original host.
 * @return YES means the current user is the original host. Otherwise not.
 */
- (BOOL)isSelfOriginalHost;

/**
 * @brief Query if user can claim host(be host) or not. 
 * @return YES means able, otherwise not.
 */
- (BOOL)canReclaimHost;

/**
 * @brief Reclaim the role of the host.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)reclaimHost;
/**
 * @brief Normal participant claims host by host-key.
 * @param hostKey Host key.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)claimHostByKey:(NSString*)hostKey;
/**
 * @brief Assign a user as co-host in meeting. 
 * @param userid The ID of user to be a co-host.
 * @return If the function succeeds, it will return @c ZoomSDKError_Success, otherwise not.
 * @note The co-host cannot be assigned as co-host by himself. And the user should have the power to assign the role.
 */
- (ZoomSDKError)assignCoHost:(unsigned int)userid;

/**
 * @brief Revoke co-host role of another user in meeting.
 * @param userid The ID of co-host who will loose the co-host privilege.
 * @return If the function succeeds, it will return @c ZoomSDKError_Success, otherwise not.
 * @note Only meeting host can run the function.
 */
- (ZoomSDKError)revokeCoHost:(unsigned int)userid;

/**
 * @brief Set sharing types for the host or co-host in meeting.
 * @param shareType The custom sharing type.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)setShareSettingType:(ZoomSDKShareSettingType)shareType;

/**
 * @brief Get the sharing types for the host or co-host in meeting.
 * @param type The custom sharing type.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)getShareSettingType:(ZoomSDKShareSettingType*)type;

/**
 * @brief Determine if user's original sound is enabled.
 * @return YES means enabled, otherwise not.
 */
- (BOOL)isUseOriginalSoundOn;

/**
 * @brief Set to output original sound of mic in meeting.
 * @param enable YES means using original sound, No disabling.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)enableUseOriginalSound:(BOOL)enable;

/**
 * @brief Determine if the meeting supports user's original sound.
 * @return YES means supported, otherwise not.
 */
- (BOOL)isSupportUseOriginalSound;

/**
 * @brief Swap to show sharing screen or video.
 * @param share YES means swap to sharing screen, NO means swap to video.
 * @note Only available for Zoom native ui mode.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)swapToShowShareViewOrVideo:(BOOL)share;

/**
 * @brief Determine if the user can swap between show sharing screen or video now.
 * @return YES means can, otherwise not.
 */
- (BOOL)canSwapBetweenShareViewOrVideo;

/**
 * @brief Determine if the meeting is displaying the sharing screen now.
 * @param isShowingShareView YES means is showing sharing screen, NO means is showing video.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)isDisplayingShareViewOrVideo:(BOOL*)isShowingShareView;

/**
 * @brief Set the meeting topic on meeting info.
 * @param topic The meeting topic.
 * @return If the function succeeds, it will return @c ZoomSDKError_Success, otherwise not.
 * @deprecated This method is no longer used.
 */
- (ZoomSDKError)setMeetingTopicOnMeetingInfo:(NSString *)topic DEPRECATED_MSG_ATTRIBUTE("No longer used");

/**
 * @brief Determine if the current user can change the meeting topic.
 * @return YES means can, otherwise not.
 */
- (BOOL)canSetMeetingTopic;

/**
 * @brief Change the meeting topic.
 * @param topic The new meeting topic.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)setMeetingTopic:(NSString *)topic;

/**
 * @brief Check if the host or cohost can enable mute on entry.
 * @return YES indicates that the host or cohost can enable mute on entry. Otherwise not..
 */
- (BOOL)canEnableMuteOnEntry;

/**
 * @brief Determine if mute on entry is enabled.
 * @return YES indicates that mute on entry is enabled
 */
- (BOOL)isMuteOnEntryEnabled;

/**
 * @brief Determine if the share screen is allowed.
 * @return YES means is disable share screen,otherwise not.
 */
- (BOOL)isParticipantsShareAllowed;

/**
 * @brief Allow participants to share screen.
 * @param allow YES means allow participants use share screen,otherwise not.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)allowParticipantsToShare:(BOOL)allow;

/**
 * @brief Determine if the chat is allowed.
 * @return YES means is disable chat,otherwise not.
 */
- (BOOL)isParticipantsChatAllowed;

/**
 * @brief Allow participants to chat.
 * @param allow YES means allow participants to chat,otherwise not.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)allowParticipantsToChat:(BOOL)allow;

/**
 * @brief Determine if the participant rename is disabled.
 * @return YES means is disable participant rename,otherwise not.
 */
- (BOOL)isParticipantsRenameAllowed;

/**
 * @brief Allow participants to rename.
 * @param allow YES means allow participants to rename,otherwise not.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)allowParticipantsToRename:(BOOL)allow;

/**
 * @brief Determine if user can spotlight someone.
 * @param userID The user id of the user you want to spotlight.
 * @param result A point to enum ZoomSDKSpotlightResult, if the function call successfully, the value of 'result' means whether the user can be spotlighted.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)canSpotlight:(unsigned int)userID result:(ZoomSDKSpotlightResult*)result;

/**
 * @brief Determine if user can unspotlight someone.
 * @param userID The user id of the user you want to unspotlight.
 * @param result A point to enum ZoomSDKSpotlightResult, if the function call successfully, the value of 'result' means whether the user can be unspotlighted.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)canUnSpotlight:(unsigned int)userID result:(ZoomSDKSpotlightResult*)result;

/**
 * @brief Spotlight someone's video.
 * @param userID The user id of the user you want to spotlight.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)spotlightVideo:(unsigned int)userID;

/**
 * @brief Unspotlight someone's video.
 * @param userID The user id of the user you want to unspotlight.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)unSpotlightVideo:(unsigned int)userID;

/**
 * @brief Unspotlight all videos.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)unSpotlightAllVideos;

/**
 * @brief Get all users that has been spotlighted.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (NSArray*_Nullable)getSpotlightedUserList;

/**
 * @brief Determine if user can pin someone to first view.
 * @param userID The user id of the user you want to pin.
 * @param result A point to enum ZoomSDKPinResult, if the function call successfully, the value of 'result' means whether the user can be pined.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)canPinToFirstView:(unsigned int)userID result:(ZoomSDKPinResult*)result;

/**
 * @brief Pin user's video to first view.
 * @param userID The user id of the user you want to pin.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)pinVideoToFirstView:(unsigned int)userID;

/**
 * @brief Unpin user's video to first view.
 * @param userID The user id of the user you want to unpin.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)unPinVideoFromFirstView:(unsigned int)userID;

/**
 * @brief Unpin all videos to first view.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)unPinAllVideosFromFirstView;

/**
 * @brief Get all users that has been pined in first view.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (NSArray*_Nullable)getPinnedUserListFromFirstView;

/**
 * @brief Determine if user can pin someone to second view.
 * @param userID The user id of the user you want to pin.
 * @param result A point to enum ZoomSDKPinResult, if the function call successfully, the value of 'result' means whether the user can be pined.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)canPinToSecondView:(unsigned int)userID result:(ZoomSDKPinResult*)result;

/**
 * @brief Pin user's video to second view.
 * @param userID The user id of the user you want to pin.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)pinVideoToSecondView:(unsigned int)userID;

/**
 * @brief Unpin user's video to second view.
 * @param userID The user id of the user you want to unpin.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)unPinVideoFromSecondView:(unsigned int)userID;

/**
 * @brief Get all users that has been pined in second view.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (NSArray*_Nullable)getPinnedUserListFromSecondView;

/**
 * @brief Determine if participants can unmute themselves.
 * @return YES means can unmute themselves, otherwise not.
 */
- (BOOL)isParticipantsUnmuteSelfAllowed;

/**
 * @brief Determine whether the legal notice for chat is available.
 * @return YES means the legal notice for chat is available, otherwise not.
 */
- (BOOL)isMeetingChatLegalNoticeAvailable;

/**
 * @brief Get the chat legal notices prompt.
 * @return If the function succeeds, it will return the chat legal notices prompt. Otherwise nil.
 */
- (NSString *)getChatLegalNoticesPrompt;

/**
 * @brief Get the chat legal notices explained.
 * @return If the function succeeds, it will return the chat legal notices explained. Otherwise nil.
 */
- (NSString *)getChatLegalNoticesExplained;

/**
 * @brief Enable of disable following host's video order.
 * @param enable YES means enable, otherwise not.
 */
- (ZoomSDKError)enableFollowHostVideoOrder:(BOOL)enable;

/**
 * @brief Set the video order.
 * @param orderList The array contains the listed user id.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)setVideoOrder:(NSArray<NSNumber*>*)orderList;

/**
 * @brief Determine whether the option of following host's video order is on of off.
 * @return Yes means the option of following host's video order is on, otherwise not.
 */
- (BOOL)isFollowHostVideoOrderOn;

/**
 * @brief Determine whether this meeting support following host's video orde.
 * @return Yes means supporting, otherwise not.
 */
- (BOOL)isSupportFollowHostVideoOrder;

/**
 * @brief Get the video orde list.
 * @return If the function succeeds, it will return the video orde list., otherwise nil.
 */
- (NSArray<NSNumber*>*_Nullable)getVideoOrderList;

/**
 * @brief Lower all hands raised.
 * @param forWebinarAttendees YES means to lower all hands for webinar attendee.
 * @return If the function succeeds, it will return @c ZoomSDKError_Success, otherwise not.
 * @note When forWebinarAttendees is true, the SDK sends the lower all hands command only to
 webinar attendees. When forWebinarAttendees is false, the SDK sends the lower all hands command to anyone who is not a webinar attendee, such as the webinar host/cohost/panelist or everyone in a regular meeting.
 */
- (ZoomSDKError)lowerAllHands:(BOOL)forWebinarAttendees;

/**
 * @brief Determine whether the message can be deleted.
 * @param msgID  the message id.
 * @return Yes means can be deleted, otherwise not.
 */
- (BOOL)isChatMessageCanBeDeleted:(NSString*)msgID;

/**
 * @brief Delete chat message by message id.
 * @param msgID  the message id.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)deleteChatMessage:(NSString*)msgID;

/**
 * @brief Get all chat message id.
 * @return If the function succeeds, it will return an array with all message id, otherwise not.
 */
- (NSArray<NSString*>*_Nullable)getAllChatMessageID;

/**
 * @brief Set participants chat privilege.
 * @param privilege The chat privilege type to assign to participants.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)setParticipantsChatPrivilege:(ZoomSDKChatPrivilegeType)privilege;

/**
 * @brief Get the chat status of the current user.
 * @return If the function succeeds, it will return the info of the chat status, otherwise nil.
 */
- (ZoomSDKChatStatus *_Nullable)getChatStatus;

/**
 * @brief Determine whether the legal notice for sharing in meeting chat is available.
 * @return YES indicates the legal notice for chat is available. Otherwise NO.
 */
- (BOOL)isShareMeetingChatLegalNoticeAvailable;

/**
 * @brief Get the sharing in meeting chat started legal notices content.
 * @return If the function succeeds, it will return the the sharing in meeting chat started legal notices content. Otherwise nil.
 */
- (NSString*)getShareMeetingChatStartedLegalNoticeContent;

/**
 * @brief Get the sharing in meeting chat stopped legal notices content.
 * @return If the function succeeds, it will return the the sharing in meeting chat stopped legal notices content. Otherwise nil.
 */
- (NSString*)getShareMeetingChatStoppedLegalNoticeContent;

/**
 * @brief Stop the incoming audio.
 * @param stop YES means stop, otherwise not.
 * @return If the function succeeds, it will return the the sharing in meeting chat stopped legal notices content. Otherwise nil.
 */
- (ZoomSDKError)stopIncomingAudio:(BOOL)stop;

/**
 * @brief Determine if the incoming audio is stopped.
 * @return Yes indicates that incoming audio is stopped, otherwise not.
 */
- (BOOL)isIncomingAudioStopped;

/**
 * @brief Stop in coming video.
 * @param stop YES means stop in coming video. NO is start coming video.
 * @return If the function succeeds, it will return the the sharing in meeting chat stopped legal notices content. Otherwise nil.
 */
- (ZoomSDKError)stopIncomingVideo:(BOOL)stop;

/**
 * @brief Determine whether the coming video is stopedd.
 * @return YES indicates the coming video is stopedd. Otherwise NO.
 */
- (BOOL)isIncomingVideoStopped;

/**
 * @brief Determine if show the last used avatar in the meeting.
 * @param show YES indicates to show the last used avatar.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)showAvatar:(BOOL)show;

/**
 * @brief Determine if the meeting is showing the avatar.
 * @return YES indicates the meeting is showing the avatar.
 */
- (BOOL)isShowAvatar;

/**
 * @brief Allowing the regular attendees to start video, it can only be used in regular meeetings(no bo).
 * @param bAllow YES indicates Allowing the regular attendees to start video.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)allowParticipantsToStartVideo:(BOOL)bAllow;

/**
 * @brief Check whether the current meeting allows participants to start video, it can only be used in regular meeetings(no bo).
 * @return YES means allows participants to start video. Otherwise NO.
 */
- (BOOL)isParticipantsStartVideoAllowed;

/**
 * @brief Allowing the regular attendees to share whiteboard, it can only be used in regular meeetings(no bo).
 * @param bAllow YES indicates Allowing the regular attendees to share whiteboard.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)allowParticipantsToShareWhiteBoard:(BOOL)bAllow;

/**
 * @brief Check whether the current meeting allows participants to share whiteboard, it can only be used in regular meeetings(no bo).
 * @return YES means allows participants to share whiteboard.
 */
- (BOOL)isParticipantsShareWhiteBoardAllowed;

/**
 * @brief Check whether the current meeting allows participants to send local recording privilege requests. It can only be used in regular meetings, not in webinar or breakout room.
 * @return If allows participants to share whiteboard, the return value is YES.
 */
- (BOOL)isParticipantRequestLocalRecordingAllowed;

/**
 * @brief Allow participant to request local recording. It can only be used in regular meetings, not in webinar or breakout room.
 * @param bAllow YES indicates Allowing the regular attendees to send local recording privilege request.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)allowParticipantsToRequestLocalRecording:(BOOL)bAllow;

/**
 * @brief Check whether the current meeting auto-grants participants’ local recording privilege requests. It can only be used in regular meetings (not webinar or breakout room).
 * @return If auto grant participants local recording privilege request, the return value is YES.
 */
- (BOOL)isAutoAllowLocalRecordingRequest;

/**
 * @brief Allow participants to request local recording. It can only be used in regular meetings (not webinar or breakout room).
 * @param bAllow YES indicates Auto grant or deny the regular attendee's local recording privilege request.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)autoAllowLocalRecordingRequest:(BOOL)bAllow;

/**
 * @brief Determine whether suspend all participants activities.
 * @return YES means can suspend participants activities. Otherwise not.
 */
- (BOOL)canSuspendParticipantsActivities;

/**
 * @brief Suspend all participants activities.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)suspendParticipantsActivities;

/**
 * @brief Query if the current user can hide participant profile pictures.
 * @return If the function succeeds, it will return @c ZoomSDKError_Success, otherwise not.
 * @note: This feature is influenced by focus mode change.
 */
- (ZoomSDKError)canHideParticipantProfilePictures;

/**
 * @brief Query if the current meeting hides participant pictures.
 * @return YES means hide participant pictures, NO means show participant pictures.
 */
- (BOOL)isParticipantProfilePicturesHidden;

/**
 * @brief Hide/Show participant profile pictures.
 * @param hide YES means hide participant profile pictures, NO means show participant pictures.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)hideParticipantProfilePictures:(BOOL)hide;

/**
 * @brief Determine if alpha channel mode can be enabled.
 * @return YES means it can be enabled. Otherwise NO.
 */
- (BOOL)canEnableAlphaChannelMode;

/**
 * @brief Enable or disable alpha channel mode.
 * @param enabled YES indicates to enable alpha channel mode. Otherwise, disable it.
 * @return If the function succeeds, the return value is ZoomSDKError_Success.
 */
- (ZoomSDKError)enableAlphaChannelMode:(BOOL)enabled;

/**
 * @brief Determine if alpha channel mode is enabled.
 * @return YES indicates is in alpha channel mode. Otherwise NO.
 */
- (BOOL)isAlphaChannelModeEnabled;

/**
 * @brief Get the focus mode enabled or not by web portal.
 * @return YES means focus mode enabled. Otherwise NO.
 */
- (BOOL)isFocusModeEnabled;

/**
 * @brief Turn focus mode on or off. Focus mode on means Participants will only be able to see hosts' videos and shared content, and videos of spotlighted participants.
 * @param on Yes means to turn on. No means to turn off.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise not.
 */
- (ZoomSDKError)turnFocusModeOn:(BOOL)on;

/**
 * @brief Get the focus mode on or off.
 * @return YES means focus mode on, otherwise NO.
 */
- (BOOL)isFocusModeOn;

/**
 * @brief Get share focus mode  type indicating who can see the shared content which is controlled by host or co-host.
 * @return If the function succeeds, it will return the current share focus mode type.
 */
- (ZoomSDKFocusModeShareType)getFocusModeShareType;

/**
 * @brief Set the focus mode type indicating who can see the shared content which is controlled by host or co-host.
 * @param shareType The type of focus mode share type.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)setFocusModeShareType:(ZoomSDKFocusModeShareType)shareType;

/**
 * @brief Set to enable/disable meeting QA.
 * @param enable YES means enabled, NO disabled.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)enableMeetingQAFeature:(BOOL)enable;

/**
 * @brief Query if meeting QA is enabled in current meeting.
 * @return YES means enabled, otherwise not.
 */
- (BOOL)isMeetingQAFeatureOn;
/**
 * @brief Determine if the current user can enable participant request clould recording.
 * @return YES means the current user can enable participant request clould recording.
 */
- (BOOL)canEnableParticipantRequestCloudRecording;

/**
 * @brief Check whether the current meeting allows participants to send cloud recording privilege request, it can be used in regular meeetings and webinar (no bo).
 * @return YES means allows participants to send request.
 */
- (BOOL)isParticipantRequestCloudRecordingAllowed;

/**
 * @brief Toggle whether attendees can send requests for the host to start a cloud recording. This  can only be used in regular meeetings, not breakout rooms.
 * @param allow Yes indicates that  participants are allowed to send cloud recording privilege requests.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise not.
 */
- (ZoomSDKError)allowParticipantsToRequestCloudRecording:(BOOL)allow;

/**
 * @brief Determine if the meeting has third party telephony audio enabled.
 * @return YES means enabled, otherwise it is not enabled
 */
- (BOOL)is3rdPartyTelephonyAudioOn;

/**
 * @brief Get the information about the bot's authorized user.
 * @param userId Specify the user ID for which to get the information.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise not.
 */
- (ZoomSDKUserInfo * _Nullable)getBotAuthorizedUserInfoByUserID:(unsigned int)userId;

/**
 * @brief Get the authorizer's bot list.
 * @param userId Specify the user ID for which to get the information.
 * @return If the function succeeds, the return value is the authorizer's bot list in the meeting.
 */
- (NSArray<NSNumber*>* _Nullable)getAuthorizedBotListByUserID:(unsigned int)userId;

/**
 * @brief Determine if there is support for the virtual name tag feature.
 * @return YES means ssupports the virtual name tag feature. NO means it isn't supported.
 */
- (BOOL)isSupportVirtualNameTag;

/**
 * @brief Enable the virtual name tag feature for the account.
 * @param bEnabled YES means the feature is enabled. Otherwise is isn't.
 * @return If the function succeeds, it returns ZoomSDKError_Success.
 */
- (ZoomSDKError)enableVirtualNameTag:(BOOL)bEnabled;

/**
 * @brief Update the virtual name tag roster infomation for the account.
 * @param userRoster The virtual name tag roster info list for the specified user.
 * @return If the function succeeds, it returns @c ZoomSDKError_Success.
 * @note The maximum size of userRoster should less 20. User should specify the tagName and tagID of each ZoomSDKVirtualNameTag object. The range of tagID is 0-1024.
 */
- (ZoomSDKError)updateVirtualNameTagRosterInfo:(NSArray<ZoomSDKVirtualNameTag*>*)userRoster;

/**
 * @brief Determine if play meeting audio is enabled or not.
 * @return YES means enabled,  NO means it is not enabled.
 */
- (BOOL)isPlayMeetingAudioEnabled;

/**
 * @brief Enable or disable SDK to play meeting audio.
 * @param bEnabled YES means that SDK will play meeting audio, NO means that SDK will not play meeting audio.
 * @return If the function succeeds, it returns ZoomSDKError_Success.
 */
- (ZoomSDKError)enablePlayMeetingAudio:(BOOL)bEnabled;

/**
 * @brief Determine if contrast enhancement effect for speaker video is enabled.
 * @return YES indicates contrast enhancement effect is enabled. Otherwise NO.
 */
- (BOOL)isSpeakerContrastEnhanceEnabled;

/**
 * @brief Enable or disable contrast enhancement effect for speaker video.
 * @param bEnabled YES indicates to enable contrast enhancement effect. Otherwise, disable it.
 * @return If the function succeeds, it returns ZoomSDKError_Success.
 */
- (ZoomSDKError)enableSpeakerContrastEnhance:(BOOL)bEnabled;

/**
 * @brief Get the information about the user's parent user.
 * @param userid Specify the user ID for which to get the information.
 * @return If the function succeeds, the return value is a pointer to the ZoomSDKUserInfo.
 */
- (ZoomSDKUserInfo* _Nullable)getCompanionParentUser:(unsigned int)userid;

/**
 * @brief Get the user's child list.
 * @param userid Specify the user ID for which to get the information.
 * @return If the function succeeds, the return value is the sub-user list of user companion mode.
 */
- (NSArray<NSNumber*>* _Nullable)getCompanionChildList:(unsigned int)userid;

/**
 * @brief Assigns a user as co-host and grants privileges to manage assets after the meeting.
 * @param userid The user ID of the user to be assigned as co-host.
 * @param infoList An array of  \link ZoomSDKGrantCoOwnerAssetsInfo \endlink objects representing the assets and privileges to grant.
 * @return If the function succeeds, it returns @c ZoomSDKError_Success. Otherwise the function fails. For more information, see \link ZoomSDKError \endlink.
 *
 * @note Before calling this method, you must obtain the assets to be granted via  \link getGrantCoOwnerAssetsInfo \endlink.
 */
- (ZoomSDKError)assignCoHostWithAssetsPrivilege:(unsigned int)userid infoList:(NSArray<ZoomSDKGrantCoOwnerAssetsInfo *>*)infoList;

/**
 * @brief Assigns a user as host and grants privileges to manage assets after the meeting.
 * @param userid The user ID of the user to be assigned as host.
 * @param infoList An array of  \link ZoomSDKGrantCoOwnerAssetsInfo \endlink objects representing the assets and privileges to grant.
 * @return If the function succeeds, it returns @c ZoomSDKError_Success. Otherwise, the function fails.
 *
 * @note Before calling this method, you must obtain tThe assets to be granted via  \link getGrantCoOwnerAssetsInfo \endlink.
 */
- (ZoomSDKError)makeHostWithAssetsPrivilege:(unsigned int)userid infoList:(NSArray<ZoomSDKGrantCoOwnerAssetsInfo *>*)infoList;
@end
NS_ASSUME_NONNULL_END
