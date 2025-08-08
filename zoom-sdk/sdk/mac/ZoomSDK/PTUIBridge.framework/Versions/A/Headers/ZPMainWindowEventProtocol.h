//
//  ZPMainWindowEventProtocol.h
//  PTUIBridge
//
//  Created by fistice on 2025/2/5.
//

#ifndef ZPMainWindowEventProtocol_h
#define ZPMainWindowEventProtocol_h

@protocol ZPMainWindowEventProtocol
@optional
- (void)mainWindowWillCompress;
- (void)mainWindowDidCompress;
- (void)mainWindowWillDecompress;
- (void)mainWindowDidDecompress;
- (void)mainWindowWillClose;
- (void)mainWindowDidMiniaturize;
- (void)mainWindowDidDeminiaturize;
- (void)mainWindowDidBecomeKey;
- (void)mainWindowDidResize;

@end
#endif /* ZPMainWindowEventProtocol_h */
/*
 
 ZPMainWindowEventProtocol
 
 [ZMRouterCenter.shared registerTartget:self forProtocol:@protocol(ZPMainWindowEventProtocol)];

 [ZMRouterCenter.shared unregisterTartget:self forProtocol:@protocol(ZPMainWindowEventProtocol)];

 
 */
