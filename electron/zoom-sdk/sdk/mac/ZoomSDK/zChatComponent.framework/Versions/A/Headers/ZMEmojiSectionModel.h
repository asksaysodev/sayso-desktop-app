//
//  ZMEmojiSectionModel.h
//  ZoomUnit
//
//  Created by Mario Rao on 2023/7/18.
//

#import <Foundation/Foundation.h>
#import <zChatComponent/ZMEmojiModel.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMEmojiSectionModel : NSObject

@property (nonatomic, strong) NSString *sectionName;
@property (nonatomic, strong) NSArray<ZMEmojiModel *> *emojis;
@property (nonatomic, assign) BOOL isCustomEmoji;

+ (instancetype)modelWithName:(NSString *)name emojis:(NSArray<ZMEmojiModel *> *)emojis isCustomEmoji:(BOOL)isCustomEmoji;

- (BOOL)isFrequentlyUse;

- (BOOL)isSearchResult;

@end

NS_ASSUME_NONNULL_END
