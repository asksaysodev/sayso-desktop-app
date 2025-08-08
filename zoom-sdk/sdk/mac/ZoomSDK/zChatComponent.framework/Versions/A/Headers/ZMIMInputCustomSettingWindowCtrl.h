//
//  ZMIMInputCustomSettingWindowCtrl.h
//  zChatComponent
//
//  Created by Yong Zhou on 10/24/22.
//

#import <Cocoa/Cocoa.h>

@class ZMMeetingChatCardEntity;

NS_ASSUME_NONNULL_BEGIN

@interface ZMIMInputCustomSettingWindowCtrl : NSWindowController


/* The designated initializer.
 */
//- (instancetype)init NS_DESIGNATED_INITIALIZER;
//
- (instancetype)initWithWindow:(nullable NSWindow *)window  NS_UNAVAILABLE; // Use the designated initializer init:

- (void)setUpConfigWithSessionId:(NSString *)sessionId;

@end

NS_ASSUME_NONNULL_END
