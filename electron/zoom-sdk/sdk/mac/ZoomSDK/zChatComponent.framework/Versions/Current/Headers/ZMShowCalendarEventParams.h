//
//  ZMShowCalendarEventParams.h
//  zChatComponent
//
//  Created by Cyan.Yang on 2025/2/17.
//

#import <Foundation/Foundation.h>
#import <zChatComponent/ZMIMCommonDefine.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMShowCalendarEventParams : NSObject

@property (nonatomic, copy) NSString *meetingID;
@property (nonatomic, assign) long long meetingNumber;
@property (nonatomic, assign) long long occurrenceTime;
@property (nonatomic, copy, nullable) NSString *meetingTopic;
@property (nonatomic, copy, nullable) NSString *masterEventID;
@property (nonatomic, copy, nullable) NSString *channelID;

@property (nonatomic, assign) ZMCalendarAppCallerProduct callerProduct;
@property (nonatomic, assign) ZMCalendarAppNavigationDestination navigationDestination;

+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;

- (instancetype)initWithMeetingID:(NSString *)meetingID
                    meetingNumber:(long long)meetingNumber
                   occurrenceTime:(long long)occurrenceTime NS_DESIGNATED_INITIALIZER;

@end

NS_ASSUME_NONNULL_END
