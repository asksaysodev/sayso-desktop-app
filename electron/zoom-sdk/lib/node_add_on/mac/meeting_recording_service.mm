
#include "meeting_recording_wrap_core.h"
#import "Header_include.h"
#import "sdk_native_error.h"
#import "meetingServiceDelegate.h"
#include "meeting_service_wrap_core.h"

ZMeetingRecordingWrap &ZMeetingServiceWrap::GetMeetingRecordingCtrl()
{
    return m_meeting_recording_ctrl;
}

ZMeetingRecordingWrap::ZMeetingRecordingWrap()
{
    m_pSink = nil;
}

ZMeetingRecordingWrap::~ZMeetingRecordingWrap()
{
    [[[[ZoomSDK sharedSDK] getMeetingService] getRecordController] setDelegate:nil];
    m_pSink = nil;
}

void ZMeetingRecordingWrap::Init()
{
    
}

void ZMeetingRecordingWrap::Uninit()
{
    
}

void ZMeetingRecordingWrap::SetSink(ZNativeSDKMeetingRecordingWrapSink *pSink)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (service) {
        ZoomSDKMeetingRecordController* recordingController = [service  getRecordController];
        if (recordingController) {
            [recordingController setDelegate:[meetingServiceRecordDelegate share]];
        }
    }
    m_pSink = pSink;
}

ZNSDKError ZMeetingRecordingWrap::StartRecording(time_t &startTimestamp)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (service){
        ZoomSDKMeetingRecordController* recordingController = [service  getRecordController];
        if (recordingController) {
            ZoomSDKError ret = [recordingController startRecording:&startTimestamp];
            nativeErrorTypeHelp  Help_type;
            return Help_type.ZoomSDKErrorType(ret);
        }
    }
    return ZNSDKERR_SERVICE_FAILED;
}

ZNSDKError ZMeetingRecordingWrap::StopRecording(time_t &stopTimestamp)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (service) {
        ZoomSDKMeetingRecordController* recordingController = [service  getRecordController];
        if (recordingController) {
            ZoomSDKError ret = [recordingController stopRecording:&stopTimestamp];
            nativeErrorTypeHelp help_type;
            return help_type.ZoomSDKErrorType(ret);
        }
    }
    return ZNSDKERR_SERVICE_FAILED;
}

ZNSDKError ZMeetingRecordingWrap::CanStartRecording(bool cloud_recording, unsigned int userid)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (service) {
        ZoomSDKMeetingRecordController* recordingController = [service  getRecordController];
        if (recordingController) {
            ZoomSDKError ret = [recordingController canStartRecording:cloud_recording];
            nativeErrorTypeHelp help_type;
            return help_type.ZoomSDKErrorType(ret);
        }
    }
    return ZNSDKERR_SERVICE_FAILED;
}

ZNSDKError ZMeetingRecordingWrap::CanAllowDisAllowLocalRecording()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (service) {
        ZoomSDKMeetingRecordController* recordingController = [service  getRecordController];
        if (recordingController) {
            ZoomSDKError ret = [recordingController canAllowDisallowRecording];
            nativeErrorTypeHelp help_type;
            return help_type.ZoomSDKErrorType(ret);
        }
    }
    return ZNSDKERR_SERVICE_FAILED;
}


ZNSDKError ZMeetingRecordingWrap::StartCloudRecording()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (service) {
        ZoomSDKMeetingRecordController* recordingController = [service  getRecordController];
        if (recordingController) {
            ZoomSDKError ret = [recordingController startCloudRecording:YES];
            nativeErrorTypeHelp help_type;
            return help_type.ZoomSDKErrorType(ret);
        }
    }
    return ZNSDKERR_SERVICE_FAILED;
}

ZNSDKError ZMeetingRecordingWrap::StopCloudRecording()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (service) {
        ZoomSDKMeetingRecordController* recordingController = [service  getRecordController];
        if (recordingController) {
            ZoomSDKError ret = [recordingController startCloudRecording:NO];
            nativeErrorTypeHelp help_type;
            return help_type.ZoomSDKErrorType(ret);
        }
    }
    return ZNSDKERR_SERVICE_FAILED;
}


ZNSDKError ZMeetingRecordingWrap::IsSupportLocalRecording(unsigned int userid)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (service) {
        ZoomSDKMeetingRecordController* recordingController = [service  getRecordController];
        if (recordingController) {
            ZoomSDKError ret = [recordingController isSupportLocalRecording:userid];
            nativeErrorTypeHelp help_type;
            return help_type.ZoomSDKErrorType(ret);
        }
    }
    return ZNSDKERR_SERVICE_FAILED;
}

ZNSDKError ZMeetingRecordingWrap::DisAllowLocalRecording(unsigned int userid)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (service) {
        ZoomSDKMeetingRecordController* recordingController = [service  getRecordController];
        if (recordingController) {
            ZoomSDKError ret = [recordingController allowLocalRecording:NO User:userid];
            nativeErrorTypeHelp help_type;
            return help_type.ZoomSDKErrorType(ret);
        }
    }
    return ZNSDKERR_SERVICE_FAILED;
}

