//
//  NSControl+ZoomKit.h
//  ZCommonUI
//
//  Created by Huxley on 2018/4/12.
//  Copyright © 2018 zoom. All rights reserved.
//

#import <ZoomKit/NSView+ZoomKit.h>

NS_ASSUME_NONNULL_BEGIN
@interface NSControl (ZoomKit)

@property (retain, nonatomic) NSCursor * boundsCursor;
@property (readonly, nonatomic) NSParagraphStyle * effectiveParagraphStyle;
@property (nonatomic) NSString * accessibilityDescription;

- (void)sizeToFitWithMinWidth:(CGFloat)minWidth;
- (void)sizeToFitWithMaxWidth:(CGFloat)maxWidth;

@end
NS_ASSUME_NONNULL_END
