#pragma once
#include "common_include.h"
BEGIN_ZOOM_SDK_NAMESPACE
class IVideoSettingContextWrap;
ITestVideoDeviceHelper* InitITestVideoDeviceHelperFunc(ITestVideoDeviceHelperEvent* pEvent, IVideoSettingContextWrap* pOwner);
void UninitITestVideoDeviceHelperFunc(ITestVideoDeviceHelper* obj);
BEGIN_CLASS_DEFINE_WITHCALLBACK(ITestVideoDeviceHelper, ITestVideoDeviceHelperEvent)
NORMAL_CLASS(ITestVideoDeviceHelper)
INIT_UNINIT_WITHEVENT_AND_OWNSERVICE(ITestVideoDeviceHelper, IVideoSettingContextWrap)
virtual void SetEvent(ITestVideoDeviceHelperEvent* pEvent)
{
	external_cb = pEvent;
}

//virtual SDKError SetVideoPreviewParentWnd(HWND hParentWnd, RECT rc = _SDK_TEST_VIDEO_INIT_RECT) = 0;
DEFINE_FUNC_2(SetVideoPreviewParentWnd, SDKError, HWND, hParentWnd, RECT, rc)
//virtual SDKError TestVideoStartPreview(const zchar_t* deviceID = nullptr) = 0;
DEFINE_FUNC_1(TestVideoStartPreview, SDKError, const zchar_t*, deviceID)
//virtual SDKError TestVideoStopPreview() = 0;
DEFINE_FUNC_0(TestVideoStopPreview, SDKError)
//virtual SDKError TestVideoRotate(PREVIEW_VIDEO_ROTATION_ACTION action) = 0;
DEFINE_FUNC_1(TestVideoRotate, SDKError, PREVIEW_VIDEO_ROTATION_ACTION, action)
//virtual ICameraController* GetTestCameraController() = 0;

//virtual void OnNoVideoDeviceIsUseful() = 0;
CallBack_FUNC_0(OnNoVideoDeviceIsUseful)
//virtual void OnSelectedVideoDeviceIsChanged() = 0; 
CallBack_FUNC_0(OnSelectedVideoDeviceIsChanged)
//virtual void OnNoWindowToShowPreview() = 0; 
CallBack_FUNC_0(OnNoWindowToShowPreview)

END_CLASS_DEFINE(ITestVideoDeviceHelper)
END_ZOOM_SDK_NAMESPACE