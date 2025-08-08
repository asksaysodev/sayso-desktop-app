//
//  ZMChatAIProtocol.h
//  zChatComponent
//
//  Created by Cyan.Yang on 2025/2/27.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@protocol ZMChatAIProtocol <NSObject>

@property (nonatomic, readonly, assign) BOOL isAppLoadFinished;

- (void)forwardPayloadToAICPlus:(NSString *)payload sessionID:(NSString *)sessionID;

- (void)performAfterAppLoaded:(void(^)(void))block;

@end

#ifndef ZMSharedChatAIProtocol
#define ZMSharedChatAIProtocol ((id<ZMChatAIProtocol>) ZMSharedFor(ZMChatAIProtocol))
#endif

NS_ASSUME_NONNULL_END
