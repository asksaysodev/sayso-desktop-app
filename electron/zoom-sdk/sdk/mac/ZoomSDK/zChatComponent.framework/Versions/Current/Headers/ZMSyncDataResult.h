//
//  ZMSyncDataResult.h
//  ZoomMsgUI
//
//  Created by Groot Ding on 2022/4/27.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMSyncDataResult : NSObject

@property (copy) NSString *sessionID;

@property (copy) NSString *startMessageID;

@property (copy) NSString* threadID;

@property (copy) NSString* dbReqID;
@property (copy) NSString* xmsReqID;

@property (copy) NSArray <NSString *> *resultMessageIds;

@property (assign)  BOOL isDataFromAsync;
@property (assign)  BOOL result;

@property (assign)  NSInteger resultCode;

@property (nonatomic, assign)  BOOL isDataStateReady;
@property (assign)  BOOL isDataStateLoadingDB;
@property (assign)  BOOL isDataStateSyncing;
@property (assign)  BOOL isDataStateDirty;
@property (assign)  BOOL isDataStateError;
@property (assign)  BOOL isStartThreadRedirected;
@property (assign)  BOOL isDataStatePreviewNoPermission;

+ (ZMSyncDataResult *)createZMThreadDataResultFromSDKThreadDataResult:(void*)threadDataResult NS_RETURNS_RETAINED;

+ (ZMSyncDataResult *)createZMCommentDataResultFromSDKCommentDataResult:(void*)commentDataResult NS_RETURNS_RETAINED;

@end

NS_ASSUME_NONNULL_END
