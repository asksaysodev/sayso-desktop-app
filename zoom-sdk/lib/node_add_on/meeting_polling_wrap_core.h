#pragma once
#include "zoom_native_sdk_wrap_core_def.h"
#include "zoom_sinks_wrap_class.h"

class ZMeetingPollingWrap {
public:
	ZMeetingPollingWrap();
	virtual ~ZMeetingPollingWrap();
	void Init();
	void Uninit();
	void SetSink(ZNativeSDKPollingWrapSink* pSink);

	// interface method
	bool CanDoPolling();
	ZNList<ZNSDKPollingQuestionItem>GetPollingQuestionItemList(ZoomSTRING pollingID);
	ZNList<ZNSDKPollingAnswerItem> GetPollingRightAnswerItemList(ZoomSTRING pollingID);
	ZNList<ZNSDKPollingAnswerResultItem> GetPollingAnswerResultItemList(ZoomSTRING pollingID);
	ZNSDKPollingItem GetPollingItemByID(ZoomSTRING pollingID);
	ZNList<ZNSDKPollingItem> GetPollingItemList();
	ZNSDKError StartPolling(ZoomSTRING pollingID);
	ZNSDKError StopPolling(ZoomSTRING pollingID);
	ZNSDKError RestartPolling(ZoomSTRING pollingID);
	bool IsQuestionCaseSensitive(ZoomSTRING pollingID, ZoomSTRING questionID);
	ZoomSTRING GetPollingQuestionImagePath(ZoomSTRING pollingID, ZoomSTRING questionID);
	bool CanGetRightAnswerItemList(ZoomSTRING pollingID);

	// callbacks
	void onPollingStatusChanged(ZoomSTRING pollingID, ZNSDKPollingStatus status);
	void onPollingResultUpdated(ZoomSTRING pollingID);
	void onPollingListUpdated();
	void onPollingActionResult(ZNSDKPollingActionType actionType, ZoomSTRING pollingID, bool bSuccess, ZoomSTRING errorMsg);

private:
	ZNativeSDKPollingWrapSink* m_pSink;
};