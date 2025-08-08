//
//  ZMDisabledChatView.h
//  zChatComponent
//
//  Created by groot.ding on 6/27/23.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMDisabledChatView : NSView
@property (copy) NSString* sessionID;
- (instancetype)initWithFrame:(NSRect)frameRect withSessionID:(NSString*)sessionID;
- (void)cleanUp;
@end

NS_ASSUME_NONNULL_END
