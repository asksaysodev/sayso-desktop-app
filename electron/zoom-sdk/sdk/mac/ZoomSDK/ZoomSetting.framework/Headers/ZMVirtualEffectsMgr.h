//
//  ZMVirtualEffectsMgr.h
//  ZoomSetting
//
//  Created by Vinson Wang on 2024/4/11.
//

#import <Foundation/Foundation.h>

#import "IZMVirtualEffectsMgr.h"

NS_ASSUME_NONNULL_BEGIN

@interface ZMVirtualEffectsMgr<ViewController: ZMVirtualEffectsViewController*> : NSObject <IZMVirtualEffectsMgr>

@property (nonatomic, readonly) ZMConfSession *confSession;

@property (nonatomic, copy, nullable) void (^zoomAppCompletion)(BOOL success, NSUInteger errorCode);

- (BOOL)checkPopFaceLegalAlertIfNeed;
- (ICmmVideoDeviceController *)videoDeviceController:(NSView *)view;
- (NSView *)fetchBindedRenderView;

@end


@interface ZMVirtualEffectsMgr (VB)

@end


@interface ZMVirtualEffectsMgr (Avatar)

- (void)run3DAvatar;

@end


@interface ZMVirtualEffectsMgr (VideoFilter)

- (void)runMakeup:(NSView *)view;

@end


@interface ZMVirtualEffectsMgr (StudioEffects)

- (void)runStudioEffects:(NSView *)view;

@end


@interface ZMVirtualEffectsMgr (CommonApi)

@end

NS_ASSUME_NONNULL_END
