#ifndef _ZOOM_NODE_MEETING_RAW_ARCHIVING_CTRL_H_
#define _ZOOM_NODE_MEETING_RAW_ARCHIVING_CTRL_H_
#include "zoom_node_common_include.h"
#include "zoom_native_sdk_wrap_core.h"
#include "zoom_singleton_wrap_class.h"

extern ZNativeSDKWrap _g_native_wrap;
class ZOOM_NODE_HIDE ZoomNodeMeetingRawArchivingCtrlWrap :
	public ZoomWrapObject<ZoomNodeMeetingRawArchivingCtrlWrap >
{
	friend class ZoomWrapObject<ZoomNodeMeetingRawArchivingCtrlWrap >;
private:
	ZoomNodeMeetingRawArchivingCtrlWrap();
	~ZoomNodeMeetingRawArchivingCtrlWrap();
public:

	static void StartRawArchiving(const v8::FunctionCallbackInfo<v8::Value>& args);

	static void StopRawArchiving(const v8::FunctionCallbackInfo<v8::Value>& args);

	static v8::Persistent<v8::Function> constructor;
};
template<>
static void InitClassAttribute<ZoomNodeMeetingRawArchivingCtrlWrap >(const v8::Local<v8::FunctionTemplate>& tpl, v8::Isolate* isolate)
{
	tpl->SetClassName(v8::String::NewFromUtf8(
		isolate, "ZoomNodeMeetingRawArchivingCtrlWrap", v8::NewStringType::kInternalized).ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	NODE_SET_PROTOTYPE_METHOD(tpl, "StartRawArchiving", ZoomNodeMeetingRawArchivingCtrlWrap::StartRawArchiving);
	NODE_SET_PROTOTYPE_METHOD(tpl, "StopRawArchiving", ZoomNodeMeetingRawArchivingCtrlWrap::StopRawArchiving);

}
template<>
static v8::Persistent<v8::Function>* GetConstructor<ZoomNodeMeetingRawArchivingCtrlWrap >() {
	return &ZoomNodeMeetingRawArchivingCtrlWrap::constructor;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
