#include "setting_virtual_bg_wrap_core.h"
#include "sdk_wrap.h"
#include "virtual_bg_setting_context_wrap.h"
#include "zoom_native_to_wrap.h"
#include "sdk_events_wrap_class.h"

extern ZOOM_SDK_NAMESPACE::ISettingServiceWrap& g_setting_service_wrap;

ZSettingVirtualBGWrap::ZSettingVirtualBGWrap()
{
	SDKEventWrapMgr::GetInst().m_virtualBGSettingContextEvent.SetOwner(this);
	m_pSink = NULL;
}
ZSettingVirtualBGWrap::~ZSettingVirtualBGWrap()
{
	Uninit();
	m_pSink = NULL;
	zn_bg_image_list.clear();
	zn_bg_video_list.clear();
	SDKEventWrapMgr::GetInst().m_virtualBGSettingContextEvent.SetOwner(NULL);
}
void ZSettingVirtualBGWrap::Init()
{
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetVirtualBGSettings().Init_Wrap(&g_setting_service_wrap);
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetVirtualBGSettings().SetVirtualBGEvent(&SDKEventWrapMgr::GetInst().m_virtualBGSettingContextEvent);
}
void ZSettingVirtualBGWrap::Uninit()
{
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetVirtualBGSettings().SetVirtualBGEvent(NULL);
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetVirtualBGSettings().Uninit_Wrap();
}
void ZSettingVirtualBGWrap::SetSink(ZNativeSDKVirtualBGSettingContextWrapSink* pSink)
{
	m_pSink = pSink;
}
bool ZSettingVirtualBGWrap::isValidImageItem(int64_t imageHandle)
{
	bool isValid = false;
	for (int i = 0; i < zn_bg_image_list.size(); ++i)
	{
		if (imageHandle == zn_bg_image_list[i].imageHandle)
		{
			isValid = true;
			break;
		}
	}
	return isValid;
}
bool ZSettingVirtualBGWrap::isValidVideoItem(int64_t imageHandle)
{
	bool isValid = false;
	for (int i = 0; i < zn_bg_video_list.size(); ++i)
	{
		if (imageHandle == zn_bg_video_list[i].imageHandle)
		{
			isValid = true;
			break;
		}
	}
	return isValid;
}
bool ZSettingVirtualBGWrap::IsSupportVirtualBG()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetVirtualBGSettings().IsSupportVirtualBG();
}
bool ZSettingVirtualBGWrap::IsDeviceSupportSmartVirtualBG()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetVirtualBGSettings().IsDeviceSupportSmartVirtualBG();
}
bool ZSettingVirtualBGWrap::IsSupportVirtualBackgroundVideo()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetVirtualBGSettings().IsSupportVirtualBackgroundVideo();
}
bool ZSettingVirtualBGWrap::IsDeviceSupportSmartVirtualBackgroundVideo()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetVirtualBGSettings().IsDeviceSupportSmartVirtualBackgroundVideo();
}
bool ZSettingVirtualBGWrap::IsDeviceSupportGreenVirtualBackgroundVideo()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetVirtualBGSettings().IsDeviceSupportGreenVirtualBackgroundVideo();
}
bool ZSettingVirtualBGWrap::IsUsingGreenScreenOn()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetVirtualBGSettings().IsUsingGreenScreenOn();
}
ZNSDKError ZSettingVirtualBGWrap::SetUsingGreenScreen(bool bUse)
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetVirtualBGSettings().SetUsingGreenScreen(bUse);
	return Map2WrapDefine(sdkerr);
}
bool ZSettingVirtualBGWrap::IsAllowToAddNewVBItem()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetVirtualBGSettings().IsAllowToAddNewVBItem();
}
bool ZSettingVirtualBGWrap::IsAllowToRemoveVBItem()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetVirtualBGSettings().isAllowToRemoveVBItem();
}
ZNSDKError ZSettingVirtualBGWrap::AddBGImage(ZoomSTRING filePath)
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetVirtualBGSettings().AddBGImage(filePath.c_str());
	return Map2WrapDefine(sdkerr);
}
ZNSDKError ZSettingVirtualBGWrap::RemoveBGImage(int64_t imageHandle)
{
	if (!isValidImageItem(imageHandle))
	{
		return ZNSDKERR_INVALID_PARAMETER;
	}
	ZOOM_SDK_NAMESPACE::IVirtualBGImageInfo* pRemoveImage = (ZOOM_SDK_NAMESPACE::IVirtualBGImageInfo*)imageHandle;
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetVirtualBGSettings().RemoveBGImage(pRemoveImage);
	if (sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_SUCCESS)
	{
		for (int i = 0; i < zn_bg_image_list.size(); ++i) {
			if (zn_bg_image_list[i].imageHandle == imageHandle) {
				zn_bg_image_list.erase(zn_bg_image_list.begin() + i);
				break;
			}
		}
	}
	return Map2WrapDefine(sdkerr);
}
ZNList<ZNVirtualBGImageInfo> ZSettingVirtualBGWrap::GetBGImageList()
{
	zn_bg_image_list.clear();
	ZOOM_SDK_NAMESPACE::IList<ZOOM_SDK_NAMESPACE::IVirtualBGImageInfo* >* sdk_list = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetVirtualBGSettings().GetBGImageList();
	if (NULL == sdk_list)
	{
		return zn_bg_image_list;
	}
	int count = sdk_list->GetCount();
	for (int index = 0; index < count; index++)
	{
		ZNVirtualBGImageInfo zn_imageinfo = ConvertSDKVirtualBGImageInfoToZNVirtualBGImageInfo(sdk_list->GetItem(index));
		zn_bg_image_list.push_back(zn_imageinfo);
	}
	return zn_bg_image_list;
}
ZNSDKError ZSettingVirtualBGWrap::UseBGImage(int64_t imageHandle)
{
	if (!isValidImageItem(imageHandle))
	{
		return ZNSDKERR_INVALID_PARAMETER;
	}
	ZOOM_SDK_NAMESPACE::IVirtualBGImageInfo* pImage = (ZOOM_SDK_NAMESPACE::IVirtualBGImageInfo*)imageHandle;
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetVirtualBGSettings().UseBGImage(pImage);
	return Map2WrapDefine(sdkerr);
}
ZNZoomSDKColor ZSettingVirtualBGWrap::GetBGReplaceColor()
{
	ZNZoomSDKColor zn_color;
	DWORD color = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetVirtualBGSettings().GetBGReplaceColor();
	zn_color.red   = (color >> 16) & 0xFF;
    zn_color.green = (color >> 8)  & 0xFF;
    zn_color.blue  = (color)       & 0xFF;
	return zn_color;
}
ZNSDKError ZSettingVirtualBGWrap::BeginSelectReplaceVBColor()
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetVirtualBGSettings().BeginSelectReplaceVBColor();
	return Map2WrapDefine(sdkerr);
}
ZNSDKError ZSettingVirtualBGWrap::AddBGVideo(ZoomSTRING filePath)
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetVirtualBGSettings().AddBGVideo(filePath.c_str());
	return Map2WrapDefine(sdkerr);
}
ZNSDKError ZSettingVirtualBGWrap::RemoveBGVideo(int64_t imageHandle)
{
	if (!isValidVideoItem(imageHandle))
	{
		return ZNSDKERR_INVALID_PARAMETER;
	}
	ZOOM_SDK_NAMESPACE::IVirtualBGImageInfo* pRemoveVideo = (ZOOM_SDK_NAMESPACE::IVirtualBGImageInfo*)imageHandle;
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetVirtualBGSettings().RemoveBGVideo(pRemoveVideo);
	if (sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_SUCCESS)
	{
		for (int i = 0; i < zn_bg_video_list.size(); ++i) {
			if (zn_bg_video_list[i].imageHandle == imageHandle) {
				zn_bg_video_list.erase(zn_bg_video_list.begin() + i);
				break;
			}
		}
	}
	return Map2WrapDefine(sdkerr);
}
ZNList<ZNVirtualBGImageInfo> ZSettingVirtualBGWrap::GetBGVideoList()
{
	zn_bg_video_list.clear();
	ZOOM_SDK_NAMESPACE::IList<ZOOM_SDK_NAMESPACE::IVirtualBGImageInfo* >* sdk_list = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetVirtualBGSettings().GetBGVideoList();
	if (NULL == sdk_list)
	{
		return zn_bg_video_list;
	}
	int count = sdk_list->GetCount();
	for (int index = 0; index < count; index++)
	{
		ZNVirtualBGImageInfo zn_imageinfo = ConvertSDKVirtualBGImageInfoToZNVirtualBGImageInfo(sdk_list->GetItem(index));
		zn_bg_video_list.push_back(zn_imageinfo);
	}
	return zn_bg_video_list;
}
ZNSDKError ZSettingVirtualBGWrap::UseBGVideo(int64_t imageHandle)
{
	if (!isValidVideoItem(imageHandle))
	{
		return ZNSDKERR_INVALID_PARAMETER;
	}
	ZOOM_SDK_NAMESPACE::IVirtualBGImageInfo* pImage = (ZOOM_SDK_NAMESPACE::IVirtualBGImageInfo*)imageHandle;
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetVirtualBGSettings().UseBGVideo(pImage);
	return Map2WrapDefine(sdkerr);
}

// Event Callbacks
void ZSettingVirtualBGWrap::onVBImageDidDownloaded()
{
	if (m_pSink)
	{
		m_pSink->onVBImageDidDownloaded();
	}
}
void ZSettingVirtualBGWrap::onGreenVBDidUpdateWithReplaceColor(ZNZoomSDKColor selectedColor)
{
	if (m_pSink)
	{
		m_pSink->onGreenVBDidUpdateWithReplaceColor(selectedColor);
	}
}
void ZSettingVirtualBGWrap::onSelectedVBImageChanged()
{
	if (m_pSink)
	{
		m_pSink->onSelectedVBImageChanged();
	}
}
void ZSettingVirtualBGWrap::onVBVideoUploadedResult(bool success, ZNVBVideoError zn_error)
{
	if (m_pSink)
	{
		m_pSink->onVBVideoUploadedResult(success, zn_error);
	}
}