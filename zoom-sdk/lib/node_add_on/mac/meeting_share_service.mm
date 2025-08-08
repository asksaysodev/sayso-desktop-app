
#include "Header_include.h"
#include "meeting_share_wrap_core.h"
#include "sdk_native_error.h"
#include "meeting_service_wrap_core.h"
#include "meetingServiceDelegate.h"

ZMeetingShareWrap &ZMeetingServiceWrap::GetMeetingShareCtrl()
{
    return m_meeting_share_ctrl;
}

ZMeetingShareWrap::ZMeetingShareWrap()
{
    m_pSink = 0;
}

ZMeetingShareWrap::~ZMeetingShareWrap()
{
    m_pSink = 0;
    ZoomSDKASController *asController =[[[ZoomSDK sharedSDK] getMeetingService] getASController];
    if (asController) {
        asController.delegate = nil;
    }
}

void ZMeetingShareWrap::Init()
{
    
}

void ZMeetingShareWrap::Uninit()
{
    
}

void ZMeetingShareWrap::SetSink(ZNativeSDKMeetingShareWrapSink *pSink)
{
    m_pSink = pSink;
    ZoomSDKASController *asController =[[[ZoomSDK sharedSDK] getMeetingService] getASController];
    if (asController) {
        asController.delegate = [meetingASControllerDelegate share];
    }
}

ZNSDKError ZMeetingShareWrap::StartAppShare(ZoomSTRING hShareApp)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service){
        return ZNSDKERR_SERVICE_FAILED;
    }
    NSString *ID = [NSString stringWithCString:hShareApp.c_str() encoding:NSUTF8StringEncoding];
    CGWindowID windowID = ID.intValue;
    ZoomSDKASController *as = [service getASController];
    if (as) {
        ZoomSDKError error = [as startAppShare:windowID];
        nativeErrorTypeHelp  Help_type;
        return Help_type.ZoomSDKErrorType(error);
    }
    return ZNSDKERR_SERVICE_FAILED;
}

ZNSDKError ZMeetingShareWrap::StartMonitorShare(ZoomSTRING monitorID)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service){
        return ZNSDKERR_SERVICE_FAILED;
    }
    NSString *ID = [NSString stringWithCString:monitorID.c_str() encoding:NSUTF8StringEncoding];
    CGDirectDisplayID  displayID = ID.intValue;
    ZoomSDKASController *as = [service getASController];
    if (as) {
        ZoomSDKError error = [as startMonitorShare:displayID];
        nativeErrorTypeHelp  Help_type;
        return Help_type.ZoomSDKErrorType(error);
    }
    return ZNSDKERR_SERVICE_FAILED;
}

ZNSDKError ZMeetingShareWrap::StopShare()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service){
        return ZNSDKERR_SERVICE_FAILED;
    }
    ZoomSDKASController *as = [service getASController];
    if (as) {
        ZoomSDKError error = [as stopShare];
        nativeErrorTypeHelp  Help_type;
        return Help_type.ZoomSDKErrorType(error);
    }
    return ZNSDKERR_SERVICE_FAILED;
}

bool ZMeetingShareWrap::IsWhiteboardLegalNoticeAvailable()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return false;
    
    ZoomSDKASController *as = [service getASController];
    if (!as)
        return false;
    return [as isWhiteboardLegalNoticeAvailable];
}

ZoomSTRING ZMeetingShareWrap::GetWhiteboardLegalNoticesPrompt()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return "";
    
    ZoomSDKASController *as = [service getASController];
    if (!as)
        return "";
    NSString *whiteBoardStr = [as getWhiteboardLegalNoticesPrompt];
    
    return whiteBoardStr ? [whiteBoardStr UTF8String] : "";
}

ZoomSTRING ZMeetingShareWrap::GetWhiteboardLegalNoticesExplained()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return "";
    
    ZoomSDKASController *as = [service getASController];
    if (!as)
        return "";
    NSString *whiteboardLegalNotices = [as getWhiteboardLegalNoticesExplained];
    
    return whiteboardLegalNotices ? [whiteboardLegalNotices UTF8String] : "";
}

ZNSDKError ZMeetingShareWrap::SetAudioShareMode(ZNAudioShareMode mode)
{
    ZoomSDKMeetingService *serviceController = [[ZoomSDK sharedSDK] getMeetingService];
    if (!serviceController)
        return ZNSDKERR_SERVICE_FAILED;
    
    ZoomSDKASController *asController = [serviceController getASController];
    if (!asController)
        return ZNSDKERR_SERVICE_FAILED;
    
    nativeErrorTypeHelp  Help_type;
    ZoomSDKAudioShareMode shareMode = Help_type.ZoomSDKAudioShareModeMap(mode);
    ZoomSDKError error = [asController setAudioShareMode:shareMode];
    return Help_type.ZoomSDKErrorType(error);
}

ZNSDKError ZMeetingShareWrap::GetAudioShareMode(ZNAudioShareMode& mode)
{
    ZoomSDKMeetingService *serviceController = [[ZoomSDK sharedSDK] getMeetingService];
    if (!serviceController)
        return ZNSDKERR_SERVICE_FAILED;
    
    ZoomSDKASController *asController = [serviceController getASController];
    if (!asController)
        return ZNSDKERR_SERVICE_FAILED;
    
    ZoomSDKAudioShareMode shareMode = ZoomSDKAudioShareMode_Mono;
    ZoomSDKError error = [asController getAudioShareMode:&shareMode];
    nativeErrorTypeHelp  Help_type;
    mode = Help_type.ZNAudioShareModeMap(shareMode);
    return Help_type.ZoomSDKErrorType(error);
}

//callback
void ZMeetingShareWrap::onSharingStatus(ZNZoomSDKSharingSourceInfo shareInfo)
{
    if (m_pSink)
    {
        m_pSink->onSharingStatus(shareInfo);
    }
}

void ZMeetingShareWrap::onFailedToStartShare()
{
    if (m_pSink)
    {
        m_pSink->onFailedToStartShare();
    }
}

void ZMeetingShareWrap::onOptimizingShareForVideoClipStatusChanged(ZNZoomSDKSharingSourceInfo shareInfo)
{
    if (m_pSink)
    {
        m_pSink->onOptimizingShareForVideoClipStatusChanged(shareInfo);
    }
}