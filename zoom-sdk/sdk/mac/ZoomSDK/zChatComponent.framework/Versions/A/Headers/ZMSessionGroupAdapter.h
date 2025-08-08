//
//  ZMSessionGroupAdapter.h
//  SaasBeePTUIModule
//
//  Created by Justin Fang on 5/13/14.
//  Copyright (c) 2014 Zipow. All rights reserved.
//

#import <Foundation/Foundation.h>

#if defined(__cplusplus)
#import <zPTApp/SaasBeePTAppInterface.h>
#endif

#import <zChatComponent/ZMSessionGroupAdapter.h>
#import <zChatComponent/ZMMemberInformationProtocol.h>
#import <zChatComponent/ZMChannelModule.h>
#import <zChatComponent/ZMChannelInitInfo.h>

extern ZMMemberInformationSourceType const kZMMemberInformationSourceType_Channel;


NS_ASSUME_NONNULL_BEGIN

enum
{
    GroupType_None = 0,
    GroupType_1V1,
    GroupType_MUC,
    GroupType_Channel,
};

typedef NS_ENUM(NSUInteger, ZMGroupAnnouncerType)
{
    ZMGroupAnnouncerType_EveryOne = 0,
    ZMGroupAnnouncerType_AdminOnly,
    ZMGroupAnnouncerType_AdminAndSpecialMember
};

typedef NS_ENUM(NSUInteger, ZMChannelUpdateState) {
    ZMChannelUpdateNone                       = 0,
    ZMChannelUpdateName                       = 1<<1,
    ZMChannelUpdateFlag                       = 1<<2,
    ZMChannelUpdateMember                     = 1<<3,
    ZMChannelUpdateDesc                       = 1<<4,
    ZMChannelUpdateInfo                       = 1<<5,
};

/*!
 * Contains channel/MUC data, including info about admins and members.
 */
@class ZMBuddyAdapter;
@class ZMSessionGroupProperty;
@class ZMUserInfo;
@class ZMGroupPersistentMeetingInfo;

@interface ZMSessionGroupAdapter : NSObject <ZMMemberInformationProtocol>
{
    NSUInteger _hashKey;
}
@property (nonatomic, retain) ZMSessionGroupProperty *_Nullable groupProperty;
@property (nonatomic, retain) NSString* m_gID;
@property (nonatomic, retain) NSString* m_gName;
@property (nonatomic, retain, readonly) NSImage *customChannelIcon;
@property (nonatomic, copy, readonly) NSString *channelIconID;
@property (nonatomic, retain) NSString* m_gOwner;
@property (nonatomic, copy) NSString* m_gDesc;
@property (nonatomic, retain) NSMutableArray* m_gAdmins;
@property (nonatomic, retain) NSMutableArray* m_buddies;
@property (nonatomic, retain) NSMutableArray* pendingBuddies;
@property (nonatomic, retain, nullable) NSArray * subAdmins;
@property (nonatomic, assign) BOOL m_amIInGroup;
@property (nonatomic, assign) BOOL isForceE2E;
@property (nonatomic, assign) BOOL isSearchableGroup;
@property (nonatomic, assign) BOOL isRestrictSameorg;
@property (nonatomic, assign) BOOL isPublicRoom;
@property (nonatomic, assign) BOOL isPrivateRoom;
@property (nonatomic, assign) BOOL isRoom;
@property (nonatomic, readonly) BOOL isShareSpaceOpenChannel;
#if defined(__cplusplus)
@property (nonatomic, assign) NS_SSB_XMPP::kMUCFlag flag;
#endif
@property (nonatomic, assign) BOOL amIAnnouncer;// Indicates whether I can speak in this group
@property (nonatomic, readonly) ZMGroupAnnouncerType announcerType;
@property (nonatomic, readonly) NSArray *announcers;
#if defined(__cplusplus)
@property (nonatomic, readonly) ns_zoom_messager::AtAllOption atAllOptionType;
#endif
@property (nonatomic, assign) BOOL isRestricHistoryMessages;
@property (nonatomic, assign, readonly) BOOL isPersistentMeetingGroup;
@property (nonatomic, assign, readonly) BOOL isNeedInsertPMCGroupChatSysMsg;
@property (nonatomic, assign, readonly) ZMWhoCanAddAppInChat whoCanAddAppInChat;
@property (nonatomic, assign, readonly) BOOL isSyncedAfterLogin;
@property (nonatomic, assign, readonly) BOOL isNeedInsertPMCGroupChatUnbindSysMsg;
@property (nonatomic, retain, readonly) ZMGroupPersistentMeetingInfo * _Nullable persistentMeetingInfo;
@property (nonatomic, assign, readonly) BOOL isPMCExistRealMessage;
@property (nonatomic, assign, readonly) BOOL isPMCDisableChat;
@property (nonatomic, readonly) NSArray<NSString*>* m_MUCNames;
@property (nonatomic, readonly) NSArray<ZMUserInfo *>* mucMemberInfos;
@property (nonatomic, assign, readonly) BOOL isAudited; //ZOOM-552941

