//
//  ZMSmartReplyManager.h
//  zChatComponent
//
//  Created by Zoro.Fu on 2023/7/13.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef void(^ZMSmartReplyResponse)(NSString *sessionId, NSString *threadId, NSArray<NSString *> *replyArr);

@interface ZMSmartReplyRequest : NSObject

@property (nonatomic, readonly) NSString *sessionId;
@property (nonatomic, readonly) NSString *threadId;

- (void)requestWithResponse:(ZMSmartReplyResponse)response;
- (void)telemetryWithSelectedString:(nullable NSString *)string;
- (void)cancel;

- (BOOL)isRunningForMessages:(NSArray<NSString *> *)messageIds;

@end

@interface ZMSmartReplyManager : NSObject

+ (instancetype)sharedInstance;

- (BOOL)isSessionEnableSmartReply:(NSString *)sessionId;

- (ZMSmartReplyRequest *)requestWithSessionId:(NSString *)sessionId
                                     threadId:(nullable NSString *)threadId
                                  messageIds:(NSArray<NSString *> *)messageIds
                                  isSpecific:(BOOL)isSpecific;



@end

NS_ASSUME_NONNULL_END
