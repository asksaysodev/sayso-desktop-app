//
//  ZMZAppWebViewDefine.h
//  ZoomAppUISDK
//
//  Created by Jax Wu on 2023/12/4.
//

#ifndef ZMZAppWebViewDefine_h
#define ZMZAppWebViewDefine_h
#import <ZoomUnit/ZoomUnit.h>
#import <ZoomUnit/ZPZAppAdapter.h>
#import <zUnifyWebView/ZMUnifyWebViewProtocol.h>
#import <zUnifyWebView/ZMUnifyWebViewDefine.h>
#import <zUnifyWebView/ZMUnifyWebView.h>

extern NSUInteger const MAX_JS_PARAM_LENGTH; // ZOOM-273773
extern NSString *const kWebScriptHandlerName;
extern NSString *const kWebKitCookieSharedFolderPrefix;
extern NSString *const kWebKitCookiePTFolderPrefix;
extern NSString *const kWebKitCookieConfFolderPrefix;
extern NSString *const kWebKitCookieChatAppFolderPrefix;
extern NSString *const kWebKitCookiePhoneFolderPrefix;
extern NSString *const kWebKitCookieLabsFolderPrefix;
extern NSString *const kCallbackMethod;
extern NSString *const kCallbackKeyData;
extern NSString *const kWebViewEmptyURL;
extern NSString *const kWebViewSrcDocURL;
extern NSString *const kWebViewErrorURL;
extern NSString *const kErrorLog;
extern NSString *const kCSPErrorLog;
extern NSString *const kNavErrorLog;
extern NSString *const kConfIdSchemePrefix;

#define NSStringMultiline(...) [[NSString alloc] initWithCString:#__VA_ARGS__ encoding:NSUTF8StringEncoding]
#define CustomAppOnGetContextError ZM_GET_NSERROR(@"kAppOnGetContextError", 0, @"App context is invalid")
#define CustomAppOnDomainCheckError ZM_GET_NSERROR(@"kAppOnDomainCheckError", 0, @"App On Domain Check Error")

@protocol ZMAppWebViewInstance <ZMUnifyWebViewInstance>

/// Get the configed content size of self
- (NSSize)getUnifyWebConfiguredSize;

/// Get the appId related with self
- (NSString *)getInstanceAppId;

/// Modify appType
/// - Parameter appType: new appType
- (void)transitToNewAppType:(NSInteger)appType;

/// Get the app location with self
- (ZMAppLocation)getAppLocation;
@end



@class ZMRegisterJSApiParam;
@protocol ZMAppJSHandlerProtocol <NSObject>
- (void)onRegisterJSApiCalledWithReqId:(NSString *)reqId apiParam:(ZMRegisterJSApiParam *)params;
@end

typedef NS_ENUM(NSInteger, ZMAppMessageFunction) {
    ZMAppMessageFunction_None = 0,
    ZMAppMessageFunction_UploadDumpFile = 1,
    ZMAppMessageFunction_OpenApp = 2,
    ZMAppMessageFunction_CloseApp = 3,
    ZMAppMessageFunction_BulletReady = 4,
    ZMAppMessageFunction_MousePenetrate = 5,
    ZMAppMessageFunction_JumpMessage = 6,
    ZMAppMessageFunction_ViewApp = 7,
};

typedef NS_ENUM(NSInteger, ZMAppMessageEvent) {
    ZMAppMessageEvent_None = 0,
    ZMAppMessageEvent_OnDumpFileUploaded = 1,
    ZMAppMessageEvent_RecieveMessageFromOtherApp = 2,
    ZMAppMessageEvent_OpenApp = 3,
    ZMAppMessageEvent_CloseApp = 4,
};

@protocol ZMAppMessageHandlerProtocol <NSObject>
- (void)onAppMessageRecevied:(NSString *)jscallId funcName:(ZMAppMessageFunction)funcName argsJason:(NSString *)args webViewId:(NSString *)webViewId;
@end

@interface ZMRegisterJSApiParam : NSObject

