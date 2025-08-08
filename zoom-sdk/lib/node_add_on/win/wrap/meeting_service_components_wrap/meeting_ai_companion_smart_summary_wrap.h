#pragma once
#include "common_include.h"
BEGIN_ZOOM_SDK_NAMESPACE
class IMeetingAICompanionControllerWrap;
IMeetingAICompanionSmartSummaryHelper* InitIMeetingAICompanionSmartSummaryHelperFunc(IMeetingAICompanionSmartSummaryHelperEvent* pEvent, IMeetingAICompanionControllerWrap* pOwner);
void UninitIMeetingAICompanionSmartSummaryHelperFunc(IMeetingAICompanionSmartSummaryHelper* obj);
BEGIN_CLASS_DEFINE_WITHCALLBACK(IMeetingAICompanionSmartSummaryHelper, IMeetingAICompanionSmartSummaryHelperEvent)
NORMAL_CLASS(IMeetingAICompanionSmartSummaryHelper)
INIT_UNINIT_WITHEVENT_AND_OWNSERVICE(IMeetingAICompanionSmartSummaryHelper, IMeetingAICompanionControllerWrap)
virtual SDKError SetEvent(IMeetingAICompanionSmartSummaryHelperEvent* pEvent)
{
	external_cb = pEvent;
	return SDKERR_SUCCESS;	
}

//virtual void onSmartSummaryStateNotSupported() = 0;
CallBack_FUNC_0(onSmartSummaryStateNotSupported)
//virtual void onSmartSummaryStateSupportedButDisabled(IMeetingEnableSmartSummaryHandler* handler) = 0;
CallBack_FUNC_1(onSmartSummaryStateSupportedButDisabled, IMeetingEnableSmartSummaryHandler*, handler)
//virtual void onSmartSummaryStateEnabledButNotStarted(IMeetingStartSmartSummaryHandler* handler) = 0;
CallBack_FUNC_1(onSmartSummaryStateEnabledButNotStarted, IMeetingStartSmartSummaryHandler*, handler)
//virtual void onSmartSummaryStateStarted(IMeetingStopSmartSummaryHandler* handler) = 0;
CallBack_FUNC_1(onSmartSummaryStateStarted, IMeetingStopSmartSummaryHandler*, handler)
//virtual void onFailedToStartSmartSummary(bool bTimeout) = 0;
CallBack_FUNC_1(onFailedToStartSmartSummary, bool, bTimeout)
//virtual void onSmartSummaryEnableRequestReceived(IMeetingApproveEnableSmartSummaryHandler* handler) = 0;
CallBack_FUNC_1(onSmartSummaryEnableRequestReceived, IMeetingApproveEnableSmartSummaryHandler*, handler)
//virtual void onSmartSummaryStartRequestReceived(IMeetingApproveStartSmartSummaryHandler* handler) = 0;
CallBack_FUNC_1(onSmartSummaryStartRequestReceived, IMeetingApproveStartSmartSummaryHandler*, handler)
//virtual void onSmartSummaryEnableActionCallback(IMeetingEnableSmartSummaryActionHandler* handler) = 0;
CallBack_FUNC_1(onSmartSummaryEnableActionCallback, IMeetingEnableSmartSummaryActionHandler*, handler)

END_CLASS_DEFINE(IMeetingAICompanionSmartSummaryHelper)
END_ZOOM_SDK_NAMESPACE