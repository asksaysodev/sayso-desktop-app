//
//  ZMPSDefine.h
//  ZoomClips
//
//  Created by Michael Lin on 2023/1/5.
//

#ifndef ZMPSDefine_h
#define ZMPSDefine_h

#import <zPSApp/define/PSDefine.h>
#import <ZoomSetting/ZMPSObject.h>
#import <ZoomKit/ZoomKit.h>
#import <ZoomSetting/ZMPSAppBridge.h>

extern ps::PSRect PSRectFromNSRect(NSRect rect);
extern SSB_MC_DATA_BLOCK_COLOR PSColorFromNSColor(NSColor *color);
extern NSColor *NSColorFromPSColor(SSB_MC_DATA_BLOCK_COLOR color);
extern ZMPSScene zmSceneFromPSScene(ps::PSScene psScene);
extern ps::PSScene psSceneFromZMScene(ZMPSScene zmScene);
extern ps::PSFrameResolution psResolutionFromZMResolution(ZMPSResolutionType resolution);
extern ZMPSResolutionType zmResolutionFromPSResolution(ps::PSFrameResolution resolution);
extern ps::PSFrameSize psFrameSizeFromZMResolution(ZMPSResolutionType resolution);


#endif /* ZMPSDefine_h */
