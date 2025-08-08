#include "meeting_ai_companion_wrap.h"
#include "meeting_service_wrap.h"
BEGIN_ZOOM_SDK_NAMESPACE
IMeetingAICompanionController* InitIMeetingAICompanionControllerFunc(IMeetingAICompanionCtrlEvent* pEvent, IMeetingServiceWrap* pOwner)
{
	if (pOwner && pOwner->GetSDKObj())
	{
		ZOOM_SDK_NAMESPACE::IMeetingAICompanionController* pObj = pOwner->GetSDKObj()->GetMeetingAICompanionController();
		if (pObj)
		{
			pObj->SetEvent(pEvent);
		}
		return pObj;
	}

	return NULL;
}

void UninitIMeetingAICompanionControllerFunc(IMeetingAICompanionController* obj)
{
	if (obj)
	{
		obj->SetEvent(NULL);
	}
}

IMPL_FUNC_AND_MEMBER(IMeetingAICompanionController, T_GetMeetingAICompanionSmartSummaryHelper, IMeetingAICompanionSmartSummaryHelperWrap);
IMPL_FUNC_AND_MEMBER(IMeetingAICompanionController, T_GetMeetingAICompanionQueryHelper, IMeetingAICompanionQueryHelperWrap);
#if (defined UserInterfaceClass)
IMPL_FUNC_0(IMeetingAICompanionController, GetMeetingAICompanionSmartSummaryHelper, IMeetingAICompanionSmartSummaryHelper*, NULL)
IMPL_FUNC_0(IMeetingAICompanionController, GetMeetingAICompanionQueryHelper, IMeetingAICompanionQueryHelper*, NULL)
#endif

//virtual bool IsTurnoffAllAICompanionsSupported() = 0;
IMPL_FUNC_0(IMeetingAICompanionController, IsTurnoffAllAICompanionsSupported, bool, false)
//virtual bool IsTurnOnAllAICompanionsSupported() = 0;
IMPL_FUNC_0(IMeetingAICompanionController, IsTurnOnAllAICompanionsSupported, bool, false)
//virtual bool CanTurnOffAllAICompanions() = 0;
IMPL_FUNC_0(IMeetingAICompanionController, CanTurnOffAllAICompanions, bool, false)
//virtual bool CanTurnOnAllAICompanions() = 0;
IMPL_FUNC_0(IMeetingAICompanionController, CanTurnOnAllAICompanions, bool, false)
//virtual SDKError TurnOffAllAICompanions(bool bDeleteAssets) = 0;
IMPL_FUNC_1(IMeetingAICompanionController, TurnOffAllAICompanions, SDKError, bool, bDeleteAssets, SDKERR_UNINITIALIZE)
//virtual SDKError TurnOnAllAICompanions() = 0;
IMPL_FUNC_0(IMeetingAICompanionController, TurnOnAllAICompanions, SDKError, SDKERR_UNINITIALIZE)
//virtual bool CanRequestTurnoffAllAICompanions() = 0;
IMPL_FUNC_0(IMeetingAICompanionController, CanRequestTurnoffAllAICompanions, bool, false)
//virtual bool CanRequestTurnOnAllAICompanions() = 0;
IMPL_FUNC_0(IMeetingAICompanionController, CanRequestTurnOnAllAICompanions, bool, false)
//virtual SDKError RequestTurnoffAllAICompanions() = 0;
IMPL_FUNC_0(IMeetingAICompanionController, RequestTurnoffAllAICompanions, SDKError, SDKERR_UNINITIALIZE)
//virtual SDKError RequestTurnOnAllAICompanions() = 0;
IMPL_FUNC_0(IMeetingAICompanionController, RequestTurnOnAllAICompanions, SDKError, SDKERR_UNINITIALIZE)

END_ZOOM_SDK_NAMESPACE