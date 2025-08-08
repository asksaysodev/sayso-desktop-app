//
//  ZMEmojiCommentInfo.h
//  ZoomMsgUI
//
//  Created by Groot Ding on 2022/4/27.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMEmojiCommentInfo : NSObject

@property (copy)    NSString* operatorJID;

@property (assign)  NSInteger timeStamp;

- (instancetype)initWithOperatorJID:(NSString*)jid timestamp:(NSInteger)inTimestamp;
@end

NS_ASSUME_NONNULL_END
