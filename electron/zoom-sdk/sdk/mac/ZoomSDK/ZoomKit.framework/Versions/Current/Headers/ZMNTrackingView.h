//
//  ZMNTrackingView.h
//  zChatUI
//
//  Created by groot.ding on 2019/6/13.
//  Copyright © 2019 Zoom. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <ZoomKit/ZMBaseView.h>

@interface ZMNTrackingView : ZMBaseView

@property (copy) void (^mouseLocationChange)(BOOL isEntered);

- (BOOL)isCurrentMouseOnView;

@end
