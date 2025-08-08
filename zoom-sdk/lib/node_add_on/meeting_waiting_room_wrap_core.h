#pragma once
#include "zoom_native_sdk_wrap_core_def.h"
#include "zoom_sinks_wrap_class.h"

class ZMeetingWaitingRoomWrap
{
public:
	ZMeetingWaitingRoomWrap();
	virtual ~ZMeetingWaitingRoomWrap();
	void Init();
	void Uninit();
	void SetSink(ZNativeSDKMeetingWaitingRoomWrapSink* pSink);

	bool IsSupportWaitingRoom();
	bool IsWaitingRoomOnEntryFlagOn();
	ZNSDKError EnableWaitingRoomOnEntry(bool bEnable);
	ZNList<unsigned int> GetWaitingRoomList();
	ZNUserInfomation GetWaitingRoomUserInfoByID(unsigned int userid);
	ZNSDKError AdmitToMeeting(unsigned int userid);
	ZNSDKError AdmitAllToMeeting();
	ZNSDKError PutInWaitingRoom(unsigned int userid);
	bool IsAudioEnabledInWaitingRoom();
	bool IsVideoEnabledInWaitingRoom();
	ZNSDKError RequestCustomWaitingRoomData();
	bool CanRenameUser();
	ZNSDKError RenameUser(unsigned userid, ZoomSTRING newName);
	bool CanExpelUser();
	ZNSDKError ExpelUser(unsigned int userid);

	ZNSDKError Retry();
	ZNSDKError Ignore();

	//callback
	void onWaitingRoomUserJoin(unsigned int userID);
	void onWaitingRoomUserLeft(unsigned int userID);
	void onWaitingRoomPresetAudioStatusChanged(bool bAudioCanTurnOn);
	void onWaitingRoomPresetVideoStatusChanged(bool bVideoCanTurnOn);
	void onCustomWaitingRoomDataUpdated(ZNCustomWaitingRoomData bData);
	void onWaitingRoomUserNameChanged(unsigned int userID, ZoomSTRING userName);
	void onWaitingRoomEntranceEnabled(bool bIsEnabled);
	
private:
	ZNativeSDKMeetingWaitingRoomWrapSink* m_pSink;
};