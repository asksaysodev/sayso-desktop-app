/**
 * @file ZoomSDKNewBreakoutRoomController.h
 * @brief Zoom SDK Breakout Room (BO) management interfaces.
 */

#import <Foundation/Foundation.h>
#import <ZoomSDK/ZoomSDKErrors.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * @class ZoomSDKBOMeetingInfo
 * @brief Breakout Room meeting information interface.
 */
@interface ZoomSDKBOMeetingInfo : NSObject

/**
 * @brief Get breakout meeting id.
 * @return The breakout meeting id.
 */
- (NSString *_Nullable)getBOID;

/**
 * @brief Get breakout meeting nmae.
 * @return The breakout meeting name.
 */
- (NSString *_Nullable)getBOName;

/**
 * @brief Get breakout meeting user list.
 * @return If the function succeeds, will get the breakout meeting user list.
 */
- (NSArray *_Nullable)getBOUserlist;

/**
 * @brief Get breakout meeting user status.
 * @param userID The user's user ID.
 * @return If the function succeeds, will return user status.
 */
- (ZoomSDKBOUserStatus)getBOUserStatusWithUserID:(NSString *)userID;
@end


/**
 * @class ZoomSDKBOOption
 * @brief Defines breakout room configuration options.
 */
@interface ZoomSDKBOOption : NSObject

/**
 * @brief Set the countdown after closing breakout room.
 */
@property(nonatomic,assign)ZoomSDKBOStopCountDown  countDown;

/**
 * @brief Enable or disable that participant can choose breakout room.
 */
@property(nonatomic,assign)BOOL  isParticipantCanChooseBO;

/**
 * @brief Enable or disable that participant can return to main session at any time.
 */
@property(nonatomic,assign)BOOL  isParticipantCanReturnToMainSessionAtAnyTime;

/**
 * @brief Enable or disable that auto move all assigned participants to breakout room.
 */
@property(nonatomic,assign)BOOL  isAutoMoveAllAssignedParticipantsEnabled;

/**
 * @brief YES means it's timer BO, otherwise not.
 */
@property(nonatomic,assign)BOOL  isBOTimerEnabled;

/**
 * @brief YES means time is up, will stop BO auto, otherwise not.
 */
@property(nonatomic,assign)BOOL  isTimerAutoStopBOEnabled;

/**
 * @brief Seconds of BO timer duration, NOTE: when nTimerDuration is 0, it means that the BO duration is 30*60 seconds.
 */
@property(nonatomic,assign)unsigned int  nTimerDuration;

//  WebinarBo
/**
 * @brief Enable or disable webinar attendee join webinar BO. When it changes, the BO data will be reset.
 */
@property(nonatomic,assign) BOOL isAttendeeContained;

/**
 * @brief Enable or disable that panelist can choose breakout room.
 */
@property(nonatomic,assign) BOOL isPanelistCanChooseBO;

/**
 * @brief Enable or disable that Attendee can choose breakout room, invalid when attendee is not contained.
 */
@property(nonatomic,assign) BOOL isAttendeeCanChooseBO;

/**
 * @brief Enable or disable that max roomUser limits in BO room.
 */
@property(nonatomic,assign) BOOL isUserConfigMaxRoomUserLimitsEnabled;

/**
 * @brief Numbers of max roomUser limits in BO room, default is 20.
 */
@property(nonatomic,assign)unsigned int nUserConfigMaxRoomUserLimits;

@end


/**
 * @protocol ZoomSDKBOMeetingCreatorDelegate
 * @brief Delegate protocol for breakout room creation related callbacks.
 */
@protocol ZoomSDKBOMeetingCreatorDelegate <NSObject>
@optional

/**
 * @brief Callback of breakout room create success.
 * @param boID Breakout meeting ID.
 * @deprecated Use \link ZoomSDKBOMeetingCreatorDelegate::onCreateBOResponse:boID: \endlink instead.
 */
