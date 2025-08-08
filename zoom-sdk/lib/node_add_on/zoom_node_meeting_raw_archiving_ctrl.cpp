#include "zoom_node_meeting_raw_archiving_ctrl.h"


ZoomNodeMeetingRawArchivingCtrlWrap::ZoomNodeMeetingRawArchivingCtrlWrap()
{

}

ZoomNodeMeetingRawArchivingCtrlWrap::~ZoomNodeMeetingRawArchivingCtrlWrap()
{
	
}
void ZoomNodeMeetingRawArchivingCtrlWrap::StartRawArchiving(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingRawArchivingCtrl().StartRawArchiving();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingRawArchivingCtrlWrap::StopRawArchiving(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingRawArchivingCtrl().StopRawArchiving();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
