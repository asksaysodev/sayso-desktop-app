//
//  ZMTextMessageCellView.h
//  zChatUI
//
//  Created by groot.ding on 2019/5/17.
//  Copyright © 2019 Zoom. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <zChatComponent/ZMNTextMessageCellView.h>
#import <zChatComponent/ZMDeepLinkPreviewView.h>
#import <zChatComponent/ZMNTextMessageCellView.h>

@class ZMChatMsgItemTextView;
@class ZMEditMessageViewController;

@interface ZMTextMessageCellView : ZMNTextMessageCellView <ZMDeepLinkPreviewViewDelegate>

@property (nonatomic,retain) ZMChatMsgItemTextView *linkPreviewTextView;

- (ZMEditMessageViewController *)editMessageViewController;
- (void)shouldUpdateLinkPreview;    // ZOOM-310221
- (BOOL)unreadDotIsHidden;

- (BOOL)needShowMenuForForwardedMessageAtPoint:(NSPoint)point;
- (NSAttributedString *)getForwardMessageSelectedText;
- (void)selectForwardedMessage;

@end
