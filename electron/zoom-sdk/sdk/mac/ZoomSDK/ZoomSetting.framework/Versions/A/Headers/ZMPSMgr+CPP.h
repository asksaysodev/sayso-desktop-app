//
//  ZMPSMgr+CPP.h
//  ZoomSetting
//
//  Created by Michael Lin on 2024/1/11.
//

#import <ZoomSetting/ZoomSetting.h>
#import <zPSApp/define/PSDefine.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMPSMgr (CPP)

+ (ZMPSSceneContext *)sceneContextForPSScene:(ps::PSScene)scene;

@end

NS_ASSUME_NONNULL_END
