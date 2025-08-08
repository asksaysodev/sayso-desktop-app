#include "zoom_node_meeting_reminder_ctrl.h"

ZoomNodeMeetingReminderCtrlWrap::ZoomNodeMeetingReminderCtrlWrap()
{
	_g_native_wrap.GetMeetingServiceWrap().GetMeetingReminderCtrl().SetSink(&ZoomNodeSinkWrapMgr::GetInst().m_meetingReminderWrapSink);
}

ZoomNodeMeetingReminderCtrlWrap::~ZoomNodeMeetingReminderCtrlWrap()
{
	_g_native_wrap.GetMeetingServiceWrap().GetMeetingReminderCtrl().SetSink(NULL);
}
void ZoomNodeMeetingReminderCtrlWrap::Ignore(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingReminderCtrl().Ignore();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingReminderCtrlWrap::Accept(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingReminderCtrl().Accept();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingReminderCtrlWrap::Decline(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingReminderCtrl().Decline();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingReminderCtrlWrap::SetOnReminderNotifyCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onReminderNotify.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onReminderNotify.Reset(isolate, cbfunc);

	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodeMeetingReminderCtrlWrap::SetEnableOption(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::SetEnableOptionParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::SetEnableOptionParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_option())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		ZNFeatureEnableOption _zn_option = (ZNFeatureEnableOption)proto_params.option();

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingReminderCtrl().SetEnableOption(_zn_option);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingReminderCtrlWrap::StartEnable(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingReminderCtrl().StartEnable();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingReminderCtrlWrap::DeclineEnable(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::DeclineEnableParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::DeclineEnableParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_bdeclineall())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		bool _bDeclineAll;
		convertBool(proto_params.bdeclineall(), _bDeclineAll);

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingReminderCtrl().DeclineEnable(_bDeclineAll);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingReminderCtrlWrap::IgnoreEnable(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingReminderCtrl().IgnoreEnable();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingReminderCtrlWrap::SetOnEnableReminderNotifyCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onEnableReminderNotify.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onEnableReminderNotify.Reset(isolate, cbfunc);

	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}