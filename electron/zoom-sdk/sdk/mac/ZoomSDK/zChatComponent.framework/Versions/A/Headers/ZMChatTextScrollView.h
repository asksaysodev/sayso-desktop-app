//
//  ZMChatTextScrollView.h
//  SaasBeePTUIModule
//
//  Created by John on 6/3/14.
//  Copyright (c) 2014 Zipow. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface ZMChatAlwaysOverlayScrollView : ZMIMTrackingScrollView
@property (nonatomic, assign) IBInspectable BOOL bAlwaysOverlay;
@end

@interface ZMChatCheckDownAndUpScrollView : NSScrollView

@property(nonatomic, weak) id myController;

@end
