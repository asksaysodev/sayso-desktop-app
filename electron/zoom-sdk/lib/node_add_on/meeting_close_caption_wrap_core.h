#pragma once
#include "zoom_native_sdk_wrap_core_def.h"
#include "zoom_sinks_wrap_class.h"

class ZMeetingCloseCaptionWrap {
public:
	ZMeetingCloseCaptionWrap();
	virtual ~ZMeetingCloseCaptionWrap();

	void Init();
	void Uninit();
	void SetSink(ZNativeSDKCloseCaptionWrapSink* pSink);

	// interface methods

	// callbacks
	void onLiveTranscriptionMsgInfoReceived(ZNLiveTranscriptionMessageInfo messageInfo);
	void onOriginalLanguageMsgReceived(ZNLiveTranscriptionMessageInfo messageInfo);
private:
	ZNativeSDKCloseCaptionWrapSink* m_pSink;
};