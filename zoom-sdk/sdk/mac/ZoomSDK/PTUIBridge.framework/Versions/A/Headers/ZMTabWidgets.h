//
//  ZMTabWidgets.h
//  NewUI
//
//  Created by Huxley Yang on 11/01/2018.
//  Copyright © 2018 Zipow. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "ZMMainTabsMoreMenu.h"
#import "ZMMainTabsContentView.h"

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, ZMTabTipsType) {
    ZMTabTipsTypeFirst,
    ZMTabTipsTypeEmptyMoreMenu,
    ZMTabTipsTypeMustOne
};

typedef NS_OPTIONS(NSUInteger, ZMTabUpdateBits) {
    ZMTabUpdateNone = 0,
    ZMTabUpdateSelectedTabItemChanged = 1 << 0,
    ZMTabUpdateOrderChanged = 1 << 1,
    ZMTabUpdateFrame = 1 << 2,
    ZMTabUpdateWithAnimation = 1 << 3,
    ZMTabUpdateMenu = 1 << 4,
    ZMTabUpdateLayoutDisplayItems = 1 << 5,
    ZMTabUpdateShowTip = 1 << 6,
};

typedef NS_OPTIONS(NSUInteger, ZMTabWidgetLayoutStyle) {
    ZMTabWidgetLayoutHorizontal = 0,
    ZMTabWidgetLayoutVertical = 1
};

@class ZMHTabView, ZMTabItem;

@protocol ZMTabWidgetsDelegate <NSObject>

@optional
//data source delegate
- (BOOL)availabilityOfItem:(ZMTabItem *)item;
- (BOOL)isTabNeedPop:(ZMTabItem *)tabItem;
- (BOOL)isTabLoaded:(ZMTabItem *)tabItem;
- (BOOL)isTabPopped:(ZMTabItem *)tabItem;

//base event
- (void)tabWidgetsOnSearch;
- (void)tabWidgetsOnSetting;
- (void)onTabItemsDidChanged;
- (void)showUpgradeDialogForItem:(ZMTabItem *)item;
- (void)onSelectPreItem:(ZMTabItem *)preItem currentTabItem:(ZMTabItem *)currentTabItem;
- (void)onSelectNextItem:(ZMTabItem *)nextItem currentTabItem:(ZMTabItem *)currentTabItem;

//tab mouse event
- (void)onTabItemRightMouseDown:(ZMTabItem *)tabItem event:(NSEvent *)event;
- (void)onTabItemSelected:(ZMTabItem *)tabItem;
- (void)onTabItemPreSelected:(ZMTabItem *)tabItem;
//Enter from TabItem
- (void)onTabItemDraggingEnteredFromItem:(ZMTabItem *)fromItem sender:(id<NSDraggingInfo>)sender;
//Enter from MenuItem
- (void)onTabItemDraggingEnteredFromMenuItem:(ZMMainTabsMoreMenuItem *)menuItem draggingItem:(NSDraggingItem *)draggingItem;
- (void)onTabItemPerformDragOperation:(id <NSDraggingInfo>)sender;
- (void)onTabItemDraggingEnded:(id <NSDraggingInfo>)sender;
//moreMenu mouse event
- (void)onMoreMenuItemRightMouseDown:(ZMMainTabsMoreMenuItem *)menuItem event:(NSEvent *)event;
- (void)onMoreMenuItemSelected:(ZMHMenuItem *)item;
- (void)onMoreMenuDraggingEnteredFromItem:(ZMTabItem *)fromItem toItem:(ZMTabItem *)toItem inFront:(BOOL)inFront;
- (void)onMoreMenuDraggingMovedToPoint:(NSPoint)screenPoint itemView:(nonnull ZMMainTabsMoreMenuItemView *)itemView session:(NSDraggingSession *)session;
- (void)onMoreMenuPerformDragOperation:(id <NSDraggingInfo>)sender item:(nonnull ZMMainTabsMoreMenuItem *)item;
- (void)onMoreMenuDraggingEnded:(id<NSDraggingInfo>)sender item:(nonnull ZMMainTabsMoreMenuItem *)item;

//mouse event
- (void)onMoveLeft:(ZMTabItem *)currentTabItem preItem:(ZMTabItem *)preItem;
- (void)onMoveRight:(ZMTabItem *)currentTabItem nextItem:(ZMTabItem *)nextItem;

//tips
- (void)setHasShownTabTip;

- (NSString *)getAccessibilityValue:(ZMTabItem *)item;
@end

@interface ZMTabWidgets : NSView

@property (weak) id <ZMTabWidgetsDelegate,ZMMainTabsContentViewDelegate> delegate;

@property (nonatomic, assign) BOOL needShowSearch;
@property (nonatomic, assign) BOOL needShowSetting;
@property (nonatomic, assign) BOOL needPopout;

@property (nonatomic, retain) NSMutableArray <ZMTabItem *> *itemsDisplay;
@property (nonatomic, retain) NSMutableArray <ZMTabItem *> *itemsCollapsed;
@property (nonatomic, strong) NSMutableArray<ZMTabItem *> *tabItems;

@property (nullable, retain) ZMTabItem *selectedTabItem;
@property (nonatomic, retain) ZMTabItem *focusedTabItem;
@property (nonatomic, retain) ZMTabItem *moreItem;
@property (nonatomic, retain) ZMMainTabsMoreMenu *moreMenu;
@property (nonatomic, retain) ZMBubbleObject* bubbleObj;
@property (nonatomic, assign) BOOL needShowMustOneTips;
@property (nonatomic, assign) BOOL dragFromWidget;
@property (nonatomic, assign) ZMTabWidgetLayoutStyle layoutStyle;
@property (nonatomic, strong) ZMMainTabsContentView *contentView;

- (void)updateUI;
- (void)updateUI:(ZMTabUpdateBits)updateBits;
- (void)setNeedsUpdateUI:(ZMTabUpdateBits)updateBits;
- (void)updateSelection;
- (void)moveSelectionToNextPositionRight:(BOOL)rightDirection wrapAround:(BOOL)shouldWrap;//ZOOM-31518
- (void)showFirstActiveTips;
- (void)showMustOneTabTips;
- (void)relayoutMustOneBubble;
- (void)focusFirstItem;
- (void)focusLastItem;
- (void)closeFirstActiveTips;
- (BOOL)isItemCollapsed:(ZMTabItem *)item;
- (BOOL)isMouseInMoreMenu;
- (void)setMoreItemHover:(BOOL)hover;
- (BOOL)isTabPopped:(ZMTabItem *)tabItem;
- (ZMHMenuItem *)menuItemWithTabItem:(ZMTabItem *)tabItem;
- (NSInteger)indexInMore:(ZMTabItem *)item;
- (void)moveSelectionTabAtIndex:(NSUInteger)selectIndex;
- (void)selectTabItem:(ZMTabItem *)tabItem;
- (void)selectTabLeftToRight:(BOOL)direction wrapAround:(BOOL)shouldWrap;
- (void)reloadMenuItemAccessibilityLabel:(ZMHMenuItem*)item;
- (void)onMoreButton:(ZMTabItemView *)sender;
- (BOOL)selectTabWithIdentifier:(NSString *)identifier;
- (nullable ZMTabItem *)getTabItemWithIdentifier:(NSString *)identifier;
@end

NS_ASSUME_NONNULL_END
