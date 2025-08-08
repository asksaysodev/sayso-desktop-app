//
//  ZMSysEmojiCollectionViewItem.h
//  ZoomUnit
//
//  Created by Mario Rao on 2023/7/16.
//

#import <zChatComponent/ZMSysEmojiItemView.h>

NS_ASSUME_NONNULL_BEGIN

#ifndef ZM_EMOJI_WIDTH
    #define ZM_EMOJI_WIDTH 36
#endif

#ifndef ZM_EMOJI_HEIGHT
    #define ZM_EMOJI_HEIGHT 34
#endif

#ifndef ZM_EMOJI_HEADER_WIDTH
    #define ZM_EMOJI_HEADER_WIDTH 292
#endif

#ifndef ZM_EMOJI_HEADER_HEIGHT
    #define ZM_EMOJI_HEADER_HEIGHT 24
#endif

#ifndef ZM_EMOJI_HEADER_CUSTOM_EMOJI_HEIGHT
    #define ZM_EMOJI_HEADER_CUSTOM_EMOJI_HEIGHT 32
#endif

#ifndef EMOJI_SKIN_TONE_NUM
    #define EMOJI_SKIN_TONE_NUM 6
#endif

#ifndef EMOJI_PAGE_HEIGHT
    #define EMOJI_PAGE_HEIGHT 178
#endif

extern NSUInteger s_skinToneNum;

///This is NOT a new class, just move it form ZMSysEmojiMgr.

@interface ZMSysEmojiCollectionViewItem : NSCollectionViewItem

@property(nonatomic, strong) ZMSysEmojiItemView *itemView;

- (instancetype)initWithItemViewClass:(Class)itemClass;

@end

NS_ASSUME_NONNULL_END
