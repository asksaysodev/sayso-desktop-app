
#import <Foundation/Foundation.h>
#import <zChatComponent/ZMUserInfo.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMMentionGroup : NSObject

@property (nonatomic, retain) NSString * _Nullable channelID;

- (instancetype _Nullable) initWithName:(NSString* _Nonnull)name identifier:(NSString*_Nonnull)identifier;
- (instancetype _Nonnull) copy;
- (BOOL) addMember:(ZMUserInfo* _Nonnull)jid;
- (BOOL) isAdmin:(NSString* _Nonnull)jid;
- (NSUInteger) totalMembers;
- (void) setMembers:(NSArray<ZMUserInfo*>*_Nonnull)members;
- (NSArray<ZMUserInfo*> *_Nonnull)members;
- (NSArray<ZMUserInfo*> *) membersSortedByName;
- (ZMUserInfo* _Nullable) memberAtIndex:(NSUInteger)index;
- (BOOL) hasMemberWithJid:(NSString* _Nonnull)jid;
- (BOOL) removeMemberWithJid:(NSString* _Nonnull)jid;
- (void) removeAllMembers;
- (NSString* _Nonnull) mentionGroupID;
- (NSString* _Nonnull) name;
- (NSString* _Nonnull) details;
- (NSString* _Nonnull) owner;
- (void) setMentionGroupID:(NSString* _Nonnull)groupID;
- (void) setName:(NSString* _Nonnull)name;
- (void) setDetails:(NSString* _Nonnull)details;
- (void) setOwner:(NSString* _Nonnull)name;
- (BOOL) isValid;
- (ZMUserInfo* _Nullable)memberWithJid:(NSString* _Nonnull)jid;

@end

NS_ASSUME_NONNULL_END
