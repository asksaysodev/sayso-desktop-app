#pragma once
#pragma once
#include "common_include.h"
BEGIN_ZOOM_SDK_NAMESPACE
class ISettingServiceWrap;
IWallpaperSettingContext* InitIWallpaperSettingContextFunc(IWallpaperSettingContextEvent* pEvent, ISettingServiceWrap* pOwner);
void UninitIWallpaperSettingContextFunc(IWallpaperSettingContext* obj);
BEGIN_CLASS_DEFINE_WITHCALLBACK(IWallpaperSettingContext, IWallpaperSettingContextEvent)
NORMAL_CLASS(IWallpaperSettingContext)
INIT_UNINIT_WITHEVENT_AND_OWNSERVICE(IWallpaperSettingContext, ISettingServiceWrap)
virtual SDKError SetEvent(IWallpaperSettingContextEvent* pEvent)
{
	external_cb = pEvent;
	return SDKERR_SUCCESS;
}

//virtual bool IsMeetingWallpaperEnabled() = 0;
DEFINE_FUNC_0(IsMeetingWallpaperEnabled, bool)
//virtual bool IsMeetingWallpaperThumbsReady() = 0;
DEFINE_FUNC_0(IsMeetingWallpaperThumbsReady, bool)
//virtual IWallpaperItem* GetCurrentMeetingWallpaperItem() = 0;
DEFINE_FUNC_0(GetCurrentMeetingWallpaperItem, IWallpaperItem*)
//virtual IList<IWallpaperItem* >* GetMeetingWallpaperList() = 0;
DEFINE_FUNC_0(GetMeetingWallpaperList, IList<IWallpaperItem* >*)
//virtual SDKError SetMeetingWallpaper(IWallpaperItem* item) = 0;
DEFINE_FUNC_1(SetMeetingWallpaper, SDKError, IWallpaperItem*, item)
//virtual IWallpaperItem* GetMeetingWallpaperItemByID(const zchar_t* wallpaperID) = 0;
DEFINE_FUNC_1(GetMeetingWallpaperItemByID, IWallpaperItem*, const zchar_t*, wallpaperID)
#if defined(WIN32)
//virtual bool IsPersonalWallpaperEnabled() = 0;
DEFINE_FUNC_0(IsPersonalWallpaperEnabled, bool)
//virtual IWallpaperItem* GetCurrentPersonalWallpaperItem() = 0;
DEFINE_FUNC_0(GetCurrentPersonalWallpaperItem, IWallpaperItem*)
//virtual IList<IWallpaperItem* >* GetPersonalWallpaperList() = 0;
DEFINE_FUNC_0(GetPersonalWallpaperList, IList<IWallpaperItem* >*)
//virtual SDKError SetPersonalWallpaper(IWallpaperItem* item) = 0;
DEFINE_FUNC_1(SetPersonalWallpaper, SDKError, IWallpaperItem*, item)
//virtual IWallpaperItem* GetPersonalWallpaperItemByID(const zchar_t* wallpaperID) = 0;
DEFINE_FUNC_1(GetPersonalWallpaperItemByID, IWallpaperItem*, const zchar_t*, wallpaperID)
#endif

//virtual void onMeetingWallpaperChanged(IWallpaperItem* item) = 0;
CallBack_FUNC_1(onMeetingWallpaperChanged, IWallpaperItem*, item)
//virtual void onMeetingWallpaperImageDownloadStatus(ZoomSDKWallpaperSettingStatus status, const zchar_t* wallpaperId) = 0;
CallBack_FUNC_2(onMeetingWallpaperImageDownloadStatus, ZoomSDKWallpaperSettingStatus, status, const zchar_t*, wallpaperId)
#if defined(WIN32)
//virtual void onPersonalWallpaperChanged(IWallpaperItem* item) = 0;
CallBack_FUNC_1(onPersonalWallpaperChanged, IWallpaperItem*, item)
//virtual void onPersonalWallpaperImageDownloadStatus(ZoomSDKWallpaperSettingStatus status, const zchar_t* wallpaperId) = 0;
CallBack_FUNC_2(onPersonalWallpaperImageDownloadStatus, ZoomSDKWallpaperSettingStatus, status, const zchar_t*, wallpaperId)
#endif

END_CLASS_DEFINE(IWallpaperSettingContext)
END_ZOOM_SDK_NAMESPACE
