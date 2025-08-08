
#include "zoom_node_render_addon.h"
#include "zoom_v8_to_c.h"

#include "uv_ipc_common.h"
#include "raw_data_format.h"
#include <iostream>
#include <string>
#include "zoom_sdk_napi_util_exporter.h"

extern PipeInitParams _g_pipeInitParam;

static void CreateRenderNodeObject(const v8::FunctionCallbackInfo<v8::Value>& args) {

	RenderAddonData* data =
		reinterpret_cast<RenderAddonData*>(args.Data().As<v8::External>()->Value());
	data->zoomNodeWrapInstance = ZoomNodeRenderWrap::GetNewInstance(args);
	args.GetReturnValue().Set(data->zoomNodeWrapInstance);
}


NODE_MODULE_INIT(/* exports, module, context */) {
	v8::Isolate* isolate = context->GetIsolate();

	ZoomNodeRenderWrap::Init(exports->GetIsolate());
	ZoomNodeVideoRawDataLibuvClientWrap::Init(exports->GetIsolate());
	ZoomNodeShareRawDataLibuvClientWrap::Init(exports->GetIsolate());
	ZoomNodeAudioRawDataLibuvClientWrap::Init(exports->GetIsolate());

	RenderAddonData* data = new RenderAddonData(isolate);

	v8::Local<v8::External> external = v8::External::New(isolate, data);

	exports->Set(context,
		v8::String::NewFromUtf8(isolate, "exports", v8::NewStringType::kNormal)
		.ToLocalChecked(),
		v8::FunctionTemplate::New(isolate, CreateRenderNodeObject, external)
		->GetFunction(context).ToLocalChecked()).FromJust();
}

v8::Persistent<v8::Function> ZoomNodeRenderWrap::constructor;
v8::Persistent<v8::Function> ZoomNodeVideoRawDataLibuvClientWrap::constructor;
v8::Persistent<v8::Function> ZoomNodeShareRawDataLibuvClientWrap::constructor;
v8::Persistent<v8::Function> ZoomNodeAudioRawDataLibuvClientWrap::constructor;

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

