#pragma once
#include "common_include.h"
BEGIN_ZOOM_SDK_NAMESPACE
class IMeetingServiceWrap;
IMeetingWhiteboardController* InitIMeetingWhiteboardControllerFunc(IMeetingWhiteboardCtrlEvent* pEvent, IMeetingServiceWrap* pOwner);
void UninitIMeetingWhiteboardControllerFunc(IMeetingWhiteboardController* obj);
BEGIN_CLASS_DEFINE_WITHCALLBACK(IMeetingWhiteboardController, IMeetingWhiteboardCtrlEvent)
NORMAL_CLASS(IMeetingWhiteboardController)
INIT_UNINIT_WITHEVENT_AND_OWNSERVICE(IMeetingWhiteboardController, IMeetingServiceWrap)
virtual SDKError SetEvent(IMeetingWhiteboardCtrlEvent* pEvent)
{
	external_cb = pEvent;
	return SDKERR_SUCCESS;
}

//virtual SDKError SetOwnerWindow(HWND hOwner) = 0;
DEFINE_FUNC_1(SetOwnerWindow, SDKError, HWND, hOwner)
//virtual bool IsSupportWhiteBoard() = 0;
DEFINE_FUNC_0(IsSupportWhiteBoard, bool)
//virtual bool CanStartShareWhiteboard(CannotShareReasonType& reason) = 0;
DEFINE_FUNC_1(CanStartShareWhiteboard, bool, CannotShareReasonType&, reason)
//virtual SDKError ShowDashboardView(int x = -1, int y = -1) = 0;
DEFINE_FUNC_2(ShowDashboardView, SDKError, int, x, int, y)
//virtual SDKError SetDashboardViewSize(int width = -1, int height = -1) = 0;
DEFINE_FUNC_2(SetDashboardViewSize, SDKError, int, width, int, height)
//virtual SDKError SetWhiteboardViewPos(int x = -1, int y = -1) = 0;
DEFINE_FUNC_2(SetWhiteboardViewPos, SDKError, int, x, int, y)
//virtual SDKError SetWhiteboardViewSize(int width = -1, int height = -1) = 0;
DEFINE_FUNC_2(SetWhiteboardViewSize, SDKError, int, width, int, height)
//virtual SDKError SetWhiteboardShareOption(SDKWhiteboardShareOption option) = 0;
DEFINE_FUNC_1(SetWhiteboardShareOption, SDKError, SDKWhiteboardShareOption, option)
//virtual SDKError GetWhiteboardShareOption(SDKWhiteboardShareOption& option) = 0;
DEFINE_FUNC_1(GetWhiteboardShareOption, SDKError, SDKWhiteboardShareOption&, option)
//virtual SDKError SetWhiteboardCreateOption(SDKWhiteboardCreateOption option) = 0;
DEFINE_FUNC_1(SetWhiteboardCreateOption, SDKError, SDKWhiteboardCreateOption, option)
//virtual SDKError GetWhiteboardCreateOption(SDKWhiteboardCreateOption& option) = 0;
DEFINE_FUNC_1(GetWhiteboardCreateOption, SDKError, SDKWhiteboardCreateOption&, option)
//virtual SDKError EnableParticipantsCreateWithoutHost(bool bEnable) = 0;
DEFINE_FUNC_1(EnableParticipantsCreateWithoutHost, SDKError, bool, bEnable)
//virtual bool IsParticipantsCreateWithoutHostEnabled() = 0;
DEFINE_FUNC_0(IsParticipantsCreateWithoutHostEnabled, bool)
//virtual bool IsOtherSharingWhiteboard() = 0;
DEFINE_FUNC_0(IsOtherSharingWhiteboard, bool)
//virtual bool IsSharingWhiteboardOut() = 0;
DEFINE_FUNC_0(IsSharingWhiteboardOut, bool)

//virtual void onWhiteboardStatusChanged(SDKWhiteboardStatus status) = 0;
CallBack_FUNC_1(onWhiteboardStatusChanged, SDKWhiteboardStatus, status)
//virtual void onWhiteboardSettingsChanged(SDKWhiteboardShareOption shareOption, SDKWhiteboardCreateOption createOption, bool bEnable) = 0;
CallBack_FUNC_3(onWhiteboardSettingsChanged, SDKWhiteboardShareOption, shareOption, SDKWhiteboardCreateOption, createOption, bool, bEnable)

END_CLASS_DEFINE(IMeetingWhiteboardController)
END_ZOOM_SDK_NAMESPACE