#include "Header_include.h"
#include "sdk_native_error.h"
#include "meeting_whiteboard_wrap_core.h"
#include "meeting_service_wrap_core.h"
#import "meetingServiceDelegate.h"
ZMeetingWhiteboardWrap &ZMeetingServiceWrap::GetMeetingWhiteboardCtrl()
{
    return m_meeting_whiteboard_ctrl;
}
ZMeetingWhiteboardWrap::ZMeetingWhiteboardWrap()
{
   m_pSink = 0;
}

ZMeetingWhiteboardWrap::~ZMeetingWhiteboardWrap()
{
    m_pSink = 0;
    [[[[ZoomSDK sharedSDK] getMeetingService] getMeetingWhiteboardController] setDelegate:nil];
}

void ZMeetingWhiteboardWrap::Init()
{

}
void ZMeetingWhiteboardWrap::Uninit()
{
    
}

void ZMeetingWhiteboardWrap::SetSink(ZNativeSDKMeetingWhiteboardWrapSink *pSink)
{
    [[[[ZoomSDK sharedSDK] getMeetingService] getMeetingWhiteboardController] setDelegate:[[meetingServiceWhiteboardControllerDelegate alloc] init]];
    m_pSink= pSink;
}

ZNSDKError ZMeetingWhiteboardWrap::SetOwnerWindow(ZoomSTRING zn_windowId)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) {
        return ZNSDKERR_SERVICE_FAILED;
    }

    ZoomSDKMeetingWhiteboardController *whiteboardController = [service getMeetingWhiteboardController];
    if (!whiteboardController) {
        return ZNSDKERR_SERVICE_FAILED;
    }

    NSWindow *window = nil;
    if (!zn_windowId.empty()) {
        NSInteger windowId = std::stoi(zn_windowId);
        window = [NSApp windowWithWindowNumber:windowId];
    }
    ZoomSDKError ret = [whiteboardController setParentWindow:window];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

bool ZMeetingWhiteboardWrap::IsSupportWhiteBoard()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return false;
  
    ZoomSDKMeetingWhiteboardController *whiteboardController = [service getMeetingWhiteboardController];
    if (!whiteboardController)
        return false;
    
    return [whiteboardController isSupportWhiteBoard];
}

bool ZMeetingWhiteboardWrap::CanStartShareWhiteboard(ZNCannotShareReasonType& zn_reason)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return false;
  
    ZoomSDKMeetingWhiteboardController *whiteboardController = [service getMeetingWhiteboardController];
    if (!whiteboardController)
        return false;

    ZoomSDKCannotShareReasonType reason = ZoomSDKCannotShareReasonType_None;
    bool bCan = [whiteboardController canStartShareWhiteboard:&reason];
    nativeErrorTypeHelp Help_type;
    zn_reason = Help_type.ZNCannotShareReasonTypeMap(reason);
    return bCan;
}

ZNSDKError ZMeetingWhiteboardWrap::ShowDashboardView(int x, int y)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) {
        return ZNSDKERR_SERVICE_FAILED;
    }

    ZoomSDKMeetingWhiteboardController *whiteboardController = [service getMeetingWhiteboardController];
    if (!whiteboardController) {
        return ZNSDKERR_SERVICE_FAILED;
    }

    NSPoint point = NSMakePoint(x, y);
    ZoomSDKError ret = [whiteboardController showDashboardView:point];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingWhiteboardWrap::SetDashboardViewSize(int width, int height)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) {
        return ZNSDKERR_SERVICE_FAILED;
    }

    ZoomSDKMeetingWhiteboardController *whiteboardController = [service getMeetingWhiteboardController];
    if (!whiteboardController) {
        return ZNSDKERR_SERVICE_FAILED;
    }

    NSSize size = NSMakeSize(width, height);
    ZoomSDKError ret = [whiteboardController setDashboardViewSize:size];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingWhiteboardWrap::SetWhiteboardViewPos(int x, int y)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) {
        return ZNSDKERR_SERVICE_FAILED;
    }

    ZoomSDKMeetingWhiteboardController *whiteboardController = [service getMeetingWhiteboardController];
    if (!whiteboardController) {
        return ZNSDKERR_SERVICE_FAILED;
    }

    NSPoint point = NSMakePoint(x, y);
    ZoomSDKError ret = [whiteboardController setWhiteboardViewPos:point];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingWhiteboardWrap::SetWhiteboardViewSize(int width, int height)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) {
        return ZNSDKERR_SERVICE_FAILED;
    }

    ZoomSDKMeetingWhiteboardController *whiteboardController = [service getMeetingWhiteboardController];
    if (!whiteboardController) {
        return ZNSDKERR_SERVICE_FAILED;
    }

    NSSize size = NSMakeSize(width, height);
    ZoomSDKError ret = [whiteboardController setWhiteboardViewSize:size];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingWhiteboardWrap::SetWhiteboardShareOption(ZNSDKWhiteboardShareOption zn_option)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) {
        return ZNSDKERR_SERVICE_FAILED;
    }

    ZoomSDKMeetingWhiteboardController *whiteboardController = [service getMeetingWhiteboardController];
    if (!whiteboardController) {
        return ZNSDKERR_SERVICE_FAILED;
    }

    nativeErrorTypeHelp Help_type;
    ZoomSDKWhiteboardShareOption option = Help_type.ZoomSDKWhiteboardShareOptionMap(zn_option);
    ZoomSDKError ret = [whiteboardController setWhiteboardShareOption:option];
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingWhiteboardWrap::GetWhiteboardShareOption(ZNSDKWhiteboardShareOption& zn_option)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) {
        return ZNSDKERR_SERVICE_FAILED;
    }

    ZoomSDKMeetingWhiteboardController *whiteboardController = [service getMeetingWhiteboardController];
    if (!whiteboardController) {
        return ZNSDKERR_SERVICE_FAILED;
    }
    
    ZoomSDKWhiteboardShareOption option = [whiteboardController getWhiteboardShareOption];
    nativeErrorTypeHelp Help_type;
    zn_option = Help_type.ZNSDKWhiteboardShareOptionMap(option);
    return ZNSDKERR_SUCCESS;
}

