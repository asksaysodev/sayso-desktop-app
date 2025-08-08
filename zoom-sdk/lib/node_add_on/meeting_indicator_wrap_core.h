#pragma once
#include "zoom_native_sdk_wrap_core_def.h"
#include "zoom_sinks_wrap_class.h"

class ZMeetingIndicatorWrap
{
public:
	ZMeetingIndicatorWrap();
	virtual ~ZMeetingIndicatorWrap();
	void Init();
	void Uninit();
	void SetSink(ZNativeSDKMeetingIndicatorWrapSink* pSink);

	ZoomSTRING GetIndicatorItemId();
	ZoomSTRING GetIndicatorName();
	ZoomSTRING GetIndicatorIcon();
	ZNSDKError ShowIndicatorPanel(unsigned int x, unsigned int y, uint64_t windowId);
	ZNSDKError HideIndicatorPanel();

	//callback
	void onIndicatorItemReceived();
	void onIndicatorItemRemoved();
	
private:
	ZNativeSDKMeetingIndicatorWrapSink* m_pSink;
};
