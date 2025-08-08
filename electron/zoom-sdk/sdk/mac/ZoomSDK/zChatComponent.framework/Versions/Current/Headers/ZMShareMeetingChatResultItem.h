//
//  ZMShareMeetingChatResultItem.h
//  ChatUI
//
//  Created by Yong Zhou on 6/24/22.
//  Copyright © 2022 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSUInteger, ZMMeetingChatActionType)
{
    ZMMeetingChatActionType_Share = 0,
    ZMMeetingChatActionType_Edit,
    ZMMeetingChatActionType_Remove,
};

typedef NS_ENUM(NSUInteger, ZMMCCSessionType)
{
    ZMMCCSessionType_Group = 0,
    ZMMCCSessionType_Individual,
};

NS_ASSUME_NONNULL_BEGIN

@interface ZMShareMeetingChatResultItem : NSObject

@property (nonatomic, assign) BOOL hasAccessToPreviousSharedSession;
@property (nonatomic, assign) ZMMeetingChatActionType action;
@property (nonatomic, assign) BOOL isNewSession;
@property (nonatomic, assign) BOOL isAddExternal;
@property (nonatomic, assign) ZMMCCSessionType sessionType;
@property (nonatomic, copy) NSString *operatorUserID;
@property (nonatomic, copy) NSString *operatorJID;
@property (nonatomic, copy) NSString *sessionID;
@property (nonatomic, copy) NSString *sessionName;
@property (nonatomic, copy, nullable) NSString *meetingTopic;
@property (nonatomic, copy) NSString *meetingID;

@end

NS_ASSUME_NONNULL_END
