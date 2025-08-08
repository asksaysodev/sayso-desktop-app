#include "virtual_bg_setting_context_wrap.h"
#include "setting_service_wrap.h"
BEGIN_ZOOM_SDK_NAMESPACE
IVirtualBGSettingContext* InitIVirtualBGSettingContextFunc(IVirtualBGSettingContextEvent* pEvent, ISettingServiceWrap* pOwner)
{
	if (pOwner && pOwner->GetSDKObj())
	{
		ZOOM_SDK_NAMESPACE::IVirtualBGSettingContext* pObj = pOwner->GetSDKObj()->GetVirtualBGSettings();
		if (pObj)
		{
			pObj->SetVirtualBGEvent(pEvent);
		}
		return pObj;
	}

	return NULL;
}

void UninitIVirtualBGSettingContextFunc(IVirtualBGSettingContext* pObj)
{
	if (pObj)
	{
		pObj->SetVirtualBGEvent(NULL);
	}
}

//virtual bool IsSupportVirtualBG() = 0;
IMPL_FUNC_0(IVirtualBGSettingContext, IsSupportVirtualBG, bool, false)
//virtual bool IsDeviceSupportSmartVirtualBG() = 0;
IMPL_FUNC_0(IVirtualBGSettingContext, IsDeviceSupportSmartVirtualBG, bool, false)
//virtual bool IsSupportVirtualBackgroundVideo() = 0;
IMPL_FUNC_0(IVirtualBGSettingContext, IsSupportVirtualBackgroundVideo, bool, false)
//virtual bool IsDeviceSupportSmartVirtualBackgroundVideo() = 0;
IMPL_FUNC_0(IVirtualBGSettingContext, IsDeviceSupportSmartVirtualBackgroundVideo, bool, false)
//virtual bool IsDeviceSupportGreenVirtualBackgroundVideo() = 0;
IMPL_FUNC_0(IVirtualBGSettingContext, IsDeviceSupportGreenVirtualBackgroundVideo, bool, false)
//virtual bool IsUsingGreenScreenOn() = 0;
IMPL_FUNC_0(IVirtualBGSettingContext, IsUsingGreenScreenOn, bool, false)
//virtual SDKError SetUsingGreenScreen(bool bUse) = 0;
IMPL_FUNC_1(IVirtualBGSettingContext, SetUsingGreenScreen, SDKError, bool, bUse, SDKERR_UNINITIALIZE)
//virtual bool IsAllowToAddNewVBItem() = 0;
IMPL_FUNC_0(IVirtualBGSettingContext, IsAllowToAddNewVBItem, bool, false)
//virtual bool isAllowToRemoveVBItem() = 0;
IMPL_FUNC_0(IVirtualBGSettingContext, isAllowToRemoveVBItem, bool, false)
//virtual SDKError AddBGImage(const zchar_t* file_path) = 0;
IMPL_FUNC_1(IVirtualBGSettingContext, AddBGImage, SDKError, const zchar_t*, file_path, SDKERR_UNINITIALIZE)
//virtual SDKError RemoveBGImage(IVirtualBGImageInfo* pRemoveImage) = 0;
IMPL_FUNC_1(IVirtualBGSettingContext, RemoveBGImage, SDKError, IVirtualBGImageInfo*, pRemoveImage, SDKERR_UNINITIALIZE)
//virtual IList<IVirtualBGImageInfo* >* GetBGImageList() = 0;
IMPL_FUNC_0(IVirtualBGSettingContext, GetBGImageList, IList<IVirtualBGImageInfo* >*, NULL)
//virtual SDKError UseBGImage(IVirtualBGImageInfo* pImage) = 0;
IMPL_FUNC_1(IVirtualBGSettingContext, UseBGImage, SDKError, IVirtualBGImageInfo*, pImage, SDKERR_UNINITIALIZE)
//virtual DWORD GetBGReplaceColor() = 0;
IMPL_FUNC_0(IVirtualBGSettingContext, GetBGReplaceColor, DWORD, 0)
//virtual SDKError BeginSelectReplaceVBColor() = 0;
IMPL_FUNC_0(IVirtualBGSettingContext, BeginSelectReplaceVBColor, SDKError, SDKERR_UNINITIALIZE)
//virtual SDKError AddBGVideo(const zchar_t* file_path) = 0;
IMPL_FUNC_1(IVirtualBGSettingContext, AddBGVideo, SDKError, const zchar_t*, file_path, SDKERR_UNINITIALIZE)
//virtual SDKError RemoveBGVideo(IVirtualBGImageInfo* pRemoveVideo) = 0;
IMPL_FUNC_1(IVirtualBGSettingContext, RemoveBGVideo, SDKError, IVirtualBGImageInfo*, pRemoveVideo, SDKERR_UNINITIALIZE)
//virtual IList<IVirtualBGImageInfo* >* GetBGVideoList() = 0;
IMPL_FUNC_0(IVirtualBGSettingContext, GetBGVideoList, IList<IVirtualBGImageInfo* >*, NULL)
//virtual SDKError UseBGVideo(IVirtualBGImageInfo* pImage) = 0;
IMPL_FUNC_1(IVirtualBGSettingContext, UseBGVideo, SDKError, IVirtualBGImageInfo*, pImage, SDKERR_UNINITIALIZE)

END_ZOOM_SDK_NAMESPACE