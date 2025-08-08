#include "zoom_node_meeting_whiteboard_ctrl.h"

ZoomNodeMeetingWhiteboardCtrlWrap::ZoomNodeMeetingWhiteboardCtrlWrap()
{
	_g_native_wrap.GetMeetingServiceWrap().GetMeetingWhiteboardCtrl().SetSink(&ZoomNodeSinkWrapMgr::GetInst().m_meetingWhiteboardWrapSink);
}

ZoomNodeMeetingWhiteboardCtrlWrap::~ZoomNodeMeetingWhiteboardCtrlWrap()
{
	_g_native_wrap.GetMeetingServiceWrap().GetMeetingWhiteboardCtrl().SetSink(NULL);
}
void ZoomNodeMeetingWhiteboardCtrlWrap::SetOwnerWindow(const v8::FunctionCallbackInfo<v8::Value>& args)
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

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingWhiteboardCtrl().SetOwnerWindow(_zn_windowID);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWhiteboardCtrlWrap::IsSupportWhiteBoard(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool zn_bIs = _g_native_wrap.GetMeetingServiceWrap().GetMeetingWhiteboardCtrl().IsSupportWhiteBoard();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, zn_bIs);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWhiteboardCtrlWrap::CanStartShareWhiteboard(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	v8::HandleScope scope(isolate);
	v8::Local<v8::Object> node = v8::Object::New(isolate);

	ZNCannotShareReasonType reason;
	bool bCan = _g_native_wrap.GetMeetingServiceWrap().GetMeetingWhiteboardCtrl().CanStartShareWhiteboard(reason);

	node->Set(context, v8::String::NewFromUtf8(isolate, "bCan", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, bCan));
	node->Set(context, v8::String::NewFromUtf8(isolate, "reason", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int32_t)reason));

	args.GetReturnValue().Set(node);
}
void ZoomNodeMeetingWhiteboardCtrlWrap::ShowDashboardView(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::ShowDashboardViewParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::ShowDashboardViewParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		int _x = -1;
		if (proto_params.has_x())
		{
			_x = proto_params.x();
		}

		int _y = -1;
		if (proto_params.has_y())
		{
			_y = proto_params.y();
		}

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingWhiteboardCtrl().ShowDashboardView(_x, _y);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWhiteboardCtrlWrap::SetDashboardViewSize(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::SetDashboardViewSizeParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::SetDashboardViewSizeParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		int _width = -1;
		if (proto_params.has_width())
		{
			_width = proto_params.width();
		}

		int _height = -1;
		if (proto_params.has_height())
		{
			_height = proto_params.height();
		}

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingWhiteboardCtrl().SetDashboardViewSize(_width, _height);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWhiteboardCtrlWrap::SetWhiteboardViewPos(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::SetWhiteboardViewPosParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::SetWhiteboardViewPosParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		int _x = -1;
		if (proto_params.has_x())
		{
			_x = proto_params.x();
		}

		int _y = -1;
		if (proto_params.has_y())
		{
			_y = proto_params.y();
		}

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingWhiteboardCtrl().SetWhiteboardViewPos(_x, _y);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWhiteboardCtrlWrap::SetWhiteboardViewSize(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::SetWhiteboardViewSizeParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::SetWhiteboardViewSizeParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		int _width = -1;
		if (proto_params.has_width())
		{
			_width = proto_params.width();
		}

		int _height = -1;
		if (proto_params.has_height())
		{
			_height = proto_params.height();
		}

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingWhiteboardCtrl().SetWhiteboardViewSize(_width, _height);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWhiteboardCtrlWrap::SetWhiteboardShareOption(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::SetWhiteboardShareOptionParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::SetWhiteboardShareOptionParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_option())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		ZNSDKWhiteboardShareOption zn_option = (ZNSDKWhiteboardShareOption)proto_params.option();

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingWhiteboardCtrl().SetWhiteboardShareOption(zn_option);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWhiteboardCtrlWrap::GetWhiteboardShareOption(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	v8::HandleScope scope(isolate);
	v8::Local<v8::Object> node = v8::Object::New(isolate);

	ZNSDKWhiteboardShareOption option;
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingWhiteboardCtrl().GetWhiteboardShareOption(option);

	node->Set(context, v8::String::NewFromUtf8(isolate, "err", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int32_t)err));
	node->Set(context, v8::String::NewFromUtf8(isolate, "option", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int32_t)option));

	args.GetReturnValue().Set(node);
}
void ZoomNodeMeetingWhiteboardCtrlWrap::SetWhiteboardCreateOption(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::SetWhiteboardCreateOptionParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::SetWhiteboardCreateOptionParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_option())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		ZNSDKWhiteboardCreateOption zn_option = (ZNSDKWhiteboardCreateOption)proto_params.option();

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingWhiteboardCtrl().SetWhiteboardCreateOption(zn_option);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWhiteboardCtrlWrap::GetWhiteboardCreateOption(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	v8::HandleScope scope(isolate);
	v8::Local<v8::Object> node = v8::Object::New(isolate);

	ZNSDKWhiteboardCreateOption option;
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingWhiteboardCtrl().GetWhiteboardCreateOption(option);

	node->Set(context, v8::String::NewFromUtf8(isolate, "err", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int32_t)err));
	node->Set(context, v8::String::NewFromUtf8(isolate, "option", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int32_t)option));

	args.GetReturnValue().Set(node);
}
void ZoomNodeMeetingWhiteboardCtrlWrap::EnableParticipantsCreateWithoutHost(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::EnableParticipantsCreateWithoutHostParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::EnableParticipantsCreateWithoutHostParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_benable())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		bool _b_enable = false;
		convertBool(proto_params.benable(), _b_enable);

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingWhiteboardCtrl().EnableParticipantsCreateWithoutHost(_b_enable);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWhiteboardCtrlWrap::IsParticipantsCreateWithoutHostEnabled(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool zn_bIs = _g_native_wrap.GetMeetingServiceWrap().GetMeetingWhiteboardCtrl().IsParticipantsCreateWithoutHostEnabled();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, zn_bIs);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWhiteboardCtrlWrap::IsOtherSharingWhiteboard(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool zn_bIs = _g_native_wrap.GetMeetingServiceWrap().GetMeetingWhiteboardCtrl().IsOtherSharingWhiteboard();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, zn_bIs);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWhiteboardCtrlWrap::IsSharingWhiteboardOut(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool zn_bIs = _g_native_wrap.GetMeetingServiceWrap().GetMeetingWhiteboardCtrl().IsSharingWhiteboardOut();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, zn_bIs);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWhiteboardCtrlWrap::SetOnWhiteboardStatusChangedCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onWhiteboardStatusChanged.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onWhiteboardStatusChanged.Reset(isolate, cbfunc);

	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWhiteboardCtrlWrap::SetOnWhiteboardSettingsChangedCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onWhiteboardSettingsChanged.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onWhiteboardSettingsChanged.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}