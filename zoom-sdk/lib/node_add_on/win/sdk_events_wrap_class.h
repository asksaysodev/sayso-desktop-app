#ifndef _sdk_events_wrap_class_h_
#define _sdk_events_wrap_class_h_
#include "sdk_wrap.h"
#include "zoom_native_to_wrap.h"
#include "zoom_native_sdk_wrap_core_def.h"
#include "zoom_native_sdk_wrap_core.h"

extern ZNativeSDKWrap _g_native_wrap;
class ZAuthServiceWrapEvent : public ZOOM_SDK_NAMESPACE::IAuthServiceEvent
{
public:
	void SetOwner(ZAuthServiceWrap* obj) { owner_ = obj; }
	virtual void onAuthenticationReturn(ZOOM_SDK_NAMESPACE::AuthResult ret) {
		if (owner_) {
			if (ZOOM_SDK_NAMESPACE::AUTHRET_SUCCESS == ret)
			{
				_g_native_wrap.GetMeetingServiceWrap().Init();
				_g_native_wrap.GetMeetingServiceWrap().GetMeetingVideoCtrl().Init();
				_g_native_wrap.GetMeetingServiceWrap().GetMeetingAudioCtrl().Init();
				_g_native_wrap.GetMeetingServiceWrap().GetMeetingAnnotation().Init();
				_g_native_wrap.GetMeetingServiceWrap().GetMeetingConfigCtrl().Init();
				_g_native_wrap.GetMeetingServiceWrap().GetMeetingH323Ctrl().Init();
				_g_native_wrap.GetMeetingServiceWrap().GetMeetingParticipantsCtrl().Init();
				_g_native_wrap.GetMeetingServiceWrap().GetMeetingShareCtrl().Init();
				_g_native_wrap.GetMeetingServiceWrap().GetMeetingRecordingCtrl().Init();
				_g_native_wrap.GetMeetingServiceWrap().GetMeetingUICtrl().Init();
				_g_native_wrap.GetMeetingServiceWrap().GetSDKSMSHelper().Init();
				_g_native_wrap.GetMeetingServiceWrap().GetMeetingLiveStreamCtrl().Init();
				_g_native_wrap.GetMeetingServiceWrap().GetMeetingReminderCtrl().Init();
				_g_native_wrap.GetMeetingServiceWrap().GetMeetingChatCtrl().Init();
				_g_native_wrap.GetMeetingServiceWrap().GetMeetingWaitingRoomCtrl().Init();
				_g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().Init();
				_g_native_wrap.GetMeetingServiceWrap().GetMeetingIndicatorCtrl().Init();
				_g_native_wrap.GetMeetingServiceWrap().GetMeetingWebinarCtrl().Init();
				_g_native_wrap.GetMeetingServiceWrap().GetMeetingQACtrl().Init();
				_g_native_wrap.GetMeetingServiceWrap().GetMeetingCloseCaptionCtrl().Init();
				_g_native_wrap.GetMeetingServiceWrap().GetMeetingReactionCtrl().Init();
				_g_native_wrap.GetMeetingServiceWrap().GetMeetingPollingCtrl().Init();
				_g_native_wrap.GetMeetingServiceWrap().GetMeetingWhiteboardCtrl().Init();
				_g_native_wrap.GetMeetingServiceWrap().GetMeetingDocsCtrl().Init();
				_g_native_wrap.GetSettingServiceWrap().Init();

			}
			owner_->onAuthenticationReturn(Map2WrapDefine(ret));

		}
	}
	virtual void onLogout() {
		if (owner_) {
			owner_->onLogout();
		}

	}
	virtual void onZoomIdentityExpired() {
		if (owner_) {
			owner_->onZoomIdentityExpired();
		}
	}
	virtual void onZoomAuthIdentityExpired() {
		if (owner_) {
			owner_->onZoomAuthIdentityExpired();
		}
	}
	virtual void onLoginReturnWithReason(ZOOM_SDK_NAMESPACE::LOGINSTATUS ret, ZOOM_SDK_NAMESPACE::IAccountInfo* pAccountInfo, ZOOM_SDK_NAMESPACE::LoginFailReason reason)
	{
		if (owner_)
		{
			if (ZOOM_SDK_NAMESPACE::LOGIN_SUCCESS == ret)
			{
				_g_native_wrap.GetAuthServiceWrap().GetDirectShareHelper().Init();
			}
			owner_->onLoginReturnWithReason(Map2WrapDefine(ret), Map2WrapDefine(reason));
		}
	}
	virtual void onNotificationServiceStatus(ZOOM_SDK_NAMESPACE::SDKNotificationServiceStatus status)
	{
	}
	virtual void onNotificationServiceStatus(ZOOM_SDK_NAMESPACE::SDKNotificationServiceStatus status, ZOOM_SDK_NAMESPACE::SDKNotificationServiceError error)
	{
	}
private:
	ZAuthServiceWrap* owner_;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZMeetingServiceWrapEvent : public ZOOM_SDK_NAMESPACE::IMeetingServiceEvent
{
public:
	void SetOwner(ZMeetingServiceWrap* obj) { owner_ = obj; }
	virtual void onMeetingStatusChanged(ZOOM_SDK_NAMESPACE::MeetingStatus status, int iResult)
	{
		if (owner_) {
			owner_->onMeetingStatusChanged(Map2WrapDefine(status), iResult);
		}
	}
	virtual void onMeetingStatisticsWarningNotification(ZOOM_SDK_NAMESPACE::StatisticsWarningType type)
	{
		if (owner_) {
			owner_->onMeetingStatisticsWarningNotification(Map2WrapDefine(type));
		}
	}
	virtual void onMeetingParameterNotification(const ZOOM_SDK_NAMESPACE::MeetingParameter* pMeetingParam)
	{
		if (owner_ && pMeetingParam) {
			ZNMeetingParameter zn_meeting_parameter;
			zn_meeting_parameter.is_auto_recording_cloud = pMeetingParam->is_auto_recording_cloud;
			zn_meeting_parameter.is_auto_recording_local = pMeetingParam->is_auto_recording_local;
			zn_meeting_parameter.is_view_only = pMeetingParam->is_view_only;
			if (pMeetingParam->meeting_host)
			{
				zn_meeting_parameter.meeting_host = pMeetingParam->meeting_host;
			}
			if (pMeetingParam->meeting_topic)
			{
				zn_meeting_parameter.meeting_topic = pMeetingParam->meeting_topic;
			}
			zn_meeting_parameter.meeting_number = pMeetingParam->meeting_number;
			zn_meeting_parameter.meeting_type = Map2WrapDefine(pMeetingParam->meeting_type);
			
			owner_->onMeetingParameterNotification(zn_meeting_parameter);
		}
	}
	virtual void onSuspendParticipantsActivities()
	{
	}
	virtual void onAICompanionActiveChangeNotice(bool bActive)
	{
	}
	virtual void onMeetingTopicChanged(const zchar_t* sTopic)
	{
		if (owner_) {
			ZoomSTRING zn_topic;
			if (sTopic)
			{
				zn_topic = sTopic;
			}
			owner_->onMeetingTopicChanged(zn_topic);
		}
	}
	virtual void onMeetingFullToWatchLiveStream(const zchar_t* sLiveStreamUrl)
	{
		if (owner_) {
			ZoomSTRING zn_liveStreamUrl;
			if (sLiveStreamUrl)
			{
				zn_liveStreamUrl = sLiveStreamUrl;
			}
			owner_->onMeetingFullToWatchLiveStream(zn_liveStreamUrl);
		}
	}
private:
	ZMeetingServiceWrap* owner_;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZMeetingUIControllerWrapEvent : public ZOOM_SDK_NAMESPACE::IMeetingUIControllerEvent
{
public:
	void SetOwner(ZMeetingUICtrlWrap* obj) { owner_ = obj; }
	virtual void onInviteBtnClicked(bool& bHandled)
	{
		if (owner_) {
			owner_->onInviteBtnClicked(bHandled);
		}
	}
	virtual void onStartShareBtnClicked()
	{
		if (owner_)
		{
			owner_->onStartShareBtnClicked();
		}
	}
	virtual void onEndMeetingBtnClicked()
	{
		if (owner_)
		{
			owner_->onEndMeetingBtnClicked();
		}
	}
	virtual void onParticipantListBtnClicked()
	{
		if (owner_)
		{
			owner_->onParticipantListBtnClicked();
		}
	}
	virtual void onCustomLiveStreamMenuClicked()
	{
		if (owner_)
		{
			owner_->onCustomLiveStreamMenuClicked();
		}
	}
	virtual void onZoomInviteDialogFailed()
	{
		if (owner_)
		{
			owner_->onZoomInviteDialogFailed();
		}
	}
	virtual void onCCBTNClicked()
	{
		if (owner_)
		{
			owner_->onCCBTNClicked();
		}
	}
	virtual void onAudioBtnClicked(ZOOM_SDK_NAMESPACE::AudioBtnClickedCallbackInfo info)
	{
		if (owner_)
		{
			ZNAudioBtnClickedCallbackInfo zn_info;
			zn_info.userid_MuteUnmute = info.userid_MuteUnmute;
			zn_info.audio_clicked_action = Map2WrapDefine(info.audio_clicked_action);
			owner_->onAudioBtnClicked(zn_info);
		}
	}
	virtual void onAudioMenuBtnClicked()
	{
		if (owner_)
		{
			owner_->onAudioMenuBtnClicked();
		}
	}
	virtual void onBreakoutRoomBtnClicked()
	{
		if (owner_)
		{
			//fix me
		}
	}
private:
	ZMeetingUICtrlWrap* owner_;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZMeetingAudioCtrlWrapEvent : public ZOOM_SDK_NAMESPACE::IMeetingAudioCtrlEvent
{
public:
	void SetOwner(ZMeetingAudioWrap* obj) { owner_ = obj; }
	virtual void onUserAudioStatusChange(ZOOM_SDK_NAMESPACE::IList<ZOOM_SDK_NAMESPACE::IUserAudioStatus* >* lstAudioStatusChange, const wchar_t* strAudioStatusList)
	{
		if (owner_) {
			if (NULL == strAudioStatusList && lstAudioStatusChange && lstAudioStatusChange->GetCount() > 0)
			{
				//
				ZNList<ZNUserAudioStatus> audio_status_list;
				ZoomSTRING zn_strAudioStatusList = L"";
				for (int i = 0; i < lstAudioStatusChange->GetCount(); ++i)
				{
					ZOOM_SDK_NAMESPACE::IUserAudioStatus* pAudioStatus = lstAudioStatusChange->GetItem(i);
					ZNUserAudioStatus userAudioStatus;
					if (pAudioStatus)
					{
						userAudioStatus.userId = pAudioStatus->GetUserId();
						userAudioStatus.audioStauts = Map2WrapDefine(pAudioStatus->GetStatus());
					}

					audio_status_list.push_back(userAudioStatus);
				}
				owner_->onUserAudioStatusChange(audio_status_list, zn_strAudioStatusList);
			}
		}
	}
	virtual void onUserActiveAudioChange(ZOOM_SDK_NAMESPACE::IList<unsigned int >* plstActiveAudio)
	{
		if (owner_) {
			if (plstActiveAudio && plstActiveAudio->GetCount() > 0)
			{
				//
				ZNList<unsigned int> active_audio_list;
				for (int i = 0; i < plstActiveAudio->GetCount(); ++i)
				{
					unsigned int userid = plstActiveAudio->GetItem(i);
					active_audio_list.push_back(userid);
				}
				owner_->onUserActiveAudioChange(active_audio_list);
			}
		}
	}
	virtual void onHostRequestStartAudio(ZOOM_SDK_NAMESPACE::IRequestStartAudioHandler* handler_)
	{

	}
	virtual void onJoin3rdPartyTelephonyAudio(const zchar_t* audioInfo)
	{

	}
	virtual void onMuteOnEntryStatusChange(bool bEnabled)
	{
		if (owner_)
		{
			owner_->onMuteOnEntryStatusChange(bEnabled);
		}
	}
private:
	ZMeetingAudioWrap* owner_;
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZMeetingVideoCtrlWrapEvent : public ZOOM_SDK_NAMESPACE::IMeetingVideoCtrlEvent
{
public:
	void SetOwner(ZMeetingVideoWrap* obj) { owner_ = obj; }
	virtual void onUserVideoStatusChange(unsigned int userId, ZOOM_SDK_NAMESPACE::VideoStatus status)
	{
		if (owner_) {

			owner_->onUserVideoStatusChange(userId, Map2WrapDefine(status));
		}
	}
	virtual void onSpotlightVideoChangeNotification(bool bSpotlight, unsigned int userid)
	{
		//
	}
	virtual void onSpotlightedUserListChangeNotification(ZOOM_SDK_NAMESPACE::IList<unsigned int >* lstSpotlightedUserID)
	{
		//
	}
	virtual void onHostRequestStartVideo(ZOOM_SDK_NAMESPACE::IRequestStartVideoHandler* handler_)
	{

	}
	virtual void onActiveSpeakerVideoUserChanged(unsigned int userid)
	{
		if (owner_) {
			owner_->onActiveSpeakerVideoUserChanged(userid);
		}
	}
	virtual void onActiveVideoUserChanged(unsigned int userid)
	{
		if (owner_) {
			owner_->onActiveVideoUserChanged(userid);
		}
	}
	virtual void onHostVideoOrderUpdated(ZOOM_SDK_NAMESPACE::IList<unsigned int >* orderList)
	{

	}
	virtual void onFollowHostVideoOrderChanged(bool bFollow)
	{

	}
	virtual void onLocalVideoOrderUpdated(ZOOM_SDK_NAMESPACE::IList<unsigned int >* localOrderList)
	{
		
	}
	virtual void onUserVideoQualityChanged(ZOOM_SDK_NAMESPACE::VideoConnectionQuality quality, unsigned int userid)
	{
		
	}
	virtual void onVideoAlphaChannelStatusChanged(bool isAlphaModeOn)
	{

	}
	virtual void onCameraControlRequestReceived(unsigned int userId, ZOOM_SDK_NAMESPACE::CameraControlRequestType requestType, ZOOM_SDK_NAMESPACE::ICameraControlRequestHandler* pHandler)
	{

	}
	virtual void onCameraControlRequestResult(unsigned int userId, ZOOM_SDK_NAMESPACE::CameraControlRequestResult result)
	{

	}
private:
	ZMeetingVideoWrap* owner_;
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZRequestStartCloudRecordingHandler
{
public:
	static ZRequestStartCloudRecordingHandler& GetInst()
	{
		static ZRequestStartCloudRecordingHandler inst;
		return inst;
	}
	void SetHandler(ZOOM_SDK_NAMESPACE::IRequestStartCloudRecordingHandler *handler)
	{
		m_pHandler = handler;
	}

	unsigned int GetRequesterId()
	{
		unsigned int zn_id;
		if (m_pHandler)
		{
			zn_id = m_pHandler->GetRequesterId();
		}
		return zn_id;
	}

	ZoomSTRING GetRequesterName()
	{
		ZoomSTRING zn_name;
		if (m_pHandler)
		{
			const zchar_t* name = m_pHandler->GetRequesterName();
			if (name)
			{
				zn_name = name;
			}
		}
		return zn_name;
	}

	ZNSDKError Start()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_WRONG_USAGE;
		if (m_pHandler)
		{
			err = m_pHandler->Start();
		}
		m_pHandler = NULL;
		return Map2WrapDefine(err);
	}

	ZNSDKError Deny(bool bDenyAll)
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_WRONG_USAGE;
		if (m_pHandler)
		{
			err = m_pHandler->Deny(bDenyAll);
		}
		m_pHandler = NULL;
		return Map2WrapDefine(err);
	}
private:
	ZRequestStartCloudRecordingHandler() :m_pHandler(NULL) {}
	ZOOM_SDK_NAMESPACE::IRequestStartCloudRecordingHandler* m_pHandler;
};

class ZRequestEnableAndStartSmartRecordingHandler
{
public:
	static ZRequestEnableAndStartSmartRecordingHandler& GetInst()
	{
		static ZRequestEnableAndStartSmartRecordingHandler inst;
		return inst;
	}
	void SetHandler(ZOOM_SDK_NAMESPACE::IRequestEnableAndStartSmartRecordingHandler *handler)
	{
		m_pHandler = handler;
	}

	unsigned int GetRequestEnableAndStartSmartRecordingUserId()
	{
		unsigned int zn_user_id;
		if (m_pHandler)
		{
			zn_user_id = m_pHandler->GetRequestUserId();
		}
		return zn_user_id;
	}

	ZoomSTRING GetRequestEnableAndStartSmartRecordingTipString()
	{
		ZoomSTRING zn_tip;
		if (m_pHandler)
		{
			const zchar_t* tip = m_pHandler->GetTipString();
			if (tip)
			{
				zn_tip = tip;
			}
		}
		return zn_tip;
	}

	ZNSDKError StartCloudRecordingWithoutEnableSmartRecording()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_WRONG_USAGE;
		if (m_pHandler)
		{
			err = m_pHandler->StartCloudRecordingWithoutEnableSmartRecording();
		}
		m_pHandler = NULL;
		return Map2WrapDefine(err);
	}

	ZNSDKError AgreeToEnableAndStartSmartRecording(bool bAllMeetings)
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_WRONG_USAGE;
		if (m_pHandler)
		{
			err = m_pHandler->AgreeToEnableAndStart(bAllMeetings);
		}
		m_pHandler = NULL;
		return Map2WrapDefine(err);
	}

	ZNSDKError DeclineEnableAndStartSmartRecording(bool bDenyAll)
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_WRONG_USAGE;
		if (m_pHandler)
		{
			err = m_pHandler->Decline(bDenyAll);
		}
		m_pHandler = NULL;
		return Map2WrapDefine(err);
	}
private:
	ZRequestEnableAndStartSmartRecordingHandler() :m_pHandler(NULL) {}
	ZOOM_SDK_NAMESPACE::IRequestEnableAndStartSmartRecordingHandler* m_pHandler;
};

