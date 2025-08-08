#pragma once
#include "common_include.h"
BEGIN_ZOOM_SDK_NAMESPACE
IZoomSDKAudioRawDataHelper* InitIZoomSDKAudioRawDataHelperFunc(IZoomSDKAudioRawDataDelegate* pEvent);
void UninitIZoomSDKAudioRawDataHelperFunc(IZoomSDKAudioRawDataHelper* obj);

BEGIN_CLASS_DEFINE_WITHCALLBACK(IZoomSDKAudioRawDataHelper, IZoomSDKAudioRawDataDelegate)
STAITC_CLASS(IZoomSDKAudioRawDataHelper)
INIT_UNINIT_WITHEVENT(IZoomSDKAudioRawDataHelper)

SDKError SetEvent(IZoomSDKAudioRawDataDelegate* pEvent)
{
	external_cb = pEvent;
	return SDKERR_SUCCESS;
}
//virtual SDKError subscribe(IZoomSDKAudioRawDataDelegate* pDelegate) = 0;
DEFINE_FUNC_1(subscribe, SDKError, IZoomSDKAudioRawDataDelegate*, pDelegate)
//virtual SDKRawDataError unSubscribe() = 0;
DEFINE_FUNC_0(unSubscribe, SDKError)


//virtual void onMixedAudioRawDataReceived(AudioRawData* data_) = 0;
CallBack_FUNC_1(onMixedAudioRawDataReceived, AudioRawData*, data_)
//virtual void onOneWayAudioRawDataReceived(AudioRawData* data_, unsigned int user_id) = 0;
CallBack_FUNC_2(onOneWayAudioRawDataReceived, AudioRawData*, data_, unsigned int, user_id)
//virtual void onShareAudioRawDataReceived(AudioRawData* data_, uint32_t user_id) = 0;
CallBack_FUNC_2(onShareAudioRawDataReceived, AudioRawData*, data_, unsigned int, user_id)
//virtual void onOneWayInterpreterAudioRawDataReceived(AudioRawData* data_, const zchar_t* pLanguageName) = 0;
CallBack_FUNC_2(onOneWayInterpreterAudioRawDataReceived, AudioRawData*, data_, const zchar_t*, pLanguageName)

END_CLASS_DEFINE(IZoomSDKAudioRawDataHelper)
END_ZOOM_SDK_NAMESPACE