//
//  ZMAICChatPlainTextItem.h
//  ZoomAIC
//
//  Created by Karen Cao on 9/13/23.
//

#ifndef ZMAICChatPlainTextItem_h
#define ZMAICChatPlainTextItem_h

#import <ZoomAIC/ZMAICChatItem.h>

@protocol ZMAICChatPlainTextItem <ZMAICChatItem>

@property (nonatomic, strong, nullable) NSString* content;
@property (nonatomic, strong, nullable) NSColor* contentColor;
@property (nonatomic, strong, nullable) NSDate* date;

@end

#endif /* ZMAICChatPlainTextItem_h */
