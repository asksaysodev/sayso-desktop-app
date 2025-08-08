//
//  ZMLinkTextField.h
//  ZCommonUI
//
//  Created by groot.ding on 23/04/2020.
//  Copyright © 2020 zoom. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <ZoomKit/ZMLabel.h>
#import <ZoomKit/ZMMutableSelectProtocal.h>
#import <ZoomKit/NSView+ZoomKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMLinkTextField : ZMLabel <ZMMutableSelectProtocal,ZMkeyViewProtocal>

@property (nonatomic, assign) BOOL needDrawFocusRing;
@property (nonatomic, assign) BOOL needDrawFocusRingAttribute;

- (BOOL)zmSelectKeyAttribute:(BOOL)next allowCycle:(BOOL)allowCycle;

@end

NS_ASSUME_NONNULL_END
