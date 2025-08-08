//
//  ZMPTSinkProtocol.h
//  PTUIBridge
//
//  Created by fistice on 2024/10/25.
//

#ifndef ZMPTSinkProtocol_h
#define ZMPTSinkProtocol_h

#include <zPTApp/SaasBeePTAppInterface.h>

@protocol ZMPTSinkProtocol <NSObject>
@optional
- (void)initModule;
- (void)sinkAppEvent:(PT_EVENT)ptEvent result:(CmmUInt32)result content:(void *)content;
- (void)onZpnsCommonSidepanelReceived;
- (void)onAICompanionSettingUpdate;
- (void)onZpnsCommandClearListReceived:(const std::map<CString, std::vector<CString>>&)clear_list;
@end
#endif /* ZMPTSinkProtocol_h */
