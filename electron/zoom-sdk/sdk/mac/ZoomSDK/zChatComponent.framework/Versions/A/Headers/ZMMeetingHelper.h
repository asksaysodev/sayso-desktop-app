//
//  ZMMeetingHelper.h
//  zChatComponent
//
//  Created by groot.ding on 6/29/23.
//

#import <Foundation/Foundation.h>
#import <zChatComponent/ZMIMCommonDefine.h>


NS_ASSUME_NONNULL_BEGIN

@class ZMBuddyAdapter;

@interface ZMMeetingHelper : NSObject

+ (void)startMeetingWithJid:(NSString *)jid entrance:(NSInteger)entrance;

+ (void)startMeetingWithVideo:(BOOL)video jid:(NSString *)jid entrance:(NSInteger)entrance;

+ (void)startMeetingWithVideo:(BOOL)video buddy:(ZMBuddyAdapter *)buddy eventSourceType:(ZMEvent_Source_Type)estType entrance:(NSInteger)entrance;

+ (void)inviteMeetingWithBuddy:(ZMBuddyAdapter *)buddy eventSourceType:(ZMEvent_Source_Type)estType;

+ (BOOL)defaultMeetStartWithVideo;
+ (void)setDefaultMeetStartWithVideo:(BOOL)needVideo;

+ (int)getCallStatus;

+ (BOOL)isInMeeting;

+ (void)joinMeeting:(NSString *)sessionId meetingNumber:(long)meetingNumber password:(NSString *)password;

+ (NSNumber*)GetActiveMeetingNo;

+ (NSString*)getCurrentMeetingID;

+ (BOOL)joinMeetingWithURL:(NSString*)url;

+ (NSString*)getJoinMeetingURL:(NSString*)meetingNumber;

+ (BOOL)isHideMeetEntranceForNoMeetingLicenseUser;

+ (BOOL)isNoMeetingLicenseUser;

@end

NS_ASSUME_NONNULL_END
