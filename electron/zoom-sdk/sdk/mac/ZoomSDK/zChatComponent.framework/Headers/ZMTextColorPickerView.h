//
//  ZMTextColorPickerView.h
//  zChatComponent
//
//  Created by Yong Zhou on 6/1/21.
//  Copyright © 2021 Zoom. All rights reserved.
//

#import "ZMTextFormatEntity.h"

@interface ZMTextColorPickerView : NSView
@property (nonatomic, readonly, assign) NSUInteger currentTextColorIndex;
@property (nonatomic, readonly, assign) NSUInteger currentBgColorIndex;
@property (nonatomic, copy) void (^onTextColorBtnClicked)(ZMTextColorItem *colorItem);
@property (nonatomic, copy) void (^onBgColorBtnClicked)(ZMTextColorItem *colorItem);

+ (instancetype)sharedColorPickerView;

- (instancetype)init NS_UNAVAILABLE;  // Use the designated initializer sharedColorPickerView
- (instancetype)initWithFrame:(NSRect)frameRect NS_UNAVAILABLE;  // Use the designated initializer sharedColorPickerView

- (void)showRelativeToRect:(NSRect)positioningRect toView:(NSView *)positioningView
                  rectEdge:(NSRectEdge)rectEdge alignment:(ZMPopoverAlignment)alignment;

- (void)selectTextColorWithColorStr:(NSString *)colorStr;
- (void)selectBgColorWithColorStr:(NSString *)colorStr;

- (void)close;

@end



