#include "zoom_node_meeting_audio_ctrl.h"

ZoomNodeMeetingAudioCtrlWrap::ZoomNodeMeetingAudioCtrlWrap()
{
	_g_native_wrap.GetMeetingServiceWrap().GetMeetingAudioCtrl().SetSink(&ZoomNodeSinkWrapMgr::GetInst().m_meetingAudioCtrlWrapSink);
}

ZoomNodeMeetingAudioCtrlWrap::~ZoomNodeMeetingAudioCtrlWrap()
{
	_g_native_wrap.GetMeetingServiceWrap().GetMeetingAudioCtrl().SetSink(NULL);
}
void ZoomNodeMeetingAudioCtrlWrap::MuteAudio(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::MuteAudioParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::MuteAudioParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_allowunmutebyself() ||
			!proto_params.has_userid()
			)
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		int _userid = proto_params.userid();
		bool _b_allowunmutebyself = false;
		convertBool(proto_params.allowunmutebyself(), _b_allowunmutebyself);

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAudioCtrl().MuteAudio(_userid, _b_allowunmutebyself);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAudioCtrlWrap::UnMuteAudio(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::UnmuteAudioParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::UnmuteAudioParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_userid())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		int _userid = proto_params.userid();
		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAudioCtrl().UnMuteAudio(_userid);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAudioCtrlWrap::JoinVoip(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAudioCtrl().JoinVoip();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAudioCtrlWrap::LeaveVoip(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAudioCtrl().LeaveVoip();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAudioCtrlWrap::EnableMuteOnEntry(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::EnableMuteOnEntryParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::EnableMuteOnEntryParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_benable() ||
			!proto_params.has_allowunmutebyself()
			)
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		bool _bEnable = false;
		convertBool(proto_params.benable(), _bEnable);

		bool _allowUnmuteBySelf = false;
		convertBool(proto_params.allowunmutebyself(), _allowUnmuteBySelf);

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAudioCtrl().EnableMuteOnEntry(_bEnable, _allowUnmuteBySelf);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAudioCtrlWrap::CanEnableMuteOnEntry(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool zn_bCan = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAudioCtrl().CanEnableMuteOnEntry();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, zn_bCan);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAudioCtrlWrap::IsMuteOnEntryEnabled(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool zn_bIs = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAudioCtrl().IsMuteOnEntryEnabled();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, zn_bIs);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAudioCtrlWrap::SetMeetingAudioStatusCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onUserAudioStatusChange.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onUserAudioStatusChange.Reset(isolate, cbfunc);

	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAudioCtrlWrap::SetUserActiveAudioChangeCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onUserActiveAudioChange.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onUserActiveAudioChange.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAudioCtrlWrap::EnablePlayChimeWhenEnterOrExit(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::EnablePlayChimeWhenEnterOrExitParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::EnablePlayChimeWhenEnterOrExitParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_benable())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		bool _bEnable = false;
		convertBool(proto_params.benable(), _bEnable);

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAudioCtrl().EnablePlayChimeWhenEnterOrExit(_bEnable);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAudioCtrlWrap::EnablePlayMeetingAudio(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::EnablePlayMeetingAudioParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::EnablePlayMeetingAudioParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_benable())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		bool _bEnable = false;
		convertBool(proto_params.benable(), _bEnable);

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAudioCtrl().EnablePlayMeetingAudio(_bEnable);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAudioCtrlWrap::IsPlayMeetingAudioEnabled(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool zn_bIs = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAudioCtrl().IsPlayMeetingAudioEnabled();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, zn_bIs);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAudioCtrlWrap::SetOnMuteOnEntryStatusChangeCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onMuteOnEntryStatusChange.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onMuteOnEntryStatusChange.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
