//
//  ZMTabButton.h
//  zChatUI
//
//  Created by Huxley Yang on 7/12/23.
//

#import <Cocoa/Cocoa.h>

@class ZMTabControlItem;

NS_ASSUME_NONNULL_BEGIN

@interface ZMTabButton : ZPTrackingButton

+ (instancetype)tabButtonWithItem:(ZMTabControlItem *)item;

@property (strong, nonatomic) ZMTabControlItem *item;

@property (nonatomic, getter=isSelected) BOOL selected;

@property (copy, nonatomic) NSString *accessibilityIndexDescription;

- (void)updateUI;

@end

NS_ASSUME_NONNULL_END
