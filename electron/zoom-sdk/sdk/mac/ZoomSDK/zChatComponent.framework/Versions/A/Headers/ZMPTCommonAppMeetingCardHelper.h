//
//  ZMPTCommonAppMeetingCardHelper.h
//  zChatComponent
//
//  Created by Cyan.Yang on 2024/1/23.
//

#import <Foundation/Foundation.h>
#import <zChatComponent/ZMScheduleMeetingInfo.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, ZMCMeetingAssetsCardInteraction) {
    ZMCMeetingAssetsCardInteractionNone = 0,
    ZMCMeetingAssetsCardInteractionClickRecording = 3,
    ZMCMeetingAssetsCardInteractionClickMeetingInfo = 4,
    ZMCMeetingAssetsCardInteractionClickSummary = 5,
    ZMCMeetingAssetsCardInteractionClickShareButton = 6,
    ZMCMeetingAssetsCardInteractionClickAICMenu = 7,
    ZMCMeetingAssetsCardInteractionClickMoreButton = 8,
};

@interface ZMPTCommonAppMeetingCardHelper : NSObject

@property (nonatomic, readonly, nullable) ZMScheduleMeetingInfo *scheduleMeetingInfo;

+ (void)trackInteraction:(ZMCMeetingAssetsCardInteraction)interaction sessionID:(NSString *)sessionID messageID:(NSString *)messageID;

+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;
- (nullable instancetype)initWithMessageID:(NSString *)messageID sessionID:(NSString *)sessionID NS_DESIGNATED_INITIALIZER;

- (void)viewInCalendar:(NSString *)calendarLink;
- (void)openCMCChat:(NSString *)subCMCSessionId fromSession:(NSString *)sessionId;
- (void)copyInvitation:(NSString *)joinURL;

@end

NS_ASSUME_NONNULL_END
