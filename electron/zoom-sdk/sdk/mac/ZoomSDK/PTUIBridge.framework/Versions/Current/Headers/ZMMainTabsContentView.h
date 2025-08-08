//
//  ZMMainTabsContentView.h
//  ChatUI
//
//  Created by fistice on 2024/11/7.
//  Copyright © 2024 Zoom. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "ZMTabItem.h"

NS_ASSUME_NONNULL_BEGIN

typedef NS_OPTIONS(NSUInteger, ZMTabWidgetDragAction) {
    ZMTabWidgetDragActionNone = 0,
    ZMTabWidgetDragPopout = 1,
    ZMTabWidgetDragMergeBack = 2
};

@protocol ZMMainTabsContentViewDelegate <NSObject>
@optional
- (void)moveItemView:(NSView *)view toView:(NSView *)targetView;
- (void)updateViewDraggingState:(NSView *)view isDraggingFromTab:(BOOL)isDraggingFromTab;
- (BOOL)isViewCanDrag:(NSView *)view;
- (void)draggingSessionEnd:(NSView *)view toView:(NSView *)targetView;
- (void)onTabDraggingMergeBack:(ZMTabItem *)tabItem;
- (void)onTabDraggingPopOut:(ZMTabItem *)tabItem;
- (BOOL)isTabCanPop:(ZMTabItem *)tabItem;
- (BOOL)isTabPopped:(ZMTabItem *)tabItem;
- (BOOL)isInMoreMenu;
- (BOOL)isCanPerformDragFromMenu:(id<NSDraggingInfo>)sender;
- (ZMTabWidgetDragAction)checkpopOutTab:(NSDraggingSession *)session;

@end
@interface ZMMainTabsContentView : NSView

@property (weak) id <ZMMainTabsContentViewDelegate> delegate;
@property (weak) ZMTabItemView *draggingView;
@end

NS_ASSUME_NONNULL_END
