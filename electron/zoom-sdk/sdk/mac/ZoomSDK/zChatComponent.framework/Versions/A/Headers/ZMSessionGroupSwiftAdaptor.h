//
//  ZMSessionGroupSwiftAdaptor.h
//  zChatComponent
//
//  Created by Cyan.Yang on 2024/6/27.
//

#import <Foundation/Foundation.h>
#import <zChatComponent/ZMBuddySwiftAdaptor.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMSessionGroupAdapter;

NS_SWIFT_NAME(SessionGroup)
@interface ZMSessionGroupSwiftAdaptor : NSObject

@property (nonatomic, readonly, strong) ZMSessionGroupAdapter *sessionGroup;

@property (nonatomic, readonly) NSString *name;
@property (nonatomic, readonly) NSString *groupID;

@property (nonatomic, readonly, getter=isSubCMC) BOOL subCMC;

@property (nonatomic, readonly, nullable) NSString *groupCommonInfoParentID;

+ (NSArray<ZMSessionGroupSwiftAdaptor *> *)allChannels;
+ (ZMSessionGroupSwiftAdaptor * _Nullable)channelSessionByID:(NSString *)sessionID;

+ (NSArray<ZMBuddySwiftAdaptor *> * _Nullable)orderedMembersInSessionWithID:(NSString *)sessionID NS_SWIFT_NAME(orderedMembers(inSessionWithID:));

+ (BOOL)isSubCMCBySessionId:(NSString *)sessionID;
+ (BOOL)isDMBySessionId:(NSString *)sessionID;
- (NSString *)getGroupCommonInfoParentID:(NSString *)subCMCSessionId;

+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;
- (instancetype)initWithSessionGroup:(ZMSessionGroupAdapter *)sessionGroup NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithSessionId:(NSString *)sessionId NS_DESIGNATED_INITIALIZER;

@end

NS_ASSUME_NONNULL_END
