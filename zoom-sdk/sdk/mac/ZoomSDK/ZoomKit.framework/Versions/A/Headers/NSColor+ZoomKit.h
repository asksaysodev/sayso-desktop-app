//
//  NSColor+ZoomKit.h
//  ZCommonUI
//
//  Created by Huxley on 2018/4/11.
//  Copyright © 2018 zoom. All rights reserved.
//

#import <ZoomKit/ZMBase.h>

NS_ASSUME_NONNULL_BEGIN

APPKIT_EXTERN CGColorRef ZMCGColorCreateSRGB(CGFloat red, CGFloat green, CGFloat blue, CGFloat alpha);

APPKIT_EXTERN NSColor * NSColorFRGB(CGFloat r, CGFloat g, CGFloat b);
APPKIT_EXTERN NSColor * NSColorFRGBA(CGFloat r, CGFloat g, CGFloat b, CGFloat a);
APPKIT_EXTERN CGColorRef CGColorFRGB(CGFloat r, CGFloat g, CGFloat b);
APPKIT_EXTERN CGColorRef CGColorFRGBA(CGFloat r, CGFloat g, CGFloat b, CGFloat a);

/**
 Convenience Color method.
 @param r 0 ~ 255
 @param g 0 ~ 255
 @param b 0 ~ 255
 @return Calibrated RGB color, alpha is 1
 */
APPKIT_EXTERN NSColor * NSColorRGB(NSInteger r, NSInteger g, NSInteger b);
APPKIT_EXTERN CGColorRef CGColorRGB(NSInteger r, NSInteger g, NSInteger b);

/**
 Convenience Color method.
 @param r 0 ~ 255
 @param g 0 ~ 255
 @param b 0 ~ 255
 @param a 0 ~ 1.0
 @return Calibrated RGB color
 */
APPKIT_EXTERN NSColor * NSColorRGBA(NSInteger r, NSInteger g, NSInteger b, CGFloat a);
APPKIT_EXTERN CGColorRef CGColorRGBA(NSInteger r, NSInteger g, NSInteger b, CGFloat a);


/**
  Convenience Color method.

 @param rgbValue hex value like 0xffffff
 @return Calibrated RGB color
 */
APPKIT_EXTERN NSColor * NSColorFromHex(NSInteger rgbValue);
APPKIT_EXTERN NSColor * NSColorFromRGBAHex(NSUInteger rgbaValue);
APPKIT_EXTERN CGColorRef CGColorFromHex(NSInteger rgbValue);

APPKIT_EXTERN NSColor * NSColorFromHexString(NSString *hexValue);

/**
 Convenience Color method.
 
 @param light and dark color hex value like 0xffffff
 @return Calibrated ZMAppearanceColor
 */
APPKIT_EXTERN NSColor * ZMHexColor(NSUInteger light, NSUInteger dark);

/**
 Convenience Color method.
 @param light hex value like 0xffffff
 @param lightAlpha 0 ~ 1
 @param dark hex value like 0xffffff
 @param darkAlpha 0 ~ 1
 @return Calibrated ZMAppearanceColor base on RGB color
 */
APPKIT_EXTERN NSColor * ZMHexColorWithAlpha(NSUInteger light, CGFloat lightAlpha,  NSUInteger dark, CGFloat darkAlpha);

/**
 Convenience Color method.
 
 @param rgbValue  hex value like 0xffffff
 @param alpha 0 ~ 1
 @return Calibrated RGB color
 */
APPKIT_EXTERN NSColor * NSColorFromHexWithAlpha(NSInteger rgbValue,float alpha);

@interface NSColor (ZoomKit)

@property (readonly) CGColorRef zm_CGColor NS_RETURNS_INNER_POINTER;
+ (nullable NSColor *)zm_colorWithCGColor:(nullable CGColorRef)cgColor;

/**
 * Pri == primary
 * Sec == Secondary
 * Tri == Tertiary
 * Des == Destructive
*/
#pragma mark - ZMButton Background Color
@property (class, readonly) NSColor *zmBtnPriNormalBgColor;
@property (class, readonly) NSColor *zmBtnSecNormalBgColor;
@property (class, readonly) NSColor *zmBtnTriNormalBgColor;
@property (class, readonly) NSColor *zmBtnDesNormalBgColor;
@property (class, readonly) NSColor *zmBtnTxtNormalBgColor;

