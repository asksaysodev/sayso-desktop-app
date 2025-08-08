//
//  ZMHTextField.h
//  ZCommonUI
//
//  Created by Huxley on 2020/4/5.
//  Copyright © 2020 zoom. All rights reserved.
//

#import <ZoomKit/ZMTextField.h>
#import <ZoomKit/ZMLabel.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMHTextField : ZMTextField

//Input Alert
@property (assign) NSUInteger alertCount;
@property (assign) NSPoint alertPosition;
@property (copy,nullable) NSColor *defaultColor;
@property (copy,nullable) NSColor *alertColor;
@property (assign, readonly) BOOL isAlert;

@end

@interface ZMAccTextField : ZMLabel

@property (copy,nullable) NSString *linkURL;
@property (assign) BOOL handleOpenLinkOnTarget;

@end

NS_ASSUME_NONNULL_END