ZNSDKError ZMeetingRecordingWrap::AllowLocalRecording(unsigned int userid)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (service) {
        ZoomSDKMeetingRecordController* recordingController = [service  getRecordController];
        if (recordingController) {
            ZoomSDKError ret = [recordingController allowLocalRecording:YES User:userid];
            nativeErrorTypeHelp help_type;
            return help_type.ZoomSDKErrorType(ret);
        }
    }
    return ZNSDKERR_SERVICE_FAILED;
}

ZNSDKError ZMeetingRecordingWrap::RequestStartCloudRecording()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
    
    ZoomSDKMeetingRecordController* recordingController = [service  getRecordController];
    if (!recordingController)
        return ZNSDKERR_SERVICE_FAILED;
    
    ZoomSDKError ret = [recordingController requestStartCloudRecording];
    nativeErrorTypeHelp help_type;
    return help_type.ZoomSDKErrorType(ret);
}

bool ZMeetingRecordingWrap::IsSmartRecordingEnabled()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return false;
  
    ZoomSDKMeetingRecordController* recordingController = [service  getRecordController];
    if (!recordingController)
        return false;
    
    return [recordingController isSmartRecordingEnabled];
}

bool ZMeetingRecordingWrap::CanEnableSmartRecordingFeature()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return false;
  
    ZoomSDKMeetingRecordController* recordingController = [service  getRecordController];
    if (!recordingController)
        return false;
    
    return [recordingController canEnableSmartRecordingFeature];
}

ZNSDKError ZMeetingRecordingWrap::EnableSmartRecording()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;

    ZoomSDKMeetingRecordController* recordingController = [service  getRecordController];
    if (!recordingController)
        return ZNSDKERR_SERVICE_FAILED;
    
    ZoomSDKError ret = [recordingController enableSmartRecording];
    nativeErrorTypeHelp help_type;
    return help_type.ZoomSDKErrorType(ret);
}

//callback

void ZMeetingRecordingWrap::onRecording2MP4Done(bool bsuccess, int iResult, ZoomSTRING szPath)
{
    if (m_pSink) {
        m_pSink->onRecording2MP4Done(bsuccess, iResult, szPath);
    }
}

void ZMeetingRecordingWrap::onRecording2MP4Processing(int iPercentage)
{
    if (m_pSink) {
        m_pSink->onRecording2MP4Processing(iPercentage);
    }
    
}

void ZMeetingRecordingWrap::onRecordingStatus(ZNRecordingStatus status)
{
    if (m_pSink) {
        m_pSink->onRecordingStatus(status);
    }
}


void ZMeetingRecordingWrap::onRecordPrivilegeChanged(bool bCanRec)
{
    if (m_pSink) {
        m_pSink->onRecordPrivilegeChanged(bCanRec);
    }
}

void ZMeetingRecordingWrap::onCloudRecordingStatus(ZNRecordingStatus status)
{
    if (m_pSink) {
        m_pSink->onCloudRecordingStatus(status);
    }
}

void ZMeetingRecordingWrap::onCloudRecordingStorageFull(time_t gracePeriodDate)
{
    if (m_pSink) {
        m_pSink->onCloudRecordingStorageFull(gracePeriodDate);
    }
}

void ZMeetingRecordingWrap::onRequestCloudRecordingResponse(ZNRequestStartCloudRecordingStatus status)
{
    if (m_pSink) {
        m_pSink->onRequestCloudRecordingResponse(status);
    }
}

unsigned int ZMeetingRecordingWrap::GetRequesterId()
{
    unsigned int requester_id;
    ZoomSDKRequestStartCloudRecordingHandler *handler = [meetingServiceRecordDelegate share].cloudRecordingHandler;
    if (!handler)
        return requester_id;
        
    return [handler requesterId];
}

ZoomSTRING ZMeetingRecordingWrap::GetRequesterName()
{
    ZoomSDKRequestStartCloudRecordingHandler *handler = [meetingServiceRecordDelegate share].cloudRecordingHandler;
    if (!handler)
        return "";
        
    NSString *requester_name = [handler requesterName];
    if (!requester_name)
        return "";
    return [requester_name UTF8String];
}

ZNSDKError ZMeetingRecordingWrap::Start()
{
    ZoomSDKRequestStartCloudRecordingHandler *handler = [meetingServiceRecordDelegate share].cloudRecordingHandler;
    if (handler) {
        ZoomSDKError ret = [handler start];
        nativeErrorTypeHelp help_type;
        return help_type.ZoomSDKErrorType(ret);
    }
    return ZNSDKERR_WRONG_USEAGE;
}

