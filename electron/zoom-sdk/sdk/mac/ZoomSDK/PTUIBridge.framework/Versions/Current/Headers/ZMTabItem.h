//
//  ZMTabItem.h
//  ZoomDemo
//
//  Created by Huxley Yang on 10/01/2018.
//  Copyright © 2018 Yang. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "ZMUnreadView.h"
#import "ZMMainTabsMoreMenu.h"
#import <ZoomUnit/ZoomUnit.h>
#import <PTUIBridge/IZMMainTabsData.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMTabItemView, ZMTabItem,ZMPTButtonPigment;

@protocol ZMTabItemContentDelegate <NSObject>

- (void)tabItemDidChangeEffectiveStyle:(ZMTabItem *)item;
- (void)onRightMouseDownTabItem:(ZMTabItem *)itemA event:(NSEvent *)event;
- (void)onRightMouseDown:(NSEvent *)event item:(ZMMainTabsMoreMenuItem *)item;

- (NSString *)getAccessibilityValue:(ZMTabItem *)item;
@end

#pragma mark - ZMTabItem

@interface ZMTabItem : NSObject

@property (copy) NSString *text;

@property (copy) NSString *toolTip;

@property (retain) NSImage *image;

@property (nonatomic, weak) id<ZMBaseTabsDataProtocol,ZMPTTabNavigationProtocol> zmtabItemDelegate;

@property (nonatomic, assign) NSInteger badgeValue;

@property (nonatomic, assign) BOOL showError;

@property (nonatomic, assign) BOOL showRedDot;

@property (nonatomic, assign) BOOL showBeta;

@property (nonatomic, assign) BOOL showNew;

@property (nonatomic, assign) BOOL showFreeTrial;

@property (nonatomic, assign) NSInteger tag;
@property (nonatomic, retain) NSString *identifier;

@property (nonatomic, assign) BOOL enable;

@property (nonatomic, assign) BOOL draggable;

@property (nonatomic, assign) BOOL draggingNow;

@property (nonatomic, assign) BOOL isLine;

@property (nonatomic, assign) BOOL isEmpty;

@property (nonatomic, assign) BOOL isPin;

@property (nonatomic, assign) BOOL isSetting;

@property (nonatomic, assign) BOOL isPopout;

@property (nonatomic, assign) BOOL isAlreadyPopout;

@property (nonatomic, assign) BOOL isRegisterTab;

@property (nonatomic, weak) ZMMainTabsMoreMenuItem *menuItem;

@property (nonatomic, retain) ZMPTButtonPigment *pigment;

+ (instancetype)itemWithText:(NSString *)text image:(NSImage *)image identifier:(NSString *)identifier isPin:(BOOL)isPin;
+ (instancetype)itemWithText:(NSString *)text image:(NSImage *)image identifier:(NSString *)identifier isPin:(BOOL)isPin pigment:(ZMPTButtonPigment *)pigment;
+ (instancetype)itemWithText:(NSString *)text image:(NSImage *)image;
+ (instancetype)lineItem;
+ (instancetype)emptyItem;

- (ZMTabItemView *)itemView;

- (ZMUnreadStyle)effectiveStyle;
@property (weak) id <ZMTabItemContentDelegate> contentDelegate;

@end
#pragma mark - ZMTabItemView

@interface ZMTabItemView : PZPTrackingButton

@property (nonatomic,weak,readonly) ZMTabItem *tabItem;

@property (nonatomic, getter=isSelected, assign) BOOL selected;

@property (nonatomic, assign) BOOL isDefaultItem;

@property (nonatomic, assign) BOOL isMoreItem;

@property (nonatomic, assign) BOOL drawDefaultBackground;

@property (nonatomic, retain) NSImage *capture;

@property (nonatomic, assign) CGFloat popoutStartOffSetX;

- (instancetype)initWithTabItem:(ZMTabItem *)tabItem;
- (void)setupSubviews;
- (CGFloat)getFittingWidth;
- (CGFloat)getFittingHeight;
@property (nonatomic, retain, readonly) __kindof NSView *informativeView;
- (void)updateImageRepresentation;

@end

NS_ASSUME_NONNULL_END
