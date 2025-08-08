//
//  NSAttributedString+ZoomMsgUI-Swift.h
//  zChatComponent
//
//  Created by Cyan.Yang on 2024/6/28.
//

#import <AppKit/AppKit.h>

@class ZMViewAttachmentCell;
@interface NSAttributedString (ZoomMsgUI_Swift)

- (NSArray<ZMViewAttachmentCell *> *)getViewAttachmentCells;

- (NSArray<NSTextAttachmentCell *> *)getViewAttachmentCellProtocolCells;

- (void)cleanUpViewAttachmentCellProtocolCells;

- (NSDictionary *)customEmojiAttributesAtIndex:(NSInteger)index;
- (NSMutableAttributedString *_Nonnull)attributedStringWithPlainCustomEmoji;
- (NSMutableAttributedString *_Nonnull)plainStringWithCustomEmoji;
- (NSMutableAttributedString *_Nonnull)attributedStringWithFixedCustomEmojiSize;
- (NSString *_Nonnull)stringWithPlainCustomEmoji;
- (NSInteger)customEmojiCount;

@end
