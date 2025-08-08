//
//  ZMAICConst.h
//  ZoomAIC
//
//  Created by Jun Pang on 9/9/23.
//

#ifndef ZMAICConst_h
#define ZMAICConst_h

#import <Zoomkit/ZoomKit.h>

typedef NS_ENUM(NSUInteger, ZMAICScenario) {
    ZMAICGeneral = 0,
    ZMAICMeeting,
};

#define ZMAICStartView (ZMAICChatUIStart)
#define ZMAICQuestionView (ZMAICChatUIQuesionList|ZMAICChatUIChat|ZMAICChatUIInputView|ZMAICChatUIPrivacy)
#define ZMAICQuestionViewDisabled (ZMAICChatUIQuesionList|ZMAICChatUIChat|ZMAICChatUIDisabledView)
#define ZMAICChat (ZMAICChatUIInputView|ZMAICChatUIPrivacy|ZMAICChatUIChat|ZMAICChatUIFloatingQuestion)
#define ZMAICChatDisabled (ZMAICChatUIChat|ZMAICChatUIDisabledView)

typedef NS_OPTIONS(NSUInteger, ZMAICChatUI){
    ZMAICChatUINone = 0,
    ZMAICChatUIStart = 1 << 0,
    ZMAICChatUIQuesionList = 1 << 1,
    ZMAICChatUIInputView = 1 << 2,
    ZMAICChatUIPrivacy = 1 << 3,
    ZMAICChatUIChat = 1 << 4,
    ZMAICChatUIFloatingQuestion = 1 << 5,
    ZMAICChatUIDisabledView = 1 << 6,
};

typedef NS_ENUM(NSUInteger, ZMAICChatStateEventType) {
    ZMAICChatStateEventOpenStartView,
    ZMAICChatStateEventOpenQuestionListView,
    ZMAICChatStateEventOpenChatView,
    ZMAICChatStateEventSentQuestion,
    ZMAICChatStateEventReceivedAnswer,
    ZMAICChatStateEventChatEnabled,
    ZMAICChatStateEventChatDisabled,
    ZMAICChatStateEventFeatureOff,
    ZMAICChatStateEventSendInfo,
};

typedef NS_ENUM(NSUInteger, ZMAICChatItemType) {
    ZMAICChatItemTypeQuestionSend = 0,
    ZMAICChatItemTypePlainTextResponse = 1,
    ZMAICChatItemTypeSystemPlainText = 2,
    ZMAICChatItemTypeLoading = 3,
};

typedef NS_ENUM(NSUInteger, ZMAICSenderFromType) {
    ZMAICSenderFromTypeUser = 0,
    ZMAICSenderFromTypeAI = 1,
    ZMAICSenderFromTypeSystem = 2,
};

typedef NS_ENUM(NSUInteger, ZMAICAIFeedbackType){
    ZMAICAIFeedbackType_None = 0,
    ZMAICAIFeedbackType_Voting,
    ZMAICAIFeedbackType_Upvote,
    ZMAICAIFeedbackType_Downvote,
};

@protocol IZMAICRouterCenter <ZMRouterCenter>
@end
#define ZMAICRouterCenter (id<IZMAICRouterCenter>)[ZMRouterCenter.shared singletonForProtocol:@protocol(IZMAICRouterCenter)]

#define ZMAICEventPostFor(scenario) ((id<ZMAICChatUIEvent>)[ZMAICRouterCenter posterForProtocol:@protocol(ZMAICChatUIEvent) sender:nil object:nil tag:scenario])
#define ZMAICEventPost ZMAICEventPostFor(self.scenario)
#define ZMAICEventRegister(scenario, target) [ZMAICRouterCenter registerTartget:target forProtocol:@protocol(ZMAICChatUIEvent) object:nil tag:scenario];

#endif /* ZMAICConst_h */
