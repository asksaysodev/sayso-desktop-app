//
//  ZMPSRender.h
//  ZoomClips
//
//  Created by Michael Lin on 2022/12/30.
//

#import <Foundation/Foundation.h>
#ifdef __cplusplus
#import <zPSApp/media/IPSRender.h>
#endif
#import <ZoomSetting/ZMPSObject.h>
#import <ZoomSetting/ZMPSMediaClient.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMPSRender : ZMPSObject

#ifdef __cplusplus
@property (nonatomic, readonly, nullable) ps::IPSRender *ipsRender;
#endif

- (nullable instancetype)initWithView:(NSView *)view scene:(ZMPSScene)scene;
- (void)generateRenderIfNeeded;
#ifdef __cplusplus
- (void)setMirrorStrategy:(ps::PSMirrorStrategy)strategy;
- (void)setAspectMode:(SSB_MC_VIDEO_ASPECT_MODE)aspect;
#endif
- (void)startPreviewVideoWithResolutionType:(ZMPSResolutionType)resolutionType immersive:(BOOL)immersive;
- (void)stopPreviewVideo;
- (void)updateReplaceVBPickColorByPosition:(CGPoint)position;
- (void)notifyViewSizeChanged;
- (void)enterPickColorMode;
- (void)leavePickColorMode;
- (BOOL)isInPickingColorMode;

@end

NS_ASSUME_NONNULL_END
