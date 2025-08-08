//
//  ZMBuddyProtocal.h
//  ZoomMsgUI
//
//  Created by Groot Ding on 2022/4/26.
//

#ifndef ZMBuddyProtocal_h
#define ZMBuddyProtocal_h

#import <Cocoa/Cocoa.h>
#import <ZoomKit/ZoomKit.h>
#import <ZoomUnit/ZMProfileConfig.h>

NS_ASSUME_NONNULL_BEGIN

typedef void(^ZMBuddyListenCompletion)(NSArray <NSString *> * _Nullable jids);

typedef NS_ENUM(NSInteger, ZMBuddyCacheType){
    ZMBuddyCacheType_IM,
    ZMBuddyCacheType_Meeting,
};

typedef NS_ENUM(NSInteger, ZMAddContactSourceType){
    ZMAddContactSource_newChat,
    ZMAddContactSource_chatMainPanel,
    ZMAddContactSource_contactsTab,
    ZMAddContactSource_profileCard,
    ZMAddContactSource_channelMemberPanel,
    ZMAddContactSource_other,
};

@class ZMBuddyAdapter;
@class ZMUserSignatureData;
@class ZMUserProfileInfo;
@class ZMAssignedGroupSearchResult;

@protocol ZMBuddyOperateProtocol <NSObject>

- (nullable void *)getRawDataOfBuddyByJid:(nonnull NSString *)jid;

- (nullable ZMBuddyAdapter *)getBuddyByJId:(nonnull NSString *)jid;

- (nullable ZMBuddyAdapter *)getBuddyByEmail:(nonnull NSString*)email;

- (nullable ZMBuddyAdapter *)getBuddyByEmailFromCache:(nonnull NSString*)email;

- (nullable ZMBuddyAdapter *)getBuddyByJId:(nonnull NSString *)jid sessionId:(nullable NSString *)sessionId;

- (nullable ZMBuddyAdapter *)getBuddyByZoomBuddy:(nonnull void *)pBuddy;

- (nullable ZMBuddyAdapter *)getBuddyByZoomBuddyWithRefresh:(nonnull void *)buddyTmp refresh:(BOOL)refresh sessionId:(NSString *)sessionId;

- (nullable ZMBuddyAdapter *)getBuddyCreateIfNotExistByJID:(nonnull NSString *)jid;

- (nullable ZMBuddyAdapter *)getBuddyCreateIfNotExistByEmail:(nonnull NSString *)email;

- (nullable ZMBuddyAdapter*)getPendingBuddyByPendingID:(nonnull NSString *)pendingID userName:(nullable NSString*)userName;

- (nullable ZMBuddyAdapter *)getCallQueueBuddyByZoomBuddy:(nonnull void *)pBuddy;

- (nullable NSArray<ZMBuddyAdapter*>*)quickGetSortedMatchedBuddiesByKey:(nonnull NSString*)key withFilter:(NSInteger)filterValue;

- (ZMUserProfileInfo *)getUserProfileInfoByJid:(NSString *)jid;

- (ZMUserProfileInfo *)getUserProfileInfoByJid:(NSString *)jid remoteCallBack:(void(^)(ZMUserProfileInfo *info))remoteCallBack;

- (BOOL)getBuddiesPresence:(nonnull NSArray*)jids shouldSubscribe:(BOOL)bSubscribe;

- (NSInteger)subscribeContactsPresence_v2:(nonnull NSArray*)jids forType:(int)type;

- (nullable NSMutableArray*)sortBudddies:(nullable NSArray*)buddiesArray;

- (void)refreshBuddyVCard:(nonnull NSString *)jid;

- (void)refreshBuddyVCards:(nonnull NSArray<NSString *> *)jids; //ZOOM-68776

- (void)refreshBuddyVCards:(nonnull NSArray<NSString *> *)jids rightNow:(BOOL)rightNow;

- (void)refreshBuddyVCards:(nonnull NSArray<NSString *> *)jids rightNow:(BOOL)rightNow sessionId:(nullable NSString *)sessionId;

