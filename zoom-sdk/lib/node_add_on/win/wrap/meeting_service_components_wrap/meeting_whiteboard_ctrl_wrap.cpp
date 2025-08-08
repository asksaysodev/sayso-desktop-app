#include "meeting_whiteboard_ctrl_wrap.h"
#include "meeting_service_wrap.h"
BEGIN_ZOOM_SDK_NAMESPACE
IMeetingWhiteboardController* InitIMeetingWhiteboardControllerFunc(IMeetingWhiteboardCtrlEvent* pEvent, IMeetingServiceWrap* pOwner)
{
	if (pOwner && pOwner->GetSDKObj())
	{
		ZOOM_SDK_NAMESPACE::IMeetingWhiteboardController* pObj = pOwner->GetSDKObj()->GetMeetingWhiteboardController();
		if (pObj)
		{
			pObj->SetEvent(pEvent);
		}
		return pObj;
	}

	return NULL;
}

void UninitIMeetingWhiteboardControllerFunc(IMeetingWhiteboardController* obj)
{
	if (obj)
	{
		obj->SetEvent(NULL);
	}
}

//virtual SDKError SetOwnerWindow(HWND hOwner) = 0;
IMPL_FUNC_1(IMeetingWhiteboardController, SetOwnerWindow, SDKError, HWND, hOwner, SDKERR_UNINITIALIZE)
//virtual bool IsSupportWhiteBoard() = 0;
IMPL_FUNC_0(IMeetingWhiteboardController, IsSupportWhiteBoard, bool, false)
//virtual bool CanStartShareWhiteboard(CannotShareReasonType& reason) = 0;
IMPL_FUNC_1(IMeetingWhiteboardController, CanStartShareWhiteboard, bool, CannotShareReasonType&, reason, false)
//virtual SDKError ShowDashboardView(int x = -1, int y = -1) = 0;
IMPL_FUNC_2(IMeetingWhiteboardController, ShowDashboardView, SDKError, int, x, int, y, SDKERR_UNINITIALIZE)
//virtual SDKError SetDashboardViewSize(int width = -1, int  height = -1) = 0;
IMPL_FUNC_2(IMeetingWhiteboardController, SetDashboardViewSize, SDKError, int, width, int, height, SDKERR_UNINITIALIZE)
//virtual SDKError SetWhiteboardViewPos(int x = -1, int y = -1) = 0;
IMPL_FUNC_2(IMeetingWhiteboardController, SetWhiteboardViewPos, SDKError, int, x, int, y, SDKERR_UNINITIALIZE)
//virtual SDKError SetWhiteboardViewSize(int width = -1, int height = -1) = 0;
IMPL_FUNC_2(IMeetingWhiteboardController, SetWhiteboardViewSize, SDKError, int, width, int, height, SDKERR_UNINITIALIZE)
//virtual SDKError SetWhiteboardShareOption(SDKWhiteboardShareOption option) = 0;
IMPL_FUNC_1(IMeetingWhiteboardController, SetWhiteboardShareOption, SDKError, SDKWhiteboardShareOption, option, SDKERR_UNINITIALIZE)
//virtual SDKError GetWhiteboardShareOption(SDKWhiteboardShareOption& option) = 0;
IMPL_FUNC_1(IMeetingWhiteboardController, GetWhiteboardShareOption, SDKError, SDKWhiteboardShareOption&, option, SDKERR_UNINITIALIZE)
//virtual SDKError SetWhiteboardCreateOption(SDKWhiteboardCreateOption option) = 0;
IMPL_FUNC_1(IMeetingWhiteboardController, SetWhiteboardCreateOption, SDKError, SDKWhiteboardCreateOption, option, SDKERR_UNINITIALIZE)
//virtual SDKError GetWhiteboardCreateOption(SDKWhiteboardCreateOption& option) = 0;
IMPL_FUNC_1(IMeetingWhiteboardController, GetWhiteboardCreateOption, SDKError, SDKWhiteboardCreateOption&, option, SDKERR_UNINITIALIZE)
//virtual SDKError EnableParticipantsCreateWithoutHost(bool bEnable) = 0;
IMPL_FUNC_1(IMeetingWhiteboardController, EnableParticipantsCreateWithoutHost, SDKError, bool, bEnable, SDKERR_UNINITIALIZE)
//virtual bool IsParticipantsCreateWithoutHostEnabled() = 0;
IMPL_FUNC_0(IMeetingWhiteboardController, IsParticipantsCreateWithoutHostEnabled, bool, false)
//virtual bool IsOtherSharingWhiteboard() = 0;
IMPL_FUNC_0(IMeetingWhiteboardController, IsOtherSharingWhiteboard, bool, false)
//virtual bool IsSharingWhiteboardOut() = 0;
IMPL_FUNC_0(IMeetingWhiteboardController, IsSharingWhiteboardOut, bool, false)

END_ZOOM_SDK_NAMESPACE