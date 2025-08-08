//
//  ZMScheduleMeetingCardView.h
//  ChatUI
//
//  Created by simon shang on 2022/5/20.
//  Copyright © 2022 Zoom. All rights reserved.
//

#import <ZoomKit/ZoomKit.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMScheduleMeetingInfo;
@class ZMScheduleMeetingCardView;

@protocol ZMScheduleMeetingCardViewDelegate <NSObject>

- (void)scheduleMeetingCardView:(ZMScheduleMeetingCardView *)scheduleMeetingCardView joinMeeting:(NSString *)sessionId meetingNumber:(long)meetingNumber password:(NSString *)password;
- (void)scheduleMeetingCardView:(ZMScheduleMeetingCardView *)scheduleMeetingCardView fromSession:(NSString *)sessionId openCMCchat:(NSString *)subCMCSessionId;
- (void)scheduleMeetingCardView:(ZMScheduleMeetingCardView *)scheduleMeetingCardView viewInCalendar:(NSString *)calendarLink;
- (void)scheduleMeetingCardView:(ZMScheduleMeetingCardView *)scheduleMeetingCardView copyInvitation:(NSString *)joinURL;
- (void)scheduleMeetingCardView:(ZMScheduleMeetingCardView *)scheduleMeetingCardView editMeeting:(NSString *)sessionId meetingNumber:(long)meetingNumber occurrenceTime:(long)occurrenceTime meetingMasterEventId:(NSString*)meetingMasterEventId;
- (void)scheduleMeetingCardView:(ZMScheduleMeetingCardView *)scheduleMeetingCardView deleteMeeting:(NSString *)sessionId meetingNumber:(long)meetingNumber occurrenceTime:(long)occurrenceTime meetingMasterEventId:(NSString*)meetingMasterEventId;
- (void)scheduleMeetingCardView:(ZMScheduleMeetingCardView *)scheduleMeetingCardView joinFromARoom:(NSString *)sessionId meetingNumber:(long)meetingNumber;
- (void)scheduleMeetingCardView:(ZMScheduleMeetingCardView *)scheduleMeetingCardView moreParticipantsClicked:(ZMScheduleMeetingInfo *)meetingInfo;
- (BOOL)scheduleMeetingCardView:(ZMScheduleMeetingCardView *)scheduleMeetingCardView isInMeeting:(long)meetingNumber;

@end

typedef NS_ENUM(NSUInteger, ZMScheduleMeetingCardViewDisplayContext) {
    ZMScheduleMeetingCardViewDisplayContextDefault = 0,
    ZMScheduleMeetingCardViewDisplayContextAppTemplate = 1,
};

@interface ZMScheduleMeetingCardView : ZMBaseView

@property (nonatomic, retain) ZMScheduleMeetingInfo *scheduleMeetingInfo;

@property (nonatomic, weak) id<ZMScheduleMeetingCardViewDelegate> scheduleMeetingCardViewDelegate;

@property (nonatomic, assign) BOOL showsEventCardOnClick;

- (instancetype)initWithFrame:(NSRect)frameRect displayContext:(ZMScheduleMeetingCardViewDisplayContext)displayContext NS_DESIGNATED_INITIALIZER;

- (void)updateUI;

- (CGFloat)calculateHeightWithWidth:(CGFloat)width;

@end

NS_ASSUME_NONNULL_END
