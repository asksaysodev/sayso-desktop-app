//
//  ZMBaseWKWebViewController.h
//  zChatUI
//
//  Created by Jed Zheng on 24/4/2023.
//  Copyright © 2023 Zoom. All rights reserved.
//

#import <ZoomKit/ZoomKit.h>
#ifdef ENABLE_ZAPP
#import <zUnifyWebView/ZMUnifyWebURLSchemeHelper.h>
#import <ZoomAppUISDK/ZoomAppUISDK.h>
#import <ZoomUnit/ZPZAppAdapter.h>
#endif

typedef NS_ENUM(NSUInteger, ZMWebViewErrorType) {
    ZMWebViewErrorTypeLoadError,
    ZMWebViewErrorTypeOtherError
};

NS_ASSUME_NONNULL_BEGIN

@interface ZMBaseWKWebViewController : ZMBaseViewController
@property(nonatomic, assign, readonly) ZMAppType appType;
@property (nonatomic, assign) BOOL isRemoteUrl;
@property (nonatomic, assign) BOOL isRemoteLoaded;

#ifdef ENABLE_ZAPP
@property(nonatomic, readonly)id<ZMUnifyWebViewInstance> zAppWebView;
#endif

- (instancetype)initWithAppType:(ZMAppType)appType;
- (void)loadWebViewData;
- (void)starLoadingIndicator;

- (void)stopLoadingIndicatorNeedShowError:(BOOL)showError;
- (void)stopLoadingIndicatorNeedShowError:(BOOL)showError errorType:(ZMWebViewErrorType)errorType;
- (void)makeWebViewFirstResponderIfPossible;

- (void)webResourceLoadStart;
- (void)webResourceLoadFinish;

#ifdef ENABLE_ZAPP
- (ZMZAppWebViewConfiguration *)webViewCustomConfiguration;
- (ZMZAppWebViewService *)zAppWebViewService;
- (ZMUnifyWebURLSchemeHelper *)unifyWebURLSchemeHelper;
- (NSString *)processDisplayName;
#endif

@end

NS_ASSUME_NONNULL_END
