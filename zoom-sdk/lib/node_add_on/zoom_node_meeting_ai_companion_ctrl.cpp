#include "zoom_node_meeting_ai_companion_ctrl.h"

ZoomNodeMeetingAICompanionCtrlWrap::ZoomNodeMeetingAICompanionCtrlWrap()
{
	_g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().SetSink(&ZoomNodeSinkWrapMgr::GetInst().m_meetingAICompanionWrapSink);
}

ZoomNodeMeetingAICompanionCtrlWrap::~ZoomNodeMeetingAICompanionCtrlWrap()
{
	_g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().SetSink(NULL);
}
void ZoomNodeMeetingAICompanionCtrlWrap::IsTurnoffAllAICompanionsSupported(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool zn_bIs = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().IsTurnoffAllAICompanionsSupported();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, zn_bIs);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::IsTurnOnAllAICompanionsSupported(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool zn_bIs = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().IsTurnOnAllAICompanionsSupported();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, zn_bIs);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::CanTurnOffAllAICompanions(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool zn_bIs = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().CanTurnOffAllAICompanions();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, zn_bIs);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::CanTurnOnAllAICompanions(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool zn_bIs = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().CanTurnOnAllAICompanions();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, zn_bIs);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::TurnOffAllAICompanions(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::TurnOffAllAICompanionsParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::TurnOffAllAICompanionsParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_deleteassets())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		bool _b_deleteassets = false;
		convertBool(proto_params.deleteassets(), _b_deleteassets);

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().TurnOffAllAICompanions(_b_deleteassets);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::TurnOnAllAICompanions(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().TurnOnAllAICompanions();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::CanRequestTurnoffAllAICompanions(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool zn_bIs = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().CanRequestTurnoffAllAICompanions();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, zn_bIs);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::CanRequestTurnOnAllAICompanions(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool zn_bIs = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().CanRequestTurnOnAllAICompanions();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, zn_bIs);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::RequestTurnoffAllAICompanions(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().RequestTurnoffAllAICompanions();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::RequestTurnOnAllAICompanions(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().RequestTurnOnAllAICompanions();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::GetFeatureList(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	ZNList<ZNAICompanionFeature> zn_list = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().GetFeatureList();

	v8::Local<v8::Array> nodes = v8::Array::New(isolate);
	for (unsigned int i = 0; i < zn_list.size(); ++i) {
		nodes->Set(context, i, v8::Integer::New(isolate, (int32_t)zn_list[i]));
	}
	args.GetReturnValue().Set(nodes);
}
void ZoomNodeMeetingAICompanionCtrlWrap::GetAssetsDeletedFeatureList(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	ZNList<ZNAICompanionFeature> zn_list = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().GetAssetsDeletedFeatureList();

	v8::Local<v8::Array> nodes = v8::Array::New(isolate);
	for (unsigned int i = 0; i < zn_list.size(); ++i) {
		nodes->Set(context, i, v8::Integer::New(isolate, (int32_t)zn_list[i]));
	}
	args.GetReturnValue().Set(nodes);
}
void ZoomNodeMeetingAICompanionCtrlWrap::TurnOnAgain(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().TurnOnAgain();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::AgreeTurnOff(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().AgreeTurnOff();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::GetRequestUserID(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	unsigned int sender_id = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().GetRequestUserID();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (uint32_t)sender_id);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::IsTurnOn(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool zn_bIs = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().IsTurnOn();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, zn_bIs);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::Agree(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::AgreeAICompanionFeatureSwitchParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::AgreeAICompanionFeatureSwitchParams>(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_deleteassets())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		bool _b_deleteassets = false;
		convertBool(proto_params.deleteassets(), _b_deleteassets);

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().Agree(_b_deleteassets);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::Decline(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().Decline();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::SetOnAICompanionFeatureTurnOffByParticipantCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onAICompanionFeatureTurnOffByParticipant.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onAICompanionFeatureTurnOffByParticipant.Reset(isolate, cbfunc);

	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::SetOnAICompanionFeatureSwitchRequestedCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onAICompanionFeatureSwitchRequested.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onAICompanionFeatureSwitchRequested.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::SetOnAICompanionFeatureSwitchRequestResponseCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onAICompanionFeatureSwitchRequestResponse.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onAICompanionFeatureSwitchRequestResponse.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::SetOnAICompanionFeatureCanNotBeTurnedOffCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onAICompanionFeatureCanNotBeTurnedOff.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onAICompanionFeatureCanNotBeTurnedOff.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodeMeetingAICompanionCtrlWrap::EnableSmartSummary(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().EnableSmartSummary();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::IsForRequestEnableSmartSummary(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool zn_bIs = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().IsForRequestEnableSmartSummary();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, zn_bIs);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::StartSmartSummary(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().StartSmartSummary();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::IsForRequestStartSmartSummary(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool zn_bIs = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().IsForRequestStartSmartSummary();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, zn_bIs);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::StopSmartSummary(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().StopSmartSummary();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::GetSenderEnableSmartSummaryUserID(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	unsigned int sender_id = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().GetSenderEnableSmartSummaryUserID();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (uint32_t)sender_id);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::ContinueApproveEnableSmartSummary(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().ContinueApproveEnableSmartSummary();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::GetSenderStartSmartSummaryUserID(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	unsigned int sender_id = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().GetSenderStartSmartSummaryUserID();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (uint32_t)sender_id);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::ApproveStartSmartSummary(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().ApproveStartSmartSummary();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::DeclineStartSmartSummary(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().DeclineStartSmartSummary();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::GetEnableSmartSummaryActionTipTitle(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZoomSTRING requester_name = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().GetEnableSmartSummaryActionTipTitle();
	v8::Local<v8::String> bret = v8::String::NewFromUtf8(isolate, zs2s(requester_name).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::GetEnableSmartSummaryActionTipString(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZoomSTRING requester_name = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().GetEnableSmartSummaryActionTipString();
	v8::Local<v8::String> bret = v8::String::NewFromUtf8(isolate, zs2s(requester_name).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::ConfirmEnableSmartSummaryAction(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().ConfirmEnableSmartSummaryAction();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::CancelEnableSmartSummaryAction(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().CancelEnableSmartSummaryAction();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::SetOnSmartSummaryStateNotSupportedCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onSmartSummaryStateNotSupported.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onSmartSummaryStateNotSupported.Reset(isolate, cbfunc);

	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::SetOnSmartSummaryStateSupportedButDisabledCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onSmartSummaryStateSupportedButDisabled.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onSmartSummaryStateSupportedButDisabled.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::SetOnSmartSummaryStateEnabledButNotStartedCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onSmartSummaryStateEnabledButNotStarted.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onSmartSummaryStateEnabledButNotStarted.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::SetOnSmartSummaryStateStartedCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onSmartSummaryStateStarted.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onSmartSummaryStateStarted.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::SetOnFailedToStartSmartSummaryCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onFailedToStartSmartSummary.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onFailedToStartSmartSummary.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::SetOnSmartSummaryEnableRequestReceivedCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onSmartSummaryEnableRequestReceived.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onSmartSummaryEnableRequestReceived.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::SetOnSmartSummaryStartRequestReceivedCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onSmartSummaryStartRequestReceived.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onSmartSummaryStartRequestReceived.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::SetOnSmartSummaryEnableActionCallbackCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onSmartSummaryEnableActionCallback.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onSmartSummaryEnableActionCallback.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodeMeetingAICompanionCtrlWrap::CanChangeQuerySetting(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	bool zn_bCan = false;
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().CanChangeQuerySetting(zn_bCan);
	v8::HandleScope scope(isolate);
	v8::Local<v8::Object> node = v8::Object::New(isolate);
	node->Set(context, v8::String::NewFromUtf8(isolate, "err", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int32_t)err));
	node->Set(context, v8::String::NewFromUtf8(isolate, "bCan", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_bCan));

	args.GetReturnValue().Set(node);
}
void ZoomNodeMeetingAICompanionCtrlWrap::ChangeQuerySettings(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::ChangeQuerySettingsParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::ChangeQuerySettingsParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_setting())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		ZNMeetingAICompanionQuerySettingOptions zn_setting = (ZNMeetingAICompanionQuerySettingOptions)proto_params.setting();

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().ChangeQuerySettings(zn_setting);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::GetSelectedQuerySetting(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	int setting = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().GetSelectedQuerySetting();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)setting);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::IsAICompanionQueryLegalNoticeAvailable(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	bool zn_bAvailable = false;
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().IsAICompanionQueryLegalNoticeAvailable(zn_bAvailable);
	v8::HandleScope scope(isolate);
	v8::Local<v8::Object> node = v8::Object::New(isolate);
	node->Set(context, v8::String::NewFromUtf8(isolate, "err", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int32_t)err));
	node->Set(context, v8::String::NewFromUtf8(isolate, "bAvailable", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_bAvailable));

	args.GetReturnValue().Set(node);
}
void ZoomNodeMeetingAICompanionCtrlWrap::GetAICompanionQueryLegalNoticesPrompt(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZoomSTRING notices_prompt = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().GetAICompanionQueryLegalNoticesPrompt();
	v8::Local<v8::String> bret = v8::String::NewFromUtf8(isolate, zs2s(notices_prompt).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::GetAICompanionQueryLegalNoticesExplained(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZoomSTRING notices_explained = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().GetAICompanionQueryLegalNoticesExplained();
	v8::Local<v8::String> bret = v8::String::NewFromUtf8(isolate, zs2s(notices_explained).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::IsAICompanionQueryPrivacyLegalNoticeAvailable(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	bool zn_bAvailable = false;
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().IsAICompanionQueryPrivacyLegalNoticeAvailable(zn_bAvailable);
	v8::HandleScope scope(isolate);
	v8::Local<v8::Object> node = v8::Object::New(isolate);
	node->Set(context, v8::String::NewFromUtf8(isolate, "err", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int32_t)err));
	node->Set(context, v8::String::NewFromUtf8(isolate, "bAvailable", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_bAvailable));

	args.GetReturnValue().Set(node);
}
void ZoomNodeMeetingAICompanionCtrlWrap::GetAICompanionQueryPrivacyLegalNoticesPrompt(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZoomSTRING notices_prompt = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().GetAICompanionQueryPrivacyLegalNoticesPrompt();
	v8::Local<v8::String> bret = v8::String::NewFromUtf8(isolate, zs2s(notices_prompt).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
	args.GetReturnValue().Set(bret);
}

void ZoomNodeMeetingAICompanionCtrlWrap::EnableQuery(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().EnableQuery();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::IsForRequestEnableQuery(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool zn_bIs = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().IsForRequestEnableQuery();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, zn_bIs);
	args.GetReturnValue().Set(bret);
}

void ZoomNodeMeetingAICompanionCtrlWrap::StartMeetingQuery(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().StartMeetingQuery();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::IsForRequestStartMeetingQuery(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool zn_bIs = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().IsForRequestStartMeetingQuery();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, zn_bIs);
	args.GetReturnValue().Set(bret);
}

void ZoomNodeMeetingAICompanionCtrlWrap::GetDefaultQueryQuestions(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();
	ZNList<ZoomSTRING> zn_list = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().GetDefaultQueryQuestions();

	v8::Local<v8::Array> nodes = v8::Array::New(isolate);
	for (unsigned int i = 0; i < zn_list.size(); ++i) {
		nodes->Set(context, i, v8::String::NewFromUtf8(isolate, zs2s(zn_list[i]).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
	}
	args.GetReturnValue().Set(nodes);
}
void ZoomNodeMeetingAICompanionCtrlWrap::SendQueryQuestion(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::SendQueryQuestionParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::SendQueryQuestionParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_question())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		ZoomSTRING _zn_question = s2zs(proto_params.question());

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().SendQueryQuestion(_zn_question);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::StopMeetingQuery(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().StopMeetingQuery();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::CanSendQuery(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool zn_bCan = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().CanSendQuery();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, zn_bCan);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::RequestSendQueryPrivilege(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().RequestSendQueryPrivilege();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::GetSenderEnableQueryUserID(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	unsigned int userId = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().GetSenderEnableQueryUserID();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (uint32_t)userId);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::ContinueApproveEnableQuery(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().ContinueApproveEnableQuery();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::GetSenderStartQueryUserID(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	unsigned int userId = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().GetSenderStartQueryUserID();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (uint32_t)userId);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::ApproveStartQuery(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().ApproveStartQuery();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::DeclineStartQuery(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().DeclineStartQuery();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::AICompanionQueryFeedback(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::AICompanionQueryFeedbackParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::AICompanionQueryFeedbackParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_feedbacktype())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		ZNMeetingAICompanionQueryFeedbackType zn_feedbackType = (ZNMeetingAICompanionQueryFeedbackType)proto_params.feedbacktype();

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().AICompanionQueryFeedback(zn_feedbackType);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodeMeetingAICompanionCtrlWrap::GetEnableQueryActionTipTitle(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZoomSTRING tip_title = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().GetEnableQueryActionTipTitle();
	v8::Local<v8::String> bret = v8::String::NewFromUtf8(isolate, zs2s(tip_title).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::GetEnableQueryActionTipString(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZoomSTRING tip_string = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().GetEnableQueryActionTipString();
	v8::Local<v8::String> bret = v8::String::NewFromUtf8(isolate, zs2s(tip_string).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::ConfirmEnableQueryAction(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().ConfirmEnableQueryAction();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::CancelEnableQueryAction(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().CancelEnableQueryAction();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::GetSenderQueryUserID(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	unsigned int userId = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().GetSenderQueryUserID();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (uint32_t)userId);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::ApproveSendQuery(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().ApproveSendQuery();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::DeclineSendQuery(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::DeclineSendQueryParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::DeclineSendQueryParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_bdeclineall())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		bool _b_declineall = false;
		convertBool(proto_params.bdeclineall(), _b_declineall);

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().DeclineSendQuery(_b_declineall);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::SetOnQueryStateNotSupportedCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
            ZoomNodeSinkHelper::GetInst().onQueryStateNotSupported.Clear();
            break;
        }
        if (!args[0]->IsFunction())
        {
            err = ZNSDKERR_INVALID_PARAMETER;
            break;
        }

        v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
        ZoomNodeSinkHelper::GetInst().onQueryStateNotSupported.Reset(isolate, cbfunc);
    } while (false);
    
    v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
    args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::SetOnQueryStateSupportedButDisabledCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
            ZoomNodeSinkHelper::GetInst().onQueryStateSupportedButDisabled.Clear();
            break;
        }
        if (!args[0]->IsFunction())
        {
            err = ZNSDKERR_INVALID_PARAMETER;
            break;
        }

        v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
        ZoomNodeSinkHelper::GetInst().onQueryStateSupportedButDisabled.Reset(isolate, cbfunc);
    } while (false);
    
    v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
    args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::SetOnQueryStateEnabledButNotStartedCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
            ZoomNodeSinkHelper::GetInst().onQueryStateEnabledButNotStarted.Clear();
            break;
        }
        if (!args[0]->IsFunction())
        {
            err = ZNSDKERR_INVALID_PARAMETER;
            break;
        }

        v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
        ZoomNodeSinkHelper::GetInst().onQueryStateEnabledButNotStarted.Reset(isolate, cbfunc);
    } while (false);
    
    v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
    args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::SetOnQueryStateStartedCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
            ZoomNodeSinkHelper::GetInst().onQueryStateStarted.Clear();
            break;
        }
        if (!args[0]->IsFunction())
        {
            err = ZNSDKERR_INVALID_PARAMETER;
            break;
        }

        v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
        ZoomNodeSinkHelper::GetInst().onQueryStateStarted.Reset(isolate, cbfunc);
    } while (false);
    
    v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
    args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::SetOnQuerySettingChangedCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
            ZoomNodeSinkHelper::GetInst().onQuerySettingChanged.Clear();
            break;
        }
        if (!args[0]->IsFunction())
        {
            err = ZNSDKERR_INVALID_PARAMETER;
            break;
        }

        v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
        ZoomNodeSinkHelper::GetInst().onQuerySettingChanged.Reset(isolate, cbfunc);
    } while (false);
    
    v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
    args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::SetOnFailedToStartQueryCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
            ZoomNodeSinkHelper::GetInst().onFailedToStartQuery.Clear();
            break;
        }
        if (!args[0]->IsFunction())
        {
            err = ZNSDKERR_INVALID_PARAMETER;
            break;
        }

        v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
        ZoomNodeSinkHelper::GetInst().onFailedToStartQuery.Reset(isolate, cbfunc);
    } while (false);
    
    v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
    args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::SetOnReceiveRequestToEnableQueryCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
            ZoomNodeSinkHelper::GetInst().onReceiveRequestToEnableQuery.Clear();
            break;
        }
        if (!args[0]->IsFunction())
        {
            err = ZNSDKERR_INVALID_PARAMETER;
            break;
        }

        v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
        ZoomNodeSinkHelper::GetInst().onReceiveRequestToEnableQuery.Reset(isolate, cbfunc);
    } while (false);
    
    v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
    args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::SetOnReceiveRequestToStartQueryCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
            ZoomNodeSinkHelper::GetInst().onReceiveRequestToStartQuery.Clear();
            break;
        }
        if (!args[0]->IsFunction())
        {
            err = ZNSDKERR_INVALID_PARAMETER;
            break;
        }

        v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
        ZoomNodeSinkHelper::GetInst().onReceiveRequestToStartQuery.Reset(isolate, cbfunc);
    } while (false);
    
    v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
    args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::SetOnReceiveQueryAnswerCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
            ZoomNodeSinkHelper::GetInst().onReceiveQueryAnswer.Clear();
            break;
        }
        if (!args[0]->IsFunction())
        {
            err = ZNSDKERR_INVALID_PARAMETER;
            break;
        }

        v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
        ZoomNodeSinkHelper::GetInst().onReceiveQueryAnswer.Reset(isolate, cbfunc);
    } while (false);
    
    v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
    args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::SetOnQueryEnableActionCallback(const v8::FunctionCallbackInfo<v8::Value>& args)
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
            ZoomNodeSinkHelper::GetInst().onQueryEnableActionCallback.Clear();
            break;
        }
        if (!args[0]->IsFunction())
        {
            err = ZNSDKERR_INVALID_PARAMETER;
            break;
        }

        v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
        ZoomNodeSinkHelper::GetInst().onQueryEnableActionCallback.Reset(isolate, cbfunc);
    } while (false);
    
    v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
    args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::SetOnSendQueryPrivilegeChangedCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
            ZoomNodeSinkHelper::GetInst().onSendQueryPrivilegeChanged.Clear();
            break;
        }
        if (!args[0]->IsFunction())
        {
            err = ZNSDKERR_INVALID_PARAMETER;
            break;
        }

        v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
        ZoomNodeSinkHelper::GetInst().onSendQueryPrivilegeChanged.Reset(isolate, cbfunc);
    } while (false);
    
    v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
    args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::SetOnFailedToRequestSendQueryCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
            ZoomNodeSinkHelper::GetInst().onFailedToRequestSendQuery.Clear();
            break;
        }
        if (!args[0]->IsFunction())
        {
            err = ZNSDKERR_INVALID_PARAMETER;
            break;
        }

        v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
        ZoomNodeSinkHelper::GetInst().onFailedToRequestSendQuery.Reset(isolate, cbfunc);
    } while (false);
    
    v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
    args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingAICompanionCtrlWrap::SetOnReceiveRequestToSendQueryCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
            ZoomNodeSinkHelper::GetInst().onReceiveRequestToSendQuery.Clear();
            break;
        }
        if (!args[0]->IsFunction())
        {
            err = ZNSDKERR_INVALID_PARAMETER;
            break;
        }

        v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
        ZoomNodeSinkHelper::GetInst().onReceiveRequestToSendQuery.Reset(isolate, cbfunc);
    } while (false);
    
    v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
    args.GetReturnValue().Set(bret);
}