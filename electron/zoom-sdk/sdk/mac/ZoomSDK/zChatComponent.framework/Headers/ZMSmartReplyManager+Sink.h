//
//  ZMSmartReplyManager+Sink.h
//  zChatComponent
//
//  Created by Zoro.Fu on 2023/7/13.
//

#import "ZMSmartReplyManager.h"

NS_ASSUME_NONNULL_BEGIN

@interface ZMSmartReplyManager (Sink)

- (void)smartReplyResponseWithReqId:(NSString *)reqId replyArr:(NSArray *)replyArr;

@end

NS_ASSUME_NONNULL_END
