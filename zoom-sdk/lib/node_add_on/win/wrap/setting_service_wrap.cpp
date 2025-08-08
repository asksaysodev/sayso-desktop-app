#pragma once
#include "setting_service_wrap.h"
BEGIN_ZOOM_SDK_NAMESPACE
ZOOM_SDK_NAMESPACE::ISettingService* InitISettingServiceFunc()
{
	ZOOM_SDK_NAMESPACE::ISettingService* pObj(NULL);
	CSDKImpl::GetInst().CreateSettingService(&pObj);
	return pObj;
}

void UninitISettingServiceFunc(ZOOM_SDK_NAMESPACE::ISettingService* obj)
{
	CSDKImpl::GetInst().DestroySettingService(obj);
}

#if defined(WIN32)
//virtual SDKError ShowSettingDlg(ShowSettingDlgParam& param) = 0;
IMPL_FUNC_1(ISettingService, ShowSettingDlg, SDKError, ShowSettingDlgParam&, param, SDKERR_UNINITIALIZE)
//virtual SDKError HideSettingDlg() = 0;
IMPL_FUNC_0(ISettingService, HideSettingDlg, SDKError, SDKERR_UNINITIALIZE)
//virtual IGeneralSettingContext* GetGeneralSettings() = 0;
IMPL_FUNC_0(ISettingService, GetGeneralSettings, IGeneralSettingContext*, NULL)
//virtual IAccessibilitySettingContext* GetAccessibilitySettings() = 0;
IMPL_FUNC_0(ISettingService, GetAccessibilitySettings, IAccessibilitySettingContext*, NULL)
IMPL_FUNC_0(ISettingService, GetSettingUIStrategy, ISettingUIStrategy*, NULL)
#endif
//virtual IStatisticSettingContext* GetStatisticSettings() = 0;
IMPL_FUNC_0(ISettingService, GetStatisticSettings, IStatisticSettingContext*, NULL)
//virtual IShareSettingContext* GetShareSettings() = 0;
IMPL_FUNC_0(ISettingService, GetShareSettings, IShareSettingContext*, NULL)

#if (defined UserInterfaceClass)
//virtual IVideoSettingContext* GetVideoSettings() = 0;
IMPL_FUNC_0(ISettingService, GetVideoSettings, IVideoSettingContext*, NULL)
//virtual IAudioSettingContext* GetAudioSettings() = 0;
IMPL_FUNC_0(ISettingService, GetAudioSettings, IAudioSettingContext*, NULL)
//virtual IRecordingSettingContext* GetRecordingSettings() = 0;
IMPL_FUNC_0(ISettingService, GetRecordingSettings, IRecordingSettingContext*, NULL)
//virtual IWallpaperSettingContext* GetWallpaperSettings() = 0;
IMPL_FUNC_0(ISettingService, GetWallpaperSettings, IWallpaperSettingContext*, NULL)
//virtual IVirtualBGSettingContext* GetVirtualBGSettings() = 0;
IMPL_FUNC_0(ISettingService, GetVirtualBGSettings, IVirtualBGSettingContext*, NULL)

#endif
IMPL_FUNC_AND_MEMBER(ISettingService, T_GetAudioSettings, IAudioSettingContextWrap)
IMPL_FUNC_AND_MEMBER(ISettingService, T_GetVideoSettings, IVideoSettingContextWrap)
IMPL_FUNC_AND_MEMBER(ISettingService, T_GetRecordingSettings, IRecordingSettingContextWrap)
IMPL_FUNC_AND_MEMBER(ISettingService, T_GetWallpaperSettings, IWallpaperSettingContextWrap)
IMPL_FUNC_AND_MEMBER(ISettingService, T_GetVirtualBGSettings, IVirtualBGSettingContextWrap)
END_ZOOM_SDK_NAMESPACE
