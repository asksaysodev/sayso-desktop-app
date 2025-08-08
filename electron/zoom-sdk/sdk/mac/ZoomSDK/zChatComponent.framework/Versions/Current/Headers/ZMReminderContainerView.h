//
//  ZMReminderContainerView.h
//  zChatComponent
//
//  Created by groot.ding on 9/28/23.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMChatMsgItemInfo;

@interface ZMReminderContainerView : NSView
- (NSSize)calculateViewSize;
/// Updates the stored message info. The message info is used to determine what data this view displays.
- (void)updateMessageInfo:(ZMChatMsgItemInfo * _Nullable)info;
- (void)updateUI;
@end

NS_ASSUME_NONNULL_END
