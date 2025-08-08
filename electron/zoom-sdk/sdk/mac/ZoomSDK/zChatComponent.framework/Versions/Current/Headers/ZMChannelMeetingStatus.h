//
//  ZMChannelMeetingStatus.h
//  ChatUI
//
//  Created by simon shang on 2022/6/1.
//  Copyright © 2022 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSUInteger, ZMStatusType)
{
    ZMStatusType_None = 0,
    ZMStatusType_Started = 1,
    ZMStatusType_Finished = 2,
    ZMStatusType_Changed = 4,
    ZMStatusType_Canceled = 8
};

typedef NS_ENUM(NSUInteger, ZMEditRecurringType)
{
    ZMEditRecurringType_None = 0,
    ZMEditRecurringType_Single,
    ZMEditRecurringType_All
};

NS_ASSUME_NONNULL_BEGIN

@interface ZMChannelMeetingStatus : NSObject

@property (nonatomic, assign) ZMStatusType statusType;
@property (nonatomic, copy) NSString *messageId;
@property (nonatomic, copy) NSString *sessionId;
@property (nonatomic, assign) long meetingNumber;
@property (nonatomic, assign) long startedTime;
@property (nonatomic, copy) NSString *topic;
@property (nonatomic, assign) long startTime;
@property (nonatomic, assign) long endTime;
@property (nonatomic, assign) long occurrence_time;
@property (nonatomic, assign) ZMEditRecurringType editRecurringType;
@property (nonatomic, assign) BOOL isActiveChange;
@property (nonatomic, assign) BOOL isActive;

@end

NS_ASSUME_NONNULL_END
