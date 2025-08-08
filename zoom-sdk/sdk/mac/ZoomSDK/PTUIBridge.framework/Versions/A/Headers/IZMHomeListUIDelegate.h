//
//  IZMHomeListUIDelegate.h
//  PTUIBridge
//
//  Created by Slevin Zhang on 2025/4/25.
//

#ifndef IZMHomeListUIDelegate_h
#define IZMHomeListUIDelegate_h

@protocol IZMHomeListUIDelegate

- (BOOL)isUIReady;
- (BOOL)isSidebarItemDisplayed;
- (BOOL)shouldShowNarrowList;
- (BOOL)hasTransferMeetings;

- (BOOL)showMeetingsOnView:(NSView *)view;
- (void)showMeetingsOnNarrowView:(NSView *)view;

@end

#endif
