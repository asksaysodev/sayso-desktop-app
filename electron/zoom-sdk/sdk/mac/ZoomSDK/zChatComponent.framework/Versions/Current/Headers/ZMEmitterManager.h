
#import <Foundation/Foundation.h>
#import <zChatComponent/ZMEmitterManagerProtocol.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMEmitterManager : NSObject <ZMEmitterManagerProtocol>

+ (instancetype)sharedInstance;
+ (void)releaseInstance;

@end

NS_ASSUME_NONNULL_END