- (void)refreshBuddyChatAvailable:(nonnull NSString *)jid;

- (BOOL)refreshBuddyBigPicture:(nonnull NSString*)jid;

- (BOOL)isUserBlocked:(nonnull NSString *)jid;

- (NSArray <NSString *> *)getAllBlockContacts;

- (nullable NSString*)getBuddyScreenNameByJID:(nullable NSString*)jid;

- (void)blockUsers:(nonnull NSArray <NSString *> *)uids
        completion:(nullable void(^)(NSArray <NSString *> * _Nonnull jids, NSError * _Nullable error))completion;

- (void)unBlockUsers:(nonnull NSArray <NSString *> *)uids 
          completion:(nullable void(^)(NSArray <NSString *> * _Nonnull jids,NSError * _Nullable error))completion;

- (BOOL)unblockContacts:(nonnull NSArray*)contacts;

- (BOOL)isDeactivedBuddy:(nonnull NSString*)jid;

- (BOOL)isTerminatedBuddy:(nonnull NSString*)jid;

- (BOOL)isZoomRoomContact:(nonnull NSString*)jid;

- (BOOL)isAutoAcceptFECCBuddy:(nonnull NSString *)inJID;

- (BOOL)addToFECCGroup:(nonnull NSString *)inJID;

- (BOOL)removeFromFECCGroup:(nonnull NSString *)inJID;

/// User ID is Buddy ID.
- (nullable NSString *)getMyUserIDBySessionId:(nullable NSString *)sessionId;
- (nullable NSString *)getMyJidBySessionId:(nullable NSString *)sessionId;
- (nullable NSString *)getMyScreenNameBySessionId:(nullable NSString *)sessionId;

- (BOOL)checkMySelf:(nonnull NSString *)jid sessionId:(nullable NSString *)sessionId;

- (BOOL)isMyContact:(nonnull NSString *)jid sessionId:(nullable NSString *)sessionId;

- (BOOL)isZoomRoom:(nonnull NSString *)jid sessionId:(nullable NSString *)sessionId;

- (BOOL)checkBuddyInAddonsGroup:(nonnull NSString *)jid;

- (BOOL)isRobot:(nonnull NSString *)jid;

- (BOOL)isAuditRobot:(nonnull NSString *)jid;

- (nullable NSString*)getMeetingBuddyScreenNameByJID:(nullable NSString*)jid sessionId:(nullable NSString *)sessionId;

- (BOOL)setUserSignature:(nonnull NSString*)string requestID:(NSString * _Nullable * _Nullable)requestID;

- (BOOL)setUserSignatureData:(nonnull ZMUserSignatureData *)signatureData requestID:(NSString * _Nullable * _Nullable)requestID;

- (BOOL)setUserSignatureAsCloseReminder:(nonnull NSArray *)jids;

- (void)cleanBuddiesCacheByType:(ZMBuddyCacheType)type;

- (BOOL)isAutoAcceptFECCOn;

- (BOOL)isAutoAcceptAudioControlOn;
- (BOOL)isAutoAcceptAudioControlBuddy:(NSString *)inJID;
- (BOOL)addToAudioControlGroup:(NSString *)inJID;
- (BOOL)removeFromAudioControlGroup:(NSString *)inJID;

- (BOOL)isRealSameOrg:(nonnull NSString *)jid with:(nonnull NSString *)anotherJid;
- (BOOL)isRealSameOrgWithJid:(nonnull NSString *)jid hasError:(BOOL *)hasError;

- (BOOL)isExternalLabelDisabled;

@end

@protocol ZMBuddyUpdateCallback <NSObject>

- (void)didAllBuddyListUpdate;

- (void)didMyBuddyUpdate;

- (void)didBuddyUpdate:(nonnull NSString *)jid;

- (void)didBuddiesUpdate:(nonnull NSArray <NSString *> *)jids;

- (void)didBuddyRemove:(nonnull NSString *)jid;

