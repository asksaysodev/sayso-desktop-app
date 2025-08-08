#include "Header_include.h"
#include "sdk_native_error.h"
#include "meeting_webinar_wrap_core.h"
#include "meeting_service_wrap_core.h"
#import "meetingServiceDelegate.h"
ZMeetingWebinarWrap &ZMeetingServiceWrap::GetMeetingWebinarCtrl()
{
    return m_meeting_webinar_ctrl;
}
ZMeetingWebinarWrap::ZMeetingWebinarWrap()
{
   m_pSink = 0;
}

ZMeetingWebinarWrap::~ZMeetingWebinarWrap()
{
    m_pSink = 0;
    [[[[ZoomSDK sharedSDK] getMeetingService] getWebinarController] setDelegate:nil];
}

void ZMeetingWebinarWrap::Init()
{

}
void ZMeetingWebinarWrap::Uninit()
{
    
}

void ZMeetingWebinarWrap::SetSink(ZNativeSDKMeetingWebinarWrapSink *pSink)
{
    [[[[ZoomSDK sharedSDK] getMeetingService] getWebinarController] setDelegate:[WebinarControllerDelegate share]];
    m_pSink= pSink;
}

ZNSDKError ZMeetingWebinarWrap::PromptAttendee2Panelist(unsigned int userid)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
  
    ZoomSDKWebinarController *webinarController = [service getWebinarController];
    if (!webinarController)
        return ZNSDKERR_SERVICE_FAILED;

    ZoomSDKError ret = [webinarController PromoteAttendee2Panelist:userid];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingWebinarWrap::DepromptPanelist2Attendee(unsigned int userid)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
  
    ZoomSDKWebinarController *webinarController = [service getWebinarController];
    if (!webinarController)
        return ZNSDKERR_SERVICE_FAILED;

    ZoomSDKError ret = [webinarController DepromotePanelist2Attendee:userid];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

bool ZMeetingWebinarWrap::IsSupportAttendeeTalk()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return false;
  
    ZoomSDKWebinarController *webinarController = [service getWebinarController];
    if (!webinarController)
        return false;
    
    ZoomSDKWebinarMeetingStatus* webinarMeetingStatus = [webinarController getZoomSDKWebinarMeetingStatus];
    if (webinarMeetingStatus)
    {
        return [webinarMeetingStatus isSupportAttendeeTalk];
    } 
    return false;
}

ZNSDKError ZMeetingWebinarWrap::AllowAttendeeTalk(unsigned int userid)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
  
    ZoomSDKWebinarController *webinarController = [service getWebinarController];
    if (!webinarController)
        return ZNSDKERR_SERVICE_FAILED;

    ZoomSDKError ret = [webinarController AllowAttendeeTalk:userid];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingWebinarWrap::DisallowAttendeeTalk(unsigned int userid)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
  
    ZoomSDKWebinarController *webinarController = [service getWebinarController];
    if (!webinarController)
        return ZNSDKERR_SERVICE_FAILED;

    ZoomSDKError ret = [webinarController DisallowAttendeeTalk:userid];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingWebinarWrap::AllowPanelistStartVideo()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
  
    ZoomSDKWebinarController *webinarController = [service getWebinarController];
    if (!webinarController)
        return ZNSDKERR_SERVICE_FAILED;

    ZoomSDKError ret = [webinarController AllowPanelistStartVideo];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingWebinarWrap::DisallowPanelistStartVideo()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
  
    ZoomSDKWebinarController *webinarController = [service getWebinarController];
    if (!webinarController)
        return ZNSDKERR_SERVICE_FAILED;

    ZoomSDKError ret = [webinarController DisallowPanelistStartVideo];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingWebinarWrap::AllowWebinarEmojiReaction()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
  
    ZoomSDKWebinarController *webinarController = [service getWebinarController];
    if (!webinarController)
        return ZNSDKERR_SERVICE_FAILED;

    ZoomSDKError ret = [webinarController allowWebinarEmojiReaction];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingWebinarWrap::DisallowWebinarEmojiReaction()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
  
    ZoomSDKWebinarController *webinarController = [service getWebinarController];
    if (!webinarController)
        return ZNSDKERR_SERVICE_FAILED;

    ZoomSDKError ret = [webinarController disallowWebinarEmojiReaction];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

bool ZMeetingWebinarWrap::IsWebinarEmojiReactionSupported()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return false;
  
    ZoomSDKWebinarController *webinarController = [service getWebinarController];
    if (!webinarController)
        return false;
    
    ZoomSDKWebinarMeetingStatus* webinarMeetingStatus = [webinarController getZoomSDKWebinarMeetingStatus];
    if (webinarMeetingStatus)
    {
        return [webinarMeetingStatus isWebinarEmojiReactionSupported];
    } 
    return false;
}