- (void)onBOCreateSuccess:(NSString *)boID DEPRECATED_MSG_ATTRIBUTE("Use -onCreateBOResponse:boID: instead");

/**
 * @brief When the pre-assigned data download status changes, you will receive the event.
 * @param status download status.
 */
- (void)onWebPreAssignBODataDownloadStatusChanged:(ZoomSDKPreAssignBODataStatus)status;

/**
 * @brief You will receive the event when the option changes.
 * @param newOption The new bo option.
 */
- (void)onBOOptionChanged:(ZoomSDKBOOption*)newOption;

/**
 * @brief The callback notification of "-createBreakoutRoom:".
 * @param bSuccess Indicate whether the creation is actually successful, YES indicates success, NO indicates failure.
 * @param boID If the creation is successful, its value is the breakout room's ID, otherwise the value is nil.
 */
- (void)onCreateBOResponse:(BOOL)bSuccess boID:(NSString *)boID;

/**
 * @brief The callback notification of ''-removeBO:'.
 * @param bSuccess Used to indicate whether the creation is actually successful, YES indicates success, NO indicates failure.
 * @param boID Identifies which breakout room is being removed.
 */
- (void)onRemoveBOResponse:(BOOL)bSuccess boID:(NSString *)boID;

/**
 * @brief The callback notification of "-updateBOName:BOID".
 * @param bSuccess Used to indicate whether the creation is actually successful, YES indicates success, NO indicates failure.
 * @param boID Identifies which breakout room is being updated.
 */
- (void)onUpdateBONameResponse:(BOOL)bSuccess boID:(NSString *)boID;
@end

/**
 * @class ZoomSDKBOMeetingCreator
 * @brief Interface for creating and managing breakout rooms.
 */
@interface ZoomSDKBOMeetingCreator : NSObject
/**
 * @brief Set or get the delegate to receive breakout room creation callbacks.
 */
@property(nonatomic, assign, nullable)id<ZoomSDKBOMeetingCreatorDelegate>  delegate;

/**
 * @brief Create a breakout room.
 * @param name The breakout room name.
 * @return If the function succeeds, will return YES.
 * @note 1. This function is compatible with meeting breakout rooms and webinar breakout rooms.  2. This function is asynchronous, onCreateBOResponse is the corresponding callback notification. 3. Webinar breakout room only support Zoomui Mode.
 */
- (BOOL)createBreakoutRoom:(NSString *)name;

/**
 * @brief Creator breakout meeting.
 * @param name Breakout meeting name,should less than 50 characters.
 * @return If the function succeeds, will return breakout meeting id, otherwise return nil,
 * @deprecated Use \link ZoomSDKBOMeetingCreator::createBreakoutRoom: \endlink instead.
 */
- (NSString *_Nullable)createBOWithBOName:(NSString*)name DEPRECATED_MSG_ATTRIBUTE("Use -createBreakoutRoom: instead");

/**
 * @brief Creator breakout meeting in batch.
 * @param nameList Breakout meeting name list, the element of nameList should less than 50 characters.
 * @return If the function succeeds, will return ZoomSDKError_Success.
 */
- (ZoomSDKError)createBOWithBONameList:(NSArray<NSString *>*_Nullable)nameList;

/**
 * @brief Creator webinar breakout meeting.
 * @param nameList Breakout meeting name list, the element of nameList should less than 50 characters.
 * @return If the function succeeds, will return YES.
 */
- (BOOL)createWebinarBO:(NSArray<NSString *>*)nameList;

/**
 * @brief Update breakout meeting information. '-onUpdateBONameResponse:boID:' is the corresponding callback notification.
 * @param newName Breakout meeting new name.
 * @param ID Breakout meeting ID.
 * @return If the function succeeds, will return ZoomSDKError_Success.
 */
- (ZoomSDKError)updateBOName:(NSString*)newName  BOID:(NSString *)ID;

