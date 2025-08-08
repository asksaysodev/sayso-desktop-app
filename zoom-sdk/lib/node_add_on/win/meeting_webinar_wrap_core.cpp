#include "meeting_webinar_wrap_core.h"
#include "sdk_wrap.h"
#include "meeting_service_components_wrap/meeting_webinar_ctrl_wrap.h"
#include "zoom_native_to_wrap.h"
#include "sdk_events_wrap_class.h"

extern ZOOM_SDK_NAMESPACE::IMeetingServiceWrap& g_meeting_service_wrap;

ZMeetingWebinarWrap::ZMeetingWebinarWrap()
{
	SDKEventWrapMgr::GetInst().m_meetingWebinarCtrlEvent.SetOwner(this);
	m_pSink = NULL;
}
ZMeetingWebinarWrap::~ZMeetingWebinarWrap()
{
	Uninit();
	m_pSink = NULL;
	SDKEventWrapMgr::GetInst().m_meetingWebinarCtrlEvent.SetOwner(NULL);
}
void ZMeetingWebinarWrap::Init()
{
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWebinarController().Init_Wrap(&g_meeting_service_wrap);
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWebinarController().SetEvent(&SDKEventWrapMgr::GetInst().m_meetingWebinarCtrlEvent);
}
void ZMeetingWebinarWrap::Uninit()
{
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWebinarController().SetEvent(NULL);
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWebinarController().Uninit_Wrap();
}
void ZMeetingWebinarWrap::SetSink(ZNativeSDKMeetingWebinarWrapSink* pSink)
{
	m_pSink = pSink;
}
ZNSDKError ZMeetingWebinarWrap::PromptAttendee2Panelist(unsigned int userid)
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN;
	sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWebinarController().PromptAttendee2Panelist(userid);
	return Map2WrapDefine(sdkerr);
}
ZNSDKError ZMeetingWebinarWrap::DepromptPanelist2Attendee(unsigned int userid)
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN;
	sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWebinarController().DepromptPanelist2Attendee(userid);
	return Map2WrapDefine(sdkerr);
}
bool ZMeetingWebinarWrap::IsSupportAttendeeTalk()
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN;
	sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWebinarController().IsSupportAttendeeTalk();
	if (sdkerr == ZOOM_SDK_NAMESPACE::SDKERR_SUCCESS) {
		return true;
	} else {
		return false;
	}
}
ZNSDKError ZMeetingWebinarWrap::AllowAttendeeTalk(unsigned int userid)
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN;
	sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWebinarController().AllowAttendeeTalk(userid);
	return Map2WrapDefine(sdkerr);
}
ZNSDKError ZMeetingWebinarWrap::DisallowAttendeeTalk(unsigned int userid)
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN;
	sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWebinarController().DisallowAttendeeTalk(userid);
	return Map2WrapDefine(sdkerr);
}
ZNSDKError ZMeetingWebinarWrap::AllowPanelistStartVideo()
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN;
	sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWebinarController().AllowPanelistStartVideo();
	return Map2WrapDefine(sdkerr);
}
ZNSDKError ZMeetingWebinarWrap::DisallowPanelistStartVideo()
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN;
	sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWebinarController().DisallowPanelistStartVideo();
	return Map2WrapDefine(sdkerr);
}
ZNSDKError ZMeetingWebinarWrap::AllowWebinarEmojiReaction()
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN;
	sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWebinarController().AllowWebinarEmojiReaction();
	return Map2WrapDefine(sdkerr);
}
ZNSDKError ZMeetingWebinarWrap::DisallowWebinarEmojiReaction()
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN;
	sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWebinarController().DisallowWebinarEmojiReaction();
	return Map2WrapDefine(sdkerr);
}
bool ZMeetingWebinarWrap::IsWebinarEmojiReactionSupported()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWebinarController().IsWebinarEmojiReactionSupported();
}
ZNSDKError ZMeetingWebinarWrap::AllowAttendeeRaiseHand()
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN;
	sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWebinarController().AllowAttendeeRaiseHand();
	return Map2WrapDefine(sdkerr);
}
ZNSDKError ZMeetingWebinarWrap::DisallowAttendeeRaiseHand()
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN;
	sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWebinarController().DisallowAttendeeRaiseHand();
	return Map2WrapDefine(sdkerr);
}
ZNSDKError ZMeetingWebinarWrap::AllowAttendeeViewTheParticipantCount()
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN;
	sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWebinarController().AllowAttendeeViewTheParticipantCount();
	return Map2WrapDefine(sdkerr);
}
ZNSDKError ZMeetingWebinarWrap::DisallowAttendeeViewTheParticipantCount()
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN;
	sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWebinarController().DisallowAttendeeViewTheParticipantCount();
	return Map2WrapDefine(sdkerr);
}
int ZMeetingWebinarWrap::GetParticipantCount()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWebinarController().GetParticipantCount();
}
ZNWebinarMeetingStatus ZMeetingWebinarWrap::GetWebinarMeetingStatus()
{
	ZNWebinarMeetingStatus zn_webinarMeetingStatus;
	ZOOM_SDK_NAMESPACE::WebinarMeetingStatus* webinarMeetingStatus = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWebinarController().GetWebinarMeetingStatus();
	if (webinarMeetingStatus)
	{
		zn_webinarMeetingStatus.allow_panellist_start_video = webinarMeetingStatus->allow_panellist_start_video;
		zn_webinarMeetingStatus.allow_attendee_chat = webinarMeetingStatus->allow_attendee_chat;
		zn_webinarMeetingStatus.allow_emoji_reaction = webinarMeetingStatus->allow_emoji_reaction;
		zn_webinarMeetingStatus.allow_attendee_raise_hand = webinarMeetingStatus->allow_attendee_raise_hand;
		zn_webinarMeetingStatus.allow_attendee_view_participant_count = webinarMeetingStatus->allow_attendee_view_participant_count;
	}
	return zn_webinarMeetingStatus;
}
ZNSDKError ZMeetingWebinarWrap::SetPanelistChatPrivilege(ZNSDKPanelistChatPrivilege privilege)
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN;
	sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWebinarController().SetPanelistChatPrivilege(Map2WrapDefine(privilege));
	return Map2WrapDefine(sdkerr);
}
ZNSDKError ZMeetingWebinarWrap::GetPanelistChatPrivilege(ZNSDKPanelistChatPrivilege& zn_privilege)
{
	ZOOM_SDK_NAMESPACE::SDKPanelistChatPrivilege privilege;
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWebinarController().GetPanelistChatPrivilege(privilege);
	zn_privilege = Map2WrapDefine(privilege);
	return Map2WrapDefine(sdkerr); 
}
ZNSDKError ZMeetingWebinarWrap::SetAttendeeViewMode(ZNSDKAttendeeViewMode mode)
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN;
	sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWebinarController().SetAttendeeViewMode(Map2WrapDefine(mode));
	return Map2WrapDefine(sdkerr);
}
ZNSDKAttendeeViewMode ZMeetingWebinarWrap::GetAttendeeViewMode()
{
	ZOOM_SDK_NAMESPACE::SDKAttendeeViewMode sdkMode = ZOOM_SDK_NAMESPACE::SDKAttendeeViewMode_None;
	sdkMode = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWebinarController().GetAttendeeViewMode();
	return Map2WrapDefine(sdkMode);
}
ZoomSTRING ZMeetingWebinarWrap::GetWebinarLegalNoticesPrompt()
{
	const zchar_t* prompt = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWebinarController().getWebinarLegalNoticesPrompt();
	ZoomSTRING zn_prompt;
	if (prompt)
	{
		zn_prompt = prompt;
	}
	return zn_prompt;
}
bool ZMeetingWebinarWrap::GetWebinarLegalNoticesExplained(ZNWebinarLegalNoticesExplainedInfo& zn_explained_info)
{
	ZOOM_SDK_NAMESPACE::WebinarLegalNoticesExplainedInfo param;
	bool result = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWebinarController().getWebinarLegalNoticesExplained(param);
	if (param.explained_content)
	{
		zn_explained_info.explained_content = param.explained_content;
	}
	if (param.url_register_account_owner)
	{
		zn_explained_info.url_register_account_owner = param.url_register_account_owner;
	}
	if (param.url_register_terms)
	{
		zn_explained_info.url_register_terms = param.url_register_terms;
	}
	if (param.url_register_privacy_policy)
	{
		zn_explained_info.url_register_privacy_policy = param.url_register_privacy_policy;
	}
	return result;
}
void ZMeetingWebinarWrap::onPromptAttendee2PanelistResult(int result)
{
	if (m_pSink)
	{
		m_pSink->onPromptAttendee2PanelistResult(result);
	}
}
void ZMeetingWebinarWrap::onDepromptPanelist2AttendeeResult(int result)
{
	if (m_pSink)
	{
		m_pSink->onDepromptPanelist2AttendeeResult(result);
	}
}
void ZMeetingWebinarWrap::onAllowPanelistStartVideoNotification()
{
	if (m_pSink)
	{
		m_pSink->onAllowPanelistStartVideoNotification();
	}
}
void ZMeetingWebinarWrap::onDisallowPanelistStartVideoNotification()
{
	if (m_pSink)
	{
		m_pSink->onDisallowPanelistStartVideoNotification();
	}
}
void ZMeetingWebinarWrap::onSelfAllowTalkNotification()
{
	if (m_pSink)
	{
		m_pSink->onSelfAllowTalkNotification();
	}
}
void ZMeetingWebinarWrap::onSelfDisallowTalkNotification()
{
	if (m_pSink)
	{
		m_pSink->onSelfDisallowTalkNotification();
	}
}
void ZMeetingWebinarWrap::onAllowAttendeeChatNotification()
{
	if (m_pSink)
	{
		m_pSink->onAllowAttendeeChatNotification();
	}
}
void ZMeetingWebinarWrap::onDisallowAttendeeChatNotification()
{
	if (m_pSink)
	{
		m_pSink->onDisallowAttendeeChatNotification();
	}
}
void ZMeetingWebinarWrap::onAllowWebinarReactionStatusChanged(bool can_reaction)
{
	if (m_pSink)
	{
		m_pSink->onAllowWebinarReactionStatusChanged(can_reaction);
	}
}
void ZMeetingWebinarWrap::onAllowAttendeeRaiseHandStatusChanged(bool can_raiseHand)
{
	if (m_pSink)
	{
		m_pSink->onAllowAttendeeRaiseHandStatusChanged(can_raiseHand);
	}
}
void ZMeetingWebinarWrap::onAllowAttendeeViewTheParticipantCountStatusChanged(bool can_viewParticipantCount)
{
	if (m_pSink)
	{
		m_pSink->onAllowAttendeeViewTheParticipantCountStatusChanged(can_viewParticipantCount);
	}
}
void ZMeetingWebinarWrap::onAttendeeAudioStatusNotification(unsigned int userid, bool can_talk, bool is_muted)
{
	if (m_pSink)
	{
		m_pSink->onAttendeeAudioStatusNotification(userid, can_talk, is_muted);
	}
}
void ZMeetingWebinarWrap::onAttendeePromoteConfirmResult(bool agree, unsigned int userid)
{
	if (m_pSink)
	{
		m_pSink->onAttendeePromoteConfirmResult(agree, userid);
	}
}