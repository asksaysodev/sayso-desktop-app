//
//  IZMPTMeetingUpdate.h
//  PTUIBridge
//
//  Created by Slevin Zhang on 2025/5/13.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@protocol IZMPTMeetingUpdate

@optional
- (void)meetingStateChange:(NSInteger)state;
- (void)transferMeetingsListUpdated;

@end

NS_ASSUME_NONNULL_END
