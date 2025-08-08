//
//  ZMZappUIProtocol.h
//  zAppUI
//
//  Created by TOTTI on 11/13/21.
//  Copyright © 2021 us.zoom. All rights reserved.
//

#ifndef ZMZappUIProtocol_h
#define ZMZappUIProtocol_h

@class ZMZAppConfShareInfo;
NS_ASSUME_NONNULL_BEGIN

@protocol ZMZappUIProtocol <ZMRoutableObject>

- (BOOL)isZappServiceReadyForAppType:(ZMAppType)appType;
- (void)initConfZappService;
- (void)uninitConfZappService:(BOOL)needDelay;
//zappui in pt
- (void)onAppTabSelected:(BOOL)selected;
- (void)onLabsTabSelected:(BOOL)selected;
- (void)onZoomLogoutFinished;
- (void)checkAndUpdateAccount;
- (void)tryToOpenAppInPanel;
- (void)clearWebCache;
- (void)cleanAllMirrorAppsInPT;
- (void)cleanSavedMirrorAppsInPT;
- (void)cleanAllMirrorAppsInMT;
- (void)restoreMirrorAppsIfNeeded;
- (void)onWebviewForceUpdateInPT;
- (void)relayoutZappBubbleIfNeeded;
- (void)openZappByFax:(NSString *)appId invitationId:(NSString *)invitationId;
//for setting
- (NSImage *)settingIcon;
- (NSImage *)settingIconHighlignt;
- (NSView *)getZAppSettingView;

//zappui in meeting
- (void)windowDidEnterFullScreen;
- (void)windowDidExitFullScreen;
- (void)jumpZappByShareInfo:(ZMZAppConfShareInfo *)shareInfo;

- (void)handleZAppButtonClicked:(NSView *)view;
- (void)sendingShareDidStarted;
- (void)sendingShareDidStopped;
- (BOOL)iszAppSharingByShareAppId;
- (void)onSwitchMinimalVideoMode:(BOOL)toMinimal;

- (void)restoreZappPanel;
- (BOOL)sendAppToAllAttendeesWithAppId:(NSString *)appId
                                  name:(NSString *)appName
                        invitationInfo:(ZMZappConfShareConfigure *)invitationInfo
                                  type:(NSUInteger)type;
- (BOOL)needShowAllParticipatesInWebinar:(NSString *)appId;
- (BOOL)sendAppToSelectedAttendeesWithAppId:(NSString *)appId
                                       name:(NSString *)appName
                             invitationInfo:(ZMZappConfShareConfigure *)invitationInfo
                                  attendees:(NSDictionary *)attendees
                                   menuType:(NSUInteger)menuType;
- (BOOL)shareVirtualBackground:(NSString *)filePath appId:(NSString *)appId menuType:(ZMZAppSendMenuType)menuType attendees:(NSArray *)attendees;
- (NSString *)getInvitationThumbnailPath:(NSString *)msgId;
- (void)downloadInvitationThumbnail:(NSString *)msgId downloadParam:(ZMZappDownloadParam *)param;
- (BOOL)isNeedShowTipsAtSidePanelRemoved;
- (void)showExistPushedApp;
- (void)onMeetingChatStatusChanged;
- (void)updateZappViewFrame;
- (NSString *)getConfCurAppId;
- (BOOL)needHideInShareCtrl;
- (void)popOutWindow;
- (void)closePopoverControllerIfNeeded;

