//
//  ZMEmojiUnicodeData.h
//  SaasBeePTUIModule
//
//  Created by likevin on 7/11/17.
//  Copyright © 2017 Zipow. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <zChatComponent/ZMEmojiModel.h>

#define FAVORITE_EMOJI_MAX_COUNT 16

extern NSString *FavoriteEmojiKey(ZMCustomEmojiSource source);
@class ZMEmojiUnicodeData;

@protocol ZMEmojiUnicodeDataDelegate <NSObject>
@optional
- (BOOL)canShowTWFlagEmoji;
- (ZMCustomEmojiSource)emojiSourceForEmojiUnicodeData:(ZMEmojiUnicodeData *)unicodeData;
- (BOOL)isCustomEmojiDisabledForEmojiUnicodeData:(ZMEmojiUnicodeData *)unicodeData;
@end

@interface ZMEmojiUnicodeData : NSObject

@property (nonatomic, retain, readonly) NSMutableArray<ZMEmojiModel *> *frequentArray;
@property (nonatomic, retain, readonly) NSMutableArray<ZMEmojiModel *> *frequentWithoutCustomEmojiArray;

//unicode emoji
@property(nonatomic, retain, readonly) NSDictionary<NSString *, NSString *> *unicode2EmojiNameMap;
@property(nonatomic, retain) NSMutableDictionary<NSString *, NSString *> *emojiName2UnicodeMap;
@property (nonatomic, strong) NSMutableDictionary<NSString *, ZMEmojiModel *> *unicodeEmojiName2ModelMap;

@property (nonatomic, weak) id<ZMEmojiUnicodeDataDelegate> delegate;

- (void)initData;
+ (NSArray *)skinToneEmojiArray;
- (ZMCustomEmojiSource)currentCustomEmojiSource;
- (NSArray *)categoryArrayWithCustomEmoji:(BOOL)withCustomEmoji;
- (void)scheduleSaveFavoriteEmojiList;
/// Get the selected emojis setting by admin
- (NSArray<ZMEmojiModel *> *)selectedEmojiStringArray;
- (BOOL)isEmojiAllowedInSelectedEmoji:(NSString *)emoji;
- (BOOL)isEmojiNameAllowedInSelectedEmoji:(NSString *)emoji;
- (NSMutableArray<ZMEmojiModel *> *)getFrequentMutableArray;
- (NSArray<ZMEmojiModel *> *)getLottieArray;
- (NSArray<ZMEmojiModel *> *)getEmojiArrayByCategory:(NSString*)category;
- (void)addFavorite:(ZMEmojiModel *)model;
- (void)removeFavoriteCustomEmojiWithFileID:(NSString *)fileID;
- (BOOL)contains1CharacterEmojiStr:(NSString *)emojiStr;
- (NSString *)tryReplaceCusEmojiWithSystemEmoji:(NSString *)emojiStr;
- (NSArray<ZMEmojiModel *> *)availableFrequentlyUsedEmojiWithCustomEmoji:(BOOL)withCustomEmoji;
- (void)cleanUp;

- (NSMutableArray<NSString *> *)convertToEmojiStringWithModelArray:(NSArray<ZMEmojiModel *> *)emojiModelArr;
- (NSMutableArray<ZMEmojiModel *> *)convertToModelWithEmojiStringArray:(NSArray<NSString *> *)emojiStrArr;

@end
