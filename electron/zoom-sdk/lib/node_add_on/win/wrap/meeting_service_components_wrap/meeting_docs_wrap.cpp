#include "meeting_docs_wrap.h"
#include "meeting_service_wrap.h"
BEGIN_ZOOM_SDK_NAMESPACE
IMeetingDocsController* InitIMeetingDocsControllerFunc(IMeetingDocsCtrlEvent* pEvent, IMeetingServiceWrap* pOwner)
{
	if (pOwner && pOwner->GetSDKObj())
	{
		ZOOM_SDK_NAMESPACE::IMeetingDocsController* pObj = pOwner->GetSDKObj()->GetMeetingDocsController();
		if (pObj)
		{
			pObj->SetEvent(pEvent);
		}
		return pObj;
	}

	return NULL;
}

void UninitIMeetingDocsControllerFunc(IMeetingDocsController* obj)
{
	if (obj)
	{
		obj->SetEvent(NULL);
	}
}
//virtual bool IsSupportDocs() = 0;
IMPL_FUNC_0(IMeetingDocsController, IsSupportDocs, bool, false)
//virtual bool CanStartShareDocs(CannotShareReasonType& reason) = 0;
IMPL_FUNC_1(IMeetingDocsController, CanStartShareDocs, bool, CannotShareReasonType&, reason, false)
//virtual bool CanSetDocsOption() = 0;
IMPL_FUNC_0(IMeetingDocsController, CanSetDocsOption, bool, false)
//virtual SDKError SetDocsShareOption(SDKDocsShareOption option) = 0;
IMPL_FUNC_1(IMeetingDocsController, SetDocsShareOption, SDKError, SDKDocsShareOption, option, SDKERR_UNINITIALIZE)
//virtual SDKError GetDocsShareOption(SDKDocsShareOption& option) = 0;
IMPL_FUNC_1(IMeetingDocsController, GetDocsShareOption, SDKError, SDKDocsShareOption&, option, SDKERR_UNINITIALIZE)
//virtual SDKError SetDocsCreateOption(SDKDocsCreateOption option) = 0;
IMPL_FUNC_1(IMeetingDocsController, SetDocsCreateOption, SDKError, SDKDocsCreateOption, option, SDKERR_UNINITIALIZE)
//virtual SDKError GetDocsCreateOption(SDKDocsCreateOption& option) = 0;
IMPL_FUNC_1(IMeetingDocsController, GetDocsCreateOption, SDKError, SDKDocsCreateOption&, option, SDKERR_UNINITIALIZE)
//virtual bool IsOtherSharingDocs() = 0;
IMPL_FUNC_0(IMeetingDocsController, IsOtherSharingDocs, bool, false)
//virtual bool IsSharingDocsOut() = 0;
IMPL_FUNC_0(IMeetingDocsController, IsSharingDocsOut, bool, false)
//virtual SDKError SetOwnerWindow(HWND hOwner) = 0;
IMPL_FUNC_1(IMeetingDocsController, SetOwnerWindow, SDKError, HWND, hOwner, SDKERR_UNINITIALIZE)
//virtual SDKError ShowDocsDashboard(int x = -1, int y = -1) = 0;
IMPL_FUNC_2(IMeetingDocsController, ShowDocsDashboard, SDKError, int, x, int, y, SDKERR_UNINITIALIZE)
//virtual SDKError SetDocDashboardSize(int width = -1, int  height = -1) = 0;
IMPL_FUNC_2(IMeetingDocsController, SetDocDashboardSize, SDKError, int, width, int, height, SDKERR_UNINITIALIZE)
//virtual SDKError SetDocsViewPos(int x = -1, int y = -1) = 0;
IMPL_FUNC_2(IMeetingDocsController, SetDocsViewPos, SDKError, int, x, int, y, SDKERR_UNINITIALIZE)
//virtual SDKError SetDocsViewSize(int width = -1, int  height = -1) = 0;
IMPL_FUNC_2(IMeetingDocsController, SetDocsViewSize, SDKError, int, width, int, height, SDKERR_UNINITIALIZE)
//virtual SDKError ShowDocsViewWindow(unsigned int sourceID) = 0;
IMPL_FUNC_1(IMeetingDocsController, ShowDocsViewWindow, SDKError, unsigned int, sourceID, SDKERR_UNINITIALIZE)
//virtual SDKError CloseDocsViewWindow() = 0;
IMPL_FUNC_0(IMeetingDocsController, CloseDocsViewWindow, SDKError, SDKERR_UNINITIALIZE)
//virtual IList<IZoomSDKDocSharingSourceInfo* >* GetDocsSharingSourceInfoList(unsigned int userID) = 0;
IMPL_FUNC_1(IMeetingDocsController, GetDocsSharingSourceInfoList, IList<IZoomSDKDocSharingSourceInfo* >*, unsigned int, userID, NULL)
//virtual IList<unsigned int>* GetViewableDocsSharingUserList() = 0;
IMPL_FUNC_0(IMeetingDocsController, GetViewableDocsSharingUserList, IList<unsigned int>*, NULL)
END_ZOOM_SDK_NAMESPACE
