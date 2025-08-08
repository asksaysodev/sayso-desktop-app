//
//  IZMLoggingMgr.h
//  ZoomSetting
//
//  Created by Francis Zhuo on 7/11/23.
//

#import <Foundation/Foundation.h>
#import <ZoomKit/ZMRoutableObject.h>

NS_ASSUME_NONNULL_BEGIN

@protocol IZMLoggingMgr <ZMRoutableObject>
- (void)enableLoggingIfNeeded;
@end

NS_ASSUME_NONNULL_END
