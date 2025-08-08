//
//  ZMPSWallpaperStatusProtocol.h
//  ZoomSetting
//
//  Created by Michael Lin on 2024/1/15.
//

#ifndef ZMPSWallpaperStatusProtocol_h
#define ZMPSWallpaperStatusProtocol_h

@protocol ZMPSWallpaperStatusProtocol <NSObject>

@optional
- (void)psOnWallpaperConfigReady;
- (void)psOnWallpaperConfigChanged;
- (void)psOnWallpaperItemsChanged;

@end

#endif /* ZMPSWallpaperStatusProtocol_h */
