#ifndef _ZOOM_NODE_MEETING_CLOSE_CAPTION_CTRL_H_
#define _ZOOM_NODE_MEETING_CLOSE_CAPTION_CTRL_H_
#include "zoom_node_common_include.h"
#include "zoom_native_sdk_wrap_core.h"
#include "zoom_singleton_wrap_class.h"

extern ZNativeSDKWrap _g_native_wrap;

class ZOOM_NODE_HIDE ZoomNodeCloseCaptionCtrlWrap :
	public ZoomWrapObject<ZoomNodeCloseCaptionCtrlWrap >
{
	friend class ZoomWrapObject<ZoomNodeCloseCaptionCtrlWrap >;
private:
	ZoomNodeCloseCaptionCtrlWrap();
	~ZoomNodeCloseCaptionCtrlWrap();
public:
	/// \brief Set the callback event when a live transcription message is received.
	/// \param 1. callback(function)  Callback transcription received.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnLiveTranscriptionMsgInfoReceivedCB(const v8::FunctionCallbackInfo<v8::Value>& args);

	/// \brief Set the callback event when ther original transcription message is received.
	/// \param 1. callback(function)  Callback transcription received.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnOriginalLanguageMsgReceivedCB(const v8::FunctionCallbackInfo<v8::Value>& args);

	static v8::Persistent<v8::Function> constructor;
};
template<>
static void InitClassAttribute<ZoomNodeCloseCaptionCtrlWrap >(const v8::Local<v8::FunctionTemplate>& tpl, v8::Isolate* isolate)
{
	tpl->SetClassName(v8::String::NewFromUtf8(
		isolate, "ZoomNodeCloseCaptionCtrlWrap", v8::NewStringType::kInternalized).ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnLiveTranscriptionMsgInfoReceivedCB", ZoomNodeCloseCaptionCtrlWrap::SetOnLiveTranscriptionMsgInfoReceivedCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnOriginalLanguageMsgReceivedCB", ZoomNodeCloseCaptionCtrlWrap::SetOnOriginalLanguageMsgReceivedCB);


}
template<>
static v8::Persistent<v8::Function>* GetConstructor<ZoomNodeCloseCaptionCtrlWrap >() {

	return &ZoomNodeCloseCaptionCtrlWrap::constructor;
}

#endif