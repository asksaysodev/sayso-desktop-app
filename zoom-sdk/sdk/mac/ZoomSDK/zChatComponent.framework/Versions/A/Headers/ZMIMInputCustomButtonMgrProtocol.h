//
//  ZMIMInputCustomButtonMgrProtocol.h
//  zChatComponent
//
//  Created by Yong Zhou on 11/2/22.
//

#ifndef ZMIMInputCustomButtonMgrProtocol_h
#define ZMIMInputCustomButtonMgrProtocol_h

@class ZMIMInputCustomButtonItem,ZMChatAppInfo;

@protocol ZMIMInputCustomButtonMgrProtocol <NSObject>

- (nullable NSArray<ZMIMInputCustomButtonItem*> *)getCustomizedComposeShortcutsWithSessionId:(nullable NSString *)sessionId;

- (nullable ZMChatAppInfo*)getChatAppInfoFromAppJID:(nonnull NSString *)appJID sessionId:(nullable NSString *)sessionId;

- (BOOL)updateCustomizedComposeShortcutsConfig:(nullable NSArray<ZMIMInputCustomButtonItem*> *)customizedShortsItems;

- (BOOL)resetCustomizedComposeShortcutsConfig;

@end

#ifndef ZMShareInputCustomButtonMgrProtocol
#define ZMShareInputCustomButtonMgrProtocol (id<ZMIMInputCustomButtonMgrProtocol>)ZMSharedFor(ZMIMInputCustomButtonMgrProtocol)
#endif


#endif /* ZMIMInputCustomButtonMgrProtocol_h */