/**
 * @brief Remove breakout meeting by ID. '-onRemoveBOResponse:boID:' is the corresponding callback notification.
 * @param BOID Need remove breakout meeting ID.
 * @return If the function succeeds, will return ZoomSDKError_Success.
 */
- (ZoomSDKError)removeBO:(NSString*)BOID;

/**
 * @brief Assign user to Breakout meeting.
 * @param userID Assigned user ID.
 * @param ID Breakout meeting ID.
 * @return If the function succeeds, will return ZoomSDKError_Success.
 */
- (ZoomSDKError)assignUserToBO:(NSString *)userID  BOID:(NSString *)ID;

/**
 * @brief Removed user from Breakout meeting.
 * @param userID Removed user ID.
 * @param ID Breakout meeting ID.
 * @return If the function succeeds, will return ZoomSDKError_Success.
 */
- (ZoomSDKError)removeUserFromBO:(NSString *)userID  BOID:(NSString *)ID;

/**
 * @brief Set BO option.
 * @param boOption The option that you want to set.
 * @return If the function succeeds, will return ZoomSDKError_Success.
 */
- (ZoomSDKError)setBOOption:(ZoomSDKBOOption *_Nullable)boOption;

/**
 * @brief Get BO option.
 * @return The value is bo option object.
 */
- (ZoomSDKBOOption *_Nullable)getBOOption;

/**
 * @brief Check whether web enabled the pre-assigned option when scheduling a meeting.
 * @return If return YES means enabled, otherwise not.
 */
- (BOOL)isWebPreAssignBOEnabled;

/**
 * @brief Request web pre-assigned data and create those rooms.
 * @return If the function succeeds, will return ZoomSDKError_Success.
 */
- (ZoomSDKError)requestAndUseWebPreAssignBOList;

/**
 * @brief Get the downloading status of pre-assigned data.
 * @return If the function succeeds, the return value is the web pre-assigned data download status.
 */
- (ZoomSDKPreAssignBODataStatus)getWebPreAssignBODataStatus;
@end


/**
 * @brief Delegate to receive breakout room admin callbacks.
 */
@protocol ZoomSDKBOMeetingAdminDelegate <NSObject>
@optional
/**
 * @brief Host will recieve this callback when attendee request for help.
 * @param userID The ID of user who request for help.
 */
- (void)onHelpRequestReceived:(NSString *)userID;

/**
 * @brief Callback of start breakout room fail.
 * @param errCode The reason of fail.
 */
- (void)onStartBOError:(ZoomSDKBOControllerError)errCode;

/**
 * @brief If it's timer BO, after start BO, you will receive the event. .
 * @param remaining Remaining time before bo end ,the unit is seconds.
 * @param isTimesUpNotice If the isTimesUpNotice is YES means the bo meeting will end automatically,otherwise not.
 */
- (void)onBOEndTimerUpdated:(unsigned int)remaining isTimesUpNotice:(BOOL)isTimesUpNotice;

/**
 * @brief The callback notification of "-startBO".
 * @param bSuccess Indicates whether the startup is actually successful. YES indicates success, and NO indicates failure.
 */
- (void)onStartBOResponse:(BOOL)bSuccess;

/**
 * @brief The callback notification of "-stopBO".
 * @param bSuccess Indicates whether the stop is actually successful. YES indicates success, and NO indicates failure.
 */
- (void)onStopBOResponse:(BOOL)bSuccess;
@end

@interface ZoomSDKBOMeetingAdmin : NSObject
/**
 * @brief Set or get the delegate to receive breakout room admin callbacks.
 */
@property(nonatomic,assign,nullable)id<ZoomSDKBOMeetingAdminDelegate>  delegate;
/**
 * @brief Start breakout meeting. '-onStartBOResponse:' is the corresponding callback notification.
 * @return If the function succeeds, will return ZoomSDKError_Success.
 */
- (ZoomSDKError)startBO;

/**
 * @brief Stop breakout meeting. '-onStopBOResponse:' is the corresponding callback notification.
 * @return If the function succeeds, will return ZoomSDKError_Success.
 */
