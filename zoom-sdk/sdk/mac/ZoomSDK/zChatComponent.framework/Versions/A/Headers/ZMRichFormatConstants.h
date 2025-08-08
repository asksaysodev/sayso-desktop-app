//
//  ZMRichFormatConstants.h
//  ZoomMsgUI
//
//  Created by Yong Zhou on 4/21/22.
//

#ifndef ZMRichFormatConstants_h
#define ZMRichFormatConstants_h

typedef NS_OPTIONS(NSUInteger, ZMTextFormatStyle)
{
    ZMTextFormatStyle_None                = 0,
    ZMTextFormatStyle_Bold                = 1<<0,
    ZMTextFormatStyle_Italic              = 1<<1,
    ZMTextFormatStyle_Underline           = 1<<2,
    ZMTextFormatStyle_Strikethrough       = 1<<3,
    ZMTextFormatStyle_Color               = 1<<4,
    ZMTextFormatStyle_Size                = 1<<5,
    ZMTextFormatStyle_AddLink             = 1<<6,
    ZMTextFormatStyle_Paragraph           = 1<<7,
    ZMTextFormatStyle_BulletList          = 1<<8,
    ZMTextFormatStyle_NumberList          = 1<<9,
    ZMTextFormatStyle_IncreaseIndent      = 1<<10,
    ZMTextFormatStyle_DecreaseIndent      = 1<<11,
    ZMTextFormatStyle_Quote               = 1<<12,
    ZMTextFormatStyle_InlineCode          = 1<<13,
    ZMTextFormatStyle_CodeBlock           = 1<<14,
    ZMTextFormatStyle_ClearStyle          = 1<<15,
    ZMTextFormatStyle_Reserved            = 1<<16,
    ZMTextFormatStyle_ChatApp             = 1<<17,
};

typedef NS_OPTIONS(NSInteger, ZMInlineCodeStyle) {
    ZMInlineCodeStyleNone         = 0x00,
    ZMInlineCodeStyleNormal       = 0x01,
    ZMInlineCodeStyleBold         = 0x02,
};

typedef NS_ENUM (NSInteger, ZMTextFormatSizeType) {
    ZMTextFormatSize_Small = 0,
    ZMTextFormatSize_Medium,
    ZMTextFormatSize_Large
};

typedef NS_ENUM (NSInteger, ZMTextFormatParagraphType) {
    ZMTextFormatParagraph_Heading1 = 0,
    ZMTextFormatParagraph_Heading2,
    ZMTextFormatParagraph_Heading3,
    ZMTextFormatParagraph_Paragraph
};

typedef NS_ENUM (NSInteger, ZMTextFormatListType) {
    ZMTextFormatList_BulletList = 0,
    ZMTextFormatList_NumberList,
    ZMTextFormatList_QuoteList,
    ZMTextFormatList_CodeblockList
};

typedef NS_ENUM (NSInteger, ZMTextFormatTextColorType) {
    ZMTextFormatTextColor_Foreground = 0,
    ZMTextFormatTextColor_Background,
};

typedef NS_OPTIONS(NSInteger, ZMDrawingListColorType) {
    ZMDrawingListColor_Normal      = 0x00,
    ZMDrawingListColor_Quote       = 0x01
};

typedef struct {
    BOOL isQuote;
    BOOL isHeading;
    BOOL isCodeblock;
    BOOL isNumberList;
    BOOL isBulletList;
} ZMParagraphRTFStyle;

typedef struct {
    NSInteger charIndex;
    NSUInteger number;
    ZMTextFormatListType listType;
} ZMRTFNumberIndexInfo;

typedef NS_OPTIONS(NSUInteger, ZMParagraphListOptions) {
    ZMParagraphListNone = 0,
    ZMParagraphListQuote,
    ZMParagraphListBullet,
    ZMParagraphListNumber,
    ZMParagraphListCodeblock,
    ZMParagraphListQuoteWithBullet,
    ZMParagraphListQuoteWithNumber,
    ZMParagraphListQuoteWithCodeblock,
};

