//
//  IZMMTWindowModeMgr.h
//  ZoomSetting
//
//  Created by Yutong.Xiu on 2024/6/3.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@protocol IZMMTWindowModeMgr <NSObject>

@property (nonatomic, assign, readonly) BOOL isDualModeButtonEnabled;
@property (nonatomic, assign, readonly) BOOL isPopoutButtonEnabled;
@property (nonatomic, copy, readonly) NSString *dualModeRadioButtonTip;
@property (nonatomic, copy, readonly) NSString *popoutRadioButtonTip;


- (BOOL)isDualModeSelected;
- (BOOL)isPopoutSelected;

- (void)selectDualMode;
- (void)toggleDualMode; // for dual mode keyboard shortcuts

- (void)selectPopoutMode;

- (void)updateWindowModeSettings;
- (BOOL)isMeetingTabPopoutEnabled;
- (BOOL)isDualModeEnabled;
- (BOOL)isInPopoutMode;

@end

@class ZMMTWindowModeMgr;

@protocol ZMMTWindowModeProtocol <NSObject>

@optional
- (void)windowModeSettingsDidChanged;

@end

NS_ASSUME_NONNULL_END
