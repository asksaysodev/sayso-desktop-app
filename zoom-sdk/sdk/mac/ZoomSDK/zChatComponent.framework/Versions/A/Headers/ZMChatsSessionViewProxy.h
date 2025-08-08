//
//  ZMChatsSessionViewProxy.h
//  zChatComponent
//
//  Created by Cyan.Yang on 2024/10/31.
//

#import <AppKit/AppKit.h>

@protocol ZMChatsSessionViewProxy <NSObject>

- (void)embedContentViewIntoView:(NSView *)view;
- (void)restoreContentViewLayout;

@end
