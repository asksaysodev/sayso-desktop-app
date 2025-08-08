//
//  ZMThemeMgr.h
//  ZoomSetting
//
//  Created by Francis Zhuo on 2024/2/4.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMThemeMgr : NSObject

+ (instancetype)shared;

- (void)getCurrentThemeColor:(void (^)(NSColor *color))themeColorBlock 
              inClassicTheme:(void (^)(void))inClassicThemeBlock;

@end

NS_ASSUME_NONNULL_END