- (ZoomSDKError)stopBO;

/**
 * @brief Assign user to is runing breakout meeting.
 * @param userID Assigned user ID.
 * @param ID Breakout meeting ID.
 * @return If the function succeeds, will return ZoomSDKError_Success.
 */
- (ZoomSDKError)assignNewUserToRunningBO:(NSString *)userID  BOID:(NSString *)ID;

/**
 * @brief Switch Assigned user to differnt Breakout meeting(BO-A to BO-B).
 * @param userID Assigned user ID.
 * @param ID Breakout meeting ID.
 * @return If the function succeeds, will return ZoomSDKError_Success.
 */
- (ZoomSDKError)switchAssignedUserToRunningBO:(NSString *)userID  BOID:(NSString *)ID;

/**
 * @brief  Determine if the user can start breakout room.
 * @return If return YES means can start breakout room,otherwise not.
 */
- (BOOL)canStartBO;

/**
 * @brief User request to join breakout room.
 * @param requestUserID The user id.
 * @return If the function succeeds, will return ZoomSDKError_Success.
 */
- (ZoomSDKError)joinBOByUserRequest:(NSString *)requestUserID;

/**
 * @brief Notify attendee request help result.
 * @param userID The user id of requested help.
 * @return If the function succeeds, will return ZoomSDKError_Success.
 */
- (ZoomSDKError)ignoreUserHelpRequest:(NSString *)userID;

/**
 * @brief Broadcast message.
 * @param message The broadcast context.
 * @return If the function succeeds, will return ZoomSDKError_Success.
 */
- (ZoomSDKError)broadcastMessage:(NSString *)message;

/**
 * @brief Host invite user return to main session, When BO is started and user is in BO.
 * @param userID The user id.
 * @return If the function succeeds, will return ZoomSDKError_Success.
 */
- (ZoomSDKError)inviteBOUserReturnToMainSession:(NSString *)userID;

/**
 * @brief Query if the current meeting supports broadcasting host's voice to BO.
 * @return YES means that the meeting supports this, otherwise it's not supported.
 */
- (BOOL)isBroadcastVoiceToBOSupport;

/**
 * @brief Query if the host now has the ability to broadcast voice to BO.
 * @return YES means that the host now has the ability, otherwise the host does not.
 */
- (BOOL)canBroadcastVoiceToBO;

/**
 * @brief Start or stop broadcasting voice to BO.
 * @param start YES for start and NO for stop.
 * @return If the function succeeds, will return ZoomSDKError_Success.
 */
- (ZoomSDKError)broadcastVoiceToBO:(BOOL)start;
@end


/**
 * @class ZoomSDKBOMeetingAssistant
 * @brief Manages operations for joining and leaving breakout meetings as an assistant.
 */
@interface ZoomSDKBOMeetingAssistant : NSObject

/**
 * @brief Join Breakout meeting by Breakout meeting ID.
 * @param ID Breakout meeting ID.
 * @return If the function succeeds, will return ZoomSDKError_Success.
 */
- (ZoomSDKError)joinBO:(NSString*)ID;

/**
 * @brief Leave Breakout meeting.
 * @return If the function succeeds, will return ZoomSDKError_Success.
 */
- (ZoomSDKError)leaveBO;
@end


/**
 * @brief Delegate to receive breakout room attendee callbacks.
 */
@protocol ZoomSDKBOMeetingAttendeeDelegate <NSObject>
@optional
/**
 * @brief Notify current user the request for help result.
 * @param result It is the request for help result.
 */
- (void)onHelpRequestHandleResultReceived:(ZoomSDKRequest4HelpResult)result;

/**
 * @brief Notify the host join current breakout room.
 */
- (void)onHostJoinedThisBOMeeting;

/**
 * @brief Notify the host leave current breakout room.
 */
- (void)onHostLeaveThisBOMeeting;

@end


