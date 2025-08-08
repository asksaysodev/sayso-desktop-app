//
//  ZMAICChatItem.h
//  ZoomAIC
//
//  Created by Karen Cao on 9/13/23.
//

#ifndef ZMAICChatItem_h
#define ZMAICChatItem_h

@protocol ZMAICChatItem <NSObject>

@property(nonatomic) ZMAICChatItemType type;
@property(nonatomic) ZMAICSenderFromType from;

@end

#endif /* ZMAICChatItem_h */
