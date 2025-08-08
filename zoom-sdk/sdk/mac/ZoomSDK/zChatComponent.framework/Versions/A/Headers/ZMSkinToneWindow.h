//
//  ZMSkinToneWindow.h
//  ZoomUnit
//
//  Created by Mario Rao on 2023/7/20.
//

#import <Cocoa/Cocoa.h>
#import <zChatComponent/ZMSkinToneButton.h>

#ifndef SKIN_TONE_CHANGED
    #define SKIN_TONE_CHANGED (@"SKIN_TONE_CHANGED")
#endif

NS_ASSUME_NONNULL_BEGIN

@interface ZMSkinToneWindow : NSWindow{
    id              _localMouseEventMonitor;
    id              _globalEventMonitor;
}

@property (nonatomic, weak) id<ZMSkinToneButtonDelegate> actionDelegate;
@property (nonatomic, assign) BOOL forceDarkMode;

- (void)cleanUp;
- (void)makeFirstResponderOnButton;
- (void)resetBtnHoverStatus;

@end

NS_ASSUME_NONNULL_END
