//
//  ZMVirtualEffectsViewController.h
//  ZoomSetting
//
//  Created by Vinson Wang on 2024/3/26.
//

#import <ZoomKit/ZoomKit.h>

#import <ZoomSetting/IZMVirtualEffectsMgr.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMVirtualEffectsViewController : ZMBaseViewController

@property (nonatomic, assign) ZMVEScene scene;

- (void)reloadData;

- (void)updatePreview;

- (void)greenScreenStateChanged;

- (void)showVB;

- (void)showFilters;

- (void)showAvatar;

- (void)showVirtualEffects;

- (BOOL)isHumanAvatarWindowShowing;

- (void)stopPreview;

@end

NS_ASSUME_NONNULL_END
