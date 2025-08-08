//
//  ZMChannelOwnerSpecialSettingInfo.h
//  zChatComponent
//
//  Created by Simon Shang on 07/05/2025.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSUInteger, ZMChannelOwnerSpecialSettingSncStatus)
{
    ZMChannelOwnerSpecialSettingSncStatus_NotSet = 0,
    ZMChannelOwnerSpecialSettingSncStatus_Syncing,
    ZMChannelOwnerSpecialSettingSncStatus_Synced,
    ZMChannelOwnerSpecialSettingSncStatus_SyncFailed,
    ZMChannelOwnerSpecialSettingSncStatus_SameAccount
};

typedef NS_ENUM(NSUInteger, ZMInstantMeetingAccountSettingType)
{
    ZMInstantMeetingAccountSetting_all,
    ZMInstantMeetingAccountSetting_ownerAdmins,
    ZMInstantMeetingAccountSetting_disable
};


NS_ASSUME_NONNULL_BEGIN

@interface ZMChannelOwnerSpecialSettingInfo : NSObject

@property (nonatomic, copy) NSString *sessionId;
@property (nonatomic, copy) NSString *accountId;
@property (nonatomic, assign) NSInteger instantMeetingAccountLimit;
@property (nonatomic, assign) ZMChannelOwnerSpecialSettingSncStatus syncStatus;
@property (nonatomic, assign) ZMInstantMeetingAccountSettingType instantMeetingAccountSettingType;

@end

NS_ASSUME_NONNULL_END
