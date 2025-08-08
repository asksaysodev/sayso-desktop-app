//
//  ZMZAppMenuItem.h
//  ZoomAppUI
//
//  Created by Neko Chen on 2023/6/25.
//  Copyright © 2023 us.zoom. All rights reserved.
//

NS_ASSUME_NONNULL_BEGIN

@interface ZMZAppMenuItem : ZMHMenuItem

+ (ZMZAppMenuItem *)menuItemWithTitle:(NSString *)title isChecked:(BOOL)isChecked;

+ (ZMZAppMenuItem *)menuItemWithTitle:(NSString *)title isChecked:(BOOL)isChecked onlyDarkMode:(BOOL)onlyDarkMode;

+ (ZMZAppMenuItem *)menuItemWithTitle:(nullable NSString *)title
                            isChecked:(BOOL)isChecked
                       attrubiteTitle:(nullable NSAttributedString *)attrubiteTitle;

+ (ZMZAppMenuItem *)menuItemWithTitle:(nullable NSString *)title
                            isChecked:(BOOL)isChecked
                       attrubiteTitle:(nullable NSAttributedString *)attrubiteTitle
                         onlyDarkMode:(BOOL)onlyDarkMode;

+ (ZMZAppMenuItem *)menuItemWithTitle:(nullable NSString *)title
                            iconImage:(nullable NSImage *)iconImage
                            isChecked:(BOOL)isChecked
                       attrubiteTitle:(nullable NSAttributedString *)attrubiteTitle;

+ (ZMZAppMenuItem *)menuItemWithTitle:(nullable NSString *)title
                            iconImage:(nullable NSImage *)iconImage
                            isChecked:(BOOL)isChecked
                       attrubiteTitle:(nullable NSAttributedString *)attrubiteTitle
                         onlyDarkMode:(BOOL)onlyDarkMode;

- (void)setAppItemAccessibilityValue:(NSString *)accessibilityValue;

@end

NS_ASSUME_NONNULL_END
