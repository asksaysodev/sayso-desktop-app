//
//  ZMChatToolTipWindowCtrl.h
//  SaasBeePTUIModule
//
//  Created by ryan on 8/5/16.
//  Copyright © 2016 Zipow. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface ZMChatToolTipView : NSView
{
    float           _angleLoaction;
    NSString*       _string;
}

@property (assign) float angleLocation;
@property (copy) NSString* string;

@end

@interface ZMChatToolTipWindow : NSWindow

- (void)invisibleInSharing;
- (id)initWithContentRect:(NSRect)contentRect styleMask:(NSUInteger)aStyle backing:(NSBackingStoreType)bufferingType defer:(BOOL)flag;

@end

@interface ZMChatToolTipWindowCtrl : NSWindowController

+ (ZMChatToolTipWindowCtrl*)sharedController;
- (void)showWindowAtPoint:(NSPoint)point withString:(NSString*)string;
- (void)showWindowAtPoint:(NSPoint)point withString:(NSString*)string screen:(NSScreen*)inScreen;
- (void)showWindowAtPoint:(NSPoint)point withString:(NSString*)string screen:(NSScreen*)inScreen alreadyAddedToParentWindow:(BOOL)flag;

@end

typedef enum Type_Tooltip_s{
    Type_Tooltip_Normal = 0,
    Type_Tooltip_TimedChatRemoved=1,
    
}Type_Tooltip;

@interface ZMToolTipView : NSView{
    NSTextStorage*              _textStorage;
    NSColor*                    _bgColor;
    NSInteger                   _textHOffset;
    NSInteger                   _textVOffset;
    NSRange                     _actionRange;
    NSUInteger                  _curType;
}
- (void)cleanUp;
@end

@interface ZMChatToolTipMgr : NSObject{
    ZMChatToolTipWindow*        _tooltipWindow;
    ZMToolTipView*              _tooltipView;
    NSEvent*                    _mouseMoveEventMonitor;
}
+ (ZMChatToolTipMgr*)sharedInstance;
+ (void)releaseSharedInstance;

- (ZMToolTipView*)tooltipView;
- (ZMChatToolTipWindow*)tooltipWindow;
- (void)showToolTipAtPoint:(NSPoint)atPt withParentWindow:(NSWindow*)parentWindow andTooltipType:(NSUInteger)type;
- (void)tryToCloseTipWindow:(NSWindow*)parentWindow;
@end
