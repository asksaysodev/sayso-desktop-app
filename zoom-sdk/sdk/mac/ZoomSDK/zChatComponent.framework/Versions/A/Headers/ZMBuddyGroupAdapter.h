//
//  ZMBuddyGroupAdapter.h
//  Zoom
//
//  Created by Justin Fang on 4/13/14.
//  Copyright (c) 2013 Zoom Video Communications, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#if defined(__cplusplus)
#include "zPTApp/SaasBeePTAppInterface.h"
#import <dataModule/zDataConstants.h>
#endif

@class ZMBuddyAdapter;
@interface ZMBuddyGroupAdapter : NSObject

@property (nonatomic, retain) NSString* m_gID;
@property (nonatomic, retain) NSString* m_gName;
@property (nonatomic, copy) NSString* assistantId; // actually it's  boss's Id
@property (nonatomic, copy) NSString* xmppID;
@property (nonatomic, retain) NSMutableArray*  m_buddies;
@property (nonatomic, retain) NSArray*  buddyJids;
@property (nonatomic, assign) BOOL m_canEdit;
#if defined(__cplusplus)
@property (nonatomic, assign) BuddyGroupType m_type;
@property (nonatomic, assign) BuddyGroupMemberCountMode memberCountMode;
#endif
@property (nonatomic, assign) BOOL isDirectoryGroup;
@property (nonatomic, assign) NSInteger totalMembersCount;
@property (nonatomic, assign) NSInteger fuzzyMembersCount;
@property (readonly) BOOL isCustomGroup;

#if defined(__cplusplus)
- (id)initWithBuddyGroup:(NS_ZOOM_MESSAGER::IZoomBuddyGroup*)buddyGroup;
#endif

- (NSString*)getGroupID;
- (NSString*)getGroupName;
- (NSUInteger)getBuddyCount;
- (ZMBuddyAdapter*)getBuddyAt:(NSUInteger)index;
- (NSArray*)getBuddies;
- (NSUInteger)getBuddyCountWithoutMyself;
- (NSArray*)getBuddiesWithoutMyself;
- (BOOL)isContainBuddy:(NSString *)jid;
@end