static const CGFloat  ZMFormatParagraphSpacing = 5.0f;
static const CGFloat  ZMFormatCodeblockLineSpacing = 3.0f;
static const CGFloat  ZMFormatCodeblockParagraphSpacing = 10.0f;
static const CGFloat  ZMFormatPerIndent = 40.0f;
static const CGFloat  ZMFormatMaxParagraphIndent = 400.0f;
static const CGFloat  ZMQuoteEndSpacing = 5;
static const CGFloat  ZMMaxIndentLevel = 4;

static NSString* const ZMRTFPlaceholderEmptyString = @" ";
static NSString* const ZMQuoteSymbolString = @"\u2033";
static NSString* const ZMDefaultServerTextColor = @"222230";
static NSString* const ZMDefaultServerTextDarkColor = @"FFFFFF";
static NSString* const ZMDefaultServerTextBgColor = @"FFFFFF";
static NSString* const ZMDefaultServerTextBgDarkColor = @"222230";
static inline NSColor * ZMInputTextViewDefaultTextColor(void)   { return ZMHexColor(0x222230, 0xFFFFFF); }
static inline NSColor * ZMInputTextViewDefaultTextBgColor(void) { return ZMHexColor(0xFFFFFF, 0x222230); }
static inline NSColor * ZMTextFormatQuoteTextColor(void)        { return ZMHexColor(0X555B62, 0xDFE3E8); }
static inline NSColor * ZMTextFormatCodeblockTextColor(void)    { return ZMHexColor(0x555B62, 0xFFFFFF); }
static inline NSColor * ZMTextFormatInLineCodeTextColor(void)   { return ZMHexColor(0x555B62, 0xFFFFFF); }
static inline NSColor * ZMTextFormatNewQuoteSymbolColor(void)   { return NSColorFromHex(0xC2C5CA); }

static inline NSString *ZMTextFormatDefaultTextColor(void)
{
    if (NSApp.isDarkMode) {
        return ZMDefaultServerTextDarkColor;
    }
    return ZMDefaultServerTextColor;
}

static inline NSString *ZMTextFormatDefaultTextBgColor(void)
{
    if (NSApp.isDarkMode) {
        return ZMDefaultServerTextBgDarkColor;
    }
    return ZMDefaultServerTextBgColor;
}

static NSString* const ZMInsertLinkAttributeName = @"ZMInsertLinkAttributeName";
// ZOOM-628330:
// This attribute contributes a virtual link that cannot be clicked directly. However,
// menu items and smart selection are still preserved. Clients should combine it with
// other style attributes to make the text look like a link.
static NSString* const ZMPhantomLinkAttributeName = @"ZMPhantomLinkAttributeName";

static NSString* const ZMQuoteAttributeName = @"ZMQuoteAttributeName";
static uint8     const ZMQuoteBeginFlag = 1;

static NSString* const ZMInlineCodeAttributeName = @"ZMInlineCodeAttributeName";

static NSString* const ZMCodeblockLanguageAttributeName = @"ZMCodeblockLanguageAttributeName";

static inline CGFloat   ZMBulletListDefaultIndent(void)    { return (2.0 * ZMDisplayFontSize());}
static inline CGFloat   ZMNumberListDefaultIndent(void)    { return (2.0 * ZMDisplayFontSize());}
static inline CGFloat   ZMCodeblockListDefaultIndent(void) { return (2.5 * ZMDisplayFontSize());}
static inline CGFloat   ZMQuoteListDefaultIndent(void)     { return (1.0 * ZMDisplayFontSize());}
static inline CGFloat   ZMInLineCodeBorderMargin(void)     { return (0.2 * ZMDisplayFontSize());}
// default indent for quote with order/un-order list
static inline CGFloat   ZMQuoteWithOrderListDefaultIndent(void)    { return (2.5 * ZMDisplayFontSize());}
// default indent for quote with code-block
static inline CGFloat   ZMQuoteWithCodeblockDefaultIndent(void)    { return (3.5 * ZMDisplayFontSize());}

