//
//  ZMPTPolicyUpdateProtocol.h
//  PTUIBridge
//
//  Created by Walt.Li on 2025/4/21.
//

NS_ASSUME_NONNULL_BEGIN

@protocol ZMPTPolicyUpdateProtocol <NSObject>
- (void)policyUpdate:(zpref::zPolicyId)policy;
@end

NS_ASSUME_NONNULL_END