ZNSDKError ZMeetingWhiteboardWrap::SetWhiteboardCreateOption(ZNSDKWhiteboardCreateOption zn_option)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) {
        return ZNSDKERR_SERVICE_FAILED;
    }

    ZoomSDKMeetingWhiteboardController *whiteboardController = [service getMeetingWhiteboardController];
    if (!whiteboardController) {
        return ZNSDKERR_SERVICE_FAILED;
    }

    nativeErrorTypeHelp Help_type;
    ZoomSDKWhiteboardCreateOption option = Help_type.ZoomSDKWhiteboardCreateOptionMap(zn_option);
    ZoomSDKError ret = [whiteboardController setWhiteboardCreateOption:option];
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingWhiteboardWrap::GetWhiteboardCreateOption(ZNSDKWhiteboardCreateOption& zn_option)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) {
        return ZNSDKERR_SERVICE_FAILED;
    }

    ZoomSDKMeetingWhiteboardController *whiteboardController = [service getMeetingWhiteboardController];
    if (!whiteboardController) {
        return ZNSDKERR_SERVICE_FAILED;
    }
    
    ZoomSDKWhiteboardCreateOption option = [whiteboardController getWhiteboardCreateOption];
    nativeErrorTypeHelp Help_type;
    zn_option = Help_type.ZNSDKWhiteboardCreateOptionMap(option);
    return ZNSDKERR_SUCCESS;
}

ZNSDKError ZMeetingWhiteboardWrap::EnableParticipantsCreateWithoutHost(bool bEnable)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
  
    ZoomSDKMeetingWhiteboardController *whiteboardController = [service getMeetingWhiteboardController];
    if (!whiteboardController)
        return ZNSDKERR_SERVICE_FAILED;

    ZoomSDKError ret = [whiteboardController enableParticipantsCreateWithoutHost:bEnable];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

bool ZMeetingWhiteboardWrap::IsParticipantsCreateWithoutHostEnabled()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return false;
  
    ZoomSDKMeetingWhiteboardController *whiteboardController = [service getMeetingWhiteboardController];
    if (!whiteboardController)
        return false;
    
    return [whiteboardController isParticipantsCreateWithoutHostEnabled];
}

bool ZMeetingWhiteboardWrap::IsOtherSharingWhiteboard()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return false;
  
    ZoomSDKMeetingWhiteboardController *whiteboardController = [service getMeetingWhiteboardController];
    if (!whiteboardController)
        return false;
    
    return [whiteboardController isOtherSharingWhiteboard];
}

bool ZMeetingWhiteboardWrap::IsSharingWhiteboardOut()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return false;
  
    ZoomSDKMeetingWhiteboardController *whiteboardController = [service getMeetingWhiteboardController];
    if (!whiteboardController)
        return false;
    
    return [whiteboardController isSharingWhiteboardOut];
}

//callback
void ZMeetingWhiteboardWrap::onWhiteboardStatusChanged(ZNSDKWhiteboardStatus status)
{
    if (m_pSink) {
        m_pSink->onWhiteboardStatusChanged(status);
    }
}

void ZMeetingWhiteboardWrap::onWhiteboardSettingsChanged(ZNSDKWhiteboardShareOption shareOption, ZNSDKWhiteboardCreateOption createOption, bool bEnable)
{
    if (m_pSink) {
        m_pSink->onWhiteboardSettingsChanged(shareOption, createOption, bEnable);
    }
}