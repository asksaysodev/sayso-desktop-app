#pragma once
#include "zoom_native_sdk_wrap_core_def.h"
#include "zoom_sinks_wrap_class.h"

class ZMeetingAudioWrap
{
public:
	ZMeetingAudioWrap();
	virtual ~ZMeetingAudioWrap();
	void Init();
	void Uninit();
	void SetSink(ZNativeSDKMeetingAudioWrapSink* pSink);

	ZNSDKError MuteAudio(unsigned int userid, bool allowUnmuteBySelf);
	ZNSDKError UnMuteAudio(unsigned int userid);
	ZNSDKError JoinVoip();
	ZNSDKError LeaveVoip();
	ZNSDKError EnablePlayChimeWhenEnterOrExit(bool bEnable);
	ZNSDKError EnableMuteOnEntry(bool bEnable, bool allowUnmuteBySelf);
	bool CanEnableMuteOnEntry();
	bool IsMuteOnEntryEnabled();
	ZNSDKError EnablePlayMeetingAudio(bool bEnable);
	bool IsPlayMeetingAudioEnabled();

	//callback
	void onUserAudioStatusChange(ZNList<ZNUserAudioStatus> lstAudioStatusChange, ZoomSTRING strAudioStatusList);
	void onUserActiveAudioChange(ZNList<unsigned int > lstActiveAudio);
	void onMuteOnEntryStatusChange(bool bEnabled);

private:
	ZNativeSDKMeetingAudioWrapSink* m_pSink;

};
