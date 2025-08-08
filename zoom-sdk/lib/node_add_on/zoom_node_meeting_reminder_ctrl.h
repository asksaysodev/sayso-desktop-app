#ifndef _ZOOM_NODE_MEETING_REMINDER_CTRL_H_
#define _ZOOM_NODE_MEETING_REMINDER_CTRL_H_
#include "zoom_node_common_include.h"
#include "zoom_native_sdk_wrap_core.h"
#include "zoom_singleton_wrap_class.h"

extern ZNativeSDKWrap _g_native_wrap;

class ZOOM_NODE_HIDE ZoomNodeMeetingReminderCtrlWrap :
	public ZoomWrapObject<ZoomNodeMeetingReminderCtrlWrap >
{
	friend class ZoomWrapObject<ZoomNodeMeetingReminderCtrlWrap >;
private:
	ZoomNodeMeetingReminderCtrlWrap();
	~ZoomNodeMeetingReminderCtrlWrap();
public:
	/// \brief Ignore the reminder.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void Ignore(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Accept the reminder.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void Accept(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Decline the reminder.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void Decline(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Set meeting reminder controller callback function.
	/// \param 1. callback(function)  Callback event of the reminder dialog show.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnReminderNotifyCB(const v8::FunctionCallbackInfo<v8::Value>& args);

	/// \brief Set the option indicating which meetings smart recording will be enabled for.
	/// \param option Specify the option.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise the function fails. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetEnableOption(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Start the smart recording feature.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise the function fails. To get extended error information, see \link ZNSDKError \endlink enum.
	static void StartEnable(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Decline the reminder.
	/// \param bDeclineAll TRUE means decline all reminders,and participants cannot send requests again until the host changes the setting. FALSE means that the host declines only this specific request, not all requests.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise the function fails. To get extended error information, see \link ZNSDKError \endlink enum.
	static void DeclineEnable(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Ignore the reminder.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise the function fails. To get extended error information, see \link ZNSDKError \endlink enum.
	static void IgnoreEnable(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Set callback event of the enable reminder dialog show.
	/// \param 1. callback(function) Callback event of the enable reminder dialog show.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise the function fails. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnEnableReminderNotifyCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	
	static v8::Persistent<v8::Function> constructor;
};
template<>
void InitClassAttribute<ZoomNodeMeetingReminderCtrlWrap >(const v8::Local<v8::FunctionTemplate>& tpl, v8::Isolate* isolate)
{
	tpl->SetClassName(v8::String::NewFromUtf8(
		isolate, "ZoomNodeMeetingReminderCtrlWrap", v8::NewStringType::kInternalized).ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	NODE_SET_PROTOTYPE_METHOD(tpl, "Ignore", ZoomNodeMeetingReminderCtrlWrap::Ignore);
	NODE_SET_PROTOTYPE_METHOD(tpl, "Accept", ZoomNodeMeetingReminderCtrlWrap::Accept);
	NODE_SET_PROTOTYPE_METHOD(tpl, "Decline", ZoomNodeMeetingReminderCtrlWrap::Decline);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnReminderNotifyCB", ZoomNodeMeetingReminderCtrlWrap::SetOnReminderNotifyCB);

	NODE_SET_PROTOTYPE_METHOD(tpl, "SetEnableOption", ZoomNodeMeetingReminderCtrlWrap::SetEnableOption);
	NODE_SET_PROTOTYPE_METHOD(tpl, "StartEnable", ZoomNodeMeetingReminderCtrlWrap::StartEnable);
	NODE_SET_PROTOTYPE_METHOD(tpl, "DeclineEnable", ZoomNodeMeetingReminderCtrlWrap::DeclineEnable);
	NODE_SET_PROTOTYPE_METHOD(tpl, "IgnoreEnable", ZoomNodeMeetingReminderCtrlWrap::IgnoreEnable);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnEnableReminderNotifyCB", ZoomNodeMeetingReminderCtrlWrap::SetOnEnableReminderNotifyCB);
}
template<>
v8::Persistent<v8::Function>* GetConstructor<ZoomNodeMeetingReminderCtrlWrap >() {
	return &ZoomNodeMeetingReminderCtrlWrap::constructor;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif
