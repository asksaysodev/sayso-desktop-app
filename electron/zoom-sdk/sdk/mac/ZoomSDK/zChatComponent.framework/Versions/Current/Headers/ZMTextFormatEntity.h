//
//  ZMTextFormatEntity.h
//  SaasBeePTUIModule
//
//  Created by Yong Zhou on 6/17/21.
//  Copyright © 2021 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <zChatComponent/ZMRichFormatConstants.h>

@interface ZMTextFormatInfo : NSObject
@property (nonatomic, assign) NSUInteger currentFormat;
@property (nonatomic, assign) ZMTextFormatSizeType sizeType;
@property (nonatomic, assign) ZMTextFormatParagraphType paragraphHeadingType;
@property (nonatomic, copy) NSString *textColorStr;
@property (nonatomic, copy) NSString *textBgColorStr;
@end

@interface ZMTextColorItem : NSObject
@property (nonatomic, assign) NSUInteger index;
@property (nonatomic, copy) NSString *lightColorString;
@property (nonatomic, copy) NSString *darkColorString;
@property (nonatomic, strong) NSColor *color;

+ (NSInteger)hexValueFromColor:(NSColor*)color;
+ (NSString *)hexStrFromColor:(NSColor*)color;
+ (NSString*)convertToStandardServerTextColor:(NSString*)tmpColor;
+ (NSString*)convertToStandardLocalTextColor:(NSString*)tmpColor;
+ (NSString*)convertToStandardServerBackgroundColor:(NSString*)tmpColor;
+ (NSString*)convertToStandardLocalTextBgColor:(NSString*)tmpColor;
+ (NSColor*)convertToLocalTypingAttributesTextColor:(NSString*)tmpColor;
+ (NSColor*)convertToLocalTypingAttributesTextBgColor:(NSString*)tmpColor;

@end
