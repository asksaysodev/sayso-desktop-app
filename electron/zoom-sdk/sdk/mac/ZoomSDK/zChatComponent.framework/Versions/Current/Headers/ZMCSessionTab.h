//
//  ZMCSessionTab.h
//  zChatComponent
//
//  Created by Huxley Yang on 12/25/24.
//

#ifndef ZMCSessionTab_h
#define ZMCSessionTab_h

#include <zPTApp/SaasBeePTAppInterface.h>
#import <zChatApp/SessionTabs/IZoomSessionTabs.h>

@class ZMChatSessionGroupTabInfo;

@protocol ZMCSessionTabAPI <NSObject> //!IZoomSessionTabs
@optional

@property (nonatomic, readonly, getter=isChannelTabsEnabled) BOOL channelTabsEnabled;
@property (nonatomic, readonly, getter=isAddNotesToTabsEnabled) BOOL addNotesToTabsEnabled;
@property (nonatomic, readonly, getter=isAddDocsToTabsEnabled) BOOL addDocsToTabsEnabled;
@property (nonatomic, readonly, getter=isEnableResourceTab) BOOL enableResourceTab;
@property (nonatomic, readonly, getter=isEnhancedPinEnabled) BOOL enhancedPinEnabled;

- (BOOL)addGroupTabWithSessionId:(NSString *)sessionId info:(const NS_ZOOM_MESSAGER::GroupTabInfo)info requestID:(NSString **)pReqID;
- (BOOL)deleteGroupTabWithSessionId:(NSString *)sessionId tabIndex:(uint32_t)tabIndex requestID:(NSString **)pReqID;
- (BOOL)modifyGroupTabNameWithSessionId:(NSString *)sessionId tabIndex:(uint32_t)tabIndex newName:(NSString *)newName requestID:(NSString **)pReqID;
- (BOOL)updateGroupTabsIndexWithSessionId:(NSString *)sessionId tabsList:(NSArray *)tabsList requestID:(NSString **)pReqID;
- (void)getSessionTabsWithSessionId:(NSString *)sessionId tabsList:(std::list<NS_ZOOM_MESSAGER::GroupTabInfo>&)tabsList;

@end

@protocol ZMCSessionTabSink <NSObject> //!IZoomSessionTabsUI
@optional

//! action ack
- (void)onAddGroupTabWithRequestID:(NSString *)reqId sessionId:(NSString *)sessionId errCode:(int32_t)errCode;
- (void)onDeleteGroupTabWithRequestID:(NSString *)reqId sessionId:(NSString *)sessionId errCode:(int32_t)errCode;
- (void)onModifyGroupTabNameWithRequestID:(NSString *)reqId sessionId:(NSString *)sessionId errCode:(int32_t)errCode;
- (void)onUpdateGroupTabsIndexWithRequestID:(NSString *)reqId sessionId:(NSString *)sessionId errCode:(int32_t)errCode;

//!  sync from other client
- (void)notifyGroupTabsUpdateWithSessionId:(NSString *)sessionId;
- (void)notifyGroupTabsActionInfo:(const ns_zoom_messager::GroupTabActionInfo&)actionInfo;

//! for p2p session, read data from DB ready or fetch data from server ready
- (void)notifySessionTabsReady;

@end

@protocol ZMCSessionTabUI <NSObject>
@optional


@end


#endif /* ZMCSessionTab_h */
