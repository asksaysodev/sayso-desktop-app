#pragma once
#include "zoom_native_sdk_wrap_core_def.h"
#include "meeting_annotation_wrap_core.h"
#include "meeting_audio_wrap_core.h"
#include "meeting_video_wrap_core.h"
#include "meeting_recording_wrap_core.h"
#include "meeting_participants_wrap_core.h"
#include "meeting_share_wrap_core.h"
#include "meeting_h323_wrap_core.h"
#include "meeting_config_wrap_core.h"
#include "meeting_AAN_wrap_core.h"
#include "meeting_raw_archiving_wrap_core.h"
#include "meeting_live_stream_wrap_core.h"
#include "meeting_reminder_wrap_core.h"
#include "meeting_chat_wrap_core.h"
#include "meeting_waiting_room_wrap_core.h"
#include "meeting_ai_companion_wrap_core.h"
#include "meeting_indicator_wrap_core.h"
#include "meeting_webinar_wrap_core.h"
#include "meeting_qa_wrap_core.h"
#include "meeting_reaction_wrap_core.h"
#include "meeting_close_caption_wrap_core.h"
#include "meeting_polling_wrap_core.h"
#include "meeting_whiteboard_wrap_core.h"
#include "meeting_docs_wrap_core.h"
#include "zoom_sdk_sms_helper_wrap_core.h"
#include "zoom_sinks_wrap_class.h"

class ZMeetingInfoWrap
{
public:
	ZMeetingInfoWrap(){}
	virtual ~ZMeetingInfoWrap(){}

	ZoomSTRING GetMeetingTopic();
	ZNMeetingType GetMeetingType();
	unsigned long long GetMeetingNumber();
	ZoomSTRING GetMeetingID();
	ZoomSTRING GetInviteEmailTemplate();
	ZoomSTRING GetInviteEmailTitle();
	ZoomSTRING GetJoinMeetingUrl();
	ZoomSTRING GetMeetingHostTag();
};
class ZMeetingUICtrlWrap
{
public:
	ZMeetingUICtrlWrap();
	virtual ~ZMeetingUICtrlWrap();
	void Init();
	void Uninit();
	void SetSink(ZNativeSDKMeetingUICtrlWrapSink* pSink);

	ZNSDKError ShowChatDlg(ZNShowChatDlgParam showChatDlgParam);
	ZNSDKError HideChatDlg();
	ZNSDKError EnterFullScreen(bool bFirstView, bool bSecView);
	ZNSDKError ExitFullScreen(bool bFirstView, bool bSecView);
	ZNSDKError SwitchToVideoWall();
	ZNSDKError SwitchToActiveSpeaker();
	ZNSDKError MoveFloatVideoWnd(ZoomSTRING left, ZoomSTRING top);
	ZNSDKError ShowSharingToolbar(bool bShow);
	ZNSDKError SwitchFloatVideoToActiveSpkMod();
	ZNSDKError ChangeFloatToActiveSpkVideoSize(ZNSDKFloatVideoType type);
	ZNSDKError SwitchFloatVideoToGalleryMod();
	ZNSDKError ShowParticipantsListWnd(bool bShow);
	ZNSDKError ShowBottomFloatToolbarWnd(bool bShow);
	ZNSDKError ShowJoinAudioDlg();
	ZNSDKError HideJoinAudioDlg();
	ZNSDKError GetWallViewPageInfo(ZNVideoWallPageInfoParam& param);
	ZNSDKError ShowPreOrNextPageVideo(bool bPageUP);
	ZNSDKError ShowSharingFrameWindows(bool bShow);
	ZNSDKError GetCurrentSplitScreenModeInfo(ZNSplitScreenInfo& info);
	ZNSDKError SwitchSplitScreenMode(bool bSplit);

	ZNSDKError BackToMeeting();
	ZNSDKError GetMeetingUIWnd(ZoomSTRING& hFirstView, ZoomSTRING& hSecondView);
	ZNSDKError SwitchMinimizeUIMode4FirstScreenMeetingUIWnd(ZNSDKMinimizeUIMode mode);
	bool IsMinimizeModeOfFirstScreenMeetingUIWnd(ZNSDKMinimizeUIMode& mode);
	ZNSDKError SwapToShowShareViewOrVideo(bool bToDisplayShare);
	ZNSDKError IsDisplayingShareViewOrVideo(bool& bIsShare);
	ZNSDKError CanSwapToShowShareViewOrVideo(bool& bCan);
	ZNSDKError SetMeetingTopic(ZoomSTRING meetingtopic);
	ZNSDKError SetCustomizedCloudRecordingMgrUrl(ZoomSTRING crmURL);
	ZNSDKError SetCustomizedInvitationDomain(ZoomSTRING invitation_domain);
	ZNSDKError SetCustomizedPollingUrl(ZoomSTRING pollingURL, bool bCreate);

	//callback
	void onInviteBtnClicked(bool& bHandled);

	void onStartShareBtnClicked();
	void onEndMeetingBtnClicked();
	void onParticipantListBtnClicked();
	void onCustomLiveStreamMenuClicked();
	void onZoomInviteDialogFailed();
	void onCCBTNClicked();
	void onAudioBtnClicked(ZNAudioBtnClickedCallbackInfo info);
	void onAudioMenuBtnClicked();
private:
	ZNativeSDKMeetingUICtrlWrapSink* m_pSink;
};
class ZMeetingServiceWrap
{
public:
	ZMeetingServiceWrap();
	virtual ~ZMeetingServiceWrap();
	void Init();
	void Uninit();
	void SetSink(ZNativeSDKMeetingWrapSink* pSink);
	
