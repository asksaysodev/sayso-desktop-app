//
//  ZMAudioSettingProtocol.h
//  ZoomSetting
//
//  Created by Javenlee Li on 2023/6/21.
//

#import <Foundation/Foundation.h>
#import <ZoomKit/ZMRoutableObject.h>

NS_ASSUME_NONNULL_BEGIN

@protocol ZMAudioSettingProtocol <ZMRoutableObject>

- (void)changeMicDeviceId:(NSString*)deviceId deviceName:(NSString*)deviceName;
- (void)changeSpeakerDeviceId:(NSString*)deviceId deviceName:(NSString*)deviceName;

- (BOOL)refreshDevice:(unsigned int)value deviceType:(unsigned int)type;

@end

NS_ASSUME_NONNULL_END
