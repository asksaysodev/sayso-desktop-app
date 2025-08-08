//
//  ZMIMInputCustomButtonItem.h
//  zChatComponent
//
//  Created by Yong Zhou on 10/31/22.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, ZMChatInputBarButtonDragStyle)
{
    ZMChatInputBarButtonDragDisable = 0,
    ZMChatInputBarButtonDragableShow,
    ZMChatInputBarButtonDragableHidden,
};

typedef NS_ENUM(NSUInteger, ZMChatInputBarSysCellStyle)
{
    ZMChatInputBarSysCell_MiddleLine = 0,
    ZMChatInputBarSysCell_MiddleLineWithText,
    ZMChatInputBarSysCell_Text,
};

typedef NS_ENUM(NSUInteger, ZMIMInputButtonSpecifiedDisplayFlag)
{
    ZMIMInputButtonSpecifiedDisplay_None = 0,
    ZMIMInputButtonSpecifiedDisplay_RichFormat   = 1 << 0,
    ZMIMInputButtonSpecifiedDisplay_Emoji        = 1 << 1,
    ZMIMInputButtonSpecifiedDisplay_Giphy        = 1 << 2,
    ZMIMInputButtonSpecifiedDisplay_File         = 1 << 3,
    ZMIMInputButtonSpecifiedDisplay_ScreenShot   = 1 << 4,
    ZMIMInputButtonSpecifiedDisplay_Audio        = 1 << 5,
    ZMIMInputButtonSpecifiedDisplay_Video        = 1 << 6,
    ZMIMInputButtonSpecifiedDisplay_CodeSnippet  = 1 << 7,
    ZMIMInputButtonSpecifiedDisplay_ScreenShotOption = 1 << 8,
};

@interface ZMIMInputCustomButtonItem : NSObject

@property (nonatomic, assign) ZMIMInputButtonSpecifiedDisplayFlag specifiedFlag;
@property (nonatomic, readonly, assign) ZMChatInputBarButtonDragStyle dragableStyle;
@property (nonatomic, assign) BOOL isSystemItem;
@property (nonatomic, assign) ZMChatInputBarSysCellStyle systemStyle;
/* Zoom predetermined button which can no longer be draged */
@property (nonatomic, assign) BOOL isFixedItem;
/* isInternal is set previous then hiden */
/** If true => internal shortcuts, false => third-party shortcuts */
@property (nonatomic, assign) BOOL isInternal;
@property (nonatomic, assign, getter = isHiden) BOOL hiden;
@property (nonatomic, copy) NSString *titleName;
@property (nonatomic, nullable, retain) NSImage *imageIcon;
/** (internal shortcuts id: name, 3rd-party shortcuts: botjid) ==>key */
@property (nonatomic, copy) NSString *shortcutID;

@property (nonatomic, assign, getter=isDisabled) BOOL disabled;

- (BOOL)enabledChatAppInSession:(NSString *)sessionId;

@end

NS_ASSUME_NONNULL_END
