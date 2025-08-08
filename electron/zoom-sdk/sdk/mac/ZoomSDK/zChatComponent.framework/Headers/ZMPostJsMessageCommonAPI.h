//
//  ZMPostJsMessageCommonAPI.h
//  ChatUIEx
//
//  Created by Jed Zheng on 20/7/2023.
//

#import <Foundation/Foundation.h>
#include <zPTApp/SaasBeePTAppInterface.h>

#ifndef ZMPostJsMessageCommonAPI_h
#define ZMPostJsMessageCommonAPI_h

#ifdef ENABLE_ZOOM_CHAT_JS
class ZMPostJsMessageCommonAPI: public NS_ZOOM_MESSAGER::zChatSDKJsCallBack
#else
class ZMPostJsMessageCommonAPI
#endif
{
public:
    ZMPostJsMessageCommonAPI(){}
    virtual void PostJsonMsgToWebView(void* msgRoot);
    virtual void PostJsonMsgToWebViewWithCallId(const Cmm::CString& jsCallId, void* msgRoot);
};
#endif /* ZMPostJSMessageCommonAPI_h */


@interface ZMChatSdkJsCallMgr : NSObject
+ (instancetype)sharedInstance;
- (void)releaseInstance;
- (void)addJsCallSink;
- (void)removeJsCallSink;
- (NSArray *)sdkRegisteredFunctionNames;

- (void)handleJsMessageToSdk:(NSDictionary *)param sourceType:(NSInteger)type;
@end