@property (nonatomic, copy) NSString *appId;
@property (nonatomic, copy) NSString *appName;
@property (nonatomic, copy) NSString *currentURL;
@property (nonatomic, copy) NSString *webViewId;
@property (nonatomic, assign) ZMAppType appType;
@property (nonatomic, copy) NSString *jsCallId;
@property (nonatomic, copy) NSString *moduleName;
@property (nonatomic, copy) NSString *funcName;
@property (nonatomic, copy) NSString *argsJson;
@property (nonatomic, assign) BOOL isAppConnectedAfterMeeting;
@property (nonatomic, assign) BOOL bPopup;
@end

@interface ZMRegisterJSApiResult : NSObject

@property (nonatomic, assign) NSInteger errorCode;
@property (nonatomic, copy) NSString *resultData;
@property (nonatomic, copy) NSString *errorMsg;

@end

typedef void (^ZAppURLSchemePreCompletion)(NSURLRequest *processedRequest);

@interface ZMZAppWebViewConfiguration : ZMUnifyWebViewConfiguration

@property (nonatomic, assign) ZMAppType appType;
@property (nonatomic, copy) NSString *appId;
@property (nonatomic, assign) BOOL sharedStorage;
@property (nonatomic, assign) BOOL isLauncherApp;
@property (nonatomic, assign) BOOL usedForAuthorize;
@property (nonatomic, assign) ZMAppLocation appLocation;
@property (nonatomic, assign) BOOL isLocalApp;
@property (nonatomic, copy)  NSData* (^localAppCustomURLHandler)(NSURLRequest *request);
@property (nonatomic, copy)  void (^localAppRequsetPreprocessing)(NSURLRequest *request, ZAppURLSchemePreCompletion completionBlock);

- (BOOL)isSameDataStoreIdentify:(ZMZAppWebViewConfiguration *)config;

- (BOOL)isZoomApps;
- (BOOL)isThirdPartApps;
- (BOOL)isInternalApps;
- (BOOL)isSideCar;
- (BOOL)isOnZoom;
- (BOOL)isVCard;
- (BOOL)isActivivtyCenter;
- (BOOL)isSearchApp;
- (BOOL)isSpotsApp;
- (BOOL)isAICApp;

@end

typedef NS_ENUM(NSInteger, ZMZAppRefreshType) {
    ZMZAppRefreshType_Default = 0, // Current Page
    ZMZAppRefreshType_Home,
    ZMZAppRefreshType_AppDataOnly,
};

typedef NS_ENUM(NSInteger, ZMZAppOpenLauncherPurpose) {
    ZMZAppOpenLauncherPurpose_Default = 0,
    ZMZAppOpenLauncherPurpose_AppDetailPage,
    ZMZAppOpenLauncherPurpose_DiscoverMoreApps,
    ZMZAppOpenLauncherPurpose_ZappPopover,
    ZMZAppOpenLauncherPurpose_InClientOAuth,
    ZMZAppOpenLauncherPurpose_ChatChannel,
};


typedef NS_ENUM(NSInteger, ZMZAppProductTokenExpiredType) {
    ZMZAppProductTokenExpiredType_Lobby = 0,
    ZMZAppProductTokenExpiredType_ZoomRoom,
};

typedef NS_ENUM(NSInteger, ZMZAppChatAppAction) {
    ZMZAppChatAppAction_Unknown = 0,
    ZMZAppChatAppAction_RefreshPage,
};

typedef NS_ENUM(NSInteger, ZMZAppOpenInvitationState) {
    ZMZAppOpenInvitationState_ShowDescriptionPage = 0,
    ZMZAppOpenInvitationState_ShowHomePage,
};

@interface ZMAppOpenLauncherParam : NSObject

@property(nonatomic, assign) BOOL isNeedNavigate;
@property(nonatomic, copy) NSString *appID;
@property(nonatomic, copy) NSString *inClientOauthUrl;
@property(nonatomic, copy) NSString *channelId;
@property(nonatomic, assign) ZMZAppOpenLauncherPurpose purpose;

@end

@interface ZMAppOpenAppResultExtraInfo: NSObject