ZNSDKError ZMeetingWebinarWrap::AllowAttendeeRaiseHand()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
  
    ZoomSDKWebinarController *webinarController = [service getWebinarController];
    if (!webinarController)
        return ZNSDKERR_SERVICE_FAILED;

    ZoomSDKError ret = [webinarController allowAttendeeRaiseHand];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingWebinarWrap::DisallowAttendeeRaiseHand()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
  
    ZoomSDKWebinarController *webinarController = [service getWebinarController];
    if (!webinarController)
        return ZNSDKERR_SERVICE_FAILED;

    ZoomSDKError ret = [webinarController disallowAttendeeRaiseHand];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingWebinarWrap::AllowAttendeeViewTheParticipantCount()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
  
    ZoomSDKWebinarController *webinarController = [service getWebinarController];
    if (!webinarController)
        return ZNSDKERR_SERVICE_FAILED;

    ZoomSDKError ret = [webinarController allowAttendeeViewTheParticipantCount];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingWebinarWrap::DisallowAttendeeViewTheParticipantCount()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
  
    ZoomSDKWebinarController *webinarController = [service getWebinarController];
    if (!webinarController)
        return ZNSDKERR_SERVICE_FAILED;

    ZoomSDKError ret = [webinarController disallowAttendeeViewTheParticipantCount];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

int ZMeetingWebinarWrap::GetParticipantCount()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
  
    ZoomSDKWebinarController *webinarController = [service getWebinarController];
    if (!webinarController)
        return ZNSDKERR_SERVICE_FAILED;

    return [webinarController getParticipantCount];
}
 
ZNWebinarMeetingStatus ZMeetingWebinarWrap::GetWebinarMeetingStatus()
{
    ZNWebinarMeetingStatus zn_webinarMeetingStatus;
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return zn_webinarMeetingStatus;
  
    ZoomSDKWebinarController *webinarController = [service getWebinarController];
    if (!webinarController)
        return zn_webinarMeetingStatus;

    ZoomSDKWebinarMeetingStatus* webinarMeetingStatus = [webinarController getZoomSDKWebinarMeetingStatus];
    if (webinarMeetingStatus)
    {
        zn_webinarMeetingStatus.allow_panellist_start_video = [webinarMeetingStatus isAllowPanellistStartVideo];
        zn_webinarMeetingStatus.allow_attendee_chat = [webinarMeetingStatus isAllowAttendeeChat];
        zn_webinarMeetingStatus.allow_emoji_reaction = [webinarMeetingStatus isWebinarEmojiReactionAllowed];
        zn_webinarMeetingStatus.allow_attendee_raise_hand = [webinarMeetingStatus isAllowRaiseHand];
        zn_webinarMeetingStatus.allow_attendee_view_participant_count = [webinarMeetingStatus canAttendeesViewParticipantCount];
    } 
    return zn_webinarMeetingStatus;
}

ZNSDKError ZMeetingWebinarWrap::SetPanelistChatPrivilege(ZNSDKPanelistChatPrivilege zn_privilege)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
  
    ZoomSDKWebinarController *webinarController = [service getWebinarController];
    if (!webinarController)
        return ZNSDKERR_SERVICE_FAILED;

    nativeErrorTypeHelp Help_type;
    ZoomSDKPanelistChatPrivilege privilege = Help_type.ZoomSDKPanelistChatPrivilegeMap(zn_privilege);
    ZoomSDKError ret = [webinarController setPanelistChatPrivilege:privilege];
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingWebinarWrap::GetPanelistChatPrivilege(ZNSDKPanelistChatPrivilege& zn_privilege)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
  
    ZoomSDKWebinarController *webinarController = [service getWebinarController];
    if (!webinarController)
        return ZNSDKERR_SERVICE_FAILED;

    nativeErrorTypeHelp Help_type;
    ZoomSDKPanelistChatPrivilege privilege = ZoomSDKPanelistChatPrivilege_PanelistOnly;
    ZoomSDKError ret = [webinarController getPanelistChatPrivilege:&privilege];
    zn_privilege = Help_type.ZNSDKPanelistChatPrivilegeMap(privilege);
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingWebinarWrap::SetAttendeeViewMode(ZNSDKAttendeeViewMode zn_mode)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
    
    ZoomSDKWebinarController *webinarController = [service getWebinarController];
    if (!webinarController)
        return ZNSDKERR_SERVICE_FAILED;

    nativeErrorTypeHelp Help_type;
    ZoomSDKAttendeeViewMode mode = Help_type.ZoomSDKAttendeeViewModeMap(zn_mode);
    ZoomSDKError ret = [webinarController setAttendeeViewMode:mode];
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKAttendeeViewMode ZMeetingWebinarWrap::GetAttendeeViewMode()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKAttendeeViewMode_None;
    
    ZoomSDKWebinarController *webinarController = [service getWebinarController];
    if (!webinarController)
        return ZNSDKAttendeeViewMode_None;

    ZoomSDKAttendeeViewMode mode = [webinarController getAttendeeViewMode];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZNSDKAttendeeViewModeMap(mode);
}

