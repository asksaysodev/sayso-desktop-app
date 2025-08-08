//
//  ZMZappMsgAttachCell.h
//  zChatUI
//
//  Created by TOTTI on 9/21/20.
//  Copyright © 2020 Zoom. All rights reserved.
//
//#import "zoom_client_conf.h"

@interface ZMZappMsgAttachCell : NSTextAttachmentCell

@property (copy) NSString *appId;
@property (copy) NSString *iconPath;
@property (copy) NSString *displayName;
@property (copy) NSString *appName;
@property (copy) NSString *senderName;
@property (assign) BOOL isSender;
@property (assign) NSRect appNameRect;
@property (assign) BOOL isCustomStyle;
@property (assign) NSRect cellFrameRect;

- (void)onMouseClickOnPoint:(NSPoint)point andControlView:(NSTextView *)controlView;

@end
