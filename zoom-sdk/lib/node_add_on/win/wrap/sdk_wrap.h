#pragma once
#include "auth_service_wrap.h"
#include "meeting_service_wrap.h"
#include "setting_service_wrap.h"
#include "network_connection_handler_wrap.h"
#if defined(WIN32)
#include "ui_hook_wrap.h"
#include "customized_ui_components_wrap/customized_ui_mgr_wrap.h"
#include "customized_resource_helper_wrap.h"
#endif
#include "rawdata_audio_helper_wrap.h"
#include "rawdata_render_wrap.h"
#include "rawdata_video_helper_wrap.h"

BEGIN_ZOOM_SDK_NAMESPACE
class CSDKWrap
{
public:
	static CSDKWrap& GetInst();
	SDKError InitSDK(const zchar_t* sdkpath, InitParam& initParam);
	SDKError CleanUPSDK();
	const zchar_t* GetSDKVersion();
	INetworkConnectionHelperWrap& GetNetworkConnectionHelperWrap();
	IAuthServiceWrap& GetAuthServiceWrap();
	IMeetingServiceWrap& GetMeetingServiceWrap();
	ISettingServiceWrap& GetSettingServiceWrap();

	IZoomSDKVideoSourceHelperWrap& GetRawdataVideoSourceHelperWrap();
	IZoomSDKAudioRawDataHelperWrap& GetAudioRawdataHelperWrap();

private:
	CSDKWrap();
};

class CSDKExtWrap
{
public:
	static CSDKExtWrap& GetInst();
#if defined(WIN32)
	IUIHookerWrap& GetUIHookerWrap();
	ICustomizedResourceHelperWrap& GetCustomizedResourceHelperWrap();
#endif

private:
	CSDKExtWrap();
};

#if defined(WIN32)
class CSDKCustomizedUIWrap
{
public:
	static CSDKCustomizedUIWrap& GetInst();
	ICustomizedUIMgrWrap& GetCustomizedUIMgrWrap();
private:
	CSDKCustomizedUIWrap();
};
#endif

class CSDKRendererWrap
{
public:
	static CSDKRendererWrap& GetInst();
	IZoomSDKRendererWrap& GetRendererWrap();
private:
	CSDKRendererWrap();
};

END_ZOOM_SDK_NAMESPACE