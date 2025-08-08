//
//  ZMTabControl.h
//  zChatUI
//
//  Created by Huxley Yang on 7/4/23.
//

#import <Cocoa/Cocoa.h>
#import <zChatComponent/ZMTabControlMenu.h>

NS_ASSUME_NONNULL_BEGIN

APPKIT_EXTERN NSString * const kZMChatTabControlDragUTI;

@interface ZMTabControlItem : NSObject <NSCopying>

@property (nonatomic, copy, nullable) NSString *title;
@property (nonatomic, copy, nullable) NSString *tooltip;

@property (nonatomic, copy, nullable) NSString *accessibilityTitle;

@property (nonatomic, copy, nullable) NSImage *icon;
@property (nonatomic, copy, nullable) NSImage *alternativeIcon;
@property (nonatomic, copy, nullable) NSString *iconURL;
@property (nonatomic) BOOL progressing;

@property (nonatomic, strong, nullable) ZMHMenu *menu;
@property (nonatomic) BOOL menuOnly;//no action, just show menu only

@property (nullable, weak) id target;
@property (nullable) SEL action;

@property (nonatomic, assign) BOOL expandForUnread;
@property (nonatomic, assign) BOOL hasUnread;
@property (nonatomic, getter=isEnable) BOOL enable;
@property (nonatomic, getter=isDraggable) BOOL draggable;
@property (nonatomic, getter=isSelectable) BOOL selectable;

@property (nonatomic, getter=isCloseable) BOOL closeable;
@property (nullable) SEL closeAction;

@property (nonatomic, strong, nullable) id representedObject;

+ (instancetype)itemWithTitle:(nullable NSString *)title icon:(nullable NSImage *)icon;
+ (instancetype)itemWithTitle:(nullable NSString *)title icon:(nullable NSImage *)icon menu:(nullable ZMHMenu *)menu;
+ (instancetype)itemWithMenu:(ZMHMenu *)menu;//menuOnly = YES, icon = NewUI_Ellipsis

@end

@class ZMTabControl;

@protocol ZMTabControlDelegate<NSObject>

@optional

- (void)tabControl:(ZMTabControl *)tabControl didChangeSelectionTo:(ZMTabControlItem *)item byUserClick:(BOOL)byUserCick;
- (void)tabControl:(ZMTabControl *)tabControl didClickedItem:(ZMTabControlItem *)item;//for item cannot select

- (void)tabControlWantsReorderTo:(NSArray <ZMTabControlItem *> *)items;

@end


@interface ZMTabControl : NSView

@property (weak) id <ZMTabControlDelegate> delegate;
@property (nonatomic, strong, nullable) ZMTabControlItem *itemAddNew;
@property (nonatomic) BOOL showAddNew;
@property (nonatomic, getter=isDraggable) BOOL draggable;

@property (nonatomic, copy) NSArray <ZMTabControlItem *> *items;

@property (strong, readonly) ZMTabControlItem *itemSelected;
@property (nonatomic) NSPoint startPoint;

- (void)reload;

- (void)selectItem:(ZMTabControlItem *)item;

- (void)layoutTabButtonsWithAnimate:(BOOL)animate;

- (__kindof NSButton *)buttonWithItem:(ZMTabControlItem *)item;

@end

NS_ASSUME_NONNULL_END
