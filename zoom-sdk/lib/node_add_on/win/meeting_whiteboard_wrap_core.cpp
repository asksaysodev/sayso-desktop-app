#include "meeting_whiteboard_wrap_core.h"
#include "sdk_wrap.h"
#include "meeting_service_components_wrap/meeting_whiteboard_ctrl_wrap.h"
#include "zoom_native_to_wrap.h"
#include "sdk_events_wrap_class.h"

extern ZOOM_SDK_NAMESPACE::IMeetingServiceWrap& g_meeting_service_wrap;

ZMeetingWhiteboardWrap::ZMeetingWhiteboardWrap()
{
	SDKEventWrapMgr::GetInst().m_meetingWhiteboardCtrlEvent.SetOwner(this);
	m_pSink = NULL;
}
ZMeetingWhiteboardWrap::~ZMeetingWhiteboardWrap()
{
	Uninit();
	m_pSink = NULL;
	SDKEventWrapMgr::GetInst().m_meetingWhiteboardCtrlEvent.SetOwner(NULL);
}
void ZMeetingWhiteboardWrap::Init()
{
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWhiteboardController().Init_Wrap(&g_meeting_service_wrap);
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWhiteboardController().SetEvent(&SDKEventWrapMgr::GetInst().m_meetingWhiteboardCtrlEvent);
}
void ZMeetingWhiteboardWrap::Uninit()
{
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWhiteboardController().SetEvent(NULL);
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWhiteboardController().Uninit_Wrap();
}
void ZMeetingWhiteboardWrap::SetSink(ZNativeSDKMeetingWhiteboardWrapSink* pSink)
{
	m_pSink = pSink;
}
ZNSDKError ZMeetingWhiteboardWrap::SetOwnerWindow(ZoomSTRING windowID)
{
	if (!windowID.empty())
	{
		int sdk_handle = std::stoi(windowID, nullptr, 16);
		return Map2WrapDefine(ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWhiteboardController().SetOwnerWindow((HWND)sdk_handle));
	}
	else
	{
		return Map2WrapDefine(ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWhiteboardController().SetOwnerWindow(NULL));
	}
}
bool ZMeetingWhiteboardWrap::IsSupportWhiteBoard()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWhiteboardController().IsSupportWhiteBoard();
}
bool ZMeetingWhiteboardWrap::CanStartShareWhiteboard(ZNCannotShareReasonType& zn_reason)
{
	ZOOM_SDK_NAMESPACE::CannotShareReasonType reason;
	bool bCan = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWhiteboardController().CanStartShareWhiteboard(reason);
	zn_reason = Map2WrapDefine(reason);
	return bCan; 
}
ZNSDKError ZMeetingWhiteboardWrap::ShowDashboardView(int x, int y)
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWhiteboardController().ShowDashboardView(x, y);
	return Map2WrapDefine(sdkerr);
}
ZNSDKError ZMeetingWhiteboardWrap::SetDashboardViewSize(int width, int height)
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWhiteboardController().SetDashboardViewSize(width, height);
	return Map2WrapDefine(sdkerr);
}
ZNSDKError ZMeetingWhiteboardWrap::SetWhiteboardViewPos(int x, int y)
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWhiteboardController().SetWhiteboardViewPos(x, y);
	return Map2WrapDefine(sdkerr);
}
ZNSDKError ZMeetingWhiteboardWrap::SetWhiteboardViewSize(int width, int height)
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWhiteboardController().SetWhiteboardViewSize(width, height);
	return Map2WrapDefine(sdkerr);
}
ZNSDKError ZMeetingWhiteboardWrap::SetWhiteboardShareOption(ZNSDKWhiteboardShareOption zn_option)
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWhiteboardController().SetWhiteboardShareOption(Map2SDKDefine(zn_option));
	return Map2WrapDefine(sdkerr);
}
ZNSDKError ZMeetingWhiteboardWrap::GetWhiteboardShareOption(ZNSDKWhiteboardShareOption& zn_option)
{
	ZOOM_SDK_NAMESPACE::SDKWhiteboardShareOption option;
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWhiteboardController().GetWhiteboardShareOption(option);
	zn_option = Map2WrapDefine(option);
	return Map2WrapDefine(sdkerr);
}
ZNSDKError ZMeetingWhiteboardWrap::SetWhiteboardCreateOption(ZNSDKWhiteboardCreateOption zn_option)
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWhiteboardController().SetWhiteboardCreateOption(Map2SDKDefine(zn_option));
	return Map2WrapDefine(sdkerr);
}
ZNSDKError ZMeetingWhiteboardWrap::GetWhiteboardCreateOption(ZNSDKWhiteboardCreateOption& zn_option)
{
	ZOOM_SDK_NAMESPACE::SDKWhiteboardCreateOption option;
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWhiteboardController().GetWhiteboardCreateOption(option);
	zn_option = Map2WrapDefine(option);
	return Map2WrapDefine(sdkerr);
}
ZNSDKError ZMeetingWhiteboardWrap::EnableParticipantsCreateWithoutHost(bool bEnable)
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN;
	sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWhiteboardController().EnableParticipantsCreateWithoutHost(bEnable);
	return Map2WrapDefine(sdkerr);
}
bool ZMeetingWhiteboardWrap::IsParticipantsCreateWithoutHostEnabled()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWhiteboardController().IsParticipantsCreateWithoutHostEnabled();
}
bool ZMeetingWhiteboardWrap::IsOtherSharingWhiteboard()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWhiteboardController().IsOtherSharingWhiteboard();
}
bool ZMeetingWhiteboardWrap::IsSharingWhiteboardOut()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWhiteboardController().IsSharingWhiteboardOut();
}

void ZMeetingWhiteboardWrap::onWhiteboardStatusChanged(ZNSDKWhiteboardStatus status)
{
	if (m_pSink)
	{
		m_pSink->onWhiteboardStatusChanged(status);
	}
}
void ZMeetingWhiteboardWrap::onWhiteboardSettingsChanged(ZNSDKWhiteboardShareOption shareOption, ZNSDKWhiteboardCreateOption createOption, bool bEnable)
{
	if (m_pSink)
	{
		m_pSink->onWhiteboardSettingsChanged(shareOption, createOption, bEnable);
	}
}