#include "Header_include.h"
#include "sdk_native_error.h"
#include "meeting_close_caption_wrap_core.h"
#include "meeting_service_wrap_core.h"
#import "meetingServiceDelegate.h"

ZMeetingCloseCaptionWrap &ZMeetingServiceWrap::GetMeetingCloseCaptionCtrl()
{
    return m_close_caption_ctrl;
}
ZMeetingCloseCaptionWrap::ZMeetingCloseCaptionWrap()
{
    m_pSink = 0;
}

ZMeetingCloseCaptionWrap::~ZMeetingCloseCaptionWrap()
{
    m_pSink = 0;
    [[[[ZoomSDK sharedSDK] getMeetingService] getMeetingCloseCaptionController] setDelegate:nil];
}

void ZMeetingCloseCaptionWrap::SetSink(ZNativeSDKCloseCaptionWrapSink *pSink)
{
    [[[[ZoomSDK sharedSDK] getMeetingService] getCloseCaptionController] setDelegate:[[meetingServiceCloseCaptionDelegate alloc] init]];
    m_pSink= pSink;
}

void ZMeetingCloseCaptionWrap::Init()
{

}
void ZMeetingCloseCaptionWrap::Uninit()
{
    
}

void ZMeetingCloseCaptionWrap::onLiveTranscriptionMsgInfoReceived(ZNLiveTranscriptionMessageInfo messageInfo)
{
    if (m_pSink)
    {
        m_pSink->onLiveTranscriptionMsgInfoReceived(messageInfo);
    }
}

void ZMeetingCloseCaptionWrap::onOriginalLanguageMsgReceived(ZNLiveTranscriptionMessageInfo messageInfo)
{
    if (m_pSink)
    {
        m_pSink->onOriginalLanguageMsgReceived(messageInfo);
    }
}