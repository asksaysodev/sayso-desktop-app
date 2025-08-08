//
//  ZMPSShareMgr.h
//  ZoomClips
//
//  Created by Michael Lin on 2023/1/11.
//

#import <Foundation/Foundation.h>
#import <ZoomSetting/ZMPSObject.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMPSShareMgr : ZMPSObject

- (void)setMagicWindowFilterOnOFF:(BOOL)isOn;
- (void)changeShareFilterWindowID:(NSNumber *)windowID onOFF:(BOOL)isOn;
- (void)unregisterSink;
- (void)registerSink;
- (void)updateShareFilterMode;
- (BOOL)isShareDesktopDisabled;
- (BOOL)isMultiSelectAppShareTooltipDisabled;
- (void)disableMultiSelectAppShareTooltip:(BOOL)disabled;
- (BOOL)isMultiSelectDesktopShareTooltipDisabled;
- (void)disableMultiSelectDesktopShareTooltip:(BOOL)disabled;
- (NSUInteger)getMaxNumberSelectMultiDesktop;
- (void)setOptimizeForVideoCapture:(BOOL)isOn;

@end

NS_ASSUME_NONNULL_END
