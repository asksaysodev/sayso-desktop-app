#pragma once
#include "zoom_native_sdk_wrap_core_def.h"
#include "zoom_sinks_wrap_class.h"

class ZMeetingShareWrap
{
public:
	ZMeetingShareWrap();
	virtual ~ZMeetingShareWrap();
	void Init();
	void Uninit();
	void SetSink(ZNativeSDKMeetingShareWrapSink* pSink);

	void onSharingStatus(ZNZoomSDKSharingSourceInfo shareInfo);
	void onFailedToStartShare();
	void onOptimizingShareForVideoClipStatusChanged(ZNZoomSDKSharingSourceInfo shareInfo);

	ZNSDKError StartAppShare(ZoomSTRING hShareApp);
	ZNSDKError StartMonitorShare(ZoomSTRING monitorID);
	ZNSDKError StopShare();
	bool IsWhiteboardLegalNoticeAvailable();
	ZoomSTRING GetWhiteboardLegalNoticesPrompt();
	ZoomSTRING GetWhiteboardLegalNoticesExplained();
	ZNSDKError SetAudioShareMode(ZNAudioShareMode mode);
	ZNSDKError GetAudioShareMode(ZNAudioShareMode& mode);
private:
	ZNativeSDKMeetingShareWrapSink* m_pSink;
};
