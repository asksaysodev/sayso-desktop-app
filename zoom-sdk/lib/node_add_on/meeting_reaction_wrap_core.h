#pragma once
#include "zoom_native_sdk_wrap_core_def.h"
#include "zoom_sinks_wrap_class.h"

class ZMeetingReactionWrap {
public:
	ZMeetingReactionWrap();
	virtual ~ZMeetingReactionWrap();
	void Init();
	void Uninit();
	void SetSink(ZNativeSDKMeetingReactionWrapSink *pSink);

	// interface methods

	// callbacks
	void onEmojiReactionReceived(unsigned int userid, ZNSDKEmojiReactionType type, ZNSDKEmojiReactionSkinTone skinTone);
	void onEmojiReactionReceivedInWebinar(ZNSDKEmojiReactionType type);

private:
	ZNativeSDKMeetingReactionWrapSink *m_pSink;
};