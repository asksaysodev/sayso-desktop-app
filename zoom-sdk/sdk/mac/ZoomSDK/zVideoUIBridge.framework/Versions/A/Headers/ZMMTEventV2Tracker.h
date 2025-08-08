//
//  ZMMTEventV2Tracker.h
//  VideoUIBridge
//
//  Created by vinson.wang on 2025/3/25.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMMTV2Telemetry : NSObject

@property(nonatomic, assign) e_Interaction_feature_name featureName;
@property(nonatomic, assign) e_Interaction_event_source eventSource;
@property(nonatomic, assign) e_Interaction_event_type eventType;
@property(nonatomic, assign) e_Interaction_event_location eventLocation;
@property(nonatomic, assign) e_Interaction_event_name eventName;

@property(nonatomic, assign) e_Interaction_sub_feature_name subFeatureName;
@property(nonatomic, strong, nullable) NSString * eventTraceSource;
@property(nonatomic, assign) e_meetings_v2_Interaction_event_status eventStatus;
@property(nonatomic, assign) e_meetings_v2_ShareOptionsEntity_share_content_type shareContentType;

@property(nonatomic, assign) BOOL wallpaperEntity;
@property(nonatomic, assign) BOOL shareOptionsEntity;

@end


@interface ZMMTEventV2Tracker : NSObject

+ (void)trackMeetingEvent:(ZMMTV2Telemetry *)meetingEvent;

@end

NS_ASSUME_NONNULL_END
