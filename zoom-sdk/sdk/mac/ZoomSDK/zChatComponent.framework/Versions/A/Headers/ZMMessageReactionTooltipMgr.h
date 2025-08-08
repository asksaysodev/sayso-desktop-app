//
//  ZMMessageReactionTooltipMgr.h
//  zChatUI
//
//  Created by likevin on 2019/7/15.
//  Copyright © 2019 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>

@class ZMReactionItemView, ZMReactionEmojiInfo, ZMEmojiHoverView;


@interface ZMMessageReactionTooltipMgr : NSObject
+ (ZMMessageReactionTooltipMgr*)sharedInstance;
+ (void)releaseInstance;

- (void)showToolTipWithSessionID:(NSString*)inSID messageID:(NSString*)inMsgID reactionView:(ZMReactionItemView*)inReactionView;
- (void)hideTooltipWithReactionView:(ZMReactionItemView*)inReactionView;

- (void)onTooltipWindowDidClosed:(id)sender;
- (void)updateReactionListWithSessionId:(NSString *)sessionId;
@end



@interface ZMReactionTooltipView : NSView

@property (nonatomic,strong) ZMEmojiHoverView *emojiView;

- (void)updateUIWithTooltipArray:(NSArray<NSString*>*) tooltipArray reactionInfo:(ZMReactionEmojiInfo *)reactionInfo;
- (void)onWindowDidClosed:(id)sender;
@end
