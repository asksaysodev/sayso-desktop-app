//
//  ZMPTTimedChatSperatorCell.h
//  zChatComponent
//
//  Created by groot.ding on 2/28/24.
//

#import <ZoomKit/ZoomKit.h>
#import <zChatComponent/ZMViewAttachmentCellProtocol.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMPTTimedChatSperatorCell : ZMPTBaseTextAttachmentCell <ZMViewAttachmentCellProtocol> {
    NSRect                      _iconRect;
    NSTextStorage*              _textStorage;
    NSRange                     _textRange;
    NSInteger                   _textHOffset;
    NSInteger                   _textVOffset;
}
@property (nullable, nonatomic, strong) NSColor* backgroundColor;
@property (nonatomic, assign) BOOL isHiddenHelpButton;
- (void)updateIconTitle:(NSString*)msg;
- (void)setCustomAttributedString:(NSAttributedString*)attributedString;
+ (nullable NSAttributedString*)attrStringWithString:(NSString*)string;
+ (nullable NSAttributedString*)attrStringWithString:(NSString*)string backgroundColor:(NSColor* _Nullable)backgroundColor;
@end

NS_ASSUME_NONNULL_END
