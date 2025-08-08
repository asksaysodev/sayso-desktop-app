//
//  IZMMTIQMgr.h
//  VideoUIBridge
//
//  Created by Jun Pang on 7/14/23.
//

#ifndef IZMMTIQMgr_h
#define IZMMTIQMgr_h
#import <zVideoUIBridge/ZMIQConstant.h>

@class ZMQueryResponse;
@protocol IZMMTIQMgr <ZMRoutableObject>

@optional
- (BOOL)isAICViewShowing;
- (void)initAICApp;
- (BOOL)isBtnAiCNeedShow;
- (BOOL)isAicRemindStartNeedShow;
- (BOOL)isAicCatchUpNeedShow;
- (BOOL)canShowAicStartWithRecordingStartCoachMark;
- (BOOL)isRemindTurnOnAICompanionLocked;
- (void)requestToSaveRemindHostTurnOnAICWebSetting:(BOOL)value;
- (NSInteger)onBtnAicClicked;
//- (void)onSetStartAicWithRecordingStart;
- (void)startSummaryQuestionsWithAccess;
- (void)onShowRecordingWithAicCoachMark;
- (void)onHideRecordingWithAicCoachMark;
- (BOOL)canShowBackWithCatchMeUp;
- (void)onShowReactionWithAicAction;
- (void)onReactionWithAicActionClicked:(NSString *)msg;
- (void)updateStartByselfStatus;
- (void)openQueriesPanel;
- (void)closeQueriesPanel;
- (void)toggleAICSidePanel;
- (BOOL)needCloseQueriesPanel;
- (void)tryToCloseQueriesPanel;
- (void)onStopQueriesButtonClicked;
- (void)actionPopup;
- (void)actionPopin;
- (void)showPopup;
- (void)onToggleMiniMode;
- (void)onWindowDidEnterFullScreen;
- (void)onWindowDidExitFullScreen;
- (void)showRequestEnableQueriesPanelIfNeed:(ZMMTAICFeatureType)featureType;
- (void)showAttendeeRequestStartPanelIfNeed:(ZMMTAICFeatureType)featureType isAccessRequest:(BOOL)isAccessRequest;
- (void)showHostStartConfirmPanelIfNeed:(ZMMTAICFeatureType)featureType;
- (void)showRequestStartPanelIfNeed:(NSString *)reqId 
                           senderId:(ZMUserID)senderId
                        featureType:(ZMMTAICFeatureType)featureType
                    isAccessRequest:(BOOL)isAccessRequest;
- (void)closeRequestStartQueriesPanel;
- (void)closeHostStartConfirmPanelIfNeed;
- (void)showAdminReceiveRequestEnableAICIfNeed:(ZMUserID)senderId enableBySelf:(BOOL)isBySelf;
- (void)showAdminReceiveRequestPanelIfNeed:(ZMUserID)senderId enableBySelf:(BOOL)isBySelf featureType:(ZMMTAICFeatureType)featureType;
- (void)closeNormalPanel;
- (void)closeAllPanel:(BOOL)needCloseDisclaimer;
- (void)setIsPopout:(BOOL)isPopout;
- (void)showTurnOffPanelIfNeed:(NSString *)reqId
                      senderId:(ZMUserID)senderId
                     aicOption:(long long)aicOption
                  actionBySelf:(BOOL)isBySelf
                    includeAll:(BOOL)includeAll;

- (void)refreshTurnOffPanel;
- (void)refreshTurnOnPanel;
- (void)refreshTurnOnAgainPanel;
- (void)onTurnOffButtonClicked;
- (BOOL)hasHostOrCohostInMeeting;
- (void)showTurnOnPanelIfNeed:(ZMUserID)senderId enableBySelf:(BOOL)isBySelf;
- (void)onTurnOnAllButtonClicked;
- (void)showAICStartTipIfNeeded:(BOOL)ignoreConfReady;
- (void)hideAICStartTip;
- (void)showAICStopTipIfNeeded;
- (void)hideAICStopTip;
- (void)showAttendeeRequestTurnOffPanelIfNeed;
- (void)updatePopoutButton;
- (void)makeFirstResponderIfPossible;
- (void)try2ShowAICSidePanelCoachMark;
- (void)showAICSidePanelCoachMark;
- (void)AICSidePanelCoachMarkShown;
- (void)writeMonitorLog:(NSInteger)eventType
            initiatorId:(unsigned int)initiatorId
            requesterId:(unsigned int)requesterId
             receiverId:(unsigned int)receiverId
           featureTypes:(NSInteger)featureTypes
                  reqId:(NSString *)reqId
         isDeleteAssets:(BOOL)isDeleteAssets;
- (void)showStartWebinarPanelIfNeed;

@end

@protocol IZMMTIQEvent <NSObject>

@optional
- (void)onQueryRequestResponseReceived:(BOOL)isTimeOut response:(ZMQueryResponse*)response;
- (void)onReceiveRequestToEnableQueryMsg:(ZMUserID)senderId;
- (void)onReceiveRequestToEnableAICMsg:(ZMUserID)senderId aicOption:(long long)aicOption;
- (void)onReceiveRequestToStartQueriesMsg:(NSString *)reqId senderId:(ZMUserID)senderId;
- (void)onQuerySwitchStatusChanged:(BOOL)enable;
- (void)onReceiveStartQueryResponseMsg:(BOOL)agree timeout:(BOOL)timeout;
- (void)onReceiveSwitchAICResponseMsg:(BOOL)timeout agree:(BOOL)agree switchAction:(NSInteger)switchAction;
- (void)onReceiveSwitchAICRequestMsg:(NSString *)reqId
                            senderId:(ZMUserID)senderId
                           aicOption:(long long)aicOption
                        switchAction:(NSInteger)switchAction;
- (void)onReceiveChangeQueryPrivilegeSettingReqMsg:(NSString *)reqId senderId:(ZMUserID)senderId expectSetting:(NSInteger)expectSetting;
- (void)onReceiveChangeQueryPrivilegeSettingRspMsg:(BOOL)timeout decline:(BOOL)decline;
- (void)onReportActivityForTranscriptResultReceived:(NSString *)reportId type:(NSInteger)type result:(BOOL)result;
@end

#endif /* IZMMTIQMgr_h */
