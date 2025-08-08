//
//  ZMPinMessageCallback.h
//  zChatUI
//
//  Created by groot.ding on 25/09/2020.
//  Copyright © 2020 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "zPTApp/SaasBeePTAppInterface.h"

NS_ASSUME_NONNULL_BEGIN

@class ZMMessageAdapter;
@class ZMPinMessageInfo;

@interface ZMPinMessageCallback : NSObject

@property (nonatomic,assign) NSInteger result;

@property (nonatomic,copy) NSString *requestId;

@property (nonatomic,copy) NSString *sessionId;
@property (nonatomic,copy) NSString *actionOwnerId;
@property (nonatomic,copy) NSString *actionOwnerName;

@property (nonatomic,copy) NSString *msgId;

@property (nonatomic,assign) long long tmServerSide;

@property (nonatomic,retain) NSArray <ZMPinMessageInfo *> *pinMessages;

@property (nonatomic,retain) NSDictionary <NSString *,ZMPinMessageInfo *> *topPinMessageDict;

// for remove pinned message
@property (nonatomic,assign) BOOL hasRemovedTop;
@property (nonatomic,copy) NSString *removedTopMessageId;

@property (nonatomic,assign) BOOL hasMore;
@property (nonatomic,assign) long long nextTimeInterval;

@property (nonatomic,assign) BOOL isTriggerByRevokeMsg;

+ (instancetype)callbackByInfo:(const ns_zoom_messager::PinMessageCallBackInfo &)callbackInfo;

@end

NS_ASSUME_NONNULL_END
