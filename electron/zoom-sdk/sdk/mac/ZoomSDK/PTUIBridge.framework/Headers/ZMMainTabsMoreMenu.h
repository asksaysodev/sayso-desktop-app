//
//  ZMMainTabsMoreMenu.h
//  zChatUI
//
//  Created by Huxley Yang on 10/11/22.
//  Copyright © 2022 Zoom. All rights reserved.
//

#import <ZoomKit/ZoomKit.h>
@class ZMMainTabsMoreMenuItem;
@class ZMTabItem;
NS_ASSUME_NONNULL_BEGIN

@class ZMTabItem;
@interface ZMMainTabsMoreMenuItemView : ZMHMenuItemView


@property (weak) ZMMainTabsMoreMenuItem *representedItem;
@property (nonatomic, retain) NSImage *capture;
- (ZMTabItem *)tabItem;

@end

@protocol ZMMainTabsMoreMenuItemActionProtocol <NSObject>

- (void)onRightMouseDown:(NSEvent *)event item:(ZMMainTabsMoreMenuItem *)item;
- (void)onDraggingEnteredFromItem:(ZMTabItem *)fromItem toItem:(ZMTabItem *)toItem inFront:(BOOL)inFront;
- (void)onDraggingMovedToPoint:(NSPoint)screenPoint itemView:(ZMMainTabsMoreMenuItemView *)itemView session:(NSDraggingSession *)session;
- (void)onPerformDragOperation:(id <NSDraggingInfo>)sender item:(ZMMainTabsMoreMenuItem *)item;
- (void)onDraggingEnded:(id<NSDraggingInfo>)sender item:(ZMMainTabsMoreMenuItem *)item;

@end

@interface ZMMainTabsMoreMenu : ZMHMenu

@end

@interface ZMMainTabsMoreMenuItem : ZMHMenuItem

@property (nonatomic, strong,nullable) NSImage *hoverImage;

@property BOOL asDraggingImage;

@property (nonatomic, assign) BOOL draggingNow;

@property (nonatomic, weak) id<ZMMainTabsMoreMenuItemActionProtocol> itemActionDelegate;

- (ZMTabItem *)tabItem;

@end



NS_ASSUME_NONNULL_END
