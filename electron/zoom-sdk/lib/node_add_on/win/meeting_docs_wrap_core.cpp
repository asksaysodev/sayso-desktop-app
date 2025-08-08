#include "meeting_docs_wrap_core.h"
#include "sdk_wrap.h"
#include "meeting_service_components_wrap/meeting_docs_wrap.h"
#include "zoom_native_to_wrap.h"
#include "sdk_events_wrap_class.h"

extern ZOOM_SDK_NAMESPACE::IMeetingServiceWrap& g_meeting_service_wrap;

ZMeetingDocsWrap::ZMeetingDocsWrap()
{
	SDKEventWrapMgr::GetInst().m_meetingDocsCtrlEvent.SetOwner(this);
	m_pSink = NULL;
}
ZMeetingDocsWrap::~ZMeetingDocsWrap()
{
	Uninit();
	m_pSink = NULL;
	SDKEventWrapMgr::GetInst().m_meetingDocsCtrlEvent.SetOwner(NULL);
}
void ZMeetingDocsWrap::Init()
{
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingDocsController().Init_Wrap(&g_meeting_service_wrap);
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingDocsController().SetEvent(&SDKEventWrapMgr::GetInst().m_meetingDocsCtrlEvent);
}
void ZMeetingDocsWrap::Uninit()
{
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingDocsController().SetEvent(NULL);
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingDocsController().Uninit_Wrap();
}
void ZMeetingDocsWrap::SetSink(ZNativeSDKMeetingDocsWrapSink* pSink)
{
	m_pSink = pSink;
}
bool ZMeetingDocsWrap::IsSupportDocs()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingDocsController().IsSupportDocs();
}
bool ZMeetingDocsWrap::CanStartShareDocs(ZNCannotShareReasonType& zn_reason)
{
	ZOOM_SDK_NAMESPACE::CannotShareReasonType reason;
	bool bCan = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingDocsController().CanStartShareDocs(reason);
	zn_reason = Map2WrapDefine(reason);
	return bCan;
}
bool ZMeetingDocsWrap::CanSetDocsOption()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingDocsController().CanSetDocsOption();
}
ZNSDKError ZMeetingDocsWrap::SetDocsShareOption(ZNSDKDocsShareOption zn_option)
{
	ZOOM_SDK_NAMESPACE::SDKDocsShareOption option = Map2SDKDefine(zn_option);
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingDocsController().SetDocsShareOption(option);
	return Map2WrapDefine(sdkerr);
}
ZNSDKError ZMeetingDocsWrap::GetDocsShareOption(ZNSDKDocsShareOption& zn_option)
{
	ZOOM_SDK_NAMESPACE::SDKDocsShareOption option;
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingDocsController().GetDocsShareOption(option);
	zn_option = Map2WrapDefine(option);
	return Map2WrapDefine(sdkerr);
}
ZNSDKError ZMeetingDocsWrap::SetDocsCreateOption(ZNSDKDocsCreateOption zn_option)
{
	ZOOM_SDK_NAMESPACE::SDKDocsCreateOption option = Map2SDKDefine(zn_option);
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingDocsController().SetDocsCreateOption(option);
	return Map2WrapDefine(sdkerr);
}
ZNSDKError ZMeetingDocsWrap::GetDocsCreateOption(ZNSDKDocsCreateOption& zn_option)
{
	ZOOM_SDK_NAMESPACE::SDKDocsCreateOption option;
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingDocsController().GetDocsCreateOption(option);
	zn_option = Map2WrapDefine(option);
	return Map2WrapDefine(sdkerr);
}
bool ZMeetingDocsWrap::IsOtherSharingDocs()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingDocsController().IsOtherSharingDocs();
}
bool ZMeetingDocsWrap::IsSharingDocsOut()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingDocsController().IsSharingDocsOut();
}
ZNSDKError ZMeetingDocsWrap::SetOwnerWindow(ZoomSTRING windowID)
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_SUCCESS;
	if (!windowID.empty())
	{
		int sdk_handle = std::stoi(windowID, nullptr, 16);
		sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingDocsController().SetOwnerWindow((HWND)sdk_handle);
	}
	else
	{
		sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingDocsController().SetOwnerWindow(NULL);
	}
	return Map2WrapDefine(sdkerr);
}
ZNSDKError ZMeetingDocsWrap::ShowDocsDashboard(int x, int y)
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingDocsController().ShowDocsDashboard(x, y);
	return Map2WrapDefine(sdkerr);
}
ZNSDKError ZMeetingDocsWrap::SetDocDashboardSize(int width, int height)
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingDocsController().SetDocDashboardSize(width, height);
	return Map2WrapDefine(sdkerr);
}
ZNSDKError ZMeetingDocsWrap::SetDocsViewPos(int x, int y)
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingDocsController().SetDocsViewPos(x, y);
	return Map2WrapDefine(sdkerr);
}
ZNSDKError ZMeetingDocsWrap::SetDocsViewSize(int width, int height)
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingDocsController().SetDocsViewSize(width, height);
	return Map2WrapDefine(sdkerr);
}
ZNSDKError ZMeetingDocsWrap::ShowDocsViewWindow(unsigned int sourceID)
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingDocsController().ShowDocsViewWindow(sourceID);
	return Map2WrapDefine(sdkerr);
}
ZNSDKError ZMeetingDocsWrap::CloseDocsViewWindow()
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingDocsController().CloseDocsViewWindow();
	return Map2WrapDefine(sdkerr);
}
ZNList<ZNZoomSDKDocSharingSourceInfo> ZMeetingDocsWrap::GetDocsSharingSourceInfoList(unsigned int userID)
{
	ZNList<ZNZoomSDKDocSharingSourceInfo> zn_list;
	ZOOM_SDK_NAMESPACE::IList<ZOOM_SDK_NAMESPACE::IZoomSDKDocSharingSourceInfo* >* sdk_list = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingDocsController().GetDocsSharingSourceInfoList(userID);
	if (NULL == sdk_list)
	{
		return zn_list;
	}	
	int count = sdk_list->GetCount();
	for (int i = 0; i < count; ++i)
	{
		ZOOM_SDK_NAMESPACE::IZoomSDKDocSharingSourceInfo* pInfo = sdk_list->GetItem(i);
		zn_list.push_back(ConvertSDKZoomSDKDocSharingSourceInfoToZNZoomSDKDocSharingSourceInfo(pInfo));
	}
	return zn_list;
}
ZNList<unsigned int> ZMeetingDocsWrap::GetViewableDocsSharingUserList()
{
	ZNList<unsigned int> zn_list;
	ZOOM_SDK_NAMESPACE::IList<unsigned int>* sdk_list = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingDocsController().GetViewableDocsSharingUserList();
	if (NULL == sdk_list)
	{
		return zn_list;
	}	
	int count = sdk_list->GetCount();
	for (int i = 0; i < count; ++i)
		zn_list.push_back(sdk_list->GetItem(i));
	return zn_list;
}

// Event Callbacks
void ZMeetingDocsWrap::onDocsSharingSourceInfoChanged(ZNZoomSDKDocSharingSourceInfo info)
{
	if (m_pSink)
	{
		m_pSink->onDocsSharingSourceInfoChanged(info);
	}
}
void ZMeetingDocsWrap::onDocsPermissionChanged(ZNSDKDocsCreateOption createOption, ZNSDKDocsShareOption option)
{
	if (m_pSink)
	{
		m_pSink->onDocsPermissionChanged(createOption, option);
	}
}