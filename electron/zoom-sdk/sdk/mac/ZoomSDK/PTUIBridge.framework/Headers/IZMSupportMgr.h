//
//  IZMSupportMgr.h
//  PTUIBridge
//
//  Created by davies zhao on 2025/5/16.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@protocol IZMSupportMgr <NSObject>
- (NSString*)getWebChatBrowserUrl;
- (void)getJWT:(nullable void (^)(NSString *token))completionHandler;
- (void)loadWebChatPage:(NSString*)url;
- (BOOL)showSupportWebChatHelpIfNeeded;

@end

NS_ASSUME_NONNULL_END