ZNSDKError ZMeetingRecordingWrap::Deny(bool denyAll)
{
    ZoomSDKRequestStartCloudRecordingHandler *handler = [meetingServiceRecordDelegate share].cloudRecordingHandler;
    if (handler) {
        ZoomSDKError ret = [handler deny:denyAll];
        nativeErrorTypeHelp help_type;
        return help_type.ZoomSDKErrorType(ret);
    }
    return ZNSDKERR_WRONG_USEAGE;
}

void ZMeetingRecordingWrap::onStartCloudRecordingRequested()
{
    if (m_pSink) {
        m_pSink->onStartCloudRecordingRequested();
    }
}

unsigned int ZMeetingRecordingWrap::GetRequestEnableAndStartSmartRecordingUserId()
{
    unsigned int request_user_id = 0;
    ZoomSDKRequestEnableAndStartSmartRecordingHandler *handler = [meetingServiceRecordDelegate share].enableAndStartSmartRecordingHandler;
    if (!handler)
        return request_user_id;
        
    return [handler requestUserID];
}

ZoomSTRING ZMeetingRecordingWrap::GetRequestEnableAndStartSmartRecordingTipString()
{
    ZoomSDKRequestEnableAndStartSmartRecordingHandler *handler = [meetingServiceRecordDelegate share].enableAndStartSmartRecordingHandler;
    if (!handler)
        return "";
        
    NSString *tip_string = [handler tipString];
    if (!tip_string)
        return "";
    return [tip_string UTF8String];
}

ZNSDKError ZMeetingRecordingWrap::StartCloudRecordingWithoutEnableSmartRecording()
{
    ZoomSDKRequestEnableAndStartSmartRecordingHandler *handler = [meetingServiceRecordDelegate share].enableAndStartSmartRecordingHandler;
    if (handler) {
        ZoomSDKError ret = [handler startCloudRecordingWithoutEnableSmartRecording];
        nativeErrorTypeHelp help_type;
        return help_type.ZoomSDKErrorType(ret);
    }
    return ZNSDKERR_WRONG_USEAGE;
}

ZNSDKError ZMeetingRecordingWrap::AgreeToEnableAndStartSmartRecording(bool allMeetings)
{
    ZoomSDKRequestEnableAndStartSmartRecordingHandler *handler = [meetingServiceRecordDelegate share].enableAndStartSmartRecordingHandler;
    if (handler) {
        ZoomSDKError ret = [handler agreeToEnableAndStart:allMeetings];
        nativeErrorTypeHelp help_type;
        return help_type.ZoomSDKErrorType(ret);
    }
    return ZNSDKERR_WRONG_USEAGE;
}

ZNSDKError ZMeetingRecordingWrap::DeclineEnableAndStartSmartRecording(bool denyAll)
{
    ZoomSDKRequestEnableAndStartSmartRecordingHandler *handler = [meetingServiceRecordDelegate share].enableAndStartSmartRecordingHandler;
    if (handler) {
        ZoomSDKError ret = [handler deny:denyAll];
        nativeErrorTypeHelp help_type;
        return help_type.ZoomSDKErrorType(ret);
    }
    return ZNSDKERR_WRONG_USEAGE;
}

void ZMeetingRecordingWrap::onEnableAndStartSmartRecordingRequested()
{
    if (m_pSink) {
        m_pSink->onEnableAndStartSmartRecordingRequested();
    }
}

ZoomSTRING ZMeetingRecordingWrap::GetSmartRecordingEnableActionTipString()
{
    ZoomSDKSmartRecordingEnableActionHandler *handler = [meetingServiceRecordDelegate share].smartRecordingEnableActionHandler;
    if (!handler)
        return "";
        
    NSString *tip_string = [handler tipString];
    if (!tip_string)
        return "";
    return [tip_string UTF8String];
}

ZNSDKError ZMeetingRecordingWrap::ActionConfirmEnableSmartRecording(bool allMeetings)
{
    ZoomSDKSmartRecordingEnableActionHandler *handler = [meetingServiceRecordDelegate share].smartRecordingEnableActionHandler;
    if (handler) {
        ZoomSDKError ret = [handler actionConfirm:allMeetings];
        nativeErrorTypeHelp help_type;
        return help_type.ZoomSDKErrorType(ret);
    }
    return ZNSDKERR_WRONG_USEAGE;
}

ZNSDKError ZMeetingRecordingWrap::ActionCancelEnableSmartRecording()
{
    ZoomSDKSmartRecordingEnableActionHandler *handler = [meetingServiceRecordDelegate share].smartRecordingEnableActionHandler;
    if (handler) {
        ZoomSDKError ret = [handler actionCancel];
        nativeErrorTypeHelp help_type;
        return help_type.ZoomSDKErrorType(ret);
    }
    return ZNSDKERR_WRONG_USEAGE;
}

void ZMeetingRecordingWrap::onSmartRecordingEnableActionCallback()
{
    if (m_pSink) {
        m_pSink->onSmartRecordingEnableActionCallback();
    }
}