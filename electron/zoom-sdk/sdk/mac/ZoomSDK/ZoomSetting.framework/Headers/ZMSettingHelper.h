//
//  ZMSettingHelper.h
//  ZoomSetting
//
//  Created by Francis Zhuo on 2024/3/4.
//

#import <ZoomKit/ZoomKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMSettingHelper : NSObject
+ (instancetype)shared;
- (BOOL)disableSwitchColorTheme;
@property (nonatomic, assign, class, readonly) BOOL usingzUI;
@end

NS_ASSUME_NONNULL_END
