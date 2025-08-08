#include "meeting_raw_archiving_wrap_core.h"
#import "Header_include.h"
#include "sdk_native_error.h"
#include "meeting_service_wrap_core.h"

ZMeetingRawArchivingWrap& ZMeetingServiceWrap::GetMeetingRawArchivingCtrl()
{
    return m_meeting_raw_archiving_ctrl;
}

ZMeetingRawArchivingWrap::ZMeetingRawArchivingWrap()
{

}

ZMeetingRawArchivingWrap::~ZMeetingRawArchivingWrap()
{
 
}

void ZMeetingRawArchivingWrap::Init()
{

}
void ZMeetingRawArchivingWrap::Uninit()
{
    
}

ZNSDKError ZMeetingRawArchivingWrap::StartRawArchiving()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if(!service)
        return ZNSDKERR_SERVICE_FAILED;
    
    ZoomSDKRawArchivingController* archivingController = [service  getRawArchivingController];
    if(!archivingController)
        return ZNSDKERR_SERVICE_FAILED;
    
    ZoomSDKError ret = [archivingController startRawArchiving];
    nativeErrorTypeHelp  Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingRawArchivingWrap::StopRawArchiving()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if(!service)
        return ZNSDKERR_SERVICE_FAILED;
    
    ZoomSDKRawArchivingController* archivingController = [service  getRawArchivingController];
    if(!archivingController)
        return ZNSDKERR_SERVICE_FAILED;
    
    ZoomSDKError ret = [archivingController stopRawArchiving];
    nativeErrorTypeHelp  Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