ZoomSTRING ZMeetingWebinarWrap::GetWebinarLegalNoticesPrompt()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return "";
    
    ZoomSDKWebinarController *webinarController = [service getWebinarController];
    if (!webinarController)
        return "";

    NSString *noticesPrompt = [webinarController getWebinarRegistrationLegalNoticesPrompt];
    if (!noticesPrompt)
        return "";
    return [noticesPrompt UTF8String];
}

bool ZMeetingWebinarWrap::GetWebinarLegalNoticesExplained(ZNWebinarLegalNoticesExplainedInfo& zn_explained_info)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return false;
    
    ZoomSDKWebinarController *webinarController = [service getWebinarController];
    if (!webinarController)
        return false;

    ZoomSDKWebinarRegistrationExplainInfo *explainInfo = [webinarController getWebinarRegistrationLegalNoticesExplained];
    if (!explainInfo)
    {
        return false;
    }   

    if (explainInfo.content)
    {
        zn_explained_info.explained_content = [explainInfo.content UTF8String];
    }
    if (explainInfo.accountOwnerLink)
    {
        zn_explained_info.url_register_account_owner = [explainInfo.accountOwnerLink UTF8String];
    }
    if (explainInfo.termLink)
    {
        zn_explained_info.url_register_terms = [explainInfo.termLink UTF8String];
    }
    if (explainInfo.policyLink)
    {
        zn_explained_info.url_register_privacy_policy = [explainInfo.policyLink UTF8String];
    }  
    return true;
}

//callback
void ZMeetingWebinarWrap::onPromptAttendee2PanelistResult(int result)
{
    if (m_pSink) {
        m_pSink->onPromptAttendee2PanelistResult(result);
    }
}

void ZMeetingWebinarWrap::onDepromptPanelist2AttendeeResult(int result)
{
    if (m_pSink) {
        m_pSink->onDepromptPanelist2AttendeeResult(result);
    }
}

void ZMeetingWebinarWrap::onAllowPanelistStartVideoNotification()
{
    if (m_pSink) {
        m_pSink->onAllowPanelistStartVideoNotification();
    }
}

void ZMeetingWebinarWrap::onDisallowPanelistStartVideoNotification()
{
    if (m_pSink) {
        m_pSink->onDisallowPanelistStartVideoNotification();
    }
}

void ZMeetingWebinarWrap::onSelfAllowTalkNotification()
{
    if (m_pSink) {
        m_pSink->onSelfAllowTalkNotification();
    }
}

void ZMeetingWebinarWrap::onSelfDisallowTalkNotification()
{
    if (m_pSink) {
        m_pSink->onSelfDisallowTalkNotification();
    }
}

void ZMeetingWebinarWrap::onAllowAttendeeChatNotification()
{
    if (m_pSink) {
        m_pSink->onAllowAttendeeChatNotification();
    }
}

void ZMeetingWebinarWrap::onDisallowAttendeeChatNotification()
{
    if (m_pSink) {
        m_pSink->onDisallowAttendeeChatNotification();
    }
}

void ZMeetingWebinarWrap::onAllowWebinarReactionStatusChanged(bool can_reaction)
{
    if (m_pSink) {
        m_pSink->onAllowWebinarReactionStatusChanged(can_reaction);
    }
}

void ZMeetingWebinarWrap::onAllowAttendeeRaiseHandStatusChanged(bool can_raiseHand)
{
    if (m_pSink) {
        m_pSink->onAllowAttendeeRaiseHandStatusChanged(can_raiseHand);
    }
}

void ZMeetingWebinarWrap::onAllowAttendeeViewTheParticipantCountStatusChanged(bool can_viewParticipantCount)
{
    if (m_pSink) {
        m_pSink->onAllowAttendeeViewTheParticipantCountStatusChanged(can_viewParticipantCount);
    }
}

void ZMeetingWebinarWrap::onAttendeeAudioStatusNotification(unsigned int userid, bool can_talk, bool is_muted)
{
    if (m_pSink) {
        m_pSink->onAttendeeAudioStatusNotification(userid, can_talk, is_muted);
    }
}

void ZMeetingWebinarWrap::onAttendeePromoteConfirmResult(bool agree, unsigned int userid)
{
    if (m_pSink) {
        m_pSink->onAttendeePromoteConfirmResult(agree, userid);
    }
}