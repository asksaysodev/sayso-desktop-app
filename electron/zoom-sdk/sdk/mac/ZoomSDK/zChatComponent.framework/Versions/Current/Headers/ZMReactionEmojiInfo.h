//
//  ZMReactionEmojiInfo.h
//  ZCommonUI
//
//  Created by groot.ding on 21/07/2020.
//  Copyright © 2020 zoom. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <zChatComponent/ZMEmojiModel.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMReactionEmojiInfo : NSObject

@property (nonatomic,strong,readonly)  NSString *emojiUnicode;
@property (nonatomic,strong,readonly)  ZMCustomEmojiModel *customEmoji;
@property (nonatomic,assign)  NSInteger count;
@property (nonatomic,assign)  NSInteger firstTimeStamp;
@property (nonatomic,assign)  BOOL isContainMe;

- (instancetype)initWithEmojiUnicode:(NSString*)emojiUnicode count:(NSInteger)count isContainMe:(BOOL)isContainMe firstTimestamp:(NSInteger)firstTimestamp;

- (instancetype)initWithCustomEmojiName:(NSString *)emojiName fileID:(NSString *)fileID source:(ZMCustomEmojiSource)source count:(NSInteger)count isContainMe:(BOOL)isContainMe firstTimestamp:(NSInteger)firstTimestamp;

- (BOOL)isCustomEmoji;

- (NSString *)emojiDescription;

@end

NS_ASSUME_NONNULL_END
