//
//  IZMHomeTab.h
//  PTUIBridge
//
//  Created by Slevin Zhang on 2025/4/25.
//

#ifndef IZMHomeUpcomingViewController_h
#define IZMHomeUpcomingViewController_h

#import <AppKit/AppKit.h>
#import <ZoomKit/ZMBase.h>

@protocol IZMHomeUpcomingViewController <ZMRoutableObject, ZMBaseTabsDataProtocol, ZMViewProviding>

- (void)meetingStateChange:(NSInteger)state;
- (void)transferMeetingsListUpdated;
- (void)updateViewFrame:(NSRect)frame;

@end

#endif
