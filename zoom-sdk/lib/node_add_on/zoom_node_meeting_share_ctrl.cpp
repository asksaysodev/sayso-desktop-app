#include "zoom_node_meeting_share_ctrl.h"


ZoomNodeMeetingShareCtrlWrap::ZoomNodeMeetingShareCtrlWrap()
{
	_g_native_wrap.GetMeetingServiceWrap().GetMeetingShareCtrl().SetSink(&ZoomNodeSinkWrapMgr::GetInst().m_meetingShareWrapSink);
}

ZoomNodeMeetingShareCtrlWrap::~ZoomNodeMeetingShareCtrlWrap()
{
	_g_native_wrap.GetMeetingServiceWrap().GetMeetingShareCtrl().SetSink(NULL);
}
void ZoomNodeMeetingShareCtrlWrap::StartAppShare(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::StartAppShareParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::StartAppShareParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_hshareapp())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		ZoomSTRING _zn_hShare_app;
		_zn_hShare_app = s2zs(proto_params.hshareapp());

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingShareCtrl().StartAppShare(_zn_hShare_app);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingShareCtrlWrap::StartMonitorShare(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::StartMonitorShareParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::StartMonitorShareParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_monitorid())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		ZoomSTRING _zn_monitorID;
		_zn_monitorID = s2zs(proto_params.monitorid());
		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingShareCtrl().StartMonitorShare(_zn_monitorID);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingShareCtrlWrap::StopShare(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingShareCtrl().StopShare();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingShareCtrlWrap::SetOnSharingStatusCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onSharingStatus.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onSharingStatus.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingShareCtrlWrap::SetOnFailedToStartShareCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onFailedToStartShare.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onFailedToStartShare.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingShareCtrlWrap::SetOnOptimizingShareForVideoClipStatusChangedCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onOptimizingShareForVideoClipStatusChanged.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onOptimizingShareForVideoClipStatusChanged.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingShareCtrlWrap::IsWhiteboardLegalNoticeAvailable(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool bDisable = _g_native_wrap.GetMeetingServiceWrap().GetMeetingShareCtrl().IsWhiteboardLegalNoticeAvailable();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, bDisable);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingShareCtrlWrap::GetWhiteboardLegalNoticesPrompt(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZoomSTRING zn_prompt = _g_native_wrap.GetMeetingServiceWrap().GetMeetingShareCtrl().GetWhiteboardLegalNoticesPrompt();
	v8::Local<v8::String> bret = v8::String::NewFromUtf8(isolate, zs2s(zn_prompt).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingShareCtrlWrap::GetWhiteboardLegalNoticesExplained(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZoomSTRING zn_explained = _g_native_wrap.GetMeetingServiceWrap().GetMeetingShareCtrl().GetWhiteboardLegalNoticesExplained();
	v8::Local<v8::String> bret = v8::String::NewFromUtf8(isolate, zs2s(zn_explained).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingShareCtrlWrap::SetAudioShareMode(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::SetAudioShareModeParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::SetAudioShareModeParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_mode())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		ZNAudioShareMode _zn_mode = (ZNAudioShareMode)proto_params.mode();

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingShareCtrl().SetAudioShareMode(_zn_mode);
	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingShareCtrlWrap::GetAudioShareMode(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	ZNAudioShareMode _zn_mode;
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingShareCtrl().GetAudioShareMode(_zn_mode);

	v8::HandleScope scope(isolate);
	v8::Local<v8::Object> node = v8::Object::New(isolate);
	node->Set(context, v8::String::NewFromUtf8(isolate, "err", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int32_t)err));
	node->Set(context, v8::String::NewFromUtf8(isolate, "audioShareMode", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int32_t)_zn_mode));

	args.GetReturnValue().Set(node);
}