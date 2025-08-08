//
//  IZMLoginMgr.h
//  ChatUI
//
//  Created by davies zhao on 2023/5/5.
//  Copyright © 2023 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>
#ifdef __cplusplus
#import <zPTApp/SaasBeePTAppInterface.h>
#endif
#import <ZoomUnit/ZoomUnit-Swift.h>

NS_ASSUME_NONNULL_BEGIN

#define kZMLogin_SignUp_Source_SignInPage @"sign_in_page"
#define kZMLogin_SignUp_Source_WelcomePage @"welcome_page"

typedef NS_ENUM(NSUInteger, EnforceLoginErrorType) {
    Enforce_Login_Error_Type_Normal = 0,
    Enforce_Login_Error_Type_To_Government,
    Enforce_Login_Error_Type_To_Commercial,
};

@class ZMThirdPartLoginModel;
@class ZMDeepLinkParams, ZMToastCoordinator;

@protocol IZMLoginMgr <ZMRoutableObject>

- (void)initModule;
//window
@property (nullable, strong, readonly) NSWindow *window;
- (BOOL)isWindowLoaded;
- (void)showWindow:(nullable id)sender;
- (void)showWindowAndSwitchToLoginPage;
- (void)showWindowAlertView:(nullable NSView *)view;
- (void)close;

@property (assign, readonly) BOOL isLogin;
- (BOOL)isInLoadingStatus;
- (BOOL)isInLoginProgress;

@property (nonatomic, strong) ZMToastCoordinator *toastCoordinator;
@property (nonatomic, assign) EnforceLoginErrorType showGovInfo;
@property (nonatomic, assign) BOOL isSignedUpLoginUser;

- (void)switchToLoginMethodsTab;
- (void)switchToSignUpTabWithSource:(nullable NSString *)source;
- (void)switchToLoginTabForZapp;
- (void)switchToBirthdayTab;

- (void)switchVanityUrl;

- (void)joinMeeting;

- (void)showSSOErrorMessage:(nullable NSString *)string;

- (void)onMFARequestReturnWithResult:(NSInteger)result;

- (void)showRestrictedLoginDomainAlert;
- (void)showWebStartMeetingRestrictedDomainAlert;

- (nullable NSString*)makeSSODomain:(nullable NSString*)domain;
- (void)openSSOLoginWithBaseUrl:(nullable NSString *)baseUrl;//ZOOM-87404
- (void)openSSOUrl:(nullable NSURL *)inUrl;
//ZOOM-68190
- (void)showLoginUIWithSnsType:(NSInteger)snsType userName:(nullable NSString *)userName checkLoginStatue:(BOOL)checkLoginStatue;

- (void)resetAutoLogoutType:(BOOL)needDelay;
- (void)authSuccessfulWithInfo:(nullable NSDictionary *)userInfo;
- (void)fbIMReconnecting;
- (void)performLoginAction;

- (void)didLogin;
- (void)webLoginNotification:(NSInteger)resultValue;
- (void)imLoginFinishedWithResult:(NSInteger)paramInt;
- (void)logOutFinished:(int)logoutType;
- (void)callStatusChange:(int)callStatusIntValue;
- (void)IPCWebStartAutoLogin;
- (void)IPCClaimHost:(int)snsType;
- (void)onGoogleWebAccessFail;
- (nullable NSURL *)generateSSOUrlWithPrefix:(nullable NSString *)prefixString;
- (void)onConnectRetrurn:(NSInteger)err;
- (void)onOTPRequestReturnWithResult:(NSInteger)result;


- (void)loginPKCEFaceBook:(nullable NSString*)encryptedTokenStr code_verifier:(nullable NSString *)codeVerifier expiresIn:(NSInteger)expires_in errorCode:(NSInteger)error errorString:(nullable NSString*)errStr;
- (void)sinkLoginPKCEGoogle:(nullable NSString*)encryptedTokenStr codeVerifier:(nullable NSString*)codeVerifier errorCode:(NSInteger)error errorString:(nullable NSString*)errStr;
- (void)sinkLoginPKCESSO:(nullable NSString*)encryptTokenStr codeVerifier:(nullable NSString *)codeVerifier;
- (void)showSignToJoinOptionWindowWithLinkUrl:(nullable NSString *)detailLinkUrl couldSkip:(BOOL)couldSkip accountName:(nullable NSString *)accountNameString emailDomain:(nullable NSString *)emailDomainString;//ZOOM-114845
- (void)showLoginDialogWithPrefillEmail:(nullable NSString *)prefillEmail;//ZOOM-114845
- (void)sinkLoginWithOAuth:(NSInteger)snsType accessToken:(nullable NSString *)accessToken expiresCode:(NSInteger)expiresCode errorCode:(NSInteger)errorCode errorStr:(nullable NSString *)errorStr openId:(nullable NSString *)openId idtkToken:(nullable NSString *)idtkToken zsToken:(nullable NSString *)zsToken appId:(nullable NSString *)appId;
- (void)webLaunchedToLoginWithType:(NSInteger)loginType ssoUrl:(nullable NSString *)ssoUrl ssoPrefix:(nullable NSString *)ssoPrefix ssoPostfix:(nullable NSString *)ssoPostfix domain:(nullable NSString *)domain;
- (void)onShowLoginDisclaimerDialogWithLanguage:(nullable NSString *)language title:(nullable NSString *)title description:(nullable NSString *)description linkUrl:(nullable NSString *)linkUrl linkText:(nullable NSString *)linkText;
- (void)onShowPasswordExpiredDialog:(nullable NSString *)passwordUrl;
- (void)showOTPChangePasswordAlertView:(nullable NSString *)url;
- (void)onOtpNotificationConfirmResponse:(NSInteger)result;

- (void)showReportProblemWindow;
- (void)resetReportProblemEmailString:(nullable NSString *)tipString;

//sign out
- (void)signOutWithSwichAccount:(BOOL)isSwitchAccount;
- (void)signOutWithSwichAccount:(BOOL)isSwitchAccount isForced:(BOOL)isForced shouldCheckClips:(BOOL)shouldCheckClips;


- (void)onReachabilityFlagsChanged:(BOOL)reachable ipAddress:(NSString*)ip;

#ifdef __cplusplus
- (void)showMFAWithData:(const NS_ZWEBSERVICE::MultiFactorAuth&)mfa;
- (void)showIDPLoadingView:(ISBPTAppSink::GetSecurityParamsStatus)status;
- (void)notifyIDPSSOAction:(SB_webservice::IDPSSOActionParam)param;
- (void)showOTPLoginAlertView:(const SB_webservice::OtpNotificationInfo) info;
#endif
- (void)onRequestInitialConfigReady:(BOOL)success;
@end

@protocol ZMLoginEventProtocol <NSObject>
@optional
- (void)loginTabView:(NSTabView *)tabView didSelectTabViewItem:(NSTabViewItem *)tabViewItem;
- (void)workspaceListUpdated;
- (void)meetingStatusChange:(int)callStatusIntValue;

@end

NS_ASSUME_NONNULL_END