- (void)shareZappBy:(NSDictionary *)shareParam;
- (NSString *)getShareAppWindowName;
- (ZPShareSelectWindowModel *)getZappPanelShareModel;
- (NSArray<ZPAppShareSelectWindowModel *> *)getZappPopOutWindowModel;
- (NSArray<ZPAppShareSelectWindowModel *> *)getAppsSharelList;
- (NSArray<NSString *> *)getMTOpenAppIdList;
- (BOOL)isAppInCollaborateMode:(NSString *)appId;
- (BOOL)isAppCollaborateEnable:(NSString *)appId;
- (BOOL)isAppCanCollaborate:(NSString *)appId;
- (void)startCollaboration:(NSString *)appId;
- (void)shareWindowOpenApp:(NSString *)appId;
- (void)getAppInfo:(NSString *)appId completion:(void (^)(NSString *appName, NSString *iconPath))completion;
- (void)closeShareWindowApp:(NSString *)appId;
- (void)showDiscoveryPage;
- (void)showAppWithAppId:(NSString *)appId;
- (BOOL)isGetCollaborativeAppsFeatureEnable;
- (NSView *)getAppShareWindowWorkView;
- (NSView *)getAppshareWindowFootView;
- (void)notifyAppTabSelect:(BOOL)isAppTab;
- (void)shareSelectWindowShow:(BOOL)isShow;
- (void)shareWindowAppTabWillHide;
- (void)setMTShareAppId:(NSString *)appId;
- (void)stopAppShare;
- (BOOL)isAppSharing;
- (void)setAppPopWindowSize:(NSSize)size appId:(NSString *)appId;
- (void)shareZappInPanel:(NSDictionary *)shareParam;
- (void)updateShareWindowSidePanelImg:(NSString *)appId;

- (void)showAppInConf:(NSString *)appId;
- (void)onWebviewForceUpdate;
- (void)onCefWebviewStatusChanged:(ZAppWebViewStatus)status;

- (void)OnShareSourceClosed:(NSUInteger)userId;
- (void)windowWillEnterFullScreen;
- (void)setNeedPopupZapp:(BOOL)needPopupZapp;
- (void)closeZappPopoverTips;
- (void)cleanupConfZappMgr;
- (NSWindow *)getConfMainWindow;
- (void)openPushedAppById:(NSString *)appId;
- (void)showPushAppTipIfNeed;
- (void)showPushedAppToastIfNeed;

- (void)updateAppWhenAcountMismatch;
- (void)onPTLogin;
- (void)onCopyLink:(NSView *)positionView appId:(NSString *)appId appType:(ZMAppType)appType;
//BO
- (void)onMyRosterCompleted:(ZMZappConfSessionType)sessionType;
- (void)setAppInPanel:(BOOL)appInPanel;
- (void)setRestoreBOAppId:(NSString *)appId;
- (void)setPanelWidth:(CGFloat)panelWidth;
- (void)setShouldPersistApp:(BOOL)persist;
- (void)setMainWindowWidth:(CGFloat)width;
//toast & popover
- (void)showReceiveAppToastFrom:(NSString *)senderName shareInfo:(ZMZAppConfShareInfo *)shareInfo;
- (void)showSentAppToastWithType:(NSInteger)messageType appId:(NSString *)appId receiveId:(int)receiveId receiveName:(NSString *)receiveName;
- (void)showZappPopoverTipOfView:(NSView *)view appType:(ZMAppType)appType;
- (void)closeZappPopover:(ZMAppType)appType needReminder:(BOOL)needReminder;
- (BOOL)isZappPopoverTipShowing:(ZMAppType)appType;
- (BOOL)isZappNewDockShowing;
- (ZPZAppHeader *)convertZappHead:(zoom_apps::ZappHead)head;
- (void)onToggleZappFeature:(zappEnableState)state appType:(ZMAppType)appType;
- (void)onConfReady;
- (void)OnVirtualBackgroundSet:(NSUInteger)errorCode eventId:(NSString *)eventId;
- (NSString *)getLaunchTrackingId:(NSString *)appId appType:(ZMAppType)appType;
- (void)relayoutPluginView;
- (NSString *)getCurrentPluginAppId;
- (NSView *)getPluginView;
- (void)cleanCurrentPluginResource;

- (void)needShowAppAfterPTLogin;
- (void)needShowAppAfterPTLogout;

