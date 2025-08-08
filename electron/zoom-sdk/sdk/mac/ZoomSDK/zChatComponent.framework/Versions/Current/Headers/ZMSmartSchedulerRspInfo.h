//
//  ZMSmartSchedulerRspInfo.h
//  zChatComponent
//
//  Created by Simon Shang on 2023/8/23.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMSmartIntentButtonParam : NSObject

@property (nonatomic, copy) NSString *messageId;
@property (nonatomic, copy) NSString *sessionId;
@property (nonatomic, copy) NSString *value;

- (NSString *)getSmartIntentTitle;

@end

@interface ZMSmartSchedulerRspInfo : NSObject

@property (nonatomic, retain) NSArray *intentButtons;
@property (nonatomic, retain) NSArray *noIntentButtons;

@end

NS_ASSUME_NONNULL_END
