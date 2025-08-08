//
//  ZMPSShareStatusProtocol.h
//  ZoomSetting
//
//  Created by Michael Lin on 2024/1/11.
//

#ifndef ZMPSShareStatusProtocol_h
#define ZMPSShareStatusProtocol_h

#import <zPSApp/media/IPSShare.h>

@protocol ZMPSShareStatusProtocol <NSObject>

@optional
- (void)psOnShareCapturerStatusChanged:(NSInteger)msg
                                reason:(NSInteger)reason;
- (void)psOnMacOverlayEnabled:(BOOL)enable;
- (void)psOnWindowOutOfDeviceWithWindowID:(CGWindowID)windowID active:(BOOL)active;

@end

#endif /* ZMPSShareStatusProtocol_h */
