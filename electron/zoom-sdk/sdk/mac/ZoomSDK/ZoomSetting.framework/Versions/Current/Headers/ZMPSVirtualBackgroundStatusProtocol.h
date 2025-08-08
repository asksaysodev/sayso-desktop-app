//
//  ZMPSVirtualBackgroundStatusProtocol.h
//  ZoomSetting
//
//  Created by Michael Lin on 2024/1/15.
//

#ifndef ZMPSVirtualBackgroundStatusProtocol_h
#define ZMPSVirtualBackgroundStatusProtocol_h

#import <Appkit/Appkit.h>
@protocol ZMPSVirtualBackgroundStatusProtocol <NSObject>

- (void)psNotifyVirtualBackgroundEnableChanged;
- (void)psOnVBConfigChanged;
- (void)psOnVBItemsChanged;
- (void)psVBGreenScreenColorChanged:(nullable NSColor *)color;

@end

#endif /* ZMPSVirtualBackgroundStatusProtocol_h */
