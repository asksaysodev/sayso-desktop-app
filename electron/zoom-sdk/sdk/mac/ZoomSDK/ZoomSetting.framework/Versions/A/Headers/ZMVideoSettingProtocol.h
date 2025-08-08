//
//  ZMVideoSettingProtocol.h
//  ZoomSetting
//
//  Created by Javenlee Li on 2023/6/21.
//

#import <Foundation/Foundation.h>
#import <ZoomKit/ZMRoutableObject.h>
NS_ASSUME_NONNULL_BEGIN

@protocol ZMVideoSettingProtocol <ZMRoutableObject>

- (void)changeCameraDeviceId:(NSString*)deviceId deviceName:(NSString*)deviceName;

- (void)handleVideoDeviceRunStarted:(void *)renderView type:(int)videoType;
@end

NS_ASSUME_NONNULL_END
