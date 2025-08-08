//
//  ZMChanelRemindedUpcomingItem.h
//  zChatComponent
//
//  Created by Simon Shang on 2024/5/8.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMChanelRemindedUpcomingItem : NSObject

@property (nonatomic, assign) long meetingNumber;
@property (nonatomic, copy) NSString *channelId;
@property (nonatomic, copy) NSString *meetingTopic;


@end

NS_ASSUME_NONNULL_END