#pragma mark - Collaborate Zapp UI Interface
- (BOOL)isCenterCollaborateLayoutStyle; // only for UI judgement
- (BOOL)isCollaborateLayoutStyle;
- (BOOL)isMySelfCollaborateSender;
- (void)switchToCollaborateMode:(NSWindow *)parentWindow frame:(NSRect)frame;
- (void)updateCollaborateWindowLayout:(NSRect)windowRect;
- (void)startOAuthInClient:(NSString *)appId;
- (void)showJoinCollaborateInvitationIfNeed;
- (void)joinCollaborate:(ZMZAppToastInfo *)toastInfo isFromJs:(BOOL)isFromJs;
- (NSString *)getCurrentCollaborateAppId;
- (ZMZAppCollaborateData *)getCollaborationData:(BOOL)bFromBoMaster;
- (nullable NSMutableDictionary *)getCollaborateViewInfoDictOfType:(ZMZappCollaborateViewType)type
                                                             appId:(nullable NSString *)appId
                                                    isPlayTogether:(nullable BOOL *)isPlayTogether;
- (void)endCollaborateShowAlertIfNeed:(NSString *)appId;
- (void)windowDidStartVideo;
- (void)windowDidStopVideo;
- (void)showCollaborationPolicySettingWindow;
- (void)closeCollaborationPolicySettingWindow;
- (void)exitCollaborateModeOfAppId:(nullable NSString *)appId options:(ZMZappExitCollaborateModeOptions)options isFromJs:(BOOL)isFromJs;
- (void)showCollaborateOrPushAppFailAlertOfErrorMsg:(NSString *)errorMsg;

//ZOOM-341533
- (NSUInteger)getOpenedAppsCount;
- (BOOL)needShowZappUnreadFlag;
- (BOOL)isLastMeetingHasKeepApps;
- (void)setKeepAppsInMeetings;
- (BOOL)needShowErrorWebView;
- (void)cleanConfZappUIData;
- (void)closeConfZappUI;
- (void)cleanConfCacheStatus:(ZMZAppCacheDataModule)cleanModules;

- (void)updateZappPopOverTipWithPositionView:(NSView *)positionView;

// ZOOM-358916
- (void)userRenameWithId:(NSUInteger)inUserId;
- (void)userAvatarChangedWithId:(NSUInteger)inUserId;

//sidepanel
- (void)onZAppSidePanelResized;
- (void)backupZApp:(id)sender action:(ZMSidePanelWindowAction)action;
- (void)restoreZApp:(id)sender action:(ZMSidePanelWindowAction)action;
- (void)onExpandModeChanged:(BOOL)isExpand;
- (void)removeZappsView;
- (void)onZappViewRemoved;
- (void)setMTPopOut:(BOOL)popOut;
- (void)setNeedPopupAfterExitFullScreen:(BOOL)popOut;
- (void)setMTRestoreZapp:(BOOL)restoreZapp;
- (void)notifyWindowPopoutWithAppId:(NSString *)appId isPopout:(BOOL)isPopout height:(NSInteger)height width:(NSInteger)width;
- (void)onAppWindowRemoved;

// Cef
- (void)cefAgentDownloadDidFailed;
- (void)cefAgentDownloadDidFinished:(NSString *)path;
- (void)cefAgentUpgrade;

// ZOOM-380822
- (void)triggerJSEventOnMeetingViewChanged:(NSInteger)viewType;

- (NSString *)getAppInvitationInfoUrl:(NSString *)appId type:(NSUInteger)linkType;
- (NSImage *)getZappImageByName:(NSString *)name;
- (BOOL)checkAppInstalled:(NSString *)appId completion:(ZMCallbackBlock)completion;

- (NSString *)getZappChannelLauncherUrlWithChannelId:(NSString *)channelId;

- (NSArray *)getOpenedAndUsedApps;
- (BOOL)isPushAppOpened:(NSString *)appId;
- (void)cleanUsedAppList;

// VF
- (void)zoomMeetingWillLeave;

//ZOOM-414131
- (BOOL)needShowGuestInfo;

// leave and join meeting
- (void)notifyConferenceUIDidCreate;
- (void)notifyPtClearJoinMeetingCache;

//bullet
- (void)chatMessageReceived:(NSString *)jsonMsg;
- (BOOL)isBulletAppOpened;
- (BOOL)isBulletEnable;
- (void)openBullet;
- (void)closeBulletApp:(BOOL)needClean;
- (void)setBulletChecked:(BOOL)checked;

//app dock
- (NSString *)getSidePanelAppId;

