//
//  ZMSearchBuddyParam.h
//  ChatUI
//
//  Created by Groot Ding on 27/12/21.
//  Copyright © 2021 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMSearchBuddyParam : NSObject

@property (nonatomic,copy) NSString *key;
@property (nonatomic,copy) NSString *contactType;
@property (nonatomic,copy) NSString *reqSource;
@property (nonatomic,assign) BOOL isForInviteToMeeting;
@property (nonatomic,assign) BOOL externalFlag;
@property (nonatomic,assign) int category;

+ (instancetype)paramWithContactTypes:(NSArray <NSNumber *>*)contactTypes;

@end

NS_ASSUME_NONNULL_END
