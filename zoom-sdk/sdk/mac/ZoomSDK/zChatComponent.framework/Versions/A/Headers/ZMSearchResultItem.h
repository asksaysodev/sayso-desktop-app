
//
//  ZMSearchResultItem.h
//  zChatUI
//
//  Created by groot.ding on 2018/7/9.
//  Copyright © 2018 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef enum : NSUInteger {
    ZMSearchResultNone,
    ZMSearchResultCustomSession,
    ZMSearchResultContacts,
    ZMSearchResultChannel,
    ZMSearchResultMessage,
    ZMSearchResultFile,
    ZMSearchResultImageFile,
    ZMSearchResultCloudContact,
	ZMSearchResultMUC,
	ZMSearchResultBot,
    ZMSearchResultPublicChannel,
    ZMSearchResultSMSContact,
    ZMSearchResultSMSSession,
    ZMSearchResultSMSMessage,
    ZMSearchResultMeeting,
    ZMSearchResultSharedSpace,
    ZMSearchResultMessageID
} ZMSearchResultType;

typedef enum : NSUInteger {
    ZMSearchResultMatchTypNone,
    ZMSearchResultMatchTypExact,
    ZMSearchResultMatchTypSubString,
    ZMSearchResultMatchTypDefault,
} ZMSearchResultMatchType;


@interface ZMSearchResultItem : NSObject

@property (nonatomic,assign) ZMSearchResultType resultType;

@property (nonatomic,assign) ZMSearchResultMatchType matchType;

@property (nonatomic,retain) id resultData;

@property (nonatomic,copy) NSString *searchKey;

@end
