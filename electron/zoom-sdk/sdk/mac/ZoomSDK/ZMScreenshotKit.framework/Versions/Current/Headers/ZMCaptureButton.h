//
//  ZMCaptureButton.h
//  ScreenCapture
//
//  Created by javenlee on 2018/3/20.
//  Copyright © 2018 ZOOM. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface ZMCaptureButtonCell : NSButtonCell

@end

#pragma mark -- ZMBackgroundButton
@interface ZMBackgroundButton : NSButton
{
    BOOL    _hovered;
    NSTrackingArea* _trackingArea;
}
@property(nonatomic, assign) BOOL hovered;
@property(nonatomic, strong) NSTrackingArea* trackingArea;
@end

#pragma mark *** Cancel Button ***
@interface ZMCancelButton: ZMBackgroundButton

@end

#pragma mark -- ZMCaptureButton
@interface ZMCaptureButton : ZMBackgroundButton

@property(nonatomic, assign) BOOL isSelected;

@end

#pragma mark -- ZMColorButton
@interface ZMColorButton : NSButton

@property(nonatomic, strong)NSColor *fillColor;
@property(nonatomic, strong)NSColor *borderColor;
@property(nonatomic, assign) BOOL isSelected;
@end

#pragma mark -- ZMStrokeWidthButton
@interface ZMStrokeWidthButton : ZMColorButton

@property(nonatomic, assign)float strokeWidth;
@end

@interface ZMMosaicColorButton : ZMBackgroundButton
@property(nonatomic, strong)NSColor *fillColor;
@property(nonatomic, strong)NSColor *borderColor;
@property(nonatomic, assign) BOOL isSelected;
@end

