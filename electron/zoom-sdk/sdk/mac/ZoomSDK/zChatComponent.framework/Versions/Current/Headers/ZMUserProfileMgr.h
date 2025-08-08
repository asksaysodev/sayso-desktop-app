//
//  ZMUserProfileMgr.h
//  zChatComponent
//
//  Created by Jed Zheng on 8/6/2023.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMUserProfileMgr : NSObject
+ (NSViewController *)getNewVcardViewController:(NSString *)uid floatConfig:(ZMProfileConfig *)config;
@end

NS_ASSUME_NONNULL_END
