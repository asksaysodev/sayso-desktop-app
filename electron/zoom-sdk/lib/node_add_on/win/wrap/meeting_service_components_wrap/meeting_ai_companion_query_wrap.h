#pragma once
#include "common_include.h"
BEGIN_ZOOM_SDK_NAMESPACE
class IMeetingAICompanionControllerWrap;
IMeetingAICompanionQueryHelper* InitIMeetingAICompanionQueryHelperFunc(IMeetingAICompanionQueryHelperEvent* pEvent, IMeetingAICompanionControllerWrap* pOwner);
void UninitIMeetingAICompanionQueryHelperFunc(IMeetingAICompanionQueryHelper* obj);
BEGIN_CLASS_DEFINE_WITHCALLBACK(IMeetingAICompanionQueryHelper, IMeetingAICompanionQueryHelperEvent)
NORMAL_CLASS(IMeetingAICompanionQueryHelper)
INIT_UNINIT_WITHEVENT_AND_OWNSERVICE(IMeetingAICompanionQueryHelper, IMeetingAICompanionControllerWrap)
virtual SDKError SetEvent(IMeetingAICompanionQueryHelperEvent* pEvent)
{
	external_cb = pEvent;
	return SDKERR_SUCCESS;	
}

//virtual SDKError CanChangeQuerySetting(bool& bCan) = 0;
DEFINE_FUNC_1(CanChangeQuerySetting, SDKError, bool&, bCan)
//virtual SDKError ChangeQuerySettings(MeetingAICompanionQuerySettingOptions setting) = 0;
DEFINE_FUNC_1(ChangeQuerySettings, SDKError, MeetingAICompanionQuerySettingOptions, setting)
//virtual MeetingAICompanionQuerySettingOptions GetSelectedQuerySetting() = 0;
DEFINE_FUNC_0(GetSelectedQuerySetting, MeetingAICompanionQuerySettingOptions)
//virtual SDKError IsAICompanionQueryLegalNoticeAvailable(bool& bAvailable) = 0;
DEFINE_FUNC_1(IsAICompanionQueryLegalNoticeAvailable, SDKError, bool&, bAvailable)
//virtual const zchar_t* GetAICompanionQueryLegalNoticesPrompt() = 0;
DEFINE_FUNC_0(GetAICompanionQueryLegalNoticesPrompt, const zchar_t*)
//virtual const zchar_t* GetAICompanionQueryLegalNoticesExplained() = 0;
DEFINE_FUNC_0(GetAICompanionQueryLegalNoticesExplained, const zchar_t*)
//virtual SDKError IsAICompanionQueryPrivacyLegalNoticeAvailable(bool& bAvailable) = 0;
DEFINE_FUNC_1(IsAICompanionQueryPrivacyLegalNoticeAvailable, SDKError, bool&, bAvailable)
//virtual const zchar_t* GetAICompanionQueryPrivacyLegalNoticesPrompt() = 0;
DEFINE_FUNC_0(GetAICompanionQueryPrivacyLegalNoticesPrompt, const zchar_t*)

//virtual void onQueryStateNotSupported() = 0;
CallBack_FUNC_0(onQueryStateNotSupported)
//virtual void onQueryStateSupportedButDisabled(IMeetingEnableQueryHandler* pHandler) = 0;
CallBack_FUNC_1(onQueryStateSupportedButDisabled, IMeetingEnableQueryHandler*, pHandler)
//virtual void onQueryStateEnabledButNotStarted(IMeetingStartQueryHandler* pHandler) = 0;
CallBack_FUNC_1(onQueryStateEnabledButNotStarted, IMeetingStartQueryHandler*, pHandler)
//virtual void onQueryStateStarted(IMeetingSendQueryHandler* pHandler) = 0;
CallBack_FUNC_1(onQueryStateStarted, IMeetingSendQueryHandler*, pHandler)
//virtual void onQuerySettingChanged(MeetingAICompanionQuerySettingOptions eSetting) = 0;
CallBack_FUNC_1(onQuerySettingChanged, MeetingAICompanionQuerySettingOptions, eSetting)
//virtual void onFailedToStartQuery(bool bTimeout) = 0;
CallBack_FUNC_1(onFailedToStartQuery, bool, bTimeout)
//virtual void onReceiveRequestToEnableQuery(IMeetingApproveEnableQueryHandler* pHandler) = 0;
CallBack_FUNC_1(onReceiveRequestToEnableQuery, IMeetingApproveEnableQueryHandler*, pHandler)
//virtual void onReceiveRequestToStartQuery(IMeetingApproveStartQueryHandler* pHandler) = 0;
CallBack_FUNC_1(onReceiveRequestToStartQuery, IMeetingApproveStartQueryHandler*, pHandler)
//virtual void onReceiveQueryAnswer(IMeetingAICompanionQueryItem* pQueryItem) = 0;
CallBack_FUNC_1(onReceiveQueryAnswer, IMeetingAICompanionQueryItem*, pQueryItem)
//virtual void onQueryEnableActionCallback(IMeetingEnableQueryActionHandler* pHandler) = 0;
CallBack_FUNC_1(onQueryEnableActionCallback, IMeetingEnableQueryActionHandler*, pHandler)
//virtual void onSendQueryPrivilegeChanged(bool canSendQuery) = 0;
CallBack_FUNC_1(onSendQueryPrivilegeChanged, bool, canSendQuery)
//virtual void onFailedToRequestSendQuery(bool bTimeout) = 0;
CallBack_FUNC_1(onFailedToRequestSendQuery, bool, bTimeout)
//virtual void onReceiveRequestToSendQuery(IMeetingApproveSendQueryHandler* pHandler) = 0;
CallBack_FUNC_1(onReceiveRequestToSendQuery, IMeetingApproveSendQueryHandler*, pHandler)

END_CLASS_DEFINE(IMeetingAICompanionQueryHelper)
END_ZOOM_SDK_NAMESPACE