//
//  ZMSyncMessageFilter.h
//  zChatUI
//
//  Created by groot.ding on 2019/7/10.
//  Copyright © 2019 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ZMSyncMessageFilter : NSObject

@property (copy) NSString *sessionId;
@property (copy) NSString *threadId;
@property (assign) long long threadServerTime;

@property (copy) NSString *messageId;
@property (assign) long long messageServerTime;
@property (assign) long long messageVisableTime;

@property (nonatomic,assign) NSInteger reqFailCount;
@property (assign) NSInteger pageSize;
@property (assign) BOOL isSyncHistroy;

@property (assign) BOOL isFetchThread;

@property (assign) BOOL needLoaclDBQuery;
@property (assign) BOOL needWebQuery;

@property (assign) BOOL needQueryXmsPreview;

@property (assign) BOOL isSyncUnreadComments;

+ (instancetype)filterWithSessionId:(NSString *)sessionId messageId:(NSString *)messageId messageServerTime:(long long)messageServerTime;

+ (instancetype)filterWithSessionId:(NSString *)sessionId messageId:(NSString *)messageId messageServerTime:(long long)messageServerTime pageSize:(NSInteger)pageSize isSyncHistroy:(BOOL)isSyncHistroy;

+ (instancetype)filterWithSessionId:(NSString *)sessionId messageId:(NSString *)messageId messageServerTime:(long long)messageServerTime threadId:(NSString *)threadId threadServerTime:(long long)threadServerTime pageSize:(NSInteger)pageSize isSyncHistroy:(BOOL)isSyncHistroy isFetchThread:(BOOL)isFetchThread;

@end

