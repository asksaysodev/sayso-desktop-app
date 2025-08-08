//
//  ZMMoveableButton.m
//  ZoomKit
//
//  Created by Francis Zhuo on 9/3/21.
//

#import "ZMMoveableButton.h"
@interface ZMMoveableButton()
@property(assign) BOOL isMouseDown;
@property(assign) NSPoint mouseDownPosition;
@end

@implementation ZMMoveableButton

- (void)mouseDown:(NSEvent *)event{
    self.isMouseDown = YES;
    self.mouseDownPosition = NSEvent.mouseLocation;
    [super mouseDown:event];
}

- (BOOL)sendAction:(SEL)action to:(id)target{
    if(self.isMouseDown){
        self.isMouseDown = NO;
        
        NSPoint point = NSEvent.mouseLocation;
        NSRect rect = NSZeroRect;
        rect.origin = self.mouseDownPosition;
        rect = NSInsetRect(rect, -2, -2);
        
        if(NSPointInRect(point, rect)){
            return [super sendAction:action to:target];
        }
        else{
            return YES;
        }
    }
    else{
        return [super sendAction:action to:target];
    }
}

- (BOOL)mouseDownCanMoveWindow{
    return YES;
}

@end
