//
//  ZMDLPPolicy.h
//  zChatUI
//
//  Created by Ryan Shen on 2/12/20.
//  Copyright © 2020 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, ZMDLPRuleType)
{
    ZMDLPRuleType_NotSet = 0,
    ZMDLPRuleType_Keyword,
    ZMDLPRuleType_Regular,
};

typedef NS_ENUM(NSUInteger, ZMDLPActionType)
{
    ZMDLPActionType_NotSet = 0,
    ZMDLPActionType_NotPrompt,
    ZMDLPActionType_MatchPrompt,
    ZMDLPActionType_Block,
};

typedef NS_ENUM(NSUInteger, ZMDLPUserActionType)
{
    ZMDLPUserActionType_NotSet = 0,
    ZMDLPUserActionType_Send,
    ZMDLPUserActionType_SendOnConfirm,
    ZMDLPUserActionType_SendOnConfirm_Cancel,
    ZMDLPUserActionType_SendOnConfirm_Block,
};

@interface ZMDLPPolicy : NSObject

@property (retain) NSString* policyID;
@property (retain) NSString* policyName;
@property (retain) NSString* policyDes;
@property (retain) NSString* ruleValue;
@property (assign) ZMDLPRuleType ruleType;
@property (assign) ZMDLPActionType actionType;
@property (assign) BOOL isMatchContentAnonymous;

@end

@interface ZMDLPPolicyResult : NSObject

@property (retain) ZMDLPPolicy* policy;
@property (retain) NSString* content;
@property (retain) NSString* keyword;

@end

@interface ZMDLPPolicyEvent : NSObject

@property (retain) NSString* eventID;
@property (retain) NSString* policyID;
@property (retain) NSString* messageID;
@property (retain) NSString* recipient;
@property (retain) NSString* content;
@property (retain) NSString* keyword;
@property (retain) NSString* sessionID;
@property (retain) NSString* groupType;
@property (assign) ZMDLPUserActionType userActionType;
@property (assign) unsigned long long violateTime;
@property (assign) unsigned long long serverTime;

@end

NS_ASSUME_NONNULL_END
