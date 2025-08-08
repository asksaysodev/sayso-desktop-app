//
//  ZMSTPolicyUpdateProtocol.h
//  ZoomSetting
//
//  Created by Francis Zhuo on 2025/2/14.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@protocol ZMSTPolicyUpdateProtocol <NSObject>
- (void)policyUpdate:(zpref::zPolicyId)policy;
@end

NS_ASSUME_NONNULL_END