class ZSmartRecordingEnableActionHandler
{
public:
	static ZSmartRecordingEnableActionHandler& GetInst()
	{
		static ZSmartRecordingEnableActionHandler inst;
		return inst;
	}
	void SetHandler(ZOOM_SDK_NAMESPACE::ISmartRecordingEnableActionHandler *handler)
	{
		m_pHandler = handler;
	}

	ZoomSTRING GetSmartRecordingEnableActionTipString()
	{
		ZoomSTRING zn_tip;
		if (m_pHandler)
		{
			const zchar_t* tip = m_pHandler->GetTipString();
			if (tip)
			{
				zn_tip = tip;
			}
		}
		return zn_tip;
	}

	ZNSDKError ActionConfirmEnableSmartRecording(bool bAllMeetings)
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_WRONG_USAGE;
		if (m_pHandler)
		{
			err = m_pHandler->ActionConfirm(bAllMeetings);
		}
		m_pHandler = NULL;
		return Map2WrapDefine(err);
	}

	ZNSDKError ActionCancelEnableSmartRecording()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_WRONG_USAGE;
		if (m_pHandler)
		{
			err = m_pHandler->ActionCancel();
		}
		m_pHandler = NULL;
		return Map2WrapDefine(err);
	}
private:
	ZSmartRecordingEnableActionHandler() :m_pHandler(NULL) {}
	ZOOM_SDK_NAMESPACE::ISmartRecordingEnableActionHandler* m_pHandler;
};

class ZMeetingRecordingCtrlWrapEvent : public ZOOM_SDK_NAMESPACE::IMeetingRecordingCtrlEvent
{
public:
	void SetOwner(ZMeetingRecordingWrap* obj) { owner_ = obj; }
	virtual void onRecording2MP4Done(bool bsuccess, int iResult, const wchar_t* szPath)
	{
		if (owner_)
		{
			ZoomSTRING zn_szPath;
			if (szPath)
			{
				zn_szPath = szPath;
			}
			owner_->onRecording2MP4Done(bsuccess, iResult, zn_szPath);
		}
	}
	virtual void onRecording2MP4Processing(int iPercentage)
	{
		if (owner_)
		{
			owner_->onRecording2MP4Processing(iPercentage);
		}
	}
	virtual void onRecordingStatus(ZOOM_SDK_NAMESPACE::RecordingStatus status)
	{
		if (owner_)
		{
			owner_->onRecordingStatus(Map2WrapDefine(status));
		}
	}
	virtual void onCloudRecordingStatus(ZOOM_SDK_NAMESPACE::RecordingStatus status)
	{
		if (owner_)
		{
			owner_->onCloudRecordingStatus(Map2WrapDefine(status));
		}
	}
	virtual void onRecordPrivilegeChanged(bool bCanRec)
	{
		if (owner_)
		{
			owner_->onRecordPrivilegeChanged(bCanRec);
		}
	}
	virtual void onCustomizedLocalRecordingSourceNotification(ZOOM_SDK_NAMESPACE::ICustomizedLocalRecordingLayoutHelper* layout_helper)
	{
		
	}
	virtual void onLocalRecordingPrivilegeRequestStatus(ZOOM_SDK_NAMESPACE::RequestLocalRecordingStatus status)
	{
		
	}
	virtual void onRequestCloudRecordingResponse(ZOOM_SDK_NAMESPACE::RequestStartCloudRecordingStatus status)
	{
		if (owner_)
		{
			owner_->onRequestCloudRecordingResponse(Map2WrapDefine(status));
		}
	}
	virtual void onStartCloudRecordingRequested(ZOOM_SDK_NAMESPACE::IRequestStartCloudRecordingHandler* handler)
	{
		if (owner_)
		{
			ZRequestStartCloudRecordingHandler::GetInst().SetHandler(handler);
			owner_->onStartCloudRecordingRequested();
		}
	}
	virtual void onEnableAndStartSmartRecordingRequested(ZOOM_SDK_NAMESPACE::IRequestEnableAndStartSmartRecordingHandler* handler)
	{
		if (owner_)
		{
			ZRequestEnableAndStartSmartRecordingHandler::GetInst().SetHandler(handler);
			owner_->onEnableAndStartSmartRecordingRequested();
		}
	}
	virtual void onSmartRecordingEnableActionCallback(ZOOM_SDK_NAMESPACE::ISmartRecordingEnableActionHandler* handler)
	{
		if (owner_)
		{
			ZSmartRecordingEnableActionHandler::GetInst().SetHandler(handler);
			owner_->onSmartRecordingEnableActionCallback();
		}
	}
	virtual void onLocalRecordingPrivilegeRequested(ZOOM_SDK_NAMESPACE::IRequestLocalRecordingPrivilegeHandler* handler)
	{
		
	}
	virtual void onCloudRecordingStorageFull(time_t gracePeriodDate)
	{
		if (owner_)
		{
			owner_->onCloudRecordingStorageFull(gracePeriodDate);
		}
	}
private:
	ZMeetingRecordingWrap* owner_;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZMeetingShareCtrlWrapEvent : public ZOOM_SDK_NAMESPACE::IMeetingShareCtrlEvent
{
public:
	void SetOwner(ZMeetingShareWrap* obj) { owner_ = obj; }
	virtual void onSharingStatus(ZOOM_SDK_NAMESPACE::ZoomSDKSharingSourceInfo shareInfo)
	{
		if (owner_) {
			ZNZoomSDKSharingSourceInfo zn_shareInfo = ConvertSDKSharingSourceInfoToZNSharingSourceInfo(shareInfo);
			owner_->onSharingStatus(zn_shareInfo);
		}
	}
	virtual void onFailedToStartShare()
	{
		if (owner_) {
			owner_->onFailedToStartShare();
		}
	}
	virtual void onLockShareStatus(bool bLocked)
	{

	}
	virtual void onShareContentNotification(ZOOM_SDK_NAMESPACE::ZoomSDKSharingSourceInfo shareInfo)
	{

	}
	virtual void onMultiShareSwitchToSingleShareNeedConfirm(ZOOM_SDK_NAMESPACE::IShareSwitchMultiToSingleConfirmHandler* handler_)
	{

	}
	virtual void onShareSettingTypeChangedNotification(ZOOM_SDK_NAMESPACE::ShareSettingType type)
	{

	}
	virtual void onSharedVideoEnded()
	{

	}
	virtual void onVideoFileSharePlayError(ZOOM_SDK_NAMESPACE::ZoomSDKVideoFileSharePlayError error)
	{

	}
	virtual void onOptimizingShareForVideoClipStatusChanged(ZOOM_SDK_NAMESPACE::ZoomSDKSharingSourceInfo shareInfo)
	{
		if (owner_) {
			ZNZoomSDKSharingSourceInfo zn_shareInfo = ConvertSDKSharingSourceInfoToZNSharingSourceInfo(shareInfo);
			owner_->onOptimizingShareForVideoClipStatusChanged(zn_shareInfo);
		}
	}
private:
	ZMeetingShareWrap* owner_;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZMeetingAnnotationCtrlWrapEvent : public ZOOM_SDK_NAMESPACE::IMeetingAnnotationSupportEvent
{
public:
	void SetOwner(ZMeetingAnnotationWrap* obj) { owner_ = obj; }
	virtual void onSupportAnnotationStatus(unsigned int nShareSourceID, bool bSupportAnnotation)
	{
		if (owner_) {
			owner_->onSupportAnnotationStatus(nShareSourceID, bSupportAnnotation);
		}
	}
private:
	ZMeetingAnnotationWrap* owner_;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZMeetingParticipantsCtrlEvent : public ZOOM_SDK_NAMESPACE::IMeetingParticipantsCtrlEvent
{
public:
	void SetOwner(ZMeetingParticipantsWrap* obj) { owner_ = obj; }
	virtual void onUserJoin(ZOOM_SDK_NAMESPACE::IList<unsigned int >* lstUserID, const wchar_t* strUserList)
	{
		if (owner_ && lstUserID) {
			ZNList<unsigned int> userId_list;
			ZoomSTRING zn_strUserList = L"";
			if (strUserList)
				zn_strUserList = strUserList;
			for (int i = 0; i < lstUserID->GetCount(); ++i)
			{

				unsigned int zn_userid;

				zn_userid = lstUserID->GetItem(i);

				userId_list.push_back(zn_userid);
			}

			owner_->onUserJoin(userId_list, zn_strUserList);

		}
	}
	virtual void onUserLeft(ZOOM_SDK_NAMESPACE::IList<unsigned int >* lstUserID, const wchar_t* strUserList)
	{
		if (owner_ && lstUserID) {
			ZNList<unsigned int> userId_list;
			ZoomSTRING zn_strUserList = L"";
			if (strUserList)
				zn_strUserList = strUserList;
			for (int i = 0; i < lstUserID->GetCount(); ++i)
			{

				unsigned int zn_userid;

				zn_userid = lstUserID->GetItem(i);

				userId_list.push_back(zn_userid);
			}

			owner_->onUserLeft(userId_list, zn_strUserList);

		}
	}
	virtual void onHostChangeNotification(unsigned int userId)
	{
		if (owner_) {
			owner_->onHostChangeNotification(userId);
		}
	}
	virtual void onLowOrRaiseHandStatusChanged(bool bLow, unsigned int userid)
	{

	}
	virtual void onCoHostChangeNotification(unsigned int userId, bool isCoHost)
	{

	}
	virtual void onInvalidReclaimHostkey()
	{

	}
	virtual void onUserNamesChanged(ZOOM_SDK_NAMESPACE::IList<unsigned int>* lstUserID)
	{

	}
	virtual void onAllHandsLowered()
	{

	}
	virtual void onLocalRecordingStatusChanged(unsigned int user_id, ZOOM_SDK_NAMESPACE::RecordingStatus status)
	{

	}
	virtual void onAllowParticipantsRenameNotification(bool bAllow)
	{

	}
	virtual void onAllowParticipantsUnmuteSelfNotification(bool bAllow)
	{

	}
	virtual void onAllowParticipantsStartVideoNotification(bool bAllow)
	{

	}
	virtual void onAllowParticipantsShareWhiteBoardNotification(bool bAllow)
	{

	}
	virtual void onRequestLocalRecordingPrivilegeChanged(ZOOM_SDK_NAMESPACE::LocalRecordingRequestPrivilegeStatus status)
	{

	}
	virtual void onAllowParticipantsRequestCloudRecording(bool bAllow)
	{
		if (owner_)
		{
			owner_->onAllowParticipantsRequestCloudRecording(bAllow);
		}
	}
	virtual void onInMeetingUserAvatarPathUpdated(unsigned int userID)
	{

	}
	virtual void onParticipantProfilePictureStatusChange(bool bHidden)
	{
		
	}
	virtual void onFocusModeStateChanged(bool bEnabled)
	{

	}
	virtual void onFocusModeShareTypeChanged(ZOOM_SDK_NAMESPACE::FocusModeShareType type)
	{

	}
	virtual void onBotAuthorizerRelationChanged(unsigned int authorizeUserID)
	{
		if (owner_) {
			owner_->onBotAuthorizerRelationChanged(authorizeUserID);
		}
	}
	virtual void onVirtualNameTagStatusChanged(bool bOn, unsigned int userID)
	{

	}
	virtual void onVirtualNameTagRosterInfoUpdated(unsigned int userID)
	{

	}
	virtual void onCreateCompanionRelation(unsigned int parentUserID, unsigned int childUserID)
	{

	}
	virtual void onRemoveCompanionRelation(unsigned int childUserID)
	{

	}
	virtual void onGrantCoOwnerPrivilegeChanged(bool canGrantOther)
	{

	}
private:
	ZMeetingParticipantsWrap* owner_;
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZMeetingH323CtrlWrapEvent : public ZOOM_SDK_NAMESPACE::IMeetingH323HelperEvent
{
public:
	void SetOwner(ZMeetingH323Wrap* obj) { owner_ = obj; }
	virtual void onCalloutStatusNotify(ZOOM_SDK_NAMESPACE::H323CalloutStatus status)
	{
		if (owner_) {
			owner_->onH323CalloutStatusNotify(Map2WrapDefine(status));
		}
	}
	virtual void onParingH323Result(ZOOM_SDK_NAMESPACE::H323ParingResult result, UINT64 meetingNumber)
	{
		//
	}
private:
	ZMeetingH323Wrap* owner_;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZMeetingPasswordAndScreenNameHandler
{
public:
	static ZMeetingPasswordAndScreenNameHandler& GetInst()
	{
		static ZMeetingPasswordAndScreenNameHandler inst;
		return inst;
	}
	void SetHandler(ZOOM_SDK_NAMESPACE::IMeetingPasswordAndScreenNameHandler *handler)
	{
		m_pHandler = handler;
	}

	ZNRequiredInfoType GetRequiredInfoType()
	{
		ZOOM_SDK_NAMESPACE::IMeetingPasswordAndScreenNameHandler::RequiredInfoType type = ZOOM_SDK_NAMESPACE::IMeetingPasswordAndScreenNameHandler::REQUIRED_INFO_TYPE_NONE;
		if (m_pHandler)
			type = m_pHandler->GetRequiredInfoType();
		return Map2WrapDefine(type);
	}

	bool InputMeetingPasswordAndScreenName(ZoomSTRING meetingPassword, ZoomSTRING screenName)
	{
		bool bret = false;
		if (m_pHandler)
			bret = m_pHandler->InputMeetingPasswordAndScreenName(meetingPassword.c_str(), screenName.c_str());
		m_pHandler = NULL;
		return bret;
	}
	bool InputMeetingIDAndScreenName(ZoomSTRING meetingID, ZoomSTRING screenName)
	{
		bool bret = false;
		if (m_pHandler)
			bret = m_pHandler->InputMeetingPasswordAndScreenName(meetingID.c_str(), screenName.c_str());
		m_pHandler = NULL;
		return bret;
	}
	bool InputMeetingScreenName(ZoomSTRING screenName)
	{
		bool bret = false;
		if (m_pHandler)
			bret = m_pHandler->InputMeetingScreenName(screenName.c_str());
		m_pHandler = NULL;
		return bret;
	}
	void Cancel()
	{
		if (m_pHandler)
			m_pHandler->Cancel();
		m_pHandler = NULL;
	}
private:
	ZMeetingPasswordAndScreenNameHandler() :m_pHandler(NULL) {}
	ZOOM_SDK_NAMESPACE::IMeetingPasswordAndScreenNameHandler* m_pHandler;
};
class ZWebinarNeedRegisterHandler
{
public:
	static ZWebinarNeedRegisterHandler& GetInst()
	{
		static ZWebinarNeedRegisterHandler inst;
		return inst;
	}
	void SetHandler(ZOOM_SDK_NAMESPACE::IWebinarNeedRegisterHandler *handler)
	{
		m_pHandler = handler;
	}
	ZNWebinarNeedRegisterType GetWebinarNeedRegisterType()
	{
		ZOOM_SDK_NAMESPACE::IWebinarNeedRegisterHandler::WebinarNeedRegisterType type = ZOOM_SDK_NAMESPACE::IWebinarNeedRegisterHandler::WebinarReg_NONE;
		if (m_pHandler)
			type = m_pHandler->GetWebinarNeedRegisterType();
		return Map2WrapDefine(type);
	}
	ZoomSTRING GetWebinarRegisterUrl()
	{
		ZoomSTRING zn_url;
		if (m_pHandler)
		{
			ZOOM_SDK_NAMESPACE::IWebinarNeedRegisterHandlerByUrl* m_pURL = (ZOOM_SDK_NAMESPACE::IWebinarNeedRegisterHandlerByUrl*)m_pHandler;
			zn_url = m_pURL->GetWebinarRegisterUrl();
		}
		m_pHandler = NULL;
		return zn_url;
	}
	void Release()
	{
		if (m_pHandler)
		{
			ZOOM_SDK_NAMESPACE::IWebinarNeedRegisterHandlerByUrl* m_pURL = (ZOOM_SDK_NAMESPACE::IWebinarNeedRegisterHandlerByUrl*)m_pHandler;
			m_pURL->Release();
		}
		m_pHandler = NULL;
	}
	ZNSDKError InputWebinarRegisterEmailAndScreenName(ZoomSTRING email, ZoomSTRING screenName)
	{
		ZNSDKError zn_err = ZNSDKERR_UNKNOWN;
		if (m_pHandler)
		{
			ZOOM_SDK_NAMESPACE::IWebinarNeedRegisterHandlerByEmail* m_pURL = (ZOOM_SDK_NAMESPACE::IWebinarNeedRegisterHandlerByEmail*)m_pHandler;
			zn_err = Map2WrapDefine(m_pURL->InputWebinarRegisterEmailAndScreenName(email.c_str(), screenName.c_str()));
		}
		m_pHandler = NULL;
		return zn_err;
	}
	void Cancel()
	{
		if (m_pHandler)
		{
			ZOOM_SDK_NAMESPACE::IWebinarNeedRegisterHandlerByEmail* m_pURL = (ZOOM_SDK_NAMESPACE::IWebinarNeedRegisterHandlerByEmail*)m_pHandler;
			m_pURL->Cancel();
		}
		m_pHandler = NULL;
	}
private:
	ZWebinarNeedRegisterHandler() :m_pHandler(NULL) {}
	ZOOM_SDK_NAMESPACE::IWebinarNeedRegisterHandler* m_pHandler;
};
class ZEndOtherMeetingToJoinMeetingHandler
{
public:
	static ZEndOtherMeetingToJoinMeetingHandler& GetInst()
	{
		static ZEndOtherMeetingToJoinMeetingHandler inst;
		return inst;
	}
	void SetHandler(ZOOM_SDK_NAMESPACE::IEndOtherMeetingToJoinMeetingHandler *handler)
	{
		m_pHandler = handler;
	}
	bool EndOtherMeeting()
	{
		bool bret = false;
		if (m_pHandler)
		{
			bret = m_pHandler->EndOtherMeeting();
		}
		m_pHandler = NULL;
		return bret;
	}
	void Cancel()
	{
		if (m_pHandler)
		{
			m_pHandler->Cancel();
		}
		m_pHandler = NULL;
	}
private:
	ZEndOtherMeetingToJoinMeetingHandler() :m_pHandler(NULL) {}
	ZOOM_SDK_NAMESPACE::IEndOtherMeetingToJoinMeetingHandler* m_pHandler;
};
class ZWebinarInputScreenNameHandler
{
public:
	static ZWebinarInputScreenNameHandler& GetInst()
	{
		static ZWebinarInputScreenNameHandler inst;
		return inst;
	}
	void SetHandler(ZOOM_SDK_NAMESPACE::IWebinarInputScreenNameHandler *handler)
	{
		m_pHandler = handler;
	}
	ZNSDKError InputName(ZoomSTRING screenName)
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_WRONG_USAGE;
		if (m_pHandler)
		{
			err = m_pHandler->InputName(screenName.c_str());
		}
		m_pHandler = NULL;
		return Map2WrapDefine(err);
	}
	ZNSDKError Cancel()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_WRONG_USAGE;
		if (m_pHandler)
		{
			err = m_pHandler->Cancel();
		}
		m_pHandler = NULL;
		return Map2WrapDefine(err);
	}
private:
	ZWebinarInputScreenNameHandler() :m_pHandler(NULL) {}
	ZOOM_SDK_NAMESPACE::IWebinarInputScreenNameHandler* m_pHandler;
};
class ZMeetingInputUserInfoHandler
{
public:
	static ZMeetingInputUserInfoHandler& GetInst()
	{
		static ZMeetingInputUserInfoHandler inst;
		return inst;
	}
	void SetHandler(ZOOM_SDK_NAMESPACE::IMeetingInputUserInfoHandler *handler)
	{
		m_pHandler = handler;
	}

	ZoomSTRING GetDefaultDisplayName()
	{
		ZoomSTRING zn_display_name;
		if (m_pHandler)
		{
			const zchar_t* display_name = m_pHandler->GetDefaultDisplayName();
			if (display_name)
			{
				zn_display_name = display_name;
			}
		}
		return zn_display_name;
	}

	bool CanModifyDefaultDisplayName()
	{
		bool bret = false;
		if (m_pHandler)
			bret = m_pHandler->CanModifyDefaultDisplayName();
		return bret;
	}

	bool IsValidEmail(ZoomSTRING email)
	{
		bool bret = false;
		if (m_pHandler)
			bret = m_pHandler->IsValidEmail(email.c_str());
		return bret;
	}

	ZNSDKError InputUserInfo(ZoomSTRING name, ZoomSTRING email)
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_WRONG_USAGE;
		if (m_pHandler)
		{
			err = m_pHandler->InputUserInfo(name.c_str(), email.c_str());
		}
		m_pHandler = NULL;
		return Map2WrapDefine(err);
	}

	ZNSDKError CancelInputUserInfo()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_WRONG_USAGE;
		if (m_pHandler)
		{
			m_pHandler->Cancel();
			err = ZOOM_SDK_NAMESPACE::SDKERR_SUCCESS;
		}
		m_pHandler = NULL;
		return Map2WrapDefine(err);
	}
private:
	ZMeetingInputUserInfoHandler() :m_pHandler(NULL) {}
	ZOOM_SDK_NAMESPACE::IMeetingInputUserInfoHandler* m_pHandler;
};
class ZMeetingArchiveConfirmHandler
{
public:
	static ZMeetingArchiveConfirmHandler& GetInst()
	{
		static ZMeetingArchiveConfirmHandler inst;
		return inst;
	}
	void SetHandler(ZOOM_SDK_NAMESPACE::IMeetingArchiveConfirmHandler *handler)
	{
		m_pHandler = handler;
	}

