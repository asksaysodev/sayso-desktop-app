#include "meeting_reaction_wrap_core.h"

#include "meeting_service_components_wrap/meeting_qa_wrap.h"
#include "sdk_events_wrap_class.h"
#include "sdk_wrap.h"
#include "zoom_native_to_wrap.h"

extern ZOOM_SDK_NAMESPACE::IMeetingServiceWrap &g_meeting_service_wrap;

ZMeetingReactionWrap::ZMeetingReactionWrap() : m_pSink(NULL) 
{
	SDKEventWrapMgr::GetInst().m_emojiReactionControllerEvent.SetOwner(this);
	m_pSink = 0;
}

ZMeetingReactionWrap::~ZMeetingReactionWrap() 
{
	Uninit();
	m_pSink = 0;
	SDKEventWrapMgr::GetInst().m_emojiReactionControllerEvent.SetOwner(NULL);
}

void ZMeetingReactionWrap::SetSink(ZNativeSDKMeetingReactionWrapSink *pSink) 
{
	m_pSink = pSink;
}

void ZMeetingReactionWrap::Init() 
{
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingEmojiReactionController().Init_Wrap(&g_meeting_service_wrap);
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingEmojiReactionController().SetEvent(&SDKEventWrapMgr::GetInst().m_emojiReactionControllerEvent);
}

void ZMeetingReactionWrap::Uninit() 
{
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingEmojiReactionController().SetEvent(NULL);
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingEmojiReactionController().Uninit_Wrap();
}

void ZMeetingReactionWrap::onEmojiReactionReceived(unsigned int userid, ZNSDKEmojiReactionType type, ZNSDKEmojiReactionSkinTone skinTone) 
{
	if (m_pSink) {
		m_pSink->onEmojiReactionReceived(userid, type, skinTone);
	}
}

void ZMeetingReactionWrap::onEmojiReactionReceivedInWebinar(ZNSDKEmojiReactionType type) 
{
	if (m_pSink) {
		m_pSink->onEmojiReactionReceivedInWebinar(type);
	}
	return;
}