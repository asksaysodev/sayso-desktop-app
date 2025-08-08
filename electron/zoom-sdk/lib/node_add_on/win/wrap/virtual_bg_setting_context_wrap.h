#pragma once
#include "common_include.h"
#include "test_video_device_helper_wrap.h"
BEGIN_ZOOM_SDK_NAMESPACE
class ISettingServiceWrap;
IVirtualBGSettingContext* InitIVirtualBGSettingContextFunc(IVirtualBGSettingContextEvent* pEvent, ISettingServiceWrap* pOwner);
void UninitIVirtualBGSettingContextFunc(IVirtualBGSettingContext* obj);
BEGIN_CLASS_DEFINE_WITHCALLBACK(IVirtualBGSettingContext, IVirtualBGSettingContextEvent)
NORMAL_CLASS(IVirtualBGSettingContext)
INIT_UNINIT_WITHEVENT_AND_OWNSERVICE(IVirtualBGSettingContext, ISettingServiceWrap)
virtual SDKError SetVirtualBGEvent(IVirtualBGSettingContextEvent* pEvent)
{
	external_cb = pEvent;
	return SDKERR_SUCCESS;
}
//virtual bool IsSupportVirtualBG() = 0;
DEFINE_FUNC_0(IsSupportVirtualBG, bool)
//virtual bool IsDeviceSupportSmartVirtualBG() = 0;
DEFINE_FUNC_0(IsDeviceSupportSmartVirtualBG, bool)
//virtual bool IsSupportVirtualBackgroundVideo() = 0;
DEFINE_FUNC_0(IsSupportVirtualBackgroundVideo, bool)
//virtual bool IsDeviceSupportSmartVirtualBackgroundVideo() = 0;
DEFINE_FUNC_0(IsDeviceSupportSmartVirtualBackgroundVideo, bool)
//virtual bool IsDeviceSupportGreenVirtualBackgroundVideo() = 0;
DEFINE_FUNC_0(IsDeviceSupportGreenVirtualBackgroundVideo, bool)
//virtual bool IsUsingGreenScreenOn() = 0;
DEFINE_FUNC_0(IsUsingGreenScreenOn, bool)
//virtual SDKError SetUsingGreenScreen(bool bUse) = 0;
DEFINE_FUNC_1(SetUsingGreenScreen, SDKError, bool, bUse)
//virtual bool IsAllowToAddNewVBItem() = 0;
DEFINE_FUNC_0(IsAllowToAddNewVBItem, bool)
//virtual bool isAllowToRemoveVBItem() = 0;
DEFINE_FUNC_0(isAllowToRemoveVBItem, bool)
//virtual SDKError AddBGImage(const zchar_t* file_path) = 0;
DEFINE_FUNC_1(AddBGImage, SDKError, const zchar_t*, file_path)
//virtual SDKError RemoveBGImage(IVirtualBGImageInfo* pRemoveImage) = 0;
DEFINE_FUNC_1(RemoveBGImage, SDKError, IVirtualBGImageInfo*, pRemoveImage)
//virtual IList<IVirtualBGImageInfo* >* GetBGImageList() = 0;
DEFINE_FUNC_0(GetBGImageList, IList<IVirtualBGImageInfo* >*)
//virtual SDKError UseBGImage(IVirtualBGImageInfo* pImage) = 0;
DEFINE_FUNC_1(UseBGImage, SDKError, IVirtualBGImageInfo*, pImage)
//virtual DWORD GetBGReplaceColor() = 0;
DEFINE_FUNC_0(GetBGReplaceColor, DWORD)
//virtual SDKError BeginSelectReplaceVBColor() = 0;
DEFINE_FUNC_0(BeginSelectReplaceVBColor, SDKError)
//virtual SDKError AddBGVideo(const zchar_t* file_path) = 0;
DEFINE_FUNC_1(AddBGVideo, SDKError, const zchar_t*, file_path)
//virtual SDKError RemoveBGVideo(IVirtualBGImageInfo* pRemoveVideo) = 0;
DEFINE_FUNC_1(RemoveBGVideo, SDKError, IVirtualBGImageInfo*, pRemoveVideo)
//virtual IList<IVirtualBGImageInfo* >* GetBGVideoList() = 0;
DEFINE_FUNC_0(GetBGVideoList, IList<IVirtualBGImageInfo* >*)
//virtual SDKError UseBGVideo(IVirtualBGImageInfo* pImage) = 0;
DEFINE_FUNC_1(UseBGVideo, SDKError, IVirtualBGImageInfo*, pImage)

//virtual void onVBImageDidDownloaded() = 0;
CallBack_FUNC_0(onVBImageDidDownloaded)
//virtual void onGreenVBDidUpdateWithReplaceColor(DWORD selectedColor) = 0;
CallBack_FUNC_1(onGreenVBDidUpdateWithReplaceColor, DWORD, selectedColor)
//virtual void onSelectedVBImageChanged() = 0;
CallBack_FUNC_0(onSelectedVBImageChanged)
//virtual void OnVideoThumbReady(const zchar_t* file_path) = 0;
CallBack_FUNC_1(OnVideoThumbReady, const zchar_t*, file_path)
//virtual void OnVideoThumbError(const zchar_t* file_path, VBVideoError error) = 0;
CallBack_FUNC_2(OnVideoThumbError, const zchar_t*, file_path, VBVideoError, error)
//virtual void OnVideoPlayError(const zchar_t* file_path, VBVideoError error) = 0;
CallBack_FUNC_2(OnVideoPlayError, const zchar_t*, file_path, VBVideoError, error)

END_CLASS_DEFINE(IVirtualBGSettingContext)
END_ZOOM_SDK_NAMESPACE