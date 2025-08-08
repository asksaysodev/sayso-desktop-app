//
//  ZMContactsInfo.h
//  zChatUI
//
//  Created by groot.ding on 2018/5/2.
//  Copyright © 2018 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "dataModule/zDataConstants.h"
#import <ZoomUnit/ZMSipContactItemProtocol.h>

typedef NS_ENUM(NSInteger, ZMContactsGroupType)
{
    ZMContactsGroupType_all,
    ZMContactsGroupType_favorites,
    ZMContactsGroupType_custom,
    ZMContactsGroupType_external,
    ZMContactsGroupType_vipGroup,
    ZMContactsGroupType_request,
    ZMContactsGroupType_autoAnswer,
    ZMContactsGroupType_apps,
    ZMContactsGroupType_rooms,
    ZMContactsGroupType_deskPhones,
    ZMContactsGroupType_roomDevices,
    ZMContactsGroupType_company,
    ZMContactsGroupType_syncOn,
    ZMContactsGroupType_phone,
    ZMContactsGroupType_personalContact,
    ZMContactsGroupType_cloudContact,
	ZMContactsGroupType_assignedgroup,
    ZMContactsGroupType_fecc,
    ZMContactsGroupType_feac,
    ZMContactsGroupType_CallQueue,
    ZMContactsGroupType_SharedLineGroup,
    ZMContactsGroupType_AutoReceptionist,
    ZMContactsGroupType_CommonAreaPhone,
};

typedef NS_ENUM(NSInteger, ZMContactsSessionGroupType)
{
    ZMContactsSessionGroup_None,
    ZMContactsSessionGroup_MyContacts,
    ZMContactsSessionGroup_AssignedGroups,
    ZMContactsSessionGroup_ZPEntities,
    ZMContactsSessionGroup_AllContacts,
};

@class ZMBuddyAdapter;
@class ZMSubscribeRequestAdapter;
@class ZMSipContactItem;

@interface ZMContactsInfo : NSObject

@property (retain) ZMBuddyAdapter* buddy;

@property (retain) id <ZMSipContactItem> cloudBuddy;

@property (nonatomic,assign) ZMContactsGroupType groupType;

@property (nonatomic,copy) NSString *buddyGroupId;

@property (nonatomic,assign) BOOL isHoverSelected;

- (id)initWithBuddy:(ZMBuddyAdapter*)buddy;
- (id)initWithCloudBuddy:(id <ZMSipContactItem>)cloudBuddy;

@end

@interface ZMContactsGroupInfo : NSObject

@property (nonatomic,copy) NSString* name;
@property (nonatomic,copy) NSString* groupID;
@property (nonatomic,copy) NSString* xmppGroupID;
@property (nonatomic,copy) NSString* indexGroupID;
@property (nonatomic,assign) ZMContactsGroupType type;
@property (nonatomic,assign) NSInteger contactsCount;
@property (nonatomic,assign) NSInteger fuzzyContactsCount;
@property (nonatomic,assign) NSInteger unreadCount;
@property (nonatomic,assign) BOOL isExpand;
@property (nonatomic,assign) BOOL isGroupSection;
@property (nonatomic,readonly) BOOL isCustomGroup;
@property (nonatomic,assign) BOOL isHoverSelected;
@property (nonatomic,assign) ZMContactsSessionGroupType sessionType;
@property (nonatomic,assign) BuddyGroupMemberCountMode memberCountMode;
@property (nonatomic,assign) BOOL syncAccurateMemberCountError;
@property (nonatomic,assign) BOOL isRefreshingAccurateMemberCount;
//used in show cloud contact subcatalog
@property (nonatomic,assign) BOOL isSubcatalog;
@property (nonatomic,weak) NSTreeNode *node;

@property (nonatomic, assign) NSInteger totalMembersCount;

@end
