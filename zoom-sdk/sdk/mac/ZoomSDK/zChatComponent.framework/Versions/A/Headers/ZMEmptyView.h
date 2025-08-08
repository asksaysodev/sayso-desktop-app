//
//  ZMEmptyView.h
//  zChatComponent
//
//  Created by groot.ding on 6/27/23.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMEmptyView: NSView

@property (nonatomic, strong) ZMMultiLineTextField *emptyTitleField;
@property (nonatomic, strong) ZMMultiLineTextField *emptySubtitleField;
- (instancetype)initWithFrame:(NSRect)frameRect title:(NSString *)title subtitle:(NSString *)subtitle image:(NSImage *)image;
@end
NS_ASSUME_NONNULL_END
