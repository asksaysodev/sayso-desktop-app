/**
 * @file ZoomSDKAuthService.h
 * @brief Interface for Zoom SDK authorization and login services.
 */

#import <Cocoa/Cocoa.h>
#import <ZoomSDK/ZoomSDKErrors.h>
#import <ZoomSDK/ZoomSDKNotificationServiceController.h>

NS_ASSUME_NONNULL_BEGIN
/**
 * @class ZoomSDKAuthContext
 * @brief Context object containing authentication token information.
 */
@interface ZoomSDKAuthContext : NSObject

/**
 * @brief JWT token used for SDK authentication.
 */
@property(nonatomic, copy, nullable) NSString *jwtToken;

@end


/**
 * @class ZoomSDKWebinarRegistrationExplainInfo
 * @brief Holds explanatory information related to webinar registration.
 */
@interface ZoomSDKWebinarRegistrationExplainInfo : NSObject
/**
 * @brief content content of the webinar Registration Explain Info
 */
@property(nonatomic, copy, readonly, nullable) NSString *content;
/**
 * @brief accountOwnerLink accountOwnerLink of the webinar Registration Explain Info
 */
@property(nonatomic, copy, readonly, nullable) NSString *accountOwnerLink;
/**
 * @brief termLink termLink of the webinar Registration Explain Info
 */
@property(nonatomic, copy, readonly, nullable) NSString *termLink;
/**
 * @brief policyLink policyLink of the webinar Registration Explain Info
 */
@property(nonatomic, copy, readonly, nullable) NSString *policyLink;

@end


/**
 * @class ZoomSDKAccountInfo
 * @brief Provides information about the currently logged-in user account.
 */
@interface ZoomSDKAccountInfo : NSObject
/**
 * @brief Get user's screen name.
 * @return If the function succeeds, it will return user's screen name. 
 */
- (NSString*_Nullable) getDisplayName;
/**
 * @brief Get the type of user.
 * @return If the function succeeds, it will return the type of the user.
 */
- (ZoomSDKUserType) getSDKUserType;
@end



@protocol ZoomSDKAuthDelegate;
/**
 * @class ZoomSDKAuthService
 * @brief Provides APIs to authorize the Zoom SDK and manage login/logout.
 */
@interface ZoomSDKAuthService : NSObject
{
    id<ZoomSDKAuthDelegate> _delegate;
}
/**
 * @brief Delegate to receive auth and login/logout events.
 */
@property (assign, nonatomic, nullable) id<ZoomSDKAuthDelegate> delegate;

/**
 * @brief New authenticate SDK.
 * @param jwttoken A ZoomSDKAuthContext object containing auth information.
 * @return If the function succeeds, it will return @c ZoomSDKError_Success.
 * @note If the jwttoken expired, will return \link ZoomSDKAuthDelegate::onZoomAuthIdentityExpired \endlink callback.
 */
- (ZoomSDKError)sdkAuth:(ZoomSDKAuthContext*)jwttoken;

/**
 * @brief Determine if SDK is authorized.
 * @return YES means that it is authorized, otherwise failed.
 */
- (BOOL)isAuthorized;

/**
* @brief Generate SSO login web url.
* @param prefixOfVanityUrl The prefix of vanity url.
* @return If the function succeeds, will return url of can launch app.
*/
- (NSString*_Nullable)generateSSOLoginWebURL:(NSString*)prefixOfVanityUrl;

/**
 * @brief Login ZOOM with uri protocol.
 * @param uriProtocol For the parameter to be used for sso account login.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)SSOLoginWithWebUriProtocol:(NSString*)uriProtocol;
/**
 * @brief Logout ZOOM.
 * @return If the function succeeds, it will return ZoomSDKError_Success, meanwhile it will call asynchronously onZoomSDKLogout.
 */
- (ZoomSDKError)logout;

/**
 * @brief Get user's account information. 
 * @return When user logged in, it will return ZoomSDKAccountInfo object if the function calls successfully. Otherwise returns nil.
 */
- (ZoomSDKAccountInfo *_Nullable)getAccountInfo;

/**
 * @brief Get SDK identity.
 * @return The SDK identity.
 */
- (NSString*_Nullable)getSDKIdentity;

/**
 * @brief Enable or disable auto register notification service. This is enabled by default.
 * @param enable YES means enabled, otherwise not.
 */
- (void)enableAutoRegisterNotificationServiceForLogin:(BOOL)enable;

/**
 * @brief Register notification service.
 * @param accessToken Initialize parameter of notification service.
 * @return If the function succeeds, it will return @c ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)registerNotificationService:(NSString*)accessToken;

/**
 * @brief Unregister notification service.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)unregisterNotificationService;

/**
 * @brief Get notification service controller interface.
 * @return If the function succeeds, it will return a ZoomSDKZpnsServiceController object.
 */
- (ZoomSDKNotificationServiceController *)getNotificationServiceController;
@end


/**
 * @protocol ZoomSDKAuthDelegate
 * @brief Delegate protocol to receive SDK authorization and login/logout events.
 */
@protocol ZoomSDKAuthDelegate <NSObject>

@required
/**
 * @brief Specify to get the response of ZOOM SDK authorization. 
 * @param returnValue Notify user that the authentication status changes.
 *
 */
- (void)onZoomSDKAuthReturn:(ZoomSDKAuthError)returnValue;

/**
 * @brief Specify to get the response of ZOOM SDK authorization identity expired.
 */
 - (void)onZoomAuthIdentityExpired;

@optional
/**
 * @brief Specify to get the response of ZOOM SDK Login.
 * @param loginStatus Notify user of login status.
 * @param reason Notify user that the failed reason.
 */
- (void)onZoomSDKLoginResult:(ZoomSDKLoginStatus)loginStatus failReason:(ZoomSDKLoginFailReason)reason;
/**
 * @brief Specify to get the response of ZOOM SDK logout.
 */
- (void)onZoomSDKLogout;

/**
 * @brief Specify to get the response if ZOOM identity is expired.
 * @note User will be forced to logout once ZOOM SDK identity expired.
 */
- (void)onZoomIdentityExpired;

/**
 * @brief Notification service status changed callback.
 * @param status The value of transfer meeting service.
 * @param error Connection Notification service fail error code.
 */
- (void)onNotificationServiceStatus:(ZoomSDKNotificationServiceStatus)status error:(ZoomSDKNotificationServiceError)error;
@end
NS_ASSUME_NONNULL_END
