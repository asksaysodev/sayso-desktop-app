//
//  ZMInputTextView.h
//  ChatUI
//
//  Created by Yong Zhou on 4/20/22.
//  Copyright © 2022 Zoom. All rights reserved.
//

#import <zChatComponent/ZMRichFormatTextView.h>
#import <zChatComponent/ZMChatsAtListEntity.h>
#import <zChatComponent/ZMCompletionStorage.h>
#import <zChatComponent/ZMInputboxSyncInfo.h>

@class ZMIMMessageDraft, ZMInputTextView;
@class ZMInputBoxLinkInfoSyncedResult;
@class ZMImageNode;

@protocol ZMInputTextViewDelegate <NSObject>
@required
- (BOOL)isFileAccepted;
- (void)textViewDidImportFiles:(NSArray<NSString*>*)files;
- (void)textViewDidImportWebImages:(NSArray<ZMImageNode *> *)images;

- (NSString *)zmInputTextView_currentSessionId;
- (NSString *)zmInputTextView_currentThreadId;
- (NSString *)zmInputTextView_currentMsgId;
- (BOOL)zmInputTextView_enableParseDeeplink;

@optional
- (void)zmInputTextViewDidFindSyncResult:(ZMInputBoxLinkInfoSyncedResult *)linkInfo;
- (void)zmInputTextView_showDisableDeeplink;
- (void)textViewDidChangeResponder:(BOOL)firstResponder;
- (BOOL)zmInputTextView_shouldPasteCustomEmoji:(ZMInputTextView *)inputTextView;
- (void)zmInputTextView_privatePasteDone;
/*!
 @discussion Called when pasting at mentions (ZMPTAtUserCell).
 @return true if the jid should be pasted as an attachment cell, false if a fallback string should be pasted as plaintext
 */
- (BOOL)canPasteAtMentionForJID:(NSString*)jid;
- (BOOL)zmInputTextViewShouldPasteExternalLinks:(ZMInputTextView *)textView;
- (BOOL)zmInputTextViewShouldOpenExternalLinks:(ZMInputTextView *)textView;

// if return YES, textView will not handle the event.
- (BOOL)zmInputTextView:(ZMInputTextView *)textView keyDown:(NSEvent *)theEvent;
- (void)zmInputTextViewDidQuoteMessagerLink:(NSTextView *)textView;
- (void)zmInputTextViewDidHoverLink:(NSString *)linkStr glyphRangeRect:(NSRect)rect;

- (BOOL)zmInputTextViewAllowChanegListIndentWhenPressTabKey:(NSTextView *)textView;

@end

@interface ZMInputTextView : ZMRichFormatTextView

@property (nonatomic, weak) id<ZMInputTextViewDelegate> zmDelegate;
@property (nonatomic, assign) BOOL needsDrawPlaceholder;
@property (nonatomic, assign) BOOL needsParseSystemEmoji;
@property (nonatomic, assign) BOOL isFromPaste;
@property (nonatomic, assign) BOOL appendingSpaceAtTheEndOfSelectedRange;
@property (nonatomic, assign) BOOL shouldParseCommand;
@property (nonatomic, assign) NSRange keyRange; //for showing @list at right position
@property (nonatomic, assign) BOOL shouldShowList;
@property (nonatomic, assign) BOOL needsCallCompletion; // @Ryan.Shen
@property (nonatomic, assign) ZMChatsAtListType nameKeyType;
@property (nonatomic, readonly, strong) NSDictionary *key2EmojiDict;

@property (nonatomic, strong) NSMutableSet *fetchLinkReqIDSet;
@property (nonatomic, strong) NSMutableDictionary<NSString*, NSDictionary<NSString*,ZMInputBoxLinkInfoSyncedResult*>*> *docLinkInfo;
@property (nonatomic, readonly) ZMCompletionStorage *completionStorage;

- (void)clearContent;
- (void)clearContentAndUndo:(BOOL)clearUndo;

/// Will draw custom placeholder. Enable needsDrawPlaceholder if your want to use it.
//- (void)setPlaceholder:(NSString *)placeholder;
//- (void)setAttributedPlaceholder:(NSAttributedString *)zmAttributedPlaceholder;
//- (NSDictionary *)placeholderAttributes;
- (NSDictionary *)getDefaultPlaceholderAttributes;

- (NSString *)fetchSessionId;
- (NSString *)fetchThreadId;
- (NSString *)fetchMsgId;

- (NSString *)contentString;
- (NSString *)contentStringWithEmojiCount:(NSInteger *)count;

- (void)resetToZMAvailableTypingAttribute;

- (BOOL)isMentionCellAtIndex:(NSInteger)index;

- (BOOL)IsValidEmailCharAtIndex:(NSInteger)index;

- (void)parseSystemEmoji;

- (void)parseSticker API_DEPRECATED("Feature removed for several years.", macos(10.0,13.0));

- (void)replaceInputWithEmoji:(NSDictionary *)dict withRange:(NSRange)inRange;

- (BOOL)makeShorterForATAttachment:(NSTextAttachment*)attachment inRange:(NSRange)range;
- (void)updateAtMentionName:(NSArray<NSString *> *)updated;
- (BOOL)updateAtMentionAttachmentSymbol:(NSTextAttachment*)attachment inRange:(NSRange)range showAtSymbol:(BOOL)shouldHaveAtSymbol;

// draft
- (NSString*)getDraftString;

- (NSString *)getSelectedDraftString;

- (void)setDraft:(ZMIMMessageDraft*)draftItem;

- (NSString*)getUserNameWhenSelectionChange;

- (NSString*)getCommandKeyWhenSelectionChange;

- (BOOL)shouldShowZoomCommandList;

- (BOOL)isEditingTail;
- (BOOL)insertCompletionIfNeeds:(NSInteger)key;

@end
