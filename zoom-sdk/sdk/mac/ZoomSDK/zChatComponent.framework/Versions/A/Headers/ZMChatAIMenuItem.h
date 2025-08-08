//
//  ZMChatAIMenuItem.h
//  zChatComponent
//
//  Created by Cyan.Yang on 2024/11/19.
//

#import <Foundation/Foundation.h>

#ifdef __cplusplus
struct AICConfSetting;
#endif

NS_ASSUME_NONNULL_BEGIN

NS_SWIFT_NAME(ChatAIMenuItem)
@interface ZMChatAIMenuItem : NSObject

@property (nonatomic, assign) long long type;
@property (nonatomic, copy) NSString *promptID;
@property (nonatomic, copy) NSString *title;
@property (nonatomic, copy) NSString *text;

#ifdef __cplusplus
- (instancetype)initWithConfSetting:(const AICConfSetting &)confSetting NS_DESIGNATED_INITIALIZER;
#endif

+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;

@end

NS_ASSUME_NONNULL_END
