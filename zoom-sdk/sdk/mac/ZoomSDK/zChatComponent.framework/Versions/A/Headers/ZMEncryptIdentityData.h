//
//  ZMEncryptIdentityData.h
//  ChatUI
//
//  Created by Ben Xiao on 2022/7/29.
//  Copyright © 2022 Zoom. All rights reserved.
//

#include "zPTApp/SaasBeePTAppInterface.h"
#include "zPTKBCrypto/ZoomKBCrypto.h"

using namespace ZOOM_PT_KB_CRYPTO_NS;

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, ZMEncryptIdentityType) {
    ZMEncryptIdentityTypeEmail,
    ZMEncryptIdentityTypePhoneNum,
    ZMEncryptIdentityTypeExtensionNum,
    ZMEncryptIdentityTypeAccountDomain,
    ZMEncryptIdentityTypeAccountId,
    ZMEncryptIdentityTypeFingerPrint
};

typedef NS_ENUM(NSInteger, ZMEncryptUnReviewedType) {
    ZMEncryptUnReviewedTypeNone,
    ZMEncryptUnReviewedTypeKeyOnly,
    ZMEncryptUnReviewedTypeDeviceOnly,
    ZMEncryptUnReviewedTypeIdentityOnly,
    ZMEncryptUnReviewedTypeKeyAndDevice,
    ZMEncryptUnReviewedTypeIdentityAndOther
};

typedef NS_ENUM(NSInteger, ZMEncryptInfoType) {
    ZMEncryptInfoTypeGroup_Device,
    ZMEncryptInfoTypeGroup_Devices,
    ZMEncryptInfoTypeGroup_NewDevice,
    ZMEncryptInfoTypeGroup_OtherDevices,
    ZMEncryptInfoTypeGroup_Account,
    ZMEncryptInfoTypeGroup_History,
    ZMEncryptInfoTypeGroup_Key,
    ZMEncryptInfoTypeGroup_Keys,
    ZMEncryptInfoTypeGroup_AccountChange,
    ZMEncryptInfoTypeGroup_ReviewActivity
};

typedef enum {
    ZMEncryptMoreInfoHeadType_None,
    ZMEncryptMoreInfoHeadType_BackupKey,
    ZMEncryptMoreInfoHeadType_Other
}ZMEncryptMoreInfoHeadType;

@interface ZMEncryptBaseData : NSObject
@property (nonatomic, assign) BOOL isHover;
- (BOOL)isRemoved;
- (ZMEncryptMoreInfoHeadType)getMoreInfoHeadType;
@end

@interface ZMEncryptInfoGroupData : ZMEncryptBaseData

@property (nonatomic, copy) NSString *title;
@property (nonatomic, assign) ZMEncryptInfoType groupType;
@property (nonatomic, assign) BOOL isExpanded;

@end


@interface ZMEncryptIdentity : ZMEncryptBaseData

@property (nonatomic, assign) long long addTime;
@property (nonatomic, assign) long long removeTime;
@property (nonatomic, assign) long long updateTime;
@property (nonatomic, assign) BOOL trusted;
@property (nonatomic, assign) BOOL unreviewed;
@property (nonatomic, assign) ZMEncryptIdentityType type;

//@property (nonatomic, assign) BOOL isHistory;

- (NSString *)name;
- (NSString *)desc;
- (NSString *)capitalDesc;
- (NSImage *)icon;
- (NSString *)timeString;

- (NSString *)addedTime;
- (NSString *)removedTime;
- (NSString *)updatedTime;

@end

@interface ZMEncryptEmail : ZMEncryptIdentity

+ (instancetype)identityBy:(ZmIdentityAndDevices::EmailId)pIdentity;
@property (nonatomic, copy) NSString *email;

@end

@interface ZMEncryptPhoneNum : ZMEncryptIdentity

+ (instancetype)identityBy:(ZmIdentityAndDevices::PhoneNumberId)pIdentity;
@property (nonatomic, copy) NSString *phoneNum;

@end

@interface ZMEncryptExtensionNum : ZMEncryptIdentity

+ (instancetype)identityBy:(ZmIdentityAndDevices::PhoneExtensionId)pIdentity;
@property (nonatomic, copy) NSString *extensionNum;

@end

@interface ZMEncryptAccountDomain : ZMEncryptIdentity

+ (instancetype)identityBy:(ZmIdentityAndDevices::ADNId)pIdentity;
@property (nonatomic, copy) NSString *accountDomainName;

@end

@interface ZMEncryptAccountId : ZMEncryptIdentity

+ (instancetype)identityBy:(ZmIdentityAndDevices::AccountId)pIdentity;
@property (nonatomic, copy) NSString *accountId;

@end

@interface ZMEncryptFingerprint : ZMEncryptIdentity

+ (nullable instancetype)identityBy:(ZmIdentityAndDevices::FingerprintId)pIdentity;
@property (nonatomic, copy) NSString *fingerprint;
@property (nonatomic, assign) BOOL isAdmin;

@end


//bool is_active_escrow_device = false;
//bool ea_pending = false;
@interface ZMEncryptDevice : ZMEncryptBaseData

