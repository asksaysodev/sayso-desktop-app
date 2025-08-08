//
//  IZMPTDirectShareMgr.h
//  PTUIBridge
//
//  Created by Walt.Li on 2025/4/2.
//

#ifndef IZMPTDirectShareMgr_h
#define IZMPTDirectShareMgr_h

@protocol IZMPTDirectShareMgr <NSObject>

//control
- (void)start;
- (BOOL)startWithPairKey:(nonnull NSString *)pairKey;
- (void)stop;
- (void)toggle;//toggles the state between started and stopped.
//status
- (BOOL)isPresenting;
- (BOOL)isPresentingInConf;
- (BOOL)isPresentingLocal;
//availability
- (BOOL)isAvailable;//whether direct share feature is available
- (BOOL)shouldHideScreenScreenButton;//for home share screen button
- (BOOL)canStart;//whehter direct share can start
//others
- (void)shareStautsChanged:(int)inStatus;//user for IZPPTAppSink
@end

#endif /* IZMPTDirectShareMgr_h */
