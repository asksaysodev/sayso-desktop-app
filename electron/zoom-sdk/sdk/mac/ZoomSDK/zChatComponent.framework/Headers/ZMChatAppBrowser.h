//
//  ZMChatAppBrowser.h
//  ChatUI
//
//  Created by Zoro.Fu on 2022/8/24.
//

#include <zChatApp/IZoomMessenger.h>

NS_ASSUME_NONNULL_BEGIN

#define ZMZappOpenFromMessage @"shortcutInMessage"
#define ZMZappOpenFromCompose @"shortcutInCompose"
#define ZMZappOpenFromBotMessage @"actionInBotMessage"
#define ZMZappOpenFromSlashCommand @"slashCommand"
#define ZMZappOpenFromDocumentPreview @"documentPreview"
#define ZMZappOpenFromThread @"shortcutInThread"
#define ZMZappOpenFromHoverLinkInMessage @"hoverLinkInMessage"
#define ZMZappOpenFromHoverLinkInCompose @"hoverLinkInCompose"
#define ZMZappOpenFromChatTab @"chatTab"
#define ZMZappOpenFromChatTabPicker @"chatTabPicker"

typedef NS_ENUM(NSUInteger, ZMChatAppCloseReason) {
    ZMChatAppClose_Default,
    ZMChatAppClose_Auto,
    ZMChatAppClose_Force
};

typedef NS_ENUM(NSUInteger, ZMChatAppShortcutSessionType) {
    ZMChatAppShortcutSession_DM = 11,
    ZMChatAppShortcutSession_Channel = 12
};

@class  ZMChatAppContext, ZMChatAppViewController;

#ifdef ENABLE_ZAPP
@class ZMZAppWebViewService;
#endif

@protocol ZMChatAppActions <NSObject>

#ifdef ENABLE_ZAPP
- (ZMZAppWebViewService *)zAppWebViewService;
#endif

- (ZMChatAppViewController *_Nullable)openApp:(ZMChatAppContext *)appContext;
- (void)openFloatApp:(ZMChatAppContext *)appContext;
- (void)closeFloatApp;
- (void)forceCloseFloatApp;

/// Should customize and handle the sourceType in ZMChatAppContext to close a specific floating app. Do not use it immediately.
- (void)forceCloseFloatApp:(ZMChatAppContext *)appContext;
- (BOOL)isFloatAppShowing;

- (void)openMenuForWebView:(NSString *)webViewId menuConfig:(NSDictionary *)config;

- (void)updateContext:(NSDictionary *)context webViewId:(NSString *)webViewId;

@end

@protocol ZMChatAppEvent <NSObject>

//- (void)onCloseApp:(NSString * _Nullable)reason;

- (NSString *_Nullable)getZappChannelLauncherUrlWithChannelId:(NSString *)sessionId;

- (void)updateAppViewController:(ZMChatAppViewController *)vc webViewId:(NSString *)webViewId;
- (void)removeAppViewControllerWithWebViewId:(NSString *)webViewId;

@end

@protocol ZMChatAppJSEvent <NSObject>

#ifdef ENABLE_ZAPP
- (void *)zappJSRegister;
- (void *)zappServiceHelper;
#endif

- (BOOL)checkIsAppOpenFromWebChat:(NSString *)webViewId;

- (BOOL)webChatHandledAPI:(NSString *)funcName;

- (void)webView:(NSString *)webViewId sendJsApiToWebChat:(NSDictionary *)data;

- (BOOL)checkActiveAppWithAppId:(NSString *)appId webViewId:(NSString *)webViewId;

- (NSDictionary *)getChatContextWithWebViewId:(NSString *)webViewId;

- (NSString *)getChatMessageWithSessionId:(NSString *)sessionId threadId:(NSString *)threadId messageId:(NSString *)messageId;

- (void)closeModal:(NSString *)webViewId;

- (BOOL)webView:(NSString *)webViewId editCompose:(NSString *)message action:(NSString *)action range:(NSRange)range;

- (void)webView:(NSString *)webViewId appId:(NSString *)appId appendCardToCompose:(const CAString &)previewJson;

- (void)showAppList;

- (void)webView:(NSString *)webViewId sizeDidChanged:(NSSize)size;

- (void)setConfirmationOnDiscard:(BOOL)needConfirmation;

- (void)webView:(NSString *)webViewId setCloseButtonHidden:(BOOL)isHidden;

- (void)webView:(NSString *)webViewId shareDocs:(NSDictionary *)docsInfo;

@end

@interface ZMChatAppBrowser : NSObject

@property (class, readonly) id<ZMChatAppActions> shared;

@property (class, readonly) id<ZMChatAppEvent> eventShared;

@property (class, readonly) id<ZMChatAppJSEvent> jsEventShared;

@end

NS_ASSUME_NONNULL_END
