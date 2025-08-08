//
//  ZMAICResponseItemAI.h
//  ZoomAIC
//
//  Created by Karen Cao on 9/25/23.
//

#import <ZoomAIC/ZMAICChatPlainTextItem.h>
#import <ZoomAIC/ZMAICChatItemAvatarized.h>
#import <ZoomAIC/ZMAICChatItemBubbled.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMAICResponseItemAI : NSObject<ZMAICChatPlainTextItem,ZMAICChatItemAvatarized,ZMAICChatItemBubbled>
@property(nonatomic, strong) NSString* itemId;
@property(nonatomic) ZMAICAIFeedbackType voteType;
@property(nonatomic)CGFloat textWidth;
@end

NS_ASSUME_NONNULL_END
