#ifndef _ZOOM_NODE_MEETING_REACTION_CTRL_H_
#define _ZOOM_NODE_MEETING_REACTION_CTRL_H_
#include "zoom_node_common_include.h"
#include "zoom_native_sdk_wrap_core.h"
#include "zoom_singleton_wrap_class.h"

extern ZNativeSDKWrap _g_native_wrap;

class ZOOM_NODE_HIDE ZoomNodeMeetingReactionCtrlWrap :
	public ZoomWrapObject<ZoomNodeMeetingReactionCtrlWrap >
{
	friend class ZoomWrapObject<ZoomNodeMeetingReactionCtrlWrap >;
private:
	ZoomNodeMeetingReactionCtrlWrap();
	~ZoomNodeMeetingReactionCtrlWrap();
public:
	/// \brief Set the callback event when an emoji is received
	/// \param 1. callback(function)  Callback emoji received.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnEmojiReactionReceivedCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	
	/// \brief Set the callback event when an emoji is received in a webinar
	/// \param 1. callback(function)  Callback emoji received in webinar.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnEmojiReactionReceivedInWebinarCB(const v8::FunctionCallbackInfo<v8::Value>& args);

	static v8::Persistent<v8::Function> constructor;
};
template<>
static void InitClassAttribute<ZoomNodeMeetingReactionCtrlWrap >(const v8::Local<v8::FunctionTemplate>& tpl, v8::Isolate* isolate)
{
	tpl->SetClassName(v8::String::NewFromUtf8(
		isolate, "ZoomNodeMeetingReactionCtrlWrap", v8::NewStringType::kInternalized).ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnEmojiReactionReceivedCB", ZoomNodeMeetingReactionCtrlWrap::SetOnEmojiReactionReceivedCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnEmojiReactionReceivedInWebinarCB", ZoomNodeMeetingReactionCtrlWrap::SetOnEmojiReactionReceivedInWebinarCB);

}
template<>
static v8::Persistent<v8::Function>* GetConstructor<ZoomNodeMeetingReactionCtrlWrap >() {
	return &ZoomNodeMeetingReactionCtrlWrap::constructor;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif
