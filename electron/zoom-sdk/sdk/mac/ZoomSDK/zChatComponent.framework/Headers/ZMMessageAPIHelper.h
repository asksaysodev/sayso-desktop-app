//
//  ZMMessageAPIHelper.h
//  zChatComponent
//
//  Created by Kevin Li on 2022/8/4.
//

#import <Foundation/Foundation.h>
#import <dataModule/zDataConstants.h>
#import <zChatComponent/ZMIMCommonDefine.h>
#import <zChatComponent/ZMIMDataInfo.h>
#import <zChatComponent/ZMMessageAPIHelper.h>

NS_ASSUME_NONNULL_BEGIN


@class ZMChannelMemberSearchFilter;


@interface ZMAtEventTelemetryData : NSObject

@property (nonatomic, copy) NSString* jid;
@property (nonatomic, assign) NSInteger index;

@end


@interface ZMMessageAPIHelper : NSObject

+ (ZoomIMPresence)getMyPresence;
+ (ZoomIMPresenceStatus)getMyPresenceStatus;

+ (void)joinMeetingWithMessageID:(NSString * _Nullable)messageID sessionID:(NSString * _Nullable)sessionID;
+ (BOOL)isSharePointEnabledInChannel:(nullable NSString*)groupID;
+ (ZMFileStorageType)currentFileStorageType;
+ (BOOL)isSharePointUser;
+ (BOOL)isSharePointOrBoxUser;
+ (NSInteger)getSessionFileSotrageType:(nonnull NSString*)sessionID;
+ (E2EChatOption)getMyE2EOption:(nullable NSString *)sessionID;
+ (BOOL)isE2ESelectiveModeEnabled:(nullable NSString *)sessionID;
+ (BOOL)isWhiteboardURL:(nonnull NSString*)url sessionID:(nullable NSString*)sessionID;
+ (BOOL)searchMessageFileDisable;

// ZOOM-628330 describes what are considered as internal links.
// Currently, internal links should include:
//  - Join meeting link
//  - Deeplinks such as "Link to chat message"
//  - Whiteboard links
+ (BOOL)isZoomInternalURL:(nonnull NSString *)URLString sessionID:(nullable NSString *)sessionID;

+ (BOOL)isValidJoinMeetingLink:(nonnull NSString *)URLString;
+ (BOOL)isValidJoinHuddleLink:(nonnull NSString *)URLString;
+ (BOOL)isURLLinkCanJumpToTab:(nonnull NSString *)URLString;

+ (BOOL)isMessageAtChannelAllowPreview:(nonnull NSString*)sessionId withMessageID:(nonnull NSString*)messageID groupID:(nonnull NSString*)groupID;

+ (BOOL)isSessionExistByID:(nonnull NSString*)sessionID ;

+ (BOOL)sendSmartScheduleButtonCommand:(NSDictionary *)info;

+ (BOOL)trackAppActionInteraction:(NSString *)action messageID:(NSString *)messageID sessionID:(NSString *)sessionID;

+ (BOOL)isLargeChannelForMemberListOptimization:(NSString*)sessionId;
+ (NSString*)searchChannelMember:(ZMChannelMemberSearchFilter*)searchFilter;
+ (BOOL)isNotInChannelAtMentionEnabled;
+ (BOOL)isShortKeyNotInChannelSearchEnabled;
+ (BOOL)isChannelCanAddExternalByDefaultEnabled;

/// Exhaustively searches members, returning both those IN and NOT-IN a channel and exclude apps.
+ (NSString*)exhaustiveSearchMember:(ZMExhaustiveMemberSearchFilter*)filter;

+ (nullable NSArray<NSString*>*)sortChannelMemberSearchResult:(nonnull NSString*)sessionId threadId:(nullable NSString*)threadId keyword:(nullable NSString*)keyword jidArray:(nonnull NSArray<NSString*>*)jidArray hasMore:(nullable BOOL*)hasmore;

+ (nullable NSString*)searchChannelByKeyword:(nonnull NSString*)keyword;

+ (nullable NSString*)searchChannelByKeyword:(nonnull NSString*)keyword channelFilter:(ZMChannelSearchFilter*)channelFilter;

+ (nullable NSString*)getMarketplaceURLForMioSupport; //ZOOM-386910

+ (NSString*)getURLByType:(int)type;

+ (BOOL)isEnableImprovedMentionSortLogic:(NSString *)sessionId;
+ (void)sendAtEventTelemetryWithList:(nullable NSArray*)list type:(ZMMessageSrcType)srcType;
+ (nullable NSArray*)getAtMentionSortedListForSessionID:(nullable NSString*)sessionID threadID:(nullable NSString*)threadID key:(nullable NSString*)key hasMore:(nullable BOOL*)hasmore;
+ (nullable NSArray*)getAtMentionSortedListForSessionID:(nullable NSString*)sessionID threadID:(nullable NSString*)threadID key:(nullable NSString*)key includeApps:(BOOL)includeApps hasMore:(nullable BOOL*)hasMore;
+ (nullable NSArray*)getAtMentionSortedListForDM:(NSString* _Nonnull)buddyId key:(nullable NSString*)key includeApps:(BOOL)includeApps;

+ (BOOL)isEnableOutOfOffice;

+ (BOOL)isInOfflineMode;

+ (BOOL)isEnablePersonalContactEdition;

+ (BOOL)joinHuddleByURL:(NSString *)huddleUrl;

+ (BOOL)getManageAppInChannelMoreDetailLink:(NSString *_Nullable*_Nullable)moreDetailLink;

+ (NSInteger)getChannelInstantMeetingLimitMembers;
+ (BOOL)isAccountDisableChannelInstantMeeting;
+ (BOOL)isAccountChannelInstantMeetingOnlyOwnerAdmin;

+ (BOOL)getChannelOwnerSpecialSetting:(NSString *_Nonnull)channelID;

+ (BOOL)isSipPhoneEnabled;
+ (BOOL)isSipVoicemailEnabled;
+ (BOOL)queryVoiceMailItemByCallID:(NSString *)callId;

//ZOOM-670825
+ (BOOL)isMentionGroupEnhancementEnabled;
@end

NS_ASSUME_NONNULL_END
