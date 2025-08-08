//
//  ZMAddExternalUserInfo.h
//  zChatComponent
//
//  Created by Simon Shang on 2023/10/25.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSUInteger, ZMSncStatus)
{
    ZMNotSet = 0,
    ZMSyncing,
    ZMSynced,
    ZMSyncFailed
};

typedef NS_ENUM(NSUInteger, ZMAccountAdminWhoInviteExternalToGroupType)
{
    ZMAccountAdminWhoInviteExternalToGroupType_None,
    ZMAccountAdminWhoInviteExternalToGroupType_EveryOne,
    ZMAccountAdminWhoInviteExternalToGroupType_MemberInOrg,
    ZMAccountAdminWhoInviteExternalToGroupType_AccountOwnerAdmin
};

NS_ASSUME_NONNULL_BEGIN

@interface ZMAddExternalUserInfo : NSObject

@property (nonatomic, copy) NSString *userId;
@property (nonatomic, assign) BOOL canAddExternal;
@property (nonatomic, assign) ZMSncStatus syncStatus;
@property (nonatomic, assign) ZMAccountAdminWhoInviteExternalToGroupType whoCanInviteExternal;

@end

NS_ASSUME_NONNULL_END
