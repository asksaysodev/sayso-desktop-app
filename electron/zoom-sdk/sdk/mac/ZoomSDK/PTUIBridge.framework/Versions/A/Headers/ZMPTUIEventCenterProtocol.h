//
//  ZMPTUIEventCenterProtocol.h
//  PTUIBridge
//
//  Created by fistice on 2025/2/5.
//

#ifndef ZMPTUIEventCenterProtocol_h
#define ZMPTUIEventCenterProtocol_h

@protocol ZMPTUIEventCenterProtocol <NSObject>
@optional
- (void)onMachineWillSleep;
- (void)onMachineDidWake;
@end
#endif /* ZMPTUIEventCenterProtocol_h */
