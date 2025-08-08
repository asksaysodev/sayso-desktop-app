#include "test_audio_device_helper_wrap.h"
#include "audio_setting_context_wrap.h"
BEGIN_ZOOM_SDK_NAMESPACE
ITestAudioDeviceHelper* InitITestAudioDeviceHelperFunc(ITestAudioDeviceHelperEvent* pEvent, IAudioSettingContextWrap* pOwner)
{
	if (pOwner && pOwner->GetSDKObj())
	{
		ZOOM_SDK_NAMESPACE::ITestAudioDeviceHelper* pObj = pOwner->GetSDKObj()->GetTestAudioDeviceHelper();
		if (pObj)
		{
			pObj->SetEvent(pEvent);
		}
		return pObj;
	}

	return NULL;
}

void UninitITestAudioDeviceHelperFunc(ITestAudioDeviceHelper* obj)
{
	if (obj)
	{
		obj->SetEvent(NULL);
	}
}

//virtual SDKError TestMicStartRecording(const zchar_t* deviceID = nullptr);
IMPL_FUNC_1(ITestAudioDeviceHelper, TestMicStartRecording, SDKError, const zchar_t*, deviceID, SDKERR_UNINITIALIZE)
//virtual SDKError TestMicStopTesting();
IMPL_FUNC_0(ITestAudioDeviceHelper, TestMicStopTesting, SDKError, SDKERR_UNINITIALIZE)
//virtual SDKError TestMicPlayRecording();
IMPL_FUNC_0(ITestAudioDeviceHelper, TestMicPlayRecording, SDKError, SDKERR_UNINITIALIZE)
//virtual SDKError TestSpeakerStartPlaying(const zchar_t* deviceID = nullptr);
IMPL_FUNC_1(ITestAudioDeviceHelper, TestSpeakerStartPlaying, SDKError, const zchar_t*, deviceID, SDKERR_UNINITIALIZE)
//virtual SDKError TestSpeakerStopPlaying();
IMPL_FUNC_0(ITestAudioDeviceHelper, TestSpeakerStopPlaying, SDKError, SDKERR_UNINITIALIZE)
//virtual SDKError SetTimerInterval(unsigned int timerInterval);
IMPL_FUNC_1(ITestAudioDeviceHelper, SetTimerInterval, SDKError, unsigned int, timerInterval, SDKERR_UNINITIALIZE)

END_ZOOM_SDK_NAMESPACE
