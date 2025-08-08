//
//  ZMScheduleMeetingInfo.h
//  ChatUI
//
//  Created by simon shang on 2022/5/7.
//  Copyright © 2022 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, ZMScheduleMeetingStatus)
{
    ZMScheduleMeetingStatusNotStarted = 0,
    ZMScheduleMeetingStatusStarted = 1,
    ZMScheduleMeetingStatusFinished = 2,
    ZMScheduleMeetingStatusChanged = 4,
    ZMScheduleMeetingStatusFinishedAndChanged = 6,
    ZMScheduleMeetingStatusCanceled = 8,
    ZMScheduleMeetingStatusCountdown = 16,
    ZMScheduleMeetingStatusBotFinished = 32,
    ZMScheduleMeetingStatusInstantStarted = 100,
    ZMScheduleMeetingStatusInstantEnded = 101
};

@interface ZMScheduleMeetingInfo : NSObject

@property (nonatomic, copy) NSString *id;
@property (nonatomic, copy) NSString *cmcChannelId;
@property (nonatomic, copy) NSString *hostId;
@property (nonatomic, copy) NSString *accountId;
@property (nonatomic, assign) long number;
@property (nonatomic, copy) NSString *topic;
@property (nonatomic, copy) NSString *password;
@property (nonatomic, assign) NSInteger status;// 0 - not started, 1 - started, 2 - finished , 4- changed, 8-cancelled
@property (nonatomic, assign) NSInteger type;// 0 - pre-schedule, 1 - instant, 2 - schedule, 3 - repeat
@property (nonatomic, assign) long realDuration;
@property (nonatomic, assign) long startTime;
@property (nonatomic, assign) long endTime;
@property (nonatomic, copy) NSString *timezone;
@property (nonatomic, assign) NSInteger options;
@property (nonatomic, assign) long original_mtg_number;
@property (nonatomic, assign) long originalFrom;
@property (nonatomic, assign) long scheduleTime;
@property (nonatomic, assign) long modifyTime;
@property (nonatomic, assign) NSInteger meetingType;
@property (nonatomic, copy) NSString *sessionId;
@property (nonatomic, copy) NSString *joinURL;
@property (nonatomic, copy) NSString *messageId;
@property (nonatomic, assign) long startedTime;
@property (nonatomic, assign) long occurrenceTime;
@property (nonatomic, copy) NSString *calendarEventLink;
@property (nonatomic, copy) NSString *meetingMasterEventId;
@property (nonatomic, retain, nullable) NSArray *attendees;// total participants during the meeting
@property (nonatomic, assign) long realStartTime;// PMC meeting start time
@property (nonatomic, assign) long realEndTime;// PMC meeting end time
@property (nonatomic, assign) long recurring_end_time;
@property (nonatomic, copy) NSString *recurring_interval_type;
@property (nonatomic, copy) NSString *endType;
@property (nonatomic, assign) long occurrences_count;
@property (nonatomic, assign) long interval_length;
@property (nonatomic, retain) NSArray *day_of_interval;
@property (nonatomic, assign) long month_date;
@property (nonatomic, assign) long week_of_interval;
@property (nonatomic, assign) BOOL isOccurrenceInfoEmpty;
@property (nonatomic, assign) NSUInteger attendee_count;// total participants number of the ended meeting
@property (nonatomic, retain, nullable) NSArray *attendee_list;//  top 14 participants of the ended meeting
@property (nonatomic, assign) NSUInteger inviteeCount;
@property (nonatomic, assign) BOOL isFromMeetingList;
@property (nonatomic, retain) NSArray *dynamicTop7Attendees;// dynamic top 7 participants during the meeting
@property (nonatomic, assign) NSUInteger dynamicAttendeeCount;// dynamic total participants during the meeting
@property (nonatomic, assign) BOOL isBotCard;
@property (nonatomic, assign) BOOL isActive;
@property (nonatomic, assign) BOOL showsChatButton;

@property (nonatomic, readonly, getter=isHostedByMyself) BOOL hostedByMyself;
@property (nonatomic, readonly, getter=isInstantMeeting) BOOL instantMeeting;
@property (nonatomic, readonly, getter=isOutdated) BOOL outdated;
@property (nonatomic, readonly, assign) ZMScheduleMeetingStatus meetingStatus;
@property (nonatomic, readonly, copy) NSArray<NSString *> *attendeesInMeeting;
@property (nonatomic, readonly, assign) NSUInteger totalAttendeeCountInMeeting;

- (NSString * _Nullable)recurrenceDescription;

@end

NS_ASSUME_NONNULL_END
