//
//  ZMMessageReactionMgr.h
//  zChatUI
//
//  Created by likevin on 2019/5/17.
//  Copyright © 2019 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <zChatComponent/ZMEmojiModel.h>

@class ZMReactionContainarView, ZMIMOnlyImageButton, ZMReactionEmojiInfo;

@interface ZMMessageReactionMgr : NSObject

@property (nonatomic, assign) BOOL canShowCustomEmoji;

+ (ZMMessageReactionMgr*)sharedInstance;
+ (void)releaseInstance;

//- (void)recoverSimpleReactionPanel;

- (NSString *)channelID;

- (void)showReactionPanelWithSender:(NSButton*)sender channelID:(NSString*)channelID messageID:(NSString*)msgID msgItemView:(NSView*)msgItemView;

- (void)scheduleHideReactionPanelWithSender:(NSButton*)sender channelID:(NSString*)channelID messageID:(NSString*)msgID msgItemView:(NSView*)msgItemView;

- (BOOL)isReactionPanelVisibleWithSession:(NSString*)sessionID msgID:(NSString*)msgID;

- (void)closeReactionPanel;

- (void)showEmojiPanel;

- (void)updateEmojiReactionView;

- (void)onReactionWithEmojiString:(NSString*)emojiCode channelID:(NSString*)channelID messageID:(NSString*)msgID;
- (void)onReactionWithEmojiReaction:(ZMReactionEmojiInfo *)reactionInfo toneIndex:(NSInteger)index channelID:(NSString*)channelID messageID:(NSString*)msgID;
- (void)onReactionWithEmojiModel:(ZMCustomEmojiModel *)emojiModel channelID:(NSString*)channelID messageID:(NSString*)msgID;
- (NSArray*)getEmojiDetailInfoWithChanelID:(NSString*)channelID messageID:(NSString*)msgID emojiString:(NSString*)emojiString isLocalOnly:(BOOL)isLocalOnly;
- (NSArray*)getEmojiDetailInfoWithChanelID:(NSString*)channelID messageID:(NSString*)msgID customEmoji:(ZMCustomEmojiModel *)customEmoji isLocalOnly:(BOOL)isLocalOnly;

- (BOOL)isMyEmojiReactionWithChanelID:(NSString*)channelID messageID:(NSString*)msgID emojiString:(NSString*)emojiString;

- (BOOL)isHasReactionWithChanelID:(NSString*)channelID messageID:(NSString*)msgID;

- (BOOL)updateReactionView:(ZMReactionContainarView*)reactionBtnContainer chanelID:(NSString*)channelID messageID:(NSString*)msgID hoverReactionButton:(ZMIMOnlyImageButton*)hoverReactionBtn anchorView:(NSView*)anchorView isShowReaction:(BOOL)bShowReaction;

- (NSInteger)expRecationViewHeightWithChanelID:(NSString*)channelID messageID:(NSString*)msgID maxWidth:(NSInteger)maxWidth outMaxLineWidth:(NSInteger*)maxLineWidth anchorView:(NSView *)anchorView isShowReaction:(BOOL)bShowReaction;

//
- (void)closeEmojiPanel;
- (void)onEmojiWindowClosed;
- (void)onMessageItemViewRemovedWithID:(NSString*)msgID;

@end
