//
//  ZMBuddyAdapter.h
//  Zoom
//
//  Created by Deal Shao on 9/25/13.
//  Copyright (c) 2013 Zoom Video Communications, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <zChatComponent/ZMIMCommonDefine.h>

#if defined(__cplusplus)
#include <cmmlib/CmmString.h>
#import <zPTApp/SaasBeePTAppInterface.h>
#endif

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, ZMBuddyUpdateState) {
    ZMBuddyUpdateNone                       = 0,
    ZMBuddyUpdateName                       = 1<<1,
    ZMBuddyUpdateEmail                      = 1<<2,
    ZMBuddyUpdatePhone                      = 1<<3,
    ZMBuddyUpdateAvatar                     = 1<<4,
    ZMBuddyUpdatePresence                   = 1<<5,
    ZMBuddyUpdatePersonNote                 = 1<<6,
    ZMBuddyUpdateSipPhone                   = 1<<7,
    ZMBuddyUpdateStatus                     = 1<<8,
    ZMBuddyUpdateDeactivatedOrTerminated    = 1<<9,
    ZMBuddyUpdateOthers                     = 1<<10,
    ZMBuddyUpdateManager                    = 1<<11,
    ZMBuddyUpdateSipDirectNumber            = 1<<12,
    ZMBuddyUpdateHuddleStatus               = 1<<13,
};

typedef NS_ENUM(NSUInteger, ZMWorkLocation)
{
    ZMTurnOff = 0,
    ZMInOffice,
    ZMRemote
};

@class ZMRoomCalloutDevice;
@class ZMUserSignatureData;

@interface ZMMTBuddyModel : NSObject

@property (nonatomic, assign) BOOL isInMeeting;
@property (nonatomic, copy, nullable) NSString *userGuid;
@property (nonatomic, copy, nonnull) NSString *userJid;
@property (nonatomic, copy) NSString *displayName;
@property (nonatomic, assign) NSInteger joinIndex;

#if defined(__cplusplus)
+ (nullable ZMMTBuddyModel *)modelWithBuddy:(NS_ZOOM_MESSAGER::MeetBuddyInfo)buddy;
#endif
- (BOOL)isConfirmToSearchKey:(NSString*)searchKey;

@end

@interface ZMBuddyAdapter : NSObject

@property (nonatomic, retain) NSString* m_jID;
@property (nonatomic, retain) NSString* m_buddyID; //ZOOM-421897
@property (nonatomic, retain) NSString* m_accountID; 
@property (nonatomic, retain) NSString* m_phoneNumber;
@property (nonatomic, retain) NSString* m_screenName;
@property (nonatomic, retain) NSString* m_pronoun;
@property (nonatomic, retain) NSString* m_firstName;
@property (nonatomic, retain) NSString* m_lastName;
@property (nonatomic, retain) NSString* m_email;
@property (nonatomic, retain) NSString* m_department;
@property (nonatomic, retain) NSString* m_jobTitle;
@property (nonatomic, retain) NSString* m_timezoneId;
@property (nonatomic, retain) NSString* m_location;
@property (nonatomic, retain) NSString* m_companyName;
@property (nonatomic, retain) NSString* m_mobile;
@property (nonatomic, retain, nullable) NSString* m_localPicturePath;
@property (nonatomic, retain, nullable) NSString* m_localBigPicturePath;
@property (nonatomic, retain) NSString* m_managerName;
@property (nonatomic, retain) NSString* m_managerjID;
@property (nonatomic, retain) NSString* customMessage;
@property (nonatomic, retain) NSString* vanityUrl;
@property (nonatomic, retain) NSString* countryCode;
@property (nonatomic, retain) NSString* noCountryCodePhoneNumber;
@property (nonatomic, retain) NSString* sipPhoneNumber;
@property (nonatomic, retain, nullable) NSArray *m_additionalPhoneNumbers;
@property (nonatomic, assign) BOOL m_hasAdditionalPhoneNumbers;
@property (nonatomic, retain) NSArray *m_additionalCompletePhoneNumbers; //profile number with countryCode
@property (nonatomic, retain) NSMutableArray *m_additionalCompletePhoneNumbersWithoutLabel; //profile number with countryCode,do not have label
@property (nonatomic, assign) BOOL m_isPictureDownloaded;
/// You sent a contact request to the user, and your request haven't accepted yet. This pendding is global
@property (nonatomic, assign) BOOL isPending;
/// the user can be anyone(evan your contact or not). the user was invited to join this channel from you,  but hasn't joined yet. So. the user is now pending in this channel and should show in the channel's memberList
@property (nonatomic, copy) NSString *pendingUserID;
@property (nonatomic, assign) BOOL isAvailable;
@property (nonatomic, assign) BOOL isNonFriend;
@property (nonatomic, assign) NSInteger  imPresence;
#if defined(__cplusplus)
@property (nonatomic, assign) ZoomIMPresenceStatus  imPresenceStatus;
#endif
@property (nonatomic, assign) BOOL isMobileOnline;
@property (nonatomic, assign) BOOL isDesktopOnline;
@property (nonatomic, assign) BOOL isBuddy;
@property (nonatomic, assign) BOOL isSupportE2E;
@property (nonatomic, assign) BOOL isZoomRoom;
@property (nonatomic, assign) BOOL isRobot;
@property (nonatomic, assign) BOOL isDeactivated;
@property (nonatomic, assign) BOOL isTerminated;
@property (nonatomic, assign) BOOL isBlocked;
@property (nonatomic, assign) BOOL isRoomDevice;
@property (nonatomic, assign) BOOL isSharedGlobalUser;
@property (nonatomic, assign) BOOL isPersonalContact;
@property (nonatomic, assign) BOOL isZoomPhoneAppliances;
@property (nonatomic, assign) BOOL isAddToVipGroup;
@property (nonatomic, assign) BOOL isZPEntities;
@property (nonatomic, assign, readonly) BOOL isAuditRobot;
@property (nonatomic, assign, readonly) BOOL isSystemApp;
@property (nonatomic, assign) BOOL IsMeetingGuest;// meeting guest user
@property (nonatomic, retain) ZMRoomCalloutDevice *roomDeviceInfo;
@property (nonatomic, copy) NSString *robotCmdPrefix;
@property (nonatomic, copy) NSString *statusString;
@property (nonatomic, copy) NSString *introductionString;
@property (retain) NSMutableArray* directNumbers;
@property (retain) NSMutableArray* cloudPhoneNumbers;
@property (retain) NSMutableArray* cloudCustomsNumbers;
@property (copy, nullable) NSString*  extension;
@property (copy, nullable) NSString*  companyNumber;
@property (nonatomic, assign) BOOL isSameAccountContact;// same company & same site
@property (nonatomic, assign) BOOL isNotSameAccountContact;
@property (nonatomic, assign) BOOL isSameOrgContact;// same company & maybe not same site
@property (assign) BOOL isPresenceSynced;
@property (assign) BOOL canChat;
@property (assign) ZMIMBuddyPresenceType presenceType;
@property (copy) NSDate* lastMessageTime;
@property (nonatomic,readonly) NSInteger lastUpdateState;
@property (assign) NSInteger lastMatchScore;

