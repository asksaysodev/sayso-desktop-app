#pragma once
#include "zoom_native_sdk_wrap_core_def.h"
#include "zoom_sinks_wrap_class.h"
class ZSettingWallpaperWrap
{
public:
	ZSettingWallpaperWrap();
	virtual ~ZSettingWallpaperWrap();
	void Init();
	void Uninit();

	void SetSink(ZNativeSDKWallpaperSettingContextWrapSink* pSink);
	bool IsMeetingWallpaperEnabled();
	bool IsMeetingWallpaperThumbsReady();
	ZNWallpaperItem GetCurrentMeetingWallpaperItem();
	ZNList<ZNWallpaperItem> GetMeetingWallpaperList();
	ZNSDKError SetMeetingWallpaper(ZoomSTRING wallpaperId, int transparency, ZNZoomSDKWallpaperLayoutMode mode);
	ZNWallpaperItem GetMeetingWallpaperItemByID(ZoomSTRING wallpaperId);
	bool IsPersonalWallpaperEnabled();
	ZNWallpaperItem GetCurrentPersonalWallpaperItem();
	ZNList<ZNWallpaperItem> GetPersonalWallpaperList();
	ZNSDKError SetPersonalWallpaper(ZoomSTRING wallpaperId, int transparency, ZNZoomSDKWallpaperLayoutMode mode);
	ZNWallpaperItem GetPersonalWallpaperItemByID(ZoomSTRING wallpaperId);

	//callback
	void onMeetingWallpaperChanged(ZNWallpaperItem item);
	void onMeetingWallpaperImageDownloadStatus(ZNZoomSDKWallpaperSettingStatus status, ZoomSTRING wallpaperId);
	void onPersonalWallpaperChanged(ZNWallpaperItem item);
	void onPersonalWallpaperImageDownloadStatus(ZNZoomSDKWallpaperSettingStatus status, ZoomSTRING wallpaperId);

private:
	ZNativeSDKWallpaperSettingContextWrapSink* m_pSink;
};
