//
//  ZMWebViewDispatchMgr.h
//  ChatUIEx
//
//  Created by Jed Zheng on 28/11/2022.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMWebViewDispatchMgr : NSObject
- (void)addWebView:(NSView *)webView API_AVAILABLE(macos(10.10));
- (void)removeWebView:(NSView *)webView API_AVAILABLE(macos(10.10));
+ (ZMWebViewDispatchMgr *)sharedInstance;
- (void)evaluateJavaScript:(NSString *)javaScriptString webView:(NSView *)webView;
- (void)evaluateJavaScript:(NSString *)javaScriptString resourceType:(NSInteger)resourceType;
- (void)evaluateJavaScriptFromSDK:(NSString *)javaScriptString jsCallId:(NSString *)jsCallId;
- (void)postNativeMessageToWebWithCallId:(nullable NSString *)callId resourceTypes:(nullable NSArray *)resourceTypes jsonMsg:(NSString *)jsonMsg;

- (void)registerJSApiHandler;
@end

NS_ASSUME_NONNULL_END
