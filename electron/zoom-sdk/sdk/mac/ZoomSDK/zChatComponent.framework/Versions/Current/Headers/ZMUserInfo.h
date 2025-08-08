//
//  ZMUserInfo.h
//  SaasBeePTUIModule
//
//  Created by groot.ding on 1/18/21.
//  Copyright © 2021 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMBuddyAdapter;

@interface ZMUserInfo : NSObject

+ (instancetype)userInfoWithJid:(NSString*)jid name:(NSString*)name;
- (instancetype)initWithJid:(NSString*)jid name:(NSString*)name;

@property (nonatomic,copy) NSString *jid;
@property (nonatomic,copy) NSString *nickName;

- (ZMBuddyAdapter *)getBuddy;

@end

NS_ASSUME_NONNULL_END
