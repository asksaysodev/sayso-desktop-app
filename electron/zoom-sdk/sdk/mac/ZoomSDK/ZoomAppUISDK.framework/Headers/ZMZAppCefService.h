//
//  ZMZAppCefService.h
//  ZoomAppUISDK
//
//  Created by Neko Chen on 2023/12/29.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef void (^ZMZAppCefAgentReceiveHandler)(NSInteger width, NSInteger height);

@interface ZMZAppCefService : NSObject

+ (ZMZAppCefService *)shareInstance;

- (BOOL)launchCefAgentWithParams:(NSDictionary *)params cefPath:(NSString *)cefPath receiveHandler:(ZMZAppCefAgentReceiveHandler)receiveHandler;
- (NSString *)getCEFCurrentURL;
- (void)forceUpdateCefScreenHelper;
- (void)forceKillCefAgent;
- (void)killCefAgent;

@end

NS_ASSUME_NONNULL_END
