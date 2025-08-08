//
//  ZMRichFormatTextView.h
//  ChatUI
//
//  Created by Yong Zhou on 8/19/21.
//  Copyright © 2021 Zoom. All rights reserved.
//

#import <ZoomKit/ZoomKit.h>
#import <zChatComponent/ZMTextFormatEntity.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_OPTIONS(NSUInteger, ZMRTFResetTypingEventOperation) {
    ZMRTFResetTypingEventKeydownDelete = 1 << 0,
    ZMRTFResetTypingEventKeydownInsert = 1 << 1,
};

@interface ZMRichFormatTextView : ZMTextView

@property (nonatomic, assign) BOOL allowsZMMarkdown;
@property (nonatomic, assign) BOOL allowsHyperlink;
@property (nonatomic, assign) BOOL allowsCodeblock;
@property (nonatomic, assign, readonly) BOOL hasCodeblock;
@property (nonatomic, copy) void(^RTFChangedByKeyDoneEvent)(void);
/// cmd+k to trigger hyperLink
@property (nonatomic, copy) void(^OnHyperlinkHotKeyEvent)(void);
/// Fix system typingAttributes bug which is cached when there has markedText
@property (nonatomic, copy) NSDictionary<NSAttributedStringKey, id> *zm_unmarkTextTypingAttributes;

- (void)updateInsertionPointColor;

- (NSDictionary<NSAttributedStringKey, id> *)zmDefaultTypingAttributes;
/// Will also reset insertionPointColor to defaut value of ZMInputTextViewDefaultTextColor().
- (void)setDefaultTypingAttributes;

/// When string length is deleted from 1 to 0, will remain typingAttributes of List-format(currnetly order/unorder list). (Follow Slack&Teams)
/// List-format should only be cancel manually (e.g. "Enter" or "delete" key when current line has no content )
- (void)keepListFormatTypingAttributesWhenNoContentExist;

- (NSUInteger)enabledFormats;
- (NSUInteger)fontStyleInfo;
- (ZMTextFormatInfo*)textFormatInfo;

- (CGFloat)curAppFontSize;

- (void)boldSelectedString;
- (void)italicSelectedString;
- (void)underlineSelectedString;
- (void)strikeThroughSelectedString;
- (void)inLineSelectedString;
- (void)setColorForSelectedString:(ZMTextColorItem *)colorItem setColorType:(ZMTextFormatTextColorType)colorType;
- (void)textSizeMenuClicked:(ZMTextFormatSizeType)sizeType;
- (void)listSelectedParagraphWithType:(ZMTextFormatListType)listType;
- (void)adjustSelectedParagraphIndent:(BOOL)increaseIndent;
- (void)onTextFormatLinkUrlConfirmed:(NSString *)linkUrlString 
                       contentString:(NSAttributedString *)contentAttrStr
                       selectedRange:(NSRange)selectedRange;
- (void)onTextFormatLinkUrlRemoved:(NSRange)range;
- (void)setParagraphHeadingType:(ZMTextFormatParagraphType)paragraphType;
- (void)clearAllFormat;

- (void)quoteAsInputTextWithSelectedMessage:(NSAttributedString *)messageString;

- (BOOL)removeListTypingAttributesIfNeededAtKeydownOperation:(ZMRTFResetTypingEventOperation)operation;

- (void)convertContentFontSize;

- (BOOL)isSucceedToParseHighlightLinkFromPasteString:(NSString*)content inRange:(NSRange)inRange;

- (void)setCurColorRange:(NSRange)range;

- (ZMParagraphRTFStyle)getSelectedParagraphRTFStyle;
- (BOOL)isQuoteStyleAtSelectedRange;
- (BOOL)isCodeblockFormatAtBeginning;

- (void)requestParsingHLCodeblockToLanguage:(NSString*)displayLanguage inRange:(NSRange)codeRange;

/// When text color is specified( such as Quote, Codeblock Format), there is no need to rectify typing color.
- (BOOL)isSpecifiedTextColorStyleAtSelectedRange;

- (void)checkIfNeedsShowLanguageSelectionForCodeblockAtIndex:(NSInteger)index isCodeblockBegin:(BOOL)isCodeblockBegin;

- (void)updateReplacementString:(NSString* _Nullable)replacementString;

- (BOOL)allowChanegListIndentWhenPressTabKey;

@end

NS_ASSUME_NONNULL_END
