//
//  ZMPSCanvas+CPP.h
//  ZoomSetting
//
//  Created by Michael Lin on 2024/4/9.
//

#import <ZoomSetting/ZoomSetting.h>
#import <ZoomSetting/ZMPSCanvas.h>
#import "zPSApp/media/IPSCanvas.h"

NS_ASSUME_NONNULL_BEGIN

@class ZMPSHandleHolder;
@interface ZMPSCanvas (CPP)

@property (nonatomic, strong, nullable) ZMPSHandleHolder *handleHolder;
@property (nonatomic, readonly, nullable) ps::IPSCanvas *ipsCanvas;
- (nullable instancetype)initWithCanvas:(ps::IPSCanvas *)canvas parentCanvas:(nullable ZMPSCanvas *)parentCanvas scene:(ZMPSScene)scene;
- (nullable ps::IPSCanvas *)fetchSubCanvasWithHandle:(SSB_MC_CANVAS_HANDLE)handle;
- (void)setAspectMode:(SSB_MC_VIDEO_ASPECT_MODE)mode;
- (SSB_MC_VIDEO_ASPECT_MODE)aspectMode;

@end

@interface ZMPSRootCanvas (CPP)

- (nullable instancetype)initWithCanvas:(ps::IPSCanvas *)canvas scene:(ZMPSScene)scene;

@end

NS_ASSUME_NONNULL_END
