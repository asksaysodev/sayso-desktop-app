//
//  ZMMemberInformationProtocol.h
//  zChatComponent
//
//

#import <Foundation/Foundation.h>

#import <zChatComponent/ZMSearchResultItem.h>

#ifndef ZMMemberInformationProtocol_h
#define ZMMemberInformationProtocol_h

NS_ASSUME_NONNULL_BEGIN

typedef NSString* ZMMemberInformationSourceType NS_TYPED_EXTENSIBLE_ENUM;

@protocol ZMMemberInformationProtocol <NSObject>
- (ZMMemberInformationSourceType)sourceType;
- (NSString*)uid;
- (NSString*)name;
- (NSUInteger)memberCount;
- (BOOL)containsUser:(NSString*)userJid;
- (NSArray<NSString*>*)memberUIDs;
- (BOOL)hasLargeMemberCount;
- (BOOL)isPendingBuddy:(NSString*)pendingId;

@optional
- (BOOL)hasExternalMembers;
- (NSArray<ZMSearchResultItem*>*)memberSearchItems;

@property (nonatomic, copy, nullable) void (^onMembersDidUpdate)(void);

- (NSString*)localSearchWithKey:(NSString*)key;
- (NSString*)webSearchWithKey:(NSString*)key;
@property (nonatomic, copy, nullable) void (^onSearchCompleted)(NSString* requestId, BOOL isRemote, NSArray<ZMSearchResultItem*>* items);
@end

NS_ASSUME_NONNULL_END

#endif /* ZMMemberInformationProtocol_h */
