//
//  ZMChatEmojiToolbarView.h
//  SaasBeePTUIModule
//
//  Created by likevin on 10/9/16.
//  Copyright © 2016 Zipow. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class ZMSkinToneButton, ZMButton;

enum
{
    TAG_BUTTON_ZOOM_EMOJI = 100,
    TAG_BUTTON_STICKER,
    TAG_BUTTON_GIF_EMOJI,
    TAG_BUTTON_SETTING,
    TAG_BUTTON_CANCEL,
    TAG_BUTTON_DELETE,
    TAG_BUTTON_SKINTONE,
    TAG_BUTTON_COUNT,
    
    TAG_IMAGE_GIF_LOGO = 200,
};
enum
{
    TAG_MenuItem_Add = 200,
    TAG_MenuItem_Delete,
};

typedef NS_ENUM(NSUInteger, ZMChatEmojiToolbarStyle) {
    ZMChatEmojiToolbarStyleNone = 0,
    
    //atom
    ZMChatEmojiToolbarStyleEmoji = 1,
    ZMChatEmojiToolbarStyleGif = 1 << 1,
    ZMChatEmojiToolbarStyleSaved = 1 << 2,
    
    //combine
    ZMChatEmojiToolbarStyleEmojiAndGif = 0x3,
    ZMChatEmojiToolbarStyleEmojiAndSaved = 0x5,
    ZMChatEmojiToolbarStyleGifAndSave = 0x6, //not exist
    ZMChatEmojiToolbarStyleFull = 0x7,
};

@interface ZMChatEmojiToolbarView : NSView<NSMenuDelegate>
{
    BOOL       _isCanEditable;
    NSMenu*    _settingMenu;
    BOOL       _isMenuVisible;
    int        _currentBtnTag;
    ZMSkinToneButton * _skinToneBtn;
}

@property (nonatomic, assign) ZMChatEmojiToolbarStyle style;

- (void)cleanUp;
- (BOOL)isMenuVisible;
- (BOOL)isEditMode;
- (void)updateEditMode:(BOOL)isEditable;
- (void)updateDeleteBtnText:(NSUInteger)nNumber;
- (void)setBtnSelectedWithTag:(NSUInteger)iTagIsSelected;
@end

enum
{
    TAG_BTN_FAVORITE=300,
    TAG_BTN_PEOPLE,
    TAG_BTN_NATURE,
    TAG_BTN_FOOD,
    TAG_BTN_ACTIVITY,
    TAG_BTN_PLACES,
    TAG_BTN_OBJECTS,
    TAG_BTN_SYMBOLS,
    TAG_BTN_FLAGS,
    TAG_BTN_ZOOM,
};
@interface ZMEmojiOneToolbarView : NSView<NSAccessibilitySwitch>{
    NSInteger _selectedTag;
}
@property(nonatomic, readwrite, weak)id actionDelegate;
- (void)cleanUp;
- (void)updateSelectedTagWhenScroll:(NSInteger)btnIndex;
@end

@interface ZMEmojiReactionToolbarView : NSView{
    ZMSkinToneButton * _skinToneBtn;
    ZMButton* _recoverBtn;
}
@end
