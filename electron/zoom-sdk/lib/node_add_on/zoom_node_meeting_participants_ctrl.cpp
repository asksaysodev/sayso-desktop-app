#include "zoom_node_meeting_participants_ctrl.h"
#include "zoom_node_wrap_util.h"

ZoomNodeMeetingParticipantsCtrlWrap::ZoomNodeMeetingParticipantsCtrlWrap()
{
	_g_native_wrap.GetMeetingServiceWrap().GetMeetingParticipantsCtrl().SetSink(&ZoomNodeSinkWrapMgr::GetInst().m_meetingParticipantsWrapSink);
}

ZoomNodeMeetingParticipantsCtrlWrap::~ZoomNodeMeetingParticipantsCtrlWrap()
{

}
void ZoomNodeMeetingParticipantsCtrlWrap::SetMeetingUserJoinCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onUserJoin.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onUserJoin.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingParticipantsCtrlWrap::SetMeetingUserLeftCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onUserLeft.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onUserLeft.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingParticipantsCtrlWrap::SetMeetingHostChangeCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onHostChangeNotification.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onHostChangeNotification.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingParticipantsCtrlWrap::SetOnAllowParticipantsRequestCloudRecordingCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onAllowParticipantsRequestCloudRecording.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onAllowParticipantsRequestCloudRecording.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingParticipantsCtrlWrap::SetOnBotAuthorizerRelationChangedCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onBotAuthorizerRelationChanged.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onBotAuthorizerRelationChanged.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingParticipantsCtrlWrap::GetParticipantsList(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	ZNList<unsigned int> zn_userID_lst;
	zn_userID_lst = _g_native_wrap.GetMeetingServiceWrap().GetMeetingParticipantsCtrl().GetParticipantsList();

	v8::Local<v8::Array> nodes = v8::Array::New(isolate);
	for (unsigned int i = 0; i < zn_userID_lst.size(); ++i) {
		v8::HandleScope scope(isolate);
		v8::Local<v8::Object> node = v8::Object::New(isolate);
		node->Set(context, v8::String::NewFromUtf8(isolate, "userid", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int32_t)zn_userID_lst[i]));
		nodes->Set(context, i, node);
	}
	args.GetReturnValue().Set(nodes);
}
void ZoomNodeMeetingParticipantsCtrlWrap::GetUserInfoByUserID(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	com::electron::sdk::proto::GetUserInfoByUserIDParams proto_params;
	if (!SetProtoParam<com::electron::sdk::proto::GetUserInfoByUserIDParams >(args, proto_params))
	{
		return;
	}
	if (!proto_params.has_userid())
	{
		isolate->ThrowException(v8::Exception::TypeError(
			v8::String::NewFromUtf8(isolate, "Wrong proto params", v8::NewStringType::kInternalized).ToLocalChecked()));
		return;
	}
	unsigned int _zn_userID = proto_params.userid();

	ZNUserInfomation zn_userInfo = _g_native_wrap.GetMeetingServiceWrap().GetMeetingParticipantsCtrl().GetUserInfomationByUserID(_zn_userID);
	v8::Local<v8::Object> node = NodeWrapUtil::CreateUserInfoObject(isolate, zn_userInfo);

	args.GetReturnValue().Set(node);
}
void ZoomNodeMeetingParticipantsCtrlWrap::GetBotAuthorizedUserInfoByUserID(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	v8::Local<v8::Object> node = v8::Object::New(isolate);
	do
	{
		com::electron::sdk::proto::GetBotAuthorizedUserInfoByUserIDParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::GetBotAuthorizedUserInfoByUserIDParams >(args, proto_params))
		{
			break;
		}
		if (!proto_params.has_userid())
		{
			break;
		}
		int _userid = proto_params.userid();
		ZNUserInfomation zn_userInfo = _g_native_wrap.GetMeetingServiceWrap().GetMeetingParticipantsCtrl().GetBotAuthorizedUserInfoByUserID(_userid);
		node = NodeWrapUtil::CreateUserInfoObject(isolate, zn_userInfo);
	} while (false);

	args.GetReturnValue().Set(node);
}
void ZoomNodeMeetingParticipantsCtrlWrap::GetAuthorizedBotListByUserID(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	v8::HandleScope scope(isolate);
	v8::Local<v8::Array> nodes = v8::Array::New(isolate);
	do
	{
		com::electron::sdk::proto::GetAuthorizedBotListByUserIDParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::GetAuthorizedBotListByUserIDParams >(args, proto_params))
		{
			break;
		}
		if (!proto_params.has_userid())
		{
			break;
		}
		int _userid = proto_params.userid();
		ZNList<unsigned int> zn_list = _g_native_wrap.GetMeetingServiceWrap().GetMeetingParticipantsCtrl().GetAuthorizedBotListByUserID(_userid);

		for (unsigned int i = 0; i < zn_list.size(); ++i) {
			nodes->Set(context, i, v8::Integer::New(isolate, (int32_t)zn_list[i]));
		}

	} while (false);

	args.GetReturnValue().Set(nodes);
}
void ZoomNodeMeetingParticipantsCtrlWrap::CanEnableParticipantRequestCloudRecording(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool zn_bCan = _g_native_wrap.GetMeetingServiceWrap().GetMeetingParticipantsCtrl().CanEnableParticipantRequestCloudRecording();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, zn_bCan);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingParticipantsCtrlWrap::IsParticipantRequestCloudRecordingAllowed(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool zn_bIs = _g_native_wrap.GetMeetingServiceWrap().GetMeetingParticipantsCtrl().IsParticipantRequestCloudRecordingAllowed();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, zn_bIs);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingParticipantsCtrlWrap::AllowParticipantsToRequestCloudRecording(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::AllowParticipantsToRequestCloudRecordingParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::AllowParticipantsToRequestCloudRecordingParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_ballow())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		bool _bAllow;
		convertBool(proto_params.ballow(), _bAllow);

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingParticipantsCtrl().AllowParticipantsToRequestCloudRecording(_bAllow);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}