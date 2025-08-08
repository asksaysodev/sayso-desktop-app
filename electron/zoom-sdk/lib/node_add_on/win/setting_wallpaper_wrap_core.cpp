#include "setting_wallpaper_wrap_core.h"
#include "sdk_wrap.h"
#include "wallpaper_setting_context_wrap.h"
#include "zoom_native_to_wrap.h"
#include "sdk_events_wrap_class.h"

extern ZOOM_SDK_NAMESPACE::ISettingServiceWrap& g_setting_service_wrap;

ZSettingWallpaperWrap::ZSettingWallpaperWrap()
{
	SDKEventWrapMgr::GetInst().m_wallpaperSettingContextWrapEvent.SetOwner(this);
	m_pSink = NULL;
}
ZSettingWallpaperWrap::~ZSettingWallpaperWrap()
{
	Uninit();
	m_pSink = NULL;
	SDKEventWrapMgr::GetInst().m_wallpaperSettingContextWrapEvent.SetOwner(NULL);
}
void ZSettingWallpaperWrap::Init()
{
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetWallpaperSettings().Init_Wrap(&g_setting_service_wrap);
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetWallpaperSettings().SetEvent(&SDKEventWrapMgr::GetInst().m_wallpaperSettingContextWrapEvent);
}
void ZSettingWallpaperWrap::Uninit()
{
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetWallpaperSettings().SetEvent(NULL);
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetWallpaperSettings().Uninit_Wrap();
}
void ZSettingWallpaperWrap::SetSink(ZNativeSDKWallpaperSettingContextWrapSink* pSink)
{
	m_pSink = pSink;
}
bool ZSettingWallpaperWrap::IsMeetingWallpaperEnabled()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetWallpaperSettings().IsMeetingWallpaperEnabled();
}
bool ZSettingWallpaperWrap::IsMeetingWallpaperThumbsReady()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetWallpaperSettings().IsMeetingWallpaperThumbsReady();
}
ZNWallpaperItem ZSettingWallpaperWrap::GetCurrentMeetingWallpaperItem()
{
	ZOOM_SDK_NAMESPACE::IWallpaperItem* wallpaperItem = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetWallpaperSettings().GetCurrentMeetingWallpaperItem();
	return ConvertSDKIWallpaperItemToZNWallpaperItem(wallpaperItem);
}
ZNList<ZNWallpaperItem> ZSettingWallpaperWrap::GetMeetingWallpaperList()
{
	ZNList<ZNWallpaperItem> zn_list;
	ZOOM_SDK_NAMESPACE::IList<ZOOM_SDK_NAMESPACE::IWallpaperItem* >* wallpaperList = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetWallpaperSettings().GetMeetingWallpaperList();
	if (wallpaperList && wallpaperList->GetCount() > 0)
	{
		int count = wallpaperList->GetCount();
		for (int i = 0; i < count; i++)
		{
			ZOOM_SDK_NAMESPACE::IWallpaperItem* wallpaperItem = wallpaperList->GetItem(i);
			if (wallpaperItem) {
				ZNWallpaperItem znItem = ConvertSDKIWallpaperItemToZNWallpaperItem(wallpaperItem);
				zn_list.push_back(znItem);
			}
		}
	}
	return zn_list;
}
ZNSDKError ZSettingWallpaperWrap::SetMeetingWallpaper(ZoomSTRING wallpaperId, int transparency, ZNZoomSDKWallpaperLayoutMode mode)
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_SUCCESS;
	ZOOM_SDK_NAMESPACE::IWallpaperItem* wallpaperItem = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetWallpaperSettings().GetMeetingWallpaperItemByID(wallpaperId.c_str());
	if (wallpaperItem)
	{
		wallpaperItem->SetTransparency(transparency);
		wallpaperItem->SetWallpaperLayoutMode(Map2SDKDefine(mode));
		sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetWallpaperSettings().SetMeetingWallpaper(wallpaperItem);
	} 
	else
	{
		sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_WRONG_USAGE;
	}
	return Map2WrapDefine(sdkerr);
}
ZNWallpaperItem ZSettingWallpaperWrap::GetMeetingWallpaperItemByID(ZoomSTRING wallpaperId)
{
	ZOOM_SDK_NAMESPACE::IWallpaperItem* wallpaperItem = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetWallpaperSettings().GetMeetingWallpaperItemByID(wallpaperId.c_str());
	return ConvertSDKIWallpaperItemToZNWallpaperItem(wallpaperItem);
}
bool ZSettingWallpaperWrap::IsPersonalWallpaperEnabled()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetWallpaperSettings().IsPersonalWallpaperEnabled();
}
ZNWallpaperItem ZSettingWallpaperWrap::GetCurrentPersonalWallpaperItem()
{
	ZOOM_SDK_NAMESPACE::IWallpaperItem* wallpaperItem = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetWallpaperSettings().GetCurrentPersonalWallpaperItem();
	return ConvertSDKIWallpaperItemToZNWallpaperItem(wallpaperItem);
}
ZNList<ZNWallpaperItem> ZSettingWallpaperWrap::GetPersonalWallpaperList()
{
	ZNList<ZNWallpaperItem> zn_list;
	ZOOM_SDK_NAMESPACE::IList<ZOOM_SDK_NAMESPACE::IWallpaperItem* >* wallpaperList = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetWallpaperSettings().GetPersonalWallpaperList();
	if (wallpaperList && wallpaperList->GetCount() > 0)
	{
		int count = wallpaperList->GetCount();
		for (int i = 0; i < count; i++)
		{
			ZOOM_SDK_NAMESPACE::IWallpaperItem* wallpaperItem = wallpaperList->GetItem(i);
			if (wallpaperItem) {
				ZNWallpaperItem znItem = ConvertSDKIWallpaperItemToZNWallpaperItem(wallpaperItem);
				zn_list.push_back(znItem);
			}
		}
	}
	return zn_list;
}
ZNSDKError ZSettingWallpaperWrap::SetPersonalWallpaper(ZoomSTRING wallpaperId, int transparency, ZNZoomSDKWallpaperLayoutMode mode)
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_SUCCESS;
	ZOOM_SDK_NAMESPACE::IWallpaperItem* wallpaperItem = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetWallpaperSettings().GetPersonalWallpaperItemByID(wallpaperId.c_str());
	if (wallpaperItem)
	{
		wallpaperItem->SetTransparency(transparency);
		wallpaperItem->SetWallpaperLayoutMode(Map2SDKDefine(mode));
		sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetWallpaperSettings().SetPersonalWallpaper(wallpaperItem);
	} 
	else
	{
		sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_WRONG_USAGE;
	}
	return Map2WrapDefine(sdkerr);
}
ZNWallpaperItem ZSettingWallpaperWrap::GetPersonalWallpaperItemByID(ZoomSTRING wallpaperId)
{
	ZOOM_SDK_NAMESPACE::IWallpaperItem* wallpaperItem = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetSettingServiceWrap().T_GetWallpaperSettings().GetPersonalWallpaperItemByID(wallpaperId.c_str());
	return ConvertSDKIWallpaperItemToZNWallpaperItem(wallpaperItem);
}

void ZSettingWallpaperWrap::onMeetingWallpaperChanged(ZNWallpaperItem item)
{
	if (m_pSink)
	{
		m_pSink->onMeetingWallpaperChanged(item);
	}
}
void ZSettingWallpaperWrap::onMeetingWallpaperImageDownloadStatus(ZNZoomSDKWallpaperSettingStatus status, ZoomSTRING wallpaperId)
{
	if (m_pSink)
	{
		m_pSink->onMeetingWallpaperImageDownloadStatus(status, wallpaperId);
	}
}
void ZSettingWallpaperWrap::onPersonalWallpaperChanged(ZNWallpaperItem item)
{
	if (m_pSink)
	{
		m_pSink->onPersonalWallpaperChanged(item);
	}
}
void ZSettingWallpaperWrap::onPersonalWallpaperImageDownloadStatus(ZNZoomSDKWallpaperSettingStatus status, ZoomSTRING wallpaperId)
{
	if (m_pSink)
	{
		m_pSink->onPersonalWallpaperImageDownloadStatus(status, wallpaperId);
	}
}