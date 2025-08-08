//
//  ZMMeetingEventProtocol.h
//  zChatComponent
//
//  Created by groot.ding on 6/30/23.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@protocol ZMMeetingEventListen <ZMRoutableObject>

- (void)addListenMeetingStateChangeByTarget:(id)target completion:(ZMCallbackBlock)completion;

- (void)addListenMeetingLeaveByTarget:(id)target completion:(ZMCallbackBlock)completion;

- (void)removeAllListenByTarget:(id)target;

@end

NS_ASSUME_NONNULL_END
