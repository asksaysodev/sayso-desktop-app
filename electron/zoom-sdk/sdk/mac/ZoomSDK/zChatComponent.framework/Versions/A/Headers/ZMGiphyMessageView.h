//
//  ZMGiphyMessageView.h
//  ZCommonUI
//
//  Created by groot.ding on 28/07/2020.
//  Copyright © 2020 zoom. All rights reserved.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMPTGiphyData;

@interface ZMChatGiphyButton : NSButton
@property (copy) NSString* sizeString;
@property (assign) BOOL isExpanded;
@property (assign) BOOL isFixedFontSizeRate;
- (void)cleanUp;
- (NSSize)fitSize;
+ (NSSize)fitSizeWith:(NSString*)sizeString fontSizeRate:(NSInteger)fontSizeRate;
@end

@interface ZMGiphyMessageView : NSView

@property (nonatomic, retain) ZMPTGiphyData*       giphyData;
@property (nonatomic, copy) void (^onGiphyExtendChanged)(BOOL isExpanded);
@property (nonatomic, copy) void (^imagePressHandle)(void);
@property (assign) BOOL isFixedFontSizeRate;;
@property (nonatomic, assign) BOOL isSelected;
- (void)updateGiphyViewWithIsExpanded:(BOOL)isExpanded;
- (void)updateGiphyViewWhenTimeOut;
- (NSSize)minSize;

@end

NS_ASSUME_NONNULL_END
