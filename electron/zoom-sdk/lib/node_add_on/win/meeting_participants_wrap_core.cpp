#include "meeting_participants_wrap_core.h"
#include "sdk_wrap.h"
#include "meeting_service_components_wrap/meeting_participants_ctrl_wrap.h"
#include "zoom_native_to_wrap.h"
#include "sdk_events_wrap_class.h"
extern ZOOM_SDK_NAMESPACE::IMeetingServiceWrap& g_meeting_service_wrap;

ZMeetingParticipantsWrap::ZMeetingParticipantsWrap()
{
	SDKEventWrapMgr::GetInst().m_meetingParticipantsCtrlEvent.SetOwner(this);
	m_pSink = 0;
}
ZMeetingParticipantsWrap::~ZMeetingParticipantsWrap()
{
	Uninit();
	m_pSink = 0;
	SDKEventWrapMgr::GetInst().m_meetingParticipantsCtrlEvent.SetOwner(NULL);
}
void ZMeetingParticipantsWrap::Init()
{
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingParticipantsController().Init_Wrap(&g_meeting_service_wrap);
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingParticipantsController().SetEvent(&SDKEventWrapMgr::GetInst().m_meetingParticipantsCtrlEvent);
}
void ZMeetingParticipantsWrap::Uninit()
{
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingParticipantsController().SetEvent(NULL);
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingParticipantsController().Uninit_Wrap();
}
void ZMeetingParticipantsWrap::SetSink(ZNativeSDKMeetingParticipantsWrapSink* pSink)
{
	m_pSink = pSink;
}

void ZMeetingParticipantsWrap::onUserJoin(ZNList<unsigned int> lstUserID, ZoomSTRING strUserList)
{
	if (m_pSink)
		m_pSink->onUserJoin(lstUserID, strUserList);
}
void ZMeetingParticipantsWrap::onUserLeft(ZNList<unsigned int> lstUserID, ZoomSTRING strUserList)
{
	if (m_pSink)
		m_pSink->onUserLeft(lstUserID, strUserList);
}
void ZMeetingParticipantsWrap::onHostChangeNotification(unsigned int userId)
{
	if (m_pSink)
		m_pSink->onHostChangeNotification(userId);
}
void ZMeetingParticipantsWrap::onAllowParticipantsRequestCloudRecording(bool bAllow)
{
	if (m_pSink)
		m_pSink->onAllowParticipantsRequestCloudRecording(bAllow);
}
void ZMeetingParticipantsWrap::onBotAuthorizerRelationChanged(unsigned int authorizeUserID)
{
	if (m_pSink)
		m_pSink->onBotAuthorizerRelationChanged(authorizeUserID);
}
ZNList<unsigned int> ZMeetingParticipantsWrap::GetParticipantsList()
{
	ZNList<unsigned int> userId_list;
	ZOOM_SDK_NAMESPACE::IList<unsigned int >* lstUserID = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingParticipantsController().GetParticipantsList();
	if (NULL == lstUserID)
		return userId_list;
	for (int i = 0; i < lstUserID->GetCount(); ++i)
	{

		unsigned int zn_userid;
		
		zn_userid = lstUserID->GetItem(i);

		userId_list.push_back(zn_userid);
	}
	return userId_list;

}
ZNUserInfomation ZMeetingParticipantsWrap::GetUserInfomationByUserID(unsigned int userid)
{
	ZOOM_SDK_NAMESPACE::IUserInfo* userInfo = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingParticipantsController().GetUserByUserID(userid);
	return ConvertSDKUserInfoToZnUserInfo(userInfo);
}
ZNUserInfomation ZMeetingParticipantsWrap::GetBotAuthorizedUserInfoByUserID(unsigned int userid)
{
	ZOOM_SDK_NAMESPACE::IUserInfo* userInfo = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingParticipantsController().GetBotAuthorizedUserInfoByUserID(userid);
	return ConvertSDKUserInfoToZnUserInfo(userInfo);
}
ZNList<unsigned int> ZMeetingParticipantsWrap::GetAuthorizedBotListByUserID(unsigned int userid)
{
	ZNList<unsigned int> zn_list;
	ZOOM_SDK_NAMESPACE::IList<unsigned int >* lstRobot = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingParticipantsController().GetAuthorizedBotListByUserID(userid);
	if (NULL == lstRobot)
	{
		return zn_list;
	}

	int count = lstRobot->GetCount();
	for (int i = 0; i < count; ++i)
	{
		unsigned int zn_bot = lstRobot->GetItem(i);
		zn_list.push_back(zn_bot);
	}
	return zn_list;
}
bool ZMeetingParticipantsWrap::CanEnableParticipantRequestCloudRecording()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingParticipantsController().CanEnableParticipantRequestCloudRecording();
}
bool ZMeetingParticipantsWrap::IsParticipantRequestCloudRecordingAllowed()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingParticipantsController().IsParticipantRequestCloudRecordingAllowed();
}
ZNSDKError ZMeetingParticipantsWrap::AllowParticipantsToRequestCloudRecording(bool bAllow)
{
	return Map2WrapDefine(ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingParticipantsController().AllowParticipantsToRequestCloudRecording(bAllow));
}