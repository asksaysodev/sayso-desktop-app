#pragma once
#include <zPTApp/SaasBeePTAppInterface.h>
#include <zVideoApp/CmmConfAPI.h>
using namespace Cmm;

class ICmmConfMgrAPI;
class IZPPTAppSink;

class ZMPTSink :  public ISBPTAppSink
, public ICmmDeviceUIAPI
{
public:
    ZMPTSink(void);
    ~ZMPTSink(void);
    
    ISBPTAppAPI* GetPTAppApi();
    ICmmConfMgrAPI* GetConfAppApi();
    
    /*
     * Init PT UI Module
     * brief: call once when NOTIFY_AFTER_INIT
     *
     */
    void      InitModule();
    void    InitPTAppApi();
    void    InitConfAppApi();
    void    UninitConfAppApi();
    
public: //ISBPTAppSink
    virtual void OnGetNoAuthNumberOfMeetingsV2Response(const RequestMarketingDialogInfoParam &response, const CString &requestID) override;
    virtual void SinkAppEvent(PT_EVENT ptEvent, CmmUInt32 result, void* content = nullptr) override;
    //Raul added to make compiler happy.
    virtual void SinkIMEvent(PT_IM_EVENT ptIMEvent, CmmUInt32 result, void* content) override;
    virtual void SinkIPCEvent(PT_IPC_EVENT ptIPCEvent, CmmUInt32 result, void* content) override;
    virtual void SinkMeetingEvent(PT_MEETING_EVENT ptMeetingEvent, CmmUInt32 result, void* content) override;
    virtual void NotifyReleaseNoteForLatestVersion(const CString& version, const CString& notes) override;
    virtual void NotifyReleaseNoteForLatestVersionEx(const ReleaseNoteResult& result) override;
    virtual CmmBool PromptToInputUserNamePasswordForProxyServer( const CString& proxyHost, CmmUInt32 port, const CString& description ) override;
    virtual void SinkFavoriteEvent(PT_FAVORITE_EVENT ptEvent, CmmUInt32 result, void* content) override;
    virtual int SinkStartRecConverter(const CString& path, CmmInt64 itemID) override;
    virtual int SinkOpenRecord(CmmInt64 itemID) override;
    virtual int SinkPlayRecord(CmmInt64 itemID) override;
    virtual int SinkPlayAudioRecord(CmmInt64 itemID) override;
    virtual int SinkDeleteRecord(CmmInt64 itemID) override;
    
    virtual void SinkSearchDomainUser(const CString& requestID, CmmUInt32 result, CmmUInt32 contactCount, const NS_ZOOM_DATA::arrContacts& contacts ) override;
    virtual void SinkFavAvatarReady( const CString& userid) override;
    virtual void OnQuerySSOVanityURL(const CString& reqID, CmmUInt32 result, const CString& url) override;
    virtual void PresentToRoomStatusUpdate(PresentToRoomStatus status) override;//ZOOM-5960
    virtual void OnAppProtocolActionBlocked(UrlAction action, SSBPTERROR err) override;//[Zoom-16131]
    virtual void OnShowPrivacyDialog(const CString& privacyURL, const CString& tosURL) override;//ZOOM-40537 GDPR
    virtual void NotifyUIToLogOut() override;

#ifdef ENABLE_ZAPP
    virtual void OnToggleZappFeature(CmmInt state) override;
    virtual void OnWebviewForceUpdate(const CString& webviewVersion) override;
    virtual void OnWebviewValidityFromWeb(const std::vector<NS_ZOOM_DATA::WebViewStatusInfo>& webviewInfo) override;
#endif // ENABLE_ZAPP

    virtual void NotifyLatestUnsupportLocalhostVersion(const CString& latest_unsupport_version) override;//ZOOM-37997
    /*Add by Lin Han*/
    
    virtual void SinkChatProtEvent(PT_CHATPROTOCOL_EVENT ptEvent, const DataLunchRequest& request_info) override;
    virtual void NotifyVideoDeepLink(const VideoDeepLinkParam& param) override;
    virtual void SinkOnZoomLobbyEvent(PT_ONZOOMLOBBY_EVENT ptEvent, const RequestLobbyErrorParam& requestLobbyErrorInfo) override;//ZOOM-255235
    virtual void OnDeeplinkRoutePage(DeeplinkRoutePageParam params) override;
    virtual void OnDeeplinkPopoutActionReceived(const std::string& params) override;
    virtual void OnAlertPopoutActionReceived(const std::string& params) override;
    virtual void OnMenuPopoutActionReceived(const std::string& params) override;
    virtual void OnDynamicUIHandleMessageReceived(const std::string& params) override;
    
    virtual void OnAddWebPageDeeplink(const Cmm::CAString& url, const Cmm::CAString& name, const std::map<Cmm::CAString, Cmm::CAString>& params_map = {}, CmmBool can_navigate = CmmTrue) override;
    virtual void OnOpenZoomSupport() override;
    virtual void OnWebPageLoadResultForDeeplink(const CString& req_id, const CString& name, const CString& url) override;

    /**Add by Jed Zheng*/
    virtual void NotifyRequestLobby(const RequestLobbyParam& param) override;
    virtual CmmBool IsOnZoomWindowOpened() override;
    virtual void NotifyRequestNewLobby(const RequestLobbyParam& param) override;
    virtual void NotifyOnZoomChannelWindow(void* wnd) override;
    virtual void NotifyRemoveOnZoomChannel(const NS_ZOOM_MESSAGER::OnZoomRemoveChannelInfo& info) override;
    virtual void NotifyOnZoomWebviewForceUpdate() override;
    virtual void NotifyActivateOnZoomWindow(CmmBool bDelay = CmmFalse) override;
    virtual void JsSdkCallResultForZoomEvents(const CString& reqId, const CAString& extraData, const CString& funcName) override;
    virtual void NotifyOnZoomJoinStatusChange(const CAString& status, const CString& meetingId) override;
    virtual void NotifyParingStatusChange(const CAString& eventJson) override;
    virtual void NotifyOnCommonNotificationToLobby(const CAString& eventJson) override;
    virtual CmmBool Notify_Zpns_OnPushSetOfflineZESettingData(const CString& reqId, const CmmInt64& result) override;
    virtual CmmBool Notify_Zpns_OnPushGetOfflineZESettingData(const CString& reqId, const CmmInt64& config, const CmmInt64& result) override;
    virtual void OnGetZETokenWithFileIdDone(const CString& fileId, const CString& token, const CString& source) override;
    
    virtual void OnPromptFirstUpdate(CmmBool bShow, FirstUpdatePromptType type, CmmBool& done) override;
    virtual void OnPromptDelayUpdateNotify(CmmBool& done) override;
    virtual void RestartAppWhenNetworkResume() override;
    virtual void OnCmrStorageInfoPush(const SB_webservice::CMRStorage& info) override;
    virtual void OnCmrStorageProfileResponse(const SB_webservice::CMRStorage& info) override;
    
    virtual void OnCustomMessageFromConf(CmmUInt32 type, const CmmUInt8* pMsgData, CmmUInt32 size) override;
    virtual void OnDocStatusPush(const NS_ZOOM_DATA::DocStatusPush& push) override;
    virtual void OnTasksStatusPush(const NS_ZOOM_DATA::TasksStatusPush& push) override;//ZOOM-910581
    virtual void CopyHuddleLink(const CString& channelId) override;

public:
    virtual CmmBool OnDeviceStatusChanged(DEVICE_CMD cmd,CmmUInt32 ret=0,CmmUInt32 handle =0) override; // Along with a status code for each status
    virtual CmmBool ShowMyAudioLevel(CmmUInt32 level,CmmUInt32 handle, CmmBool isMic = CmmTrue) override;
    virtual CmmBool OnVolumeChange(CmmUInt32 vol, CmmBool isMic) override;
    ICmmAudioAPI* GetAudioObj();
    ICmmVideoAPI* GetVideoAPI();
    virtual void OnAudioDeviceSpecialInfoChange(SSB_MC_DEVICE_SPECIAL_INFO special_info, CmmAudioDeviceBriefInfo adbi) override;
    
    /*end of adding*/
public:
    //App event
    virtual void SinkShowChatPath(const CString& path) override;
    virtual CmmBool VTLS_NotifyCertItemVerifyFailed(const NS_ZWEBSERVICE::VerifyCertEvent& event) override;
    virtual void OnPKCEFacebookAuthReturn(const CString& encryptToken, const CString& code_verifier, CmmUInt32 expires_in, CmmInt32 error, const CString& error_str) override;
    virtual void OnPKCEGoogleAuthReturn(const CString& encryptedToken, const CString& code_verifier, CmmInt32 error, const CString& error_str) override;
    virtual void OnPKCESSOLoginTokenReturn(const CString& encryptToken, const CString& code_verifier) override;
    virtual void OnUpdateSipPhoneStatus(CmmUInt32 result) override;
    virtual CmmBool GetIsInSipCall() override;
#ifdef SUPPORT_ZOOM_PHONE_BLOCK_FEATURE
    virtual void OnZoomPhoneBlockStatusChanged(bool is_blocked) override;
    virtual void OnZoomPhoneZPNSLoginStatus(CmmZPNSLoginStatus status) override;
#endif
    virtual void NotifyLocalAddressChanged(const CString& old_ip, const CString& new_ip) override;
    virtual CmmBool SinkSipcallSDKCheckUpdate(const CString& latestVersion, const CString& downloadURL, const CString& releaseNote, const CString& requestID, CmmUInt32 result) override;
    virtual CmmBool SinkSipcallSDKDownload(const CString& requestID, const CString& localFilePath, CmmUInt32 result) override;
    virtual CmmBool SinkCodeSnippetPackageCheckUpdate(const CString& latestVersion, const CString& downloadURL, const CString& releaseNote, const CString& requestID, CmmUInt32 result) override;//[Zoom-32934]
    virtual CmmBool SinkPTUIExtPackageCheckUpdate(const CString& latestVersion, const CString& downloadURL, const CString& releaseNote, const CString& requestID, CmmUInt32 result) override;//[Zoom-117601]
    virtual void OnNeedForceUpgrade(const UpdateAppParam& param) override;
    virtual void OnNeedSoftUpgrade(const UpdateAppParam& param) override;
    virtual void OnUploadFeedbackResult(const CString& reqid, CmmUInt32 result) override;
    virtual void OnRevokeXmppToken(const CString& reqID, CmmUInt32 result) override;
    virtual void OnShowSignToJoinOption(const CString& detailLinkUrl, CmmBool couldSkip, const CString& accountName, const CString& emailDomain) override;
    virtual void OnShowLoginDialog(const CString& prefillEmail) override;
    virtual void OnOAuthReturn(const OAuthInfo& info) override;
    virtual void OnWebLaunchedToLogin(const WebLaunchedToLoginParam& param) override;
    virtual void OnShowLoginDisclaimerDialog(const NS_ZOOM_DATA::DisclaimerPrivacy& disclaimer) override;
    virtual void OnWebOpenLocalRecordingFolder(const OpenLocalRecordingFolderParam& param) override;
    virtual void OnShowAgeGatingDialog() override;
    virtual void OnShowCrashReport() override;
    virtual void OnStatCrashReport(const std::vector<CString>& arrDump) override;
    virtual void NotifyUnKnownURLScheme() override;
    virtual void NotifyCloudRecordingComplete(std::map<CString, CString> eventMap) override;
    virtual void OnMultiFactorAuthRequest(const NS_ZWEBSERVICE::MultiFactorAuth& mfa) override;
    virtual void NotifyZAKRefreshFailed(CmmUInt32 err_code) override;
    virtual void OnEscrowServiceEnabled() override;
    virtual void OnKeyServerSigchainAlreadyEnabled(int init_reason) override;
    virtual void OnFreeUserLicenseUpdate(CmmInt64 seqid, const CAString& eventId) override;
    virtual void OnMailAccountCreate() override;
#ifdef ENABLE_ZOOM_CCI
    virtual void NotifyPAKandCAKRefreshed(CmmUInt32 err_code) override;
    virtual void NotifyTurnOnOffCCIAudio(CmmBool isTurnOn) override;
    virtual CmmBool IsCCIUsingAudio() override;
    virtual void NotifyCCIUserDeleted() override;
    virtual void OnDownloadFileByUrlForCCI(const CString& requestID, const CString& localFilePath, CmmUInt32 result, const std::vector<CAString>& headers) override;
    virtual void SyncClientAudioSettingsToCCI() override;
    virtual void NotifyCCINetworkState(CmmBool isConnected) override;
    virtual void OpenMainWindowAndJumpToCCITab() override;
    virtual void NotifyOnDoTransferAcceptCallForCCI(const Cmm::CString& reqID, const CmmUInt32 retCode, const CAString& resp, const CAString& tracking_id) override;
    virtual void NotifyCCILeaveNativeVideo() override;
    virtual void StartEscalationFromZPToZCC(const Cmm::CString &url, const Cmm::CString &name) override;
#endif
    virtual CmmBool IsCurrentUserEnableCCI() override;
    virtual void OnRecaptchaRequest(const SB_webservice::RecaptchaInfo& info) override;
    virtual void OnShowPasswordExpiredDialog(const CString& passwordResetUrl) override;
    virtual CmmBool GetIsSipFeatureEnabled() override;
    CmmBool SinkRingtoneCheckUpdate(const CString& latestVersion, const CString& downloadURL, const CString& releaseNote, const CString& requestID, CmmUInt32 result) override; //ZOOM-181828
    virtual CmmBool SinkLocationCheckUpdate(const CString& latestVersion, const CString& downloadURL, const CString& releaseNote, const CString& requestID, CmmUInt32 result) override;
    virtual void OnGetSecurityParams(GetSecurityParamsStatus stauts, GetSecurityParamsError error) override;
    
    //Whiteboard relevants start
    virtual void OnWBJsOpenNewWindowFromUrl(const CAString& url, bool* isSucceed = nullptr) override;
    virtual void OnWBJsCloseOpenWindowForce(IWBWebViewJSMessageSender* sender) override;
    virtual void OnWBJsOpenIMDashboard() override;
    virtual void OnWBJsReportProblem() override;
    //Whiteboard relevants end
    
    virtual void OnWBJsOpenSaveFileFolder(const CString& save_path) override;
    
    virtual void NotifySendNavIconListDone(const CString& requestID, CmmUInt32 result, const CString& errorMessage) override;
    
    virtual CmmBool DoTransferMeetingFromPairedZR(const CString& resourceID, const CString& deviceID, const CString& meetingNumber) override;
    virtual CmmBool DoSwitchToComputerOnlyFromPairedZR(const CString& resourceID, const CString& deviceID, const CString& meetingNumber) override;
    virtual CmmBool JoinMeetingCompanionModeWithPairedZR(const CString& resourceID, const CString& deviceID, const CString& meetingNumber) override;
    virtual CmmBool HasNearByZR() override;
    virtual CmmBool JoinFromRoomOnly(CmmInt64 meeting_number, const CString& password, const CString& personal_link, CmmBool is_host) override;
    
    virtual CmmBool IsClientSupportTab(const CString& tab_name) override;
    
	virtual void NotifyStartCaptureOnZoomWindow(void* wnd, const CString& reqid) override;

    virtual void SinkForOpenAICByDeepLink(int returnCode, const std::map<CAString, CString>& params) override;
    
#ifdef ENABLE_MAILCLIENT_SERVICE
    virtual CmmBool SinkMailSDKCheckUpdate(const CString& latestVersion, const CString& downloadURL, const CString& releaseNote, const CString& requestID, CmmUInt32 result) override;
    virtual CmmBool SinkMailSDKDownload(const CString& requestID, const CString& localFilePath, CmmUInt32 result) override;
#endif
    virtual void SinkCheckVanityUrl(const NS_ZWEBSERVICE::CheckVanityUrlResp& resp) override;
    virtual void OnInviteToCallBySMSPush(const CmmInt64& meetingNO, const CString& messageID, CmmUInt32 result) override;
    virtual void OnInviteToCallBySMSResponse(const CString& requestID, const CString& messageID, CmmUInt32 result) override;
    virtual void OnMarketBannerInfoGet(const RequestBannerInfoParam& params, const CString& requestID) override;
    virtual void OnGetMarketNoticeResponse(const CString& requestID, CmmUInt32 result, const std::vector<zWebService_Biz::ClientMarketingBannerV2>& response) override;
    virtual void OnMarketBannerInfoClose(CmmInt32 result, const CString& requsetID) override;
    virtual void OnMarketingDialogInfoGet(const RequestMarketingDialogInfoParam &params, const CString &requestID) override;
    virtual void OnOtpNotificationReceived(const SB_webservice::OtpNotificationInfo info) override;
    virtual void onOtpNotificationConfirmResponse(const CString& requestID, CmmUInt32 result, const CString& resetPwdUrl) override;
    virtual void OnZoomPhoneLicenseStatusChange(CmmBool is_license_enabled, CmmUInt64 old_bit_options, CmmUInt64 new_bit_options) override;
    virtual void NotifyIDPSSOAction(const SB_webservice::IDPSSOActionParam& param) override;
    virtual void NotifyNeedSwitchAccount(const ISBPTAppSink::SwitchAccountDatas& switchAccountDatas, CString newAccountEmail, CString currentUserName) override;
    virtual void OnRequestVOBillingNoticeBanner(const std::vector<RequestVOBillingNoticeBanner>& banner_list) override;
    virtual void OnBillingStatusPushReceived() override;
    virtual void onZpnsCommonCoachmarkReceived(const std::vector<CmmZpnsCoachmark>& vec_coachmarks) override;
    virtual void OnScheduleBannerShow(const CString& msg) override;
    virtual void OnScheduleBannerButtonClick(const CString& msg) override;
    virtual void OnCheckNewReleaseNotesResponse(const CString& requestID, CmmUInt32 result, CmmBool isReleaseNotesAvailable) override;
    virtual void OnRequestForGetABTestResponse(const CString& requestID) override;
    virtual void OnZpnsCommonSidepanelReceived();
    virtual void OnAICompanionSettingUpdate();
    virtual void OnZpnsCommandClearListReceived(const std::map<CString, std::vector<CString>>& clear_list);
public:
#ifdef ENABLE_ZOOM_CALENDAR
    void GetCalendarSystemParam(CalendarSystemParam &calSysParam) override;
    virtual void CallPhoneNumberByCalendar(const CString& phonenumber) override;
#endif
    void SinkShowAicPlusSidePanel(CmmBool visible) override;
    CmmBool IsAicPlusSidePanelDisplayed() override;
    CmmBool SinkSendEventToAicPlus(const std::map<CString,CString>& eventParams) override;

    void RegisterDeviceBy3rdSdk(const CString& device_name) override;
    void SendMeetingDocsShareCollaboratorsInviteInfo(const std::vector<CString>& user_id) override;
    void PostJsonMessage2Web(const CAString& eventData) override;
    void setZPPTAppSink(IZPPTAppSink* appSink);
protected:
    ISBPTAppAPI*        m_pPTAppApi;
    ICmmConfMgrAPI* m_pConfAppApi;
    IZPPTAppSink * m_appSink;
public://ASR
    void NotifyToCheckUpdateASRLibrary() override;
    void NotifyToCheckUpdateASRModels(const asr_model_download_infos& models) override;
    void NotifyToPauseAllDownloadingASRModels() override;
    void OnDownloadingFileByUrl(const CString& requestID, const CString& localFilePath, CmmUInt32 result) override;
};