@property (class, readonly) NSColor *zmBtnPriHoverBgColor;
@property (class, readonly) NSColor *zmBtnSecHoverBgColor;
@property (class, readonly) NSColor *zmBtnTriHoverBgColor;
@property (class, readonly) NSColor *zmBtnDesHoverBgColor;
@property (class, readonly) NSColor *zmBtnTxtHoverBgColor;

@property (class, readonly) NSColor *zmBtnPriPressBgColor;
@property (class, readonly) NSColor *zmBtnSecPressBgColor;
@property (class, readonly) NSColor *zmBtnTriPressBgColor;
@property (class, readonly) NSColor *zmBtnDesPressBgColor;
@property (class, readonly) NSColor *zmBtnTxtPressBgColor;

@property (class, readonly) NSColor *zmBtnDisableBgColor;

#pragma mark - ZMButton Title Color
@property (class, readonly) NSColor *zmBtnPriNormalTitleColor;
@property (class, readonly) NSColor *zmBtnSecNormalTitleColor;
@property (class, readonly) NSColor *zmBtnTriNormalTitleColor;
@property (class, readonly) NSColor *zmBtnDesNormalTitleColor;
@property (class, readonly) NSColor *zmBtnTxtNormalTitleColor;

@property (class, readonly) NSColor *zmBtnPriHoverTitleColor;
@property (class, readonly) NSColor *zmBtnSecHoverTitleColor;
@property (class, readonly) NSColor *zmBtnTriHoverTitleColor;
@property (class, readonly) NSColor *zmBtnDesHoverTitleColor;
@property (class, readonly) NSColor *zmBtnTxtHoverTitleColor;

@property (class, readonly) NSColor *zmBtnPriPressTitleColor;
@property (class, readonly) NSColor *zmBtnSecPressTitleColor;
@property (class, readonly) NSColor *zmBtnTriPressTitleColor;
@property (class, readonly) NSColor *zmBtnDesPressTitleColor;
@property (class, readonly) NSColor *zmBtnTxtPressTitleColor;

@property (class, readonly) NSColor *zmBtnDisableTitleColor;

#pragma mark - ZMButton Border Color
@property (class, readonly) NSColor *zmBtnNormalBorderColor;
@property (class, readonly) NSColor *zmBtnSecNormalBorderColor;
@property (class, readonly) NSColor *zmBtnSecHoverBorderColor;
@property (class, readonly) NSColor *zmBtnSecPressBorderColor;

#pragma mark - Single color

@property (class, readonly) NSColor *zmDark;
@property (class, readonly) NSColor *zmRed;
@property (class, readonly) NSColor *zmOrange;
@property (class, readonly) NSColor *zmBlue;
@property (class, readonly) NSColor *zmBlueLight;
@property (class, readonly) NSColor *zmBlueDark;
@property (class, readonly) NSColor *zmGrey;

@property (class, readonly) NSColor *zmClearColor;
@property (class, readonly) NSColor *zmWhiteColor;
@property (class, readonly) NSColor *zmBlackColor;

@property (class, readonly) NSColor *zmDarkGrayColor;

@property (class, readonly) NSColor *zmGreyColor;

#pragma mark - Background Color

@property (class, readonly) NSColor *zmBackgroundColor;/* #FFFFFF, #0x1A1A1A */
@property (class, readonly) NSColor *zmSidebarBackgroundColor;

#pragma mark - Text color
@property (class, readonly) NSColor *zmPriTextColor;
@property (class, readonly) NSColor *zmSecTextColor;
@property (class, readonly) NSColor *zmTertiaryTextColor;
@property (class, readonly) NSColor *zmDesTextColor;
@property (class, readonly) NSColor *zmContentDesTextColor;

@property (class, readonly) NSColor *zmErrorRedColor;
@property (class, readonly) NSColor *zmFocusBlueColor;

@property (class, readonly) NSColor *zmLineColor;
@property (class, readonly) NSColor *zmPlaceholderStringColor;
@property (class, readonly) NSColor *zmHighlightColor;

@property (class, readonly) NSColor *zmPopoverTextColor;

@property (class, readonly) NSColor *zmImagePaddingColor;
@property (class, readonly) NSColor *zmImageBorderColor;
@property (class, readonly) NSColor *zmMessageAccessoryBorderColor;
@property (class, readonly) NSColor *zmMessageAccessorySelectedColor;

- (nullable NSString *)hexString;

@property (class, readonly) NSColor *zmPULightGreyColor;
@property (class, readonly) NSColor *zmSubTitleColor;
@property (class, readonly) NSColor *zmMenuHoverColor;
@end

NS_ASSUME_NONNULL_END


