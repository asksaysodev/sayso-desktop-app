//
//  ZMChatAIModule.h
//  zChatComponent
//
//  Created by Cyan.Yang on 2024/11/19.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMChatAIMenuItem;

typedef NS_ENUM(NSUInteger, ZMChatAIEventType) {
    ZMChatAIEventTypeChannelUnreadSummary,
    ZMChatAIEventTypePreMeetingCard,
    ZMChatAIEventTypePostMeetingCard,
} NS_SWIFT_NAME(ChatAIEventType);

NS_SWIFT_NAME(ChatAIModule)
@interface ZMChatAIModule : NSObject

+ (BOOL)isAICompanionPlusEnabled;

+ (BOOL)isAIMenuEnabledForSessionWithID:(NSString *)sessionID;

+ (NSArray<ZMChatAIMenuItem *> * _Nullable)AIMenuItemsOfEvent:(ZMChatAIEventType)event
                                             forSessionWithID:(NSString *)sessionID;

+ (void)openAICPanelFromAIMenuItem:(ZMChatAIMenuItem *)menuItem
                  forMessageWithID:(NSString *)messageID
                         sessionID:(NSString *)sessionID;

@end

NS_ASSUME_NONNULL_END
