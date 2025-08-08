//
//  ZMSettingConstants.h
//  ZoomSetting
//
//  Created by Francis Zhuo on 2024/3/25.
//

#import <Foundation/Foundation.h>

#ifndef ZMSettingConstants_h
#define ZMSettingConstants_h

//setting identifier
extern NSString * const kZMSettingItemIdentifierGeneral;
extern NSString * const kZMSettingItemIdentifierAudio;
extern NSString * const kZMSettingItemIdentifierVideo;
extern NSString * const kZMSettingItemIdentifierVirtual;
extern NSString * const kZMSettingItemIdentifierRecord;
extern NSString * const kZMSettingItemIdentifierAccount;
extern NSString * const kZMSettingItemIdentifierStatics;
extern NSString * const kZMSettingItemIdentifierFeedback;
extern NSString * const kZMSettingItemIdentifierPhone;
extern NSString * const kZMSettingItemIdentifierAccessibility;
extern NSString * const kZMSettingItemIdentifierNotification NS_SWIFT_NAME(kZMSettingItemIdentifierNotifications);
extern NSString * const kZMSettingItemIdentifierShortcuts;
extern NSString * const kZMSettingItemIdentifierShare;
extern NSString * const kZMSettingItemIdentifierAutoDnd; // ZOOM-285618
extern NSString * const kZMSettingItemIdentifierZoomAssistant;
extern NSString * const kZMSettingItemIdentifierApps;
extern NSString * const kZMSettingItemIdentifierManageTabNotes;
extern NSString * const kZMSettingItemIdentifierPrivacy; // ZOOM-257741
extern NSString * const kZMSettingItemIdentifierHuddles; // ZOOM-571440
extern NSString * const kZMSettingItemIdentifierAdmin; // ZOOM-736830
extern NSString * const kZMSettingItemIdentifierRingtone;

extern NSString * const kZMSettingItemIdentifier_General;
extern NSString * const kZMSettingItemIdentifier_Audio;
extern NSString * const kZMSettingItemIdentifier_Notification;
extern NSString * const kZMSettingItemIdentifier_Meeting;
extern NSString * const kZMSettingItemIdentifier_Recording;
extern NSString * const kZMSettingItemIdentifier_ShareScreen;
extern NSString * const kZMSettingItemIdentifier_Phone;
extern NSString * const kZMSettingItemIdentifier_TeamChat;
extern NSString * const kZMSettingItemIdentifier_Accessibility;
extern NSString * const kZMSettingItemIdentifier_Statistic;

//setting anchor key
extern NSString *const kZMSettingAnchorKey;
typedef NSString* ZMAnchorKey;
extern ZMAnchorKey const kZMSettingAnchorGeneralAppearance;

extern NSString *const kZMSettingJumpFromMeetingKey;

extern NSString * const kZMSettingChatItemIdentifierTranslation;
#endif
