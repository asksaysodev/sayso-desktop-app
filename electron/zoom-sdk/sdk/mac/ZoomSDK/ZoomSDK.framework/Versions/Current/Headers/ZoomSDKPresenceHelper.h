/**
 * @file ZoomSDKPresenceHelper.h
 * @brief Interfaces for contact management, presence subscription, and meeting invitations in Zoom SDK.
 */

#import <Foundation/Foundation.h>
#import <ZoomSDK/ZoomSDKErrors.h>

NS_ASSUME_NONNULL_BEGIN
/**
 * @class ZoomSDKContactInfo
 * @brief Represents contact information of a Zoom user.
 */
@interface ZoomSDKContactInfo : NSObject
/**
 * @brief Get the contact’s ID.
 */
@property (copy, nonatomic,readonly, nullable) NSString *contactID;

/**
 * @brief Get the contact’s name.
 */
@property (copy, nonatomic,readonly, nullable) NSString *contactName;

/**
 * @brief Get the contact’s presence status. such as ZoomSDKPresenceStatus_Available.
 */
@property (assign, nonatomic,readonly) ZoomSDKPresenceStatus presenceStatus;

/**
 * @brief Get the contact’s profile picture.
 */
@property (copy, nonatomic,readonly, nullable) NSString *profilePicture;

/**
 * @brief Get the contact’s personal note.
 */
@property (copy, nonatomic,readonly, nullable) NSString *personalNote;

/**
 * @brief Get the contact’s company name. such as "Acme Incorporated".
 */
@property (copy, nonatomic,readonly, nullable) NSString *companyName;

/**
 * @brief Get the contact’s department. such as "Human resources".
 */
@property (copy, nonatomic,readonly, nullable) NSString *department;

/**
 * @brief Get the contact’s job title. such as "Support engineer".
 */
@property (copy, nonatomic,readonly, nullable) NSString *jobTitle;

/**
 * @brief Get the contact’s phone number. such as "+1 000 000-0000".
 */
@property (copy, nonatomic,readonly, nullable) NSString *phoneNumber;

/**
 * @brief Get the contact’s email. such as "jillchill@example.com".
 */
@property (copy, nonatomic,readonly, nullable) NSString *email;
@end



/**
 * @class ZoomSDKInvitationMeetingHandler
 * @brief Interface for handling received meeting invitations.
 */
@interface ZoomSDKInvitationMeetingHandler : NSObject

/**
 * @brief Get the inviter’s name.
 */
@property (copy, nonatomic, readonly, nullable) NSString *senderName;

/**
 * @brief Get the inviter's ID.
 */
@property (copy, nonatomic, readonly, nullable) NSString *senderId;

/**
 * @brief Get the invite meeting number.
 */
@property (assign, nonatomic,readonly) long long meetingNumber;

/**
 * @brief Determine whether this invitation is from the channel.
 */
@property (assign, nonatomic,readonly) BOOL isChannelInvitation;

/**
 * @brief Get the channel name.
 */
@property (copy, nonatomic, readonly, nullable) NSString *channelName;

/**
 * @brief Get the channel member count.
 */
@property (assign, nonatomic,readonly) unsigned int channelMemberCount;

/**
 * @brief Set the display name in meeting.
 * @param screenName The display name in meeting.
 */
- (ZoomSDKError)setScreenName:(NSString *)screenName;
/**
 * @brief Accept the meeting invitation.
 * @return If the function succeeds, it will return ZoomSDKError_success, otherwise not.
 */
- (ZoomSDKError)accept;

/**
 * @brief Decline the meeting invitation.
 * @return If the function succeeds, it will return ZoomSDKError_success, otherwise not.
 */
- (ZoomSDKError)decline;

/**
 * @brief Mark the meeting invitation as timeout.
 * @return If the function succeeds, it will return ZoomSDKError_success, otherwise not.
 */
- (ZoomSDKError)timeout;
@end


/**
 * @protocol ZoomSDKPresenceHelperDelegate
 * @brief Callback protocol for presence and invitation-related events.
 */
@protocol ZoomSDKPresenceHelperDelegate <NSObject>

/**
 * @brief Callback to request star contact ID list.
 * @param contactIDList Star contact result value.
 */
- (void)onRequestStarContact:(NSArray <NSString *> *)contactIDList;

/**
 * @brief Callback to request the contact detail information list.
 * @param contactList Contact detail information list.
 */
