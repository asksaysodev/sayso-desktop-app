//
//  ZMZAppWebViewService.h
//  ZoomAppUISDK
//
//  Created by Jax Wu on 2023/12/18.
//

#import <Foundation/Foundation.h>
#import "ZMZAppWebViewDefine.h"

NS_ASSUME_NONNULL_BEGIN

@class ZMAppContextParam;
@class ZMZappContext;

@interface ZMZAppWebViewService : NSObject

+ (ZMZAppWebViewService *)shareInstance;

- (id<ZMAppWebViewInstance>)createZAppUnifyWebView:(ZMZAppWebViewConfiguration *)config layoutBlock:(void (^)(NSView *view))layoutBlock;
- (id<ZMAppWebViewInstance>)createZAppUnifyWebView:(ZMZAppWebViewConfiguration *)appConfig config:(WKWebViewConfiguration *)config layoutBlock:(void (^)(NSView *))layoutBlock;
- (id<ZMAppWebViewInstance>)zAppUnifyWebViewWith:(NSString *)webViewID;

- (BOOL)isExistSameConfigZAppUnifyWebView:(ZMZAppWebViewConfiguration *)config;

- (void)destroyZAppWebView:(id<ZMUnifyWebViewInstance>)unifyWebInstance;

- (BOOL)isNeedCheckWebResource:(NSString *)appId webViewId:(NSString *)webViewId appType:(ZMAppType)appType;

- (NSString *)launcherAppId:(ZMAppType)appType;

- (BOOL)registerMessageHandler:(ZMAppMessageFunction)func handler:(id<ZMAppMessageHandlerProtocol>)handler;

- (void)notifyAppMessage:(ZMAppMessageEvent)event argsJson:(NSString *)args;

- (void)unRegisterMessageHandler:(id<ZMAppMessageHandlerProtocol>)handler;

- (void)refreshApp:(ZMZAppRefreshType)type webViewID:(NSString *)webViewID;

#pragma mark - ZApp
- (void)getAppContextWithAppId:(NSString *)appId
                       appType:(ZMAppType)appType
             isForceRemoteLoad:(BOOL)isForceRemoteLoad
                   extraParams:(ZMAppContextParam *)extraParams
             completionHandler:(void (^)(ZMZappContext *context, NSError *error))completion;

/// Get  third-part app context  with specific appId and appType
/// @param appId app id
/// @param appType app type
/// @param extraParams extra params for sidecar, chatapp
/// @param completion completion handler with context or error
/// values in ZMAppContextParam
/// Zoom Apps: openPurpose
/// sidecar: targetURL, meetingNumber, openPurpose
/// chatApp: openSrc, sessionId, messageId, threadId, actionId
- (void)getAppContextWithAppId:(NSString *)appId
                       appType:(ZMAppType)appType
                   extraParams:(ZMAppContextParam *)extraParams
             completionHandler:(void (^)(ZMZappContext *context, NSError *error))completion;

/// Get zoom launcher app context
/// @param appType app type
/// @param needHeader whether u want header from the context
/// @param completion completion handler with context or error
- (ZMZappContext *)getZappLauncherContext:(ZMAppType)appType
                               needHeader:(BOOL)needHeader
                        completionHandler:(void (^)(ZMZappContext *context, NSError *error))completion;

/// Determine whether extraParams can create a webview
/// @param appId appId
/// @param appType app type
/// @param extraParams extra params
/// @param completion It will be called in the OnPreOpenApp callback
- (void)preOpenAppWithAppId:(NSString *)appId
                    appType:(ZMAppType)appType
                extraParams:(ZMAppContextParam *)extraParams completionHandler:(id<ZMUnifyAppInstance> _Nullable (^)(ZMAppOpenAppResult *result))completion;

/// Add content rule for webview to check whether resources(js/css/img) should be loaded
/// @param webView the webView is the superView parameter in the `createWebViewWithConfiguration:delegate:superView:` method
/// @param completeHandler completion handler after content rule added
- (void)addContentRuleForWebView:(id<ZMUnifyWebViewInstance>)unifyWebInstance completionHandler:(void (^)(void))completeHandler;

/// Add content rule for webview whose url is localhost to check whether resources(js/css/img) should be loaded
/// @param webView the webView is the superView parameter in the `createWebViewWithConfiguration:delegate:superView:` method
/// @param completeHandler completion handler after content rule added
- (void)addContentRuleListForLocalHostWebView:(id<ZMUnifyWebViewInstance>)unifyWebInstance completionHandler:(void (^)(void))completeHandler;

#pragma mark - Customize JS APIs
- (void)registerJSApiHandlerWithModuleName:(NSString *)moduleName apiName:(NSString *)apiName handler:(id<ZMAppJSHandlerProtocol>)handler;
- (void)unregisterJSApiHandler:(id<ZMAppJSHandlerProtocol>)handler;
- (void)setRegisterJSApiResultWithReqId:(NSString *)reqId result:(ZMRegisterJSApiResult *)result;
@end

NS_ASSUME_NONNULL_END