- (void)didBuddiesIBUpdate:(nonnull NSArray <NSString *> *)jids;

- (void)didBuddyDeactivated:(nonnull NSString *)jid;

- (void)didBuddyAsyncLoadedFromDB;

@end


@protocol ZMBuddyPresenceUpdateCallBack <NSObject>

- (void)didBuddyPresenceUpdate:(nonnull NSString *)jid;

- (void)didBuddyHuddleStatusUpdate:(nonnull NSString *)jid;

- (void)didConnectChange:(NSInteger)state;

- (void)willPresenceSubscriptionExpire:(nonnull NSArray *)jids;

- (void)didMobileOnlineCatchWithJids:(NSArray *)jids;

@end

@protocol ZMChatUIBuddyUpdateInterfaceProtocol <NSObject>

@optional

- (void)didBuddiesUpdate:(NSArray <NSString *> *)jids;

- (void)didBuddyPresenceUpdate:(NSString *)jid;

- (void)willPresenceSubscriptionExpire:(NSArray *)jids;

- (void)didBuddyHuddleStatusUpdate:(NSArray <NSString *> *)jids;

@end

@protocol ZMBuddyBlockCallBack <NSObject>

- (void)didBlockUsers:(nonnull NSArray <NSString *> *)uids;

- (void)didUnBlockUsers:(nonnull NSArray <NSString *> *)uids;

- (void)didBlockUsersUpdate;

@end

@protocol ZMBuddyUpdateListen <NSObject>

- (void)addListenBuddyAsyncLoadedByTarget:(nonnull id)target completion:(nonnull ZMBuddyListenCompletion)completion;

- (void)addListenAllBuddyListUpdateByTarget:(nonnull id)target completion:(nonnull ZMBuddyListenCompletion)completion;

- (void)addListenMySelfBuddyUpdateByTarget:(nonnull id)target completion:(nonnull ZMBuddyListenCompletion)completion;

- (void)addListenBuddiesUpdateByTarget:(nonnull id)target completion:(nonnull ZMBuddyListenCompletion)completion;

- (void)addListenBlockUsersUpdateByTarget:(nonnull id)target completion:(nonnull ZMBuddyListenCompletion)completion;

- (void)addListenIBUpdateByTarget:(nonnull id)target completion:(nonnull ZMBuddyListenCompletion)completion;

- (void)addListenBuddyDeactivated:(nonnull id)target completion:(nonnull ZMBuddyListenCompletion)completion;

- (void)addListenBuddyHuddleStatusUpdate:(nonnull id)target completion:(nonnull ZMBuddyListenCompletion)completion;

- (void)removeAllListenByTarget:(nonnull id)target;

@end


@protocol ZMSearchBuddyProtocol <NSObject>

- (BOOL)searchBuddy:(NSString *)key callBack:(void(^)(NSString *key,NSArray <NSString *> * jids,NSError *error))callBack;

/**
 search remote buddy API v2

 @param key key value
 @param param,keys @"contactType" , @"forInviteToMeeting"
 @param callBack call back block
 */
- (NSString *)searchBuddy_V2:(NSString *)key param:(NSDictionary *)param callBack:(void(^)(NSString *key,NSArray <NSString *> * jids,NSError *error))callBack;

- (void)searchBuddy_V2_Number:(NSString *)key param:(NSDictionary *)param callBack:(void(^)(NSString *key,NSArray <NSString *> * jids,NSError *error))callBack;

- (void)searchBuddy_V2_AssignGroup:(NSString *)key param:(NSDictionary *)param callBack:(void(^)(NSString *key,NSArray <ZMAssignedGroupSearchResult *> * groups,NSError *error))callBack;

- (void)searchBuddyByEmail:(NSString *)email callBack:(void(^)(NSString *key,NSArray <NSString *> * jids,NSError *error))callBack;

- (void)cancleSearchBuddyByKey:(NSString *)key;

@end

@protocol ZMBuddySearchCallback <NSObject>

@optional

