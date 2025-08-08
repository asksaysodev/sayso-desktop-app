//
//  ZMPhoneCallHelper.h
//  zChatComponent
//
//  Created by groot.ding on 6/29/23.
//

#import <Foundation/Foundation.h>
#import <ZoomUnit/ZMSipCallProtocol.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMBuddyAdapter;

@interface ZMPhoneCallHelper : NSObject

+ (BOOL)isHidePhoneCallInChat:(nonnull NSString *)jid;

+ (void)startSipCallByJid:(nonnull NSString*)jid;

+ (void)startSipCallBySipnumber:(nonnull NSString *)sipNumber;

+ (void)startSipCallByBuddy:(nonnull ZMBuddyAdapter *)buddy;

+ (BOOL)isSipEnable;

+ (BOOL)isSipCallEnable;

+ (BOOL)isInSIPCall;

+ (BOOL)hasCallingoutCall;

+ (BOOL)hasIncomingCall;

+ (BOOL)isPhoneButtonEnabled:(nonnull NSString*)jid;

+ (BOOL)isShowPhoneButton:(nonnull NSString*)jid;

+ (BOOL)isShowSMSButton:(nonnull NSString *)jid;

+ (BOOL)isBillingHourEnable;

+ (nullable id <ZMSipContactItem>)getSipContactByID:(nonnull NSString *)cloudID;

+ (nonnull NSString *)formatPhoneNumberWithCountryCode:(nonnull NSString *)country areaCode:(nonnull NSString *)areaCode andPhoneNumber:(nonnull NSString *)number;

+ (NSString *)formatUnE164PhoneNumberWithCountryCode:(NSString *)country areaCode:(NSString *)areaCode andPhoneNumber:(NSString *)number;

+ (nonnull NSArray <NSDictionary *> *)getPhoneInfoByJid:(nonnull NSString *)jid;

@end

NS_ASSUME_NONNULL_END
