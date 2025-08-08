#include "zoom_node_setting_virtual_bg_ctrl.h"
#include "zoom_node_wrap_util.h"

ZoomNodeSettingVirtualBGCtrlWrap::ZoomNodeSettingVirtualBGCtrlWrap()
{
	_g_native_wrap.GetSettingServiceWrap().GetSettingVirtualBGCtrl().SetSink(&ZoomNodeSinkWrapMgr::GetInst().m_virtualBGSettingContextWrapSink);
}

ZoomNodeSettingVirtualBGCtrlWrap::~ZoomNodeSettingVirtualBGCtrlWrap()
{
	_g_native_wrap.GetSettingServiceWrap().GetSettingVirtualBGCtrl().SetSink(NULL);
}
void ZoomNodeSettingVirtualBGCtrlWrap::IsSupportVirtualBG(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool bIs = _g_native_wrap.GetSettingServiceWrap().GetSettingVirtualBGCtrl().IsSupportVirtualBG();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, bIs);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeSettingVirtualBGCtrlWrap::IsDeviceSupportSmartVirtualBG(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool bIs = _g_native_wrap.GetSettingServiceWrap().GetSettingVirtualBGCtrl().IsDeviceSupportSmartVirtualBG();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, bIs);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeSettingVirtualBGCtrlWrap::IsSupportVirtualBackgroundVideo(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool bIs = _g_native_wrap.GetSettingServiceWrap().GetSettingVirtualBGCtrl().IsSupportVirtualBackgroundVideo();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, bIs);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeSettingVirtualBGCtrlWrap::IsDeviceSupportSmartVirtualBackgroundVideo(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool bIs = _g_native_wrap.GetSettingServiceWrap().GetSettingVirtualBGCtrl().IsDeviceSupportSmartVirtualBackgroundVideo();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, bIs);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeSettingVirtualBGCtrlWrap::IsDeviceSupportGreenVirtualBackgroundVideo(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool bIs = _g_native_wrap.GetSettingServiceWrap().GetSettingVirtualBGCtrl().IsDeviceSupportGreenVirtualBackgroundVideo();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, bIs);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeSettingVirtualBGCtrlWrap::IsUsingGreenScreenOn(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool bIs = _g_native_wrap.GetSettingServiceWrap().GetSettingVirtualBGCtrl().IsUsingGreenScreenOn();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, bIs);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeSettingVirtualBGCtrlWrap::SetUsingGreenScreen(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::SetUsingGreenScreenParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::SetUsingGreenScreenParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_buse())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		bool _bUse = false;
		convertBool(proto_params.buse(), _bUse);
		err = _g_native_wrap.GetSettingServiceWrap().GetSettingVirtualBGCtrl().SetUsingGreenScreen(_bUse);
	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeSettingVirtualBGCtrlWrap::IsAllowToAddNewVBItem(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool bIs = _g_native_wrap.GetSettingServiceWrap().GetSettingVirtualBGCtrl().IsAllowToAddNewVBItem();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, bIs);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeSettingVirtualBGCtrlWrap::IsAllowToRemoveVBItem(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool bIs = _g_native_wrap.GetSettingServiceWrap().GetSettingVirtualBGCtrl().IsAllowToRemoveVBItem();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, bIs);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeSettingVirtualBGCtrlWrap::AddBGImage(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::AddBGImageParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::AddBGImageParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_filepath())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		ZoomSTRING _zn_file_path = s2zs(proto_params.filepath());
		err = _g_native_wrap.GetSettingServiceWrap().GetSettingVirtualBGCtrl().AddBGImage(_zn_file_path);
	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeSettingVirtualBGCtrlWrap::RemoveBGImage(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::RemoveBGImageParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::RemoveBGImageParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_imagehandle())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		int64_t _imageHandle = proto_params.imagehandle();
		err = _g_native_wrap.GetSettingServiceWrap().GetSettingVirtualBGCtrl().RemoveBGImage(_imageHandle);
	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeSettingVirtualBGCtrlWrap::GetBGImageList(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	ZNList<ZNVirtualBGImageInfo> zn_list = _g_native_wrap.GetSettingServiceWrap().GetSettingVirtualBGCtrl().GetBGImageList();
	v8::Local<v8::Array> nodes = NodeWrapUtil::CreateVirtualBGImageInfoObjectList(isolate, zn_list);
	args.GetReturnValue().Set(nodes);
}
void ZoomNodeSettingVirtualBGCtrlWrap::UseBGImage(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::UseBGImageParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::UseBGImageParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_imagehandle())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		int64_t _imageHandle = proto_params.imagehandle();
		err = _g_native_wrap.GetSettingServiceWrap().GetSettingVirtualBGCtrl().UseBGImage(_imageHandle);
	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeSettingVirtualBGCtrlWrap::GetBGReplaceColor(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	ZNZoomSDKColor zn_color = _g_native_wrap.GetSettingServiceWrap().GetSettingVirtualBGCtrl().GetBGReplaceColor();
	v8::Local<v8::Object> node = v8::Object::New(isolate);
	node->Set(context, v8::String::NewFromUtf8(isolate, "red", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, zn_color.red));
	node->Set(context, v8::String::NewFromUtf8(isolate, "green", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, zn_color.green));
	node->Set(context, v8::String::NewFromUtf8(isolate, "blue", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, zn_color.blue));
	args.GetReturnValue().Set(node);
}
void ZoomNodeSettingVirtualBGCtrlWrap::BeginSelectReplaceVBColor(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetSettingServiceWrap().GetSettingVirtualBGCtrl().BeginSelectReplaceVBColor();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeSettingVirtualBGCtrlWrap::AddBGVideo(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::AddBGVideoParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::AddBGVideoParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_filepath())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		ZoomSTRING _zn_file_path = s2zs(proto_params.filepath());
		err = _g_native_wrap.GetSettingServiceWrap().GetSettingVirtualBGCtrl().AddBGVideo(_zn_file_path);
	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeSettingVirtualBGCtrlWrap::RemoveBGVideo(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::RemoveBGVideoParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::RemoveBGVideoParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_imagehandle())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		int64_t _imageHandle = proto_params.imagehandle();
		err = _g_native_wrap.GetSettingServiceWrap().GetSettingVirtualBGCtrl().RemoveBGVideo(_imageHandle);
	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeSettingVirtualBGCtrlWrap::GetBGVideoList(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	ZNList<ZNVirtualBGImageInfo> zn_list = _g_native_wrap.GetSettingServiceWrap().GetSettingVirtualBGCtrl().GetBGVideoList();
	v8::Local<v8::Array> nodes = NodeWrapUtil::CreateVirtualBGImageInfoObjectList(isolate, zn_list);
	args.GetReturnValue().Set(nodes);
}
void ZoomNodeSettingVirtualBGCtrlWrap::UseBGVideo(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::UseBGVideoParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::UseBGVideoParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_imagehandle())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		int64_t _imageHandle = proto_params.imagehandle();
		err = _g_native_wrap.GetSettingServiceWrap().GetSettingVirtualBGCtrl().UseBGVideo(_imageHandle);
	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}

