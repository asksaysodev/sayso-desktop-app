//
//  ZMAccessibilityTextField.h
//  zChatUI
//
//  Created by groot.ding on 2019/3/30.
//  Copyright © 2019 Zoom. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface ZMAccessibilityTextField : NSTextField

@property (assign) IBInspectable BOOL accessibilityVisible;

@property (nonatomic,assign) NSRect theAccessibilityFrame;

@property (nonatomic,copy, nullable) IBInspectable NSString *theAccessibilityLabel;
@property (nonatomic,copy, nullable) IBInspectable NSString *theAccessibilityRoleDescription;
@property (nonatomic,copy, nullable) IBInspectable NSString *theAccessibilityHelp;

@end
