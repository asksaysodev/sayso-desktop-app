#pragma once
#include "common_include.h"
#include "meeting_ai_companion_smart_summary_wrap.h"
#include "meeting_ai_companion_query_wrap.h"

BEGIN_ZOOM_SDK_NAMESPACE
class IMeetingServiceWrap;
IMeetingAICompanionController* InitIMeetingAICompanionControllerFunc(IMeetingAICompanionCtrlEvent* pEvent, IMeetingServiceWrap* pOwner);
void UninitIMeetingAICompanionControllerFunc(IMeetingAICompanionController* obj);
BEGIN_CLASS_DEFINE_WITHCALLBACK(IMeetingAICompanionController, IMeetingAICompanionCtrlEvent)
NORMAL_CLASS(IMeetingAICompanionController)
INIT_UNINIT_WITHEVENT_AND_OWNSERVICE(IMeetingAICompanionController, IMeetingServiceWrap)
virtual SDKError SetEvent(IMeetingAICompanionCtrlEvent* pEvent)
{
	external_cb = pEvent;
	return SDKERR_SUCCESS;
}


#if (defined UserInterfaceClass)
private:
DEFINE_FUNC_0(GetMeetingAICompanionSmartSummaryHelper, IMeetingAICompanionSmartSummaryHelper*)
DEFINE_FUNC_0(GetMeetingAICompanionQueryHelper, IMeetingAICompanionQueryHelper*)
public:
#define T_GetMeetingAICompanionSmartSummaryHelper _GetMeetingAICompanionSmartSummaryHelper
#define T_GetMeetingAICompanionQueryHelper _GetMeetingAICompanionQueryHelper
#else
#define T_GetMeetingAICompanionSmartSummaryHelper GetMeetingAICompanionSmartSummaryHelper
#define T_GetMeetingAICompanionQueryHelper GetMeetingAICompanionQueryHelper
#endif
DEFINE_FUNC_AND_MEMBER(T_GetMeetingAICompanionSmartSummaryHelper, IMeetingAICompanionSmartSummaryHelperWrap)
DEFINE_FUNC_AND_MEMBER(T_GetMeetingAICompanionQueryHelper, IMeetingAICompanionQueryHelperWrap)

//virtual bool IsTurnoffAllAICompanionsSupported() = 0;
DEFINE_FUNC_0(IsTurnoffAllAICompanionsSupported, bool)
//virtual bool IsTurnOnAllAICompanionsSupported() = 0;
DEFINE_FUNC_0(IsTurnOnAllAICompanionsSupported, bool)
//virtual bool CanTurnOffAllAICompanions() = 0;
DEFINE_FUNC_0(CanTurnOffAllAICompanions, bool)
//virtual bool CanTurnOnAllAICompanions() = 0;
DEFINE_FUNC_0(CanTurnOnAllAICompanions, bool)
//virtual SDKError TurnOffAllAICompanions(bool bDeleteAssets) = 0;
DEFINE_FUNC_1(TurnOffAllAICompanions, SDKError, bool, bDeleteAssets)
//virtual SDKError TurnOnAllAICompanions() = 0;
DEFINE_FUNC_0(TurnOnAllAICompanions, SDKError)
//virtual bool CanRequestTurnoffAllAICompanions() = 0;
DEFINE_FUNC_0(CanRequestTurnoffAllAICompanions, bool)
//virtual bool CanRequestTurnOnAllAICompanions() = 0;
DEFINE_FUNC_0(CanRequestTurnOnAllAICompanions, bool)
//virtual SDKError RequestTurnoffAllAICompanions() = 0;
DEFINE_FUNC_0(RequestTurnoffAllAICompanions, SDKError)
//virtual SDKError RequestTurnOnAllAICompanions() = 0;
DEFINE_FUNC_0(RequestTurnOnAllAICompanions, SDKError)

//virtual void onAICompanionFeatureTurnOffByParticipant(IAICompanionFeatureTurnOnAgainHandler* handler) = 0;
CallBack_FUNC_1(onAICompanionFeatureTurnOffByParticipant, IAICompanionFeatureTurnOnAgainHandler*, handler)
//virtual void onAICompanionFeatureSwitchRequested(IAICompanionFeatureSwitchHandler* handler) = 0
CallBack_FUNC_1(onAICompanionFeatureSwitchRequested, IAICompanionFeatureSwitchHandler*, handler)
//virtual void onAICompanionFeatureSwitchRequestResponse(bool bTimeout, bool bAgree, bool bTurnOn) = 0;
CallBack_FUNC_3(onAICompanionFeatureSwitchRequestResponse, bool, bTimeout, bool, bAgree, bool, bTurnOn)
//virtual void onAICompanionFeatureCanNotBeTurnedOff(IList<AICompanionFeature>* features) = 0
CallBack_FUNC_1(onAICompanionFeatureCanNotBeTurnedOff, IList<AICompanionFeature>*, features)

END_CLASS_DEFINE(IMeetingAICompanionController)
END_ZOOM_SDK_NAMESPACE