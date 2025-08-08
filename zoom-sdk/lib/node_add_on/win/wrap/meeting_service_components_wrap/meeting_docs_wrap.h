#pragma once
#include "common_include.h"
BEGIN_ZOOM_SDK_NAMESPACE
class IMeetingServiceWrap;
IMeetingDocsController* InitIMeetingDocsControllerFunc(IMeetingDocsCtrlEvent* pEvent, IMeetingServiceWrap* pOwner);
void UninitIMeetingDocsControllerFunc(IMeetingDocsController* obj);
BEGIN_CLASS_DEFINE_WITHCALLBACK(IMeetingDocsController, IMeetingDocsCtrlEvent)
NORMAL_CLASS(IMeetingDocsController)
INIT_UNINIT_WITHEVENT_AND_OWNSERVICE(IMeetingDocsController, IMeetingServiceWrap)
virtual SDKError SetEvent(IMeetingDocsCtrlEvent* pEvent)
{
	external_cb = pEvent;
	return SDKERR_SUCCESS;
}
//virtual bool IsSupportDocs() = 0;
DEFINE_FUNC_0(IsSupportDocs, bool)
//virtual bool CanStartShareDocs(CannotShareReasonType& reason) = 0;
DEFINE_FUNC_1(CanStartShareDocs, bool, CannotShareReasonType&, reason)
//virtual bool CanSetDocsOption() = 0;
DEFINE_FUNC_0(CanSetDocsOption, bool)
//virtual SDKError SetDocsShareOption(SDKDocsShareOption option) = 0;
DEFINE_FUNC_1(SetDocsShareOption, SDKError, SDKDocsShareOption, option)
//virtual SDKError GetDocsShareOption(SDKDocsShareOption& option) = 0;
DEFINE_FUNC_1(GetDocsShareOption, SDKError, SDKDocsShareOption&, option)
//virtual SDKError SetDocsCreateOption(SDKDocsCreateOption option) = 0;
DEFINE_FUNC_1(SetDocsCreateOption, SDKError, SDKDocsCreateOption, option)
//virtual SDKError GetDocsCreateOption(SDKDocsCreateOption& option) = 0;
DEFINE_FUNC_1(GetDocsCreateOption, SDKError, SDKDocsCreateOption&, option)
//virtual bool IsOtherSharingDocs() = 0;
DEFINE_FUNC_0(IsOtherSharingDocs, bool)
//virtual bool IsSharingDocsOut() = 0;
DEFINE_FUNC_0(IsSharingDocsOut, bool)
//virtual SDKError SetOwnerWindow(HWND hOwner) = 0;
DEFINE_FUNC_1(SetOwnerWindow, SDKError, HWND, hOwner)
//virtual SDKError ShowDocsDashboard(int x = -1, int y = -1) = 0;
DEFINE_FUNC_2(ShowDocsDashboard, SDKError, int, x, int, y)
//virtual SDKError SetDocDashboardSize(int width = -1, int  height = -1) = 0;
DEFINE_FUNC_2(SetDocDashboardSize, SDKError, int, width, int, height)
//virtual SDKError SetDocsViewPos(int x = -1, int y = -1) = 0;
DEFINE_FUNC_2(SetDocsViewPos, SDKError, int, x, int, y)
//virtual SDKError SetDocsViewSize(int width = -1, int  height = -1) = 0;
DEFINE_FUNC_2(SetDocsViewSize, SDKError, int, width, int, height)
//virtual SDKError ShowDocsViewWindow(unsigned int sourceID) = 0;
DEFINE_FUNC_1(ShowDocsViewWindow, SDKError, unsigned int, sourceID)
//virtual SDKError CloseDocsViewWindow() = 0;
DEFINE_FUNC_0(CloseDocsViewWindow, SDKError)
//virtual IList<IZoomSDKDocSharingSourceInfo* >* GetDocsSharingSourceInfoList(unsigned int userID) = 0;
DEFINE_FUNC_1(GetDocsSharingSourceInfoList, IList<IZoomSDKDocSharingSourceInfo* >*, unsigned int, userID)
//virtual IList<unsigned int>* GetViewableDocsSharingUserList() = 0;
DEFINE_FUNC_0(GetViewableDocsSharingUserList, IList<unsigned int>*)

//virtual void onDocsSharingSourceInfoChanged(IZoomSDKDocSharingSourceInfo* pInfo) = 0;
CallBack_FUNC_1(onDocsSharingSourceInfoChanged, IZoomSDKDocSharingSourceInfo*, pInfo)
//virtual void onDocsPermissionChanged(SDKDocsCreateOption createOption, SDKDocsShareOption option) = 0;
CallBack_FUNC_2(onDocsPermissionChanged, SDKDocsCreateOption, createOption, SDKDocsShareOption, option)
END_CLASS_DEFINE(IMeetingDocsController)
END_ZOOM_SDK_NAMESPACE
