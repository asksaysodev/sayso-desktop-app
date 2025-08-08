//
//  ZMMessageRevokeInfo.h
//  ZoomMsgUI
//
//  Created by Groot Ding on 2022/4/27.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, ZMRevokeMsgActionType) {
    ZMRevokeMsgActionType_None,
    ZMRevokeMsgActionType_RevokeMsgByAccountAdm,
    ZMRevokeMsgActionType_RevokeMsgByEventChannelAdmin,
    ZMRevokeMsgActionType_RevokeMsgByChannelAdmin,
};

@interface ZMMessageRevokeInfo : NSObject

@property (assign)  NSInteger result;
@property (copy)    NSString *actionOwner;
@property (copy)    NSString *messageOwner;
@property (assign)  NSInteger messageType;
@property (copy)    NSString *sessionId;
@property (copy)    NSString *messageId;
@property (assign)  long long messageTime;
@property (assign)  long long threadSvrTime;
@property (copy)    NSString *cmdMessageId;
@property (assign)  long long cmdMessageTime;
@property (assign)  long long previousMessageTime;
@property (assign)  BOOL isRecallThread;
@property (assign)  BOOL isRecallFileReply;
@property (assign,readonly)  ZMRevokeMsgActionType msgActionType;
@property (assign)  NSInteger msgActionTypeRaw;
@property (assign)  BOOL isOnlineRevoke;

@end

NS_ASSUME_NONNULL_END