- (void)onRequestContactDetailInfo:(NSArray <ZoomSDKContactInfo *> *)contactList;

/**
 * @brief Contact presence changed callback.
 * @param contactID  The contact’s ID.
 * @param status Contact presence status.
 */
- (void)onContactPresenceChanged:(NSString *)contactID presenceStatus:(ZoomSDKPresenceStatus)status;

/**
 * @brief Starred contact list changed callback.
 * @param contactIDList The ID list of change contacts.
 * @param add Specifies whether the contact list is added.
 */
- (void)onStarContactListChanged:(NSArray <NSString *> *)contactIDList isAdd:(BOOL)add;

/**
 * @brief Callback to receive a meeting invitation.
 * @param handler The object of ZoomSDKInvitationMeetingHandler.
 */
- (void)onReceiveInvitationToMeeting:(ZoomSDKInvitationMeetingHandler *)handler;

/**
 * @brief The meeting invitation is canceled by inviter callback.
 * @param meetingNumber Cancel meeting number.
 */
- (void)onMeetingInvitationCanceled:(long long)meetingNumber;

/**
 * @brief Callback to accept the meeting invitation on another device.
 * @param meetingNumber Accept meeting number.
 */
- (void)onMeetingAcceptedByOtherDevice:(long long)meetingNumber;

/**
 * @brief Callback to decline the meeting invitation on another device.
 * @param meetingNumber meeting number that declined by other device.
 */
- (void)onMeetingDeclinedByOtherDevice:(long long)meetingNumber;

/**
 * @brief Callback to decline the meeting invitation.
 * @param contactID The IDs of contacts who decline the invitation.
 */
- (void)onMeetingInvitationDeclined:(NSString *)contactID;
@end


/**
 * @class ZoomSDKPresenceHelper
 * @brief Interface to manage contact presence and meeting invitations.
 */
@interface ZoomSDKPresenceHelper : NSObject
/**
 * @brief Delegate to receive presence-related callbacks.
 */
@property(nonatomic, assign, nullable)id<ZoomSDKPresenceHelperDelegate> delegate;

/**
 * @brief Send a request to get the starred contact IDs.
 * @return If the function succeeds, it will return ZoomSDKError_success, otherwise not.
 */
- (ZoomSDKError)requestStarContact;

/**
 * @brief Send a request to add the contact into a starred contact list.
 * @param contactID The contact ID.
 * @return If the function succeeds, it will return ZoomSDKError_success, otherwise not.
 */
- (ZoomSDKError)addStarContact:(NSString *)contactID;

/**
 * @brief Send a request to remove a contact from the starred contact list.
 * @param contactID The contact ID.
 * @return If the function succeeds, it will return ZoomSDKError_success, otherwise not.
 */
- (ZoomSDKError)unStarContact:(NSString *)contactID;

/**
 * @brief Send an invitation to a contact to join a meeting.
 * @param contactID The contact ID.
 * @return If the function succeeds, it will return ZoomSDKError_success, otherwise not.
 */
- (ZoomSDKError)inviteContact:(NSString *)contactID;

/**
 * @brief batch invite a list of specified contact to the current meeting.
 * @param contactsIDList A list which contains contactsID of the specified user.
 * @return If the function succeeds, it will return ZoomSDKError_success, otherwise not.
 */
- (ZoomSDKError)inviteContactList:(NSArray <NSString *> *)contactIDList;

/**
 * @brief Send a request for contact detail information according to the contact ID list.
 * @param contactIDList The contact ID list.
 * @return If the function succeeds, it will return ZoomSDKError_success, otherwise not.
 */
- (ZoomSDKError)requestContactDetailInfo:(NSArray <NSString *> *)contactIDList;

/**
 * @brief Send a request to subscribe contact presence status according to the contact ID list.
 * @param contactIDList The contact ID list.
 * @return If the function succeeds, it will return ZoomSDKError_success, otherwise not.
 */
- (ZoomSDKError)subscribeContactPresence:(NSArray <NSString *> *)contactIDList;

/**
 * @brief Send a request to unsubscribe contact presence status according to the contact ID list.
 * @param contactIDList The contact ID list.
 * @return If the function succeeds, it will return ZoomSDKError_success, otherwise not.
 */
- (ZoomSDKError)unSubscribeContactPresence:(NSArray <NSString *> *)contactIDList;
@end

NS_ASSUME_NONNULL_END
