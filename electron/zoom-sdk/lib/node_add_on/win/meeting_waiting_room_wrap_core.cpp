#include "meeting_waiting_room_wrap_core.h"
#include "sdk_wrap.h"
#include "meeting_service_components_wrap/meeting_waiting_room_wrap.h"
#include "zoom_native_to_wrap.h"
#include "sdk_events_wrap_class.h"

extern ZOOM_SDK_NAMESPACE::IMeetingServiceWrap& g_meeting_service_wrap;

ZMeetingWaitingRoomWrap::ZMeetingWaitingRoomWrap()
{
	SDKEventWrapMgr::GetInst().m_meetingWaitingRoomCtrlEvent.SetOwner(this);
	m_pSink = NULL;
}
ZMeetingWaitingRoomWrap::~ZMeetingWaitingRoomWrap()
{
	Uninit();
	m_pSink = NULL;
	SDKEventWrapMgr::GetInst().m_meetingWaitingRoomCtrlEvent.SetOwner(NULL);
}
void ZMeetingWaitingRoomWrap::Init()
{
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWaitingRoomController().Init_Wrap(&g_meeting_service_wrap);
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWaitingRoomController().SetEvent(&SDKEventWrapMgr::GetInst().m_meetingWaitingRoomCtrlEvent);
}
void ZMeetingWaitingRoomWrap::Uninit()
{
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWaitingRoomController().SetEvent(NULL);
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWaitingRoomController().Uninit_Wrap();
}
void ZMeetingWaitingRoomWrap::SetSink(ZNativeSDKMeetingWaitingRoomWrapSink* pSink)
{
	m_pSink = pSink;
}
bool ZMeetingWaitingRoomWrap::IsSupportWaitingRoom()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWaitingRoomController().IsSupportWaitingRoom();
}
bool ZMeetingWaitingRoomWrap::IsWaitingRoomOnEntryFlagOn()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWaitingRoomController().IsWaitingRoomOnEntryFlagOn();
}
ZNSDKError ZMeetingWaitingRoomWrap::EnableWaitingRoomOnEntry(bool bEnable)
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN;
	sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWaitingRoomController().EnableWaitingRoomOnEntry(bEnable);
	return Map2WrapDefine(sdkerr);
}
ZNList<unsigned int> ZMeetingWaitingRoomWrap::GetWaitingRoomList()
{
	ZNList<unsigned int> waiting_room_list;
	ZOOM_SDK_NAMESPACE::IList<unsigned int>* lstWaitingRoom = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWaitingRoomController().GetWaitingRoomLst();
	if (NULL == lstWaitingRoom)
		return waiting_room_list;
	for (int i = 0; i < lstWaitingRoom->GetCount(); ++i)
	{
		unsigned int zn_waiting_room = lstWaitingRoom->GetItem(i);
		waiting_room_list.push_back(zn_waiting_room);
	}
	return waiting_room_list;
}
ZNUserInfomation ZMeetingWaitingRoomWrap::GetWaitingRoomUserInfoByID(unsigned int userid)
{
	ZOOM_SDK_NAMESPACE::IUserInfo* userInfo = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWaitingRoomController().GetWaitingRoomUserInfoByID(userid);
	return ConvertSDKUserInfoToZnUserInfo(userInfo);
}
ZNSDKError ZMeetingWaitingRoomWrap::AdmitToMeeting(unsigned int userid)
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN;
	sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWaitingRoomController().AdmitToMeeting(userid);
	return Map2WrapDefine(sdkerr);
}
ZNSDKError ZMeetingWaitingRoomWrap::AdmitAllToMeeting()
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN;
	sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWaitingRoomController().AdmitAllToMeeting();
	return Map2WrapDefine(sdkerr);
}
ZNSDKError ZMeetingWaitingRoomWrap::PutInWaitingRoom(unsigned int userid)
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN;
	sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWaitingRoomController().PutInWaitingRoom(userid);
	return Map2WrapDefine(sdkerr);
}
bool ZMeetingWaitingRoomWrap::IsAudioEnabledInWaitingRoom()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWaitingRoomController().IsAudioEnabledInWaitingRoom();
}
bool ZMeetingWaitingRoomWrap::IsVideoEnabledInWaitingRoom()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWaitingRoomController().IsVideoEnabledInWaitingRoom();
}
ZNSDKError ZMeetingWaitingRoomWrap::RequestCustomWaitingRoomData()
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN;
	sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWaitingRoomController().RequestCustomWaitingRoomData();
	return Map2WrapDefine(sdkerr);
}
bool ZMeetingWaitingRoomWrap::CanRenameUser()
{
	bool bIsCan = false;
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN;
	sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWaitingRoomController().CanRenameUser(bIsCan);
	if (sdkerr == ZOOM_SDK_NAMESPACE::SDKERR_SUCCESS) {
		return bIsCan;
	} else {
		return false;
	}
}
ZNSDKError ZMeetingWaitingRoomWrap::RenameUser(unsigned userid, ZoomSTRING newName)
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN;
	sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWaitingRoomController().RenameUser(userid, newName.c_str());
	return Map2WrapDefine(sdkerr);
}
bool ZMeetingWaitingRoomWrap::CanExpelUser()
{
	bool bIsCan = false;
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN;
	sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWaitingRoomController().CanExpelUser(bIsCan);
	if (sdkerr == ZOOM_SDK_NAMESPACE::SDKERR_SUCCESS) {
		return bIsCan;
	} else {
		return false;
	}
}
ZNSDKError ZMeetingWaitingRoomWrap::ExpelUser(unsigned int userid)
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN;
	sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingWaitingRoomController().ExpelUser(userid);
	return Map2WrapDefine(sdkerr);
}
void ZMeetingWaitingRoomWrap::onWaitingRoomUserJoin(unsigned int userID)
{
	if (m_pSink)
	{
		m_pSink->onWaitingRoomUserJoin(userID);
	}
}
void ZMeetingWaitingRoomWrap::onWaitingRoomUserLeft(unsigned int userID)
{
	if (m_pSink)
	{
		m_pSink->onWaitingRoomUserLeft(userID);
	}
}
void ZMeetingWaitingRoomWrap::onWaitingRoomPresetAudioStatusChanged(bool bAudioCanTurnOn)
{
	if (m_pSink)
	{
		m_pSink->onWaitingRoomPresetAudioStatusChanged(bAudioCanTurnOn);
	}
}
void ZMeetingWaitingRoomWrap::onWaitingRoomPresetVideoStatusChanged(bool bVideoCanTurnOn)
{
	if (m_pSink)
	{
		m_pSink->onWaitingRoomPresetVideoStatusChanged(bVideoCanTurnOn);
	}
}
void ZMeetingWaitingRoomWrap::onCustomWaitingRoomDataUpdated(ZNCustomWaitingRoomData bData)
{
	if (m_pSink)
	{
		m_pSink->onCustomWaitingRoomDataUpdated(bData);
	}
}
void ZMeetingWaitingRoomWrap::onWaitingRoomUserNameChanged(unsigned int userID, ZoomSTRING userName)
{
	if (m_pSink)
	{
		m_pSink->onWaitingRoomUserNameChanged(userID, userName.c_str());
	}
}
void ZMeetingWaitingRoomWrap::onWaitingRoomEntranceEnabled(bool bIsEnabled)
{
	if (m_pSink)
	{
		m_pSink->onWaitingRoomEntranceEnabled(bIsEnabled);
	}
}
ZNSDKError ZMeetingWaitingRoomWrap::Retry()
{
	return ZMeetingWaitingRoomHandler::GetInst().Retry();
}
ZNSDKError ZMeetingWaitingRoomWrap::Ignore()
{
	return ZMeetingWaitingRoomHandler::GetInst().Ignore();
}