- (void)didSearchBuddy:(NSUInteger)result email:(NSString*)email requestId:(NSString *)requestId jids:(NSArray <NSString *> *)jids;

- (void)didSearchBuddy:(NSUInteger)result key:(NSString*)key jids:(NSArray <NSString *> *)jids reqId:(NSString *)reqId;

- (void)didAssignGroupSearchBuddy:(NSUInteger)result key:(NSString*)key jids:(NSArray <ZMAssignedGroupSearchResult *> *)groups;
@end

@protocol ZMBuddyProfileFetchCallback <NSObject>

- (void)didFetchProfile:(unsigned int)result profileInfo:(ZMUserProfileInfo *)profileInfo;

- (void)updateBuddyFromReqID:(NSString*)reqID withManagerName:(NSString*)managerName withManagerjID:(NSString*)managerjID;

@end

@protocol ZMBuddyProfileCardProtocol <ZMRoutableObject>
- (void)showUserProfilePannel:(nonnull NSString *)uid positioningView:(nonnull NSView *)positioningView autoClose:(BOOL)autoClose sessionId:(nullable NSString*)sessionId;

- (void)showUserProfilePannel:(nonnull NSString *)uid positioningView:(nonnull NSView *)positioningView relativeRect:(NSRect)rect;

- (void)showUserProfilePannel:(nonnull NSString *)uid positioningView:(nonnull NSView *)positioningView trackType:(ZMUserProfileTrackType)trackType;

- (void)showUserProfilePannel:(nonnull NSString *)uid positioningView:(nonnull NSView *)positioningView autoClose:(BOOL)autoClose trackType:(ZMUserProfileTrackType)trackType;

- (void)showUserProfilePannel:(nonnull NSString *)uid positioningView:(nonnull NSView *)positioningView autoClose:(BOOL)autoClose trackType:(ZMUserProfileTrackType)trackType profileParam:(NSDictionary *)param;

- (void)showUserProfilePannel:(nonnull NSString *)uid positioningView:(nonnull NSView *)positioningView autoClose:(BOOL)autoClose panelSourceType:(int)sourceType;

- (void)showUserProfilePannel:(nonnull NSString *)uid positioningView:(nonnull NSView *)positioningView relativeRect:(NSRect)rect byMousePoint:(BOOL)mouse;

- (void)showUserProfilePannel:(nonnull NSString *)uid positioningView:(nonnull NSView *)positioningView relativeRect:(NSRect)rect byCenterPoint:(BOOL)center;

- (void)showAddContactPannelWithEmail:(nonnull NSString *)email addTip:(BOOL)addTip;

- (void)showAddingContactPannelWithJid:(nonnull NSString *)jid onWindow:(NSWindow * _Nullable)window;

- (void)showAddContactPannelWithEmail:(nonnull NSString *)email note:(NSString *)note source:(ZMAddContactSourceType)source onWindow:(NSWindow * _Nullable)window;

- (void)showAddingContactPannelWithJid:(nonnull NSString *)jid source:(ZMAddContactSourceType)source onWindow:(NSWindow * _Nullable)window;

- (BOOL)isMouseOnVcard;

@end

@protocol ZMBuddyProtocol <ZMBuddyOperateProtocol,ZMBuddyUpdateCallback,ZMBuddyPresenceUpdateCallBack,ZMBuddyBlockCallBack,ZMBuddyUpdateListen,ZMSearchBuddyProtocol,ZMBuddySearchCallback,ZMBuddyProfileFetchCallback>

@end

#ifndef ZMShareBuddyProtocol
#define ZMShareBuddyProtocol (id <ZMBuddyProtocol>)ZMSharedFor(ZMBuddyProtocol)
#endif

#ifndef ZMShareBuddyProfileProtocol
#define ZMShareBuddyProfileProtocol (id <ZMBuddyProfileCardProtocol>)ZMSharedFor(ZMBuddyProfileCardProtocol)
#endif

NS_ASSUME_NONNULL_END

#endif /* ZMBuddyProtocal_h */
