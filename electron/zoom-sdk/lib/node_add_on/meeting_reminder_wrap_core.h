#pragma once
#include "zoom_native_sdk_wrap_core_def.h"
#include "zoom_sinks_wrap_class.h"

class ZMeetingReminderWrap
{
public:
	ZMeetingReminderWrap();
	virtual ~ZMeetingReminderWrap();
	void Init();
	void Uninit();
	void SetSink(ZNativeSDKMeetingReminderWrapSink* pSink);

	ZNSDKError Ignore();
	ZNSDKError Accept();
	ZNSDKError Decline();

	ZNSDKError SetEnableOption(ZNFeatureEnableOption option);
	ZNSDKError StartEnable();
	ZNSDKError DeclineEnable(bool bDeclineAll);
	ZNSDKError IgnoreEnable();

	//callback
	void onReminderNotify(ZNMeetingReminderContent content);
	void onEnableReminderNotify(ZNMeetingReminderContent content);
	
private:
	ZNativeSDKMeetingReminderWrapSink* m_pSink;
};
