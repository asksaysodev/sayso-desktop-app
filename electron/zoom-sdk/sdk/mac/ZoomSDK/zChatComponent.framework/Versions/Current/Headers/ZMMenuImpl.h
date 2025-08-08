//
//  ZMMenuImpl.h
//  zChatUI
//
//  Created by groot.ding on 21/08/2020.
//  Copyright © 2020 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMMenuImpl,ZMHMenu,ZMHMenuItem;

@protocol ZMMenuImplProtocal <NSObject>

@optional

- (void)menuImpl:(ZMMenuImpl *)menuImpl willShowOrClose:(BOOL)show;

- (void)menuImpl:(ZMMenuImpl *)menuImpl menuClick:(ZMHMenuItem *)menuItem;

@end

@interface ZMMenuImpl : NSObject <ZMHMenuDelegate>

@property (nonatomic,retain) NSView *postionView;

@property (nonatomic,retain) ZMHMenu *menu;

@property (nonatomic,weak) id <ZMMenuImplProtocal> delegate;

- (void)closeMenu;

@end

NS_ASSUME_NONNULL_END
