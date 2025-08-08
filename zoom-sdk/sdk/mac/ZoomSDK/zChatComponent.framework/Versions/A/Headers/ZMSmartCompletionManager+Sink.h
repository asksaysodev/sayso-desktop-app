//
//  ZMSmartCompletionManager+Sink.h
//  zChatComponent
//
//  Created by Zoro.Fu on 2023/8/18.
//

#import "ZMSmartCompletionManager.h"

NS_ASSUME_NONNULL_BEGIN

@interface ZMSmartCompletionManager ()

- (void)sentenceCompletionResponseWithRequestId:(NSString *)reqId inputString:(NSString *)inputStr outputString:(NSString *)outputStr;

@end

NS_ASSUME_NONNULL_END
