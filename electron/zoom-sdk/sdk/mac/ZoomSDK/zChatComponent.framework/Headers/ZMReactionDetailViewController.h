//
//  ZMReactionDetailViewController.h
//  zChatUI
//
//  Created by likevin on 2019/7/17.
//  Copyright © 2019 Zoom. All rights reserved.
//

#import <Cocoa/Cocoa.h>
@class ZMReactionEmojiInfo;

@interface ZMReactionDetailViewController : ZMBaseViewController
@property(nonatomic, retain) NSArray*  buddyInfoArray;
@property(nonatomic, retain) ZMReactionEmojiInfo* reactionModel;
@property(nonatomic, assign) BOOL visible;
@property (nonatomic, retain, readonly) NSString *channelID;

- (instancetype)initWithCloseAction:(SEL)closeAction
                             target:(id)target;

- (void)updateUIWithEmojiReactionModel:(ZMReactionEmojiInfo *)reactionModel
                        buddyInfoArray:(NSArray*)jidArray
                        titleInfoArray:(NSArray *)titleArray
                             channelID:(NSString *)channelID
                             messageID:(NSString *)messageID;

/*request callback*/
- (void)onEmojiDetailInfoCallbackWithSessionID:(NSString*)channelID
                                     messageID:(NSString*)messageID
                               emojiIdentifier:(NSString *)emojiIdentifier;

// refresh UI
- (void)updateVisibleUserInfo:(NSArray<NSString *> *)jids shouldUpdateAll:(BOOL)shouldUpdateAll;

@end
