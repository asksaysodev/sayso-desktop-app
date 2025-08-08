#include "meeting_reminder_ctrl_wrap.h"
#include "meeting_service_wrap.h"
BEGIN_ZOOM_SDK_NAMESPACE
IMeetingReminderController* InitIMeetingReminderControllerFunc(IMeetingReminderEvent* pEvent, IMeetingServiceWrap* pOwner)
{
	if (pOwner && pOwner->GetSDKObj())
	{
		ZOOM_SDK_NAMESPACE::IMeetingReminderController* pObj = pOwner->GetSDKObj()->GetMeetingReminderController();
		if (pObj)
		{
			pObj->SetEvent(pEvent);
		}
		return pObj;
	}

	return NULL;
}

void UninitIMeetingReminderControllerFunc(IMeetingReminderController* obj)
{
	if (obj)
	{
		obj->SetEvent(NULL);
	}
}

END_ZOOM_SDK_NAMESPACE