//
//  ZMStickCheckButton.h
//  zChatUI
//
//  Created by Huxley on 2018/9/12.
//  Copyright © 2018 Zoom. All rights reserved.
//

#import <ZoomKit/ZMCheckButton.h>

@interface ZMStickCheckButton : ZMCheckButton

@property (retain) IBOutlet NSView *stuckView;
@property (copy) IBInspectable NSString *targetSubstring;

- (void)updateStuckView;
- (NSPoint)getStuckViewPositionForCharacterRange;

+ (CGSize)getCellSizeWithTitle:(NSString *)str width:(CGFloat)width height:(CGFloat)height;

@end