void ZoomNodeRenderWrap::GetVideoLibuvClientObj(const v8::FunctionCallbackInfo<v8::Value>& args) {
	ZoomNodeVideoRawDataLibuvClientWrap::NewInstance(args);
	if (!ZoomNodeAPIUtilHelper::GetInst().m_bInit)
	{
		ZoomNodeAPIUtilHelper::GetInst().ExportNAPIUtilFunc();
	}
}
void ZoomNodeRenderWrap::GetShareLibuvClientObj(const v8::FunctionCallbackInfo<v8::Value>& args) {
	ZoomNodeShareRawDataLibuvClientWrap::NewInstance(args);
	if (!ZoomNodeAPIUtilHelper::GetInst().m_bInit)
	{
		ZoomNodeAPIUtilHelper::GetInst().ExportNAPIUtilFunc();
	}
}
void ZoomNodeRenderWrap::GetAudioLibuvClientObj(const v8::FunctionCallbackInfo<v8::Value>& args) {
	ZoomNodeAudioRawDataLibuvClientWrap::NewInstance(args);
	if (!ZoomNodeAPIUtilHelper::GetInst().m_bInit)
	{
		ZoomNodeAPIUtilHelper::GetInst().ExportNAPIUtilFunc();
	}
}
void ZoomNodeRenderWrap::SetPipeClientInitParam(const v8::FunctionCallbackInfo<v8::Value>& args) {
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////
v8::Persistent<v8::Function> g_cb_onRenderVideoRawDataReceived;
v8::Persistent<v8::Function> g_cb_onRenderShareRawDataReceived;
v8::Persistent<v8::Function> g_cb_onMixedAudioRawDataReceived;
v8::Persistent<v8::Function> g_cb_onOneWayAudioRawDataReceived;

void HandleFreeDataBufferNotification(void* hint)
{
    UVIPCMessage* msg = (UVIPCMessage*)hint;
    if (msg)
    {
        msg->Release();
    }
}

void SendPingPongMsg(int type, unsigned int source_id);
void RunVideoFormatDataCB(char* msg_buf, int type, UVIPCMessage* msg)
{
	if ((TYPE_VIDEO != type && TYPE_SHARE != type) || NULL == msg_buf|| NULL == msg)
	{
		return;
	}
	if (TYPE_VIDEO == type && g_cb_onRenderVideoRawDataReceived.IsEmpty())
	{
		return;
	}
	if (TYPE_SHARE == type && g_cb_onRenderShareRawDataReceived.IsEmpty())
	{
		return;
	}
	fnCallbackVideoFormatDataToJS pCallbackVideoFormatDataToJS = NULL;
	pCallbackVideoFormatDataToJS = ZoomNodeAPIUtilHelper::GetInst().m_fnCallbackVideoFormatDataToJS;
	if (!pCallbackVideoFormatDataToJS)
	{
		return;
	}

	auto isolate = v8::Isolate::GetCurrent();
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();
	auto global = context->Global();

	VideoRawDataHeader& header = *(VideoRawDataHeader*)msg_buf;
	char format_[200] = { 0 };
	//type islimit width height rotation yoffset uoffset voffset
	sprintf(format_, "%s;%d;%d;%d;%d;%llu;%llu;%llu", "yuvi420", header.isLimitedI420,    //checked safe
		header.width, header.height, header.rotation, header.y_offset, header.u_offset, header.v_offset);

	char* data_buf_ptr = msg_buf + sizeof(VideoRawDataHeader);
	switch (type)
	{
	case TYPE_VIDEO:
	{
		auto fn = v8::Local<v8::Function>::New(isolate, g_cb_onRenderVideoRawDataReceived);
		msg->AddRef();
		pCallbackVideoFormatDataToJS((uint64_t)header.recvHandle, std::string(format_), data_buf_ptr, header.dataBufferLen, fn, HandleFreeDataBufferNotification, msg);
		SendPingPongMsg(TYPE_VIDEO, header.source_id);
	}
	break;
	case TYPE_SHARE:
	{
		auto fn = v8::Local<v8::Function>::New(isolate, g_cb_onRenderShareRawDataReceived);
		msg->AddRef();
		pCallbackVideoFormatDataToJS((uint64_t)header.recvHandle, std::string(format_), data_buf_ptr, header.dataBufferLen, fn, HandleFreeDataBufferNotification, msg);
		SendPingPongMsg(TYPE_SHARE, header.source_id);
	}
	break;
	default:
		break;
	}
}

void RunAudioFormatDataCB(char* msg_buf, UVIPCMessage* msg)
{
	if ((g_cb_onMixedAudioRawDataReceived.IsEmpty() && g_cb_onOneWayAudioRawDataReceived.IsEmpty())
		|| NULL == msg_buf|| NULL == msg)
	{
		return;
	}
	fnCallbackMixedAudioFormatDataToJS pCallbackMixedAudioFormatDataToJS = NULL;
	fnCallbackOnewayAudioFormatDataToJS pCallbackOnewayAudioFormatDataToJS = NULL;
	pCallbackMixedAudioFormatDataToJS = ZoomNodeAPIUtilHelper::GetInst().m_fnCallbackMixedAudioFormatDataToJS;
	pCallbackOnewayAudioFormatDataToJS = ZoomNodeAPIUtilHelper::GetInst().m_fnCallbackOnewayAudioFormatDataToJS;
	if (!pCallbackMixedAudioFormatDataToJS)
	{
		return;
	}
	if (!pCallbackOnewayAudioFormatDataToJS)
	{
		return;
	}
	auto isolate = v8::Isolate::GetCurrent();
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();
	auto global = context->Global();
	AudioRawDataHeader& header = *(AudioRawDataHeader*)msg_buf;
	if (0 == header.user_id && g_cb_onMixedAudioRawDataReceived.IsEmpty())
		return;
	if (0 != header.user_id && g_cb_onOneWayAudioRawDataReceived.IsEmpty())
		return;
	char format_[200] = { 0 };
	//type buffer_len sample_rate channel_num 
	sprintf(format_, "%s;%d;%d;%d;", "AudioRawData", header.dataBufferLen, header.sampleRate, header.channelNum);    //checked safe
	//buffer
	char* audio_data_buf_ptr = msg_buf + sizeof(AudioRawDataHeader);
	if (0 == header.user_id)
	{
		auto fn = v8::Local<v8::Function>::New(isolate, g_cb_onMixedAudioRawDataReceived);
		msg->AddRef();
		pCallbackMixedAudioFormatDataToJS(std::string(format_), audio_data_buf_ptr, header.dataBufferLen, fn, HandleFreeDataBufferNotification, msg);
	}
	else
	{
		auto fn = v8::Local<v8::Function>::New(isolate, g_cb_onOneWayAudioRawDataReceived);
		msg->AddRef();
		pCallbackOnewayAudioFormatDataToJS((int32_t)header.user_id, std::string(format_), audio_data_buf_ptr, header.dataBufferLen, fn, HandleFreeDataBufferNotification, msg);
	}
}
class UVIPCClientSink : public UVIPCSink
{
public:
	friend class ZoomNodeVideoRawDataLibuvClientWrap;
	UVIPCClientSink()
	{
		
	}
	~UVIPCClientSink()
	{
		
	}
	virtual void onMessageRecvNotification(UVIPCMessage* msg)
	{
		char* msg_buf = msg->GetBuf();
		if (NULL == msg_buf)
			return;

		RawDataCommonHeader common_header;
		memcpy(&common_header, msg_buf, sizeof(RawDataCommonHeader));   //checked safe
		switch (common_header.type)
		{
		case TYPE_VIDEO:
		case TYPE_SHARE:
			RunVideoFormatDataCB(msg_buf, common_header.type, msg);
			break;
		case TYPE_AUDIO:
			RunAudioFormatDataCB(msg_buf, msg);
			break;
		default:
			break;
		}
	}
	virtual void onConnect()
	{
	}
	virtual void onDisconnect()
	{
	}
	virtual void onIdle()
	{
		
	}

public:
	uv_loop_t* _uv_loop;
	UVIPC _uv_ipc_client;
};
UVIPCClientSink g_ipcSinkClientVideo;
UVIPCClientSink g_ipcSinkClientShare;
UVIPCClientSink g_ipcSinkClientAudio;

void SendPingPongMsg(int type, unsigned int source_id)
{
	switch (type)
	{
	case TYPE_VIDEO:
	{
		UVIPCMessage* msg = new UVIPCMessage((const char*)&source_id, sizeof(unsigned int));
		g_ipcSinkClientVideo._uv_ipc_client.SendMessage(msg);
	}
	break;
	case TYPE_SHARE:
	{
		UVIPCMessage* msg = new UVIPCMessage((const char*)&source_id, sizeof(unsigned int));
		g_ipcSinkClientShare._uv_ipc_client.SendMessage(msg);
	}
	break;
	default:
		break;
	}
}
void ZoomNodeVideoRawDataLibuvClientWrap::SetRenderVideoRawDataCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			g_cb_onRenderVideoRawDataReceived.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		g_cb_onRenderVideoRawDataReceived.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodeVideoRawDataLibuvClientWrap::StartVideoClient(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool err = false;

	g_ipcSinkClientVideo._uv_loop = uv_default_loop();

	err = g_ipcSinkClientVideo._uv_ipc_client.Start(const_cast<char*>(_g_pipeInitParam.videoPipeName.c_str()), false, g_ipcSinkClientVideo._uv_loop, &g_ipcSinkClientVideo);
	
	uv_run(g_ipcSinkClientVideo._uv_loop, UV_RUN_NOWAIT);

	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeVideoRawDataLibuvClientWrap::StopVideoClient(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool err = false;
	err = g_ipcSinkClientVideo._uv_ipc_client.Stop();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodeShareRawDataLibuvClientWrap::SetRenderShareRawDataCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			g_cb_onRenderShareRawDataReceived.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		g_cb_onRenderShareRawDataReceived.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodeShareRawDataLibuvClientWrap::StartShareClient(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    v8::Isolate* isolate = args.GetIsolate();
    bool err = false;
    
    g_ipcSinkClientShare._uv_loop = uv_default_loop();

    err = g_ipcSinkClientShare._uv_ipc_client.Start(const_cast<char*>(_g_pipeInitParam.sharePipeName.c_str()), false, g_ipcSinkClientShare._uv_loop, &g_ipcSinkClientShare);
    uv_run(g_ipcSinkClientShare._uv_loop, UV_RUN_NOWAIT);
    
    v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, err);
    args.GetReturnValue().Set(bret);
}
void ZoomNodeShareRawDataLibuvClientWrap::StopShareClient(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool err = false;
	err = g_ipcSinkClientShare._uv_ipc_client.Stop();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodeAudioRawDataLibuvClientWrap::SetRenderAudioRawDataCB(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		if (args.Length() < 2) {
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		bool wrongArguments = false;
		if (args[0]->IsNull())
		{
			g_cb_onMixedAudioRawDataReceived.Clear();
		}
		else
		{
			if (!args[0]->IsFunction())
				wrongArguments = true;
			else
			{
				v8::Local<v8::Function> cbfunc_mix = v8::Local<v8::Function>::Cast(args[0]);
				g_cb_onMixedAudioRawDataReceived.Reset(isolate, cbfunc_mix);
			}
		}
		if (args[1]->IsNull())
		{
			g_cb_onOneWayAudioRawDataReceived.Clear();
		}
		else
		{
			if (!args[1]->IsFunction())
				wrongArguments = true;
			else
			{
				v8::Local<v8::Function> cbfunc_oneway = v8::Local<v8::Function>::Cast(args[1]);
				g_cb_onOneWayAudioRawDataReceived.Reset(isolate, cbfunc_oneway);
			}
		}
		if (wrongArguments)
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodeAudioRawDataLibuvClientWrap::StartAudioClient(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool err = false;

	g_ipcSinkClientAudio._uv_loop = uv_default_loop();

	err = g_ipcSinkClientAudio._uv_ipc_client.Start(const_cast<char*>(_g_pipeInitParam.audioPipeName.c_str()), false, g_ipcSinkClientAudio._uv_loop, &g_ipcSinkClientAudio);
	uv_run(g_ipcSinkClientAudio._uv_loop, UV_RUN_NOWAIT);

	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeAudioRawDataLibuvClientWrap::StopAudioClient(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool err = false;
	err = g_ipcSinkClientAudio._uv_ipc_client.Stop();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, err);
	args.GetReturnValue().Set(bret);
}
