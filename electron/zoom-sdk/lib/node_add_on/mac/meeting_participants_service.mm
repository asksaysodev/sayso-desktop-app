

#include "meeting_participants_wrap_core.h"
#include "meeting_service_wrap_core.h"
#include "Header_include.h"
#include "sdk_native_error.h"
ZMeetingParticipantsWrap &ZMeetingServiceWrap::GetMeetingParticipantsCtrl()
{
    return m_meeting_participants_ctrl;
}

ZMeetingParticipantsWrap::ZMeetingParticipantsWrap()
{
    m_pSink = 0;
}

ZMeetingParticipantsWrap::~ZMeetingParticipantsWrap()
{
    m_pSink = 0;
}

void ZMeetingParticipantsWrap::Init()
{
    
}

void ZMeetingParticipantsWrap::Uninit()
{
    
}

void ZMeetingParticipantsWrap::SetSink(ZNativeSDKMeetingParticipantsWrapSink *pSink)
{
    m_pSink = pSink;
}

ZNUserInfomation ZMeetingParticipantsWrap::GetUserInfomationByUserID(unsigned int userid)
{
    ZNUserInfomation zn_userInfo;
    zn_userInfo.userInfoType = ZN_FAKE_USERINFO;
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) {
        return zn_userInfo;
    }
    ZoomSDKMeetingActionController *controller = [service getMeetingActionController];
    if (!controller) {
        return zn_userInfo;
    }
    ZoomSDKUserInfo *userInfo = [controller getUserByUserID:userid];
    nativeErrorTypeHelp Help_type;
    return Help_type.ConvertSDKUserInfoToZnUserInfo(userInfo);
}


ZNList<unsigned int> ZMeetingParticipantsWrap::GetParticipantsList()
{
    ZNList<unsigned int> list;
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) {
        return list;
    }
    ZoomSDKMeetingActionController  *action = [service getMeetingActionController];
    if (!action) {
        return list;
    }
    NSArray *listArr = [action getParticipantsList];
    for (NSNumber *userid in listArr) {
        unsigned int ID = userid.unsignedIntValue;
        list.push_back(ID);
    }
    return list;
}

ZNUserInfomation ZMeetingParticipantsWrap::GetBotAuthorizedUserInfoByUserID(unsigned int userId)
{
    ZNUserInfomation zn_userInfo;
    zn_userInfo.userInfoType = ZN_FAKE_USERINFO;
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) {
        return zn_userInfo;
    }
    ZoomSDKMeetingActionController *controller = [service getMeetingActionController];
    if (!controller) {
        return zn_userInfo;
    }
    ZoomSDKUserInfo *userInfo = [controller getBotAuthorizedUserInfoByUserID:userId];
    nativeErrorTypeHelp Help_type;
    return Help_type.ConvertSDKUserInfoToZnUserInfo(userInfo);
}

ZNList<unsigned int> ZMeetingParticipantsWrap::GetAuthorizedBotListByUserID(unsigned int userId)
{
    ZNList<unsigned int> list;
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) {
        return list;
    }
    ZoomSDKMeetingActionController  *action = [service getMeetingActionController];
    if (!action) {
        return list;
    }
    NSArray *listArr = [action getAuthorizedBotListByUserID:userId];
    for (NSNumber *botId in listArr) {
        unsigned int ID = botId.unsignedIntValue;
        list.push_back(ID);
    }
    return list;
}

bool ZMeetingParticipantsWrap::CanEnableParticipantRequestCloudRecording()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return false;
  
    ZoomSDKMeetingActionController *actionController = [service getMeetingActionController];
    if (!actionController)
        return false;
    
    return [actionController canEnableParticipantRequestCloudRecording];
}

bool ZMeetingParticipantsWrap::IsParticipantRequestCloudRecordingAllowed()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return false;
  
    ZoomSDKMeetingActionController *actionController = [service getMeetingActionController];
    if (!actionController)
        return false;
    
    return [actionController isParticipantRequestCloudRecordingAllowed];
}

ZNSDKError ZMeetingParticipantsWrap::AllowParticipantsToRequestCloudRecording(bool allow)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
    
    ZoomSDKMeetingActionController *actionController = [service getMeetingActionController];
    if (!actionController)
        return ZNSDKERR_SERVICE_FAILED;

    ZoomSDKError ret = [actionController allowParticipantsToRequestCloudRecording:allow];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

//callback
void ZMeetingParticipantsWrap::onHostChangeNotification(unsigned int userId)
{
    if (m_pSink)
    {
        m_pSink->onHostChangeNotification(userId);
    }
}

void ZMeetingParticipantsWrap::onUserJoin(std::vector<unsigned int> lstUserID, std::string strUserList)
{
    if (m_pSink)
    {
        m_pSink->onUserJoin(lstUserID,strUserList);
    }
}


void ZMeetingParticipantsWrap::onUserLeft(std::vector<unsigned int> lstUserID, std::string strUserList)
{
    if (m_pSink)
    {
        m_pSink->onUserLeft(lstUserID,strUserList);
    }
}

void ZMeetingParticipantsWrap::onAllowParticipantsRequestCloudRecording(bool allow)
{
    if (m_pSink) {
        m_pSink->onAllowParticipantsRequestCloudRecording(allow);
    }
}

void ZMeetingParticipantsWrap::onBotAuthorizerRelationChanged(unsigned int authorizeUserID)
{
    if (m_pSink)
    {
        m_pSink->onBotAuthorizerRelationChanged(authorizeUserID);
    }
}