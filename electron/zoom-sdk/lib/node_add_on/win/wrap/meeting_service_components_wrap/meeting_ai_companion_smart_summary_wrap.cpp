#include "meeting_ai_companion_smart_summary_wrap.h"
#include "meeting_service_wrap.h"
BEGIN_ZOOM_SDK_NAMESPACE
IMeetingAICompanionSmartSummaryHelper* InitIMeetingAICompanionSmartSummaryHelperFunc(IMeetingAICompanionSmartSummaryHelperEvent* pEvent, IMeetingAICompanionControllerWrap* pOwner)
{
	if (pOwner && pOwner->GetSDKObj())
	{
		ZOOM_SDK_NAMESPACE::IMeetingAICompanionSmartSummaryHelper* pObj = pOwner->GetSDKObj()->GetMeetingAICompanionSmartSummaryHelper();
		if (pObj)
		{
			pObj->SetEvent(pEvent);
		}
		return pObj;
	}

	return NULL;
}

void UninitIMeetingAICompanionSmartSummaryHelperFunc(IMeetingAICompanionSmartSummaryHelper* obj)
{
	if (obj)
	{
		obj->SetEvent(NULL);
	}
}

END_ZOOM_SDK_NAMESPACE