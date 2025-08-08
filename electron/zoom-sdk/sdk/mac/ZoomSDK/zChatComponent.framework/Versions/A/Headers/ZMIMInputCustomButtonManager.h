//
//  ZMIMInputCustomButtonManager.h
//  zChatComponent
//
//  Created by Yong Zhou on 10/31/22.
//

#import <Foundation/Foundation.h>
#import <zChatComponent/ZMIMInputCustomButtonItem.h>

@class ZMIMInputCustomSettingWindowCtrl;

NS_ASSUME_NONNULL_BEGIN

typedef NSString* ZMIMComposeBoxButtonInfoName NS_TYPED_ENUM;
extern ZMIMComposeBoxButtonInfoName const ZMIMComposeVideoButtonInfo;
extern ZMIMComposeBoxButtonInfoName const ZMIMComposeAudioButtonInfo;
extern ZMIMComposeBoxButtonInfoName const ZMIMComposeCodeSnippetButtonInfo;
/* Reserved Begin ↓↓ */
extern ZMIMComposeBoxButtonInfoName const ZMIMComposeFormatButtonInfo;
extern ZMIMComposeBoxButtonInfoName const ZMIMComposeEmojiButtonInfo;
extern ZMIMComposeBoxButtonInfoName const ZMIMComposeGiphyButtonInfo;
extern ZMIMComposeBoxButtonInfoName const ZMIMComposeScreenShotButtonInfo;
extern ZMIMComposeBoxButtonInfoName const ZMIMComposeScreenShotOptionButtonInfo;
extern ZMIMComposeBoxButtonInfoName const ZMIMComposeFileButtonInfo;
/* Reserved End  ↑↑ */
extern ZMIMComposeBoxButtonInfoName const ZMIMComposeButton;
extern ZMIMComposeBoxButtonInfoName const ZMIMComposeButtonAttributeLeading;
extern ZMIMComposeBoxButtonInfoName const ZMIMComposeCustomizableButton;

extern NSNotificationName const kNotificationCustomizedComposeMenusUpdated;

extern NSUInteger const kCustomizeFixedButtonStartIndex;

typedef NS_ENUM(NSInteger, ZMIMComposeCustomButtonType) {
    ZMIMComposeCustomButton_Internal = 1,
    ZMIMComposeCustomButton_ChatApp,
};

@interface ZMIMInputCustomButtonManager : NSObject

/// Index from which cell that can be moved
@property (nonatomic, readonly, assign) NSUInteger moveableBeginningIndex;

@property (nonatomic, readonly, assign) NSUInteger publicSystemDescSeparateLineIndex;

@property (nonatomic, readonly, strong) ZMIMInputCustomSettingWindowCtrl *customizeSettingWindow;

@property (nonatomic, readonly, strong) NSMutableArray<ZMIMInputCustomButtonItem*> *internalFixedButtonInfoArr;

@property (nonatomic, readonly, strong) NSMutableArray<ZMIMInputCustomButtonItem*> *customizeFixedButtonInfoArr;

@property (nonatomic, readonly, strong, nullable) NSArray<ZMIMInputCustomButtonItem*> *customizeButtonItemInfoArr;

@property (nonatomic, copy) void (^customizedComposeShortcutsChangedLocally)(void);

+ (instancetype)sharedInstance;

+ (void)releaseInstance;

- (NSMutableArray<ZMIMInputCustomButtonItem*> *)settingItemsForSession:(NSString *)sessionId;

// operation action
- (void)onCustomizedComposeShortcutsReset;
- (void)onUpdateOperationCanceled;
- (void)onCustomizedComposeShortcutsUpdated;

// datasource manage
- (void)setButtonHidden:(BOOL)hidden shortcutID:(nonnull NSString *)shortcutID;

/// Recycle useless  window. Try to close  setting window if not actived for several minutes(for memory issue)
- (void)tryToReleaseSettingWindow;

- (NSArray<ZMIMInputCustomButtonItem*> *)getCustomizeItemsWithSessionId:(NSString *)sessionId;

@end

NS_ASSUME_NONNULL_END

