//
//  IZMDeviceProtocol.h
//  ZoomUnit
//
//  Created by Javenlee.Li on 2024/8/22.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@protocol IZMDeviceProtocol <NSObject>
@optional
- (void)onDeviceStatusChanged:(int)deviceCmd param:(unsigned int)value handle:(unsigned int)handle;

@end

NS_ASSUME_NONNULL_END
