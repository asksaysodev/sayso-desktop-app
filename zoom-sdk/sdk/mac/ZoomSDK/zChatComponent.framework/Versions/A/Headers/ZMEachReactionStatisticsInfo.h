//
//  ZMEachReactionStatisticsInfo.h
//  ZCommonUI
//
//  Created by Frank on 2020/12/23.
//  Copyright © 2020 zoom. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <zChatComponent/ZMReactionInfo.h>

@interface ZMEachReactionStatisticsInfo : NSObject

@property (nonatomic, retain) ZMReactionInfo* reactionInfo;
@property (nonatomic, assign) int             count;

- (instancetype)initWithFeedbackType:(int)feedbackType withCount:(int)count;
- (instancetype)initWithEmoji:(NSString*)emojiUTF8 withCount:(int)count;
@end

