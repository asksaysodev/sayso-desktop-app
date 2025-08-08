//
//  ZMThreadCommentDictInfo.h
//  ZoomMsgUI
//
//  Created by Groot Ding on 2022/4/27.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN


@interface ZMThreadCommentState : NSObject
@property (copy)   NSString*      sessionID;
@property (copy)   NSString*      threadID;
@property (assign)  long long     threadSvrSideTime;
@property (assign) NSInteger      unreadCommentCount;
@property (assign) NSInteger      sdkUnreadCommentCount;
@property (copy)   NSString*      commentID;
@property (assign) long long      commentReadedTime;
@property (assign) long long      lastCommentSendTime;
@property (assign) BOOL           isReaded;
@property (assign) BOOL           isSubCMCMsg;

+ (ZMThreadCommentState*)findByThreadSvrSideTime:(NSInteger)threadSvrTime inArray:(NSArray <ZMThreadCommentState * >*)threadCommentStateArray;
+ (void)sort:(NSMutableArray<ZMThreadCommentState * > *)threadCommentStateArray;
@end

@interface ZMThreadCommentDictInfo : NSObject

+ (ZMThreadCommentDictInfo*)sharedInstance;

+ (void)releaseInstance;

- (NSMutableArray<ZMThreadCommentState* > *)threadCommentArrayWithSessionID:(NSString*)sessionID;
- (void)removeThreadCommentArrayWithSessionID:(NSString*)sessionID;
- (NSInteger)unreadCommentOnThread:(NSInteger)threadSvrTime sessionID:(NSString*)sessionID;

- (NSMutableArray<NSString*> *)atMessageIdArrayWithSessionID:(NSString*)sessionID;
- (void)removeAtMessageIdArrayWithSessionID:(NSString*)sessionID;

- (NSMutableDictionary *)threadCommentDictionaryWithSessionID:(NSString*)sessionID;
- (void)removethreadCommentDictionaryWithSessionID:(NSString*)sessionID;
- (long long)commentReadTimeWithThreadSvrTime:(long long)svrTime sessionID:(NSString*)sessionID;

@end

NS_ASSUME_NONNULL_END
