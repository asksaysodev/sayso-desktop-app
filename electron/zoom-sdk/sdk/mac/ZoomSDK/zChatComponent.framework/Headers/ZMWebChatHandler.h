//
//  ZMWebChatHandler.h
//  zChatComponent
//
//  Created by Zoro.Fu on 2025/3/12.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMChatAppContext;

@interface ZMWebChatHandler : NSObject

+ (instancetype)sharedInstance;

- (void)setWebChatViewId:(NSString *)webViewId;

- (BOOL)openChatAppWithChatAppContext:(ZMChatAppContext *)context;

- (void)receiveDLEv_RecvNotification:(NSDictionary *)payload;

- (void)sendDataToWebChat:(NSString *)payload;

@end

NS_ASSUME_NONNULL_END
