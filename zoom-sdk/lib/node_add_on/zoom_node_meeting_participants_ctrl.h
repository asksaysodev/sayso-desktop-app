#ifndef _ZOOM_NODE_MEETING_PARTICIPANTS_CTRL_H_
#define _ZOOM_NODE_MEETING_PARTICIPANTS_CTRL_H_
#include "zoom_node_common_include.h"
#include "zoom_native_sdk_wrap_core.h"
#include "zoom_singleton_wrap_class.h"

extern ZNativeSDKWrap _g_native_wrap;

class ZOOM_NODE_HIDE ZoomNodeMeetingParticipantsCtrlWrap :
	public ZoomWrapObject<ZoomNodeMeetingParticipantsCtrlWrap >
{
	friend class ZoomWrapObject<ZoomNodeMeetingParticipantsCtrlWrap >;
private:
	ZoomNodeMeetingParticipantsCtrlWrap();
	~ZoomNodeMeetingParticipantsCtrlWrap();
public:
	/// \brief Get the list of all the panelists in the meeting.
	/// \return If the function succeeds, the return value is the list of the panelists in the meeting.
	///Otherwise failed, the return value is an empty array.
	static void GetParticipantsList(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Get the information of specified user.
	/// \param userid(number) Specify the user ID for which you want to get the information. 
	///Zero(0) indicates to get the information of the current user.
	/// \return If the function succeeds, the return value is an object which includes the user's infomation.
	///For more details, see \link ZNUserInfomation \endlink. Otherwise failed, the return value is an empty object.
	static void GetUserInfoByUserID(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Get the information about the bot's authorized user.
	/// \param userid Specify the user ID for which to get the information. 
	/// \return If the function succeeds, the return value is a pointer to the IUserInfo. For more details, see \link IUserInfo \endlink.
	///Otherwise the function fails, and the return value is NULL.
	/// \remarks Valid for both ZOOM style and user custom interface mode.
	static void GetBotAuthorizedUserInfoByUserID(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Get the authorizer's bot list.
	/// \param userid Specify the user ID for which you want to get the information. 
	/// \return If the function succeeds, the return value is the authorizer's bot list in the meeting.
	///Otherwise the function fails, and the return value is NULL.
	/// \remarks Valid for both ZOOM style and user custom interface mode.
	static void GetAuthorizedBotListByUserID(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Determine if the current user can enable participant request clould recording.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void CanEnableParticipantRequestCloudRecording(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Check whether the current meeting allows participants to send cloud recording privilege request, This can only be used in regular meeetings and webinar(no breakout rooms).
	/// \return If allows participants to send request, the return value is true.
	static void IsParticipantRequestCloudRecordingAllowed(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Toggle whether attendees can requests for the host to start a cloud recording. This can only be used in regular meeetings and webinar(no breakout rooms).
	/// \param bAllow TRUE indicates that participants are allowed to send cloud recording privilege requests. 
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise the function fails. To get extended error information, see \link ZNSDKError \endlink enum.
	static void AllowParticipantsToRequestCloudRecording(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Set callback function of notification of users who are in the meeting.
	/// \param 1. callback(function) Callback function of notification of users who are in the meeting, onUserJoin
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetMeetingUserJoinCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Set callback function of notification of users who leave the meeting.
	/// \param 1. callback(function) Callback function of notification of users who leave the meeting, onUserLeft
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetMeetingUserLeftCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Set callback function of notification of the new host.
	/// \param 1. callback(function) Callback function of notification of the new host, onHostChangeNotification
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetMeetingHostChangeCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Set callback event that lets participants request that the host starts cloud recording.
	/// \param 1. callback(function) Callback event that lets participants request that the host starts cloud recording.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnAllowParticipantsRequestCloudRecordingCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Set callback event that the bot relationship changed in the meeting.
	/// \param 1. callback(function) callback event that the bot relationship changed in the meeting, onBotAuthorizerRelationChanged
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnBotAuthorizerRelationChangedCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	static v8::Persistent<v8::Function> constructor;
};
template<>
static void InitClassAttribute<ZoomNodeMeetingParticipantsCtrlWrap >(const v8::Local<v8::FunctionTemplate>& tpl, v8::Isolate* isolate)
{
	tpl->SetClassName(v8::String::NewFromUtf8(
		isolate, "ZoomNodeMeetingParticipantsCtrlWrap", v8::NewStringType::kInternalized).ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetParticipantsList", ZoomNodeMeetingParticipantsCtrlWrap::GetParticipantsList);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetUserInfoByUserID", ZoomNodeMeetingParticipantsCtrlWrap::GetUserInfoByUserID);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetBotAuthorizedUserInfoByUserID", ZoomNodeMeetingParticipantsCtrlWrap::GetBotAuthorizedUserInfoByUserID);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetAuthorizedBotListByUserID", ZoomNodeMeetingParticipantsCtrlWrap::GetAuthorizedBotListByUserID);
	NODE_SET_PROTOTYPE_METHOD(tpl, "CanEnableParticipantRequestCloudRecording", ZoomNodeMeetingParticipantsCtrlWrap::CanEnableParticipantRequestCloudRecording);
	NODE_SET_PROTOTYPE_METHOD(tpl, "IsParticipantRequestCloudRecordingAllowed", ZoomNodeMeetingParticipantsCtrlWrap::IsParticipantRequestCloudRecordingAllowed);
	NODE_SET_PROTOTYPE_METHOD(tpl, "AllowParticipantsToRequestCloudRecording", ZoomNodeMeetingParticipantsCtrlWrap::AllowParticipantsToRequestCloudRecording);

	NODE_SET_PROTOTYPE_METHOD(tpl, "SetMeetingUserJoinCB", ZoomNodeMeetingParticipantsCtrlWrap::SetMeetingUserJoinCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetMeetingUserLeftCB", ZoomNodeMeetingParticipantsCtrlWrap::SetMeetingUserLeftCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetMeetingHostChangeCB", ZoomNodeMeetingParticipantsCtrlWrap::SetMeetingHostChangeCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnAllowParticipantsRequestCloudRecordingCB", ZoomNodeMeetingParticipantsCtrlWrap::SetOnAllowParticipantsRequestCloudRecordingCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnBotAuthorizerRelationChangedCB", ZoomNodeMeetingParticipantsCtrlWrap::SetOnBotAuthorizerRelationChangedCB);
}
template<>
static v8::Persistent<v8::Function>* GetConstructor<ZoomNodeMeetingParticipantsCtrlWrap >() {
	return &ZoomNodeMeetingParticipantsCtrlWrap::constructor;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