/**
 * @class ZoomSDKBOMeetingAttendee
 * @brief Manages operations for attendees in breakout meetings.
 */
@interface ZoomSDKBOMeetingAttendee : NSObject
/**
 * @brief Set or get the delegate to receive breakout room attendee callbacks.
 */
@property(nonatomic,assign,nullable)id<ZoomSDKBOMeetingAttendeeDelegate>  delegate;
/**
 * @brief Join Breakout meeting.
 * @return If the function succeeds, will return ZoomSDKError_Success.
 */
- (ZoomSDKError)joinBO;

/**
 * @brief Leave breakout meeting.
 * @return If the function succeeds, will return ZoomSDKError_Success.
 */
- (ZoomSDKError)leaveBO;

/**
 * @brief Get breakout meeting name.
 * @return If the function succeeds, will return breakout meeting name.
 */
- (NSString*_Nullable)getBOName;

/**
 * @brief Request for help when user in breakout room.
 * @return If the function succeeds, will return ZoomSDKError_Success.
 */
- (ZoomSDKError)requestForHelp;

/**
 * @brief Determine if the host is in this breakout room.
 * @return YES means the host in breakout room,otherwise not.
 */
- (BOOL)isHostInThisBO;

/**
 * @brief Determine if participant can return to main session.
 * @return YES means participant can return to main session,otherwise not.
 */
- (BOOL)isCanReturnMainSession;

@end

/**
 * @brief Delegate to receive breakout room data helper callbacks.
 */
@protocol ZoomSDKBOMeetingDataHelpDelegate <NSObject>
@optional

/**
 * @brief If breakout meeting info changed, will receive the callback.
 * @param boID Breakout meeting ID.
 */
- (void)onBOMeetingInfoUpdated:(NSString *)boID;

/**
 * @brief If the unassigned user changed, will receive the callback.
 */
- (void)onUnAssignedUserUpdated;

/**
 * @brief If breakout meeting list changed, will receive the callback.
 */
- (void)onBOListInfoUpdated;
@end


/**
 * @class ZoomSDKBOMeetingDataHelp
 * @brief Provides data helper functions for breakout meetings.
 */
@interface ZoomSDKBOMeetingDataHelp : NSObject

/**
 * @brief Set or get the delegate to receive breakout room data helper callbacks.
 */
@property(nonatomic,assign,nullable)id<ZoomSDKBOMeetingDataHelpDelegate>  delegate;

/**
 * @brief Get unassigned user list.
 * @return If the function succeeds, will return unassigned user array.
 */
- (NSArray *_Nullable)getUnassignedUserList;

/**
 * @brief Get breakout meeting ID list.
 * @return If the function succeeds, will return breakout meeting ID array.
 */
- (NSArray *_Nullable)getBOMeetingIDList;

/**
 * @brief Get breakout meeting user name.
 * @param userID The user's user ID.
 * @return If the function succeeds, will return user name.
 */
- (NSString *_Nullable)getBOUserNameWithUserID:(NSString *)userID;

/**
 * @brief Get breakout meeting info.
 * @param BOID Breakout meeting ID.
 * @return If the function succeeds, will return ZoomSDKBOMeetingInfo object.
 */
- (ZoomSDKBOMeetingInfo *_Nullable)getBOMeetingInfoWithBOID:(NSString *)BOID;

/**
 * @brief Determine if it is yourself by userID.
 * @param userid The user's user ID.
 * @return YES means yourself is in the breakout meeting ,otherwise not.
 */
- (BOOL)isMyselfInBo:(NSString *)userid;

/**
 * @brief Get breakout meeting name.
 * @return The name of current breakout meeting.
 */
- (NSString*_Nullable)getCurrentBoName;
@end


/**
 * @class ZoomSDKBOMeetingReturnToMainSessionHandler
 * @brief Handler class to manage returning to main session from breakout meeting.
 */
@interface ZoomSDKBOMeetingReturnToMainSessionHandler : NSObject

