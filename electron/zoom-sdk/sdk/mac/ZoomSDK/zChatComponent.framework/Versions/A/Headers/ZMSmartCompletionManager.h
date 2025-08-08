//
//  ZMSmartCompletionManager.h
//  zChatComponent
//
//  Created by Zoro.Fu on 2023/8/18.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMSmartCompletionRequest;

typedef void(^ZMSmartCompletionResponse)(ZMSmartCompletionRequest *request);

@interface ZMSmartCompletionRequest : NSObject

@property (nonatomic, readonly) NSString *sessionId;
@property (nonatomic, readonly) NSString *threadId;
@property (nonatomic, copy)     NSString *inputString;
@property (nonatomic, readonly) NSString *suggestionString;

@property (nonatomic, assign) BOOL isReply;

- (void)requestWithResponse:(ZMSmartCompletionResponse)response;
- (void)cancel;

@end

@interface ZMSmartCompletionManager : NSObject

+ (instancetype)sharedInstance;

- (BOOL)isSessionEnableSmartCompletion:(NSString *)sessionId;

- (BOOL)isSessionEnableSpaceBarForCompletion:(NSString *)sessionId;

- (ZMSmartCompletionRequest *)requestWithSessionId:(NSString *)sessionId threadId:(NSString *_Nullable)threadId;

- (void)didSelectedCompletionInSession:(NSString *)sessionId isReply:(BOOL)isReply;

- (void)didDeleteTailInSession:(NSString *)sessionId;

@end

NS_ASSUME_NONNULL_END
