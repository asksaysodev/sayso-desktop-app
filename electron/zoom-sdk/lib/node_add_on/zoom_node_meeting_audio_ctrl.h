#ifndef _ZOOM_NODE_MEETING_AUDIO_CTRL_H_
#define _ZOOM_NODE_MEETING_AUDIO_CTRL_H_
#include "zoom_node_common_include.h"
#include "zoom_native_sdk_wrap_core.h"
#include "zoom_singleton_wrap_class.h"

extern ZNativeSDKWrap _g_native_wrap;

class ZOOM_NODE_HIDE ZoomNodeMeetingAudioCtrlWrap :
	public ZoomWrapObject<ZoomNodeMeetingAudioCtrlWrap >
{
	friend class ZoomWrapObject<ZoomNodeMeetingAudioCtrlWrap >;
private:
	ZoomNodeMeetingAudioCtrlWrap();
	~ZoomNodeMeetingAudioCtrlWrap();
public:
	/// \brief Mute the assigned user.
	/// \param 1. userid(number) Specify the user ID to mute. ZERO(0) indicates to mute all the participants.
	/// \param 2. allowUnmuteBySelf(bool) The user may unmute himself when everyone is muted.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void MuteAudio(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Unmute the assigned user. 
	/// \param 1. userid(number) Specify the user ID to unmute. ZERO(0) indicates to unmute all the participants. 
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void UnMuteAudio(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Join VoIP meeting.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void JoinVoip(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Leave VoIP meeting.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void LeaveVoip(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Mute or umute the user after joining the meeting. 
	/// \param bEnable TRUE indicates to mute the user after joining the meeting.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	/// \remarks Valid for both ZOOM style and user custom interface mode.
	static void EnableMuteOnEntry(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Check if the host or cohost can enable mute on entry.
	/// \return TRUE indicates that the host or cohost can enable mute on entry. Otherwise not. 
	/// \remarks Valid for both ZOOM style and user custom interface mode.
	static void CanEnableMuteOnEntry(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Determine if mute on entry is enabled.
	/// \return TRUE indicates that mute on entry is enabled. 
	static void IsMuteOnEntryEnabled(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Set user's audio status changed callback function.
	/// \param 1. callback(function)  Callback of user's audio status changed.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetMeetingAudioStatusCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Set the callback event that users whose audio is active changed.
	/// \param 1. callback(function)  Callback that users whose audio is active changed.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetUserActiveAudioChangeCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Callback event for the mute on entry status change. 
	/// \param bEnabled Specify whether mute on entry is enabled or not.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnMuteOnEntryStatusChangeCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief User joins or leaves the meeting in silence or no.
	/// \param bEnable TRUE indicates to play chime when the user joins or leaves the meeting.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	/// \remarks Valid for both ZOOM style and user custom interface mode.
	static void EnablePlayChimeWhenEnterOrExit(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// brief Enable or disable SDK to play meeting audio.
	/// \param bEnable True means that SDK will play meeting audio, False means that SDK will not play meeting audio.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	/// Otherwise fails. To get extended error information, see \link SDKError \endlink enum.
	/// Note: SDK will not support sharing computer sound when disabling playing meeting audio.
	static void EnablePlayMeetingAudio(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Determine if play meeting audio is enabled or not.
	/// \return TRUE means enabled, otherwise it is not enabled. 
	static void IsPlayMeetingAudioEnabled(const v8::FunctionCallbackInfo<v8::Value>& args);
	
	static v8::Persistent<v8::Function> constructor;
};
template<>
static void InitClassAttribute<ZoomNodeMeetingAudioCtrlWrap >(const v8::Local<v8::FunctionTemplate>& tpl, v8::Isolate* isolate)
{
	tpl->SetClassName(v8::String::NewFromUtf8(
		isolate, "ZoomNodeMeetingAudioCtrlWrap", v8::NewStringType::kInternalized).ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	NODE_SET_PROTOTYPE_METHOD(tpl, "MuteAudio", ZoomNodeMeetingAudioCtrlWrap::MuteAudio);
	NODE_SET_PROTOTYPE_METHOD(tpl, "UnMuteAudio", ZoomNodeMeetingAudioCtrlWrap::UnMuteAudio);
	NODE_SET_PROTOTYPE_METHOD(tpl, "JoinVoip", ZoomNodeMeetingAudioCtrlWrap::JoinVoip);
	NODE_SET_PROTOTYPE_METHOD(tpl, "LeaveVoip", ZoomNodeMeetingAudioCtrlWrap::LeaveVoip);
	NODE_SET_PROTOTYPE_METHOD(tpl, "EnableMuteOnEntry", ZoomNodeMeetingAudioCtrlWrap::EnableMuteOnEntry);
	NODE_SET_PROTOTYPE_METHOD(tpl, "CanEnableMuteOnEntry", ZoomNodeMeetingAudioCtrlWrap::CanEnableMuteOnEntry);
	NODE_SET_PROTOTYPE_METHOD(tpl, "IsMuteOnEntryEnabled", ZoomNodeMeetingAudioCtrlWrap::IsMuteOnEntryEnabled);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetMeetingAudioStatusCB", ZoomNodeMeetingAudioCtrlWrap::SetMeetingAudioStatusCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetUserActiveAudioChangeCB", ZoomNodeMeetingAudioCtrlWrap::SetUserActiveAudioChangeCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "EnablePlayChimeWhenEnterOrExit", ZoomNodeMeetingAudioCtrlWrap::EnablePlayChimeWhenEnterOrExit);
	NODE_SET_PROTOTYPE_METHOD(tpl, "EnablePlayMeetingAudio", ZoomNodeMeetingAudioCtrlWrap::EnablePlayMeetingAudio);
	NODE_SET_PROTOTYPE_METHOD(tpl, "IsPlayMeetingAudioEnabled", ZoomNodeMeetingAudioCtrlWrap::IsPlayMeetingAudioEnabled);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnMuteOnEntryStatusChangeCB", ZoomNodeMeetingAudioCtrlWrap::SetOnMuteOnEntryStatusChangeCB);
}
template<>
static v8::Persistent<v8::Function>* GetConstructor<ZoomNodeMeetingAudioCtrlWrap >() {
	return &ZoomNodeMeetingAudioCtrlWrap::constructor;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif
