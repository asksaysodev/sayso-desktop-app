//
//  ZMBase.h
//  ZCommonUI
//
//  Created by Huxley on 2018/4/11.
//  Copyright © 2018 zoom. All rights reserved.
//

#import <Cocoa/Cocoa.h>

//MARK: - Object
#ifndef ZMCleanUpToNil
#define ZMCleanUpToNil(arg) if (arg) {[arg cleanUp]; arg = nil;}
#endif

#ifndef ZMCleanUpReleaseToNil
#define ZMCleanUpReleaseToNil(arg) if (arg) {[arg cleanUp]; [arg release]; arg = nil;}
#endif

#ifndef ZMReleaseToNil
#define ZMReleaseToNil(arg) if (arg) {[arg release]; arg = nil;}
#endif

#ifndef WEAK_SELF
#define WEAK_SELF __weak __typeof(&*self)weakSelf = self;
#endif

#ifndef Weak
#define Weak(o) __weak __typeof(o) weak##o = o;
#endif

#ifndef Strong
#define Strong(o) __strong __typeof(o) o = weak##o;
#endif

static const NSUInteger ZMByteUnit = 1000; // keep the same with system's unit of calculation

NS_ASSUME_NONNULL_BEGIN

APPKIT_EXTERN CFTypeRef ZMAutorelease(CFTypeRef CF_RELEASES_ARGUMENT arg);

NS_INLINE BOOL ZMAvailableV2(NSUInteger versionMajor, NSUInteger versionMinor) {
	
	static NSInteger verMajor = 10;
	static NSInteger verMinor = 9;
	static BOOL isVersionReady = NO;
	
	if (!isVersionReady) {
        NSProcessInfo* systemInfo = [NSProcessInfo processInfo];
        NSOperatingSystemVersion version = [systemInfo operatingSystemVersion];
        verMajor = version.majorVersion;
        verMinor = version.minorVersion;
		isVersionReady = YES;
	}
	
	if(verMajor > versionMajor)
		return YES;
	if(verMajor == versionMajor && verMinor >= versionMinor)
		return YES;
	
	return NO;
}

NS_INLINE BOOL ZMAvailable(NSUInteger versionMinor) {
	
	return ZMAvailableV2(10, versionMinor);
}
NS_INLINE BOOL ZMAvailableIfEarlierThan(NSUInteger versionMajor, NSUInteger versionMinor) {
    
    return !ZMAvailableV2(versionMajor, versionMinor);
}

typedef NS_ENUM(NSUInteger, ZMUserInterfaceOrientation) {
    kZMUserInterfaceOrientationHorizontal = 0,
    kZMUserInterfaceOrientationVertical = 1
};

@protocol ZMObjectDispose <NSObject>

@required;
- (void)cleanUp;

@end

//MARK: - layout
typedef NS_ENUM(NSInteger, ZMLayoutDirection) {
    ZMLayoutDirectionLeftToRight   = 0,    // Left to right Layout direction
    ZMLayoutDirectionRightToLeft   = 1     // Right to left Layout direction
};


//MARK: - Geometry
typedef NS_OPTIONS(NSUInteger, NSRectCorner) {
    NSRectCornerNotSpecified = 0,
    NSRectCornerTopLeft = 1 << 0,
    NSRectCornerTopRight = 1 << 1,
    NSRectCornerBottomRight = 1 << 2,
    NSRectCornerBottomLeft = 1 << 3,
    NSRectCornerNone = 1 << 4, // No corners is a unique value (instead of 0) for backwards compatibility with other ZM code
    
    NSRectCornerTop = NSRectCornerTopLeft|NSRectCornerTopRight,
    NSRectCornerBottom = NSRectCornerBottomRight|NSRectCornerBottomLeft,
    NSRectCornerLeft = NSRectCornerTopLeft|NSRectCornerBottomLeft,
    NSRectCornerRight = NSRectCornerTopRight|NSRectCornerBottomRight,
    NSRectCornerAll = NSRectCornerTop|NSRectCornerBottom,
};
APPKIT_EXTERN NSRectCorner NSFlippedRectCorner(NSRectCorner corner);
APPKIT_EXTERN CACornerMask NSRectCornerToCACornerMask(NSRectCorner corners);

