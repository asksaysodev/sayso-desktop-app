#include "zoom_node_meeting_webinar_ctrl.h"

ZoomNodeMeetingWebinarCtrlWrap::ZoomNodeMeetingWebinarCtrlWrap()
{
	_g_native_wrap.GetMeetingServiceWrap().GetMeetingWebinarCtrl().SetSink(&ZoomNodeSinkWrapMgr::GetInst().m_meetingWebinarWrapSink);
}

ZoomNodeMeetingWebinarCtrlWrap::~ZoomNodeMeetingWebinarCtrlWrap()
{
	_g_native_wrap.GetMeetingServiceWrap().GetMeetingWebinarCtrl().SetSink(NULL);
}
void ZoomNodeMeetingWebinarCtrlWrap::PromptAttendee2Panelist(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::WebinarAttendeeParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::WebinarAttendeeParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_userid())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		int _userid = proto_params.userid();

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingWebinarCtrl().PromptAttendee2Panelist(_userid);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWebinarCtrlWrap::DepromptPanelist2Attendee(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::WebinarAttendeeParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::WebinarAttendeeParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_userid())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		int _userid = proto_params.userid();

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingWebinarCtrl().DepromptPanelist2Attendee(_userid);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWebinarCtrlWrap::IsSupportAttendeeTalk(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool zn_bIs = _g_native_wrap.GetMeetingServiceWrap().GetMeetingWebinarCtrl().IsSupportAttendeeTalk();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, zn_bIs);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWebinarCtrlWrap::AllowAttendeeTalk(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::WebinarAttendeeParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::WebinarAttendeeParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_userid())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		int _userid = proto_params.userid();

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingWebinarCtrl().AllowAttendeeTalk(_userid);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWebinarCtrlWrap::DisallowAttendeeTalk(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::WebinarAttendeeParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::WebinarAttendeeParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_userid())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		int _userid = proto_params.userid();

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingWebinarCtrl().DisallowAttendeeTalk(_userid);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWebinarCtrlWrap::AllowPanelistStartVideo(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingWebinarCtrl().AllowPanelistStartVideo();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWebinarCtrlWrap::DisallowPanelistStartVideo(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingWebinarCtrl().DisallowPanelistStartVideo();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWebinarCtrlWrap::AllowWebinarEmojiReaction(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingWebinarCtrl().AllowWebinarEmojiReaction();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWebinarCtrlWrap::DisallowWebinarEmojiReaction(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingWebinarCtrl().DisallowWebinarEmojiReaction();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWebinarCtrlWrap::IsWebinarEmojiReactionSupported(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool zn_bIs = _g_native_wrap.GetMeetingServiceWrap().GetMeetingWebinarCtrl().IsWebinarEmojiReactionSupported();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, zn_bIs);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWebinarCtrlWrap::AllowAttendeeRaiseHand(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingWebinarCtrl().AllowAttendeeRaiseHand();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWebinarCtrlWrap::DisallowAttendeeRaiseHand(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingWebinarCtrl().DisallowAttendeeRaiseHand();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWebinarCtrlWrap::AllowAttendeeViewTheParticipantCount(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingWebinarCtrl().AllowAttendeeViewTheParticipantCount();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWebinarCtrlWrap::DisallowAttendeeViewTheParticipantCount(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingWebinarCtrl().DisallowAttendeeViewTheParticipantCount();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWebinarCtrlWrap::GetParticipantCount(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	int count = _g_native_wrap.GetMeetingServiceWrap().GetMeetingWebinarCtrl().GetParticipantCount();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int64_t)count);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWebinarCtrlWrap::GetWebinarMeetingStatus(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	v8::HandleScope scope(isolate);
	v8::Local<v8::Object> node = v8::Object::New(isolate);

	ZNWebinarMeetingStatus zn_status;
	zn_status = _g_native_wrap.GetMeetingServiceWrap().GetMeetingWebinarCtrl().GetWebinarMeetingStatus();

	node->Set(context, v8::String::NewFromUtf8(isolate, "allow_panellist_start_video", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_status.allow_panellist_start_video));
	node->Set(context, v8::String::NewFromUtf8(isolate, "allow_attendee_chat", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_status.allow_attendee_chat));
	node->Set(context, v8::String::NewFromUtf8(isolate, "allow_emoji_reaction", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_status.allow_emoji_reaction));
	node->Set(context, v8::String::NewFromUtf8(isolate, "allow_attendee_raise_hand", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_status.allow_attendee_raise_hand));
	node->Set(context, v8::String::NewFromUtf8(isolate, "allow_attendee_view_participant_count", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_status.allow_attendee_view_participant_count));

	args.GetReturnValue().Set(node);
}
void ZoomNodeMeetingWebinarCtrlWrap::SetPanelistChatPrivilege(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::SetPanelistChatPrivilegeParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::SetPanelistChatPrivilegeParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_privilege())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		ZNSDKPanelistChatPrivilege zn_privilege = (ZNSDKPanelistChatPrivilege)proto_params.privilege();

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingWebinarCtrl().SetPanelistChatPrivilege(zn_privilege);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWebinarCtrlWrap::GetPanelistChatPrivilege(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	v8::HandleScope scope(isolate);
	v8::Local<v8::Object> node = v8::Object::New(isolate);

	ZNSDKPanelistChatPrivilege privilege;
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingWebinarCtrl().GetPanelistChatPrivilege(privilege);

	node->Set(context, v8::String::NewFromUtf8(isolate, "err", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int32_t)err));
	node->Set(context, v8::String::NewFromUtf8(isolate, "privilege", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int32_t)privilege));

	args.GetReturnValue().Set(node);
}
void ZoomNodeMeetingWebinarCtrlWrap::SetAttendeeViewMode(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::SetAttendeeViewModeParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::SetAttendeeViewModeParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_mode())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		ZNSDKAttendeeViewMode zn_mode = (ZNSDKAttendeeViewMode)proto_params.mode();

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingWebinarCtrl().SetAttendeeViewMode(zn_mode);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWebinarCtrlWrap::GetAttendeeViewMode(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	int mode = _g_native_wrap.GetMeetingServiceWrap().GetMeetingWebinarCtrl().GetAttendeeViewMode();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)mode);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWebinarCtrlWrap::GetWebinarLegalNoticesPrompt(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZoomSTRING zn_prompt = _g_native_wrap.GetMeetingServiceWrap().GetMeetingWebinarCtrl().GetWebinarLegalNoticesPrompt();
	v8::Local<v8::String> bret = v8::String::NewFromUtf8(isolate, zs2s(zn_prompt).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWebinarCtrlWrap::GetWebinarLegalNoticesExplained(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	v8::HandleScope scope(isolate);
	v8::Local<v8::Object> node = v8::Object::New(isolate);

	ZNWebinarLegalNoticesExplainedInfo zn_explained_info;
	bool err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingWebinarCtrl().GetWebinarLegalNoticesExplained(zn_explained_info);

	node->Set(context, v8::String::NewFromUtf8(isolate, "err", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, err));
	node->Set(context, v8::String::NewFromUtf8(isolate, "explained_content", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_explained_info.explained_content).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
	node->Set(context, v8::String::NewFromUtf8(isolate, "url_register_account_owner", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_explained_info.url_register_account_owner).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
	node->Set(context, v8::String::NewFromUtf8(isolate, "url_register_terms", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_explained_info.url_register_terms).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
	node->Set(context, v8::String::NewFromUtf8(isolate, "url_register_privacy_policy", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_explained_info.url_register_privacy_policy).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());

	args.GetReturnValue().Set(node);
}
void ZoomNodeMeetingWebinarCtrlWrap::SetOnPromptAttendee2PanelistResultCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onPromptAttendee2PanelistResult.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onPromptAttendee2PanelistResult.Reset(isolate, cbfunc);

	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWebinarCtrlWrap::SetOnDepromptPanelist2AttendeeResultCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onDepromptPanelist2AttendeeResult.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onDepromptPanelist2AttendeeResult.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWebinarCtrlWrap::SetOnAllowPanelistStartVideoNotificationCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onAllowPanelistStartVideoNotification.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onAllowPanelistStartVideoNotification.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWebinarCtrlWrap::SetOnDisallowPanelistStartVideoNotificationCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onDisallowPanelistStartVideoNotification.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onDisallowPanelistStartVideoNotification.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWebinarCtrlWrap::SetOnSelfAllowTalkNotificationCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onSelfAllowTalkNotification.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onSelfAllowTalkNotification.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWebinarCtrlWrap::SetOnSelfDisallowTalkNotificationCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onSelfDisallowTalkNotification.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onSelfDisallowTalkNotification.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWebinarCtrlWrap::SetOnAllowAttendeeChatNotificationCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onAllowAttendeeChatNotification.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onAllowAttendeeChatNotification.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWebinarCtrlWrap::SetOnDisallowAttendeeChatNotificationCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onDisallowAttendeeChatNotification.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onDisallowAttendeeChatNotification.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWebinarCtrlWrap::SetOnAllowWebinarReactionStatusChangedCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onAllowWebinarReactionStatusChanged.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onAllowWebinarReactionStatusChanged.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWebinarCtrlWrap::SetOnAllowAttendeeRaiseHandStatusChangedCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onAllowAttendeeRaiseHandStatusChanged.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onAllowAttendeeRaiseHandStatusChanged.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWebinarCtrlWrap::SetOnAllowAttendeeViewTheParticipantCountStatusChangedCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onAllowAttendeeViewTheParticipantCountStatusChanged.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onAllowAttendeeViewTheParticipantCountStatusChanged.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWebinarCtrlWrap::SetOnAttendeeAudioStatusNotificationCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onAttendeeAudioStatusNotification.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onAttendeeAudioStatusNotification.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingWebinarCtrlWrap::SetOnAttendeePromoteConfirmResultCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onAttendeePromoteConfirmResult.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onAttendeePromoteConfirmResult.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}