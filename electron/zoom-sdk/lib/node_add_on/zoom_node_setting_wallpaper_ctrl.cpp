#include "zoom_node_setting_wallpaper_ctrl.h"
#include "zoom_node_wrap_util.h"

ZoomNodeSettingWallpaperCtrlWrap::ZoomNodeSettingWallpaperCtrlWrap()
{
	_g_native_wrap.GetSettingServiceWrap().GetSettingWallpaperCtrl().SetSink(&ZoomNodeSinkWrapMgr::GetInst().m_wallpaperSettingContextWrapSink);
}

ZoomNodeSettingWallpaperCtrlWrap::~ZoomNodeSettingWallpaperCtrlWrap()
{
	_g_native_wrap.GetSettingServiceWrap().GetSettingWallpaperCtrl().SetSink(NULL);
}

void ZoomNodeSettingWallpaperCtrlWrap::SetOnMeetingWallpaperChangedCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onMeetingWallpaperChanged.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onMeetingWallpaperChanged.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeSettingWallpaperCtrlWrap::SetOnMeetingWallpaperImageDownloadStatusCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onMeetingWallpaperImageDownloadStatus.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onMeetingWallpaperImageDownloadStatus.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeSettingWallpaperCtrlWrap::SetOnPersonalWallpaperChangedCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onPersonalWallpaperChanged.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onPersonalWallpaperChanged.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeSettingWallpaperCtrlWrap::SetOnPersonalWallpaperImageDownloadStatusCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onPersonalWallpaperImageDownloadStatus.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onPersonalWallpaperImageDownloadStatus.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeSettingWallpaperCtrlWrap::IsMeetingWallpaperEnabled(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool bIs = _g_native_wrap.GetSettingServiceWrap().GetSettingWallpaperCtrl().IsMeetingWallpaperEnabled();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, bIs);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeSettingWallpaperCtrlWrap::IsMeetingWallpaperThumbsReady(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool bIs = _g_native_wrap.GetSettingServiceWrap().GetSettingWallpaperCtrl().IsMeetingWallpaperThumbsReady();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, bIs);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeSettingWallpaperCtrlWrap::GetCurrentMeetingWallpaperItem(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	v8::HandleScope scope(isolate);
	ZNWallpaperItem zn_item = _g_native_wrap.GetSettingServiceWrap().GetSettingWallpaperCtrl().GetCurrentMeetingWallpaperItem();
	v8::Local<v8::Object> node = NodeWrapUtil::CreateWallpaperItemObject(isolate, zn_item);
	args.GetReturnValue().Set(node);
}
void ZoomNodeSettingWallpaperCtrlWrap::GetMeetingWallpaperList(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();
	ZNList<ZNWallpaperItem> zn_list = _g_native_wrap.GetSettingServiceWrap().GetSettingWallpaperCtrl().GetMeetingWallpaperList();

	v8::Local<v8::Array> nodes = v8::Array::New(isolate);
	for (unsigned int i = 0; i < zn_list.size(); ++i) {
		v8::Local<v8::Object> node = NodeWrapUtil::CreateWallpaperItemObject(isolate, zn_list[i]);
		nodes->Set(context, i, node);
	}
	args.GetReturnValue().Set(nodes);
}
void ZoomNodeSettingWallpaperCtrlWrap::SetMeetingWallpaper(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::SetWallpaperParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::SetWallpaperParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_wallpaperid() ||
			!proto_params.has_transparency() ||
			!proto_params.has_mode()
			)
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		ZoomSTRING _zn_wallpaperId = s2zs(proto_params.wallpaperid());
		int _zn_transparency = proto_params.transparency();
		ZNZoomSDKWallpaperLayoutMode _zn_mode = (ZNZoomSDKWallpaperLayoutMode)proto_params.mode();

		err = _g_native_wrap.GetSettingServiceWrap().GetSettingWallpaperCtrl().SetMeetingWallpaper(_zn_wallpaperId, _zn_transparency, _zn_mode);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeSettingWallpaperCtrlWrap::GetMeetingWallpaperItemByID(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	v8::HandleScope scope(isolate);
	com::electron::sdk::proto::GetWallpaperItemByIDParams proto_params;
	if (!SetProtoParam<com::electron::sdk::proto::GetWallpaperItemByIDParams >(args, proto_params))
	{
		return;
	}
	if (!proto_params.has_wallpaperid())
	{
		return;
	}
	ZoomSTRING _zn_wallpaperId = s2zs(proto_params.wallpaperid());
	ZNWallpaperItem zn_item = _g_native_wrap.GetSettingServiceWrap().GetSettingWallpaperCtrl().GetMeetingWallpaperItemByID(_zn_wallpaperId);
	v8::Local<v8::Object> node = NodeWrapUtil::CreateWallpaperItemObject(isolate, zn_item);

	args.GetReturnValue().Set(node);
}
void ZoomNodeSettingWallpaperCtrlWrap::IsPersonalWallpaperEnabled(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool bIs = _g_native_wrap.GetSettingServiceWrap().GetSettingWallpaperCtrl().IsPersonalWallpaperEnabled();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, bIs);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeSettingWallpaperCtrlWrap::GetCurrentPersonalWallpaperItem(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	v8::HandleScope scope(isolate);
	ZNWallpaperItem zn_item = _g_native_wrap.GetSettingServiceWrap().GetSettingWallpaperCtrl().GetCurrentPersonalWallpaperItem();
	v8::Local<v8::Object> node = NodeWrapUtil::CreateWallpaperItemObject(isolate, zn_item);
	args.GetReturnValue().Set(node);
}
void ZoomNodeSettingWallpaperCtrlWrap::GetPersonalWallpaperList(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();
	ZNList<ZNWallpaperItem> zn_list = _g_native_wrap.GetSettingServiceWrap().GetSettingWallpaperCtrl().GetPersonalWallpaperList();

	v8::Local<v8::Array> nodes = v8::Array::New(isolate);
	for (unsigned int i = 0; i < zn_list.size(); ++i) {
		v8::Local<v8::Object> node = NodeWrapUtil::CreateWallpaperItemObject(isolate, zn_list[i]);
		nodes->Set(context, i, node);
	}
	args.GetReturnValue().Set(nodes);
}
void ZoomNodeSettingWallpaperCtrlWrap::SetPersonalWallpaper(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::SetWallpaperParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::SetWallpaperParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_wallpaperid() ||
			!proto_params.has_transparency() ||
			!proto_params.has_mode()
			)
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		ZoomSTRING _zn_wallpaperId = s2zs(proto_params.wallpaperid());
		int _zn_transparency = proto_params.transparency();
		ZNZoomSDKWallpaperLayoutMode _zn_mode = (ZNZoomSDKWallpaperLayoutMode)proto_params.mode();

		err = _g_native_wrap.GetSettingServiceWrap().GetSettingWallpaperCtrl().SetPersonalWallpaper(_zn_wallpaperId, _zn_transparency, _zn_mode);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeSettingWallpaperCtrlWrap::GetPersonalWallpaperItemByID(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	v8::HandleScope scope(isolate);
	com::electron::sdk::proto::GetWallpaperItemByIDParams proto_params;
	if (!SetProtoParam<com::electron::sdk::proto::GetWallpaperItemByIDParams >(args, proto_params))
	{
		return;
	}
	if (!proto_params.has_wallpaperid())
	{
		return;
	}
	ZoomSTRING _zn_wallpaperId = s2zs(proto_params.wallpaperid());
	ZNWallpaperItem zn_item = _g_native_wrap.GetSettingServiceWrap().GetSettingWallpaperCtrl().GetPersonalWallpaperItemByID(_zn_wallpaperId);
	v8::Local<v8::Object> node = NodeWrapUtil::CreateWallpaperItemObject(isolate, zn_item);

	args.GetReturnValue().Set(node);
}