//
//  ZPMainWindowProtocol.h
//  PTUIBridge
//
//  Created by fistice on 2/28/25.
//

#import <PTUIBridge/ZMPTRouterCenter.h>

@class ZMToastCoordinator;
@protocol ZPMainWindowProtocol <ZMPTRoutableObject>
@property (readonly) NSWindow *window;
@property (readonly) ZMToastCoordinator *toastCoordinator;
@property (readonly) NSView *bannerView;

@property (readonly, nonatomic) BOOL isWindowHadLoad;//ZOOM-228117

@property (readonly, nonatomic) BOOL isWindowLoaded;
@property (readonly, getter=isCompressed) BOOL compressed;
- (void)toggleCompress:(nullable id)sender;
- (void)decompressWithSize:(CGSize)size completionHandler:(nullable void (^)(void))completionHandler;
- (void)showWindow:(id)sender;
- (void)checkWindowFrameToMinWidth:(CGFloat)minWidth;

@property (strong, readwrite, nullable) NSTouchBar *touchBar;

@end

// #import <PTUIBridge/ZMPTMainWindowMgrProtocol.h>

// toastCoordinator
// ZMPTSharedFor(ZPMainWindowProtocol)
