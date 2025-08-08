#include "meeting_indicator_wrap_core.h"
#include "sdk_wrap.h"
#include "meeting_service_components_wrap/meeting_indicator_wrap.h"
#include "zoom_native_to_wrap.h"
#include "sdk_events_wrap_class.h"

extern ZOOM_SDK_NAMESPACE::IMeetingServiceWrap& g_meeting_service_wrap;

ZMeetingIndicatorWrap::ZMeetingIndicatorWrap()
{
	SDKEventWrapMgr::GetInst().m_meetingIndicatorCtrlEvent.SetOwner(this);
	m_pSink = NULL;
}
ZMeetingIndicatorWrap::~ZMeetingIndicatorWrap()
{
	Uninit();
	m_pSink = NULL;
	SDKEventWrapMgr::GetInst().m_meetingIndicatorCtrlEvent.SetOwner(NULL);
}
void ZMeetingIndicatorWrap::Init()
{
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingIndicatorController().Init_Wrap(&g_meeting_service_wrap);
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingIndicatorController().SetEvent(&SDKEventWrapMgr::GetInst().m_meetingIndicatorCtrlEvent);
}
void ZMeetingIndicatorWrap::Uninit()
{
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingIndicatorController().SetEvent(NULL);
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingIndicatorController().Uninit_Wrap();
}
void ZMeetingIndicatorWrap::SetSink(ZNativeSDKMeetingIndicatorWrapSink* pSink)
{
	m_pSink = pSink;
}
void ZMeetingIndicatorWrap::onIndicatorItemReceived()
{
	if (m_pSink)
	{
		m_pSink->onIndicatorItemReceived();
	}
}
void ZMeetingIndicatorWrap::onIndicatorItemRemoved()
{
	if (m_pSink)
	{
		m_pSink->onIndicatorItemRemoved();
	}
}
ZoomSTRING ZMeetingIndicatorWrap::GetIndicatorItemId()
{
	return ZMeetingIndicatorHandler::GetInst().GetIndicatorItemId();
}
ZoomSTRING ZMeetingIndicatorWrap::GetIndicatorName()
{
	return ZMeetingIndicatorHandler::GetInst().GetIndicatorName();
}
ZoomSTRING ZMeetingIndicatorWrap::GetIndicatorIcon()
{
	return ZMeetingIndicatorHandler::GetInst().GetIndicatorIcon();
}
ZNSDKError ZMeetingIndicatorWrap::ShowIndicatorPanel(unsigned int x, unsigned int y, uint64_t windowId)
{
	return ZMeetingIndicatorHandler::GetInst().ShowIndicatorPanel(x, y);
}
ZNSDKError ZMeetingIndicatorWrap::HideIndicatorPanel()
{
	return ZMeetingIndicatorHandler::GetInst().HideIndicatorPanel();
}