//
//  ZMAICChatStateEvent.h
//  ZoomAIC
//
//  Created by Karen Cao on 9/16/23.
//

NS_ASSUME_NONNULL_BEGIN

#define GET_MAKE_EVENT_MACRO(_1, _2, MACRO_NAME, ...) MACRO_NAME

#define ZMAICMakeEvent1(event) [ZMAICChatStateEvent build:event]
#define ZMAICMakeEvent2(event, data) [ZMAICChatStateEvent build:event withData:data]

#define ZMAICMakeEvent(...) GET_MAKE_EVENT_MACRO(__VA_ARGS__, ZMAICMakeEvent2, ZMAICMakeEvent1)(__VA_ARGS__)


@interface ZMAICChatStateEvent : NSObject
@property (nonatomic) ZMAICChatStateEventType eventType;
@property id data;

+(instancetype)build:(ZMAICChatStateEventType)event;
+(instancetype)build:(ZMAICChatStateEventType)event withData:(id)data;
@end

NS_ASSUME_NONNULL_END