// Event Callbacks
void ZoomNodeSettingVirtualBGCtrlWrap::SetOnVBImageDidDownloadedCB(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		if (args.Length() < 1)
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (args[0]->IsNull())
		{
			ZoomNodeSinkHelper::GetInst().onVBImageDidDownloaded.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
        
		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onVBImageDidDownloaded.Reset(isolate, cbfunc);
	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeSettingVirtualBGCtrlWrap::SetOnGreenVBDidUpdateWithReplaceColorCB(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		if (args.Length() < 1)
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (args[0]->IsNull())
		{
			ZoomNodeSinkHelper::GetInst().onGreenVBDidUpdateWithReplaceColor.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
        
		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onGreenVBDidUpdateWithReplaceColor.Reset(isolate, cbfunc);
	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeSettingVirtualBGCtrlWrap::SetOnSelectedVBImageChangedCB(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		if (args.Length() < 1)
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (args[0]->IsNull())
		{
			ZoomNodeSinkHelper::GetInst().onSelectedVBImageChanged.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
        
		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onSelectedVBImageChanged.Reset(isolate, cbfunc);
	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeSettingVirtualBGCtrlWrap::SetOnVBVideoUploadedResultCB(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		if (args.Length() < 1)
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (args[0]->IsNull())
		{
			ZoomNodeSinkHelper::GetInst().onVBVideoUploadedResult.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
        
		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onVBVideoUploadedResult.Reset(isolate, cbfunc);
	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}