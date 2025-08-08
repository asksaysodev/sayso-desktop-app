//
//  ZMAICChatItemBubbled.h
//  ZoomAIC
//
//  Created by Karen Cao on 9/13/23.
//

#ifndef ZMAICChatItemBubbled_h
#define ZMAICChatItemBubbled_h

@protocol ZMAICChatItemBubbled <NSObject>

@property (nonatomic, strong) NSColor* bubbleColor;
@property (nonatomic) CGFloat borderWidth;
@property (nonatomic, strong) NSColor* borderColor;
@property (nonatomic) CGFloat cornerRadius;

@end

#endif /* ZMAICChatItemBubbled_h */
