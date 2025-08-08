//
//  ZMChatsStarredMessageMgr.h
//  zChatUI
//
//  Created by Ryan Shen on 2018/8/4.
//  Copyright © 2018 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSUInteger, ZMChatsStarMessageActionType)
{
    ZMChatsStarMessageActionType_starMessage,
    ZMChatsStarMessageActionType_unstarMessage,
    ZMChatsStarMessageActionType_loadMoreMessage,
    ZMChatsStarMessageActionType_syncMessage,
};

@class ZMMessageAdapter;

@protocol ZMChatsStarredMessageCallBack <NSObject>

- (void)messagesUnstarred:(NSArray<NSDictionary*>*)messagesInfo;
- (void)starredMesagesUpdate:(NSArray<NSDictionary*>*)allMessagesInfo;
- (void)messagesStarred:(NSArray<ZMMessageAdapter*>*)messages;

@end

@protocol ZMChatsStarredAllMessageCallBack <NSObject>

- (void)loadMoreStarredMessages:(NSArray<ZMMessageAdapter*>*)messages;

@end

@interface ZMChatsStarredMessageMgr : NSObject

+ (ZMChatsStarredMessageMgr*)shareInstance;

//callback
- (void)onStarMessageDataUpdate;
- (void)onStarMessagesDataSyncBackWithResult:(int)reslut messages:(NSArray*)messages requestID:(NSString*)requestID;

- (void)addListeningWithTarget:(id<ZMChatsStarredMessageCallBack>)target;
- (void)removeListeningWithTarget:(id<ZMChatsStarredMessageCallBack>)target;

- (NSUInteger)starredMessageCount;

- (BOOL)hasMoreAllStarredMessageByStartMessageTime:(long long)msgTime startMessageSession:(NSString *)sessionId;

//action
- (NSArray<ZMMessageAdapter*>*)requestToGetStarredMessageInSession:(NSString*)sessionID withTarget:(id<ZMChatsStarredMessageCallBack>)target;
- (NSArray<ZMMessageAdapter*>*)requestToGetAllStarredMessageByInfo:(NSDictionary*)info withTarget:(id<ZMChatsStarredMessageCallBack>)target hasMore:(BOOL*)hasmore;
- (void)requestToSyncMessage:(NSDictionary*)syncInfo withTarget:(id<ZMChatsStarredMessageCallBack>)target;

@end
