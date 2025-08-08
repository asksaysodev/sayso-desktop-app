#include "meeting_share_wrap_core.h"
#include "sdk_wrap.h"
#include "meeting_service_components_wrap/meeting_sharing_wrap.h"
#include "zoom_native_to_wrap.h"
#include "sdk_events_wrap_class.h"

extern ZOOM_SDK_NAMESPACE::IMeetingServiceWrap& g_meeting_service_wrap;

ZMeetingShareWrap::ZMeetingShareWrap()
{
	SDKEventWrapMgr::GetInst().m_meetingShareCtrlWrapEvent.SetOwner(this);
	m_pSink = 0;
}
ZMeetingShareWrap::~ZMeetingShareWrap()
{
	Uninit();
	m_pSink = 0;
	SDKEventWrapMgr::GetInst().m_meetingShareCtrlWrapEvent.SetOwner(NULL);
}
void ZMeetingShareWrap::Init()
{
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingShareController().Init_Wrap(&g_meeting_service_wrap);
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingShareController().SetEvent(&SDKEventWrapMgr::GetInst().m_meetingShareCtrlWrapEvent);
}
void ZMeetingShareWrap::Uninit()
{
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingShareController().SetEvent(NULL);
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingShareController().Uninit_Wrap();
}
ZNSDKError ZMeetingShareWrap::StartAppShare(ZoomSTRING hShareApp)
{
	if (!hShareApp.empty())
	{
		int sdk_share_handle = std::stoi(hShareApp, nullptr, 16);
		return Map2WrapDefine(ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingShareController().StartAppShare((HWND)sdk_share_handle));
	}
	else
	{
		return Map2WrapDefine(ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingShareController().StartAppShare(NULL));
	}
	
}
ZNSDKError ZMeetingShareWrap::StartMonitorShare(ZoomSTRING monitorID)
{
	if (!monitorID.empty())
	{
		const wchar_t* sdk_monitorID = monitorID.c_str();
		return Map2WrapDefine(ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingShareController().StartMonitorShare(sdk_monitorID));
	}

	return Map2WrapDefine(ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingShareController().StartMonitorShare(NULL));
	
}
ZNSDKError ZMeetingShareWrap::StopShare()
{
	ZOOM_SDK_NAMESPACE::SDKError err = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingShareController().StopShare();
	return Map2WrapDefine(err);
}
void ZMeetingShareWrap::onSharingStatus(ZNZoomSDKSharingSourceInfo shareInfo)
{
	if (m_pSink)
	{
		m_pSink->onSharingStatus(shareInfo);
	}
}
void ZMeetingShareWrap::onFailedToStartShare()
{
	if (m_pSink)
	{
		m_pSink->onFailedToStartShare();
	}
}
void ZMeetingShareWrap::onOptimizingShareForVideoClipStatusChanged(ZNZoomSDKSharingSourceInfo shareInfo)
{
	if (m_pSink)
	{
		m_pSink->onOptimizingShareForVideoClipStatusChanged(shareInfo);
	}
}
void ZMeetingShareWrap::SetSink(ZNativeSDKMeetingShareWrapSink* pSink)
{
	m_pSink = pSink;
}
bool ZMeetingShareWrap::IsWhiteboardLegalNoticeAvailable()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingShareController().IsWhiteboardLegalNoticeAvailable();
}
ZoomSTRING ZMeetingShareWrap::GetWhiteboardLegalNoticesPrompt()
{
	ZoomSTRING zn_WhiteboardLegalNoticesPrompt;
	zn_WhiteboardLegalNoticesPrompt = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingShareController().getWhiteboardLegalNoticesPrompt();
	return zn_WhiteboardLegalNoticesPrompt;
}
ZoomSTRING ZMeetingShareWrap::GetWhiteboardLegalNoticesExplained()
{
	ZoomSTRING zn_WhiteboardLegalNoticesExpained;
	zn_WhiteboardLegalNoticesExpained = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingShareController().getWhiteboardLegalNoticesExplained();
	return zn_WhiteboardLegalNoticesExpained;
}
ZNSDKError ZMeetingShareWrap::SetAudioShareMode(ZNAudioShareMode mode)
{
	return Map2WrapDefine(ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingShareController().SetAudioShareMode((ZOOMSDK::AudioShareMode)mode));
}
ZNSDKError ZMeetingShareWrap::GetAudioShareMode(ZNAudioShareMode& mode)
{
	ZOOM_SDK_NAMESPACE::AudioShareMode sdk_mode;

	ZNSDKError err = Map2WrapDefine(ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingShareController().GetAudioShareMode(sdk_mode));
	mode = (ZNAudioShareMode)sdk_mode;
	return err;
}