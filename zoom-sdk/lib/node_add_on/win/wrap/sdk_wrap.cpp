#include "sdk_wrap.h"
#include "sdk_loader.h"
BEGIN_ZOOM_SDK_NAMESPACE
CSDKWrap& CSDKWrap::GetInst()
{
	static CSDKWrap wrap;
	return wrap;
}

SDKError CSDKWrap::InitSDK(const zchar_t* sdkpath, InitParam& initParam)
{
	SDKError sdkerr(SDKERR_MODULE_LOAD_FAILED);
	do 
	{
		if (!CSDKImpl::GetInst().ConfigSDKModule(sdkpath))
		{
			break;
		}
#if defined(WIN32)
		CSDKExtWrap::GetInst().GetCustomizedResourceHelperWrap().ApplyAll();
#endif
		sdkerr = CSDKImpl::GetInst().InitSDK(initParam);
	} while (false);

	return sdkerr;
}

SDKError CSDKWrap::CleanUPSDK()
{
	return CSDKImpl::GetInst().CleanUPSDK();
}

const zchar_t* CSDKWrap::GetSDKVersion()
{
	return CSDKImpl::GetInst().GetSDKVersion();
}

IAuthServiceWrap& CSDKWrap::GetAuthServiceWrap()
{
	return IAuthServiceWrap::GetInst();
}

IMeetingServiceWrap& CSDKWrap::GetMeetingServiceWrap()
{
	return IMeetingServiceWrap::GetInst();
}

ISettingServiceWrap& CSDKWrap::GetSettingServiceWrap()
{
	return ISettingServiceWrap::GetInst();
}

IZoomSDKVideoSourceHelperWrap& CSDKWrap::GetRawdataVideoSourceHelperWrap()
{
	return IZoomSDKVideoSourceHelperWrap::GetInst();
}

IZoomSDKAudioRawDataHelperWrap& CSDKWrap::GetAudioRawdataHelperWrap()
{
	return IZoomSDKAudioRawDataHelperWrap::GetInst();
}

INetworkConnectionHelperWrap& CSDKWrap::GetNetworkConnectionHelperWrap()
{
	return INetworkConnectionHelperWrap::GetInst();
}

CSDKWrap::CSDKWrap()
{
}
/////////////////////////////////////////////////////////////////////
CSDKExtWrap::CSDKExtWrap()
{

}

CSDKExtWrap& CSDKExtWrap::GetInst()
{
	static CSDKExtWrap inst;
	return inst;
}

#if defined(WIN32)
IUIHookerWrap& CSDKExtWrap::GetUIHookerWrap()
{
	return IUIHookerWrap::GetInst();
}

ICustomizedResourceHelperWrap& CSDKExtWrap::GetCustomizedResourceHelperWrap()
{
	return ICustomizedResourceHelperWrap::GetInst();
}

/////////////////////////////////////////////////////////////////////
CSDKCustomizedUIWrap::CSDKCustomizedUIWrap()
{

}

CSDKCustomizedUIWrap& CSDKCustomizedUIWrap::GetInst()
{
	static CSDKCustomizedUIWrap inst;
	return inst;
}

ICustomizedUIMgrWrap& CSDKCustomizedUIWrap::GetCustomizedUIMgrWrap()
{
	return ICustomizedUIMgrWrap::GetInst();
}
#endif
/////////////////////////////////////////////////////////////////////////
CSDKRendererWrap::CSDKRendererWrap()
{

}

CSDKRendererWrap& CSDKRendererWrap::GetInst()
{
	static CSDKRendererWrap inst;
	return inst;
}

IZoomSDKRendererWrap& CSDKRendererWrap::GetRendererWrap()
{
	return IZoomSDKRendererWrap::GetInst();
}


END_ZOOM_SDK_NAMESPACE