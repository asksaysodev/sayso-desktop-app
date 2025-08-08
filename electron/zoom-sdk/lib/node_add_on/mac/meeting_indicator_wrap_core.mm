#include "Header_include.h"
#include "sdk_native_error.h"
#include "meeting_indicator_wrap_core.h"
#include "meeting_service_wrap_core.h"
#import "meetingServiceDelegate.h"
ZMeetingIndicatorWrap &ZMeetingServiceWrap::GetMeetingIndicatorCtrl()
{
    return m_meeting_indicator_ctrl;
}
ZMeetingIndicatorWrap::ZMeetingIndicatorWrap()
{
   m_pSink = 0;
}

ZMeetingIndicatorWrap::~ZMeetingIndicatorWrap()
{
    m_pSink = 0;
    [[[[ZoomSDK sharedSDK] getMeetingService] getMeetingIndicatorController] setDelegate:nil];
}

void ZMeetingIndicatorWrap::Init()
{

}
void ZMeetingIndicatorWrap::Uninit()
{
    
}

void ZMeetingIndicatorWrap::SetSink(ZNativeSDKMeetingIndicatorWrapSink *pSink)
{
    [[[[ZoomSDK sharedSDK] getMeetingService] getMeetingIndicatorController] setDelegate:[meetingServiceIndicatorControllerDelegate share]];
    m_pSink= pSink;
}

//callback
void ZMeetingIndicatorWrap::onIndicatorItemReceived()
{
    if (m_pSink) {
        m_pSink->onIndicatorItemReceived();
    }
}

void ZMeetingIndicatorWrap::onIndicatorItemRemoved()
{
    if (m_pSink) {
        m_pSink->onIndicatorItemRemoved();
    }
}

ZoomSTRING ZMeetingIndicatorWrap::GetIndicatorItemId()
{
    ZoomSDKMeetingIndicatorHandle *handler = [meetingServiceIndicatorControllerDelegate share].indicatorHandle;
    if (!handler)
        return "";
        
    NSString *indicator_item_id = handler.indicatorItemId;
    if (!indicator_item_id)
        return "";
    return [indicator_item_id UTF8String];
}

ZoomSTRING ZMeetingIndicatorWrap::GetIndicatorName()
{
    ZoomSDKMeetingIndicatorHandle *handler = [meetingServiceIndicatorControllerDelegate share].indicatorHandle;
    if (!handler)
        return "";
        
    NSString *indicator_name = handler.indicatorName;
    if (!indicator_name)
        return "";
    return [indicator_name UTF8String];
}

ZoomSTRING ZMeetingIndicatorWrap::GetIndicatorIcon()
{
    ZoomSDKMeetingIndicatorHandle *handler = [meetingServiceIndicatorControllerDelegate share].indicatorHandle;
    if (!handler)
        return "";
        
    NSString *indicator_icon = handler.indicatorIcon;
    if (!indicator_icon)
        return "";
    return [indicator_icon UTF8String];
}

ZNSDKError ZMeetingIndicatorWrap::ShowIndicatorPanel(unsigned int x, unsigned int y, uint64_t windowId)
{
    ZoomSDKMeetingIndicatorHandle *handler = [meetingServiceIndicatorControllerDelegate share].indicatorHandle;
    if (!handler)
        return ZNSDKERR_WRONG_USEAGE;

    NSPoint point = NSMakePoint(x, y);

    NSWindow *parentWindow = [NSApp windowWithWindowNumber:windowId];
    if(!parentWindow)
        return ZNSDKERR_WRONG_USEAGE;
    ZoomSDKError ret = [handler showIndicatorPanel:point parentWindow:parentWindow];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingIndicatorWrap::HideIndicatorPanel()
{
    ZoomSDKMeetingIndicatorHandle *handler = [meetingServiceIndicatorControllerDelegate share].indicatorHandle;
    if (!handler)
        return ZNSDKERR_WRONG_USEAGE;

    ZoomSDKError ret = [handler hideIndicatorPanel];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}