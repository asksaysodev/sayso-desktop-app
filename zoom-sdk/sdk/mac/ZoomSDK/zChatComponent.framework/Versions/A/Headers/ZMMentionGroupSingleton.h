
#import <Cocoa/Cocoa.h>
#import <zChatComponent/ZMMentionGroupCollection.h>

@interface ZMMentionGroupSingleton : NSObject
+ (instancetype _Nonnull) shared;
- (ZMMentionGroupCollection* _Nonnull) addCollectionForChannelName:(NSString* _Nullable)name
                                                         channelID:(NSString* _Nonnull)channelID;
- (ZMMentionGroupCollection* _Nonnull) lookupCollectionForChannelID:(NSString* _Nonnull)channelID;
- (ZMMentionGroupCollection* _Nullable) collectionForChannelOrNull:(NSString* _Nonnull)channelID;
- (ZMMentionGroup* _Nullable) mentionGroupByID:(NSString* _Nonnull)mentionGroupID;
- (void) clear;
- (void) refresh;
- (void) initCallback;
+ (NSString* _Nonnull) shrinkTitle:(NSString* _Nonnull)name
    toWidth:(long)width
    usingFont:(NSFont* _Nonnull)font
    expressingCount:(unsigned long)count;
+ (BOOL)areMentionGroupsEnabled;
@end

extern ZMUserInfo * _Nullable userFromJid (NSString * _Nullable jid);
extern NSString * _Nonnull ZM_UTF8_to_NSString(const char * _Nullable string);

#define kServerTimeout (30)

#define kMentionGroupNotificationCreatedMyGroup @"kMentionGroupNotificationCreatedMyGroup"
#define kMentionGroupNotificationDeletedMyGroup @"kMentionGroupNotificationDeletedMyGroup"
#define kMentionGroupNotificationUpdatedMyGroup @"kMentionGroupNotificationUpdatedMyGroup"
#define kMentionGroupNotificationAddedMembersMyGroup @"kMentionGroupNotificationAddedMembersMyGroup"
#define kMentionGroupNotificationRemovedMembersMyGroup @"kMentionGroupNotificationRemovedMembersMyGroup"
#define kMentionGroupNotificationGroupCreatedElsewhere @"kMentionGroupNotificationGroupCreatedElsewhere"
#define kMentionGroupNotificationGroupDeletedElsewhere @"kMentionGroupNotificationGroupDeletedElsewhere"
#define kMentionGroupNotificationGroupUpdatedElsewhere @"kMentionGroupNotificationGroupUpdatedElsewhere"
#define kMentionGroupNotificationAddedMembersElsewhere @"kMentionGroupNotificationAddedMembersElsewhere"
#define kMentionGroupNotificationRemovedMembersElsewhere @"kMentionGroupNotificationRemovedMembersElsewhere"
#define kMentionGroupNotificationServerDeletedChannel @"kMentionGroupNotificationServerDeletedChannel"
#define kMentionGroupNotificationDataInitialized @"kMentionGroupNotificationDataInitialized"
