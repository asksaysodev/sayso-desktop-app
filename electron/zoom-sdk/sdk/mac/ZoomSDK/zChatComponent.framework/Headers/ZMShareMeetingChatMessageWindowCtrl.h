//
//  ZMShareMeetingChatMessageWindowCtrl.h
//  zChatComponent
//
//  Created by Yong Zhou on 7/7/22.
//

#import <Cocoa/Cocoa.h>

@class ZMMeetingChatCardEntity;

NS_ASSUME_NONNULL_BEGIN

@interface ZMShareMeetingChatMessageWindowCtrl : NSWindowController

+ (instancetype)sharedInstance;

+ (void)releaseInstance;

- (void)showWindowAtFrameTopLeftPoint:(NSPoint)point withMeetingInfo:(ZMMeetingChatCardEntity*)meetingEntity;

- (void)close;

@end

NS_ASSUME_NONNULL_END
