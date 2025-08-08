#include "meeting_close_caption_wrap_core.h"
#include "meeting_service_components_wrap/meeting_closedcaption_ctrl_wrap.h"
#include "sdk_events_wrap_class.h"
#include "sdk_wrap.h"
#include "zoom_native_to_wrap.h"

extern ZOOM_SDK_NAMESPACE::IMeetingServiceWrap& g_meeting_service_wrap;

ZMeetingCloseCaptionWrap::ZMeetingCloseCaptionWrap() : m_pSink(NULL) 
{
	SDKEventWrapMgr::GetInst().m_closeCaptionControllerEvent.SetOwner(this);
	m_pSink = 0;
}
ZMeetingCloseCaptionWrap::~ZMeetingCloseCaptionWrap() 
{
	Uninit();
	m_pSink = 0;
	SDKEventWrapMgr::GetInst().m_closeCaptionControllerEvent.SetOwner(NULL);
}

void ZMeetingCloseCaptionWrap::Init() 
{
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingClosedCaptionController().Init_Wrap(&g_meeting_service_wrap);
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingClosedCaptionController().SetEvent(&SDKEventWrapMgr::GetInst().m_closeCaptionControllerEvent);
}
void ZMeetingCloseCaptionWrap::Uninit() 
{
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingClosedCaptionController().SetEvent(NULL);
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingClosedCaptionController().Uninit_Wrap();
}
void ZMeetingCloseCaptionWrap::SetSink(ZNativeSDKCloseCaptionWrapSink *pSink) 
{
	m_pSink = pSink;
}

void ZMeetingCloseCaptionWrap::onLiveTranscriptionMsgInfoReceived(ZNLiveTranscriptionMessageInfo messageInfo) 
{
	if (m_pSink) {
		m_pSink->onLiveTranscriptionMsgInfoReceived(messageInfo);
	}
}
void ZMeetingCloseCaptionWrap::onOriginalLanguageMsgReceived(ZNLiveTranscriptionMessageInfo messageInfo) 
{
	if (m_pSink) {
		m_pSink->onOriginalLanguageMsgReceived(messageInfo);
	}
}