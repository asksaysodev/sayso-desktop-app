//
//  ZMPTAppProtocol.h
//  ZoomMsgUI
//
//  Created by Groot Ding on 2022/5/20.
//

#ifndef ZMPTAppProtocol_h
#define ZMPTAppProtocol_h

#include <zPTApp/SaasBeePTAppInterface.h>

@protocol ZMPTAppProtocol <NSObject>

- (ISBPTAppAPI *)getPTAppApi;
#ifdef ENABLE_ZAPP
- (IZappService *)getZAppApiService:(NSUInteger)runningEnv;
#endif
- (BOOL)isPTZappServiceReady;

@end

@protocol ZMConfAppProtocol <NSObject>

#ifdef ENABLE_ZAPP
- (IZappService *)getConfAppApi;
#endif
- (BOOL)isConfZappServiceReady;

@end

#ifndef ZMSharePTAppProtocol
#define ZMSharePTAppProtocol (id <ZMPTAppProtocol>)ZMSharedFor(ZMPTAppProtocol)
#endif

#endif /* ZMPTAppProtocol_h */
