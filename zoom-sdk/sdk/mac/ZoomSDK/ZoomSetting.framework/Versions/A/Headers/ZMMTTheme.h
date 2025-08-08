//
//  ZMMTTheme.h
//  VideoUI
//
//  Created by Francis Zhuo on 2024/1/6.
//  Copyright © 2024 zoom.us. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <ZoomKit/ZMPigment.h>
#import <ZoomSetting/ZMMTToolbarTheme.h>
#import <ZoomSetting/ZMMTVideoTheme.h>
#import <ZoomSetting/ZoomSettingSwiftInterface.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMMTTheme : NSObject
@property(nonatomic,strong) ZMPigment* windowFrame;
@property(nonatomic,strong) ZMPigment* videoFrame;
@property(nonatomic,strong) ZMPigment* bottomFrame;
@property(assign) NSEdgeInsets videoFramePadding;

@property(nonatomic, strong) ZMMTTitlebarTheme* titlebarTheme;

@property(nonatomic, strong) ZMMTNavigationTheme* navigation;

@property (nonatomic, strong) ZMMTPopoutTheme *popout;

@property(strong) ZMMTVideoTheme* videoTheme;
@property(nonatomic, strong) ZMPigment* sidepanel;
@property(strong) ZMMTToolbarTheme* toolbarTheme;

@property(nonatomic, strong) ZMMTFloatPanelTheme* floatPanelTheme;
@property(assign, readonly) ZMThemeName name;

+ (ZMMTTheme*)display;

+ (void)setThemeWithName:(ZMThemeName)name;
+ (void)lockThemeWithName:(ZMThemeName)name;
+ (void)unlockTheme;
+ (instancetype)themeForName:(ZMThemeName)name;

@end

NS_ASSUME_NONNULL_END
