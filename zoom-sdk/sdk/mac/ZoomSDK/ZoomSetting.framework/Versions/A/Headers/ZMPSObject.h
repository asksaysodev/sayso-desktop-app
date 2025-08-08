//
//  ZMPSObject.h
//  ZoomSetting
//
//  Created by Michael Lin on 2024/1/10.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, ZMPSScene) {
    ZMPSSceneNone,
    ZMPSSceneDefault,
    ZMPSSceneAsyncRecording,
    ZMPSSceneNDI,
    ZMPSScenePSL,
};

@class ZMPSAppMgr, ZMPSCanvasMgr, ZMPSContextMgr, ZMPSMediaClient;
@class ZMPSAvatarMgr, ZMPSVirtualBackgroundMgr, ZMPSWallpaperMgr;
@class ZMPSVideoMgr, ZMPSAudioMgr, ZMPSShareMgr, ZMPSRenderMgr;
@class ZMPSAsyncRecordingMgr, ZMPSSceneContext, ZMPSAnnotationMgr;
@interface ZMPSObject : NSObject

@property (nonatomic, assign) ZMPSScene scene;

- (instancetype)initWithScene:(ZMPSScene)scene;

- (ZMPSAppMgr *)appMgr;
- (ZMPSCanvasMgr *)canvasMgr;
- (ZMPSRenderMgr *)renderMgr;
- (ZMPSContextMgr *)contextMgr;
- (ZMPSMediaClient *)mediaClient;
- (ZMPSAvatarMgr *)avatarMgr;
- (ZMPSVirtualBackgroundMgr *)virtualBackgroundMgr;
- (ZMPSWallpaperMgr *)wallpaperMgr;
- (ZMPSVideoMgr *)videoMgr;
- (ZMPSAudioMgr *)audioMgr;
- (ZMPSShareMgr *)shareMgr;
- (ZMPSAsyncRecordingMgr *)asyncRecordingMgr;
- (ZMPSAnnotationMgr *)annotationMgr;
- (ZMPSSceneContext *)sceneContext;

@end

NS_ASSUME_NONNULL_END
