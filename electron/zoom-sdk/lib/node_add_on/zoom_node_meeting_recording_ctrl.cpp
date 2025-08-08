#include "zoom_node_meeting_recording_ctrl.h"

ZoomNodeMeetingRecordingCtrlWrap::ZoomNodeMeetingRecordingCtrlWrap()
{
	_g_native_wrap.GetMeetingServiceWrap().GetMeetingRecordingCtrl().SetSink(&ZoomNodeSinkWrapMgr::GetInst().m_meetingRecordingWrapSink);
}

ZoomNodeMeetingRecordingCtrlWrap::~ZoomNodeMeetingRecordingCtrlWrap()
{
	_g_native_wrap.GetMeetingServiceWrap().GetMeetingRecordingCtrl().SetSink(NULL);
}
void ZoomNodeMeetingRecordingCtrlWrap::StartRecording(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	time_t _zn_startTimestamp = 0;
	err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingRecordingCtrl().StartRecording(_zn_startTimestamp);
	v8::HandleScope scope(isolate);
	v8::Local<v8::Object> node = v8::Object::New(isolate);
	node->Set(context, v8::String::NewFromUtf8(isolate, "err", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int32_t)err));
	node->Set(context, v8::String::NewFromUtf8(isolate, "startTimestamp", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int64_t)_zn_startTimestamp));

	args.GetReturnValue().Set(node);
}
void ZoomNodeMeetingRecordingCtrlWrap::StopRecording(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	
	time_t zn_stopTimestamp = 0;

	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingRecordingCtrl().StopRecording(zn_stopTimestamp);
	v8::HandleScope scope(isolate);
	v8::Local<v8::Object> node = v8::Object::New(isolate);
	node->Set(context, v8::String::NewFromUtf8(isolate, "err", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int32_t)err));
	node->Set(context, v8::String::NewFromUtf8(isolate, "stopTimestamp", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int64_t)zn_stopTimestamp));

	args.GetReturnValue().Set(node);
}
void ZoomNodeMeetingRecordingCtrlWrap::CanStartRecording(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::CanStartRecordingParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::CanStartRecordingParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_bcloudrecording() ||
			!proto_params.has_userid()
			)
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		bool _bCloudRecording = false;
		convertBool(proto_params.bcloudrecording(), _bCloudRecording);
		unsigned int _zn_userid = proto_params.userid();

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingRecordingCtrl().CanStartRecording(_bCloudRecording, _zn_userid);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingRecordingCtrlWrap::CanAllowDisAllowLocalRecording(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingRecordingCtrl().CanAllowDisAllowLocalRecording();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingRecordingCtrlWrap::StartCloudRecording(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingRecordingCtrl().StartCloudRecording();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingRecordingCtrlWrap::StopCloudRecording(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingRecordingCtrl().StopCloudRecording();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingRecordingCtrlWrap::IsSupportLocalRecording(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::IsSupportLocalRecordingParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::IsSupportLocalRecordingParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_userid())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		unsigned int _zn_userid = proto_params.userid();

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingRecordingCtrl().IsSupportLocalRecording(_zn_userid);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingRecordingCtrlWrap::AllowLocalRecording(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::AllowLocalRecordingParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::AllowLocalRecordingParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_userid())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		unsigned int _zn_userid = proto_params.userid();

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingRecordingCtrl().AllowLocalRecording(_zn_userid);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingRecordingCtrlWrap::DisAllowLocalRecording(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::DisAllowLocalRecordingParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::DisAllowLocalRecordingParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_userid())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		unsigned int _zn_userid = proto_params.userid();

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingRecordingCtrl().DisAllowLocalRecording(_zn_userid);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingRecordingCtrlWrap::RequestStartCloudRecording(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingRecordingCtrl().RequestStartCloudRecording();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingRecordingCtrlWrap::IsSmartRecordingEnabled(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool zn_bIs = _g_native_wrap.GetMeetingServiceWrap().GetMeetingRecordingCtrl().IsSmartRecordingEnabled();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, zn_bIs);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingRecordingCtrlWrap::CanEnableSmartRecordingFeature(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool zn_bCan = _g_native_wrap.GetMeetingServiceWrap().GetMeetingRecordingCtrl().CanEnableSmartRecordingFeature();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, zn_bCan);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingRecordingCtrlWrap::EnableSmartRecording(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingRecordingCtrl().EnableSmartRecording();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingRecordingCtrlWrap::GetRequesterId(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	unsigned int requester_id = _g_native_wrap.GetMeetingServiceWrap().GetMeetingRecordingCtrl().GetRequesterId();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (uint32_t)requester_id);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingRecordingCtrlWrap::GetRequesterName(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZoomSTRING requester_name = _g_native_wrap.GetMeetingServiceWrap().GetMeetingRecordingCtrl().GetRequesterName();
	v8::Local<v8::String> bret = v8::String::NewFromUtf8(isolate, zs2s(requester_name).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingRecordingCtrlWrap::Start(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingRecordingCtrl().Start();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingRecordingCtrlWrap::Deny(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::DenyRequestStartCloudRecordingParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::DenyRequestStartCloudRecordingParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_bdenyall())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		bool _bDenyAll;
		convertBool(proto_params.bdenyall(), _bDenyAll);

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingRecordingCtrl().Deny(_bDenyAll);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingRecordingCtrlWrap::GetRequestEnableAndStartSmartRecordingUserId(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	unsigned int request_user_id = _g_native_wrap.GetMeetingServiceWrap().GetMeetingRecordingCtrl().GetRequestEnableAndStartSmartRecordingUserId();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (uint32_t)request_user_id);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingRecordingCtrlWrap::GetRequestEnableAndStartSmartRecordingTipString(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZoomSTRING tip_string = _g_native_wrap.GetMeetingServiceWrap().GetMeetingRecordingCtrl().GetRequestEnableAndStartSmartRecordingTipString();
	v8::Local<v8::String> bret = v8::String::NewFromUtf8(isolate, zs2s(tip_string).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingRecordingCtrlWrap::StartCloudRecordingWithoutEnableSmartRecording(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingRecordingCtrl().StartCloudRecordingWithoutEnableSmartRecording();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingRecordingCtrlWrap::AgreeToEnableAndStartSmartRecording(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::AgreeToEnableAndStartSmartRecordingParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::AgreeToEnableAndStartSmartRecordingParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_ballmeetings())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		bool _bAllMeetings;
		convertBool(proto_params.ballmeetings(), _bAllMeetings);

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingRecordingCtrl().AgreeToEnableAndStartSmartRecording(_bAllMeetings);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingRecordingCtrlWrap::DeclineEnableAndStartSmartRecording(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::DeclineEnableAndStartSmartRecordingParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::DeclineEnableAndStartSmartRecordingParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_bdenyall())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		bool _bDenyAll;
		convertBool(proto_params.bdenyall(), _bDenyAll);

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingRecordingCtrl().DeclineEnableAndStartSmartRecording(_bDenyAll);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingRecordingCtrlWrap::GetSmartRecordingEnableActionTipString(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZoomSTRING tip_string = _g_native_wrap.GetMeetingServiceWrap().GetMeetingRecordingCtrl().GetSmartRecordingEnableActionTipString();
	v8::Local<v8::String> bret = v8::String::NewFromUtf8(isolate, zs2s(tip_string).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingRecordingCtrlWrap::ActionConfirmEnableSmartRecording(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::ActionConfirmEnableSmartRecordingParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::ActionConfirmEnableSmartRecordingParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_ballmeetings())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		bool _bAllMeetings;
		convertBool(proto_params.ballmeetings(), _bAllMeetings);

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingRecordingCtrl().ActionConfirmEnableSmartRecording(_bAllMeetings);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingRecordingCtrlWrap::ActionCancelEnableSmartRecording(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingRecordingCtrl().ActionCancelEnableSmartRecording();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingRecordingCtrlWrap::SetonRecording2MP4DoneCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onRecording2MP4Done.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onRecording2MP4Done.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingRecordingCtrlWrap::SetonRecording2MP4ProcessingCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onRecording2MP4Processing.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onRecording2MP4Processing.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingRecordingCtrlWrap::SetonRecordingStatusCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onRecordingStatus.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onRecordingStatus.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingRecordingCtrlWrap::SetonCloudRecordingStatusCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onCloudRecordingStatus.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onCloudRecordingStatus.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingRecordingCtrlWrap::SetonRecordPrivilegeChangedCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onRecordPrivilegeChanged.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onRecordPrivilegeChanged.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingRecordingCtrlWrap::SetonCloudRecordingStorageFullCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onCloudRecordingStorageFull.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onCloudRecordingStorageFull.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingRecordingCtrlWrap::SetonRequestCloudRecordingResponseCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onRequestCloudRecordingResponse.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onRequestCloudRecordingResponse.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingRecordingCtrlWrap::SetonStartCloudRecordingRequestedCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onStartCloudRecordingRequested.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onStartCloudRecordingRequested.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingRecordingCtrlWrap::SetonEnableAndStartSmartRecordingRequestedCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onEnableAndStartSmartRecordingRequested.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onEnableAndStartSmartRecordingRequested.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingRecordingCtrlWrap::SetonSmartRecordingEnableActionCallback(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onSmartRecordingEnableActionCallback.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onSmartRecordingEnableActionCallback.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}