@property(nonatomic, copy) NSString *installUrl;
@property(nonatomic, copy) NSString *invitationUUID;
@property(nonatomic, copy) NSString *launchTrackingId;
@property(nonatomic, assign) BOOL isDraftApp;
@property(nonatomic, assign) BOOL isDisabled;
@property(nonatomic, assign) BOOL isBetaApp;
@property(nonatomic, assign) ZAppOpenAppPurpose openPurpose;
@property(nonatomic, assign) ZAppOpenAppMode launchMode;

@property(nonatomic, assign) ZAppPreapprovalState approvalState;
@property(nonatomic, assign) ZMZAppOpenInvitationState state;

@end

@interface ZMAppOpenAppResult: NSObject

@property(nonatomic, copy) NSString *appID;
@property(nonatomic, copy) NSString *appName;
@property(nonatomic, copy) NSString *trackingID;
@property(nonatomic, assign) BOOL bSuccess;
@property(nonatomic, assign) NSInteger zAppErrorCode;
@property(nonatomic, assign) NSInteger httpRetCode;
@property(nonatomic, strong) ZMAppOpenAppResultExtraInfo *extraInfo;
@property(nonatomic, assign) BOOL localLoadFailed;
@property(nonatomic, assign) BOOL canOpenApp;
@property(nonatomic, copy) NSString *marketplaceErrorMsg;

@end

@interface ZMAppOpenLauncherResult: NSObject

@property(nonatomic, assign) BOOL bSuccess;
@property(nonatomic, assign) BOOL localLoadFailed;
@property(nonatomic, strong) ZMAppOpenLauncherParam *launcerParam;

@end

@interface ZMAppRequestAuthInfo: NSObject
@property(nonatomic, strong) NSString *appId;
@property(nonatomic, strong) NSString *installUrl;
@end

@interface ZMAppAuthResult: NSObject
@property(nonatomic, assign) BOOL result;
@property(nonatomic, strong) NSString *appId;
@property(nonatomic, strong) NSString *authWebviewId;
@end

@protocol ZMUnifyAppInstance;

@protocol ZMUnifyAppInstanceDelegate <NSObject>

@optional
- (BOOL)isAllowNavigate:(id<ZMUnifyAppInstance>)appInstance urlString:(NSString *)urlString;
- (NSDictionary *)getExtraRequestHeader:(id<ZMUnifyAppInstance>)appInstance;
- (void)onAppOpen:(id<ZMUnifyAppInstance>)appInstance result:(ZMAppOpenAppResult *)result;
- (void)onLauncherOpen:(id<ZMUnifyAppInstance>)appInstance result:(ZMAppOpenLauncherResult *)result;

- (void)onProductTokenExpired:(id<ZMUnifyAppInstance>)appInstance type:(ZMZAppProductTokenExpiredType)type;
- (void)onChatAppStatusChange:(id<ZMUnifyAppInstance>)appInstance appID:(NSString *)appID action:(ZMZAppChatAppAction)action;
- (BOOL)shouldShowAppOpenErrorView:(id<ZMUnifyAppInstance>)appInstance result:(ZMAppOpenAppResult *)result;

- (void)onJ2cSendAppToBackground:(id<ZMUnifyAppInstance>)appInstance;
- (void)onJ2cSendAppToForeground:(id<ZMUnifyAppInstance>)appInstance;

- (NSString*)onAppRequestAuth:(id<ZMUnifyAppInstance>)appInstance authInfo:(ZMAppRequestAuthInfo*)authInfo;
- (void)onAppAuthDone:(id<ZMUnifyAppInstance>)appInstance authResult:(ZMAppAuthResult*)authResult;

@end

@protocol ZMUnifyAppInstance <ZMAppWebViewInstance>

- (void)configAppDelegate:(id<ZMUnifyAppInstanceDelegate>)appDelegate;
- (void)openApp:(ZMAppContextParam *)extraParams;
- (void)openAppLauncher:(ZMAppOpenLauncherParam *)params;
- (void)refreshApp:(ZMZAppRefreshType)type;
- (NSString *)registerFilePathId:(NSString*)filePath intention:(int)intention;
- (void *)getZappSink;

@end

#endif /* ZMZAppWebViewDefine_h */
