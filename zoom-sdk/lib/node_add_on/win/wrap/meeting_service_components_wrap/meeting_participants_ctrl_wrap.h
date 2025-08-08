#pragma once
#include "common_include.h"
BEGIN_ZOOM_SDK_NAMESPACE
class IMeetingServiceWrap;
IMeetingParticipantsController* InitIMeetingParticipantsControllerFunc(IMeetingParticipantsCtrlEvent* pEvent, IMeetingServiceWrap* pOwner);
void UninitIMeetingParticipantsControllerFunc(IMeetingParticipantsController* obj);
BEGIN_CLASS_DEFINE_WITHCALLBACK(IMeetingParticipantsController, IMeetingParticipantsCtrlEvent)
NORMAL_CLASS(IMeetingParticipantsController)
INIT_UNINIT_WITHEVENT_AND_OWNSERVICE(IMeetingParticipantsController, IMeetingServiceWrap)
virtual SDKError SetEvent(IMeetingParticipantsCtrlEvent* pEvent)
{
	external_cb = pEvent;
	return SDKERR_SUCCESS;
}
//virtual IList<unsigned int >* GetParticipantsList() = 0;
DEFINE_FUNC_0(GetParticipantsList, IList<unsigned int >*)
//virtual IUserInfo* GetMySelfUser() = 0;
DEFINE_FUNC_0(GetMySelfUser, IUserInfo*)
//virtual IUserInfo* GetUserByUserID(unsigned int userid) = 0;
DEFINE_FUNC_1(GetUserByUserID, IUserInfo*, unsigned int, userid)
//virtual IUserInfo* GetAuthorizeUserByRobotUserID(unsigned int robotUserId) = 0;
DEFINE_FUNC_1(GetBotAuthorizedUserInfoByUserID, IUserInfo*, unsigned int, userid)
//virtual IList<unsigned int >* GetRobotListByAuthorizeUserID(unsigned int userid) = 0;
DEFINE_FUNC_1(GetAuthorizedBotListByUserID, IList<unsigned int >*, unsigned int, userid)
//virtual SDKError LowerAllHands(bool isWebinarAttendee) = 0;
DEFINE_FUNC_1(LowerAllHands, SDKError, bool, isWebinarAttendee)
//virtual SDKError ChangeUserName(const unsigned int userid, const zchar_t* userName, bool bSaveUserName) = 0;
DEFINE_FUNC_3(ChangeUserName, SDKError, const unsigned int, userid, const zchar_t*, userName, bool, bSaveUserName)

//virtual SDKError LowerHand(unsigned int userid) = 0;
DEFINE_FUNC_1(LowerHand, SDKError, unsigned int, userid)
//virtual SDKError RaiseHand() = 0;
DEFINE_FUNC_0(RaiseHand, SDKError)
//virtual SDKError MakeHost(unsigned int userid) = 0;
DEFINE_FUNC_1(MakeHost, SDKError, unsigned int, userid)
//virtual SDKError ExpelUser(unsigned int userid) = 0;
DEFINE_FUNC_1(ExpelUser, SDKError, unsigned int, userid)
//virtual SDKError ReclaimHost() = 0;
DEFINE_FUNC_0(ReclaimHost, SDKError)
//virtual SDKError CanReclaimHost(bool& bCanReclaimHost) = 0;
DEFINE_FUNC_1(CanReclaimHost, SDKError, bool&, bCanReclaimHost)
//virtual SDKError ReclaimHostByHostKey(const zchar_t* host_key) = 0;
DEFINE_FUNC_1(ReclaimHostByHostKey, SDKError, const zchar_t*, host_key)

//virtual SDKError CanbeCohost(unsigned int userid) = 0;
DEFINE_FUNC_1(CanbeCohost, SDKError, unsigned int, userid)
//virtual SDKError AssignCoHost(unsigned int userid) = 0;
DEFINE_FUNC_1(AssignCoHost, SDKError, unsigned int, userid)
//virtual SDKError RevokeCoHost(unsigned int userid) = 0;
DEFINE_FUNC_1(RevokeCoHost, SDKError, unsigned int, userid)
//virtual bool CanEnableParticipantRequestCloudRecording() = 0;
DEFINE_FUNC_0(CanEnableParticipantRequestCloudRecording, bool)
//virtual bool IsParticipantRequestCloudRecordingAllowed() = 0;
DEFINE_FUNC_0(IsParticipantRequestCloudRecordingAllowed, bool)
//virtual SDKError AllowParticipantsToRequestCloudRecording(bool bAllow) = 0;
DEFINE_FUNC_1(AllowParticipantsToRequestCloudRecording, SDKError, bool, bAllow)

