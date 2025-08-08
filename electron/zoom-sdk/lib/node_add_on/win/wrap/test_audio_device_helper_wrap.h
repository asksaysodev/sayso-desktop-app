#pragma once
#include "common_include.h"
BEGIN_ZOOM_SDK_NAMESPACE
class IAudioSettingContextWrap;
ITestAudioDeviceHelper* InitITestAudioDeviceHelperFunc(ITestAudioDeviceHelperEvent* pEvent, IAudioSettingContextWrap* pOwner);
void UninitITestAudioDeviceHelperFunc(ITestAudioDeviceHelper* obj);
BEGIN_CLASS_DEFINE_WITHCALLBACK(ITestAudioDeviceHelper, ITestAudioDeviceHelperEvent)
NORMAL_CLASS(ITestAudioDeviceHelper)
INIT_UNINIT_WITHEVENT_AND_OWNSERVICE(ITestAudioDeviceHelper, IAudioSettingContextWrap)
virtual void SetEvent(ITestAudioDeviceHelperEvent* pEvent)
{
	external_cb = pEvent;
}
//virtual SDKError TestMicStartRecording(const zchar_t* deviceID = nullptr);
DEFINE_FUNC_1(TestMicStartRecording, SDKError, const zchar_t*, deviceID)
//virtual SDKError TestMicStopTesting();
DEFINE_FUNC_0(TestMicStopTesting, SDKError)
//virtual SDKError TestMicPlayRecording();
DEFINE_FUNC_0(TestMicPlayRecording, SDKError)
//virtual SDKError TestSpeakerStartPlaying(const zchar_t* deviceID = nullptr);
DEFINE_FUNC_1(TestSpeakerStartPlaying, SDKError, const zchar_t*, deviceID)
//virtual SDKError TestSpeakerStopPlaying();
DEFINE_FUNC_0(TestSpeakerStopPlaying, SDKError)
//virtual SDKError SetTimerInterval(unsigned int timerInterval);
DEFINE_FUNC_1(SetTimerInterval, SDKError, unsigned int, timerInterval)

//virtual void OnMicSpkVolumeChanged(unsigned int MicVolume, unsigned int SpkVolume) = 0;
CallBack_FUNC_2(OnMicSpkVolumeChanged, unsigned int, MicVolume, unsigned int, SpkVolume)
//virtual void OnNoAudioDeviceIsUseful(bool bMicOrSpk) = 0; 
CallBack_FUNC_1(OnNoAudioDeviceIsUseful, bool, bMicOrSpk)
//virtual void OnTestMicStatusChanged(SDK_TESTMIC_STATUS status,bool& bHandled) = 0; 
CallBack_FUNC_2(OnTestMicStatusChanged, SDK_TESTMIC_STATUS, status, bool&, bHandled)
//virtual void OnSelectedAudioDeviceIsChanged() = 0;
CallBack_FUNC_0(OnSelectedAudioDeviceIsChanged)

END_CLASS_DEFINE(ITestAudioDeviceHelper)
END_ZOOM_SDK_NAMESPACE
