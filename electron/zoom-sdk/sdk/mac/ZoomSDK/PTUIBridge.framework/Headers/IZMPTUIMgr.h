//
//  IZMPTUIMgr.h
//  PTUIBridge
//
//  Created by fistice on 2024/10/23.
//

#ifndef IZMPTUIMgr_h
#define IZMPTUIMgr_h

@class ZMToastCoordinator;

@protocol IZMPTUIMgr <NSObject>
- (NSWindow *)responsibleWindow;
- (ZMToastCoordinator *)toastCoordinator;
- (void)showFeedbackWindow;
@end
//ZMSharedFor(IZMPTUIMgr)
#endif /* IZMPTUIMgr_h */