//virtual void onUserJoin(IList<unsigned int >* lstUserID, const zchar_t* strUserList = NULL) = 0;
CallBack_FUNC_2(onUserJoin, IList<unsigned int >*, lstUserID, const zchar_t*, strUserList)
//virtual void onUserLeft(IList<unsigned int >* lstUserID, const zchar_t* strUserList = NULL) = 0;
CallBack_FUNC_2(onUserLeft, IList<unsigned int >*, lstUserID, const zchar_t*, strUserList)
//virtual void onHostChangeNotification(unsigned int userId) = 0;
CallBack_FUNC_1(onHostChangeNotification, unsigned int, userId)
//virtual void onLowOrRaiseHandStatusChanged(bool bLow, unsigned int userid) = 0;
CallBack_FUNC_2(onLowOrRaiseHandStatusChanged, bool, bLow, unsigned int, userid)
//virtual void onCoHostChangeNotification(unsigned int userId, bool isCoHost) = 0;
CallBack_FUNC_2(onCoHostChangeNotification, unsigned int, userId, bool, isCoHost)
//virtual void onInvalidReclaimHostkey() = 0;
CallBack_FUNC_0(onInvalidReclaimHostkey)
//virtual void onLowerAllHands() = 0;
CallBack_FUNC_0(onAllHandsLowered)
//virtual void onUserNamesChanged(IList<unsigned int>* lstUserID) = 0;
CallBack_FUNC_1(onUserNamesChanged, IList<unsigned int>*, lstUserID)
//virtual void onLocalRecordingStatusChanged(unsigned int user_id, RecordingStatus status) = 0;
CallBack_FUNC_2(onLocalRecordingStatusChanged, unsigned int, userId, RecordingStatus, status)
//virtual void onAllowParticipantsRenameNotification(bool bAllow) = 0;
CallBack_FUNC_1(onAllowParticipantsRenameNotification, bool, bAllow)
//virtual void onAllowParticipantsUnmuteSelfNotification(bool bAllow) = 0;
CallBack_FUNC_1(onAllowParticipantsUnmuteSelfNotification, bool, bAllow)
//virtual void onAllowParticipantsStartVideoNotification(bool bAllow) = 0;
CallBack_FUNC_1(onAllowParticipantsStartVideoNotification, bool, bAllow)
//virtual void onAllowParticipantsShareWhiteBoardNotification(bool bAllow) = 0;
CallBack_FUNC_1(onAllowParticipantsShareWhiteBoardNotification, bool, bAllow)
//virtual void onRequestLocalRecordingPrivilegeChanged(LocalRecordingRequestPrivilegeStatus status) = 0;
CallBack_FUNC_1(onRequestLocalRecordingPrivilegeChanged, LocalRecordingRequestPrivilegeStatus, status)
//virtual void onAllowParticipantsRequestCloudRecording(bool bAllow) = 0;
CallBack_FUNC_1(onAllowParticipantsRequestCloudRecording, bool, bAllow)
//virtual void onInMeetingUserAvatarPathUpdated(unsigned int userID) = 0;
CallBack_FUNC_1(onInMeetingUserAvatarPathUpdated, unsigned int, userID)
//virtual void onParticipantProfilePictureStatusChange(bool bHidden) = 0;
CallBack_FUNC_1(onParticipantProfilePictureStatusChange, bool, bHidden)
//virtual void onFocusModeStateChanged(bool bEnabled) = 0;
CallBack_FUNC_1(onFocusModeStateChanged, bool, bEnabled)
//virtual void onFocusModeShareTypeChanged(FocusModeShareType type) = 0;
CallBack_FUNC_1(onFocusModeShareTypeChanged, FocusModeShareType, type)
//virtual void onBotAuthorizerRelationChanged(unsigned int authorizeUserID) = 0;
CallBack_FUNC_1(onBotAuthorizerRelationChanged, unsigned int, authorizeUserID)
//virtual void onVirtualNameTagStatusChanged(bool bOn, unsigned int userID) = 0;
CallBack_FUNC_2(onVirtualNameTagStatusChanged, bool, isCoHost, unsigned int, userID)
//virtual void onVirtualNameTagRosterInfoUpdated(unsigned int userID) = 0;
CallBack_FUNC_1(onVirtualNameTagRosterInfoUpdated, unsigned int, userID)
#if defined(WIN32)
//virtual void onCreateCompanionRelation(unsigned int parentUserID, unsigned int childUserID) = 0;
CallBack_FUNC_2(onCreateCompanionRelation, unsigned int, parentUserID, unsigned int, childUserID)
//virtual void onRemoveCompanionRelation(unsigned int childUserID) = 0;
CallBack_FUNC_1(onRemoveCompanionRelation, unsigned int, childUserID)
#endif
//virtual void onGrantCoOwnerPrivilegeChanged(bool canGrantOther) = 0;
CallBack_FUNC_1(onGrantCoOwnerPrivilegeChanged, bool, canGrantOther)
END_CLASS_DEFINE(IMeetingParticipantsController)
END_ZOOM_SDK_NAMESPACE