#include "test_video_device_helper_wrap.h"
#include "video_setting_context_wrap.h"
#include "virtual_bg_setting_context_wrap.h"
BEGIN_ZOOM_SDK_NAMESPACE
ITestVideoDeviceHelper* InitITestVideoDeviceHelperFunc(ITestVideoDeviceHelperEvent* pEvent, IVideoSettingContextWrap* pOwner)
{
	if (pOwner && pOwner->GetSDKObj())
	{
		ZOOM_SDK_NAMESPACE::ITestVideoDeviceHelper* pObj = pOwner->GetSDKObj()->GetTestVideoDeviceHelper();
		if (pObj)
		{
			pObj->SetEvent(pEvent);
		}
		return pObj;
	}

	return NULL;
}

void UninitITestVideoDeviceHelperFunc(ITestVideoDeviceHelper* obj)
{
	if (obj)
	{
		obj->SetEvent(NULL);
	}
}

//virtual SDKError SetVideoPreviewParentWnd(HWND hParentWnd, RECT rc = _SDK_TEST_VIDEO_INIT_RECT) = 0;
IMPL_FUNC_2(ITestVideoDeviceHelper, SetVideoPreviewParentWnd, SDKError, HWND, hParentWnd, RECT, rc, SDKERR_UNINITIALIZE)
//virtual SDKError TestVideoStartPreview(const zchar_t* deviceID = nullptr) = 0;
IMPL_FUNC_1(ITestVideoDeviceHelper, TestVideoStartPreview, SDKError, const zchar_t*, deviceID, SDKERR_UNINITIALIZE)
//virtual SDKError TestVideoStopPreview() = 0;
IMPL_FUNC_0(ITestVideoDeviceHelper, TestVideoStopPreview, SDKError, SDKERR_UNINITIALIZE)
//virtual SDKError TestVideoRotate(PREVIEW_VIDEO_ROTATION_ACTION action) = 0;
IMPL_FUNC_1(ITestVideoDeviceHelper, TestVideoRotate, SDKError, PREVIEW_VIDEO_ROTATION_ACTION, action, SDKERR_UNINITIALIZE)

END_ZOOM_SDK_NAMESPACE
