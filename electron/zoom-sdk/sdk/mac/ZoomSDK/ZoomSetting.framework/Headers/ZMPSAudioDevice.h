//
//  ZMPSAudioDevice.h
//  ZoomSetting
//
//  Created by Michael Lin on 2024/1/11.
//

#import <Foundation/Foundation.h>
#ifdef __cplusplus
#import <zPSApp/define/PSDefine.h>
#endif
#import <ZoomSetting/ZMPSDevice.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMPSAudioDevice: ZMPSDevice

#ifdef __cplusplus
- (nullable instancetype)initWithAudioDevice:(const ps::PSAudioDevice&)device scene:(ZMPSScene)scene;
#endif

@end

NS_ASSUME_NONNULL_END
