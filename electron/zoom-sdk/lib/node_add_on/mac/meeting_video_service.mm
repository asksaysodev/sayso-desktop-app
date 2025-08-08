

#include "meeting_video_wrap_core.h"
#include "meeting_service_wrap_core.h"
#include "Header_include.h"
#include "sdk_native_error.h"
#import "meetingServiceDelegate.h"
ZMeetingVideoWrap  &ZMeetingServiceWrap::GetMeetingVideoCtrl()
{
    return m_meeting_video_ctrl;
}

ZMeetingVideoWrap::ZMeetingVideoWrap()
{
    m_pSink = 0;
    
}

ZMeetingVideoWrap::~ZMeetingVideoWrap()
{
    m_pSink = 0;
}

void ZMeetingVideoWrap::Init()
{

}

void ZMeetingVideoWrap::Uninit()
{
    
}

void ZMeetingVideoWrap::SetSink(ZNativeSDKMeetingVideoWrapSink *pSink)
{
    m_pSink = pSink;
}

ZNSDKError ZMeetingVideoWrap::MuteVideo(unsigned int userId)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service){
        return ZNSDKERR_SERVICE_FAILED;
    }
    ZoomSDKMeetingActionController *action = [service getMeetingActionController];
    if (action) {
        ZoomSDKError ret = [action actionMeetingWithCmd:ActionMeetingCmd_MuteVideo userID:userId onScreen:ScreenType_First];
        nativeErrorTypeHelp  Help_type;
        return Help_type.ZoomSDKErrorType(ret);
    }
    return ZNSDKERR_SERVICE_FAILED;
}

ZNSDKError ZMeetingVideoWrap::UnMuteVideo(unsigned int userId)
{
    
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service){
        return ZNSDKERR_SERVICE_FAILED;
    }
    ZoomSDKMeetingActionController *action = [service getMeetingActionController];
    if (action) {
        ZoomSDKError ret = [action actionMeetingWithCmd:ActionMeetingCmd_UnMuteVideo userID:userId onScreen:ScreenType_First];
        nativeErrorTypeHelp  Help_type;
        return Help_type.ZoomSDKErrorType(ret);
    }
    return ZNSDKERR_SERVICE_FAILED;
}

ZNSDKError ZMeetingVideoWrap::PinVideo(bool bPin, bool bFirstView, unsigned int userId)
{
//    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
//    if (!service){
//        return ZNSDKERR_SERVICE_FAILED;
//    }
//    ZoomSDKMeetingActionController *action = [service getMeetingActionController];
//    if (action) {
//        ZoomSDKError ret(ZoomSDKError_UnKnown);
//        if (bFirstView) {
//            ret = [action actionMeetingWithCmd:ActionMeetingCmd_PinVideo userID:userId onScreen:ScreenType_First];
//        }else{
//            ret = [action actionMeetingWithCmd:ActionMeetingCmd_PinVideo userID:userId onScreen:ScreenType_Second];
//        }
//        nativeErrorTypeHelp  Help_type;
//        return Help_type.ZoomSDKErrorType(ret);
//    }
    return ZNSDKERR_NO_IMPL;
}

ZNSDKError ZMeetingVideoWrap::SpotlightVideo(bool bSpotlight, unsigned int userId)
{
//    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
//    if (!service){
//        return ZNSDKERR_SERVICE_FAILED;
//    }
//    ZoomSDKMeetingActionController *action = [service getMeetingActionController];
//    if (action) {
//        ZoomSDKError ret = [action actionMeetingWithCmd:ActionMeetingCmd_SpotlightVideo userID:userId onScreen:ScreenType_First];
//        nativeErrorTypeHelp  Help_type;
//        return Help_type.ZoomSDKErrorType(ret);
//    }
    return ZNSDKERR_NO_IMPL;
}

ZNSDKError ZMeetingVideoWrap::HideOrShowNoVideoUserOnVideoWall(bool bHide)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service){
        return ZNSDKERR_SERVICE_FAILED;
    }
    ZoomSDKMeetingUIController *action = [service getMeetingUIController];
    if (action) {
        ZoomSDKError ret = [action hideOrShowNoVideoUserOnVideoWall:bHide];
        nativeErrorTypeHelp Help_type;
        return Help_type.ZoomSDKErrorType(ret);
    }
    return ZNSDKERR_SERVICE_FAILED;
}

ZNSDKError ZMeetingVideoWrap::SetVideoQualityPreference(ZNSDKVideoPreferenceMode zn_mode, uint32_t minimumFrameRate, uint32_t maximumFrameRate)
{
    ZoomSDKSettingService *service = [[ZoomSDK sharedSDK] getSettingService];
    if (!service) {
        return ZNSDKERR_SERVICE_FAILED;
    }

    ZoomSDKVideoSetting *video = [service getVideoSetting];
    if (!video) {
        return ZNSDKERR_SERVICE_FAILED;
    }

    ZoomSDKVideoPreferenceSetting *preferenceSetting = [[ZoomSDKVideoPreferenceSetting alloc] init];
    nativeErrorTypeHelp Help_type;
    preferenceSetting.mode = Help_type.SDKVideoPreferenceModeMap(zn_mode); 
    preferenceSetting.minimumFrameRate = minimumFrameRate;
    preferenceSetting.maximumFrameRate = maximumFrameRate;
    ZoomSDKError ret = [video setVideoQualityPreference:preferenceSetting];
    return Help_type.ZoomSDKErrorType(ret);
}

//callback
void ZMeetingVideoWrap::onUserVideoStatusChange(unsigned int userId, ZNVideoStatus status)
{
    if (m_pSink) {
        m_pSink->onUserVideoStatusChange(userId, status);
    }
}

void ZMeetingVideoWrap::onActiveVideoUserChanged(unsigned int userid)
{
    if (m_pSink) {
        m_pSink->onActiveVideoUserChanged(userid);
    }
}

void ZMeetingVideoWrap::onActiveSpeakerVideoUserChanged(unsigned int userid)
{
    if (m_pSink) {
        m_pSink->onActiveSpeakerVideoUserChanged(userid);
    }
}
