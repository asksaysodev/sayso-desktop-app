//
//  ZMPTTheme.h
//  ZoomSetting
//
//  Created by fistice on 2024-02-21.
//

#import <Foundation/Foundation.h>
#import <ZoomKit/ZMPigment.h>
#import <ZoomUnit/IZMThemeMgr.h>
NS_ASSUME_NONNULL_BEGIN

@protocol ZMPTThemeProtocol <NSObject>
- (void)themeDidChanged;
@end
@interface ZMPTPigment : ZMPigment <ZMPTThemeProtocol>
@property(nonatomic, copy) void (^pigmentTube)(ZMThemeName name, __kindof ZMPTPigment *pigment);
+ (instancetype)pigmentWithTube:(void(^)(ZMThemeName name, __kindof ZMPTPigment *pigment))pigmentTube;
@end

@interface ZMPTButtonPigment : ZMPTPigment <ZMButtonPigmentProtocol>

@property(nonatomic, assign) BOOL matchImageColorWithTitle;

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

@property(nonatomic, strong, nullable) NSColor* titleColor;
@property(nonatomic, strong, nullable) NSColor* normalTitleColor;
@property(nonatomic, strong, nullable) NSColor* hoverTitleColor;
@property(nonatomic, strong, nullable) NSColor* pressTitleColor;
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

@interface ZMPTTheme : NSObject

+ (instancetype)display;

+ (void)setThemeWithName:(ZMThemeName)name;

@property (assign) ZMThemeName selectedThemeName;

@property (nonatomic, strong) ZMPTPigment *mainPigment;
@property (nonatomic, strong) ZMPTPigment *headerPigment;
@property (nonatomic, strong) ZMPTButtonPigment* mainTabButtonPigment;
@property (nonatomic, strong) ZMPTButtonPigment* mainTabUpdateVersionButtonPigment;
@property (nonatomic, strong) ZMPTButtonPigment* imageButtonPigment;
@property (nonatomic, strong) ZMPTButtonPigment* searchButtonPigment;
@property (nonatomic, strong) ZMPTPigment* sidebarPigment;
@property (nonatomic, strong) ZMPTPigment* sidebarListPigment;
@property (nonatomic, strong) ZMPTPigment* sidebarLeftTabPigment;

@end
NS_ASSUME_NONNULL_END
