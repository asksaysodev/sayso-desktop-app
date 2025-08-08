//
//  ZMRichFormatParagraphStyle.h
//  ChatUI
//
//  Created by Yong Zhou on 9/8/21.
//  Copyright © 2021 Zoom. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <zChatComponent/ZMRichFormatConstants.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMRichFormatParagraphStyle : NSMutableParagraphStyle

@property (nonatomic, assign) BOOL isBulletFormat;
@property (nonatomic, assign) BOOL isNumberListFormat;
@property (nonatomic, assign) BOOL isIndentFormat;
@property (nonatomic, assign) BOOL isQuoteFormat;
@property (nonatomic, assign) BOOL isQuoteBegin;
@property (nonatomic, assign) BOOL isHeadingFormat;
@property (nonatomic, assign) BOOL isCodeBlockFormat;

@property (nonatomic, assign) NSInteger indentLevel;

@property (nonatomic, assign) NSRange characterRange;

@property (nonatomic, assign) NSRange numberRange;

@property (nonatomic, assign) BOOL ingoreParagraphCache;

@property (nonatomic, assign, readonly) ZMParagraphListOptions listOption;

+ (ZMRichFormatParagraphStyle *)zm_mutableCopywithParagraphStyle:(NSParagraphStyle*)mStyle NS_RETURNS_RETAINED;
+ (ZMRichFormatParagraphStyle *)zm_RetainedListParagraphWithParagraphStyle:(NSParagraphStyle*)style NS_RETURNS_RETAINED;
+ (ZMRichFormatParagraphStyle *)zm_defaultParagraphStyle;

- (BOOL)isSameFormat:(ZMRichFormatParagraphStyle *)paragrahStyle;

- (BOOL)isParagraphStyleMatchingListType:(ZMTextFormatListType)type;

@end

NS_ASSUME_NONNULL_END
