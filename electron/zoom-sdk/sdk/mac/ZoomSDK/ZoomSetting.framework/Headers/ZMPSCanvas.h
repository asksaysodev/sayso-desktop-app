//
//  ZMPSCanvas.h
//  ZoomClips
//
//  Created by Michael Lin on 2023/1/4.
//

#import <Foundation/Foundation.h>
#import <ZoomSetting/ZMPSObject.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMPSCanvas, ZMPSRender;
@interface ZMPSCanvas : ZMPSObject

@property (nonatomic, weak) ZMPSCanvas *parentCanvas;
@property (nonatomic, assign) NSRect frame;
@property (nonatomic, weak, nullable) NSColor *backgroundColor;
@property (nonatomic, assign) BOOL backgroundTransparent;
- (nullable ZMPSCanvas *)generateShareSubCanvasWithFrame:(NSRect)frame;
- (nullable ZMPSCanvas *)generateVideoSubCanvasWithFrame:(NSRect)frame;
- (void)addBackgroundPic:(nullable NSImage *)pic;
- (void)removeBackgroundPic;
- (void)addFilterPic:(nullable NSImage *)pic;
- (void)removeFilterPic;
- (void)addTopPic:(nullable NSImage *)pic;
- (void)removeTopPic;
- (void)addSharePic:(nullable NSImage *)pic;
- (void)removeSharePic;
- (void)bringToTop;
- (void)bringToBottom;
- (void)bringToUpper;
- (void)bringToLower;

@end

@interface ZMPSRootCanvas : ZMPSCanvas

- (void)run;
- (void)stop;
- (void)freezeRendering;
- (void)thawRendering;
- (void)redraw;
- (void)publish;
- (void)stopPublish;

@end

NS_ASSUME_NONNULL_END
