//
//  ZMEmojiCollectionViewHeader.h
//  ZoomUnit
//
//  Created by Mario Rao on 2023/7/18.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMEmojiCollectionViewHeader : ZMBaseView

@property (nonatomic, assign) BOOL forceDarkMode;

@property (nonatomic, copy) void (^rightActionBlock)(void);
@property (nonatomic, copy) void (^leftActionBlock)(void);

- (void)updateTitle:(NSString *)title;
- (void)updateLeftActionImage:(NSImage *)image toolTip:(NSString *)toolTip;
- (void)updateRightActionImage:(NSImage *)image toolTip:(NSString *)toolTip;

@end

NS_ASSUME_NONNULL_END
