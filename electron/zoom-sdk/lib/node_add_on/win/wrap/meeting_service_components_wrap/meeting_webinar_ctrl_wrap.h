#pragma once
#include "common_include.h"
BEGIN_ZOOM_SDK_NAMESPACE
class IMeetingServiceWrap;
IMeetingWebinarController* InitIMeetingWebinarControllerFunc(IMeetingWebinarCtrlEvent* pEvent, IMeetingServiceWrap* pOwner);
void UninitIMeetingWebinarControllerFunc(IMeetingWebinarController* obj);
BEGIN_CLASS_DEFINE_WITHCALLBACK(IMeetingWebinarController, IMeetingWebinarCtrlEvent)
NORMAL_CLASS(IMeetingWebinarController)
INIT_UNINIT_WITHEVENT_AND_OWNSERVICE(IMeetingWebinarController, IMeetingServiceWrap)
virtual SDKError SetEvent(IMeetingWebinarCtrlEvent* pEvent)
{
	external_cb = pEvent;
	return SDKERR_SUCCESS;
}

//virtual SDKError PromptAttendee2Panelist(unsigned int userid) = 0;
DEFINE_FUNC_1(PromptAttendee2Panelist, SDKError, unsigned int, userid)
//virtual SDKError DepromptPanelist2Attendee(unsigned int userid) = 0;
DEFINE_FUNC_1(DepromptPanelist2Attendee, SDKError, unsigned int, userid)
//virtual SDKError IsSupportAttendeeTalk() = 0;
DEFINE_FUNC_0(IsSupportAttendeeTalk, SDKError)
//virtual SDKError AllowAttendeeTalk(unsigned int userid) = 0;
DEFINE_FUNC_1(AllowAttendeeTalk, SDKError, unsigned int, userid)
//virtual SDKError DisallowAttendeeTalk(unsigned int userid) = 0;
DEFINE_FUNC_1(DisallowAttendeeTalk, SDKError, unsigned int, userid)
//virtual SDKError AllowPanelistStartVideo() = 0;
DEFINE_FUNC_0(AllowPanelistStartVideo, SDKError)
//virtual SDKError DisallowPanelistStartVideo() = 0;
DEFINE_FUNC_0(DisallowPanelistStartVideo, SDKError)
//virtual SDKError AllowWebinarEmojiReaction() = 0;
DEFINE_FUNC_0(AllowWebinarEmojiReaction, SDKError)
//virtual SDKError DisallowWebinarEmojiReaction() = 0;
DEFINE_FUNC_0(DisallowWebinarEmojiReaction, SDKError)
//virtual bool IsWebinarEmojiReactionSupported() = 0;
DEFINE_FUNC_0(IsWebinarEmojiReactionSupported, bool)
//virtual SDKError AllowAttendeeRaiseHand() = 0;
DEFINE_FUNC_0(AllowAttendeeRaiseHand, SDKError)
//virtual SDKError DisallowAttendeeRaiseHand() = 0;
DEFINE_FUNC_0(DisallowAttendeeRaiseHand, SDKError)
//virtual SDKError AllowAttendeeViewTheParticipantCount() = 0;
DEFINE_FUNC_0(AllowAttendeeViewTheParticipantCount, SDKError)
//virtual SDKError DisallowAttendeeViewTheParticipantCount() = 0;
DEFINE_FUNC_0(DisallowAttendeeViewTheParticipantCount, SDKError)
//virtual int GetParticipantCount() = 0;
DEFINE_FUNC_0(GetParticipantCount, int)
//virtual WebinarMeetingStatus* GetWebinarMeetingStatus() = 0;
DEFINE_FUNC_0(GetWebinarMeetingStatus, WebinarMeetingStatus*)
//virtual SDKError SetPanelistChatPrivilege(SDKPanelistChatPrivilege privilege) = 0;
DEFINE_FUNC_1(SetPanelistChatPrivilege, SDKError, SDKPanelistChatPrivilege, privilege)
//virtual SDKError GetPanelistChatPrivilege(SDKPanelistChatPrivilege& privilege) = 0;
DEFINE_FUNC_1(GetPanelistChatPrivilege, SDKError, SDKPanelistChatPrivilege&, privilege)
#if defined(WIN32)
//virtual SDKError SetAttendeeViewMode(SDKAttendeeViewMode mode) = 0;
DEFINE_FUNC_1(SetAttendeeViewMode, SDKError, SDKAttendeeViewMode, mode)
//virtual SDKAttendeeViewMode GetAttendeeViewMode() = 0;
DEFINE_FUNC_0(GetAttendeeViewMode, SDKAttendeeViewMode)
//virtual const zchar_t* getWebinarLegalNoticesPrompt() = 0;
DEFINE_FUNC_0(getWebinarLegalNoticesPrompt, const zchar_t*)
//virtual bool getWebinarLegalNoticesExplained(WebinarLegalNoticesExplainedInfo& explained_info) = 0;
DEFINE_FUNC_1(getWebinarLegalNoticesExplained, bool, WebinarLegalNoticesExplainedInfo&, explained_info)
#endif
//virtual void onPromptAttendee2PanelistResult(int result) = 0;
CallBack_FUNC_1(onPromptAttendee2PanelistResult, int, result)
//virtual void onDepromptPanelist2AttendeeResult(int result) = 0;
CallBack_FUNC_1(onDepromptPanelist2AttendeeResult, int, result)
//virtual void onAllowPanelistStartVideoNotification() = 0;
CallBack_FUNC_0(onAllowPanelistStartVideoNotification)
//virtual void onDisallowPanelistStartVideoNotification() = 0;
CallBack_FUNC_0(onDisallowPanelistStartVideoNotification)
//virtual void onSelfAllowTalkNotification() = 0;
CallBack_FUNC_0(onSelfAllowTalkNotification)
//virtual void onSelfDisallowTalkNotification() = 0;
CallBack_FUNC_0(onSelfDisallowTalkNotification)
//virtual void onAllowAttendeeChatNotification() = 0;
CallBack_FUNC_0(onAllowAttendeeChatNotification)
//virtual void onDisallowAttendeeChatNotification() = 0;
CallBack_FUNC_0(onDisallowAttendeeChatNotification)
//virtual void onAttendeeAudioStatusNotification(unsigned int userid, bool can_talk, bool is_muted) = 0;
CallBack_FUNC_3(onAttendeeAudioStatusNotification, unsigned int, userid, bool, can_talk, bool, is_muted)
// virtual void onAttendeePromoteConfirmResult(bool agree, unsigned int userid) = 0;
CallBack_FUNC_2(onAttendeePromoteConfirmResult, bool, agree, unsigned int, userid)
//virtual void onAllowWebinarReactionStatusChanged(bool can_reaction) = 0;
CallBack_FUNC_1(onAllowWebinarReactionStatusChanged, bool, can_reaction)
//virtual void onAllowAttendeeRaiseHandStatusChanged(bool can_riseHand) = 0;
CallBack_FUNC_1(onAllowAttendeeRaiseHandStatusChanged, bool, can_riseHand)
//virtual void onAllowAttendeeViewTheParticipantCountStatusChanged(bool can_viewParticipantCount) = 0;
CallBack_FUNC_1(onAllowAttendeeViewTheParticipantCountStatusChanged, bool, can_viewParticipantCount)

END_CLASS_DEFINE(IMeetingWebinarController)
END_ZOOM_SDK_NAMESPACE