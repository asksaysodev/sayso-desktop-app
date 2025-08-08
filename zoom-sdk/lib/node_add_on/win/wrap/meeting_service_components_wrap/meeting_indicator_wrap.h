#pragma once
#include "common_include.h"
BEGIN_ZOOM_SDK_NAMESPACE
class IMeetingServiceWrap;
IMeetingIndicatorController* InitIMeetingIndicatorControllerFunc(IMeetingIndicatorCtrlEvent* pEvent, IMeetingServiceWrap* pOwner);
void UninitIMeetingIndicatorControllerFunc(IMeetingIndicatorController* obj);
BEGIN_CLASS_DEFINE_WITHCALLBACK(IMeetingIndicatorController, IMeetingIndicatorCtrlEvent)
NORMAL_CLASS(IMeetingIndicatorController)
INIT_UNINIT_WITHEVENT_AND_OWNSERVICE(IMeetingIndicatorController, IMeetingServiceWrap)
virtual SDKError SetEvent(IMeetingIndicatorCtrlEvent* pEvent)
{
	external_cb = pEvent;
	return SDKERR_SUCCESS;
}

//virtual void onIndicatorItemReceived(IMeetingIndicatorHandler* handler) = 0;	
CallBack_FUNC_1(onIndicatorItemReceived, IMeetingIndicatorHandler*, handler)
//virtual void onIndicatorItemRemoved(IMeetingIndicatorHandler* handler) = 0;
CallBack_FUNC_1(onIndicatorItemRemoved, IMeetingIndicatorHandler*, handler)

END_CLASS_DEFINE(IMeetingIndicatorController)
END_ZOOM_SDK_NAMESPACE