//
//  ZuiNSView.h
//  zUI
//
//  Created by Rob Christenson on 8/27/24.
//

#ifndef ZuiNSView_h
#define ZuiNSView_h

#import <Cocoa/Cocoa.h>

#import <zUI/view.h>
#import <zUI/independent_callbacks.h>

// forward declaration
struct tagzUIClientInteractData;
typedef tagzUIClientInteractData zUIClientInteractData;

ZUI_API
@interface ZuiNSView : NSView
@property (nonatomic, assign) std::shared_ptr<zui::ViewNode> viewPtr;
@property (nonatomic, assign, readonly) std::shared_ptr<zui::View> zuiView;//The blue print C++ class object
@property (nonatomic, assign) zUIClientInteractData telemetryData;

- (instancetype)initWithView:(const zui::View&)zuiView;
- (void)registerOnSizeCallback:(zui::OnSizeUpdateFunc)pFunc;
- (void)registerOnSizeCallbackSimple:(zui::OnSizeUpdateFuncSimple)pFunc;
- (void)registerOnSizeCallbackWithContext:(zui::OnSizeUpdateFuncWithContext)pFunc;
- (void)setTelemetrySubFeatureName:(int)subFeatureName;
- (void)setTelemetryEventSource:(int)eventSource;
- (void)setTelemetryFeatureName:(int)featureName;
- (void)setTelemetryEventLocation:(int)eventLocation;
- (void)setTelemetryEventName:(int)eventName;
- (void)enableTelemetry:(BOOL)enabled;
- (void)setTelemetryData:(int)eventSource featureName:(int)featureName subFeatureName:(int)subFeatureName eventLocation:(int)eventLocation eventName:(int)eventName enableAll:(BOOL)enableAll;

@end


#endif /* ZuiNSView_h */
