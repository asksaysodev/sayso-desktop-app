//
//  ZMRTFShortcutWindowCtrl.h
//  ZoomMsgUI
//
//  Created by Yong Zhou on 4/26/22.
//  Copyright © 2022 Zoom. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <zChatComponent/ZMRichFormatConstants.h>

@class ZMChatAppShortcutInfo;

@protocol ZMRTFShortcutWindowCtrlDelegate <NSObject>
@optional
- (void)onBoldMenuClicked;
- (void)onStrikeThroughMenuClicked;
- (void)onItalicMenuClicked;
- (void)onBulletedListMenuClicked;
- (void)onNumberedListMenuClicked;
- (void)onMoreButtonClicked;
- (void)onAddLinkMenuClicked;
- (void)onChatAppButtonClicked:(ZMChatAppShortcutInfo *)shortcutInfo;
@end

@interface ZMRTFShortcutWindowCtrl : NSWindowController

@property (nonatomic, weak) id<ZMRTFShortcutWindowCtrlDelegate> delegate;
@property (nonatomic, assign) BOOL reserveStatus;
@property (nonatomic, assign) BOOL disableAddLink;
@property (nonatomic, assign) BOOL disableRichFormat;

+ (instancetype)defaultWindowController;

- (void)updateFormatChatApp:(NSArray<ZMChatAppShortcutInfo *> *)shortcutInfoArr;

- (void)setButtonsStatus:(NSInteger)fontStyleInfo;

- (void)selectedNextButton;
- (void)selectedPreviewButton;
- (BOOL)clickSelectedButton;

@end