	ZoomSTRING GetArchiveConfirmContent()
	{
		ZoomSTRING zn_content;
		if (m_pHandler)
		{
			const zchar_t* content = m_pHandler->GetArchiveConfirmContent();
			if (content)
			{
				zn_content = content;
			}
		}
		return zn_content;
	}

	ZNSDKError JoinWithArchive(bool bStartArchive)
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_WRONG_USAGE;
		if (m_pHandler)
		{
			err = m_pHandler->JoinWithArchive(bStartArchive);
		}
		m_pHandler = NULL;
		return Map2WrapDefine(err);
	}
private:
	ZMeetingArchiveConfirmHandler() :m_pHandler(NULL) {}
	ZOOM_SDK_NAMESPACE::IMeetingArchiveConfirmHandler* m_pHandler;
};

class ZMeetingConfigCtrlWrapFreeMeetingEvent : public ZOOM_SDK_NAMESPACE::IMeetingConfigurationEvent
{
public:
	void SetOwner(ZMeetingConfigWrap* obj) { owner_ = obj; }
	virtual void onFreeMeetingRemainTime(unsigned int leftTime)
	{
		if (owner_) {
			owner_->onFreeMeetingRemainTime(leftTime);
		}
	}
	virtual void onFreeMeetingRemainTimeStopCountDown()
	{
		if (owner_) {
			owner_->onFreeMeetingRemainTimeStopCountDown();
		}
	}
	virtual void onFreeMeetingNeedToUpgrade(ZOOM_SDK_NAMESPACE::IMeetingConfigurationFreeMeetingEvent::FreeMeetingNeedUpgradeType type_, const wchar_t* gift_url)
	{
		if (owner_) {
			ZoomSTRING zn_gift_url = L"";
			if (gift_url)
			{
				zn_gift_url = gift_url;
			}
			owner_->onFreeMeetingNeedToUpgrade(Map2WrapDefine(type_), zn_gift_url);
		}
	}
	virtual void onFreeMeetingUpgradeToGiftFreeTrialStart()
	{
		if (owner_) {
			owner_->onFreeMeetingUpgradeToGiftFreeTrialStart();
		}
	}
	virtual void onFreeMeetingUpgradeToGiftFreeTrialStop()
	{
		if (owner_) {
			owner_->onFreeMeetingUpgradeToGiftFreeTrialStop();
		}
	}
	virtual void onFreeMeetingUpgradeToProMeeting()
	{
		if (owner_) {
			owner_->onFreeMeetingUpgradeToProMeeting();
		}
	}
	virtual void onInputMeetingPasswordAndScreenNameNotification(ZOOM_SDK_NAMESPACE::IMeetingPasswordAndScreenNameHandler* pHandler)
	{
		if (owner_ && pHandler) {
			ZMeetingPasswordAndScreenNameHandler::GetInst().SetHandler(pHandler);
			owner_->onInputMeetingPasswordAndScreenNameNotification();
		}
	}
	virtual void onAirPlayInstructionWndNotification(bool bShow, const wchar_t* airhostName)
	{
		if (owner_) {
			ZoomSTRING zn_airHostName = L"";
			if (airhostName)
			{
				zn_airHostName = airhostName;
			}
			owner_->onAirPlayInstructionWndNotification(bShow, zn_airHostName);
		}
	}
	virtual void onWebinarNeedRegisterNotification(ZOOM_SDK_NAMESPACE::IWebinarNeedRegisterHandler* pHandler)
	{
		if (owner_ && pHandler) {
			ZWebinarNeedRegisterHandler::GetInst().SetHandler(pHandler);
			owner_->onWebinarNeedRegisterNotification();
		}
	}
	virtual void onEndOtherMeetingToJoinMeetingNotification(ZOOM_SDK_NAMESPACE::IEndOtherMeetingToJoinMeetingHandler* pHandler)
	{
		if (owner_ && pHandler) {
			ZEndOtherMeetingToJoinMeetingHandler::GetInst().SetHandler(pHandler);
			owner_->onEndOtherMeetingToJoinMeetingNotification();
		}
	}
	virtual void onWebinarNeedInputScreenName(ZOOM_SDK_NAMESPACE::IWebinarInputScreenNameHandler* pHandler)
	{
		if (owner_ && pHandler) {
			ZWebinarInputScreenNameHandler::GetInst().SetHandler(pHandler);
			owner_->onWebinarNeedInputScreenName();
		}
	}
	virtual void onJoinMeetingNeedUserInfo(ZOOM_SDK_NAMESPACE::IMeetingInputUserInfoHandler* pHandler)
	{
		if (owner_ && pHandler) {
			ZMeetingInputUserInfoHandler::GetInst().SetHandler(pHandler);
			owner_->onJoinMeetingNeedUserInfo();
		}
	}
	virtual void onUserConfirmToStartArchive(ZOOM_SDK_NAMESPACE::IMeetingArchiveConfirmHandler* pHandler)
	{
		if (owner_ && pHandler) {
			ZMeetingArchiveConfirmHandler::GetInst().SetHandler(pHandler);
			owner_->onUserConfirmToStartArchive();
		}
	}
	virtual void onUserConfirmRecoverMeeting(ZOOM_SDK_NAMESPACE::IMeetingConfirmRecoverHandler* handler)
	{

	}
private:
	ZMeetingConfigWrap* owner_;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZMeetingReminderHandler
{
public:
	static ZMeetingReminderHandler& GetInst()
	{
		static ZMeetingReminderHandler inst;
		return inst;
	}
	void SetHandler(ZOOM_SDK_NAMESPACE::IMeetingReminderHandler *handler)
	{
		m_pHandler = handler;
	}

	ZNSDKError Ignore()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_WRONG_USAGE;
		if (m_pHandler)
		{
			m_pHandler->Ignore();
			err = ZOOM_SDK_NAMESPACE::SDKERR_SUCCESS;
		}
		m_pHandler = NULL;
		return Map2WrapDefine(err);
	}

	ZNSDKError Accept()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_WRONG_USAGE;
		if (m_pHandler)
		{
			m_pHandler->Accept();
			err = ZOOM_SDK_NAMESPACE::SDKERR_SUCCESS;
		}
		m_pHandler = NULL;
		return Map2WrapDefine(err);
	}

	ZNSDKError Decline()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_WRONG_USAGE;
		if (m_pHandler)
		{
			m_pHandler->Decline();
			err = ZOOM_SDK_NAMESPACE::SDKERR_SUCCESS;
		}
		m_pHandler = NULL;
		return Map2WrapDefine(err);
	}
private:
	ZMeetingReminderHandler() :m_pHandler(NULL) {}
	ZOOM_SDK_NAMESPACE::IMeetingReminderHandler* m_pHandler;
};

class ZMeetingEnableReminderHandler
{
public:
	static ZMeetingEnableReminderHandler& GetInst()
	{
		static ZMeetingEnableReminderHandler inst;
		return inst;
	}
	void SetHandler(ZOOM_SDK_NAMESPACE::IMeetingEnableReminderHandler *handler)
	{
		m_pHandler = handler;
	}

	ZNSDKError SetEnableOption(ZNFeatureEnableOption option)
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_WRONG_USAGE;
		if (m_pHandler)
		{
			err = m_pHandler->SetEnableOption(Map2WrapDefine(option));
		}
		return Map2WrapDefine(err);
	}

	ZNSDKError Start()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_WRONG_USAGE;
		if (m_pHandler)
		{
			err = m_pHandler->Start();
		}
		m_pHandler = NULL;
		return Map2WrapDefine(err);
	}

	ZNSDKError Decline(bool bDeclineAll)
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_WRONG_USAGE;
		if (m_pHandler)
		{
			err = m_pHandler->Decline(bDeclineAll);
		}
		m_pHandler = NULL;
		return Map2WrapDefine(err);
	}

	ZNSDKError Ignore()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_WRONG_USAGE;
		if (m_pHandler)
		{
			err = m_pHandler->Ignore();
		}
		m_pHandler = NULL;
		return Map2WrapDefine(err);
	}
private:
	ZMeetingEnableReminderHandler() :m_pHandler(NULL) {}
	ZOOM_SDK_NAMESPACE::IMeetingEnableReminderHandler* m_pHandler;
};

class ZMeetingReminderCtrlEvent : public ZOOM_SDK_NAMESPACE::IMeetingReminderEvent
{
public:
	void SetOwner(ZMeetingReminderWrap* obj) { owner_ = obj; }
	virtual void onReminderNotify(ZOOM_SDK_NAMESPACE::IMeetingReminderContent* reminder, ZOOM_SDK_NAMESPACE::IMeetingReminderHandler* handle)
	{
		if (owner_ && reminder)
		{
			ZMeetingReminderHandler::GetInst().SetHandler(handle);

			ZNMeetingReminderContent zn_meeting_reminder;
			zn_meeting_reminder.title = reminder->GetTitle();
			zn_meeting_reminder.content = reminder->GetContent();
			zn_meeting_reminder.is_blocking = reminder->IsBlocking();
			zn_meeting_reminder.type = Map2WrapDefine(reminder->GetType());
			owner_->onReminderNotify(zn_meeting_reminder);
		}
	}
	virtual void onEnableReminderNotify(ZOOM_SDK_NAMESPACE::IMeetingReminderContent* reminder, ZOOM_SDK_NAMESPACE::IMeetingEnableReminderHandler* handle)
	{
		if (owner_ && reminder)
		{
			ZMeetingEnableReminderHandler::GetInst().SetHandler(handle);

			ZNMeetingReminderContent zn_meeting_reminder;
			zn_meeting_reminder.title = reminder->GetTitle();
			zn_meeting_reminder.content = reminder->GetContent();
			zn_meeting_reminder.is_blocking = reminder->IsBlocking();
			zn_meeting_reminder.type = Map2WrapDefine(reminder->GetType());
			owner_->onEnableReminderNotify(zn_meeting_reminder);
		}
	}
private:
	ZMeetingReminderWrap* owner_;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZSDKFileSender
{
public:
	static ZSDKFileSender& GetInst()
	{
		static ZSDKFileSender inst;
		return inst;
	}
	void SetHandler(ZOOM_SDK_NAMESPACE::ISDKFileSender *sender)
	{
		m_pHandler = sender;
	}

	unsigned int GetReceiver()
	{
		unsigned int zn_id;
		if (m_pHandler)
		{
			zn_id = m_pHandler->GetReceiver();
		}
		return zn_id;
	}

	ZNSDKError CancelSend()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_WRONG_USAGE;
		if (m_pHandler)
		{
			m_pHandler->CancelSend();
			err = ZOOM_SDK_NAMESPACE::SDKERR_SUCCESS;
		}
		return Map2WrapDefine(err);
	}
private:
	ZSDKFileSender() :m_pHandler(NULL) {}
	ZOOM_SDK_NAMESPACE::ISDKFileSender* m_pHandler;
};

class ZSDKFileReceiver
{
public:
	static ZSDKFileReceiver& GetInst()
	{
		static ZSDKFileReceiver inst;
		return inst;
	}
	void SetHandler(ZOOM_SDK_NAMESPACE::ISDKFileReceiver *receiver)
	{
		m_pHandler = receiver;
	}

	unsigned int GetSender()
	{
		unsigned int zn_id;
		if (m_pHandler)
		{
			zn_id = m_pHandler->GetSender();
		}
		return zn_id;
	}

	ZNSDKError CancelReceive()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_WRONG_USAGE;
		if (m_pHandler)
		{
			m_pHandler->CancelReceive();
			err = ZOOM_SDK_NAMESPACE::SDKERR_SUCCESS;
		}
		return Map2WrapDefine(err);
	}

	ZNSDKError StartReceive(ZoomSTRING path)
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_WRONG_USAGE;
		if (m_pHandler)
		{
			err = m_pHandler->StartReceive(path.c_str());
		}
		return Map2WrapDefine(err);
	}
private:
	ZSDKFileReceiver() :m_pHandler(NULL) {}
	ZOOM_SDK_NAMESPACE::ISDKFileReceiver* m_pHandler;
};

