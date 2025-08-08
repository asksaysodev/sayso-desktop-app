//
//  ZMWebViewProtocol.h
//  zChatComponent
//
//  Created by Jed Zheng on 11/8/2023.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@protocol ZMWebViewProtocol <NSObject>
@optional
+ (instancetype)shared;
- (void)releaseInstance;
- (NSArray *)getRegisteredFunctionNames;
- (void)oldFunctionEvaluateJavaScript:(NSString *)javaScriptString jsCallId:(NSString *)jsCallId;
- (void)oldFunctionEvaluateJavaScript:(NSString *)javaScriptString resourceType:(NSInteger)resourceType;
- (void)handleJsMessageWithParam:(NSDictionary *)param sourceType:(NSInteger)type;
- (NSString *)getResourceStringWithResourceType:(NSInteger)resourceType;
- (void)showUserProfilePannel:(NSString *)uid positioningView:(NSView *)positioningView relativeRect:(NSRect)rect autoClose:(BOOL)autoClose;
- (void)showToast:(id)toast_String_or_AttributedString autoDisappear:(BOOL)autoDisappear type:(NSInteger)type;


- (void)openActivityCenterSettingWindow;
- (void)closeActivityCenterSettingWindow;
- (void)closeActivityCenterWindow;
- (void)closeActivityCenterWithSourceType:(NSInteger)sourceType webView:(NSView *)webView;

- (NSString *)parseZmdfPathWithUrlPath:(NSString *)urlPath;


- (void)onWebViewInstalled;
- (BOOL)isClientLogined;
- (void)switchToLoginWindow;
- (void)closeMTVcardWindow;
- (NSDictionary *)fetchUserInfoWithNoLoginState:(NSString *)sessionId;
- (NSDictionary *)fetchMeetingInfo;

- (BOOL)isZappModuleLoaded;
@end

NS_ASSUME_NONNULL_END
