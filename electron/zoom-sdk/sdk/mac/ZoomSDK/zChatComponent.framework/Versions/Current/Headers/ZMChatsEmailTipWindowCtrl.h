//
//  ZMChatsEmailTipWindowCtrl.h
//  zChatUI
//
//  Created by Ryan Shen on 15/12/20.
//  Copyright © 2020 Zoom. All rights reserved.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN


@interface ZMChatsEmailTipView : NSView

@property (nonatomic, assign) BOOL isBradeless;

@end


@interface ZMChatsEmailTipWindowCtrl : NSWindowController

@property (nonatomic, weak, nullable) id actionDelegate;
@property (nonatomic, assign) float maxWidth;
@property (nonatomic, assign) float minWidth;

+ (instancetype)defaultWindowController;
- (void)setContent:(NSString*)content;
- (void)setBradelessType:(BOOL)isBradeless;
- (void)readContent;

@end

NS_ASSUME_NONNULL_END
