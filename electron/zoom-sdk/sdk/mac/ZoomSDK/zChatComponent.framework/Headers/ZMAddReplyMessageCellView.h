
//
//  ZMAddReplyMessageCellView.h
//  zChatUI
//
//  Created by groot.ding on 2019/5/20.
//  Copyright © 2019 Zoom. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <zChatComponent/ZMMessageCellViewDelegate.h>
#import <zChatComponent/ZMBubbleMessageCellView.h>

@class ZMStickerInfo;
@class ZMFileEntity;
@class ZMEditMessageViewController;
@class ZMAddReplyMessageCellView;
@class ZMInputTextView;

@interface ZMAddReplyMessageCellView : ZMBubbleMessageCellView

//@property (nonatomic,copy) void (^addReplyMessage)(NSTreeNode *item,NSAttributedString *msg);
//
//@property (nonatomic,copy) void (^updateFirstResponse)(NSTreeNode *item,BOOL firstResponse);

@property (nonatomic, readonly) ZMEditMessageViewController *editMessageViewController;
- (void)makeInputViewFirstResponse;

- (void)selectUserWithJid:(NSString*)jid name:(NSString*)name isCheckAtRange:(BOOL)isCheckAt;

- (void)cleanContent;
- (void)sendSuccessAndCleanContent;
- (void)addIntegrationFile:(NSDictionary*)info;

- (void)saveDraftToCloud:(BOOL)forceCloudSync;

- (ZMInputTextView *)inputTextView;

@end