/**
 * @brief Return to main session.
 * @return If the function succeeds, will return ZoomSDKError_Success.
 */
- (ZoomSDKError)returnToMainSession;

/**
 * @brief Ignore the return invitation, after call this api, please don't use the handler unless you receive the invitation again.
 */
- (void)ignore;

@end


/**
 * @protocol ZoomSDKNewBreakoutRoomControllerDelegate
 * @brief Delegate protocol to receive new breakout room controller events and permission changes.
 */
@protocol ZoomSDKNewBreakoutRoomControllerDelegate <NSObject>
@optional

/**
 * @brief If the creator's permissions change, will receive the callback.
 * @param creatorObject ZoomSDKBOMeetingCreator class object.
 */
- (void)onHasCreatorPermission:(ZoomSDKBOMeetingCreator *_Nullable)creatorObject;

/**
 * @brief If the admin's permissions change, will receive the callback.
 * @param adminObject ZoomSDKBOMeetingAdmin object.
 */
- (void)onHasAdminPermission:(ZoomSDKBOMeetingAdmin *_Nullable)adminObject;

/**
 * @brief If the assistant's permissions change, will receive the callback.
 * @param assistantObject ZoomSDKBOMeetingAssistant object.
 */
- (void)onHasAssistantPermission:(ZoomSDKBOMeetingAssistant *_Nullable)assistantObject;

/**
 * @brief If the attendee's permissions change, will receive the callback.
 * @param attendeeObject ZoomSDKBOMeetingAttendee object.
 */
- (void)onHasAttendeePermission:(ZoomSDKBOMeetingAttendee *_Nullable)attendeeObject;

/**
 * @brief If the dataHelper's permissions change, will receive the callback.
 * @param dataHelpObject ZoomSDKBOMeetingDataHelp object.
 */
- (void)onHasDataHelperPermission:(ZoomSDKBOMeetingDataHelp *_Nullable)dataHelpObject;

/**
 * @brief If lost creator's permissions change, will receive the callback.
 */
- (void)onLostCreatorPermission;

/**
 * @brief If lost admin's permissions change, will receive the callback.
 */
- (void)onLostAdminPermission;

/**
 * @brief If lost assistant's permissions change, will receive the callback.
 */
- (void)onLostAssistantPermission;

/**
 * @brief If lost attendee's permissions change, will receive the callback.
 */
- (void)onLostAttendeePermission;

/**
 * @brief If lost dataHelper's permissions change, will receive the callback.
 */
- (void)onLostDataHelperPermission;

/**
 * @brief If host broadcast message,all attendee will revieve this callback
 * @param message The broadcast message context.
 * @param userid The sender message user id.
 * @param senderName The sender message user name.
*/
- (void)onNewBroadcastMessageReceived:(NSString *)message senderUserId:(unsigned int)userid senderName:(NSString*)senderName;

/**
 * @brief If countDown != ZoomSDKBOStopCountDown_Not, host stop BO and all users receive the event.
 * @param countDown The countdown seconds.
 */
- (void)onBOMeetingStopCountDown:(unsigned int)countDown;

/**
 * @brief If you are in BO, host invite you return to main session, you will receive the event.
 * @param userName The host name.
 * @param handler The ZoomSDKBOMeetingReturnToMainSessionHandler object.
 */
- (void)onHostInviteReturnToMainSession:(NSString*)userName handler:(ZoomSDKBOMeetingReturnToMainSessionHandler*)handler;

/**
 * @brief When host change the BO status, all users receive the event.
 * @param status Current status of BO.
 */
- (void)onBOControlStatusChanged:(ZoomSDKBOStatus)status;

/**
 * @brief Whenever the host switches you to another BO while you are assigned but haven't joined the BO, you will receive this event.
 * @param newBOName The new BO name.
 * @param newBOID The new BO ID. If the current user is ZoomSDKBOMeetingAttendee, then the 2nd parameter strNewBOID will return nil.
 */
