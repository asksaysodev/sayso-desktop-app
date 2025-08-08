#ifndef _ZOOM_NODE_MEETING_WHITEBOARD_CTRL_H_
#define _ZOOM_NODE_MEETING_WHITEBOARD_CTRL_H_
#include "zoom_node_common_include.h"
#include "zoom_native_sdk_wrap_core.h"
#include "zoom_singleton_wrap_class.h"

extern ZNativeSDKWrap _g_native_wrap;

class ZOOM_NODE_HIDE ZoomNodeMeetingWhiteboardCtrlWrap :
	public ZoomWrapObject<ZoomNodeMeetingWhiteboardCtrlWrap >
{
	friend class ZoomWrapObject<ZoomNodeMeetingWhiteboardCtrlWrap >;
private:
	ZoomNodeMeetingWhiteboardCtrlWrap();
	~ZoomNodeMeetingWhiteboardCtrlWrap();
public:
	/// \brief Set the whiteboard web view and dashboard web view owner window.
	/// \param [in] hOwner Specify the owner window. If the hOwner is nullptr, the owner window will be desktop window. 
	/// \Call this function before calling ShowDashboardView. Call this function when receiving the meeting status MEETING_STATUS_INMEETING. 
	///Otherwise, the dashboard web view owner window will be desktop window. 
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	/// \remarks Valid only for user custom interface mode.
	static void SetOwnerWindow(const v8::FunctionCallbackInfo<v8::Value>& args);
	// \brief Determine whether the current meeting supports the whiteboard or not.
	/// \return True indicates to support.
	static void IsSupportWhiteBoard(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Determine whether the current meeting can start sharing the whiteboard. 
	/// \param [out] reason The reason that no one can start sharing the whiteboard. See \link CannotShareReasonType \endlink enum.
	/// \return True indicates you can start sharing the whiteboard.
	static void CanStartShareWhiteboard(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Show the dashboard web view window.
	/// \param x: The horizontal coordinate value. 
	/// \param y: The vertical coordinate value. 
	/// \remarks If x == -1 and y = -1, the window position will be in the center of the screen where the owner window is located.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	/// \remarks Valid only for custom user interface mode.
	static void ShowDashboardView(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Set the dashboard web view window size.
	/// \param height Specify the height of the window. 
	/// \param width Specify the width of the window. 
	/// \remarks If height == -1 and width == -1, the window size will be default.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	/// \remarks Valid only for custom user interface mode.
	static void SetDashboardViewSize(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Set the whiteboard web view window position.
	/// \param x: The horizontal coordinate value. 
	/// \param y: The vertical coordinate value.
	/// \remarks If x == -1 and y = -1, the window position will be in the center of the screen where the owner window is located.
	/// \remarks If you don't call this function, the window position will be in the center of the screen where the owner window is located.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	/// \remarks Valid only for custom user interface mode.
	static void SetWhiteboardViewPos(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Set the whiteboard web view window size.
	/// \param height Specify the height of the window. 
	/// \param width Specify the width of the window. 
	/// \remarks If height == -1 and width == -1, the window size will be default.
	/// \remarks If you don't call this function, the window size will be default.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	/// \remarks Valid only for custom user interface mode.
	static void SetWhiteboardViewSize(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Set the option for who can share a whiteboard.
	/// \param [in] option New setting for who can share a whiteboard, see \link SDKWhiteboardShareOption \endlink enum.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	static void SetWhiteboardShareOption(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Get the option for who can share a whiteboard.
	/// \param [out] option Setting option for who can share a whiteboard, see \link SDKWhiteboardShareOption \endlink enum.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	static void GetWhiteboardShareOption(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Set the option for who can initiate a new whiteboard.
	/// \param [in] option Setting option for who can initiate a new whiteboard, see \link SDKWhiteboardCreateOption \endlink enum.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	static void SetWhiteboardCreateOption(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Get the option for who can initiate a new whiteboard.
	/// \param [out] option Setting option for who can initiate a new whiteboard, see \link SDKWhiteboardCreateOption \endlink enum.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	static void GetWhiteboardCreateOption(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Enable the participants to create a new whiteboard without the host in the meeting. 
	/// \param bEnable TRUE indicates to enable. FALSE not.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	static void EnableParticipantsCreateWithoutHost(const v8::FunctionCallbackInfo<v8::Value>& args);
	// \brief Determine whether enable the participants create a new whiteboard without the host in the meeting.
	/// \return True indicates that they have these permission.
	static void IsParticipantsCreateWithoutHostEnabled(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Query if other user is sharing whiteboard.
	/// \return True means other user is sharing. False means not.
	static void IsOtherSharingWhiteboard(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Query if the current user is sharing whiteboard successfully.
	/// \return True means the current user is sharing successfully. False means not.
	static void IsSharingWhiteboardOut(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Whiteboard status changed callback. Use this function to inform the user that the whiteboard has been started or stopped, and all users in the meeting can get the event.
	/// \param status Specify current whiteboard status. For more details, see \link SDKWhiteboardStatus \endlink enum.
	static void SetOnWhiteboardStatusChangedCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Callback event of whiteboard setting type changed.
	/// \param shareOption Who can share their whiteboard. For more details, see \link SDKWhiteboardShareOption \endlink enum.
	/// \param createOption Who can create a new whiteboard. For more details, see \link SDKWhiteboardCreateOption \endlink enum.
	/// \param bEnable Whether enable the participants create whiteboard without host in the meeting.
	static void SetOnWhiteboardSettingsChangedCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	
	static v8::Persistent<v8::Function> constructor;
};
template<>
void InitClassAttribute<ZoomNodeMeetingWhiteboardCtrlWrap >(const v8::Local<v8::FunctionTemplate>& tpl, v8::Isolate* isolate)
{
	tpl->SetClassName(v8::String::NewFromUtf8(
		isolate, "ZoomNodeMeetingWhiteboardCtrlWrap", v8::NewStringType::kInternalized).ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOwnerWindow", ZoomNodeMeetingWhiteboardCtrlWrap::SetOwnerWindow);
	NODE_SET_PROTOTYPE_METHOD(tpl, "IsSupportWhiteBoard", ZoomNodeMeetingWhiteboardCtrlWrap::IsSupportWhiteBoard);
	NODE_SET_PROTOTYPE_METHOD(tpl, "CanStartShareWhiteboard", ZoomNodeMeetingWhiteboardCtrlWrap::CanStartShareWhiteboard);
	NODE_SET_PROTOTYPE_METHOD(tpl, "ShowDashboardView", ZoomNodeMeetingWhiteboardCtrlWrap::ShowDashboardView);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetDashboardViewSize", ZoomNodeMeetingWhiteboardCtrlWrap::SetDashboardViewSize);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetWhiteboardViewPos", ZoomNodeMeetingWhiteboardCtrlWrap::SetWhiteboardViewPos);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetWhiteboardViewSize", ZoomNodeMeetingWhiteboardCtrlWrap::SetWhiteboardViewSize);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetWhiteboardShareOption", ZoomNodeMeetingWhiteboardCtrlWrap::SetWhiteboardShareOption);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetWhiteboardShareOption", ZoomNodeMeetingWhiteboardCtrlWrap::GetWhiteboardShareOption);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetWhiteboardCreateOption", ZoomNodeMeetingWhiteboardCtrlWrap::SetWhiteboardCreateOption);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetWhiteboardCreateOption", ZoomNodeMeetingWhiteboardCtrlWrap::GetWhiteboardCreateOption);
	NODE_SET_PROTOTYPE_METHOD(tpl, "EnableParticipantsCreateWithoutHost", ZoomNodeMeetingWhiteboardCtrlWrap::EnableParticipantsCreateWithoutHost);
	NODE_SET_PROTOTYPE_METHOD(tpl, "IsParticipantsCreateWithoutHostEnabled", ZoomNodeMeetingWhiteboardCtrlWrap::IsParticipantsCreateWithoutHostEnabled);
	NODE_SET_PROTOTYPE_METHOD(tpl, "IsOtherSharingWhiteboard", ZoomNodeMeetingWhiteboardCtrlWrap::IsOtherSharingWhiteboard);
	NODE_SET_PROTOTYPE_METHOD(tpl, "IsSharingWhiteboardOut", ZoomNodeMeetingWhiteboardCtrlWrap::IsSharingWhiteboardOut);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnWhiteboardStatusChangedCB", ZoomNodeMeetingWhiteboardCtrlWrap::SetOnWhiteboardStatusChangedCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnWhiteboardSettingsChangedCB", ZoomNodeMeetingWhiteboardCtrlWrap::SetOnWhiteboardSettingsChangedCB);
}
template<>
v8::Persistent<v8::Function>* GetConstructor<ZoomNodeMeetingWhiteboardCtrlWrap >() {
	return &ZoomNodeMeetingWhiteboardCtrlWrap::constructor;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif
