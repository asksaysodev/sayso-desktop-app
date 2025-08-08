#ifndef _ZOOM_NODE_MEETING_INDICATOR_CTRL_H_
#define _ZOOM_NODE_MEETING_INDICATOR_CTRL_H_
#include "zoom_node_common_include.h"
#include "zoom_native_sdk_wrap_core.h"
#include "zoom_singleton_wrap_class.h"

extern ZNativeSDKWrap _g_native_wrap;

class ZOOM_NODE_HIDE ZoomNodeMeetingIndicatorCtrlWrap :
	public ZoomWrapObject<ZoomNodeMeetingIndicatorCtrlWrap >
{
	friend class ZoomWrapObject<ZoomNodeMeetingIndicatorCtrlWrap >;
private:
	ZoomNodeMeetingIndicatorCtrlWrap();
	~ZoomNodeMeetingIndicatorCtrlWrap();
public:
	/// \brief get Indicator item ID.
	/// \return The item ID of the Indicator.
	static void GetIndicatorItemId(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief get Indicator name.
	/// \return The name of the Indicator.
	static void GetIndicatorName(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief get Indicator Icon path.
	/// \return The path of the Indicator.
	static void GetIndicatorIcon(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Show Indicator panel window.
	/// \param x: the horizontal coordinate value. 
	/// \param y: the coordinate coordinate value. 
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	static void ShowIndicatorPanel(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Hide Indicator panel window.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	static void HideIndicatorPanel(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Notify receive Callback event IMeetingIndicatorHandler received.
	/// \param handler Indicator handler. For more details, see \link IMeetingIndicatorHandler \endlink enum.
	static void SetOnIndicatorItemReceivedCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Notify receive Callback event IMeetingIndicatorHandler removed.
	/// \param handler Indicator handler. For more details, see \link IMeetingIndicatorHandler \endlink enum.
	static void SetOnIndicatorItemRemovedCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	
	static v8::Persistent<v8::Function> constructor;
};
template<>
void InitClassAttribute<ZoomNodeMeetingIndicatorCtrlWrap >(const v8::Local<v8::FunctionTemplate>& tpl, v8::Isolate* isolate)
{
	tpl->SetClassName(v8::String::NewFromUtf8(
		isolate, "ZoomNodeMeetingIndicatorCtrlWrap", v8::NewStringType::kInternalized).ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetIndicatorItemId", ZoomNodeMeetingIndicatorCtrlWrap::GetIndicatorItemId);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetIndicatorName", ZoomNodeMeetingIndicatorCtrlWrap::GetIndicatorName);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetIndicatorIcon", ZoomNodeMeetingIndicatorCtrlWrap::GetIndicatorIcon);
	NODE_SET_PROTOTYPE_METHOD(tpl, "ShowIndicatorPanel", ZoomNodeMeetingIndicatorCtrlWrap::ShowIndicatorPanel);
	NODE_SET_PROTOTYPE_METHOD(tpl, "HideIndicatorPanel", ZoomNodeMeetingIndicatorCtrlWrap::HideIndicatorPanel);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnIndicatorItemReceivedCB", ZoomNodeMeetingIndicatorCtrlWrap::SetOnIndicatorItemReceivedCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnIndicatorItemRemovedCB", ZoomNodeMeetingIndicatorCtrlWrap::SetOnIndicatorItemRemovedCB);
}
template<>
v8::Persistent<v8::Function>* GetConstructor<ZoomNodeMeetingIndicatorCtrlWrap >() {
	return &ZoomNodeMeetingIndicatorCtrlWrap::constructor;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif