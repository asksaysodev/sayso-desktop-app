//
//  NSMutableAttributedString+RichFormat.h
//  ChatUI
//
//  Created by Yong Zhou on 8/20/21.
//  Copyright © 2021 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <zChatComponent/ZMRichFormatConstants.h>

NS_ASSUME_NONNULL_BEGIN

@interface NSMutableAttributedString (RichFormat)

- (void)setZMBold:(BOOL)isBold forRange:(NSRange)range;

/// set italic font for selected range
/// - Parameters:
///   - range: selected range
///   - enableToogle: NO -> will always set selected font as italic; YES -> will set selected font to opposite italic property.
- (void)setZMItalicInRange:(NSRange)range enableToogle:(BOOL)enableToogle;

- (void)setZMUnderline:(BOOL)isUnderline forRange:(NSRange)range;

- (void)setZMInLineCode:(BOOL)isInLine forRange:(NSRange)range;

- (void)setZMStrikethrough:(BOOL)isStrikethrough forRange:(NSRange)range;

- (void)setParagraphIndent:(CGFloat)indent
      needsCalculateIndent:(BOOL)needsCalculateIndent
                isIncrease:(BOOL)isIncrease
                  forRange:(NSRange)range;

- (void)setParagraphIndentLevel:(NSInteger)indentLevel forRange:(NSRange)range;

- (void)setParagraphListType:(ZMTextFormatListType)listType
           codeblockLanguage:(NSString* _Nullable)displayLanguage
                     enabled:(BOOL)enabled
                    forRange:(NSRange)range
          follow1stParagraph:(BOOL)follow1stParagraph
     shouldSeperateParagraph:(BOOL)shouldSeperateParagraph;

- (void)setParagraphListType:(ZMTextFormatListType)listType
                     enabled:(BOOL)enabled
                    forRange:(NSRange)range
          follow1stParagraph:(BOOL)follow1stParagraph
     shouldSeperateParagraph:(BOOL)shouldSeperateParagraph;

- (NSInteger)getParagraphIndentLevelWithRange:(NSRange)range;

- (void)setParagraphHeadingType:(ZMTextFormatParagraphType)type 
         needsCalculateFontSize:(BOOL)needsCalculateFontSize
                       forRange:(NSRange)range
        shouldSeperateParagraph:(BOOL)shouldSeperateParagraph;

- (NSRange)safeLineRangeFromEffectiveRange:(NSRange)effectiveRange 
                                startIndex:(NSUInteger)startIndex
                                  endIndex:(NSUInteger)endIndex;

- (NSRange)longestEffectiveParagraphRangeForRange:(NSRange)range;

- (NSRange)maxSuccessiveRangeForParagraphListStyle:(ZMTextFormatListType)style atIndex:(NSUInteger)index;

- (NSUInteger)orderListIndexFromRange:(NSRange)charRange indentLevel:(NSInteger)indentLevel;

/// separate the paragraph range to many sub-paragraph ranges
/// - Parameter range: adjacent quote range
- (NSArray<NSValue*>*)subQuoteRangesFromRange:(NSRange)range;

- (void)adjustCodeblockStyleWithTypingRangeValue:(NSValue* _Nullable)typingCodeblockRangeInfo
                                needsMergeRanges:(BOOL)needsMerge;

- (void)adjustParagraphStyle;

- (void)adjustListParagraphStyle;

- (BOOL)haveListParagraphStyle;

- (BOOL)isLineBreakOnlyLineAtIndex:(NSInteger)index;

- (BOOL)isListParagraphStyleAtLocation:(NSInteger)location;

- (NSRange)foundContinuousListParagraphStyle:(NSInteger)location;

@end

NS_ASSUME_NONNULL_END
