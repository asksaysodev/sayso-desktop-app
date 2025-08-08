//
//  ZMEmojiViewItem.h
//  ZoomUnit
//
//  Created by Mario Rao on 2023/7/19.
//

#import <Cocoa/Cocoa.h>
#import <zChatComponent/ZMSysEmojiItemView.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMEmojiViewItem : NSCollectionViewItem

@property (nonatomic, readonly) __kindof ZMSysEmojiItemView *contentView;
@property (nonatomic) BOOL isHovered;

@property (nonatomic, copy) void (^itemDidClick)(ZMEmojiModel *model);
@property (nonatomic, copy) NSString *(^accessibilityLabelString)(void);

@end

@interface ZMMTEmojiViewItem: ZMEmojiViewItem

@end

NS_ASSUME_NONNULL_END
