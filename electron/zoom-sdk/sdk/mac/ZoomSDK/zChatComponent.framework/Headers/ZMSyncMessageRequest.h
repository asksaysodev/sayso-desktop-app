//
//  ZMSyncMessageRequest.h
//  zChatUI
//
//  Created by groot.ding on 2019/7/10.
//  Copyright © 2019 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>

@class ZMSyncMessageFilter;

@interface ZMSyncMessageRequest : NSObject
@property (nonatomic,copy) NSString* sessionId;
@property (nonatomic,copy) NSString* dbReqID;
@property (nonatomic,copy) NSString* xmsReqID;
@property (nonatomic,copy) NSString* startMsgId;
@property (nonatomic,assign) NSInteger requestType;
@property (nonatomic,assign) BOOL ignoreNotReadyMessage;
@property (nonatomic,assign) BOOL isSyncHistory;
@property (nonatomic,assign) NSInteger realShowMessageCount;
@property (nonatomic,copy) ZMSyncMessageFilter* syncFilter;
- (void)cleanRequest;
- (void)resetDBRequestID;
- (void)resetXMSRequestID;
- (BOOL)isValidRequest;
- (BOOL)matchRequestByDBReqId:(NSString *)dbReqId xmsReqId:(NSString *)xmsReqId;
@end
