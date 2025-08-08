#include "zoom_node_rawdata_api_ctrl.h"
#include "zoom_node_rawdata_helper_mgr_class.h"

extern ZoomNodePipeServerMgr _g_video_pipe_server;
extern ZoomNodePipeServerMgr _g_share_pipe_server;
extern ZoomNodePipeServerMgr _g_audio_pipe_server;
extern PipeInitParams _g_pipeInitParam;

ZoomNodeRawApiCtrlWrap::ZoomNodeRawApiCtrlWrap()
{
	
}

ZoomNodeRawApiCtrlWrap::~ZoomNodeRawApiCtrlWrap()
{

}
void ZoomNodeRawApiCtrlWrap::HasRawDataLicense(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool zn_bHas = _g_native_wrap.GetRawAPIWrap().HasRawDataLicense();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, zn_bHas);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeRawApiCtrlWrap::InitAudioRawDataHelper(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetRawAPIWrap().InitAudioRawDataHelper();

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeRawApiCtrlWrap::GetRawdataVideoSourceHelper(const v8::FunctionCallbackInfo<v8::Value>& args)
{

}
void ZoomNodeRawApiCtrlWrap::CreateRenderer(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::CreateRendererParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::CreateRendererParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_recvhandle())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		unsigned long long _zn_recv_handle = proto_params.recvhandle();
		err = _g_native_wrap.GetRawAPIWrap().CreateRenderer(_zn_recv_handle);
	} while (false);
	

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeRawApiCtrlWrap::DestroyRenderer(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::DestroyRendererParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::DestroyRendererParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_recvhandle())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		unsigned long long _zn_recv_handle = proto_params.recvhandle();
		err = _g_native_wrap.GetRawAPIWrap().DestroyRenderer(_zn_recv_handle);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeRawApiCtrlWrap::Subscribe(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::SubscribeParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::SubscribeParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_recvhandle() ||
			!proto_params.has_rawdatatype() ||
			!proto_params.has_subscribeid()
			)
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		uint32_t _zn_subscribeid = proto_params.subscribeid();
		ZNZoomSDKRawDataType _zn_rawdataType = (ZNZoomSDKRawDataType)proto_params.rawdatatype();
		unsigned long long _zn_recv_handle = proto_params.recvhandle();
		err = _g_native_wrap.GetRawAPIWrap().Subscribe(_zn_subscribeid, _zn_rawdataType, _zn_recv_handle);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeRawApiCtrlWrap::UnSubscribe(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::UnSubscribeParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::UnSubscribeParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_recvhandle())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		unsigned long long _zn_recv_handle = proto_params.recvhandle();
		err = _g_native_wrap.GetRawAPIWrap().UnSubscribe(_zn_recv_handle);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeRawApiCtrlWrap::SetRawDataResolution(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::SetRawDataResolutionParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::SetRawDataResolutionParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_recvhandle() ||
			!proto_params.has_resolution()
			)
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		ZNZoomSDKResolution _zn_resolution = (ZNZoomSDKResolution)proto_params.resolution();
		unsigned long long _zn_recv_handle = proto_params.recvhandle();
		err = _g_native_wrap.GetRawAPIWrap().SetRawDataResolution(_zn_resolution, _zn_recv_handle);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeRawApiCtrlWrap::GetResolution(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	ZNZoomSDKResolution _zn_resolution = ZNZoomSDKResolution_NoUse;
	do
	{
		com::electron::sdk::proto::GetResolutionParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::GetResolutionParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_recvhandle())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		unsigned long long _zn_recv_handle = proto_params.recvhandle();
		err = _g_native_wrap.GetRawAPIWrap().GetResolution(_zn_resolution, _zn_recv_handle);
	} while (false);
	

	v8::HandleScope scope(isolate);
	v8::Local<v8::Object> node = v8::Object::New(isolate);
	node->Set(context, v8::String::NewFromUtf8(isolate, "err", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int32_t)err));
	node->Set(context, v8::String::NewFromUtf8(isolate, "resolution", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int32_t)_zn_resolution));

	args.GetReturnValue().Set(node);
}
void ZoomNodeRawApiCtrlWrap::GetRawDataType(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	ZNZoomSDKRawDataType _zn_rawdataType = ZNRAW_DATA_TYPE_VIDEO;
	do
	{
		com::electron::sdk::proto::GetRawDataTypeParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::GetRawDataTypeParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_recvhandle())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		unsigned long long _zn_recv_handle = proto_params.recvhandle();
		err = _g_native_wrap.GetRawAPIWrap().GetRawDataType(_zn_rawdataType, _zn_recv_handle);
	} while (false);
	
	v8::HandleScope scope(isolate);
	v8::Local<v8::Object> node = v8::Object::New(isolate);
	node->Set(context, v8::String::NewFromUtf8(isolate, "err", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int32_t)err));
	node->Set(context, v8::String::NewFromUtf8(isolate, "rawdataType", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int32_t)_zn_rawdataType));

	args.GetReturnValue().Set(node);
}
void ZoomNodeRawApiCtrlWrap::GetSubscribeId(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	uint32_t _zn_subscribeId = 0;
	do
	{
		com::electron::sdk::proto::GetSubscribeIdParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::GetSubscribeIdParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_recvhandle())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		unsigned long long _zn_recv_handle = proto_params.recvhandle();
		err = _g_native_wrap.GetRawAPIWrap().GetSubscribeId(_zn_subscribeId, _zn_recv_handle);
	} while (false);
	
	v8::HandleScope scope(isolate);
	v8::Local<v8::Object> node = v8::Object::New(isolate);
	node->Set(context, v8::String::NewFromUtf8(isolate, "err", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int32_t)err));
	node->Set(context, v8::String::NewFromUtf8(isolate, "subscribeId", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, _zn_subscribeId));

	args.GetReturnValue().Set(node);
}
void ZoomNodeRawApiCtrlWrap::SetonRawDataStatusChangedCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onRawDataStatusChanged.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onRawDataStatusChanged.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodeRawApiCtrlWrap::SubscribeAudioRawdata(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetRawAPIWrap().SubscribeAudioRawdata();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeRawApiCtrlWrap::UnSubscribeAudioRawdata(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetRawAPIWrap().UnSubscribeAudioRawdata();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodeRawApiCtrlWrap::StartPipeServe(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	bool bsucc_video_pipe = false;
	bsucc_video_pipe = _g_video_pipe_server._uv_ipc_server.StartPipeServer(const_cast<char*>(_g_pipeInitParam.videoPipeName.c_str()), &_g_video_pipe_server);
	bool bsucc_share_pipe = false;
	bsucc_share_pipe = _g_share_pipe_server._uv_ipc_server.StartPipeServer(const_cast<char*>(_g_pipeInitParam.sharePipeName.c_str()), &_g_share_pipe_server);
	bool bsucc_audio_pipe = false;
	bsucc_audio_pipe = _g_audio_pipe_server._uv_ipc_server.StartPipeServer(const_cast<char*>(_g_pipeInitParam.audioPipeName.c_str()), &_g_audio_pipe_server);
	if (!bsucc_video_pipe || !bsucc_share_pipe || !bsucc_audio_pipe)
	{
		err = ZNSDKERR_INTERNAL_ERROR;
	}
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeRawApiCtrlWrap::StopPipeServe(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	bool bsucc_video_pipe = false;
	bsucc_video_pipe = _g_video_pipe_server._uv_ipc_server.StopPipeServer();
	_g_video_pipe_server._cached_raw_data_count_map_mutex.lock();
	_g_video_pipe_server._cached_raw_data_count_map.clear();
	_g_video_pipe_server._cached_raw_data_count_map_mutex.unlock();

	bool bsucc_share_pipe = false;
	bsucc_share_pipe = _g_share_pipe_server._uv_ipc_server.StopPipeServer();
	_g_share_pipe_server._cached_raw_data_count_map_mutex.lock();
	_g_share_pipe_server._cached_raw_data_count_map.clear();
	_g_share_pipe_server._cached_raw_data_count_map_mutex.unlock();

	bool bsucc_audio_pipe = false;
	bsucc_audio_pipe = _g_audio_pipe_server._uv_ipc_server.StopPipeServer();
	_g_audio_pipe_server._cached_raw_data_count_map_mutex.lock();
	_g_audio_pipe_server._cached_raw_data_count_map.clear();
	_g_audio_pipe_server._cached_raw_data_count_map_mutex.unlock();
	if (!bsucc_video_pipe || !bsucc_share_pipe || !bsucc_audio_pipe)
	{
		err = ZNSDKERR_INTERNAL_ERROR;
	}
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeRawApiCtrlWrap::SetPipeServeInitParam(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::SetPipeInitParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::SetPipeInitParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_videopipename() ||
			!proto_params.has_sharepipename() ||
			!proto_params.has_audiopipename() ||
			!proto_params.has_maxreadlength()
			)
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (proto_params.videopipename().empty() ||
			proto_params.sharepipename().empty() ||
			proto_params.audiopipename().empty()
			)
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
#if (defined _WIN32)
		std::string zn_prefix = "\\\\.\\pipe\\";

#else
		std::string zn_prefix = "/tmp/";
#endif
		_g_pipeInitParam.videoPipeName = zn_prefix + proto_params.videopipename();
		_g_pipeInitParam.sharePipeName = zn_prefix + proto_params.sharepipename();
		_g_pipeInitParam.audioPipeName = zn_prefix + proto_params.audiopipename();
		_g_pipeInitParam.maxReadLength = proto_params.maxreadlength();
	} while (false);
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}