- (void)onBOSwitchRequestReceived:(NSString*)newBOName newBOID:(NSString*_Nullable)newBOID;

/**
 * @brief The status of broadcasting voice to BO has been changed.
 * @param start YES for start and NO for stop.
 */
- (void)onBroadcastBOVoiceStatus:(BOOL)start;

/**
 * @brief You will receive this event when you are in a breakout room, and someone shares from the main session to the breakout room.
 * @param sharingID The sharing ID.
 * @param status The sharing status.
 * @param shareAction The pointer of share action object.
 */
- (void)onShareFromMainSession:(unsigned int)sharingID status:(ZoomSDKShareStatus)status shareAction:(ZoomSDKShareAction *)shareAction;
@end



/**
 * @class ZoomSDKNewBreakoutRoomController
 * @brief Controller class to manage breakout room features and permissions.
 */
@interface ZoomSDKNewBreakoutRoomController : NSObject
/**
 * @brief Delegate to receive breakout room controller callbacks.
 */
@property(nonatomic,assign,nullable)id<ZoomSDKNewBreakoutRoomControllerDelegate>  delegate;

/**
 * @brief Get ZoomSDKBOMeetingCreator Class object.
 * @return If the function succeeds, will return ZoomSDKBOMeetingCreator object.
 * @note Only host can get this object.
 */
- (ZoomSDKBOMeetingCreator *_Nullable)getBOMeetingCreator;

/**
 * @brief Get ZoomSDKBOMeetingAdmin Class object.
 * @return If the function succeeds, will return ZoomSDKBOMeetingAdmin object.
 * @note Host in master meeting or breakout meeting can get this object.
 */
- (ZoomSDKBOMeetingAdmin *_Nullable)getBOMeetingAdmin;

/**
 * @brief Get ZoomSDKBOMeetingAssistant Class object.
 * @return If the function succeeds, will return ZoomSDKBOMeetingAssistant object.
 * @note Host in master/breakout meeting or cohost in breakmeeting can get this object.
 */
- (ZoomSDKBOMeetingAssistant *_Nullable)getBOMeetingAssistant;

/**
 * @brief Get ZoomSDKBOMeetingAttendee Class object.
 * @return If the function succeeds, will return ZoomSDKBOMeetingAttendee object.
 * @note If you are CoHost/attendee, and are assigned to BO, you will get this object.
 */
- (ZoomSDKBOMeetingAttendee *_Nullable)getBOMeetingAttendee;

/**
 * @brief Get ZoomSDKBOMeetingDataHelp Class object.
 * @return If the function succeeds, will return ZoomSDKBOMeetingDataHelp object.
 * @note Host in master/breakout meeting or cohost in breakmeeting can get this object.
 */
- (ZoomSDKBOMeetingDataHelp *_Nullable)getBOMeetingDataHelp;

/**
 * @brief Determine if breakout meeting is start.
 * @return If the function succeeds, it will return YES, otherwise not.
 */
- (BOOL)isBOStart;

/**
 * @brief Determine if breakout meeting is enable.
 * @return If the function succeeds, it will return YES, otherwise not.
 */
- (BOOL)isBOEnable;

/**
 * @brief Determine if user is in breakout meeting.
 * @return If the function succeeds, it will return YES, otherwise not.
 */
- (BOOL)isInBOMeeting;

/**
 * @brief Get current BO status.
 * @return The value is a enum for bo status.
 */
- (ZoomSDKBOStatus)getBOStatus;

/**
 * @brief Query if the host is broadcasting voice to BO.
 * @return YES means that the host is broadcasting, otherwise it's not broadcasting.
 */
- (BOOL)isBroadcastingVoiceToBO;

/**
 * @brief Get the name of the BO you are going to.
 * @return The value is join bo name which you are going to.
 * @note When you enter a BO or are switched to another BO by the host, maybe you need the BO name to display on transfer UI.
 */
- (NSString *_Nullable)getJoiningBOName;
@end

NS_ASSUME_NONNULL_END
