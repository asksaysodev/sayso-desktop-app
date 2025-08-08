//
//  IZMHomeTransferMeetingBannerItem.h
//  PTUIBridge
//
//  Created by Slevin Zhang on 2025/4/25.
//

#ifndef IZMHomeTransferMeetingBannerItem_h
#define IZMHomeTransferMeetingBannerItem_h

@class ZMBannerItem;
@protocol IZMHomeTransferMeetingBannerItem <ZMRoutableObject>

- (ZMBannerItem *)item;
- (void)meetingStateChange;
- (void)setTransferMeetings:(NSArray *)meetings;

@end

#endif
