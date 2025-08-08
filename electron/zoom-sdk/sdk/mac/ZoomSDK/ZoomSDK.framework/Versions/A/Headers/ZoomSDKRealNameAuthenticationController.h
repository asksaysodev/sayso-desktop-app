/**
 * @file ZoomSDKRealNameAuthentication.h
 * @brief Interface for Zoom SDK Real Name Authentication module.
 */

#import <Foundation/Foundation.h>
#import <ZoomSDK/ZoomSDKErrors.h>

/**
 * @brief Enumeration of SMS verification code error.
 */
typedef enum
{
    /** Unknown error */
    ZoomSDKSMSError_Unknown,
    /** Operation success */
    ZoomSDKSMSError_Success,
    /** Failed to send SMS */
    ZoomSDKSMSError_Retrieve_SendSMSFailed,
    /** Invalid phone number */
    ZoomSDKSMSError_Retrieve_InvalidPhoneNum,
    /** Phone number already bound */
    ZoomSDKSMSError_Retrieve_PhoneNumAlreadyBound,
    /** SMS sending too frequent */
    ZoomSDKSMSError_Retrieve_PhoneNumSendTooFrequent,
    /** Verification code incorrect */
    ZoomSDKSMSError_Verify_CodeIncorrect,
    /** Verification code expired */
    ZoomSDKSMSError_Verify_CodeExpired,
    /** Unknown verification error */
    ZoomSDKSMSError_Verify_UnknownError,
}ZoomSDKSMSVerificationCodeError;

NS_ASSUME_NONNULL_BEGIN
/**
 * @class ZoomSDKRetrieveSMSVerificationCodeController
 * @brief Controller for sending SMS verification code.
 */
@interface ZoomSDKRetrieveSMSVerificationCodeController : NSObject

/**
 * @brief Send message to mobile phone.
 * @param code international area code for mobile phone number.
 * @param phoneNumber  user phone number.
 * @return If the function succeeds, will return ZoomSDKError_Success.
 */
- (ZoomSDKError)retrieveWithCountryCode:(NSString *)code PhoneNum:(NSString *)phoneNumber;

/**
 * @brief Cancel and leave meeting.
 * @return If the function succeeds, will return ZoomSDKError_Success.
 */
- (ZoomSDKError)cancelAndLeaveMeeting;
@end


/**
 * @class ZoomSDKVerifySMSVerificationCodeController
 * @brief Controller for verifying SMS verification code.
 */
@interface ZoomSDKVerifySMSVerificationCodeController : NSObject

/**
 * @brief Verify SMS verification code.
 * @param countryCode international area code for mobile phone number.
 * @param number  user phone number.
 * @param code  the SMS verification code.
 * @return If the function succeeds, will return ZoomSDKError_Success.
 */
- (ZoomSDKError)verifyWithCountryCode:(NSString *)countryCode  withPhoneNumber:(NSString *)number  withCode:(NSString *)code;

/**
 * @brief Cancel and leave meeting.
 * @return If the function succeeds, will return ZoomSDKError_Success.
 */
- (ZoomSDKError)cancelAndLeaveMeeting;
@end


/**
 * @class ZoomSDKRealNameAuthCountryInfo
 * @brief Model class to represent real-name authentication country info.
 */
@interface ZoomSDKRealNameAuthCountryInfo : NSObject
/**
 * @brief Get the country ID of mobile phone number.
 * @return the mobile phone number country ID.
 */
- (NSString *_Nullable)getCountryID;

/**
 * @brief Get the country Name of mobile phone number.
 * @return the mobile phone number country Name.
 */
- (NSString *_Nullable)getCountryName;

/**
 * @brief Get the country code of mobile phone number.
 * @return the mobile phone number country code.
 */
- (NSString *_Nullable)getCountryCode;
@end


/**
 * @protocol ZoomSDKRealNameAuthenticationDelegate
 * @brief Delegate to receive real-name authentication events.
 */
@protocol ZoomSDKRealNameAuthenticationDelegate <NSObject>

/**
 * @brief Notify support the Real-name authentication
 * @param supportCountryList  the sdk support country list.
 * @param privacyURL  the privacy url about Real-name authentication meeting
 * @param handler  object of ZoomSDKRetrieveSMSVerificationCodeController.
 */
- (void)onNeedRealNameAuthMeetingWithSupportCountryList:(NSArray *_Nullable)supportCountryList  withPrivacyURL:(NSString *)privacyURL withRetrieveSMSVerificationCodeHandler:(ZoomSDKRetrieveSMSVerificationCodeController *)handler;

/**
 * @brief Notify the send MSM verification code result
 * @param result the MSM send is success or not.
 * @param handler the handle work only when the result is ZoomSDKSMSError_Success.
 */
- (void)onRetrieveSMSVerificationCodeResult:(ZoomSDKSMSVerificationCodeError)result  withVerifySMSVerificationCodeHandle:(ZoomSDKVerifySMSVerificationCodeController *_Nullable)handler;

/**
 * @brief Notify the MSM verification code verify result.
 * @param result the SMS verification code is correct or not.
 */
- (void)onVerifySMSVerificationCodeResult:(ZoomSDKSMSVerificationCodeError)result;
@end



/**
 * @class ZoomSDKRealNameAuthenticationController
 * @brief Controller to manage Zoom SDK Real Name Authentication.
 */
@interface ZoomSDKRealNameAuthenticationController : NSObject
{
    id<ZoomSDKRealNameAuthenticationDelegate> _delegate;
}
/**
 * @brief Delegate to receive authentication events.
 */
@property(nonatomic,assign,nullable)id<ZoomSDKRealNameAuthenticationDelegate>  delegate;

/**
 * @brief Enable to show the zoom Real-name authentication meeting UI
 * @param enable Yes means show the zoom Real-name authentication meeting UI,otherwise not.
 * @return If the function succeeds, will return ZoomSDKError_Success.
 */
- (ZoomSDKError)enableZoomAuthRealNameMeetingUIShown:(BOOL)enable;

/**
 * @brief Get the country for mobile phone number supported by the SDK.
 * @return If the function succeeds, will get the support country list.
 */
- (NSArray *)getSupportPhoneNumberCountryList;

/**
 * @brief  Get the resend Verification Code Controller.
 * @return An object of ZoomSDKRetrieveSMSVerificationCodeController.
 */
- (ZoomSDKRetrieveSMSVerificationCodeController *)resendSMSVerificationCodeController;

/**
 * @brief  Get the reVerify Code Controller.
 * @return An object of ZoomSDKVerifySMSVerificationCodeController.
 */
- (ZoomSDKVerifySMSVerificationCodeController *)reVerifySMSVerificationCodeController;

/**
 * @brief  Set the default cell phone information.
 * @return If the function succeeds, will return ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)setDefaultCellPhoneInfo:(NSString*)countryCode phoneNumber:(NSString*)phoneNumber;
@end
NS_ASSUME_NONNULL_END
