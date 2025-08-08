//
//  ZMGraphic.h
//  ZMScreenCapture
//
//  Created by javenlee on 2018/2/2.
//  Copyright © 2018 ZOOM. All rights reserved.
//

#import <Cocoa/Cocoa.h>

extern NSString *ZMGraphicFillColorKey;
extern NSString *ZMGraphicStrokeColorKey;
extern NSString *ZMGraphicStrokeWidthKey;
extern NSString *ZMGraphicColorIndexKey;
extern NSString *ZMGraphicBoundsKey;
extern NSString *ZMGraphicDrawingBoundsKey;
extern NSString *ZMGraphicDrawingContentsKey;
extern NSString *ZMGraphicKeysForValuesToObserveForUndoKey;

extern NSString *ZMMosaicProperitesKey;
extern NSString *ZMMosaicGraphicTypeKey;
extern NSString *ZMMosaicGraphicBlueKey;
extern NSString *ZMMosaicGraphicPixelKey;
extern NSString *ZMMosaicGraphicScaleKey;

static CGFloat ZMGraphicHandleWidth = 8.0f;
static CGFloat ZMGraphicHandleHalfWidth = 8.0/2.0f;


typedef NS_ENUM(NSUInteger, ZMGraphicHandle){
    ZMGraphicNoHandle = 0,
    ZMGraphicUpperLeftHandle = 1,
    ZMGraphicUpperMiddleHandle = 2,
    ZMGraphicUpperRightHandle = 3,
    ZMGraphicMiddleLeftHandle = 4,
    ZMGraphicMiddleRightHandle = 5,
    ZMGraphicLowerLeftHandle = 6,
    ZMGraphicLowerMiddleHandle = 7,
    ZMGraphicLowerRightHandle = 8,
};

@interface ZMGraphic : NSObject<NSCopying> {
    
    NSRect      _bounds;
    BOOL        _isDrawingFill;
    BOOL        _isDrawingStroke;
    NSColor*    _fillColor;
    NSColor*    _strokeColor;
    CGFloat     _strokeWidth;
}
@property (nonatomic, assign)NSRect     drawingBounds;
@property (nonatomic, strong)NSMutableDictionary *properties;

//mosaic
@property (assign) NSNumber* mosaicType;
@property (assign) NSNumber* mosaicBlue;
@property (assign) NSNumber* mosaicPixel;
@property (assign) NSNumber* mosaicScale;

+ (void)translateGraphics:(NSArray *)graphics byX:(CGFloat)deltaX byY:(CGFloat)deltaY;

#pragma mark *** Drawing ***

+ (NSSet *)keyPathsForValuesAffectingDrawingBounds;
+ (NSSet *)keyPathsForValuesAffectingDrawingContents;

- (void)drawContentsInView:(NSView *)view isBeingCreateOrEdited:(BOOL)isBeingCreatedOrEditing drawHint:(BOOL)hint;
- (NSBezierPath *)bezierPathForDrawing;
- (void)drawHandlesInView:(NSView *)view;

#pragma mark *** Editing ***

+ (NSInteger)creatingSizingHandle;

//mosaic
- (void)setBlue:(NSNumber *)blue;
- (void)setPixel:(NSNumber *)pixel;
- (void)setType:(NSNumber *)type;
- (void)setScale:(NSNumber *)scale;

- (void)setBounds:(NSRect)bounds;
- (void)setColor:(NSColor *)color;
- (void)setStroke:(NSNumber *)widthNum;
- (NSRect)bounds;
- (BOOL)isDrawingFill;
- (NSColor *)fillColor;
- (BOOL)isDrawingStroke;
- (NSColor *)strokeColor;
- (CGFloat)strokeWidth;
- (NSSize)defaultSize;

- (BOOL)isContentsUnderPoint:(NSPoint)point;
- (NSInteger)handleUnderPoint:(NSPoint)point;
- (BOOL)isHandleAtPoint:(NSPoint)handlePoint underPoint:(NSPoint)point;
- (NSInteger)resizeByMovingHandle:(NSInteger)handle toPoint:(NSPoint)point;
- (NSTextView *)newEditingViewWithSuperviewBounds:(NSRect)superviewbounds;
- (void)finalizeEditingView:(NSView *)editingView;

- (void)updateProperites;
- (void)getProperitesFromDefaults;
+ (void)initWithDefaultProperites;

#pragma mark *** undo ***
- (NSSet *)keysForValuesToObserveForUndo;

@end
