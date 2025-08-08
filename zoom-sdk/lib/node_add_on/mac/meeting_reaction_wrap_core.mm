#include "Header_include.h"
#include "sdk_native_error.h"
#include "meeting_reaction_wrap_core.h"
#include "meeting_service_wrap_core.h"
#import "meetingServiceDelegate.h"

ZMeetingReactionWrap &ZMeetingServiceWrap::GetMeetingReactionCtrl()
{
    return m_meeting_reaction_ctrl;
}
ZMeetingReactionWrap::ZMeetingReactionWrap()
{
   m_pSink = 0;
}

ZMeetingReactionWrap::~ZMeetingReactionWrap()
{
    m_pSink = 0;
    [[[[ZoomSDK sharedSDK] getMeetingService] getMeetingReactionController] setDelegate:nil];
}

void ZMeetingReactionWrap::SetSink(ZNativeSDKMeetingReactionWrapSink *pSink)
{
    [[[[ZoomSDK sharedSDK] getMeetingService] getReactionController] setDelegate:[[meetingServiceReactionDelegate alloc] init]];
    m_pSink= pSink;
}

void ZMeetingReactionWrap::Init()
{

}
void ZMeetingReactionWrap::Uninit()
{
    
}

void ZMeetingReactionWrap::onEmojiReactionReceived(unsigned int userid, ZNSDKEmojiReactionType type, ZNSDKEmojiReactionSkinTone skinTone)
{
    if (m_pSink)
    {
        m_pSink->onEmojiReactionReceived(userid, type, skinTone);
    }
}

void ZMeetingReactionWrap::onEmojiReactionReceivedInWebinar(ZNSDKEmojiReactionType type)
{
    if (m_pSink)
    {
        m_pSink->onEmojiReactionReceivedInWebinar(type);
    }
}