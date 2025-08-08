//
//  ZMSessionModule.h
//  zChatUI
//
//  Created by Jed Zheng on 10/11/2021.
//  Copyright © 2021 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>

@class ZMMessageSessionAdapter, ZMMissedCallItemInfo,ZMPersonalFolderItemMode;

typedef void(^ZMSessionOperateCompletion)(NSString * sessionId, NSError* error,id param);

@protocol ZMSessionOperateCallBack <NSObject>
- (void)didUnreadCountUpdateWithSessionId:(NSString *)sessionId;
- (void)didUnreadCountCleanWithSessionId:(NSString *)sessionId;
- (void)didSessionListDataReload;
- (void)didSessionLastMessageUpdatedWithSessionIds:(NSArray *)sessionIds;

//session list
- (void)didRecentSessionListReady;
- (void)didUpdateSessionListUnreadCount;
- (void)didSessionMarkUnreadCountUpdateWithSessionId:(NSString *)sessionId;
- (void)didSessionSendErrorStatusUpdateWithSessionId:(NSString *)sessionId;
- (void)didSessionRemovedByStatueChangedWithSessionIds:(NSArray *)sessionIds;
- (void)didSessionDraftUpdateWithSessionId:(NSString *)sessionId;

- (void)didSessionMarkGroupSessionsAllAsReadWithSessionIds:(NSArray *)sessionIds errorCode:(NSInteger)errorCode;

- (void)didStarredSectionSortOptionUpdated;

//Missed call
- (void)didChatHistoryDeletedWithSessionId:(NSString *)sessionId;
- (void)didNewMissedCallAddedWithSessionId:(NSString *)sessionId missedCallInfo:(ZMMissedCallItemInfo *)info;
- (void)didMissedCallUnreadCountUpdate:(NSInteger)count;
- (void)didMissedCallDataReady;
- (void)didConnectChanged:(NSInteger)state;


- (void)didShowPMCSessionFTViewWithGroupId:(NSString *)groupId;
- (void)didToadysMeetingDataReady;
@end

@protocol ZMSessionOperateListen <NSObject>
- (void)removeAllListenByTarget:(id)target;

- (void)addListenSessionListDataReloadByTarget:(id)target completion:(ZMCallbackBlock)completion;
- (void)addListenUnreadCountUpdateByTarget:(id)target completion:(ZMCallbackBlock)completion;
- (void)addListenUnreadCountCleanByTarget:(id)target completion:(ZMCallbackBlock)completion;
- (void)addListenSessionLastMessageUpdatedByTarget:(id)target completion:(ZMCallbackBlock)completion;

- (void)addListenRecentSessionListReadyByTarget:(id)target completion:(ZMCallbackBlock)completion;
- (void)addListenSessionListUnreadUpdateByTarget:(id)target completion:(ZMCallbackBlock)completion;
- (void)addListenSessionMarkUnreadCountUpdateByTarget:(id)target completion:(ZMCallbackBlock)completion;
- (void)addListenSessionSendErrorStatusUpdateByTarget:(id)target completion:(ZMCallbackBlock)completion;
- (void)addListenSessionRemovedByStatueChangedByTarget:(id)target completion:(ZMCallbackBlock)completion;
- (void)addListenSessionDraftUpdateByTarget:(id)target completion:(ZMCallbackBlock)completion;

- (void)addListenSessionMarkGroupSessionsAllAsReadByTarget:(id)target completion:(ZMCallbackBlock)completion;

- (void)addListenStarredSectionSortOptionUpdatedByTarget:(id)target completion:(ZMCallbackBlock)completion;

//Missed call
- (void)addListenSessionChatHistoryDeletedByTarget:(id)target completion:(ZMCallbackBlock)completion;
- (void)addListenNewMissedCallAddedByTarget:(id)target completion:(ZMCallbackBlock)completion;
- (void)addListenMissedCallUnreadCountUpdateByTarget:(id)target completion:(ZMCallbackBlock)completion;
- (void)addListenMissedCallDataReadyByTarget:(id)target completion:(ZMCallbackBlock)completion;
- (void)addListenConnectChangedByTarget:(id)target completion:(ZMCallbackBlock)completion;

- (void)addListenShowPMCSessionFTViewByTarget:(id)target completion:(ZMCallbackBlock)completion;
- (void)addListenTodaysMeetingDataReadyByTarget:(id)target completion:(ZMCallbackBlock)completion;
@end

@protocol ZMSessionOperateProtocol <NSObject>

- (void)clearSessionChatHistoryWithSessionId:(NSString *)sessionId;

- (BOOL)isSessionStarred:(NSString*)sessionId;

- (BOOL)setSession:(NSString*)sessionId starred:(BOOL)bStarred;

- (NSString *)getSessionFolderIdWithSessionId:(NSString *)sessionId;

- (ZMPersonalFolderItemMode *)getSessionFolderInfoWithSessionId:(NSString *)sessionId;

- (ZMPersonalFolderItemMode *)getFolderInfoWithFolderId:(NSString *)folderId;

- (ZMMessageSessionAdapter *)findSessionById:(NSString *)sessionId;

- (NSInteger)getStarredSectionSortOption;

- (BOOL)setStarredSectionSortOptionWithSortType:(NSInteger)sortType;

- (NSInteger)getSessionsListType;

- (BOOL)setSessionsListType:(NSInteger)listType;

- (BOOL)markAsReadWithSessionIds:(NSArray *)sessionIds;

@end

@interface ZMSessionModule : NSObject<ZMSessionOperateListen, ZMSessionOperateCallBack,ZMSessionOperateProtocol>
@property (nonatomic, assign, readonly) BOOL isDataReady;
+ (instancetype)shareInstance;
@end
