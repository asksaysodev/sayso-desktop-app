//
//  SwiftViewBridge.h
//  zUI
//
//  Created by Rob Christenson on 9/12/24.
//

#ifndef SwiftViewBridge_h
#define SwiftViewBridge_h

#import <Cocoa/Cocoa.h>
#import "SwiftViewInclude.h"

NS_ASSUME_NONNULL_BEGIN

__attribute__((visibility("default")))
@interface SwiftViewBridge : NSObject

// Creates and returns and AppKit NSView corresponding to viewEnum from CustomViews
// CustomViews are declared in "SwiftViewInclude.h". For decl example, see example view 'kIndependentViewExample'
// Usage example in swift: SwiftViewBridge.createView(fromEnum: kIndependentViewExample)
+ (NSView*)createViewFromEnum:(enum CustomViews)viewEnum;

// Registers a swift function callback to be called when the size of an independent view changes
// swift function decl example: func onSizeCallback(width: Float, height: Float, viewContext: UnsafeMutableRawPointer)
// viewContext is a UnsafeMutableRawPointer representing the Obj-C NSView* object.
// To retrieve swift NSView, see 'retrieveViewContext'
// Usage example in swift: SwiftViewBridge.registerCallback(onSizeCallback:onSizeCallback, view:exampleView)
+ (void)registerCallbackOnSizeCallback:(void(float width, float height, void* view))func view:(NSView*)view;

// Accepts void* input from swift OnSizeCallback func and returns the underlying NSView
// Usage example in swift: SwiftViewBridge.retrieveViewContext(viewContext)
+ (NSView*)retrieveViewContext:(void*)context;

@end

NS_ASSUME_NONNULL_END


#endif /* SwiftViewBridge_h */