class ZMeetingChatCtrlEvent : public ZOOM_SDK_NAMESPACE::IMeetingChatCtrlEvent
{
public:
	void SetOwner(ZMeetingChatWrap* obj) { owner_ = obj; }
	virtual void onChatMsgNotification(ZOOM_SDK_NAMESPACE::IChatMsgInfo* chatMsg, const zchar_t* content)
	{
		if (owner_) {
			ZoomSTRING zn_content;
			if (content)
			{
				zn_content = content;
			}
			ZNChatMsgInfo zn_chatMsg = ConvertSDKChatMsgInfoToZNChatMsgInfo(chatMsg);

			owner_->onChatMsgNotification(zn_chatMsg, zn_content);
		}
	}
	virtual void onChatStatusChangedNotification(ZOOM_SDK_NAMESPACE::ChatStatus* status)
	{
		if (owner_) {
			ZNChatStatus zn_chat_status;
			zn_chat_status.is_chat_off = status->is_chat_off;
			zn_chat_status.is_webinar_attendee = status->is_webinar_attendee;
			zn_chat_status.is_webinar_meeting = status->is_webinar_meeting;
			if (status->is_webinar_meeting)
			{
				if (status->is_webinar_attendee) 
				{
					zn_chat_status.ut.webinar_attendee_status.can_chat = status->ut.webinar_attendee_status.can_chat;
					zn_chat_status.ut.webinar_attendee_status.can_chat_to_all_panellist_and_attendee = status->ut.webinar_attendee_status.can_chat_to_all_panellist_and_attendee;
					zn_chat_status.ut.webinar_attendee_status.can_chat_to_all_panellist = status->ut.webinar_attendee_status.can_chat_to_all_panellist;
				} else {
					zn_chat_status.ut.webinar_other_status.can_chat_to_all_panellist = status->ut.webinar_other_status.can_chat_to_all_panellist;
					zn_chat_status.ut.webinar_other_status.can_chat_to_all_panellist_and_attendee = status->ut.webinar_other_status.can_chat_to_all_panellist_and_attendee;
					zn_chat_status.ut.webinar_other_status.can_chat_to_individual = status->ut.webinar_other_status.can_chat_to_individual;
				}
			} else {
				zn_chat_status.ut.normal_meeting_status.can_chat = status->ut.normal_meeting_status.can_chat;
				zn_chat_status.ut.normal_meeting_status.can_chat_to_all = status->ut.normal_meeting_status.can_chat_to_all;
				zn_chat_status.ut.normal_meeting_status.can_chat_to_individual = status->ut.normal_meeting_status.can_chat_to_individual;
				zn_chat_status.ut.normal_meeting_status.is_only_can_chat_to_host = status->ut.normal_meeting_status.is_only_can_chat_to_host;
			}
			owner_->onChatStatusChangedNotification(zn_chat_status);
		}
	}
	virtual void onChatMsgDeleteNotification(const zchar_t* msgID, ZOOM_SDK_NAMESPACE::SDKChatMessageDeleteType deleteBy)
	{
		if (owner_) {
			ZoomSTRING zn_msgID;
			if (msgID)
			{
				zn_msgID = msgID;
				owner_->onChatMsgDeleteNotification(zn_msgID, Map2WrapDefine(deleteBy));
			}
		}
	}
	virtual void onChatMessageEditNotification(ZOOM_SDK_NAMESPACE::IChatMsgInfo* chatMsg)
	{
		if (owner_) {
			ZNChatMsgInfo zn_chatMsg = ConvertSDKChatMsgInfoToZNChatMsgInfo(chatMsg);

			owner_->onChatMessageEditNotification(zn_chatMsg);
		}
	}
	virtual void onShareMeetingChatStatusChanged(bool isStart)
	{
		if (owner_) {
			owner_->onShareMeetingChatStatusChanged(isStart);
		}
	}
	virtual void onFileSendStart(ZOOM_SDK_NAMESPACE::ISDKFileSender* sender)
	{
		if (owner_) {
			ZSDKFileSender::GetInst().SetHandler(sender);

			ZOOM_SDK_NAMESPACE::SDKFileTransferInfo* transfer_info = sender->GetTransferInfo();
			if (transfer_info)
			{
				ZNSDKFileTransferInfo zn_sender_info;
				if (transfer_info->messageID)
				{
					zn_sender_info.messageID = transfer_info->messageID;
				}
				zn_sender_info.trans_status = Map2WrapDefine(transfer_info->trans_status);
				zn_sender_info.time_stamp = transfer_info->time_stamp;
				zn_sender_info.is_send_to_all = transfer_info->is_send_to_all;
				zn_sender_info.file_size = transfer_info->file_size;
				if (transfer_info->file_name)
				{
					zn_sender_info.file_name = transfer_info->file_name;
				}
				zn_sender_info.complete_percentage = transfer_info->complete_percentage;
				zn_sender_info.complete_size = transfer_info->complete_size;
				zn_sender_info.bit_per_second = transfer_info->bit_per_second;
				owner_->onFileSendStart(zn_sender_info);
			}
		}
	}
	virtual void onFileReceived(ZOOM_SDK_NAMESPACE::ISDKFileReceiver* receiver)
	{
		if (owner_) {
			ZSDKFileReceiver::GetInst().SetHandler(receiver);

			ZOOM_SDK_NAMESPACE::SDKFileTransferInfo* transfer_info = receiver->GetTransferInfo();
			if (transfer_info)
			{
				ZNSDKFileTransferInfo zn_receiver_info;
				if (transfer_info->messageID)
				{
					zn_receiver_info.messageID = transfer_info->messageID;
				}
				zn_receiver_info.trans_status = Map2WrapDefine(transfer_info->trans_status);
				zn_receiver_info.time_stamp = transfer_info->time_stamp;
				zn_receiver_info.is_send_to_all = transfer_info->is_send_to_all;
				zn_receiver_info.file_size = transfer_info->file_size;
				if (transfer_info->file_name)
				{
					zn_receiver_info.file_name = transfer_info->file_name;
				}
				zn_receiver_info.complete_percentage = transfer_info->complete_percentage;
				zn_receiver_info.complete_size = transfer_info->complete_size;
				zn_receiver_info.bit_per_second = transfer_info->bit_per_second;
				owner_->onFileReceived(zn_receiver_info);
			}
		}
	}
	virtual void onFileTransferProgress(ZOOM_SDK_NAMESPACE::SDKFileTransferInfo* info)
	{
		if (owner_ && info) {
			ZNSDKFileTransferInfo zn_transfer_info;
			if (info->messageID)
			{
				zn_transfer_info.messageID = info->messageID;
			}
			zn_transfer_info.trans_status = Map2WrapDefine(info->trans_status);
			zn_transfer_info.time_stamp = info->time_stamp;
			zn_transfer_info.is_send_to_all = info->is_send_to_all;
			zn_transfer_info.file_size = info->file_size;
			if (info->file_name)
			{
				zn_transfer_info.file_name = info->file_name;
			}
			zn_transfer_info.complete_percentage = info->complete_percentage;
			zn_transfer_info.complete_size = info->complete_size;
			zn_transfer_info.bit_per_second = info->bit_per_second;
			owner_->onFileTransferProgress(zn_transfer_info);
		}
	}
private:
	ZMeetingChatWrap* owner_;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZMeetingWaitingRoomHandler
{
public:
	static ZMeetingWaitingRoomHandler& GetInst()
	{
		static ZMeetingWaitingRoomHandler inst;
		return inst;
	}
	void SetHandler(ZOOM_SDK_NAMESPACE::IWaitingRoomDataDownloadHandler *handler)
	{
		m_pHandler = handler;
	}

	ZNSDKError Retry()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_WRONG_USAGE;
		if (m_pHandler)
		{
			m_pHandler->Retry();
			err = ZOOM_SDK_NAMESPACE::SDKERR_SUCCESS;
		}
		m_pHandler = NULL;
		return Map2WrapDefine(err);
	}

	ZNSDKError Ignore()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_WRONG_USAGE;
		if (m_pHandler)
		{
			m_pHandler->Ignore();
			err = ZOOM_SDK_NAMESPACE::SDKERR_SUCCESS;
		}
		m_pHandler = NULL;
		return Map2WrapDefine(err);
	}
private:
	ZMeetingWaitingRoomHandler() :m_pHandler(NULL) {}
	ZOOM_SDK_NAMESPACE::IWaitingRoomDataDownloadHandler* m_pHandler;
};

class ZMeetingWaitingRoomCtrlEvent : public ZOOM_SDK_NAMESPACE::IMeetingWaitingRoomEvent
{
public:
	void SetOwner(ZMeetingWaitingRoomWrap* obj) { owner_ = obj; }
	virtual void onWaitingRoomUserJoin(unsigned int userID)
	{
		if (owner_) {
			owner_->onWaitingRoomUserJoin(userID);
		}
	}
	virtual void onWaitingRoomUserLeft(unsigned int userID)
	{
		if (owner_) {
			owner_->onWaitingRoomUserLeft(userID);
		}
	}
	virtual void onWaitingRoomPresetAudioStatusChanged(bool bAudioCanTurnOn)
	{
		if (owner_) {
			owner_->onWaitingRoomPresetAudioStatusChanged(bAudioCanTurnOn);
		}
	}
	virtual void onWaitingRoomPresetVideoStatusChanged(bool bVideoCanTurnOn)
	{
		if (owner_) {
			owner_->onWaitingRoomPresetVideoStatusChanged(bVideoCanTurnOn);
		}
	}
	virtual void onCustomWaitingRoomDataUpdated(ZOOM_SDK_NAMESPACE::CustomWaitingRoomData& bData, ZOOM_SDK_NAMESPACE::IWaitingRoomDataDownloadHandler* bHandler)
	{
		if (owner_) {
			ZMeetingWaitingRoomHandler::GetInst().SetHandler(bHandler);

			ZNCustomWaitingRoomData zn_custom_waiting_room_data;
			zn_custom_waiting_room_data.title = bData.title;
			zn_custom_waiting_room_data.description = bData.description;
			zn_custom_waiting_room_data.logo_path = bData.logo_path;
			zn_custom_waiting_room_data.video_path = bData.video_path;
			zn_custom_waiting_room_data.image_path = bData.image_path;
			zn_custom_waiting_room_data.type = Map2WrapDefine(bData.type);
			zn_custom_waiting_room_data.status = Map2WrapDefine(bData.status);

			zn_custom_waiting_room_data.page_color.background_color.red = bData.page_color.background_color.red;
			zn_custom_waiting_room_data.page_color.background_color.green = bData.page_color.background_color.green;
			zn_custom_waiting_room_data.page_color.background_color.blue = bData.page_color.background_color.blue;

			zn_custom_waiting_room_data.text_color.primary_color.red = bData.text_color.primary_color.red;
			zn_custom_waiting_room_data.text_color.primary_color.green = bData.text_color.primary_color.green;
			zn_custom_waiting_room_data.text_color.primary_color.blue = bData.text_color.primary_color.blue;

			zn_custom_waiting_room_data.text_color.secondary_color.red = bData.text_color.secondary_color.red;
			zn_custom_waiting_room_data.text_color.secondary_color.green = bData.text_color.secondary_color.green;
			zn_custom_waiting_room_data.text_color.secondary_color.blue = bData.text_color.secondary_color.blue;

			zn_custom_waiting_room_data.text_color.paragraph_color.red = bData.text_color.paragraph_color.red;
			zn_custom_waiting_room_data.text_color.paragraph_color.green = bData.text_color.paragraph_color.green;
			zn_custom_waiting_room_data.text_color.paragraph_color.blue = bData.text_color.paragraph_color.blue;

			zn_custom_waiting_room_data.text_color.hyperlink_color.red = bData.text_color.hyperlink_color.red;
			zn_custom_waiting_room_data.text_color.hyperlink_color.green = bData.text_color.hyperlink_color.green;
			zn_custom_waiting_room_data.text_color.hyperlink_color.blue = bData.text_color.hyperlink_color.blue;

			zn_custom_waiting_room_data.button_color.primary_button_color.red = bData.button_color.primary_button_color.red;
			zn_custom_waiting_room_data.button_color.primary_button_color.green = bData.button_color.primary_button_color.green;
			zn_custom_waiting_room_data.button_color.primary_button_color.blue = bData.button_color.primary_button_color.blue;
			owner_->onCustomWaitingRoomDataUpdated(zn_custom_waiting_room_data);
		}
	}
	virtual void onWaitingRoomUserNameChanged(unsigned int userID, const zchar_t* userName)
	{
		if (owner_) {
			ZoomSTRING zn_userName;
			if (userName)
			{
				zn_userName = userName;
			}
			owner_->onWaitingRoomUserNameChanged(userID, zn_userName);
		}
	}
	virtual void onWaitingRoomEntranceEnabled(bool bIsEnabled)
	{
		if (owner_) {
			owner_->onWaitingRoomEntranceEnabled(bIsEnabled);
		}
	}
private:
	ZMeetingWaitingRoomWrap* owner_;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZAICompanionFeatureTurnOnAgainHandler
{
public:
	static ZAICompanionFeatureTurnOnAgainHandler& GetInst()
	{
		static ZAICompanionFeatureTurnOnAgainHandler inst;
		return inst;
	}
	void SetHandler(ZOOM_SDK_NAMESPACE::IAICompanionFeatureTurnOnAgainHandler *handler)
	{
		m_pHandler = handler;
	}

	ZNList<ZNAICompanionFeature> GetFeatureList()
	{ 
		ZNList<ZNAICompanionFeature> zn_list;
		if (m_pHandler)
		{
			ZOOM_SDK_NAMESPACE::IList<ZOOM_SDK_NAMESPACE::AICompanionFeature>* pFeatureList = m_pHandler->GetFeatureList();
			if (pFeatureList && pFeatureList->GetCount() > 0)
			{
				int count = pFeatureList->GetCount();
				for (int i = 0; i < count; i++)
				{
					ZOOM_SDK_NAMESPACE::AICompanionFeature aiCompanionFeature = pFeatureList->GetItem(i);
					zn_list.push_back(Map2WrapDefine(aiCompanionFeature));
				}
			}
		}
		return zn_list;
	}

	ZNList<ZNAICompanionFeature> GetAssetsDeletedFeatureList()
	{ 
		ZNList<ZNAICompanionFeature> zn_list;
		if (m_pHandler)
		{
			ZOOM_SDK_NAMESPACE::IList<ZOOM_SDK_NAMESPACE::AICompanionFeature>* pFeatureList = m_pHandler->GetAssetsDeletedFeatureList();
			if (pFeatureList && pFeatureList->GetCount() > 0)
			{
				int count = pFeatureList->GetCount();
				for (int i = 0; i < count; i++)
				{
					ZOOM_SDK_NAMESPACE::AICompanionFeature aiCompanionFeature = pFeatureList->GetItem(i);
					zn_list.push_back(Map2WrapDefine(aiCompanionFeature));
				}
			}
		}
		return zn_list;
	}

	ZNSDKError TurnOnAgain()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_WRONG_USAGE;
		if (m_pHandler)
		{
			err = m_pHandler->TurnOnAgain();
		}
		m_pHandler = NULL;
		return Map2WrapDefine(err);
	}

	ZNSDKError AgreeTurnOff()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_WRONG_USAGE;
		if (m_pHandler)
		{
			err = m_pHandler->AgreeTurnOff();
		}
		m_pHandler = NULL;
		return Map2WrapDefine(err);
	}
private:
	ZAICompanionFeatureTurnOnAgainHandler() :m_pHandler(NULL) {}
	ZOOM_SDK_NAMESPACE::IAICompanionFeatureTurnOnAgainHandler* m_pHandler;
};

class ZAICompanionFeatureSwitchHandler
{
public:
	static ZAICompanionFeatureSwitchHandler& GetInst()
	{
		static ZAICompanionFeatureSwitchHandler inst;
		return inst;
	}
	void SetHandler(ZOOM_SDK_NAMESPACE::IAICompanionFeatureSwitchHandler *handler)
	{
		m_pHandler = handler;
	}

	unsigned int GetRequestUserID()
	{
		unsigned int zn_id;
		if (m_pHandler)
		{
			zn_id = m_pHandler->GetRequestUserID();
		}
		return zn_id;
	}

	bool IsTurnOn()
	{
		bool bret = false;
		if (m_pHandler)
		{
			bret = m_pHandler->IsTurnOn();
		}
		return bret;
	}

	ZNSDKError Agree(bool deleteAssets)
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_WRONG_USAGE;
		if (m_pHandler)
		{
			err = m_pHandler->Agree(deleteAssets);
		}
		m_pHandler = NULL;
		return Map2WrapDefine(err);
	}

	ZNSDKError Decline()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_WRONG_USAGE;
		if (m_pHandler)
		{
			err = m_pHandler->Decline();
		}
		m_pHandler = NULL;
		return Map2WrapDefine(err);
	}
private:
	ZAICompanionFeatureSwitchHandler() :m_pHandler(NULL) {}
	ZOOM_SDK_NAMESPACE::IAICompanionFeatureSwitchHandler* m_pHandler;
};

class ZMeetingAICompanionCtrlEvent : public ZOOM_SDK_NAMESPACE::IMeetingAICompanionCtrlEvent
{
public:
	void SetOwner(ZMeetingAICompanionWrap* obj) { owner_ = obj; }
	virtual void onAICompanionFeatureTurnOffByParticipant(ZOOM_SDK_NAMESPACE::IAICompanionFeatureTurnOnAgainHandler* handler)
	{
		if (owner_) {
			ZAICompanionFeatureTurnOnAgainHandler::GetInst().SetHandler(handler);
			owner_->onAICompanionFeatureTurnOffByParticipant();
		}
	}
	virtual void onAICompanionFeatureSwitchRequested(ZOOM_SDK_NAMESPACE::IAICompanionFeatureSwitchHandler* handler)
	{
		if (owner_) {
			ZAICompanionFeatureSwitchHandler::GetInst().SetHandler(handler);
			owner_->onAICompanionFeatureSwitchRequested();
		}
	}
	virtual void onAICompanionFeatureSwitchRequestResponse(bool bTimeout, bool bAgree, bool bTurnOn)
	{
		if (owner_) {
			owner_->onAICompanionFeatureSwitchRequestResponse(bTimeout, bAgree, bTurnOn);
		}
	}
	virtual void onAICompanionFeatureCanNotBeTurnedOff(ZOOM_SDK_NAMESPACE::IList<ZOOM_SDK_NAMESPACE::AICompanionFeature>* features)
	{
		if (owner_) {
			ZNList<ZNAICompanionFeature> zn_features;
			if (features && features->GetCount() > 0)
			{
				int count = features->GetCount();
				for (int i = 0; i < count; i++)
				{
					ZOOM_SDK_NAMESPACE::AICompanionFeature aiCompanionFeature = features->GetItem(i);
					zn_features.push_back(Map2WrapDefine(aiCompanionFeature));
				}
			}
			owner_->onAICompanionFeatureCanNotBeTurnedOff(zn_features);
		}
	}
private:
	ZMeetingAICompanionWrap* owner_;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZMeetingEnableSmartSummaryHandler
{
public:
	static ZMeetingEnableSmartSummaryHandler& GetInst()
	{
		static ZMeetingEnableSmartSummaryHandler inst;
		return inst;
	}
	void SetHandler(ZOOM_SDK_NAMESPACE::IMeetingEnableSmartSummaryHandler *handler)
	{
		m_pHandler = handler;
	}

	ZNSDKError EnableSmartSummary()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_NO_PERMISSION;
		if (m_pHandler)
		{
			err = m_pHandler->EnableSmartSummary();
		}
		return Map2WrapDefine(err);
	}

