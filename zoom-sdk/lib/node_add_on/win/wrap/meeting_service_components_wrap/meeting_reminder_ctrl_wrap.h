#pragma once
#include "common_include.h"
BEGIN_ZOOM_SDK_NAMESPACE
class IMeetingServiceWrap;
IMeetingReminderController* InitIMeetingReminderControllerFunc(IMeetingReminderEvent* pEvent, IMeetingServiceWrap* pOwner);
void UninitIMeetingReminderControllerFunc(IMeetingReminderController* obj);
BEGIN_CLASS_DEFINE_WITHCALLBACK(IMeetingReminderController, IMeetingReminderEvent)
NORMAL_CLASS(IMeetingReminderController)
INIT_UNINIT_WITHEVENT_AND_OWNSERVICE(IMeetingReminderController, IMeetingServiceWrap)
virtual SDKError SetEvent(IMeetingReminderEvent* pEvent)
{
	external_cb = pEvent;
	return SDKERR_SUCCESS;
}

//virtual void onReminderNotify(IMeetingReminderContent* content, IMeetingReminderHandler* handle) = 0;
CallBack_FUNC_2(onReminderNotify, IMeetingReminderContent*, content, IMeetingReminderHandler*, handle)
//virtual void onEnableReminderNotify(IMeetingReminderContent* content, IMeetingEnableReminderHandler* handle) = 0;
CallBack_FUNC_2(onEnableReminderNotify, IMeetingReminderContent*, content, IMeetingEnableReminderHandler*, handle)

END_CLASS_DEFINE(IMeetingReminderController)
END_ZOOM_SDK_NAMESPACE