- (void)onSessionChangedRefresh;
// ZOOM-449099 For ZR
- (BOOL)isNormalAppInMainWindow; // third-part app in main window
- (NSString *)openedNormalAppId; // main window app Id
- (NSString *)openedNormalAppName; // main window app name
- (BOOL)isAppSupportTransfer2ZRInPT;
- (void)backAndReloadAppLauncher;
- (void)onZRPairedStatusChanged:(BOOL)paired;
- (void)onFailedOpenAppOnZR:(BOOL)inMeeting errorCode:(ZMZAppOpenInZRErrorCode)errorCode appName:(nullable NSString *)appName;
- (nullable NSString *)openedNormalAppCurrentURL;

- (void)triggerJSEventOnGalleryPageChange:(NSUInteger)newPage totalPage:(NSUInteger)totalPage;

//transtion
- (void)transitAppToPT;

- (void)keepPreviousMeetingInfo;
//ZOOM-464036 for meeting dropdown
- (NSInteger)getTimerCount;
- (BOOL)getTimerData:(NSMutableDictionary<NSString *, ZMZAppDynamicIndicatorInfo *> *)timerData;
- (NSInteger)getTimerSenderId:(NSString *)appId;
- (void)showTimerIsUpToastInConf:(NSString *)appId iconPath:(NSString *)iconPath;
- (void)updateTimerIsUpToastInConf:(NSString *)appId iconPath:(NSString *)iconPath;
- (void)closeTimerAppIfNeeded;

- (void)onDumpFileUploaded:(NSString *)result;
- (void)onStartSendPureComputerAudio:(BOOL)start;

//local share
- (void)onLocalShareStopped;

- (void)appsTabDidPopOut;
- (void)labsTabDidPopOut;

// app setting
- (NSArray<ZPZAppHeader *> *)getInstallAppsList;
- (NSString *)getSettingManageUrl;
- (BOOL)setAutoOpenedInMeeting:(NSString *)appId bCheck:(BOOL)bCheck;
- (NSArray<NSString *> *)getAutoOpenedInMeeting;
- (BOOL)setKeepAppsOpenSetting:(BOOL)bCheck;
- (BOOL)getKeepAppsOpenSetting;
- (BOOL)getAutoOpenAppsFeatureEnable;
- (void)clearAppDataBtnClick:(NSWindow *)window;
- (NSMutableArray *)sortAutoOpenArray:(NSMutableArray<ZMZAppKeepModel *> *)array;
- (void)rtmsButtonClicked:(NSButton *)sender;
- (BOOL)isRTMSPanelShowing;
- (ZMMTAppRTMSPermission)getRTMSPermission;
- (void)setRTMSPermission:(ZMMTAppRTMSPermission)permission;
- (BOOL)shouldShowRTMSItemBtn;
- (NSInteger)getRTMSRequestingCount;
- (NSInteger)getRTMSActiveCount;
- (BOOL)isAnyoneRTMSActive;
- (void)showRTMSAllAppsDisabledAlert;
- (void)onHostChanged;
- (void)showRTMSCustomAlert:(NSString *)title customStr:(NSString *)customStr disclaimerID:(NSString *)disclaimerID;
- (void)updateRTMSDisclaimer;
- (void)cleanMTConsentDialogs;

- (void)triggerJsEventOnDocTimerClicked;
- (BOOL)isRTMSServiceEnabled;
- (BOOL)canUserOperateRtmsAANState;
- (void)onRTMSHasBeenDeleted;

//AAN
- (void)showRTMSAANPanel;
- (void)showRTMSAANPanelAndScrollToItemWithAppID:(nonnull NSString *)appID ownerIndex:(NSUInteger)ownerIndex;
- (NSArray<ZPZAppHeader*>*)getRTMSAppListForOwner:(CmmUInt32)ownerIndex;
- (void)onRTMSItemReady;
- (ZMBubbleObject*)getRTMSBubbleObject;
@end

NS_ASSUME_NONNULL_END

NS_ASSUME_NONNULL_BEGIN

@protocol ZMZappStatusProtocol <NSObject>
@optional
- (void)onZappEnableStatusChanged:(BOOL)enable;
@end

NS_ASSUME_NONNULL_END
#endif /* ZMZappUIProtocol_h */
