//
//  ZMReminderMessageInfo.h
//  zChatComponent
//
//  Created by Kris Smith on 4/5/22.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMMessageAdapter;

@interface ZMReminderMessageInfo : NSObject

@property (nonatomic, readonly) ZMMessageAdapter *message;

@property (nonatomic, assign) unsigned long long serverTime;
@property (nonatomic, copy) NSString *sessionId;
@property (nonatomic, copy) NSString *messageId;
@property (nonatomic, assign) NSInteger timeout;
@property (nonatomic, assign) long expiry;
@property (nonatomic, copy) NSString *note;
@property (nonatomic, copy) NSString *content;
@property (nonatomic, assign) long long displayTime;
@property (nonatomic, assign) NSUInteger status;
@property (nonatomic, assign) NSUInteger event;

- (void)updateMessageIdAndSvrSideTimeIfNeed;

@end

NS_ASSUME_NONNULL_END
