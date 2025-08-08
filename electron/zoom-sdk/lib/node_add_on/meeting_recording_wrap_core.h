#pragma once
#include "zoom_native_sdk_wrap_core_def.h"
#include "zoom_sinks_wrap_class.h"

class ZMeetingRecordingWrap
{
public:
	ZMeetingRecordingWrap();
	virtual ~ZMeetingRecordingWrap();
	void Init();
	void Uninit();
	void SetSink(ZNativeSDKMeetingRecordingWrapSink* pSink);

	ZNSDKError StartRecording(time_t& startTimestamp);
	ZNSDKError StopRecording(time_t& stopTimestamp);
	ZNSDKError CanStartRecording(bool cloud_recording, unsigned int userid);
	ZNSDKError CanAllowDisAllowLocalRecording();
	ZNSDKError StartCloudRecording();
	ZNSDKError StopCloudRecording();
	ZNSDKError IsSupportLocalRecording(unsigned int userid);
	ZNSDKError AllowLocalRecording(unsigned int userid);
	ZNSDKError DisAllowLocalRecording(unsigned int userid);
	ZNSDKError RequestStartCloudRecording();
	bool IsSmartRecordingEnabled();
	bool CanEnableSmartRecordingFeature();
	ZNSDKError EnableSmartRecording();

	unsigned int GetRequesterId();
	ZoomSTRING GetRequesterName();
	ZNSDKError Start();
	ZNSDKError Deny(bool bDenyAll);

	unsigned int GetRequestEnableAndStartSmartRecordingUserId();
	ZoomSTRING GetRequestEnableAndStartSmartRecordingTipString();
	ZNSDKError StartCloudRecordingWithoutEnableSmartRecording();
	ZNSDKError AgreeToEnableAndStartSmartRecording(bool bAllMeetings);
	ZNSDKError DeclineEnableAndStartSmartRecording(bool bDenyAll);

	ZoomSTRING GetSmartRecordingEnableActionTipString();
	ZNSDKError ActionConfirmEnableSmartRecording(bool bAllMeetings);
	ZNSDKError ActionCancelEnableSmartRecording();
	
	//callback
	void onRecording2MP4Done(bool bsuccess, int iResult, ZoomSTRING szPath);
	void onRecording2MP4Processing(int iPercentage);
	void onRecordingStatus(ZNRecordingStatus status);
	void onCloudRecordingStatus(ZNRecordingStatus status);
	void onRecordPrivilegeChanged(bool bCanRec);
	void onCloudRecordingStorageFull(time_t gracePeriodDate);
	void onRequestCloudRecordingResponse(ZNRequestStartCloudRecordingStatus status);
	void onStartCloudRecordingRequested();
	void onEnableAndStartSmartRecordingRequested();
	void onSmartRecordingEnableActionCallback();
private:
	ZNativeSDKMeetingRecordingWrapSink* m_pSink;

};
