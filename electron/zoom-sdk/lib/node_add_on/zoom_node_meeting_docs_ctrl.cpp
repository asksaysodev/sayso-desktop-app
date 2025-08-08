#include "zoom_node_meeting_docs_ctrl.h"
#include "zoom_node_wrap_util.h"

ZoomNodeMeetingDocsCtrlWrap::ZoomNodeMeetingDocsCtrlWrap()
{
	_g_native_wrap.GetMeetingServiceWrap().GetMeetingDocsCtrl().SetSink(&ZoomNodeSinkWrapMgr::GetInst().m_meetingDocsWrapSink);
}

ZoomNodeMeetingDocsCtrlWrap::~ZoomNodeMeetingDocsCtrlWrap()
{
	_g_native_wrap.GetMeetingServiceWrap().GetMeetingDocsCtrl().SetSink(NULL);
}
void ZoomNodeMeetingDocsCtrlWrap::IsSupportDocs(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool bIs = _g_native_wrap.GetMeetingServiceWrap().GetMeetingDocsCtrl().IsSupportDocs();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, bIs);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingDocsCtrlWrap::CanStartShareDocs(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	v8::HandleScope scope(isolate);

	ZNCannotShareReasonType zn_reason;
	bool bCan = _g_native_wrap.GetMeetingServiceWrap().GetMeetingDocsCtrl().CanStartShareDocs(zn_reason);

	v8::Local<v8::Object> node = v8::Object::New(isolate);
	node->Set(context, v8::String::NewFromUtf8(isolate, "bCan", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, bCan));
	node->Set(context, v8::String::NewFromUtf8(isolate, "reason", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int32_t)zn_reason));
	args.GetReturnValue().Set(node);
}
void ZoomNodeMeetingDocsCtrlWrap::CanSetDocsOption(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool bCan = _g_native_wrap.GetMeetingServiceWrap().GetMeetingDocsCtrl().CanSetDocsOption();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, bCan);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingDocsCtrlWrap::SetDocsShareOption(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::SetDocsShareOptionParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::SetDocsShareOptionParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_option())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		ZNSDKDocsShareOption zn_option = (ZNSDKDocsShareOption)proto_params.option();
		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingDocsCtrl().SetDocsShareOption(zn_option);
	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingDocsCtrlWrap::GetDocsShareOption(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	v8::HandleScope scope(isolate);

	ZNSDKDocsShareOption zn_option;
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingDocsCtrl().GetDocsShareOption(zn_option);

	v8::Local<v8::Object> node = v8::Object::New(isolate);
	node->Set(context, v8::String::NewFromUtf8(isolate, "err", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int32_t)err));
	node->Set(context, v8::String::NewFromUtf8(isolate, "option", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int32_t)zn_option));
	args.GetReturnValue().Set(node);
}
void ZoomNodeMeetingDocsCtrlWrap::SetDocsCreateOption(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::SetDocsCreateOptionParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::SetDocsCreateOptionParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_option())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		ZNSDKDocsCreateOption zn_option = (ZNSDKDocsCreateOption)proto_params.option();
		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingDocsCtrl().SetDocsCreateOption(zn_option);
	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingDocsCtrlWrap::GetDocsCreateOption(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	v8::HandleScope scope(isolate);

	ZNSDKDocsCreateOption zn_option;
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingDocsCtrl().GetDocsCreateOption(zn_option);

	v8::Local<v8::Object> node = v8::Object::New(isolate);
	node->Set(context, v8::String::NewFromUtf8(isolate, "err", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int32_t)err));
	node->Set(context, v8::String::NewFromUtf8(isolate, "option", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int32_t)zn_option));
	args.GetReturnValue().Set(node);
}
void ZoomNodeMeetingDocsCtrlWrap::IsOtherSharingDocs(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool bIs = _g_native_wrap.GetMeetingServiceWrap().GetMeetingDocsCtrl().IsOtherSharingDocs();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, bIs);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingDocsCtrlWrap::IsSharingDocsOut(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool bIs = _g_native_wrap.GetMeetingServiceWrap().GetMeetingDocsCtrl().IsSharingDocsOut();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, bIs);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingDocsCtrlWrap::SetOwnerWindow(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::SetOwnerWindowParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::SetOwnerWindowParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		ZoomSTRING _zn_windowID;
		if (proto_params.has_windowid())
		{
			_zn_windowID = s2zs(proto_params.windowid());
		}

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingDocsCtrl().SetOwnerWindow(_zn_windowID);
	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingDocsCtrlWrap::ShowDocsDashboard(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::ShowDocsDashboardParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::ShowDocsDashboardParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_x() || !proto_params.has_y())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		int _x = proto_params.x();
		int _y = proto_params.y();
		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingDocsCtrl().ShowDocsDashboard(_x, _y);
	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingDocsCtrlWrap::SetDocDashboardSize(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::SetDocDashboardSizeParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::SetDocDashboardSizeParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_width() || !proto_params.has_height())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		int _width = proto_params.width();
		int _height = proto_params.height();
		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingDocsCtrl().SetDocDashboardSize(_width, _height);
	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingDocsCtrlWrap::SetDocsViewPos(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::SetDocsViewPosParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::SetDocsViewPosParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_x() || !proto_params.has_y())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		int _x = proto_params.x();
		int _y = proto_params.y();
		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingDocsCtrl().SetDocsViewPos(_x, _y);
	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingDocsCtrlWrap::SetDocsViewSize(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::SetDocsViewSizeParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::SetDocsViewSizeParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_width() || !proto_params.has_height())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		int _width = proto_params.width();
		int _height = proto_params.height();
		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingDocsCtrl().SetDocsViewSize(_width, _height);
	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingDocsCtrlWrap::ShowDocsViewWindow(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::ShowDocsViewWindowParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::ShowDocsViewWindowParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_sourceid())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		int _sourceID = proto_params.sourceid();
		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingDocsCtrl().ShowDocsViewWindow(_sourceID);
	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingDocsCtrlWrap::CloseDocsViewWindow(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingDocsCtrl().CloseDocsViewWindow();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingDocsCtrlWrap::GetDocsSharingSourceInfoList(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	ZNList<ZNZoomSDKDocSharingSourceInfo> zn_list;	

	do
	{
		com::electron::sdk::proto::GetDocsSharingSourceInfoListParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::GetDocsSharingSourceInfoListParams >(args, proto_params))
		{
			break;
		}
		if (!proto_params.has_userid())
		{
			break;
		}

		int _userID = proto_params.userid();
		zn_list = _g_native_wrap.GetMeetingServiceWrap().GetMeetingDocsCtrl().GetDocsSharingSourceInfoList(_userID);
	} while (false);


	v8::Local<v8::Array> nodes = v8::Array::New(isolate, zn_list.size());
	for (unsigned int i = 0; i < zn_list.size(); ++i) {
		v8::Local<v8::Object> node = NodeWrapUtil::CreateZoomSDKDocSharingSourceInfoObject(isolate, zn_list[i]);
		nodes->Set(context, i, node);
	}
	args.GetReturnValue().Set(nodes);
}
void ZoomNodeMeetingDocsCtrlWrap::GetViewableDocsSharingUserList(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	ZNList<unsigned int> zn_list = _g_native_wrap.GetMeetingServiceWrap().GetMeetingDocsCtrl().GetViewableDocsSharingUserList();

	v8::Local<v8::Array> nodes = v8::Array::New(isolate, zn_list.size());
	for (unsigned int i = 0; i < zn_list.size(); ++i) {
		nodes->Set(context, i, v8::Integer::New(isolate, zn_list[i]));
	}
	args.GetReturnValue().Set(nodes);
}

// Event Callbacks
void ZoomNodeMeetingDocsCtrlWrap::SetOnDocsSharingSourceInfoChangedCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onDocsSharingSourceInfoChanged.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
        
		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onDocsSharingSourceInfoChanged.Reset(isolate, cbfunc);
	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingDocsCtrlWrap::SetOnDocsPermissionChangedCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onDocsPermissionChanged.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
        
		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onDocsPermissionChanged.Reset(isolate, cbfunc);
	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