@property (nonatomic, assign, readonly) BOOL canLeaveChannel;

@property (nonatomic, assign) BOOL isNewlyJoined;

@property (assign) NSInteger lastMatchScore;

@property (nonatomic,assign) NSInteger lastUpdateState;

#if defined(__cplusplus)
- (id)initWithSessionGroup:(NS_ZOOM_MESSAGER::IZoomGroup*)buddyGroup;
- (id)initWithSessionGroup:(NS_ZOOM_MESSAGER::IZoomGroup*)buddyGroup upDateMemberList:(BOOL)update;
#endif
- (NSArray*)getBuddyIDs;
- (NSString*)getGroupID;
- (NSString*)getGroupName;
- (NSString*)getGroupOwner;
- (NSString *)getGroupOwnerJId;
- (NSString*)getOwnerName;
- (NSUInteger)getBuddyCount;
- (NSString*)getSessionTitle;
- (BOOL)amIInGroup;
- (BOOL)amIAdmin;
- (BOOL)amIOwner;
- (BOOL)isBuddyInGroup:(nonnull NSString *)jid;
- (BOOL)isPendingBuddy:(nonnull NSString *)email;
- (nullable ZMBuddyAdapter*)getBuddyAt:(NSUInteger)index;
- (nullable ZMBuddyAdapter*)getBuddyWithJid:(NSString *)jid;
- (NSArray*)getBuddies;
- (NSUInteger)getBuddyCountWithoutMyself;
- (NSArray*)getBuddiesWithoutMyself;
- (NSArray*)getBuddiesWithoutPendingEmail;
- (NSArray*)getBuddiesWithoutMyselfAndRobot;
- (void)update;
- (void)updateAnnouncerInfo;
- (void)updateAtAllOption;
- (void)updateBuddies;
- (void)updateLastMatchScore;
- (void)updateProperty;
- (void)updateAdmins;
- (void)updateGroupPersistentMeetingInfo:(BOOL)isRequestFromWeb;
- (void)updateNeedInsertPMCsystemMsgStatus;
- (NSString*)getMucMemberListToolTip;
- (NSString*)getMucMemberListWithMaxCount:(NSInteger)memberCount;
+ (NSUInteger)groupTypeByID:(NSString*)groupID;
+ (BOOL)isGroupInfoReadyWithGroupID:(NSString *)groupID;
- (BOOL)isBroadcastAnnouncementReceiver;
- (BOOL)isMuted;
- (BOOL)isGroupMemberWithJid:(NSString *)jid;
- (BOOL)hasTopic;
- (BOOL)amIAbleToAddMembers; //ZOOM-387575
- (BOOL)currentUserCanAddExternalsWithWebExternalSettings:(ZMAccountAdminWhoInviteExternalToGroupSettingInfo*)externalSettings;
- (BOOL)currentUserCanAddMember:(NSString*)toAddJid withWebExternalSettings:(ZMAccountAdminWhoInviteExternalToGroupSettingInfo*)externalSettings;
- (BOOL)currentUserCanAddMember:(NSString*)toAddJid withWebExternalSettings:(ZMAccountAdminWhoInviteExternalToGroupSettingInfo*)externalSettings error:(NSError** _Nullable)outError;
/// Returns the jids that the current user can add to this channel.
- (NSArray*)membersCurrentUserCanAddFrom:(NSArray*)jids withWebExternalSettings:(ZMAccountAdminWhoInviteExternalToGroupSettingInfo*)externalSettings;
- (NSArray*)membersCurrentUserCanAddFrom:(NSArray*)jids withWebExternalSettings:(ZMAccountAdminWhoInviteExternalToGroupSettingInfo*)externalSettings error:(NSError** _Nullable)outError;
- (NSInteger)buddyCountWithoutMail;
- (BOOL)isArchiveChannel;

