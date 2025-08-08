//
//  ZMChatsCommandListWindowCtrl.h
//  zChatUI
//
//  Created by Ryan Shen on 2019/5/8.
//  Copyright © 2019 Zoom. All rights reserved.
//

#import <Cocoa/Cocoa.h>
@class ZMChatsCommandData;

@interface ZMChatsCommandCellView : NSTableCellView

@property (nonatomic, assign) BOOL isSelected;

- (void)updateUI;

@end

@interface ZMChatsCommandListWindow : NSWindow

- (void)invisibleInSharing;

@end

@interface ZMChatsCommandListWindowCtrl : NSWindowController <NSTableViewDelegate, NSTableViewDataSource>

@property (weak) id delegate;

+ (instancetype)defaultWindowController;
+ (ZMChatsCommandData*)getCommandForCommandString:(NSString*)cmdString forSession:(NSString*)sessionID;

- (void)reset;

- (NSString*)selectedCommandString;
- (BOOL)updateDataWithSessionID:(NSString*)sessionID key:(NSString*)key isRobot:(BOOL)isRobot isSelfChat:(BOOL)isSelfChat;
- (void)selectNextCommand;
- (void)selectPreviousCommand;

@end
