//
//  ZMChannelMeetingEvent.h
//  ChatUI
//
//  Created by simon shang on 2022/5/13.
//  Copyright © 2022 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSUInteger, ZMEventType)
{
    ZMEventType_None,
    ZMEventType_Start,
    ZMEventType_Join,
    ZMEventType_Leave,
    ZMEventType_End
};

NS_ASSUME_NONNULL_BEGIN

@interface ZMChannelMeetingEvent : NSObject

@property (nonatomic, assign) ZMEventType action;
@property (nonatomic, copy) NSString *messageId;
@property (nonatomic, copy) NSString *sessionId;
@property (nonatomic, copy) NSString *buddyId;
@property (nonatomic, copy) NSString *meetingId;
@property (nonatomic, assign) long meetingNumber;
@property (nonatomic, assign) long eventTime;
//@property (nonatomic, retain) NSArray *attendees;
@property (nonatomic, assign) BOOL isSpotMeeting;
@property (nonatomic, retain) NSArray *top7Attendees;
@property (nonatomic, assign) long attendeesCount;

@end

NS_ASSUME_NONNULL_END
