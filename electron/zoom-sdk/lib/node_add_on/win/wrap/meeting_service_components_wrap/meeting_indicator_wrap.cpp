#include "meeting_indicator_wrap.h"
#include "meeting_service_wrap.h"
BEGIN_ZOOM_SDK_NAMESPACE
IMeetingIndicatorController* InitIMeetingIndicatorControllerFunc(IMeetingIndicatorCtrlEvent* pEvent, IMeetingServiceWrap* pOwner)
{
	if (pOwner && pOwner->GetSDKObj())
	{
		ZOOM_SDK_NAMESPACE::IMeetingIndicatorController* pObj = pOwner->GetSDKObj()->GetMeetingIndicatorController();
		if (pObj)
		{
			pObj->SetEvent(pEvent);
		}
		return pObj;
	}

	return NULL;
}

void UninitIMeetingIndicatorControllerFunc(IMeetingIndicatorController* obj)
{
	if (obj)
	{
		obj->SetEvent(NULL);
	}
}

END_ZOOM_SDK_NAMESPACE