static inline NSFont *ZMTextFormatCodeBlockFont(void)
{
    NSFont *font = nil;
    if (@available(macOS 10.15, *)) {
        font = [NSFont monospacedSystemFontOfSize:ZMDisplayFontSize() weight:NSFontWeightRegular];
    }
    
    if (!font) {
        font = ZMMessageFontWithCurrentAppSize();
    }
    return font;
}

static inline NSString *ZMCodeblockFamilyName(void)
{
    static dispatch_once_t once;
    static NSString *familyName = nil;
    dispatch_once(&once, ^{
        if (@available(macOS 10.15, *)) {
            NSFont *codeblockFont = [NSFont monospacedSystemFontOfSize:ZMDisplayFontSize() weight:NSFontWeightRegular];
            if (codeblockFont) {
                familyName = codeblockFont.familyName;
                if (familyName.length > 0) {
                    familyName = [familyName copy];
                }
            }
        }
    });
    return familyName;
}

static inline NSString *intToRoman(NSInteger num) {
    
    static NSArray *values = nil;
    if (!values){
        values = [@[@1000, @900, @500, @400, @100, @90, @50, @40, @10, @9, @5, @4, @1] copy];
    }
    static NSArray *symbols = nil;
    if (!symbols){
        symbols = [@[@"M", @"CM", @"D", @"CD", @"C", @"XC", @"L", @"XL", @"X", @"IX", @"V", @"IV", @"I"] copy];
    }
    
    NSMutableString *roman = [NSMutableString string];
    
    for (NSInteger i = 0; i < values.count; i++) {
        while (num >= [values[i] integerValue]) {
            num -= [values[i] integerValue];
            [roman appendString:symbols[i]];
        }
    }
    
    return roman;
}

static inline NSString *intToAlphabet(NSInteger num) {
    
    NSMutableString *result = [NSMutableString string];
    
    while (num > 0) {
        num--;
        unichar letter = 'a' + (num % 26);
        [result insertString:[NSString stringWithFormat:@"%c", letter] atIndex:0];
        num /= 26;
    }
    
    return result;
}

static inline CGFloat IndentWithListOption(ZMParagraphListOptions listOption) {
    switch (listOption) {
        case ZMParagraphListQuote:
            return ZMQuoteListDefaultIndent();
        case ZMParagraphListBullet:
            return ZMBulletListDefaultIndent();
        case ZMParagraphListNumber:
            return ZMNumberListDefaultIndent();
        case ZMParagraphListCodeblock:
            return ZMCodeblockListDefaultIndent();
        case ZMParagraphListQuoteWithBullet:
            return ZMQuoteWithOrderListDefaultIndent();
        case ZMParagraphListQuoteWithNumber:
            return ZMQuoteWithOrderListDefaultIndent();
        case ZMParagraphListQuoteWithCodeblock:
            return ZMQuoteWithCodeblockDefaultIndent();
        default:
            break;
    }
    return 0;
}

static inline NSColor *DrawingColorFromListOption(ZMParagraphListOptions listOption)
{
    NSColor *color = ZMInputTextViewDefaultTextColor();
    switch (listOption) {
        case ZMParagraphListQuoteWithBullet:
        case ZMParagraphListQuoteWithNumber:
        case ZMParagraphListQuoteWithCodeblock: {
            color = ZMTextFormatQuoteTextColor();
        }
            break;
            
        case ZMParagraphListCodeblock: {
            color = ZMTextFormatCodeblockTextColor();
        }
            break;
            
        default:
            break;
    }
    
    return color;
}

#endif /* ZMRichFormatConstants_h */
