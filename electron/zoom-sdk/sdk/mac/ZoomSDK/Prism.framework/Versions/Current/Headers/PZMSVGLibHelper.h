//
//  ZMSVGLibHelper.h
//  SVGTools
//
//  Created by Jed Zheng on 12/12/2019.
//  Copyright © 2019 jed. All rights reserved.
//

#import <AppKit/AppKit.h>
#import <Prism/PZMSvgElement.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, ZMSVGFillStrokeType) {
    ZMSVGFillStrokeTypeNone = 0,
    ZMSVGFillStrokeTypeSystem,
    ZMSVGFillStrokeTypeCustom
};

__attribute__((visibility("hidden")))
@interface PZMSVGBaseElement : NSObject
@property(nonatomic, copy) NSString *identifier;
@property(nonatomic, retain) NSAffineTransform *transform;
@property(nonatomic, retain) NSColor *fillColor;
@property(nonatomic, retain) NSColor *strokColor;
@property(nonatomic, copy) NSString *fillColorStr;
@property(nonatomic, copy) NSString *strokColorStr;
@property(nonatomic, assign) CGFloat opacity;
@property(nonatomic, assign) CGFloat fillOpacity;
@property(nonatomic, assign) CGFloat strokeOpacity;
@property(nonatomic, copy) NSString *fillElementIdentifier;
@property(nonatomic, copy) NSString *strokeElementIdentifier;
@property(nonatomic, copy) NSString *clipPathIdentifier;
@property(nonatomic, copy) NSString *maskIdentifier;
@property(nonatomic, assign) CGFloat strokeWidth;
@property(nonatomic, assign) CGFloat strokeDashOffset;
@property(nonatomic, retain) NSMutableArray<NSString *> *strokeDashArray;
@property(nonatomic, assign) NSLineJoinStyle strokeLineJoin;
@property(nonatomic, assign) NSLineCapStyle strokeLineCap;
@property(nonatomic, assign) CGFloat miterLimit;
@property(nonatomic, assign) NSWindingRule fillRule;
@property(nonatomic, retain) NSColor *stopColor;
@property(nonatomic, assign) CGFloat stopOpacity;
@property(nonatomic, assign) CGFloat stopOffset;
@property(nonatomic, assign) ZMSVGFillStrokeType hasFill;
@property(nonatomic, assign) ZMSVGFillStrokeType hasStroke;
@property(nonatomic, assign) BOOL visible;

@property(nonatomic, assign) NSPoint ctrlPoint;
@property(nonatomic, retain) NSBezierPath *shape;
@property(nonatomic, retain) PZMSVGBaseElement *fillElement;
@property(nonatomic, retain) PZMSVGBaseElement *strokeElement;
@property(nonatomic, retain) NSArray<PZMSVGBaseElement *> *clipElements;
@property(nonatomic, retain) NSArray<PZMSVGBaseElement *> *maskElements;
@property(nonatomic, retain) PZMSVGBaseElement *parentElement;

@property(nonatomic, assign) BOOL useDefaultStrokeWidth;

- (instancetype)initWithAttributes:(nonnull NSDictionary<NSString *,NSString *> *)attributeDict;

+ (NSBezierPath *)transformPath:(NSBezierPath *)path scale:(CGFloat)scale;
@end

__attribute__((visibility("hidden")))
@interface PZMSVGLibHelper : NSObject
+ (NSImage* _Nullable)exportImageWithPath:(NSString *)path;
+ (NSImage* _Nullable)exportImageWithElement:(PZMSvgElement *)element;
+ (NSImage* _Nullable)exportImageWithElement:(PZMSvgElement *)element scale:(CGFloat)scale;
+ (NSBezierPath * _Nullable)exportFirstPathWithElement:(PZMSvgElement *)element scale:(CGFloat)scale;
@end

@interface NSColor (Utilities)

+ (NSColor *)hexColorWithString:(NSString *)hex;

@end

NS_ASSUME_NONNULL_END
