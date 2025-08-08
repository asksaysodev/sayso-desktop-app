//
//  ZMEmojiTooltipProtocol.h
//  zChatComponent
//
//  Created by Kevin Li on 2022/7/27.
//

#ifndef ZMEmojiTooltipProtocol_h
#define ZMEmojiTooltipProtocol_h

@protocol ZMEmojiTooltipProtocol <NSObject>

- (void)showAddEmojiTip:(NSString *)sessionId;
- (void)showAlreadyExistedEmojiTip:(NSString *)sessionId;

@end

#endif /* ZMEmojiTooltipProtocol_h */