	bool IsForRequestEnableSmartSummary()
	{
		bool bret = false;
		if (m_pHandler)
		{
			bret = m_pHandler->IsForRequest();
		}
		return bret;
	}
private:
	ZMeetingEnableSmartSummaryHandler() :m_pHandler(NULL) {}
	ZOOM_SDK_NAMESPACE::IMeetingEnableSmartSummaryHandler* m_pHandler;
};

class ZMeetingStartSmartSummaryHandler
{
public:
	static ZMeetingStartSmartSummaryHandler& GetInst()
	{
		static ZMeetingStartSmartSummaryHandler inst;
		return inst;
	}
	void SetHandler(ZOOM_SDK_NAMESPACE::IMeetingStartSmartSummaryHandler *handler)
	{
		m_pHandler = handler;
	}

	ZNSDKError StartSmartSummary()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_NO_PERMISSION;
		if (m_pHandler)
		{
			err = m_pHandler->StartSmartSummary();
		}
		return Map2WrapDefine(err);
	}

	bool IsForRequestStartSmartSummary()
	{
		bool bret = false;
		if (m_pHandler)
		{
			bret = m_pHandler->IsForRequest();
		}
		return bret;
	}
private:
	ZMeetingStartSmartSummaryHandler() :m_pHandler(NULL) {}
	ZOOM_SDK_NAMESPACE::IMeetingStartSmartSummaryHandler* m_pHandler;
};

class ZMeetingStopSmartSummaryHandler
{
public:
	static ZMeetingStopSmartSummaryHandler& GetInst()
	{
		static ZMeetingStopSmartSummaryHandler inst;
		return inst;
	}
	void SetHandler(ZOOM_SDK_NAMESPACE::IMeetingStopSmartSummaryHandler *handler)
	{
		m_pHandler = handler;
	}

	ZNSDKError StopSmartSummary()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_NO_PERMISSION;
		if (m_pHandler)
		{
			err = m_pHandler->StopSmartSummary();
		}
		return Map2WrapDefine(err);
	}
private:
	ZMeetingStopSmartSummaryHandler() :m_pHandler(NULL) {}
	ZOOM_SDK_NAMESPACE::IMeetingStopSmartSummaryHandler* m_pHandler;
};

class ZMeetingApproveEnableSmartSummaryHandler
{
public:
	static ZMeetingApproveEnableSmartSummaryHandler& GetInst()
	{
		static ZMeetingApproveEnableSmartSummaryHandler inst;
		return inst;
	}
	void SetHandler(ZOOM_SDK_NAMESPACE::IMeetingApproveEnableSmartSummaryHandler *handler)
	{
		m_pHandler = handler;
	}

	ZNSDKError ContinueApproveEnableSmartSummary()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_NO_PERMISSION;
		if (m_pHandler)
		{
			err = m_pHandler->ContinueApprove();
		}
		return Map2WrapDefine(err);
	}

	unsigned int GetSenderEnableSmartSummaryUserID()
	{
		unsigned int zn_id;
		if (m_pHandler)
		{
			zn_id = m_pHandler->GetSenderUserID();
		}
		return zn_id;
	}
private:
	ZMeetingApproveEnableSmartSummaryHandler() :m_pHandler(NULL) {}
	ZOOM_SDK_NAMESPACE::IMeetingApproveEnableSmartSummaryHandler* m_pHandler;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZMeetingRequestStartCaptionHandler
{
public:
	static ZMeetingRequestStartCaptionHandler& GetInst()
	{
		static ZMeetingRequestStartCaptionHandler inst;
		return inst;
	}
	void SetHandler(ZOOM_SDK_NAMESPACE::ICCRequestHandler *handler)
	{
		m_pHandler = handler;
	}

	ZNSDKError DenyStartCaptionRequest()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_NO_PERMISSION;
		if (m_pHandler)
		{
			err = m_pHandler->Deny();
		}
		return Map2WrapDefine(err);
	}

	bool IsRequestTranslationOn()
	{
		bool bret = false;
		if (m_pHandler)
		{
			bret = m_pHandler->IsRequestTranslationOn();
		}
		return bret;
	}

	unsigned int GetSenderRequestStartCaptionUserID()
	{
		unsigned int zn_id;
		if (m_pHandler)
		{
			zn_id = m_pHandler->GetSenderUserId();
		}
		return zn_id;
	}
private:
	ZMeetingRequestStartCaptionHandler() :m_pHandler(NULL) {}
	ZOOM_SDK_NAMESPACE::ICCRequestHandler* m_pHandler;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZMeetingClosedCaptionControllerEvent : public ZOOM_SDK_NAMESPACE::IClosedCaptionControllerEvent 
{
public:
	void SetOwner(ZMeetingCloseCaptionWrap *obj) { owner_ = obj; }

	virtual void onAssignedToSendCC(bool bAssigned) {}

	virtual void onClosedCaptionMsgReceived(const zchar_t* ccMsg, unsigned int sender_id, time_t time) {}

	virtual void onLiveTranscriptionStatus(ZOOM_SDK_NAMESPACE::SDKLiveTranscriptionStatus status) {}

	virtual void onOriginalLanguageMsgReceived(ZOOM_SDK_NAMESPACE::ILiveTranscriptionMessageInfo* messageInfo) {
		if (owner_) 
		{
			ZNLiveTranscriptionMessageInfo zn_message_info = ConvertSDKILiveTranscriptionMessageInfoToZNLiveTranscriptionMessageInfo(messageInfo);

			owner_->onOriginalLanguageMsgReceived(zn_message_info);
		}
	}

	virtual void onLiveTranscriptionMsgInfoReceived(ZOOM_SDK_NAMESPACE::ILiveTranscriptionMessageInfo* messageInfo) {
		if (owner_) 
		{
			ZNLiveTranscriptionMessageInfo zn_message_info = ConvertSDKILiveTranscriptionMessageInfoToZNLiveTranscriptionMessageInfo(messageInfo);

			owner_->onLiveTranscriptionMsgInfoReceived(zn_message_info);
		}
	}

	virtual void onLiveTranscriptionMsgError(ZOOM_SDK_NAMESPACE::ILiveTranscriptionLanguage* spokenLanguage, ZOOM_SDK_NAMESPACE::ILiveTranscriptionLanguage* transcriptLanguage) {}

	virtual void onRequestForLiveTranscriptReceived(unsigned int requester_id, bool bAnonymous) {}

	virtual void onRequestLiveTranscriptionStatusChange(bool bEnabled) {}

	virtual void onCaptionStatusChanged(bool bEnabled) {}

	virtual void onStartCaptionsRequestReceived(ZOOM_SDK_NAMESPACE::ICCRequestHandler* handler) {
	// if (owner_) {
	// 	ZMeetingRequestStartCaptionHandler::GetInst().SetHandler(handler);

	// 	owner_->onStartCaptionsRequestReceived();
	// }
	}

	virtual void onStartCaptionsRequestApproved() {}

	virtual void onManualCaptionStatusChanged(bool bEnabled) {}

	virtual void onSpokenLanguageChanged(ZOOM_SDK_NAMESPACE::ILiveTranscriptionLanguage* spokenLanguage)
	{
		
	}

private:
	ZMeetingCloseCaptionWrap *owner_;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZMeetingEmojiReactionControllerEvent : public ZOOM_SDK_NAMESPACE::IEmojiReactionControllerEvent {
public:
	void SetOwner(ZMeetingReactionWrap *obj) { owner_ = obj; }

	virtual void OnEmojiReactionReceived(unsigned int sender_id, ZOOM_SDK_NAMESPACE::SDKEmojiReactionType type) {
		if (owner_) {
			owner_->onEmojiReactionReceived(sender_id, (ZNSDKEmojiReactionType)type, (ZNSDKEmojiReactionSkinTone)0);
		}
	}

	virtual void OnEmojiReactionReceivedInWebinar(ZOOM_SDK_NAMESPACE::SDKEmojiReactionType type) {
		if (owner_) {
			owner_->onEmojiReactionReceivedInWebinar((ZNSDKEmojiReactionType)type);
		}
	}

	virtual void OnEmojiFeedbackReceived(unsigned int sender_id, ZOOM_SDK_NAMESPACE::SDKEmojiFeedbackType type) {}

	virtual void OnEmojiFeedbackCanceled(unsigned int sender_id) {}

private:
	ZMeetingReactionWrap *owner_;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZMeetingQACtrlEvent : public ZOOM_SDK_NAMESPACE::IMeetingQAControllerEvent {
public:
	void SetOwner(ZMeetingQAWrap *obj) { owner_ = obj; }

	virtual void OnQAConnectStatus(ZOOM_SDK_NAMESPACE::QAConnectStatus connectStatus) {}

	virtual void OnAddQuestion(const zchar_t *questionID, bool bSuccess) {
		if (owner_) 
		{
			ZoomSTRING zn_questionID;
			if (questionID) {
				zn_questionID = questionID;
			}
			owner_->onAddQuestion(zn_questionID, bSuccess);
		}
	}

	virtual void OnAddAnswer(const zchar_t *answerID, bool bSuccess) {
		if (owner_) {
			ZoomSTRING zn_answerID;
			if (answerID) {
				zn_answerID = answerID;
			}
			owner_->onAddAnswer(zn_answerID, bSuccess);
		}
	}

	virtual void OnQuestionMarkedAsDismissed(const zchar_t* questionID) {
		if (owner_) {
			ZoomSTRING zn_questionID;
			if (questionID) {
				zn_questionID = questionID;
			}
			owner_->onQuestionMarkedAsDismissed(zn_questionID);
		}
	}

	virtual void OnReopenQuestion(const zchar_t *questionID) {
		if (owner_) {
			ZoomSTRING zn_questionID;
			if (questionID) {
				zn_questionID = questionID;
			}
			owner_->onReopenQuestion(zn_questionID);
		}
	}

	virtual void OnReceiveQuestion(const zchar_t *questionID) {
		if (owner_) {
			ZoomSTRING zn_questionID;
			if (questionID) {
				zn_questionID = questionID;
			}
			owner_->onReceiveQuestion(zn_questionID);
		}
	}

	virtual void OnReceiveAnswer(const zchar_t *answerID) {
		if (owner_) {
			ZoomSTRING zn_answerID;
			if (answerID) {
				zn_answerID = answerID;
			}
			owner_->onReceiveAnswer(zn_answerID);
		}
	}

	virtual void OnUserLivingReply(const zchar_t *questionID) {
		if (owner_) {
			ZoomSTRING zn_questionID;
			if (questionID) {
				zn_questionID = questionID;
			}
			owner_->onUserLivingReply(zn_questionID);
		}
	}

	virtual void OnUserEndLiving(const zchar_t *questionID) {
		if (owner_) {
			ZoomSTRING zn_questionID;
			if (questionID) {
				zn_questionID = questionID;
			}
			owner_->onUserEndLiving(zn_questionID);
		}
	}

	virtual void OnUpvoteQuestion(const zchar_t *questionID, bool order_changed) {
		if (owner_) {
			ZoomSTRING zn_questionID;
			if (questionID) {
				zn_questionID = questionID;
			}
			owner_->onVoteupQuestion(zn_questionID, order_changed);
		}
	}

	virtual void OnRevokeUpvoteQuestion(const zchar_t *questionID, bool order_changed) {
		if (owner_) {
			ZoomSTRING zn_questionID;
			if (questionID) {
				zn_questionID = questionID;
			}
			owner_->onRevokeVoteupQuestion(zn_questionID, order_changed);
		}
	}

	virtual void OnDeleteQuestion(ZOOM_SDK_NAMESPACE::IList<const zchar_t *> *lstQuestionID) {
		if (owner_ && lstQuestionID) {
			ZNList<ZoomSTRING> questions;
			for (int i = 0; i < lstQuestionID->GetCount(); ++i) {
				questions.push_back(lstQuestionID->GetItem(i));
			}
			owner_->onDeleteQuestions(questions);
		}
	}

	virtual void OnDeleteAnswer(ZOOM_SDK_NAMESPACE::IList<const zchar_t *> *lstAnswerID) {
		if (owner_ && lstAnswerID) {
			ZNList<ZoomSTRING> answers;
			for (int i = 0; i < lstAnswerID->GetCount(); ++i) {
				answers.push_back(lstAnswerID->GetItem(i));
			}
			owner_->onDeleteAnswers(answers);
		}
	}

	virtual void OnAllowAskQuestionAnonymousStatus(bool bEnabled) {}

	virtual void OnAllowAttendeeViewAllQuestionStatus(bool bEnabled) {}

	virtual void OnAllowAttendeeVoteupQuestionStatus(bool bEnabled) {}

	virtual void OnAllowAttendeeCommentQuestionStatus(bool bEnabled) {}

	virtual void OnRefreshQAData() {}

	virtual void onMeetingQAStatusChanged(bool bEnabled) {}

	virtual void onAllowAskQuestionStatus(bool bEnabled) {}

private:
	ZMeetingQAWrap *owner_;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZMeetingPollingCtrlEvent : public ZOOM_SDK_NAMESPACE::IMeetingPollingCtrlEvent {
public:
	void SetOwner(ZMeetingPollingWrap* obj) { owner_ = obj; }
	virtual void onPollingStatusChanged(const zchar_t* pollingID, ZOOM_SDK_NAMESPACE::SDKPollingStatus status) {
		if (owner_) {
			ZoomSTRING zn_pollingID;
			if (pollingID) {
				zn_pollingID = pollingID;
			}
			owner_->onPollingStatusChanged(zn_pollingID, (ZNSDKPollingStatus)status);
		}
	}


	virtual void onPollingResultUpdated(const zchar_t* pollingID) {
		if (owner_) {
			ZoomSTRING zn_pollingID;
			if (pollingID) {
				zn_pollingID = pollingID;
			}
			owner_->onPollingResultUpdated(zn_pollingID);
		}
	}

	virtual void onPollingListUpdated() {
		if (owner_) {
			owner_->onPollingListUpdated();
		}
	}

	virtual void onPollingActionResult(ZOOM_SDK_NAMESPACE::SDKPollingActionType actionType, const zchar_t* pollingID, bool bSuccess, const zchar_t* errorMsg) {
		if (owner_) {
			ZoomSTRING zn_pollingID;
			if (pollingID) {
				zn_pollingID = pollingID;
			}
			ZoomSTRING zn_errorMsg;
			if (errorMsg) {
				zn_errorMsg = errorMsg;
			}
			owner_->onPollingActionResult((ZNSDKPollingActionType)actionType, zn_pollingID, bSuccess, zn_errorMsg);
		}
	}

	virtual void onPollingQuestionImageDownloaded(const zchar_t* questionID, const zchar_t* path) {}
	virtual void onPollingElapsedTime(const zchar_t* pollingID, unsigned int uElapsedtime) {}
	virtual void onGetRightAnswerListPrivilege(bool bCan) {}
	virtual void onPollingInactive() {}

private:
	ZMeetingPollingWrap* owner_;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZMeetingApproveStartSmartSummaryHandler
{
public:
	static ZMeetingApproveStartSmartSummaryHandler& GetInst()
	{
		static ZMeetingApproveStartSmartSummaryHandler inst;
		return inst;
	}
	void SetHandler(ZOOM_SDK_NAMESPACE::IMeetingApproveStartSmartSummaryHandler *handler)
	{
		m_pHandler = handler;
	}

	ZNSDKError ApproveStartSmartSummary()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_NO_PERMISSION;
		if (m_pHandler)
		{
			err = m_pHandler->Approve();
		}
		return Map2WrapDefine(err);
	}

	ZNSDKError DeclineStartSmartSummary()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_NO_PERMISSION;
		if (m_pHandler)
		{
			err = m_pHandler->Decline();
		}
		return Map2WrapDefine(err);
	}

	unsigned int GetSenderStartSmartSummaryUserID()
	{
		unsigned int zn_id;
		if (m_pHandler)
		{
			zn_id = m_pHandler->GetSenderUserID();
		}
		return zn_id;
	}
private:
	ZMeetingApproveStartSmartSummaryHandler() :m_pHandler(NULL) {}
	ZOOM_SDK_NAMESPACE::IMeetingApproveStartSmartSummaryHandler* m_pHandler;
};

class ZMeetingEnableSmartSummaryActionHandler
{
public:
	static ZMeetingEnableSmartSummaryActionHandler& GetInst()
	{
		static ZMeetingEnableSmartSummaryActionHandler inst;
		return inst;
	}
	void SetHandler(ZOOM_SDK_NAMESPACE::IMeetingEnableSmartSummaryActionHandler *handler)
	{
		m_pHandler = handler;
	}

	ZoomSTRING GetEnableSmartSummaryActionTipTitle()
	{
		ZoomSTRING zn_tip_title;
		if (m_pHandler)
		{
			const zchar_t* tipTitle = m_pHandler->GetTipTitle();
			if (tipTitle)
			{
				zn_tip_title = tipTitle;
			}
		}
		return zn_tip_title;
	}

	ZoomSTRING GetEnableSmartSummaryActionTipString()
	{
		ZoomSTRING zn_tip;
		if (m_pHandler)
		{
			const zchar_t* tip = m_pHandler->GetTipString();
			if (tip)
			{
				zn_tip = tip;
			}
		}
		return zn_tip;
	}

	ZNSDKError ConfirmEnableSmartSummaryAction()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_NO_PERMISSION;
		if (m_pHandler)
		{
			err = m_pHandler->Confirm();
		}
		m_pHandler = NULL;
		return Map2WrapDefine(err);
	}

	ZNSDKError CancelEnableSmartSummaryAction()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_NO_PERMISSION;
		if (m_pHandler)
		{
			err = m_pHandler->Cancel();
		}
		m_pHandler = NULL;
		return Map2WrapDefine(err);
	}
private:
	ZMeetingEnableSmartSummaryActionHandler() :m_pHandler(NULL) {}
	ZOOM_SDK_NAMESPACE::IMeetingEnableSmartSummaryActionHandler* m_pHandler;
};

