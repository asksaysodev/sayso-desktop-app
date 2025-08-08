//
//  ZMRichFormatManager.h
//  ChatUI
//
//  Created by Yong Zhou on 8/20/21.
//  Copyright © 2021 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <zChatComponent/ZMRichFormatTextView.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMRichFormatManager : NSObject

/* The designated initializer.
 */
- (instancetype)initWithTextView:(ZMRichFormatTextView*)textView NS_DESIGNATED_INITIALIZER;
- (instancetype)init NS_UNAVAILABLE; // Use the designated initializer initWithTextView:

- (void)onBoldMenuClicked;
- (void)onItalicMenuClicked;
- (void)onUnderlineMenuClicked;
- (void)onStrikeThroughMenuClicked;
- (void)onIncreaseIndentMenuClicked;
- (void)onDecraseIndentMenuClicked;
- (void)onBulletedListMenuClicked;
- (void)onNumberedListMenuClicked;
- (void)onQuotedMenuClicked;
- (void)onInLineCodeMenuClicked;
- (void)onCodeblockMenuClicked;
- (void)textSizeMenuClicked:(ZMTextFormatSizeType)sizeType;
- (void)setTextColorForSelectedString:(ZMTextColorItem *)colorItem;
- (void)setBgColorForSelectedString:(ZMTextColorItem *)colorItem;
- (void)textParagraphMenuClicked:(ZMTextFormatParagraphType)paragraphType;
- (void)clearAllFormat;


@end

NS_ASSUME_NONNULL_END
