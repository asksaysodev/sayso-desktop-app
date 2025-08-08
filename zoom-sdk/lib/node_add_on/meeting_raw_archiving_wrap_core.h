#pragma once
#include "zoom_native_sdk_wrap_core_def.h"
#include "zoom_sinks_wrap_class.h"

class ZMeetingRawArchivingWrap
{
public:
	ZMeetingRawArchivingWrap();
	virtual ~ZMeetingRawArchivingWrap();
	void Init();
	void Uninit();

	ZNSDKError StartRawArchiving();
	ZNSDKError StopRawArchiving();
};
