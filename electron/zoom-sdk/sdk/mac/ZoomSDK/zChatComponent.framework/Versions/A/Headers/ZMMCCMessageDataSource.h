//
//  ZMMCCMessageDataSource.h
//  zChatComponent
//  
//  Created by Yong Zhou on 7/12/22.
//  Copyright © 2022 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>
@class ZMChatMsgItemInfo;
@class ZMMeetingChatCardEntity;

@protocol ZMMCCMessageDataSourceDelegate <NSObject>

- (void)didMessageSyncReady;

- (void)didAllMessageSyncDone;

- (void)didSetupDataSource;

- (void)didMessageSyncError;

@end

NS_ASSUME_NONNULL_BEGIN

@interface ZMMCCMessageDataSourceImpl : NSObject

@property (nonatomic, weak) id<ZMMCCMessageDataSourceDelegate> delegate;

- (NSArray<ZMChatMsgItemInfo*>*)getMessagesWithMeetingInfo:(nonnull ZMMeetingChatCardEntity*)meetingEntity;

- (void)loadMoreMessageWtihMeetingInfo:(nonnull ZMMeetingChatCardEntity*)meetingEntity;

@end

NS_ASSUME_NONNULL_END
