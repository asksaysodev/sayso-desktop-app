//
//  ZMUnifyAppService.h
//  ZoomAppUISDK
//
//  Created by Jax Wu on 2024/4/18.
//

#import <Cocoa/Cocoa.h>
#import "ZMZAppWebViewDefine.h"

NS_ASSUME_NONNULL_BEGIN

@class ZMAppContextParam;
@class ZMZappContext;

@interface ZMUnifyAppService : NSObject

+ (ZMUnifyAppService *)shareInstance;

- (id<ZMUnifyAppInstance>)createUnifyZApp:(ZMZAppWebViewConfiguration *)config layoutBlock:(void (^)(NSView *view))layoutBlock;
- (id<ZMUnifyAppInstance>)createUnifyZApp:(ZMZAppWebViewConfiguration *)appConfig config:(WKWebViewConfiguration *)config layoutBlock:(void (^)(NSView *))layoutBlock;
- (id<ZMUnifyAppInstance>)unifyZAppWith:(NSString *)webViewID;

- (BOOL)isExistSameConfigUnifyZApp:(ZMZAppWebViewConfiguration *)config;

- (BOOL)canLoadAppFromLocal:(NSInteger)appIndex appType:(ZMAppType)appType;

- (BOOL)canLoadAppFromLocalByAppId:(NSString*)appId appType:(ZMAppType)appType;

- (void)destroyUnifyZApp:(id<ZMUnifyAppInstance>)unifyWebInstance;

@end

NS_ASSUME_NONNULL_END
