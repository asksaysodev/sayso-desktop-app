#pragma once
#include "zoom_native_sdk_wrap_core_def.h"
#include "zoom_sinks_wrap_class.h"

class ZMeetingParticipantsWrap
{
public:
	ZMeetingParticipantsWrap();
	virtual ~ZMeetingParticipantsWrap();
	void Init();
	void Uninit();
	void SetSink(ZNativeSDKMeetingParticipantsWrapSink* pSink);
	ZNList<unsigned int> GetParticipantsList();
	ZNUserInfomation GetBotAuthorizedUserInfoByUserID(unsigned int userid);
	ZNList<unsigned int> GetAuthorizedBotListByUserID(unsigned int userid);
	
	ZNUserInfomation GetUserInfomationByUserID(unsigned int userid);
	bool CanEnableParticipantRequestCloudRecording();
	bool IsParticipantRequestCloudRecordingAllowed();
	ZNSDKError AllowParticipantsToRequestCloudRecording(bool bAllow);

	//callback

	void onUserJoin(ZNList<unsigned int> lstUserID, ZoomSTRING strUserList);
	void onUserLeft(ZNList<unsigned int> lstUserID, ZoomSTRING strUserList);
	void onHostChangeNotification(unsigned int userId);
	void onAllowParticipantsRequestCloudRecording(bool bAllow);
	void onBotAuthorizerRelationChanged(unsigned int authorizeUserID);
private:
	ZNativeSDKMeetingParticipantsWrapSink* m_pSink;
};

