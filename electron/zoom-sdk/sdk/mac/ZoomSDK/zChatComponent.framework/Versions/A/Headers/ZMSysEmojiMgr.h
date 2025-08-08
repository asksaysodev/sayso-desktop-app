//
//  ZMSysEmojiMgr.h
//  SaasBeePTUIModule
//
//  Created by likevin on 7/3/17.
//  Copyright © 2017 Zipow. All rights reserved.
//

#import <zChatComponent/ZMEmojiUnicodeData.h>
#import <zChatComponent/ZMEmojiModel.h>
#import <zChatComponent/ZMSysEmojiView.h>
#import <zChatComponent/ZMSkinToneButton.h>

@class ZMBaseView;
@class ZMCustomEmojiView;
@class ZMSysEmojiToolbarItemModel;
@class ZMSysEmojiMgr;

@protocol ZMSysEmojiMgrDelegate <ZMEmojiUnicodeDataDelegate>
- (void)sysEmojiMgrDidSelectedEmoji:(ZMEmojiModel *)emojiModel atIndex:(NSInteger)index;
- (void)sysEmojiMgrDidAddFavorite:(ZMEmojiModel *)emojiModel;
- (int)sysEmojiMgrDidGetSkinTone;
@optional
- (void)sysEmojiMgrDidEnterEditCustomEmoji;
- (NSString *)currentSelectedEmojiForSysEmojiMgr:(ZMSysEmojiMgr *)sysEmojiMgr;
@end

@interface ZMSysEmojiMgr : NSObject 
@property (nonatomic, weak) id<ZMSysEmojiMgrDelegate> delegate;
@property (nonatomic, assign) BOOL forceDarkMode;
@property (nonatomic, copy) NSString *currentSelectedEmoji;

//* Custom Emoji Related *//
@property (nonatomic, strong, readonly) NSString *currentSessionID;
@property (nonatomic, assign, readonly) ZMCustomEmojiSource customEmojiSource;
@property (nonatomic, assign) BOOL disableCustomEmoji;
//* Custom Emoji Related *//

@property (nonatomic, strong, readonly) ZMEmojiUnicodeData *emojiUnicodeData;

- (instancetype)initWithDelegate:(id<ZMSysEmojiMgrDelegate>)delegate;
- (void)onSearch:(NSString*)searchString;
- (void)onTagSelected:(NSUInteger)tag;
- (void)addFavorite:(ZMEmojiModel *)emojiModel;
- (void)clickedItem:(ZMEmojiModel *)emojiModel;
- (void)resetEmojiView;
- (void)makeSearchFiledFirstResponder;
- (NSView*)getUnicodeEmojiView:(NSRect)inRect;
- (void)releaseEmojiView;
- (void)reloadSysEmojiData;
- (Class)emojiItemViewClass;
- (NSArray<ZMSysEmojiToolbarItemModel *> *)tableBarDataSource;
- (void)setSessionID:(NSString *)sessionID orEmojiSource:(ZMCustomEmojiSource)emojiSource;
- (void)createEmojiDataWhenLogin;
@end