	ZNSDKError Start(ZNStartParam startParam);
	ZNSDKError Start_WithoutLogin(ZNStartParam startParam);
	ZNSDKError Join(ZNJoinParam joinParam);
	ZNSDKError Join_WithoutLogin(ZNJoinParam joinParam);
	ZNSDKError Leave(ZNLeaveMeetingCmd cmd);
	ZNSDKError Lock();
	ZNSDKError Unlock();
	ZNSDKError HandleZoomWebUriProtocolAction(ZoomSTRING protocol_action);
	bool CanSetMeetingTopic();
	ZNSDKError SetMeetingTopic(ZoomSTRING sTopic);

	ZMeetingInfoWrap& GetMeetingInfo();
	ZMeetingUICtrlWrap& GetMeetingUICtrl();
	ZMeetingAnnotationWrap& GetMeetingAnnotation();
	ZMeetingAudioWrap& GetMeetingAudioCtrl();
	ZMeetingVideoWrap& GetMeetingVideoCtrl();
	ZMeetingRecordingWrap& GetMeetingRecordingCtrl();
    ZMeetingParticipantsWrap& GetMeetingParticipantsCtrl();
	ZMeetingShareWrap& GetMeetingShareCtrl();
	ZMeetingH323Wrap& GetMeetingH323Ctrl();
	ZMeetingConfigWrap& GetMeetingConfigCtrl();
	ZSDKSMSHelperWrap& GetSDKSMSHelper();
	ZMeetingAANWrap& GetMeetingAANCtrl();
	ZMeetingRawArchivingWrap& GetMeetingRawArchivingCtrl();
	ZMeetingLiveStreamWrap& GetMeetingLiveStreamCtrl();
	ZMeetingReminderWrap& GetMeetingReminderCtrl();
	ZMeetingChatWrap& GetMeetingChatCtrl();
	ZMeetingWaitingRoomWrap& GetMeetingWaitingRoomCtrl();
	ZMeetingAICompanionWrap& GetMeetingAICompanionCtrl();
	ZMeetingIndicatorWrap& GetMeetingIndicatorCtrl();
	ZMeetingWebinarWrap& GetMeetingWebinarCtrl();
	ZMeetingQAWrap& GetMeetingQACtrl();
	ZMeetingReactionWrap& GetMeetingReactionCtrl();
	ZMeetingCloseCaptionWrap& GetMeetingCloseCaptionCtrl();
	ZMeetingPollingWrap& GetMeetingPollingCtrl();
	ZMeetingWhiteboardWrap& GetMeetingWhiteboardCtrl();
	ZMeetingDocsWrap& GetMeetingDocsCtrl();
	ZRequestRawLiveStreamPrivilegeHandlerWrap& GetRequestRawLiveStreamPrivilegeHandler();

	ZNConnectionQuality GetSharingConnQuality();
	ZNConnectionQuality GetVideoConnQuality();
	ZNConnectionQuality GetAudioConnQuality();

	//callback
	void onMeetingStatusChanged(ZNMeetingStatus meetingStatus, int iResult);
	void onMeetingStatisticsWarningNotification(ZNStatisticsWarningType type);
	void onMeetingParameterNotification(ZNMeetingParameter parameter);
	void onMeetingTopicChanged(ZoomSTRING sTopic);
	void onMeetingFullToWatchLiveStream(ZoomSTRING sLiveStreamUrl);
private:
	ZNativeSDKMeetingWrapSink* m_pSink;
	ZMeetingInfoWrap m_meeting_info;
	ZMeetingUICtrlWrap m_meeting_ui_ctrl;
	ZMeetingAnnotationWrap m_meeting_annotation;
	ZMeetingAudioWrap m_meeting_audio_ctrl;
	ZMeetingVideoWrap m_meeting_video_ctrl;
	ZMeetingRecordingWrap m_meeting_recording_ctrl;
	ZMeetingParticipantsWrap m_meeting_participants_ctrl;
	ZMeetingShareWrap m_meeting_share_ctrl;
	ZMeetingH323Wrap m_meeting_h323_ctrl;
	ZMeetingConfigWrap m_meeting_config_ctrl;
	ZSDKSMSHelperWrap m_sdk_sms_helper;
	ZMeetingAANWrap m_meeting_AAN_ctrl;
	ZMeetingRawArchivingWrap m_meeting_raw_archiving_ctrl;
	ZMeetingLiveStreamWrap m_meeting_live_stream_ctrl;
	ZMeetingReminderWrap m_meeting_reminder_ctrl;
	ZMeetingChatWrap m_meeting_chat_ctrl;
	ZMeetingWaitingRoomWrap m_meeting_waiting_room_ctrl;
	ZMeetingAICompanionWrap m_meeting_ai_companion_ctrl;
	ZMeetingIndicatorWrap m_meeting_indicator_ctrl;
	ZMeetingWebinarWrap m_meeting_webinar_ctrl;
	ZMeetingQAWrap m_meeting_qa_ctrl;
	ZMeetingReactionWrap m_meeting_reaction_ctrl;
	ZMeetingCloseCaptionWrap m_close_caption_ctrl;
	ZMeetingPollingWrap m_polling_ctrl;
	ZMeetingWhiteboardWrap m_meeting_whiteboard_ctrl;
	ZMeetingDocsWrap m_meeting_docs_ctrl;
	ZRequestRawLiveStreamPrivilegeHandlerWrap m_meeting_request_raw_live_stream_privilege_handler;
};
