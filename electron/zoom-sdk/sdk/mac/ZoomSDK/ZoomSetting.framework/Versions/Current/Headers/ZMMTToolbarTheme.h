//
//  ZMMTToolbarTheme.h
//  VideoUI
//
//  Created by martin yu on 2024/1/30.
//  Copyright © 2024 zoom.us. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <ZoomKit/ZMPigment.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMMTButtonIcons: NSObject
@property(nonatomic, strong, nullable) NSImage* icon;
@property(nonatomic, strong, nullable) NSImage* normalIcon;
@property(nonatomic, strong, nullable) NSImage* hoverIcon;
@property(nonatomic, strong, nullable) NSImage* pressIcon;
@property(nonatomic, strong, nullable) NSImage* disabledIcon;
@property(nonatomic, strong, nullable) NSImage* selectedIcon;
@end

@interface ZMMTButtonPigment : ZMPigment <ZMButtonPigmentProtocol>
@property(nonatomic, strong, nullable) NSColor* titleColor;
@property(nonatomic, strong, nullable) NSColor* normalTitleColor;
@property(nonatomic, strong, nullable) NSColor* hoverTitleColor;
@property(nonatomic, strong, nullable) NSColor* pressTitleColor;
@property(nonatomic, strong, nullable) NSColor* disabledTitleColor;
@property(nonatomic, strong, nullable) NSColor* selectedTitleColor;

@property(nonatomic, strong, nullable) ZMPigment* bgPigment;
@property(nonatomic, strong, nullable) ZMPigment* normalBgPigment;
@property(nonatomic, strong, nullable) ZMPigment* hoverBgPigment;
@property(nonatomic, strong, nullable) ZMPigment* pressBgPigment;
@property(nonatomic, strong, nullable) ZMPigment* disabledBgPigment;
@property(nonatomic, strong, nullable) ZMPigment* selectedBgPigment;

@property(nonatomic, assign) CGFloat alphaValue;
@property(nonatomic, assign) CGFloat disableAlphaValue;

@property(nonatomic, strong) NSDictionary<NSString*, ZMMTButtonIcons*>* buttonIcons;

// Currently is used for recording button
@property(nonatomic, strong, nullable) NSDictionary<NSString*, ZMMTButtonPigment*>* supplementalButtonPigment;

@property(nonatomic, assign) BOOL matchImageColorWithTitle;

//@property(nonatomic, strong, nullable) NSImage* image;
@property(nonatomic, strong, nullable) NSImage* normalImage;
@property(nonatomic, strong, nullable) NSImage* hoverImage;
@property(nonatomic, strong, nullable) NSImage* pressImage;
@property(nonatomic, strong, nullable) NSImage* selectImage;
@property(nonatomic, strong, nullable) NSImage* disableImage;

@property(copy, nullable) ZMBackgroundDrawer backgroundDrawer;
@property(copy, nullable) ZMBackgroundDrawer normalBackgroundDrawer;
@property(copy, nullable) ZMBackgroundDrawer hoverBackgroundDrawer;
@property(copy, nullable) ZMBackgroundDrawer pressBackgroundDrawer;
@property(copy, nullable) ZMBackgroundDrawer selectBackgroundDrawer;
@property(copy, nullable) ZMBackgroundDrawer disableBackgroundDrawer;

@property(nonatomic, strong, nullable) NSImage* backgroundImage;
@property(nonatomic, strong, nullable) NSImage* normalBackgroundImage;
@property(nonatomic, strong, nullable) NSImage* hoverBackgroundImage;
@property(nonatomic, strong, nullable) NSImage* pressBackgroundImage;
@property(nonatomic, strong, nullable) NSImage* selectBackgroundImage;
@property(nonatomic, strong, nullable) NSImage* disableBackgroundImage;

@property(nonatomic, strong, nullable) NSColor* backgroundColor;
@property(nonatomic, strong, nullable) NSColor* normalBackgroundColor;
@property(nonatomic, strong, nullable) NSColor* hoverBackgroundColor;
@property(nonatomic, strong, nullable) NSColor* pressBackgoundColor;
@property(nonatomic, strong, nullable) NSColor* selectBackgroundColor;
@property(nonatomic, strong, nullable) NSColor* disableBackgroundColor;
@property(nonatomic, strong, nullable) NSColor* selectedHoverBackgroundColor;

//@property(nonatomic, strong, nullable) NSColor* titleColor;
//@property(nonatomic, strong, nullable) NSColor* normalTitleColor;
//@property(nonatomic, strong, nullable) NSColor* hoverTitleColor;
//@property(nonatomic, strong, nullable) NSColor* pressTitleColor;
@property(nonatomic, strong, nullable) NSColor* selectTitleColor;
@property(nonatomic, strong, nullable) NSColor* disableTitleColor;

//@property(nonatomic, strong, nullable) NSColor* borderColor;
@property(nonatomic, strong, nullable) NSColor* normalBorderColor;
@property(nonatomic, strong, nullable) NSColor* hoverBorderColor;
@property(nonatomic, strong, nullable) NSColor* pressBorderColor;
@property(nonatomic, strong, nullable) NSColor* selectBorderColor;
@property(nonatomic, strong, nullable) NSColor* disableBorderColor;

@property(nonatomic, strong, nullable) NSColor* unreadColor;
@end

@interface ZMMTToolbarTheme : NSObject
@property(strong) ZMPigment* fader;
@property(assign) NSInteger faderHeight;//default 58
@property(strong) ZMPigment* background;

@property(strong) NSDictionary<NSString*, ZMMTButtonPigment*>* itemPigments;
- (void)setTheme:(ZMMTToolbarTheme*)theme;

+ (instancetype)bloomTheme;
+ (instancetype)roseTheme;
+ (instancetype)agaveTheme;
+ (instancetype)customTheme;

+ (instancetype)themeForName:(ZMThemeName)name;
@end

NS_ASSUME_NONNULL_END