APPKIT_EXTERN NSRect NSAutoRectInRect(NSRect aRect, NSRect bRect);
APPKIT_EXTERN NSRectCorner NSAutoRectCorner(NSRectCorner corner);

APPKIT_EXTERN BOOL CGFloatEqual(CGFloat a, CGFloat b);
APPKIT_EXTERN BOOL CGFloatLessThan(CGFloat a, CGFloat b);
APPKIT_EXTERN const NSEdgeInsets ZMZeroEdgeInsets;
APPKIT_EXTERN BOOL ZMEdgeInsetsEqual(NSEdgeInsets aInsets, NSEdgeInsets bInsets);

APPKIT_EXTERN NSRect NSEdgeInsetRect(NSRect aRect, NSEdgeInsets edge);
APPKIT_EXTERN NSRect NSEdgeInsetRectV3(NSRect aRect, NSEdgeInsets edge, BOOL isFlipped);

typedef struct _ZMDistance {
    CGFloat x;
    CGFloat y;
} ZMDistance;

APPKIT_EXTERN ZMDistance ZMMakeDistance(CGFloat x, CGFloat y);
APPKIT_EXTERN ZMDistance ZMMakeEvenDistance(CGFloat distance);
APPKIT_EXTERN const ZMDistance ZMZeroDistance;

NS_INLINE NSRect NSOriginSquare(CGFloat length) {
    return NSMakeRect(0, 0, length, length);
};

NS_INLINE NSPoint NSRectCenter(NSRect rect) {
    return NSMakePoint(NSMidX(rect), NSMidY(rect));
}

//based screen coordinate system
APPKIT_EXTERN CGRect NSRect2CGRect(NSRect rect);
APPKIT_EXTERN NSRect CGRect2NSRect(CGRect rect);


APPKIT_EXTERN size_t ZM_Strlen( const char *str);

@protocol ZMMouseEventDelegate <NSObject>

@optional
- (void)sender:(NSView *)sender mouseDown:(NSEvent *)theEvent;
- (void)sender:(NSView *)sender mouseUp:(NSEvent *)theEvent;
- (void)sender:(NSView *)sender mouseHover:(BOOL)hovered;

@end

//MARK: - Utils
@protocol ZMUIlocalizing <NSObject>

- (void)localizingUI;//!< localize itself, subclass should override this method if necessary
- (void)localizingUIRecursively;//!< localize all view hierarchy recursively
@property (assign) BOOL localizingIgnore;

@end

typedef void(^ZMPlainBlock)(void);
APPKIT_EXTERN void CALayerNoActionChanging(NS_NOESCAPE ZMPlainBlock block);//!< disable the CALayer's implicit animation

//MARK: - Common
typedef void(^ZMViewVisibility)(NS_NOESCAPE _Nullable ZMPlainBlock block);

@protocol ZMViewProviding <NSObject> //deprecated

- (nullable NSView *)view;

@property (readonly, nonnull) ZMViewVisibility viewAppear;
@property (readonly, nonnull) ZMViewVisibility viewDisappear;

- (void)effectiveAppearanceDidChange:(NSAppearance *)appearance; //NS_REQUIRES_SUPER

- (void)viewWillMoveToWindow:(nullable NSWindow *)newWindow; //NS_REQUIRES_SUPER
- (void)viewDidMoveToWindow; //NS_REQUIRES_SUPER
- (void)viewWillMoveToSuperview:(nullable NSView *)newSuperview; //NS_REQUIRES_SUPER
- (void)viewDidMoveToSuperview; //NS_REQUIRES_SUPER;
@end

//ZOOM-71195
@protocol ZMForceUpdateUIProtocol <NSObject>
- (void)forceUpdateUIImage;
@end
NS_ASSUME_NONNULL_END