@property (assign, readonly) BOOL isAvailableAlert;//ZOOM-65418

@property (nonatomic,copy) NSString *profilePhoneNumber;
@property (nonatomic,copy) NSString *profileCountryCode;
#if defined(__cplusplus)
@property (nonatomic,assign) ssb_xmpp::ZoomBuddyGroupBuddyType buddyType;
#endif
@property (nonatomic, readonly) BOOL isAADContact;

@property (nonatomic,assign) BOOL isIMBlockedByIB;
@property (nonatomic,assign) BOOL isMeetingBlockedByIB;
@property (nonatomic,assign) BOOL isPhoneCallBlockedByIB;
@property (nonatomic,assign) BOOL isSMSBlockedByIB;
@property (nonatomic,assign) BOOL isDisableChatAndMeeting;

@property (nonatomic,assign) BOOL isReallyExternalContact;// if other side is in same org, return YES, otherwise return NO (this property only for user, not include ZoomRoom and ZoomDevice)
@property (nonatomic,assign) BOOL isContactCanChat;// if other side is your friend who belongs to different org Or is in same org, return YES, otherwise return NO (this property only for user, not include ZoomRoom and ZoomDevice)
@property (nonatomic,assign) BOOL isNotRealBuddy;
@property (nonatomic,assign) BOOL isShowInClientDirectory;


@property (nonatomic, nullable, readonly) ZMUserSignatureData *signatureData;
@property (nonatomic, nullable, readonly) NSString *signatureContent;
@property (nonatomic, nullable, readonly) NSAttributedString *calculatedSignatureContent;
@property (nonatomic, readonly) BOOL isEnableSignatureReminder;
@property (nonatomic, readonly) BOOL isSignatureOutOfDate;
@property (nonatomic, readonly) BOOL isSignatureAsClosedReminder;// if the customer has closed signature reminder for this buddy(include myself)

@property (nonatomic, assign) NSInteger resourceType; //[ZOOM-288416] Check whether desktop or mobile login

@property (nonatomic, copy) NSString *phoneNumberType;
#if defined(__cplusplus)
@property (nonatomic, assign) zoom_sipcall_app::CmmSIPPhoneNumberType matchedNumberType;
#endif
@property (nonatomic, assign) BOOL isFromPAB;

@property (nonatomic, readonly) BOOL isMyBot;

@property (nonatomic, readonly) ZMWorkLocation workLocation;
@property (nonatomic, assign) BOOL isJoinedHuddle;
@property (nonatomic, copy) NSString *huddleName;
@property (nonatomic, copy) NSString *huddleDeepLink;

@property (nonatomic, copy) NSString *sessionId;

+ (instancetype)myself;

- (BOOL)isMyself;
#if defined(__cplusplus)
- (id)initWithZoomBuddy:(NS_ZOOM_MESSAGER::IZoomBuddy*)buddy;
- (void)updateWithBuddy:(NS_ZOOM_MESSAGER::IZoomBuddy*)buddy;
#endif
- (BOOL)isConformToSearchKey:(NSString*)searchKey;
- (void)update;
- (BOOL)updatePrensence;
- (BOOL)updateHuddleStatus;
- (void)updateInformationBarrier;
- (void)cleanLastUpdateState;
- (void)refreshVCard:(BOOL)bRightNow;
- (void)forceRefreshVCard:(BOOL)bRightNow;
- (void)updateLastMatchScore;
- (NSString*)buddyDeactivatedStatusString;
- (NSString*)buddyStatusString;
- (void)updateAccountState;
- (BOOL)isChatDisable;
- (BOOL)isCanNotOpenSession;
- (BOOL)isMeetDisable;
- (BOOL)isShowPresence;
- (BOOL)canCallByExtension;
- (void)updateSignatureAsClosedReminder;
- (void)updateSignatureOutOfDate;
#if defined(__cplusplus)
- (void)getSipDirectNumbers:(std::vector<Cmm::CString>)numbers;
#endif
- (NSString*)getVoiceString;
- (BOOL)isLinkedSameOrg;
- (BOOL)isBuddyCanChatAndMeeting;
@end

NS_ASSUME_NONNULL_END
