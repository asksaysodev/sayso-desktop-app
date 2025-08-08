#include "meeting_ai_companion_query_wrap.h"
#include "meeting_service_wrap.h"
BEGIN_ZOOM_SDK_NAMESPACE
IMeetingAICompanionQueryHelper* InitIMeetingAICompanionQueryHelperFunc(IMeetingAICompanionQueryHelperEvent* pEvent, IMeetingAICompanionControllerWrap* pOwner)
{
	if (pOwner && pOwner->GetSDKObj())
	{
		ZOOM_SDK_NAMESPACE::IMeetingAICompanionQueryHelper* pObj = pOwner->GetSDKObj()->GetMeetingAICompanionQueryHelper();
		if (pObj)
		{
			pObj->SetEvent(pEvent);
		}
		return pObj;
	}

	return NULL;
}

void UninitIMeetingAICompanionQueryHelperFunc(IMeetingAICompanionQueryHelper* obj)
{
	if (obj)
	{
		obj->SetEvent(NULL);
	}
}

//virtual SDKError CanChangeQuerySetting(bool& bCan) = 0;
IMPL_FUNC_1(IMeetingAICompanionQueryHelper, CanChangeQuerySetting, SDKError, bool&, bCan, SDKERR_UNINITIALIZE)
//virtual SDKError ChangeQuerySettings(MeetingAICompanionQuerySettingOptions setting) = 0;
IMPL_FUNC_1(IMeetingAICompanionQueryHelper, ChangeQuerySettings, SDKError, MeetingAICompanionQuerySettingOptions, setting, SDKERR_UNINITIALIZE)
//virtual MeetingAICompanionQuerySettingOptions GetSelectedQuerySetting() = 0;
IMPL_FUNC_0(IMeetingAICompanionQueryHelper, GetSelectedQuerySetting, MeetingAICompanionQuerySettingOptions, MeetingAICompanionQuerySettingOptions_None)
//virtual SDKError IsAICompanionQueryLegalNoticeAvailable(bool& bAvailable) = 0;
IMPL_FUNC_1(IMeetingAICompanionQueryHelper, IsAICompanionQueryLegalNoticeAvailable, SDKError, bool&, bAvailable, SDKERR_UNINITIALIZE)
//virtual const zchar_t* GetAICompanionQueryLegalNoticesPrompt() = 0;
IMPL_FUNC_0(IMeetingAICompanionQueryHelper, GetAICompanionQueryLegalNoticesPrompt, const zchar_t*, NULL)
//virtual const zchar_t* GetAICompanionQueryLegalNoticesExplained() = 0;
IMPL_FUNC_0(IMeetingAICompanionQueryHelper, GetAICompanionQueryLegalNoticesExplained, const zchar_t*, NULL)
//virtual SDKError IsAICompanionQueryPrivacyLegalNoticeAvailable(bool& bAvailable) = 0;
IMPL_FUNC_1(IMeetingAICompanionQueryHelper, IsAICompanionQueryPrivacyLegalNoticeAvailable, SDKError, bool&, bAvailable, SDKERR_UNINITIALIZE)
//virtual const zchar_t* GetAICompanionQueryPrivacyLegalNoticesPrompt() = 0;
IMPL_FUNC_0(IMeetingAICompanionQueryHelper, GetAICompanionQueryPrivacyLegalNoticesPrompt, const zchar_t*, NULL)

END_ZOOM_SDK_NAMESPACE