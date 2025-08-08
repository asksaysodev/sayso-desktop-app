//
//  ZMCAppBridgeEventHandler.h
//  zChatComponent
//
//  Created by Cyan.Yang on 2025/4/16.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMCAppBridgeEvent : NSObject

@property (nonatomic, assign) NSInteger serviceType;
@property (nonatomic, copy) NSString *payload;
@property (nonatomic, copy) NSString *eventName;

@end

@interface ZMCAppBridgeEventHandler : NSObject

+ (instancetype)new NS_UNAVAILABLE;

+ (instancetype)handlerForEvent:(NSString *)event
                    serviceType:(NSInteger)serviceType
                          block:(void (^)(ZMCAppBridgeEvent *))block;

- (instancetype)init NS_UNAVAILABLE;
- (instancetype)initForEvent:(NSString *)event
                 serviceType:(NSInteger)serviceType
                       block:(void (^)(ZMCAppBridgeEvent *))block;

- (void)invalidate;

@end

NS_ASSUME_NONNULL_END
