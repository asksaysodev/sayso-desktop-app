/**
 * @file ZoomSDKPhoneHelper.h
 * @brief Interface for Zoom SDK phone helper to manage telephone call-out, call-in and related features.
 */

#import <ZoomSDK/ZoomSDKErrors.h>
NS_ASSUME_NONNULL_BEGIN

/**
 * @protocol ZoomSDKPhoneHelperDelegate
 * @brief Delegate protocol to receive phone call-out and call-me status updates.
 */
@protocol ZoomSDKPhoneHelperDelegate <NSObject>
/**
 * @brief Invite others by telephone call-out and send the response to the application according to the others’ status.
 * @param status The telephone’s status.
 * @param reason The reason for the failure if the status value is PhoneStatus_Failed.
 */
- (void)onInviteCalloutUserStatus:(PhoneStatus)status FailedReason:(PhoneFailedReason)reason;

/**
 * @brief Invite others to join the meeting by CALL ME and give the response to the application according to the status.
 * @param status The telephone’s status.
 * @param reason The reason for the failure if the status value is PhoneStatus_Failed.
 */
- (void)onCallMeStatus:(PhoneStatus)status FailedReason:(PhoneFailedReason)reason;
@end


/**
 * @class ZoomSDKPhoneSupportCountryInfo
 * @brief Provides information of countries that support calling out.
 */
@interface ZoomSDKPhoneSupportCountryInfo : NSObject
/**
 * @brief Get the ID of the country where a user can dial in.
 * @return The country ID. (e.g. US, CA, etc.)
 */
- (NSString*)getCountryID;
/**
 * @brief Get the country name.
 * @return The country name.
 */
- (NSString*)getCountryName;
/**
 * @brief Get the country code.
 * @return The country code. (e.g. +1, +33, etc.).
 */
- (NSString*)getCountryCode;
@end


/**
 * @class ZoomSDKCallInPhoneNumInfo
 * @brief Represents call-in phone number information.
 */
@interface ZoomSDKCallInPhoneNumInfo : NSObject
/**
 * @brief Get the current call-in number’s country ID.
 * @return The country ID.
 */
- (NSString*) getID;
/**
 * @brief Get the current call-in number’s country code.
 * @return The country code.
 */
- (NSString*) getCode;
/**
 * @brief Get the current call-in number’s country name.
 * @return The country name.
 */
- (NSString*) getName;
/** 
 * @brief Get the current call-in number’s telephone number.
 * @return The telephone number.
 */
- (NSString*) getNumber;
/**
 * @brief Get the current call-in number’s display number.
 * @return The display number.
 */
- (NSString*) getDisplayNumber;
/**
 * @brief Get the call-in number’s call type.
 * @return Call type.
 */
- (CallInNumberType) getType;
@end


/**
 * @class ZoomSDKPhoneHelper
 * @brief Provides APIs to manage Zoom phone call-in and call-out features.
 */
@interface ZoomSDKPhoneHelper : NSObject
{
    id<ZoomSDKPhoneHelperDelegate> _delegate;
    PhoneStatus _callMeStatus;
}
/**
 * @brief Set the delegate to receive phone call status updates.
 */
@property(nonatomic, assign, nullable)id<ZoomSDKPhoneHelperDelegate> delegate;
/**
 * @brief Query if it is able to dial out in the current meeting.
 * @return YES means able to dial out. Otherwise return NO.
 */
- (BOOL)isSupportPhone;

/**
 * @brief Query if it is able to dial out in the current meeting.
 * @return YES means able. Otherwise return NO.
 */
- (BOOL)isDialoutSupported;

/**
 * @brief Get the list of the countries which support calling out.
 * @return An NSArray contains ZoomSDKPhoneSupportCountryInfoList objects of all countries supporting to call out.  Otherwise nil.
 */
- (NSArray*)getSupportCountryInfo;

/**
 * @brief Invite the specified user to join the meeting by calling out.
 * @param userName The specified user’s in-meeting screen name.
 * @param number The specified user’s phone number.
 * @param countryCode The specified user’s country code must be in the support list.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise failed. 
 */
- (ZoomSDKError)inviteCalloutUser:(NSString*)userName PhoneNumber:(NSString*)number CountryCode:(NSString*)countryCode;

/**
 * @brief Cancel the invitation that is being called out by phone.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)cancelCalloutUser;

/**
 * @brief  Get the invited user’s status by calling out.
 * @return If the function succeeds, the return value is the current callout process.
 */
- (PhoneStatus)getInviteCalloutUserStatus;

/**
 * @brief Invite myself to join audio to the meeting by phone. 
 * @param number The device’s phone number.
 * @param countryCode The country code.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)callMe:(NSString*)number CountryCode:(NSString*)countryCode;

/**
 * @brief Cancel the current CALL ME action.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)hangUp;

/**
 * @brief Get the status of myself by CALL ME.
 * @return If the function succeeds, the return value is the process of the invitation by CALL ME.
 */
- (PhoneStatus)getCallMeStatus;

/**
 * @brief Get the ID of the participant who joins the meeting by calling in.
 * @return If the function succeeds, the return value is the ID of participant.
 */
- (unsigned int)getCallInParticipantID;

/**
* @brief Get the list of call-in numbers supported by the meeting.
* @return If the function succeeds, it will return an array of ZoomSDKCallInPhoneNumInfo objects.
*/
- (NSArray*_Nullable)getCallInNumberInfo;

@end
NS_ASSUME_NONNULL_END