class ZMeetingAICompanionSmartSummaryEvent : public ZOOM_SDK_NAMESPACE::IMeetingAICompanionSmartSummaryHelperEvent
{
public:
	void SetOwner(ZMeetingAICompanionWrap* obj) { owner_ = obj; }
	virtual void onSmartSummaryStateNotSupported()
	{
		if (owner_) {
			owner_->onSmartSummaryStateNotSupported();
		}
	}
	virtual void onSmartSummaryStateSupportedButDisabled(ZOOM_SDK_NAMESPACE::IMeetingEnableSmartSummaryHandler* handler)
	{
		if (owner_) {
			ZMeetingEnableSmartSummaryHandler::GetInst().SetHandler(handler);

			owner_->onSmartSummaryStateSupportedButDisabled();
		}
	}
	virtual void onSmartSummaryStateEnabledButNotStarted(ZOOM_SDK_NAMESPACE::IMeetingStartSmartSummaryHandler* handler)
	{
		if (owner_) {
			ZMeetingStartSmartSummaryHandler::GetInst().SetHandler(handler);

			owner_->onSmartSummaryStateEnabledButNotStarted();
		}
	}
	virtual void onSmartSummaryStateStarted(ZOOM_SDK_NAMESPACE::IMeetingStopSmartSummaryHandler* handler)
	{
		if (owner_) {
			ZMeetingStopSmartSummaryHandler::GetInst().SetHandler(handler);

			owner_->onSmartSummaryStateStarted();
		}
	}
	virtual void onFailedToStartSmartSummary(bool bTimeout)
	{
		if (owner_) {
			owner_->onFailedToStartSmartSummary(bTimeout);
		}
	}
	virtual void onSmartSummaryEnableRequestReceived(ZOOM_SDK_NAMESPACE::IMeetingApproveEnableSmartSummaryHandler* handler)
	{
		if (owner_) {
			ZMeetingApproveEnableSmartSummaryHandler::GetInst().SetHandler(handler);

			owner_->onSmartSummaryEnableRequestReceived();
		}
	}
	virtual void onSmartSummaryStartRequestReceived(ZOOM_SDK_NAMESPACE::IMeetingApproveStartSmartSummaryHandler* handler)
	{
		if (owner_) {
			ZMeetingApproveStartSmartSummaryHandler::GetInst().SetHandler(handler);

			owner_->onSmartSummaryStartRequestReceived();
		}
	}
	virtual void onSmartSummaryEnableActionCallback(ZOOM_SDK_NAMESPACE::IMeetingEnableSmartSummaryActionHandler* handler)
	{
		if (owner_) {
			ZMeetingEnableSmartSummaryActionHandler::GetInst().SetHandler(handler);

			owner_->onSmartSummaryEnableActionCallback();
		}
	}
private:
	ZMeetingAICompanionWrap* owner_;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZMeetingEnableQueryHandler
{
public:
	static ZMeetingEnableQueryHandler& GetInst()
	{
		static ZMeetingEnableQueryHandler inst;
		return inst;
	}
	void SetHandler(ZOOM_SDK_NAMESPACE::IMeetingEnableQueryHandler *handler)
	{
		m_pHandler = handler;
	}

	ZNSDKError EnableQuery()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_NO_PERMISSION;
		if (m_pHandler)
		{
			err = m_pHandler->EnableQuery();
		}
		return Map2WrapDefine(err);
	}

	bool IsForRequestEnableQuery()
	{
		bool bret = false;
		if (m_pHandler)
		{
			bret = m_pHandler->IsForRequest();
		}
		return bret;
	}
private:
	ZMeetingEnableQueryHandler() :m_pHandler(NULL) {}
	ZOOM_SDK_NAMESPACE::IMeetingEnableQueryHandler* m_pHandler;
};

class ZMeetingStartQueryHandler
{
public:
	static ZMeetingStartQueryHandler& GetInst()
	{
		static ZMeetingStartQueryHandler inst;
		return inst;
	}
	void SetHandler(ZOOM_SDK_NAMESPACE::IMeetingStartQueryHandler *handler)
	{
		m_pHandler = handler;
	}

	ZNSDKError StartMeetingQuery()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_NO_PERMISSION;
		if (m_pHandler)
		{
			err = m_pHandler->StartMeetingQuery();
		}
		return Map2WrapDefine(err);
	}

	bool IsForRequestStartMeetingQuery()
	{
		bool bret = false;
		if (m_pHandler)
		{
			bret = m_pHandler->IsForRequest();
		}
		return bret;
	}
private:
	ZMeetingStartQueryHandler() :m_pHandler(NULL) {}
	ZOOM_SDK_NAMESPACE::IMeetingStartQueryHandler* m_pHandler;
};

class ZMeetingSendQueryHandler
{
public:
	static ZMeetingSendQueryHandler& GetInst()
	{
		static ZMeetingSendQueryHandler inst;
		return inst;
	}
	void SetHandler(ZOOM_SDK_NAMESPACE::IMeetingSendQueryHandler *handler)
	{
		m_pHandler = handler;
	}

	ZNList<ZoomSTRING> GetDefaultQueryQuestions()
	{
		ZNList<ZoomSTRING> zn_list;
		if (m_pHandler)
		{
			ZOOM_SDK_NAMESPACE::IList<const zchar_t*>* pQueryQuestions = m_pHandler->GetDefaultQueryQuestions();
			if (pQueryQuestions && pQueryQuestions->GetCount() > 0)
			{
				int count = pQueryQuestions->GetCount();
				for (int i = 0; i < count; i++)
				{
					const zchar_t* pQuestion = pQueryQuestions->GetItem(i);
					if (pQuestion)
					{
						ZoomSTRING zn_question = pQuestion;
						zn_list.push_back(zn_question);
					}
				}
			}
		}
		return zn_list;
	}
	
	ZNSDKError SendQueryQuestion(ZoomSTRING question)
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_NO_PERMISSION;
		if (m_pHandler)
		{
			err = m_pHandler->SendQueryQuestion(question.c_str());
		}
		return Map2WrapDefine(err);
	}

	ZNSDKError StopMeetingQuery()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_NO_PERMISSION;
		if (m_pHandler)
		{
			err = m_pHandler->StopMeetingQuery();
		}
		return Map2WrapDefine(err);
	}

	bool CanSendQuery()
	{
		bool bret = false;
		if (m_pHandler)
		{
			bret = m_pHandler->CanSendQuery();
		}
		return bret;
	}

	ZNSDKError RequestSendQueryPrivilege()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_NO_PERMISSION;
		if (m_pHandler)
		{
			err = m_pHandler->RequestSendQueryPrivilege();
		}
		return Map2WrapDefine(err);
	}
private:
	ZMeetingSendQueryHandler() :m_pHandler(NULL) {}
	ZOOM_SDK_NAMESPACE::IMeetingSendQueryHandler* m_pHandler;
};

class ZMeetingApproveEnableQueryHandler
{
public:
	static ZMeetingApproveEnableQueryHandler& GetInst()
	{
		static ZMeetingApproveEnableQueryHandler inst;
		return inst;
	}
	void SetHandler(ZOOM_SDK_NAMESPACE::IMeetingApproveEnableQueryHandler *handler)
	{
		m_pHandler = handler;
	}

	unsigned int GetSenderEnableQueryUserID()
	{
		unsigned int zn_id;
		if (m_pHandler)
		{
			zn_id = m_pHandler->GetSenderUserID();
		}
		return zn_id;
	}

	ZNSDKError ContinueApproveEnableQuery()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_NO_PERMISSION;
		if (m_pHandler)
		{
			err = m_pHandler->ContinueApprove();
		}
		return Map2WrapDefine(err);
	}
private:
	ZMeetingApproveEnableQueryHandler() :m_pHandler(NULL) {}
	ZOOM_SDK_NAMESPACE::IMeetingApproveEnableQueryHandler* m_pHandler;
};

class ZMeetingApproveStartQueryHandler
{
public:
	static ZMeetingApproveStartQueryHandler& GetInst()
	{
		static ZMeetingApproveStartQueryHandler inst;
		return inst;
	}
	void SetHandler(ZOOM_SDK_NAMESPACE::IMeetingApproveStartQueryHandler *handler)
	{
		m_pHandler = handler;
	}

	unsigned int GetSenderStartQueryUserID()
	{
		unsigned int zn_id;
		if (m_pHandler)
		{
			zn_id = m_pHandler->GetSenderUserID();
		}
		return zn_id;
	}
	
	ZNSDKError ApproveStartQuery()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_NO_PERMISSION;
		if (m_pHandler)
		{
			err = m_pHandler->Approve();
		}
		return Map2WrapDefine(err);
	}

	ZNSDKError DeclineStartQuery()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_NO_PERMISSION;
		if (m_pHandler)
		{
			err = m_pHandler->Decline();
		}
		return Map2WrapDefine(err);
	}
private:
	ZMeetingApproveStartQueryHandler() :m_pHandler(NULL) {}
	ZOOM_SDK_NAMESPACE::IMeetingApproveStartQueryHandler* m_pHandler;
};

class ZMeetingAICompanionQueryItemHandler
{
public:
	static ZMeetingAICompanionQueryItemHandler& GetInst()
	{
		static ZMeetingAICompanionQueryItemHandler inst;
		return inst;
	}
	void SetHandler(ZOOM_SDK_NAMESPACE::IMeetingAICompanionQueryItem *handler)
	{
		m_pHandler = handler;
	}

	ZNSDKError AICompanionQueryFeedback(ZNMeetingAICompanionQueryFeedbackType eFeedbackType)
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_NO_PERMISSION;
		if (m_pHandler)
		{
			err = m_pHandler->Feedback(Map2SDKDefine(eFeedbackType));
		}
		return Map2WrapDefine(err);
	}
private:
	ZMeetingAICompanionQueryItemHandler() :m_pHandler(NULL) {}
	ZOOM_SDK_NAMESPACE::IMeetingAICompanionQueryItem* m_pHandler;
};

class ZMeetingEnableQueryActionHandler
{
public:
	static ZMeetingEnableQueryActionHandler& GetInst()
	{
		static ZMeetingEnableQueryActionHandler inst;
		return inst;
	}
	void SetHandler(ZOOM_SDK_NAMESPACE::IMeetingEnableQueryActionHandler *handler)
	{
		m_pHandler = handler;
	}

	ZoomSTRING GetEnableQueryActionTipTitle()
	{
		ZoomSTRING zn_tip_title;
		if (m_pHandler)
		{
			const zchar_t* tipTitle = m_pHandler->GetTipTitle();
			if (tipTitle)
			{
				zn_tip_title = tipTitle;
			}
		}
		return zn_tip_title;
	}

	ZoomSTRING GetEnableQueryActionTipString()
	{
		ZoomSTRING zn_tip;
		if (m_pHandler)
		{
			const zchar_t* tip = m_pHandler->GetTipString();
			if (tip)
			{
				zn_tip = tip;
			}
		}
		return zn_tip;
	}

	ZNSDKError ConfirmEnableQueryAction()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_NO_PERMISSION;
		if (m_pHandler)
		{
			err = m_pHandler->Confirm();
		}
		m_pHandler = NULL;
		return Map2WrapDefine(err);
	}

	ZNSDKError CancelEnableQueryAction()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_NO_PERMISSION;
		if (m_pHandler)
		{
			err = m_pHandler->Cancel();
		}
		m_pHandler = NULL;
		return Map2WrapDefine(err);
	}
private:
	ZMeetingEnableQueryActionHandler() :m_pHandler(NULL) {}
	ZOOM_SDK_NAMESPACE::IMeetingEnableQueryActionHandler* m_pHandler;
};

class ZMeetingApproveSendQueryHandler
{
public:
	static ZMeetingApproveSendQueryHandler& GetInst()
	{
		static ZMeetingApproveSendQueryHandler inst;
		return inst;
	}
	void SetHandler(ZOOM_SDK_NAMESPACE::IMeetingApproveSendQueryHandler *handler)
	{
		m_pHandler = handler;
	}

	unsigned int GetSenderQueryUserID()
	{
		unsigned int zn_id;
		if (m_pHandler)
		{
			zn_id = m_pHandler->GetSenderUserID();
		}
		return zn_id;
	}
	
	ZNSDKError ApproveSendQuery()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_NO_PERMISSION;
		if (m_pHandler)
		{
			err = m_pHandler->Approve();
		}
		return Map2WrapDefine(err);
	}

	ZNSDKError DeclineSendQuery(bool bDeclineAll)
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_NO_PERMISSION;
		if (m_pHandler)
		{
			err = m_pHandler->Decline(bDeclineAll);
		}
		return Map2WrapDefine(err);
	}
private:
	ZMeetingApproveSendQueryHandler() :m_pHandler(NULL) {}
	ZOOM_SDK_NAMESPACE::IMeetingApproveSendQueryHandler* m_pHandler;
};

class ZMeetingAICompanionQueryEvent : public ZOOM_SDK_NAMESPACE::IMeetingAICompanionQueryHelperEvent
{
public:
	void SetOwner(ZMeetingAICompanionWrap* obj) { owner_ = obj; }
	virtual void onQueryStateNotSupported()
	{
		if (owner_) {
			owner_->onQueryStateNotSupported();
		}
	}
	virtual void onQueryStateSupportedButDisabled(ZOOM_SDK_NAMESPACE::IMeetingEnableQueryHandler* handler)
	{
		if (owner_) {
			ZMeetingEnableQueryHandler::GetInst().SetHandler(handler);
			owner_->onQueryStateSupportedButDisabled();
		}
	}
	virtual void onQueryStateEnabledButNotStarted(ZOOM_SDK_NAMESPACE::IMeetingStartQueryHandler* handler)
	{
		if (owner_) {
			ZMeetingStartQueryHandler::GetInst().SetHandler(handler);
			owner_->onQueryStateEnabledButNotStarted();
		}
	}
	virtual void onQueryStateStarted(ZOOM_SDK_NAMESPACE::IMeetingSendQueryHandler* handler)
	{
		if (owner_) {
			ZMeetingSendQueryHandler::GetInst().SetHandler(handler);
			owner_->onQueryStateStarted();
		}
	}
	virtual void onQuerySettingChanged(ZOOM_SDK_NAMESPACE::MeetingAICompanionQuerySettingOptions eSetting)
	{
		if (owner_) {
			owner_->onQuerySettingChanged(Map2WrapDefine(eSetting));
		}
	}
	virtual void onFailedToStartQuery(bool bTimeout)
	{
		if (owner_) {
			owner_->onFailedToStartQuery(bTimeout);
		}
	}
	virtual void onReceiveRequestToEnableQuery(ZOOM_SDK_NAMESPACE::IMeetingApproveEnableQueryHandler* handler)
	{
		if (owner_) {
			ZMeetingApproveEnableQueryHandler::GetInst().SetHandler(handler);
			owner_->onReceiveRequestToEnableQuery();
		}
	}
	virtual void onReceiveRequestToStartQuery(ZOOM_SDK_NAMESPACE::IMeetingApproveStartQueryHandler* handler)
	{
		if (owner_) {
			ZMeetingApproveStartQueryHandler::GetInst().SetHandler(handler);
			owner_->onReceiveRequestToStartQuery();
		}
	}
	virtual void onReceiveQueryAnswer(ZOOM_SDK_NAMESPACE::IMeetingAICompanionQueryItem* pQueryItem)
	{
		if (owner_ && pQueryItem) {
			ZMeetingAICompanionQueryItemHandler::GetInst().SetHandler(pQueryItem);

			ZNMeetingAICompanionQueryItem zn_item;
			if (pQueryItem->GetQueryID())
			{
				zn_item.queryID = pQueryItem->GetQueryID();
			}
			if (pQueryItem->GetQustionContent())
			{
				zn_item.qustionContent = pQueryItem->GetQustionContent();
			}
			if (pQueryItem->GetAnswerContent())
			{
				zn_item.answerContent = pQueryItem->GetAnswerContent();
			}
			if (pQueryItem->GetErrorMsg())
			{
				zn_item.errorMsg = pQueryItem->GetErrorMsg();
			}
			zn_item.errorCode = Map2WrapDefine(pQueryItem->GetErrorCode());
			zn_item.timeStamp = pQueryItem->GetTimeStamp();
			owner_->onReceiveQueryAnswer(zn_item);
		}
	}
	virtual void onQueryEnableActionCallback(ZOOM_SDK_NAMESPACE::IMeetingEnableQueryActionHandler* handler)
	{
		if (owner_) {
			ZMeetingEnableQueryActionHandler::GetInst().SetHandler(handler);
			owner_->onQueryEnableActionCallback();
		}
	}
	virtual void onSendQueryPrivilegeChanged(bool canSendQuery)
	{
		if (owner_) {
			owner_->onSendQueryPrivilegeChanged(canSendQuery);
		}
	}
	virtual void onFailedToRequestSendQuery(bool bTimeout)
	{
		if (owner_) {
			owner_->onFailedToRequestSendQuery(bTimeout);
		}
	}
	virtual void onReceiveRequestToSendQuery(ZOOM_SDK_NAMESPACE::IMeetingApproveSendQueryHandler* handler)
	{
		if (owner_) {
			ZMeetingApproveSendQueryHandler::GetInst().SetHandler(handler);
			owner_->onReceiveRequestToSendQuery();
		}
	}
private:
	ZMeetingAICompanionWrap* owner_;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZDirectShareViaMeetingIDOrPairingCodeHandler
{
public:
	static ZDirectShareViaMeetingIDOrPairingCodeHandler& GetInst()
	{
		static ZDirectShareViaMeetingIDOrPairingCodeHandler inst;
		return inst;
	}
	void SetHandler(ZOOM_SDK_NAMESPACE::IDirectShareViaMeetingIDOrPairingCodeHandler *handler)
	{
		m_pHandler = handler;
	}

	ZNSDKError TryWithMeetingNumber(unsigned long long meetingNumber)
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_WRONG_USAGE;
		if (m_pHandler)
			err = m_pHandler->TryWithMeetingNumber(meetingNumber);
		return Map2WrapDefine(err);
	}

