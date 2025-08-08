#include "meeting_webinar_ctrl_wrap.h"
#include "meeting_service_wrap.h"
BEGIN_ZOOM_SDK_NAMESPACE
IMeetingWebinarController* InitIMeetingWebinarControllerFunc(IMeetingWebinarCtrlEvent* pEvent, IMeetingServiceWrap* pOwner)
{
	if (pOwner && pOwner->GetSDKObj())
	{
		ZOOM_SDK_NAMESPACE::IMeetingWebinarController* pObj = pOwner->GetSDKObj()->GetMeetingWebinarController();
		if (pObj)
		{
			pObj->SetEvent(pEvent);
		}
		return pObj;
	}

	return NULL;
}

void UninitIMeetingWebinarControllerFunc(IMeetingWebinarController* obj)
{
	if (obj)
	{
		obj->SetEvent(NULL);
	}
}

//virtual SDKError PromptAttendee2Panelist(unsigned int userid) = 0;
IMPL_FUNC_1(IMeetingWebinarController, PromptAttendee2Panelist, SDKError, unsigned int, userid, SDKERR_UNINITIALIZE)
//virtual SDKError DepromptPanelist2Attendee(unsigned int userid) = 0;
IMPL_FUNC_1(IMeetingWebinarController, DepromptPanelist2Attendee, SDKError, unsigned int, userid, SDKERR_UNINITIALIZE)
//virtual SDKError IsSupportAttendeeTalk() = 0;
IMPL_FUNC_0(IMeetingWebinarController, IsSupportAttendeeTalk, SDKError, SDKERR_UNINITIALIZE)
//virtual SDKError AllowAttendeeTalk(unsigned int userid) = 0;
IMPL_FUNC_1(IMeetingWebinarController, AllowAttendeeTalk, SDKError, unsigned int, userid, SDKERR_UNINITIALIZE)
//virtual SDKError DisallowAttendeeTalk(unsigned int userid) = 0;
IMPL_FUNC_1(IMeetingWebinarController, DisallowAttendeeTalk, SDKError, unsigned int, userid, SDKERR_UNINITIALIZE)
//virtual SDKError AllowPanelistStartVideo() = 0;
IMPL_FUNC_0(IMeetingWebinarController, AllowPanelistStartVideo, SDKError, SDKERR_UNINITIALIZE)
//virtual SDKError DisallowPanelistStartVideo() = 0;
IMPL_FUNC_0(IMeetingWebinarController, DisallowPanelistStartVideo, SDKError, SDKERR_UNINITIALIZE)
//virtual SDKError AllowWebinarEmojiReaction() = 0;
IMPL_FUNC_0(IMeetingWebinarController, AllowWebinarEmojiReaction, SDKError, SDKERR_UNINITIALIZE)
//virtual SDKError DisallowWebinarEmojiReaction() = 0;
IMPL_FUNC_0(IMeetingWebinarController, DisallowWebinarEmojiReaction, SDKError, SDKERR_UNINITIALIZE)
//virtual bool IsWebinarEmojiReactionSupported() = 0;
IMPL_FUNC_0(IMeetingWebinarController, IsWebinarEmojiReactionSupported, bool, false)
//virtual SDKError AllowAttendeeRaiseHand() = 0;
IMPL_FUNC_0(IMeetingWebinarController, AllowAttendeeRaiseHand, SDKError, SDKERR_UNINITIALIZE)
//virtual SDKError DisallowAttendeeRaiseHand() = 0;
IMPL_FUNC_0(IMeetingWebinarController, DisallowAttendeeRaiseHand, SDKError, SDKERR_UNINITIALIZE)
//virtual SDKError AllowAttendeeViewTheParticipantCount() = 0;
IMPL_FUNC_0(IMeetingWebinarController, AllowAttendeeViewTheParticipantCount, SDKError, SDKERR_UNINITIALIZE)
//virtual SDKError DisallowAttendeeViewTheParticipantCount() = 0;
IMPL_FUNC_0(IMeetingWebinarController, DisallowAttendeeViewTheParticipantCount, SDKError, SDKERR_UNINITIALIZE)
//virtual int GetParticipantCount() = 0;
IMPL_FUNC_0(IMeetingWebinarController, GetParticipantCount, int, 0)
//virtual WebinarMeetingStatus* GetWebinarMeetingStatus() = 0;
IMPL_FUNC_0(IMeetingWebinarController, GetWebinarMeetingStatus, WebinarMeetingStatus*, NULL)
//virtual SDKError SetPanelistChatPrivilege(SDKPanelistChatPrivilege privilege) = 0;
IMPL_FUNC_1(IMeetingWebinarController, SetPanelistChatPrivilege, SDKError, SDKPanelistChatPrivilege, privilege, SDKERR_UNINITIALIZE)
//virtual SDKError GetPanelistChatPrivilege(SDKPanelistChatPrivilege& privilege) = 0;
IMPL_FUNC_1(IMeetingWebinarController, GetPanelistChatPrivilege, SDKError, SDKPanelistChatPrivilege&, privilege, SDKERR_UNINITIALIZE)
#if defined(WIN32)
//virtual SDKError SetAttendeeViewMode(SDKAttendeeViewMode mode) = 0;
IMPL_FUNC_1(IMeetingWebinarController, SetAttendeeViewMode, SDKError, SDKAttendeeViewMode, mode, SDKERR_UNINITIALIZE)
//virtual SDKAttendeeViewMode GetAttendeeViewMode() = 0;
IMPL_FUNC_0(IMeetingWebinarController, GetAttendeeViewMode, SDKAttendeeViewMode, SDKAttendeeViewMode_None)
//virtual const zchar_t* getWebinarLegalNoticesPrompt() = 0;
IMPL_FUNC_0(IMeetingWebinarController, getWebinarLegalNoticesPrompt, const zchar_t*, NULL)
//virtual bool getWebinarLegalNoticesExplained(WebinarLegalNoticesExplainedInfo& explained_info) = 0;
IMPL_FUNC_1(IMeetingWebinarController, getWebinarLegalNoticesExplained, bool, WebinarLegalNoticesExplainedInfo&, explained_info, false)
#endif

END_ZOOM_SDK_NAMESPACE