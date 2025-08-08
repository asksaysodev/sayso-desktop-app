#pragma once
#include "zoom_native_sdk_wrap_core_def.h"
#include "zoom_sinks_wrap_class.h"

class ZSettingVirtualBGWrap
{
public:
	ZSettingVirtualBGWrap();
	virtual ~ZSettingVirtualBGWrap();
	void Init();
	void Uninit();
	void SetSink(ZNativeSDKVirtualBGSettingContextWrapSink* pSink);

	bool IsSupportVirtualBG();
	bool IsDeviceSupportSmartVirtualBG();
	bool IsSupportVirtualBackgroundVideo();
	bool IsDeviceSupportSmartVirtualBackgroundVideo();
	bool IsDeviceSupportGreenVirtualBackgroundVideo();
	bool IsUsingGreenScreenOn();
	ZNSDKError SetUsingGreenScreen(bool bUse);
	bool IsAllowToAddNewVBItem();
	bool IsAllowToRemoveVBItem();
	ZNSDKError AddBGImage(ZoomSTRING filePath);
	ZNSDKError RemoveBGImage(int64_t imageHandle);
	ZNList<ZNVirtualBGImageInfo> GetBGImageList();
	ZNSDKError UseBGImage(int64_t imageHandle);
	ZNZoomSDKColor GetBGReplaceColor();
	ZNSDKError BeginSelectReplaceVBColor();
	ZNSDKError AddBGVideo(ZoomSTRING filePath);
	ZNSDKError RemoveBGVideo(int64_t imageHandle);
	ZNList<ZNVirtualBGImageInfo> GetBGVideoList();
	ZNSDKError UseBGVideo(int64_t imageHandle);

	// callback
	void onVBImageDidDownloaded();
	void onGreenVBDidUpdateWithReplaceColor(ZNZoomSDKColor zn_selectedColor);
	void onSelectedVBImageChanged();
	void onVBVideoUploadedResult(bool success, ZNVBVideoError zn_error);

private:
	ZNativeSDKVirtualBGSettingContextWrapSink* m_pSink;
	ZNList<ZNVirtualBGImageInfo> zn_bg_image_list;
	bool isValidImageItem(int64_t imageHandle);
#ifdef _WIN32
	ZNList<ZNVirtualBGImageInfo> zn_bg_video_list;
	bool isValidVideoItem(int64_t imageHandle);
#endif
};
