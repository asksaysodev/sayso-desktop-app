//
//  ZMReportMgrProtocol.h
//  zChatComponent
//
//  Created by Kevin Li on 2022/8/4.
//

#ifndef ZMReportMgrProtocol_h
#define ZMReportMgrProtocol_h

@class ZMReportInChannelCommonInfo, ZMReportInChannelRes;

@protocol ZMReportMgrProtocol <NSObject>

- (void)onReportWithReportInfo:(ZMReportInChannelCommonInfo*)reportInfo;
- (void)onNotifyReportInChannel:(ZMReportInChannelRes*)reportInChannelRes;

@end

#ifndef ZMShareReportMgrProtocol
#define ZMShareReportMgrProtocol (id<ZMReportMgrProtocol>)ZMSharedFor(ZMReportMgrProtocol)
#endif

#endif /* ZMReportMgrProtocol_h */
