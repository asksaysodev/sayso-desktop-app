#pragma once
#include "common_include.h"
BEGIN_ZOOM_SDK_NAMESPACE

BEGIN_CLASS_DEFINE_WITHCALLBACK(ICustomizedShareRender, ICustomizedShareRenderEvent)
NORMAL_CLASS(ICustomizedShareRender)
~ICustomizedShareRenderWrap();
void Init(ICustomizedShareRender* pObj);
void UnInit();
//virtual SDKError SetEvent(ICustomizedShareRenderEvent* pEvent) = 0;
DEFINE_FUNC_1(SetEvent, SDKError, ICustomizedShareRenderEvent*, pEvent)
//virtual SDKError Show() = 0;
DEFINE_FUNC_0(Show, SDKError)
//virtual SDKError Hide() = 0;
DEFINE_FUNC_0(Hide, SDKError)
//virtual SDKError SetShareSourceID(unsigned int nShareSourceID) = 0;
DEFINE_FUNC_1(SetShareSourceID, SDKError, unsigned int, nShareSourceID)
//virtual unsigned int GetShareSourceID() = 0;
DEFINE_FUNC_0(GetShareSourceID, unsigned int)
//virtual SDKError Resize(RECT rc) = 0;
DEFINE_FUNC_1(Resize, SDKError, RECT, rc)
//virtual SDKError SetViewMode(CustomizedViewShareMode mode) = 0;
DEFINE_FUNC_1(SetViewMode, SDKError, CustomizedViewShareMode, mode)
//virtual HWND GetOwnerWnd() = 0;
DEFINE_FUNC_0(GetOwnerWnd, HWND)
//virtual SDKError HandleWindowsMoveMsg() = 0;
DEFINE_FUNC_0(HandleWindowsMoveMsg, SDKError)


//virtual void onSharingContentStartReceiving() = 0;
CallBack_FUNC_0(onSharingContentStartReceiving)
//virtual void virtual void onSharingSourceNotification(unsigned int nShareSourceID) = 0;
CallBack_FUNC_1(onSharingSourceNotification, unsigned int, nShareSourceID)
//virtual void onWindowMsgNotification(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
CallBack_FUNC_3(onWindowMsgNotification, UINT, uMsg, WPARAM, wParam, LPARAM, lParam)

END_CLASS_DEFINE(ICustomizedShareRender)
END_ZOOM_SDK_NAMESPACE