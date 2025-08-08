#include "wallpaper_setting_context_wrap.h"
#include "setting_service_wrap.h"
BEGIN_ZOOM_SDK_NAMESPACE
IWallpaperSettingContext* InitIWallpaperSettingContextFunc(IWallpaperSettingContextEvent* pEvent, ISettingServiceWrap* pOwner)
{
	if (pOwner && pOwner->GetSDKObj())
	{
		ZOOM_SDK_NAMESPACE::IWallpaperSettingContext* pObj = pOwner->GetSDKObj()->GetWallpaperSettings();
		if (pObj)
		{
			pObj->SetEvent(pEvent);
		}

		return pObj;
	}

	return NULL;
}

void UninitIWallpaperSettingContextFunc(IWallpaperSettingContext* pObj)
{
	if (pObj)
	{
		pObj->SetEvent(NULL);
	}
}

//virtual bool IsMeetingWallpaperEnabled() = 0;
IMPL_FUNC_0(IWallpaperSettingContext, IsMeetingWallpaperEnabled, bool, false)
//virtual bool IsMeetingWallpaperThumbsReady() = 0;
IMPL_FUNC_0(IWallpaperSettingContext, IsMeetingWallpaperThumbsReady, bool, false)
//virtual IWallpaperItem* GetCurrentMeetingWallpaperItem() = 0;
IMPL_FUNC_0(IWallpaperSettingContext, GetCurrentMeetingWallpaperItem, IWallpaperItem*, NULL)
//virtual IList<IWallpaperItem* >* GetMeetingWallpaperList() = 0;
IMPL_FUNC_0(IWallpaperSettingContext, GetMeetingWallpaperList, IList<IWallpaperItem* >*, NULL)
//virtual SDKError SetMeetingWallpaper(IWallpaperItem* item) = 0;
IMPL_FUNC_1(IWallpaperSettingContext, SetMeetingWallpaper, SDKError, IWallpaperItem*, item, SDKERR_UNINITIALIZE)
//virtual IWallpaperItem* GetMeetingWallpaperItemByID(const zchar_t* wallpaperID) = 0;
IMPL_FUNC_1(IWallpaperSettingContext, GetMeetingWallpaperItemByID, IWallpaperItem*, const zchar_t*, wallpaperID, NULL)
#if defined(WIN32)
//virtual bool IsPersonalWallpaperEnabled() = 0;
IMPL_FUNC_0(IWallpaperSettingContext, IsPersonalWallpaperEnabled, bool, false)
//virtual IWallpaperItem* GetCurrentPersonalWallpaperItem() = 0;
IMPL_FUNC_0(IWallpaperSettingContext, GetCurrentPersonalWallpaperItem, IWallpaperItem*, NULL)
//virtual IList<IWallpaperItem* >* GetPersonalWallpaperList() = 0;
IMPL_FUNC_0(IWallpaperSettingContext, GetPersonalWallpaperList, IList<IWallpaperItem* >*, NULL)
//virtual SDKError SetPersonalWallpaper(IWallpaperItem* item) = 0;
IMPL_FUNC_1(IWallpaperSettingContext, SetPersonalWallpaper, SDKError, IWallpaperItem*, item, SDKERR_UNINITIALIZE)
//virtual IWallpaperItem* GetPersonalWallpaperItemByID(const zchar_t* wallpaperID) = 0;
IMPL_FUNC_1(IWallpaperSettingContext, GetPersonalWallpaperItemByID, IWallpaperItem*, const zchar_t*, wallpaperID, NULL)
#endif

END_ZOOM_SDK_NAMESPACE
