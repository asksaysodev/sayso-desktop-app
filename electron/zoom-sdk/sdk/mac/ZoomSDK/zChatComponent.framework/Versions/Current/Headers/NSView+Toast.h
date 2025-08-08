//
//  NSView+Toast.h
//  zChatComponent
//
//  Created by Zoro.Fu on 2022/9/29.
//

#import <Cocoa/Cocoa.h>
#import <ZoomUnit/ZoomUnit-Swift.h>

NS_ASSUME_NONNULL_BEGIN

@interface NSView (Toast)

- (BOOL)dispatchToast:(ZMToast *)toast fromView:(NSView *_Nullable)view;

@end

@interface NSWindow (Toast)

- (ZMToastCoordinator *)currentToastCoordinator;

- (BOOL)dispatchToast:(ZMToast *)toast fromView:(NSView *_Nullable)view;

- (BOOL)retrieveToast:(ZMToast *)toast;

@end

NS_ASSUME_NONNULL_END