	ZNSDKError TryWithPairingCode(ZoomSTRING paringCode)
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_WRONG_USAGE;
		if (m_pHandler)
			err = m_pHandler->TryWithPairingCode(paringCode.c_str());
		return Map2WrapDefine(err);
	}

	ZNSDKError Cancel()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_WRONG_USAGE;
		if (m_pHandler)
			err = m_pHandler->Cancel();
		return Map2WrapDefine(err);
	}
private:
	ZDirectShareViaMeetingIDOrPairingCodeHandler() :m_pHandler(NULL) {}
	ZOOM_SDK_NAMESPACE::IDirectShareViaMeetingIDOrPairingCodeHandler* m_pHandler;
};

class ZDirectShareHelperWrapEvent : public ZOOM_SDK_NAMESPACE::IDirectShareServiceHelperEvent
{
public:
	void SetOwner(ZDirectShareHelperWrap* obj) { owner_ = obj; }
	virtual void OnDirectShareStatusUpdate(ZOOM_SDK_NAMESPACE::DirectShareStatus status, ZOOM_SDK_NAMESPACE::IDirectShareViaMeetingIDOrPairingCodeHandler* handler)
	{

		if (owner_) {
			if ((status == ZOOM_SDK_NAMESPACE::DirectShare_Need_MeetingID_Or_PairingCode) || (status == ZOOM_SDK_NAMESPACE::DirectShare_WrongMeetingID_Or_SharingKey))
			{
				ZDirectShareViaMeetingIDOrPairingCodeHandler::GetInst().SetHandler(handler);
			}
			owner_->OnDirectShareStatusUpdate(Map2WrapDefine(status));
		}
	}
	virtual void OnDirectShareSpecifyContent(ZOOM_SDK_NAMESPACE::IDirectShareSpecifyContentHandler* handler)
	{
	}
private:
	ZDirectShareHelperWrap* owner_;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZoomRetrieveSMSVerificationCodeHandler
{
public:
	static ZoomRetrieveSMSVerificationCodeHandler& GetInst()
	{
		static ZoomRetrieveSMSVerificationCodeHandler inst;
		return inst;
	}
	void SetHandler(IZoomRetrieveSMSVerificationCodeHandler* handler)
	{
		m_pHandler = handler;
	}
	bool Retrieve(const zTCHAR* country_code, const zTCHAR* phone_number)
	{
		bool b_ret = false;
		if (m_pHandler)
		{
			b_ret = m_pHandler->Retrieve(country_code, phone_number);
		}
		return b_ret;
	}
	bool CancelAndLeaveMeeting()
	{
		bool b_ret = false;
		if (m_pHandler)
		{
			b_ret = m_pHandler->CancelAndLeaveMeeting();
		}
		m_pHandler = NULL;
		return b_ret;
	}
private:
	ZoomRetrieveSMSVerificationCodeHandler() :m_pHandler(NULL) {}
	IZoomRetrieveSMSVerificationCodeHandler* m_pHandler;
};
class ZoomVerifySMSVerificationCodeHandler
{
public:
	static ZoomVerifySMSVerificationCodeHandler& GetInst()
	{
		static ZoomVerifySMSVerificationCodeHandler inst;
		return inst;
	}
	void SetHandler(IZoomVerifySMSVerificationCodeHandler* handler)
	{
		m_pHandler = handler;
	}
	bool Verify(const zTCHAR* country_code, const zTCHAR* phone_number, const zTCHAR* verification_code)
	{
		bool b_ret = false;
		if (m_pHandler)
		{
			b_ret = m_pHandler->Verify(country_code, phone_number, verification_code);
		}
		m_pHandler = NULL;
		return b_ret;
	}
	bool CancelAndLeaveMeeting()
	{
		bool b_ret = false;
		if (m_pHandler)
		{
			b_ret = m_pHandler->CancelAndLeaveMeeting();
		}
		m_pHandler = NULL;
		return b_ret;
	}
private:
	ZoomVerifySMSVerificationCodeHandler() :m_pHandler(NULL) {}
	IZoomVerifySMSVerificationCodeHandler* m_pHandler;
};

class ZZoomRealNameAuthMeetingWrapEvent : public IZoomRealNameAuthMeetingEvent
{
public:
	void SetOwner(ZSDKSMSHelperWrap* obj) { owner_ = obj; }
	virtual void onNeedRealNameAuthMeetingNotification(IVector<IZoomRealNameAuthCountryInfo* >* support_country_list, const zTCHAR* privacy_url, IZoomRetrieveSMSVerificationCodeHandler* handler)
	{
		if (owner_ && support_country_list && handler)
		{
			ZoomRetrieveSMSVerificationCodeHandler::GetInst().SetHandler(handler);
			ZNList<ZNZoomRealNameAuthCountryInfo> SMSCountryInfo_list;
			for (int i = 0; i < support_country_list->GetCount(); ++i)
			{
				ZNZoomRealNameAuthCountryInfo countryInfo;
				IZoomRealNameAuthCountryInfo* temp_pCountryInfo = support_country_list->GetItem(i);
				if (temp_pCountryInfo)
				{
					countryInfo.countryID = (const wchar_t*)temp_pCountryInfo->GetCountryID();
					countryInfo.countryName = (const wchar_t*)temp_pCountryInfo->GetCountryName();
					countryInfo.countryCode = (const wchar_t*)temp_pCountryInfo->GetCountryCode();
				}
				SMSCountryInfo_list.push_back(countryInfo);
			}
			ZoomSTRING zn_privacy_url = L"";
			if (privacy_url)
			{
				zn_privacy_url = (const wchar_t*)privacy_url;
			}
			
			owner_->onNeedRealNameAuthMeetingNotification(SMSCountryInfo_list, zn_privacy_url);
		}
	}
	virtual void onRetrieveSMSVerificationCodeResultNotification(SMSVerificationCodeErr result, IZoomVerifySMSVerificationCodeHandler* handler)
	{
		if (owner_)
		{
			if (SMSVerificationCodeErr_Success == result)
			{
				ZoomVerifySMSVerificationCodeHandler::GetInst().SetHandler(handler);
			}
			owner_->onRetrieveSMSVerificationCodeResultNotification(Map2WrapDefine(result));
		}
	}
	virtual void onVerifySMSVerificationCodeResultNotification(SMSVerificationCodeErr result)
	{
		if (owner_)
		{
			owner_->onRetrieveSMSVerificationCodeResultNotification(Map2WrapDefine(result));
		}
	}
private:
	ZSDKSMSHelperWrap* owner_;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZAudioSettingContextWrapEvent : public ZOOM_SDK_NAMESPACE::IAudioSettingContextEvent
{
public:
	void SetOwner(ZSettingAudioWrap* obj) { owner_ = obj; }
	virtual void onComputerMicDeviceChanged(ZOOM_SDK_NAMESPACE::IList<ZOOM_SDK_NAMESPACE::IMicInfo*>* pMicLst)
	{
		if (owner_)
		{
			ZNList<ZNMicInfo> zn_mic_list;
			if (pMicLst && pMicLst->GetCount() > 0)
			{
				int count = pMicLst->GetCount();
				for (int i = 0; i < count; i++)
				{
					ZOOM_SDK_NAMESPACE::IMicInfo* pMicInfo = pMicLst->GetItem(i);
					ZNMicInfo zn_mic_info;
					if (NULL == pMicInfo)
						continue;

					if (pMicInfo->GetDeviceName())
					{
						zn_mic_info.deviceName = pMicInfo->GetDeviceName();
					}
					if (pMicInfo->GetDeviceId())
					{
						zn_mic_info.deviceId = pMicInfo->GetDeviceId();
					}
					zn_mic_info.isSelectedDevice = pMicInfo->IsSelectedDevice();
					zn_mic_list.push_back(zn_mic_info);
				}
			}
			owner_->onComputerMicDeviceChanged(zn_mic_list);
		}
	}
	virtual void onComputerSpeakerDeviceChanged(ZOOM_SDK_NAMESPACE::IList<ZOOM_SDK_NAMESPACE::ISpeakerInfo*>* pSpeakerLst)
	{
		if (owner_)
		{
			ZNList<ZNSpeakerInfo> zn_speaker_list;
			if (pSpeakerLst && pSpeakerLst->GetCount() > 0)
			{
				int count = pSpeakerLst->GetCount();
				for (int i = 0; i < count; i++)
				{
					ZOOM_SDK_NAMESPACE::ISpeakerInfo* pSpeakerInfo = pSpeakerLst->GetItem(i);
					ZNSpeakerInfo zn_speaker_info;
					if (NULL == pSpeakerInfo)
						continue;

					if (pSpeakerInfo->GetDeviceName())
					{
						zn_speaker_info.deviceName = pSpeakerInfo->GetDeviceName();
					}
					if (pSpeakerInfo->GetDeviceId())
					{
						zn_speaker_info.deviceId = pSpeakerInfo->GetDeviceId();
					}
					zn_speaker_info.isSelectedDevice = pSpeakerInfo->IsSelectedDevice();
					zn_speaker_list.push_back(zn_speaker_info);
				}
			}
			owner_->onComputerSpeakerDeviceChanged(zn_speaker_list);
		}
	}
	virtual void onDefaultMicDeviceChanged(const wchar_t* deviceId, const wchar_t* deviceName)
	{
		if (owner_)
		{
			ZoomSTRING zn_deviceId;
			ZoomSTRING zn_deviceName;
			if (deviceId)
			{
				zn_deviceId = deviceId;
			}
			if (deviceName)
			{
				zn_deviceName = deviceName;
			}
			owner_->onDefaultMicDeviceChanged(zn_deviceId, zn_deviceName);
		}
	}
	virtual void onDefaultSpeakerDeviceChanged(const wchar_t* deviceId, const wchar_t* deviceName)
	{
		if (owner_)
		{
			ZoomSTRING zn_deviceId;
			ZoomSTRING zn_deviceName;
			if (deviceId)
			{
				zn_deviceId = deviceId;
			}
			if (deviceName)
			{
				zn_deviceName = deviceName;
			}
			owner_->onDefaultSpeakerDeviceChanged(zn_deviceId, zn_deviceName);
		}
	}
private:
	ZSettingAudioWrap* owner_;
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZVideoSettingContextWrapEvent : public ZOOM_SDK_NAMESPACE::IVideoSettingContextEvent
{
public:
	void SetOwner(ZSettingVideoWrap* obj) { owner_ = obj; }
	virtual void onComputerCamDeviceChanged(ZOOM_SDK_NAMESPACE::IList<ZOOM_SDK_NAMESPACE::ICameraInfo*>* pCameraList)
	{
		if (owner_)
		{
			ZNList<ZNCameraInfo> zn_camera_list;
			if (pCameraList && pCameraList->GetCount() > 0)
			{
				int count = pCameraList->GetCount();
				for (int i = 0; i < count; i++)
				{
					ZOOM_SDK_NAMESPACE::ICameraInfo* pCameraInfo = pCameraList->GetItem(i);
					ZNCameraInfo zn_camera_info;
					if (NULL == pCameraInfo)
						continue;

					if (pCameraInfo->GetDeviceName())
					{
						zn_camera_info.deviceName = pCameraInfo->GetDeviceName();
					}
					if (pCameraInfo->GetDeviceId())
					{
						zn_camera_info.deviceId = pCameraInfo->GetDeviceId();
					}
					zn_camera_info.isSelectedDevice = pCameraInfo->IsSelectedDevice();
					zn_camera_list.push_back(zn_camera_info);
				}
			}
			owner_->onComputerCamDeviceChanged(zn_camera_list);

		}
	}
	virtual void onDefaultCamDeviceChanged(const wchar_t* deviceId, const wchar_t* deviceName)
	{
		if (owner_)
		{
			ZoomSTRING zn_deviceId;
			ZoomSTRING zn_deviceName;
			if (deviceId)
			{
				zn_deviceId = deviceId;
			}
			if (deviceName)
			{
				zn_deviceName = deviceName;
			}
			owner_->onDefaultCamDeviceChanged(zn_deviceId, zn_deviceName);
		}
	}

private:
	ZSettingVideoWrap* owner_;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZRecordingSettingContextWrapEvent : public ZOOM_SDK_NAMESPACE::IRecordingSettingContextEvent
{
public:
	void SetOwner(ZSettingRecordingWrap* obj) { owner_ = obj; }
	virtual void onCloudRecordingStorageInfo(INT64 storage_total_size, INT64 storage_used_size, bool allow_exceed_storage)
	{
		if (owner_)
		{
			owner_->onCloudRecordingStorageInfo(storage_total_size, storage_used_size, allow_exceed_storage);
		}
	}
private:
	ZSettingRecordingWrap* owner_;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZWallpaperSettingContextWrapEvent : public ZOOM_SDK_NAMESPACE::IWallpaperSettingContextEvent
{
public:
	void SetOwner(ZSettingWallpaperWrap* obj) { owner_ = obj; }
	virtual void onMeetingWallpaperChanged(ZOOM_SDK_NAMESPACE::IWallpaperItem* item)
	{
		if (owner_)
		{
			owner_->onMeetingWallpaperChanged(ConvertSDKIWallpaperItemToZNWallpaperItem(item));
		}
	}
	virtual void onMeetingWallpaperImageDownloadStatus(ZOOM_SDK_NAMESPACE::ZoomSDKWallpaperSettingStatus status, const zchar_t* wallpaperId)
	{
		if (owner_)
		{
			ZoomSTRING zn_wallpaperId;
			if (wallpaperId)
			{
				zn_wallpaperId = wallpaperId;
			}
			owner_->onMeetingWallpaperImageDownloadStatus(Map2WrapDefine(status), zn_wallpaperId);
		}
	}
	virtual void onPersonalWallpaperChanged(ZOOM_SDK_NAMESPACE::IWallpaperItem* item)
	{
		if (owner_)
		{
			owner_->onPersonalWallpaperChanged(ConvertSDKIWallpaperItemToZNWallpaperItem(item));
		}
	}
	virtual void onPersonalWallpaperImageDownloadStatus(ZOOM_SDK_NAMESPACE::ZoomSDKWallpaperSettingStatus status, const zchar_t* wallpaperId)
	{
		if (owner_)
		{
			ZoomSTRING zn_wallpaperId;
			if (wallpaperId)
			{
				zn_wallpaperId = wallpaperId;
			}
			owner_->onPersonalWallpaperImageDownloadStatus(Map2WrapDefine(status), zn_wallpaperId);
		}
	}
private:
	ZSettingWallpaperWrap* owner_;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZVirtualBGSettingContextEvent : public ZOOM_SDK_NAMESPACE::IVirtualBGSettingContextEvent
{
public:
	void SetOwner(ZSettingVirtualBGWrap* obj) { owner_ = obj; }
	virtual void onVBImageDidDownloaded()
	{
		if (owner_)
		{
			owner_->onVBImageDidDownloaded();
		}
	}
	virtual void onGreenVBDidUpdateWithReplaceColor(DWORD selectedColor)
	{
		if (owner_)
		{
			ZNZoomSDKColor zn_color;
			zn_color.red   = (selectedColor >> 16) & 0xFF;
			zn_color.green = (selectedColor >> 8)  & 0xFF;
			zn_color.blue  = (selectedColor)       & 0xFF;
			owner_->onGreenVBDidUpdateWithReplaceColor(zn_color);
		}
	}
	virtual void onSelectedVBImageChanged()
	{
		if (owner_)
		{
			owner_->onSelectedVBImageChanged();
		}
	}
	virtual void OnVideoThumbReady(const zchar_t* file_path)
	{
		if (owner_)
		{
			owner_->onVBVideoUploadedResult(true, ZNVB_VideoError_None);
		}
	}
	virtual void OnVideoThumbError(const zchar_t* file_path, ZOOM_SDK_NAMESPACE::VBVideoError error)
	{
		if (owner_)
		{
			owner_->onVBVideoUploadedResult(false, Map2WrapDefine(error));
		}
	}
	virtual void OnVideoPlayError(const zchar_t* file_path, ZOOM_SDK_NAMESPACE::VBVideoError error)
	{
		if (owner_)
		{
			owner_->onVBVideoUploadedResult(false, Map2WrapDefine(error));
		}
	}
private:
	ZSettingVirtualBGWrap* owner_;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZNRequestRawLiveStreamPrivilegeHandler
{
public:
	static ZNRequestRawLiveStreamPrivilegeHandler& GetInst()
	{
		static ZNRequestRawLiveStreamPrivilegeHandler inst;
		return inst;
	}
	void SetHandler(ZOOM_SDK_NAMESPACE::IRequestRawLiveStreamPrivilegeHandler* handler)
	{
		m_pHandler = handler;
	}
	ZoomSTRING GetRequestId()
	{
		ZoomSTRING reqId;
		if (m_pHandler && m_pHandler->GetRequestId())
		{
			reqId = m_pHandler->GetRequestId();
		}
		return reqId;
	}
	unsigned int GetRequesterId()
	{
		unsigned int id = 0xffff;
		if (m_pHandler)
		{
			id = m_pHandler->GetRequesterId();
		}
		return id;
	}
	ZoomSTRING GetRequesterName()
	{
		ZoomSTRING userName;
		if (m_pHandler && m_pHandler->GetRequesterName())
		{
			userName = m_pHandler->GetRequesterName();
		}
		return userName;
	}
	ZoomSTRING GetBroadcastUrl()
	{
		ZoomSTRING userName;
		if (m_pHandler && m_pHandler->GetBroadcastUrl())
		{
			userName = m_pHandler->GetBroadcastUrl();
		}
		return userName;
	}
	ZNSDKError GrantRawLiveStreamPrivilege()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_WRONG_USAGE;
		if (m_pHandler)
			err = m_pHandler->GrantRawLiveStreamPrivilege();
		m_pHandler = NULL;
		return Map2WrapDefine(err);
	}
	ZNSDKError DenyRawLiveStreamPrivilege()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_WRONG_USAGE;
		if (m_pHandler)
			err = m_pHandler->DenyRawLiveStreamPrivilege();
		m_pHandler = NULL;
		return Map2WrapDefine(err);
	}
private:
	ZNRequestRawLiveStreamPrivilegeHandler() :m_pHandler(NULL) {}
	ZOOM_SDK_NAMESPACE::IRequestRawLiveStreamPrivilegeHandler* m_pHandler;
};


class ZMeetingLiveStreamWrapEvent : public ZOOM_SDK_NAMESPACE::IMeetingLiveStreamCtrlEvent
{
public:
	void SetOwner(ZMeetingLiveStreamWrap* obj) { owner_ = obj; }
	virtual void onLiveStreamStatusChange(ZOOM_SDK_NAMESPACE::LiveStreamStatus status)
	{
		if (owner_)
		{
			owner_->onLiveStreamStatusChange(Map2WrapDefine(status));
		}
	}
	virtual void onRawLiveStreamPrivilegeChanged(bool bHasPrivilege)
	{
		if (owner_)
		{
			owner_->onRawLiveStreamPrivilegeChanged(bHasPrivilege);
		}
	}
	virtual void onUserRawLiveStreamPrivilegeChanged(unsigned int userid, bool bHasPrivilege)
	{
		if (owner_)
		{
			owner_->onUserRawLiveStreamPrivilegeChanged(userid, bHasPrivilege);
		}
	}
	virtual void onRawLiveStreamPrivilegeRequested(ZOOM_SDK_NAMESPACE::IRequestRawLiveStreamPrivilegeHandler* handler)
	{
		if (owner_ && handler) {
			ZNRequestRawLiveStreamPrivilegeHandler::GetInst().SetHandler(handler);
			owner_->onRawLiveStreamPrivilegeRequested();
		}
	}
	virtual void onUserRawLiveStreamingStatusChanged(ZOOM_SDK_NAMESPACE::IList<ZOOM_SDK_NAMESPACE::RawLiveStreamInfo>* pLivingList)
	{
		if (owner_)
		{
			ZNList<ZNRawLiveStreamInfo> zn_rawLiveStreamInfolist;
			if (pLivingList && pLivingList->GetCount() > 0)
			{
				int count = pLivingList->GetCount();
				for (int i = 0; i < count; i++)
				{
					ZOOM_SDK_NAMESPACE::RawLiveStreamInfo RawLiveStreamInfo = pLivingList->GetItem(i);
					ZNRawLiveStreamInfo zn_live_stream_info;

					if (RawLiveStreamInfo.broadcastUrl)
					{
						zn_live_stream_info.broadcastUrl = RawLiveStreamInfo.broadcastUrl;
					}
					zn_live_stream_info.userId = RawLiveStreamInfo.userId;
					zn_rawLiveStreamInfolist.push_back(zn_live_stream_info);
				}
			}
			owner_->onUserRawLiveStreamingStatusChanged(zn_rawLiveStreamInfolist);

		}
	}
	virtual void onRawLiveStreamPrivilegeRequestTimeout()
	{
		if (owner_)
		{
			owner_->onRawLiveStreamPrivilegeRequestTimeout();
		}
	}
	virtual void onLiveStreamReminderStatusChanged(bool enable)
	{
		if (owner_)
		{
			owner_->onLiveStreamReminderStatusChanged(enable);
		}
	}
	virtual void onLiveStreamReminderStatusChangeFailed()
	{
		if (owner_)
		{
			owner_->onLiveStreamReminderStatusChangeFailed();
		}
	}
	virtual void onUserThresholdReachedForLiveStream(int percent)
	{
		if (owner_)
		{
			owner_->onUserThresholdReachedForLiveStream(percent);
		}
	}
private:
	ZMeetingLiveStreamWrap* owner_;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZMeetingIndicatorHandler
{
public:
	static ZMeetingIndicatorHandler& GetInst()
	{
		static ZMeetingIndicatorHandler inst;
		return inst;
	}
	void SetHandler(ZOOM_SDK_NAMESPACE::IMeetingIndicatorHandler *handler)
	{
		m_pHandler = handler;
	}

	ZoomSTRING GetIndicatorItemId()
	{
		ZoomSTRING zn_id;
		if (m_pHandler)
		{
			const zchar_t* id = m_pHandler->GetIndicatorItemId();
			if (id)
			{
				zn_id = id;
			}
		}
		return zn_id;
	}

	ZoomSTRING GetIndicatorName()
	{
		ZoomSTRING zn_name;
		if (m_pHandler)
		{
			const zchar_t* name = m_pHandler->GetIndicatorName();
			if (name)
			{
				zn_name = name;
			}
		}
		return zn_name;
	}

	ZoomSTRING GetIndicatorIcon()
	{
		ZoomSTRING zn_icon;
		if (m_pHandler)
		{
			const zchar_t* icon = m_pHandler->GetIndicatorIcon();
			if (icon)
			{
				zn_icon = icon;
			}
		}
		return zn_icon;
	}

	ZNSDKError ShowIndicatorPanel(unsigned int x, unsigned int y)
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_WRONG_USAGE;
		if (m_pHandler)
		{
			m_pHandler->ShowIndicatorPanel(x, y);
			err = ZOOM_SDK_NAMESPACE::SDKERR_SUCCESS;
		}
		return Map2WrapDefine(err);
	}

	ZNSDKError HideIndicatorPanel()
	{
		ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::SDKERR_WRONG_USAGE;
		if (m_pHandler)
		{
			m_pHandler->HideIndicatorPanel();
			err = ZOOM_SDK_NAMESPACE::SDKERR_SUCCESS;
		}
		return Map2WrapDefine(err);
	}
private:
	ZMeetingIndicatorHandler() :m_pHandler(NULL) {}
	ZOOM_SDK_NAMESPACE::IMeetingIndicatorHandler* m_pHandler;
};

class ZMeetingIndicatorCtrlEvent : public ZOOM_SDK_NAMESPACE::IMeetingIndicatorCtrlEvent 
{
public:
	void SetOwner(ZMeetingIndicatorWrap* obj) { owner_ = obj; }
	virtual void onIndicatorItemReceived(ZOOM_SDK_NAMESPACE::IMeetingIndicatorHandler* handler)
	{
		if (owner_)
		{
			ZMeetingIndicatorHandler::GetInst().SetHandler(handler);
			owner_->onIndicatorItemReceived();
		}
	}
	virtual void onIndicatorItemRemoved(ZOOM_SDK_NAMESPACE::IMeetingIndicatorHandler* handler)
	{
		if (owner_)
		{
			ZMeetingIndicatorHandler::GetInst().SetHandler(NULL);
			owner_->onIndicatorItemRemoved();
		}
	}
private:
	ZMeetingIndicatorWrap* owner_;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZMeetingWebinarCtrlEvent : public ZOOM_SDK_NAMESPACE::IMeetingWebinarCtrlEvent
{
public:
	void SetOwner(ZMeetingWebinarWrap* obj) { owner_ = obj; }
	virtual void onPromptAttendee2PanelistResult(int result)
	{
		if (owner_) {
			owner_->onPromptAttendee2PanelistResult(result);
		}
	}
	virtual void onDepromptPanelist2AttendeeResult(int result)
	{
		if (owner_) {
			owner_->onDepromptPanelist2AttendeeResult(result);
		}
	}
	virtual void onAllowPanelistStartVideoNotification()
	{
		if (owner_) {
			owner_->onAllowPanelistStartVideoNotification();
		}
	}
	virtual void onDisallowPanelistStartVideoNotification()
	{
		if (owner_) {
			owner_->onDisallowPanelistStartVideoNotification();
		}
	}
	virtual void onSelfAllowTalkNotification()
	{
		if (owner_) {
			owner_->onSelfAllowTalkNotification();
		}
	}
	virtual void onSelfDisallowTalkNotification()
	{
		if (owner_) {
			owner_->onSelfDisallowTalkNotification();
		}
	}
	virtual void onAllowAttendeeChatNotification()
	{
		if (owner_) {
			owner_->onAllowAttendeeChatNotification();
		}
	}
	virtual void onDisallowAttendeeChatNotification()
	{
		if (owner_) {
			owner_->onDisallowAttendeeChatNotification();
		}
	}
	virtual void onAllowWebinarReactionStatusChanged(bool can_reaction)
	{
		if (owner_) {
			owner_->onAllowWebinarReactionStatusChanged(can_reaction);
		}
	}
	virtual void onAllowAttendeeRaiseHandStatusChanged(bool can_raiseHand)
	{
		if (owner_) {
			owner_->onAllowAttendeeRaiseHandStatusChanged(can_raiseHand);
		}
	}
	virtual void onAllowAttendeeViewTheParticipantCountStatusChanged(bool can_viewParticipantCount)
	{
		if (owner_) {
			owner_->onAllowAttendeeViewTheParticipantCountStatusChanged(can_viewParticipantCount);
		}
	}
	virtual void onAttendeeAudioStatusNotification(unsigned int userid, bool can_talk, bool is_muted)
	{
		if (owner_) {
			owner_->onAttendeeAudioStatusNotification(userid, can_talk, is_muted);
		}
	}
	virtual void onAttendeePromoteConfirmResult(bool agree, unsigned int userid)
	{
		if (owner_) {
			owner_->onAttendeePromoteConfirmResult(agree, userid);
		}
	}
private:
	ZMeetingWebinarWrap* owner_;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZMeetingWhiteboardCtrlEvent : public ZOOM_SDK_NAMESPACE::IMeetingWhiteboardCtrlEvent
{
public:
	void SetOwner(ZMeetingWhiteboardWrap* obj) { owner_ = obj; }
	virtual void onWhiteboardStatusChanged(ZOOM_SDK_NAMESPACE::SDKWhiteboardStatus status)
	{
		if (owner_) {
			owner_->onWhiteboardStatusChanged(Map2WrapDefine(status));
		}
	}
	virtual void onWhiteboardSettingsChanged(ZOOM_SDK_NAMESPACE::SDKWhiteboardShareOption shareOption, ZOOM_SDK_NAMESPACE::SDKWhiteboardCreateOption createOption, bool bEnable)
	{
		if (owner_) {
			owner_->onWhiteboardSettingsChanged(Map2WrapDefine(shareOption), Map2WrapDefine(createOption), bEnable);
		}
	}
private:
	ZMeetingWhiteboardWrap* owner_;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZMeetingDocsCtrlEvent : public ZOOM_SDK_NAMESPACE::IMeetingDocsCtrlEvent
{
public:
	void SetOwner(ZMeetingDocsWrap* obj) { owner_ = obj; }
	virtual void onDocsSharingSourceInfoChanged(ZOOM_SDK_NAMESPACE::IZoomSDKDocSharingSourceInfo* pInfo)
	{
		if (owner_)
		{
			owner_->onDocsSharingSourceInfoChanged(ConvertSDKZoomSDKDocSharingSourceInfoToZNZoomSDKDocSharingSourceInfo(pInfo));
		}
	}
	virtual void onDocsPermissionChanged(ZOOM_SDK_NAMESPACE::SDKDocsCreateOption createOption, ZOOM_SDK_NAMESPACE::SDKDocsShareOption option)
	{
		if (owner_)
		{
			owner_->onDocsPermissionChanged(Map2WrapDefine(createOption), Map2WrapDefine(option));
		}
	}
private:
	ZMeetingDocsWrap* owner_;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SDKEventWrapMgr
{
public:
	//auth_service_cb
	ZAuthServiceWrapEvent m_authServiceWrapEvent;

	//meeting_service_cb
	ZMeetingServiceWrapEvent m_meetingServiceWrapEvent;

	//meeting_ui_ctrl_cb
	ZMeetingUIControllerWrapEvent m_meetingUICtrlWrapEvent;

	//meeting_audio_ctrl_cb
	ZMeetingAudioCtrlWrapEvent m_meetingAudioCtrlWrapEvent;
	
	//meeting_video_ctrl_cb
	ZMeetingVideoCtrlWrapEvent m_meetingVideoCtrlWrapEvent;

	//meeting_share_ctrl_cb
	ZMeetingShareCtrlWrapEvent m_meetingShareCtrlWrapEvent;

	//meeting_annotation_ctrl_cb
	ZMeetingAnnotationCtrlWrapEvent m_meetingAnnotationCtrlWrapEvent;

	//meeting_recording_ctrl_cb
	ZMeetingRecordingCtrlWrapEvent m_meetingRecordingCtrlWrapEvent;

	//meeting_participants_ctrl_cb
	ZMeetingParticipantsCtrlEvent m_meetingParticipantsCtrlEvent;

	//meeting_h323_ctrl_cb
	ZMeetingH323CtrlWrapEvent m_meetingH323CtrlWrapEvent;

	//meeting_config_ctrl_cb
	ZMeetingConfigCtrlWrapFreeMeetingEvent m_meetingConfigCtrlWrapEvent;

	//meeting_reminder_ctrl_cb
	ZMeetingReminderCtrlEvent m_meetingReminderCtrlEvent;

	//meeting_chat_ctrl_cb
	ZMeetingChatCtrlEvent m_meetingChatCtrlEvent;

	//meeting_waiting_room_ctrl_cb
	ZMeetingWaitingRoomCtrlEvent m_meetingWaitingRoomCtrlEvent;

	//meeting_ai_companion_ctrl_cb
	ZMeetingAICompanionCtrlEvent m_meetingAICompanionCtrlEvent;
	ZMeetingAICompanionSmartSummaryEvent m_meetingAICompanionSmartSummaryEvent;
	ZMeetingAICompanionQueryEvent m_meetingAICompanionQueryEvent;

	//direct_share_helper_cb
	ZDirectShareHelperWrapEvent m_directShareHelperWrapEvent;

	//sms_helper_cb
	ZZoomRealNameAuthMeetingWrapEvent m_zoomRealNameAuthMeetingWrapEvent;

	//audio_setting_context_cb
	ZAudioSettingContextWrapEvent m_audioSettingContextWrapEvent;

	//video_setting_context_cb
	ZVideoSettingContextWrapEvent m_videoSettingContextWrapEvent;

	//recording_setting_context_cb
	ZRecordingSettingContextWrapEvent m_recordingSettingContextWrapEvent;

	//wallpaper_setting_context_cb
	ZWallpaperSettingContextWrapEvent m_wallpaperSettingContextWrapEvent;

	//virtual_bg_setting_context_cb
	ZVirtualBGSettingContextEvent m_virtualBGSettingContextEvent;

	//meeting_live_stream_ctrl_cb
	ZMeetingLiveStreamWrapEvent m_meetingLiveStreamWrapEvent;

	//meeting_indicator_ctrl_cb
	ZMeetingIndicatorCtrlEvent m_meetingIndicatorCtrlEvent;

	//meeting_webinar_ctrl_cb
	ZMeetingWebinarCtrlEvent m_meetingWebinarCtrlEvent;

	// meeting_qa_ctrl_cb
	ZMeetingQACtrlEvent m_meetingQACtrlEvent;

	////meeting_whiteboard_ctrl_cb
	ZMeetingWhiteboardCtrlEvent m_meetingWhiteboardCtrlEvent;

	//meeting_docs_ctrl_cb
	ZMeetingDocsCtrlEvent m_meetingDocsCtrlEvent;

	// emoji_reaction_ctrl_cb
	ZMeetingEmojiReactionControllerEvent m_emojiReactionControllerEvent;

	// close_caption_ctrl_cb
	ZMeetingClosedCaptionControllerEvent m_closeCaptionControllerEvent;

	// meeting_polling_ctrl_cb
	ZMeetingPollingCtrlEvent m_meetingPollingCtrlEvent;

	static SDKEventWrapMgr& GetInst()
	{
		static SDKEventWrapMgr inst;
		return inst;
	}
	~SDKEventWrapMgr() {}
private:
	SDKEventWrapMgr() {}
};
#endif // !_sdk_sinks_wrap_class_h_
