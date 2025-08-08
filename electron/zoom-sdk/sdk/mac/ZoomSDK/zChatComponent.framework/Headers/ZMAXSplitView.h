//
//  ZMAXSplitView.h
//  zChatComponent
//
//  Created by groot.ding on 4/17/23.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMAXSplitView : NSSplitView

@property(nonatomic,retain) NSColor* dividerDrawColor;

@property (nonatomic,assign) BOOL isMouseDown;

@end

NS_ASSUME_NONNULL_END