@property (nonatomic, copy) NSString *deviceId;
@property (nonatomic, copy) NSString *deviceName;
@property (nonatomic, assign) ZmBasicUserDeviceInfo::DeviceType type;

@property (nonatomic, assign) NSInteger nameVersion;
@property (nonatomic, assign) NSInteger seqno;
@property (nonatomic, assign) long long addedUnixtimeSeconds;
@property (nonatomic, assign) long long canAccessUnixtimeSeconds;
@property (nonatomic, assign) long long revokedUnixtimeSeconds;
@property (nonatomic, assign) BOOL active;
@property (nonatomic, assign) BOOL unreviewed;
@property (nonatomic, assign) BOOL isActiveEscrowDevice;
@property (nonatomic, assign) BOOL isPending;
@property (nonatomic, assign) BOOL isPQCEnabled;

@property (nonatomic, assign) BOOL showCheckBox;
@property (nonatomic, assign) BOOL disableCheckBox;
@property (nonatomic, assign) BOOL isThisDevice;
@property (nonatomic, assign) BOOL showAccessDate;

//@property (nonatomic, assign) BOOL timeCreateType;

@property (nonatomic, assign) BOOL checked;

+ (ZMEncryptDevice *)deviceDataBy:(ZmBasicUserDeviceInfo)pDevice;
- (void)updateDeviceBy:(ZmBasicUserDeviceInfo)pDevice;

- (NSImage *)deviceIcon;
- (NSString *)getDeviceName;
- (NSString *)getMoreInfoDeviceName;
- (NSAttributedString *)descTimeString;

- (NSString *)addedTime;
- (NSString *)removedTime;
- (NSString *)dataAccessTime;
- (void)configProvisionDevice;

@end


@interface ZMEncryptIdentityData : NSObject

@property (nonatomic, assign) NSInteger seqno;
@property (nonatomic, assign) BOOL provisioned;
@property (nonatomic, assign) BOOL hasEscrowAdmin;

@property (nonatomic, strong) ZMEncryptDevice *thisDevice;
@property (nonatomic, strong) NSArray<ZMEncryptDevice *> *otherDevices;

@property (nonatomic, strong) NSArray<ZMEncryptEmail *> *currentEmails;
@property (nonatomic, strong) NSArray<ZMEncryptPhoneNum *> *currentPhoneNums;
@property (nonatomic, strong) NSArray<ZMEncryptExtensionNum *> *currentExtensions;
@property (nonatomic, strong) ZMEncryptAccountDomain *currentAccountDomain;
@property (nonatomic, strong) ZMEncryptAccountId *currentAccountId;
@property (nonatomic, strong) ZMEncryptFingerprint *currentFingerprint;
@property (nonatomic, strong) ZMEncryptFingerprint *adminFingerprint;

@property (nonatomic, strong) NSArray<ZMEncryptEmail *> *pastEmails;
@property (nonatomic, strong) NSArray<ZMEncryptPhoneNum *> *pastPhoneNums;
@property (nonatomic, strong) NSArray<ZMEncryptExtensionNum *> *pastExtensions;
@property (nonatomic, strong) NSArray<ZMEncryptAccountDomain *> *pastAccountDomains;

@property ZmBasicEscrowAdminInfo  escrow_admin_detail_;

+ (ZMEncryptIdentityData *)identityDataBy:(ZmIdentityAndDevices)pData;

- (NSMutableArray *)getCurrentAccounts;
- (NSMutableArray *)getReviewedAccounts;
- (NSMutableArray *)getPastAccountsAndDevices;
- (NSMutableArray *)getAllActiveDevices;

- (NSMutableArray *)getAllUnReviewedDevices;
- (NSMutableArray *)getUnReviewedUnActiveDevices;
- (NSMutableArray *)getUnReviewedDevices;
- (NSMutableArray *)getReviewedDevices;
- (NSMutableArray *)getProperKeyDevices;
- (nullable NSMutableArray *)getAdminDevices;

- (NSMutableArray *)getUnReviewedPureDevices;
- (NSMutableArray *)getUnReviewedKeys;
- (NSMutableArray *)getUnReviewedAccounts;
- (NSMutableArray *)getUnReviewedPastAccounts;
- (ZMEncryptUnReviewedType)getUnReviewedType;

- (ZMEncryptAccountId *)getUnReviewedAccountIdItem;

@end


@interface ZMEncryptBackupKeyGenerateData : NSObject

@property (nonatomic, copy) NSString *backupKey;
@property (nonatomic, strong) ZMEncryptDevice *device;

+ (ZMEncryptBackupKeyGenerateData *)generateDataBy:(ZmGenerateBackupKeyResult)pData;

@end


@interface ZMEncryptBackupKeyShareData : NSObject

@property (nonatomic, assign) NSInteger seqno;
@property (nonatomic, strong) NSArray<ZMEncryptDevice *> *devices;

@property (nonatomic, strong) ZMEncryptIdentityData *identityData;

+ (ZMEncryptBackupKeyShareData *)shareDataBy:(ZmDevicesToReviewForBackupKey)pData;

@end

NS_ASSUME_NONNULL_END
