//
//  ZMSTUserStatusProtocol.h
//  ZoomSetting
//
//  Created by Francis Zhuo on 2025/3/3.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@protocol ZMSTUserStatusProtocol <NSObject>
@optional
- (void)onUserZoomAssistantOptionChanged;
- (void)onUserZoomAssistantMeetingOptionChanged;
- (void)onUserZoomAssistantPhoneOptionChanged;
- (void)onUserZoomAssistantPhoneHideOptionChanged;
@end

NS_ASSUME_NONNULL_END
