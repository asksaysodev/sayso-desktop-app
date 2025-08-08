//
//  ZMEmojiReactionView.h
//  zChatUI
//
//  Created by likevin on 2019/5/17.
//  Copyright © 2019 Zoom. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface ZMEmojiReactionView : NSView

- (instancetype)initEmojiReactionView;
- (void)reloadDataForReactionContainerView;
- (void)viewDidShowWithSelectedEmojiStringArray:(NSArray *)selectedEmojiStringArray sessionId:(NSString *)sessionId;
- (void)onWindowDidClosed:(id)sender;
@end

