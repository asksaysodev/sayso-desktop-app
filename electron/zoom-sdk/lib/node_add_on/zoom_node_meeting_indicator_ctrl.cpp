#include "zoom_node_meeting_indicator_ctrl.h"

ZoomNodeMeetingIndicatorCtrlWrap::ZoomNodeMeetingIndicatorCtrlWrap()
{
	_g_native_wrap.GetMeetingServiceWrap().GetMeetingIndicatorCtrl().SetSink(&ZoomNodeSinkWrapMgr::GetInst().m_meetingIndicatorWrapSink);
}

ZoomNodeMeetingIndicatorCtrlWrap::~ZoomNodeMeetingIndicatorCtrlWrap()
{
	_g_native_wrap.GetMeetingServiceWrap().GetMeetingIndicatorCtrl().SetSink(NULL);
}
void ZoomNodeMeetingIndicatorCtrlWrap::GetIndicatorItemId(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZoomSTRING zn_id = _g_native_wrap.GetMeetingServiceWrap().GetMeetingIndicatorCtrl().GetIndicatorItemId();
	v8::Local<v8::String> bret = v8::String::NewFromUtf8(isolate, zs2s(zn_id).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingIndicatorCtrlWrap::GetIndicatorName(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZoomSTRING zn_name = _g_native_wrap.GetMeetingServiceWrap().GetMeetingIndicatorCtrl().GetIndicatorName();
	v8::Local<v8::String> bret = v8::String::NewFromUtf8(isolate, zs2s(zn_name).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingIndicatorCtrlWrap::GetIndicatorIcon(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZoomSTRING zn_icon = _g_native_wrap.GetMeetingServiceWrap().GetMeetingIndicatorCtrl().GetIndicatorIcon();
	v8::Local<v8::String> bret = v8::String::NewFromUtf8(isolate, zs2s(zn_icon).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingIndicatorCtrlWrap::ShowIndicatorPanel(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::ShowIndicatorPanelParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::ShowIndicatorPanelParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_x() ||
			!proto_params.has_y()
#if (defined TARGET_OS_MAC)
			|| !proto_params.has_windowid()
#endif
			)
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		unsigned int _x = proto_params.x();
		unsigned int _y = proto_params.y();
		uint64_t _windowId = 0;
#if (defined TARGET_OS_MAC)
		_windowId = proto_params.windowid();
#endif

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingIndicatorCtrl().ShowIndicatorPanel(_x, _y, _windowId);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingIndicatorCtrlWrap::HideIndicatorPanel(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingIndicatorCtrl().HideIndicatorPanel();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodeMeetingIndicatorCtrlWrap::SetOnIndicatorItemReceivedCB(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		if (args.Length() < 1) {
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (args[0]->IsNull())
		{
			ZoomNodeSinkHelper::GetInst().onIndicatorItemReceived.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onIndicatorItemReceived.Reset(isolate, cbfunc);

	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingIndicatorCtrlWrap::SetOnIndicatorItemRemovedCB(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		if (args.Length() < 1) {
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (args[0]->IsNull())
		{
			ZoomNodeSinkHelper::GetInst().onIndicatorItemRemoved.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onIndicatorItemRemoved.Reset(isolate, cbfunc);

	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}