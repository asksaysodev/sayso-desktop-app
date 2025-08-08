//
//  ZMEmojiModel+Init.h
//  zChatComponent
//
//  Created by Mario Rao on 2024/11/11.
//

#import "ZMEmojiModel.h"
#import "zChatApp/CustomEmoji/ICustomEmojiInterface.h"

NS_ASSUME_NONNULL_BEGIN

@interface ZMEmojiModel (Init)

+ (instancetype)modelWithEmojiInfo:(const ns_zoom_messager::zCustomEmojiInfo &)emojiInfo source:(ZMCustomEmojiSource)source;
+ (NSArray<ZMEmojiModel *> *)modelsWithEmojiList:(const ns_zoom_messager::zCustomEmojiInfoList &)emojiList source:(ZMCustomEmojiSource)source;

@end

NS_ASSUME_NONNULL_END
