//
//  ZMChatAppWebViewController.h
//  ChatUI
//
//  Created by Zoro.Fu on 2022/8/24.
//

#import <Cocoa/Cocoa.h>
#ifdef ENABLE_ZAPP
#import <ZoomAppUISDK/ZoomAppUISDK.h>
#endif

@class ZMChatAppShortcutInfo, ZMZappContext, ZMChatAppWebViewController, ZMAppContextParam;
NS_ASSUME_NONNULL_BEGIN

@protocol ZMChatAppWebViewDataSource<NSObject>

- (BOOL)isLocalApp:(NSString *)appId;

@optional

- (NSString *)appHomeUrl;

- (ZMAppContextParam *)appContextParam;
- (ZMAppOpenLauncherParam *)appOpenLauncherParam;

- (NSDictionary *)extraRequestHeader;

- (ZMZAppWebViewConfiguration *)configurationForForceRemoteLoad;

@end

@protocol ZMChatAppWebViewDelegate<NSObject>

@optional
- (void)appWebViewController:(ZMChatAppWebViewController *)vc didFinishNavigationWithURL:(NSURL *)url;
- (void)appWebViewController:(ZMChatAppWebViewController *)vc didFailedNavigationWithURL:(NSURL *)url error:(NSError *)error;
- (void)appWebViewController:(ZMChatAppWebViewController *)vc loadingStatusDidChanged:(BOOL)isLoading;
- (void)appWebViewController:(ZMChatAppWebViewController *)vc webViewDidTerminateWithReason:(NSInteger)reason;

- (void)appWebViewController:(ZMChatAppWebViewController *)vc didDestroyWebView:(NSString *)webViewId;
- (void)appWebViewController:(ZMChatAppWebViewController *)vc didCreateWebView:(NSString *)webViewId;

@end

@interface ZMChatAppWebViewController : ZMBaseViewController

@property (nonatomic, weak) id<ZMChatAppWebViewDataSource> dataSource;
@property (nonatomic, weak) id<ZMChatAppWebViewDelegate> delegate;
#ifdef ENABLE_ZAPP
@property (nonatomic, assign) ZMOpenChatAppSrc openSrc;
@property (nonatomic, assign) ZMAppLocation appLocation;
@property(nonatomic, strong) NSView<ZMAppWebViewInstance> *zAppWebView;
#endif
/// appType: ZMAppType defined in ZoomAppUISDK
- (instancetype)initWithChatAppId:(NSString *)appId appType:(NSInteger)appType dataSource:(id<ZMChatAppWebViewDataSource>)dataSource delegate:(id<ZMChatAppWebViewDelegate>)delegate;

#ifdef ENABLE_ZAPP
- (instancetype)initWithWebViewConfiguration:(ZMZAppWebViewConfiguration *)config dataSource:(nonnull id<ZMChatAppWebViewDataSource>)dataSource;
- (void)cleanZAppWebView;
#endif

- (instancetype)initWithChatAppId:(NSString *)appId
                          appType:(NSInteger)appType
                    isLauncherApp:(BOOL)isLauncherApp
                       dataSource:(id<ZMChatAppWebViewDataSource>)dataSource
                         delegate:(id<ZMChatAppWebViewDelegate>)delegate;

- (NSString *)registerFilePathId:(NSString*)filePath intention:(int)intention;

- (void)startLoad;

- (nullable NSString *)webViewId;

@end

NS_ASSUME_NONNULL_END
