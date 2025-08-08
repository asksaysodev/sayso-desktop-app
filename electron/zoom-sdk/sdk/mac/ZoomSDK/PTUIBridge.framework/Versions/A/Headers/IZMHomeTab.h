//
//  IZMHomeTab.h
//  PTUIBridge
//
//  Created by Slevin Zhang on 2025/4/25.
//

#ifndef IZMHomeTab_h
#define IZMHomeTab_h

#import <AppKit/AppKit.h>

@protocol IZMOnBoardingController;
@protocol IZMHomeTab <ZMPTRoutableObject, ZMBaseTabsDataProtocol>

- (NSTouchBar *)makeTouchBar;
- (id)listMgr;
- (void)sidebarViewDidChange;
- (void)onCalendarUIReady;
- (void)setupOnboardingChecklist;
- (void)onAICStatusChange;
- (id<IZMOnBoardingController>)onboardingController;

@end

#endif
