//
//  ZMTextFormatAddLinkView.h
//  zChatComponent
//
//  Created by Yong Zhou on 6/22/21.
//  Copyright © 2021 Zoom. All rights reserved.
//

#import "ZMTextFormatEntity.h"

@interface ZMTextFormatAddLinkView : NSView

@property (nonatomic, copy) void (^onSaveUrlBtnClicked)(NSString *linkUrlString, NSAttributedString *contentAttrStr, NSRange selectedRange);
@property (nonatomic, copy) void (^onRemoveBtnClicked)(NSRange linkRange);
@property (nonatomic, copy) void (^checkLinkURLCompletion)(BOOL isValidUrl, BOOL needsClose);
@property (nonatomic, copy) void (^checkURL)(NSString *tmpUrlString);

+ (instancetype)sharedLinkView;

- (instancetype)init NS_UNAVAILABLE;  // Use the designated initializer sharedLinkView
- (instancetype)initWithFrame:(NSRect)frameRect NS_UNAVAILABLE;  // Use the designated initializer sharedLinkView

- (void)showWithUrl:(NSString*)urlString content:(NSAttributedString*)contentAttrStr linkRange:(NSRange)range
        relatedView:(NSView *)relatedView relatedRect:(NSRect)relatedRect;

- (void)close;

@end