- (BOOL)isSubCMC;
- (BOOL)isCMCforInstantMeetings;
- (NSString *)getMtgCardMessageId;
- (NSString *)getGroupCommonInfoParentID;

+ (BOOL)isEventChannel:(NSString*)channelId;
+ (NSUInteger)getTotalMemberCount:(NSString*)channelId;
+ (BOOL)isPublicRoom:(NSString*)sessionId;
+ (BOOL)isUniversalChannelByMio:(NSString*)sessionId;
+ (BOOL)isGroupID:(NSString*)groupID;
+ (NSArray<ZMBuddyAdapter*>*)orderedMemberListInSession:(nullable NSString*)sessionID;
+ (NSArray<ZMBuddyAdapter*>*)orderedMemberListInSession:(nullable NSString*)sessionID isNeedRefresh:(BOOL)isNeedRefresh;
+ (NSArray<ZMBuddyAdapter*>*)orderedMemberWithSortExternalUsersToEnd:(nullable NSString*)sessionID;
+ (BOOL)isPersistentMeetingGroupChannel:(NSString*)channelId;
+ (BOOL)isPMCExistRealMessage:(NSString *)channelId;
+ (BOOL)isPMCDisableChat:(NSString *)channelId;
+ (BOOL)isDisableExternalByAccLevel:(NSString *)channelID;
+ (BOOL)isSubCMCBySessionId:(NSString *)sessionID;
+ (NSString*)getMainSessionIDBySubId:(NSString *)channelId;
+ (NSString*)getThreadIDBySubId:(NSString *)channelId;
//ZOOM-426762
- (BOOL)isSpotChannel;
- (NSString*)getSpotId;
- (BOOL)amIInSpot;
- (BOOL)isSpotDisabled;
- (BOOL)isHuddleBindChannel;
- (BOOL)isHuddleActiveAlert;
- (BOOL)IsChannelHuddleDisable;
@property (nonatomic, readonly, assign) BOOL canSendAtAll;
- (NSString* _Nullable)sharedSpaceID;
- (BOOL)isSyncedSharedSpaceChannel;
- (BOOL)isSharedSpaceGeneralChannel;
/// True iff the channel is in a shared space the current user is in
- (BOOL)isSharedSpaceChannel;
/// True iff the channel is in ANY shared space, even one the current user doesn't have access to
- (BOOL)isInAnySharedSpace;
/// The channel title (simliar to `m_gName`) that should be shown when the user can see context about the
/// shared space that the channel is in.
- (NSString*)titleWhenUserHasSharedSpaceContext;

- (BOOL)isZCCChannel;
- (BOOL)isZCCQueueMember:(NSString *)jid;

- (void)updateNameTo:(NSString*)name;
- (void)updateCustomChannelIcon;
+ (BOOL)isCQMemberInCQChannel:(NSString*)jid sessionID:(NSString*)sessionID;

+ (NSString *)sessionName:(NSString *)sessionId;

- (BOOL)isWorkflowBlocked;

@property (nonatomic, readonly) BOOL amISameOrgWithOwner;

- (BOOL)hasExternalMembers;

- (void)updateNewlyJoinedStatus;

@end

NS_ASSUME_NONNULL_END

