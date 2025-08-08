
#import <zChatComponent/ZMMentionGroup.h>

@class ZMMentionGroupCollection;

@protocol ZMMentionGroupCollectionDelegate
@optional
- (void) mentionGroups:(ZMMentionGroupCollection*_Nonnull)groups didCreateGroup:(ZMMentionGroup*_Nullable)group;
- (void) mentionGroups:(ZMMentionGroupCollection*_Nonnull)groups didDeleteGroup:(ZMMentionGroup*_Nullable)group;
- (void) mentionGroups:(ZMMentionGroupCollection*_Nonnull)groups didUpdateGroup:(ZMMentionGroup*_Nullable)group;
- (void) mentionGroups:(ZMMentionGroupCollection*_Nonnull)groups didAddMembersToGroup:(ZMMentionGroup*_Nullable)group;
- (void) mentionGroups:(ZMMentionGroupCollection*_Nonnull)groups didRemoveMembersFromGroup:(ZMMentionGroup*_Nullable)group;
- (void) mentionGroups:(ZMMentionGroupCollection*_Nonnull)groups didAddMentionGroupElsewhere:(ZMMentionGroup*_Nullable)group; 
- (void) mentionGroups:(ZMMentionGroupCollection*_Nonnull)groups didUpdateMentionGroupElsewhere:(ZMMentionGroup*_Nullable)group;
- (void) mentionGroups:(ZMMentionGroupCollection*_Nonnull)groups didRemoveMentionGroupElsewhere:(ZMMentionGroup*_Nullable)group;
- (void) mentionGroups:(ZMMentionGroupCollection*_Nonnull)groups didAddMentionGroupMembersElsewhere:(ZMMentionGroup*_Nullable)group;
- (void) mentionGroups:(ZMMentionGroupCollection*_Nonnull)groups didRemoveMentionGroupMembersElsewhere:(ZMMentionGroup*_Nullable)group;
- (void) serverDidDeleteChannelAndAllGroupsElsewhere:(ZMMentionGroupCollection*_Nonnull)groups;
- (void) mentionGroupsDidRefreshFromDatabaseOrServer:(ZMMentionGroupCollection*_Nonnull)groups;

@end

@interface ZMMentionGroupCollection : NSObject

@property (nonatomic, retain) NSString * _Nonnull channelID;
- (instancetype _Nullable) initWithChannelName:(NSString* _Nullable)channelName channelID:(NSString* _Nullable)channelID;

/*!
 Returns the atList that should be when sending a message containing these mention groups in this ZMMentionGroupCollection.
 @discussion Needed due to the backend requirement that we also add every member of the mention group to the atArray.
 The atList arrays contain elements on type ZMMessageMentionInfo*
 */
- (NSArray* _Nonnull)atListWithGroupsForAtList:(NSArray* _Nonnull)atList;

// Private
- (void) addGroup:(ZMMentionGroup* _Nullable)group;
- (BOOL) removeGroup:(ZMMentionGroup* _Nullable) group;

// These contact the server
- (void) refresh;
- (BOOL) requestToCreateMentionGroup:(ZMMentionGroup*_Nonnull)group;
- (BOOL) requestToDeleteMentionGroup:(ZMMentionGroup*_Nonnull) group;
- (BOOL) requestToUpdateMentionGroup:(ZMMentionGroup*_Nonnull) group
                       addingMembers:(NSArray<ZMUserInfo*>* _Nullable)membersToAdd
                     removingMembers:(NSArray<ZMUserInfo*>* _Nullable)membersToRemove;
- (BOOL) requestToAddMembersToMentionGroup:(ZMMentionGroup*_Nonnull) group members:(NSArray*_Nonnull)jids;
- (BOOL) requestToRemoveMembersFromMentionGroup:(ZMMentionGroup*_Nonnull) group members:(NSArray*_Nonnull)jids;

- (ZMMentionGroup* _Nullable) groupNamed:(NSString* _Nullable) name;
- (ZMMentionGroup* _Nullable) groupByID:(NSString* _Nullable) jid;
- (BOOL) hasGroup:(ZMMentionGroup* _Nullable) group;
- (BOOL) hasGroupByName:(NSString* _Nullable) name;
- (void) removeAllGroups;
- (NSArray* _Nullable) groups;
- (NSArray* _Nullable) newSortedGroupNames;
- (void) setChannelName:(NSString* _Nullable)name;
- (NSUInteger) count;

- (void) addDelegate:(NSObject<ZMMentionGroupCollectionDelegate> *_Nonnull)delegate;
- (void) removeDelegate: (NSObject<ZMMentionGroupCollectionDelegate> *_Nonnull)delegate;
@end

@interface ZMMentionGroupCollectionMock : ZMMentionGroupCollection
@end
