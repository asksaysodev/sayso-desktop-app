#include "zoom_node_meeting_chat_ctrl.h"
#include "zoom_node_wrap_util.h"

ZoomNodeMeetingChatCtrlWrap::ZoomNodeMeetingChatCtrlWrap()
{
	_g_native_wrap.GetMeetingServiceWrap().GetMeetingChatCtrl().SetSink(&ZoomNodeSinkWrapMgr::GetInst().m_meetingChatWrapSink);
}

ZoomNodeMeetingChatCtrlWrap::~ZoomNodeMeetingChatCtrlWrap()
{
	_g_native_wrap.GetMeetingServiceWrap().GetMeetingChatCtrl().SetSink(NULL);
}
void ZoomNodeMeetingChatCtrlWrap::GetChatStatus(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	ZNChatStatus zn_chatStatus = _g_native_wrap.GetMeetingServiceWrap().GetMeetingChatCtrl().GetChatStatus();

	v8::HandleScope scope(isolate);
	v8::Local<v8::Object> node = v8::Object::New(isolate);

	node->Set(context, v8::String::NewFromUtf8(isolate, "is_chat_off", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_chatStatus.is_chat_off));
	node->Set(context, v8::String::NewFromUtf8(isolate, "is_webinar_attendee", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_chatStatus.is_webinar_attendee));
	node->Set(context, v8::String::NewFromUtf8(isolate, "is_webinar_meeting", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_chatStatus.is_webinar_meeting));
	if (zn_chatStatus.is_webinar_meeting) {
		if (zn_chatStatus.is_webinar_attendee) {
			node->Set(context, v8::String::NewFromUtf8(isolate, "can_chat", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_chatStatus.ut.webinar_attendee_status.can_chat));
			node->Set(context, v8::String::NewFromUtf8(isolate, "can_chat_to_all_panellist_and_attendee", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_chatStatus.ut.webinar_attendee_status.can_chat_to_all_panellist_and_attendee));
			node->Set(context, v8::String::NewFromUtf8(isolate, "can_chat_to_all_panellist", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_chatStatus.ut.webinar_attendee_status.can_chat_to_all_panellist));
		} else {
			node->Set(context, v8::String::NewFromUtf8(isolate, "can_chat_to_all_panellist", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_chatStatus.ut.webinar_other_status.can_chat_to_all_panellist));
			node->Set(context, v8::String::NewFromUtf8(isolate, "can_chat_to_all_panellist_and_attendee", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_chatStatus.ut.webinar_other_status.can_chat_to_all_panellist_and_attendee));
			node->Set(context, v8::String::NewFromUtf8(isolate, "can_chat_to_individual", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_chatStatus.ut.webinar_other_status.can_chat_to_individual));
		}
	} else {
		node->Set(context, v8::String::NewFromUtf8(isolate, "can_chat", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_chatStatus.ut.normal_meeting_status.can_chat));
		node->Set(context, v8::String::NewFromUtf8(isolate, "can_chat_to_all", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_chatStatus.ut.normal_meeting_status.can_chat_to_all));
		node->Set(context, v8::String::NewFromUtf8(isolate, "can_chat_to_individual", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_chatStatus.ut.normal_meeting_status.can_chat_to_individual));
		node->Set(context, v8::String::NewFromUtf8(isolate, "is_only_can_chat_to_host", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_chatStatus.ut.normal_meeting_status.is_only_can_chat_to_host));
	}

	args.GetReturnValue().Set(node);
}
void ZoomNodeMeetingChatCtrlWrap::SetParticipantsChatPrivilege(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::SetParticipantsChatPrivilegeParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::SetParticipantsChatPrivilegeParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_privilege())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		ZNSDKChatPrivilege _zn_privilege = (ZNSDKChatPrivilege)proto_params.privilege();

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingChatCtrl().SetParticipantsChatPrivilege(_zn_privilege);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingChatCtrlWrap::IsMeetingChatLegalNoticeAvailable(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool zn_bIs = _g_native_wrap.GetMeetingServiceWrap().GetMeetingChatCtrl().IsMeetingChatLegalNoticeAvailable();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, zn_bIs);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingChatCtrlWrap::getChatLegalNoticesPrompt(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZoomSTRING zn_chatLegalNoticesPrompt = _g_native_wrap.GetMeetingServiceWrap().GetMeetingChatCtrl().getChatLegalNoticesPrompt();
	v8::Local<v8::String> bret = v8::String::NewFromUtf8(isolate, zs2s(zn_chatLegalNoticesPrompt).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingChatCtrlWrap::getChatLegalNoticesExplained(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZoomSTRING zn_chatLegalNoticesExplained = _g_native_wrap.GetMeetingServiceWrap().GetMeetingChatCtrl().getChatLegalNoticesExplained();
	v8::Local<v8::String> bret = v8::String::NewFromUtf8(isolate, zs2s(zn_chatLegalNoticesExplained).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingChatCtrlWrap::IsShareMeetingChatLegalNoticeAvailable(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool zn_bIs = _g_native_wrap.GetMeetingServiceWrap().GetMeetingChatCtrl().IsShareMeetingChatLegalNoticeAvailable();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, zn_bIs);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingChatCtrlWrap::GetShareMeetingChatStartedLegalNoticeContent(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZoomSTRING zn_chatStartedLegalNoticeContent = _g_native_wrap.GetMeetingServiceWrap().GetMeetingChatCtrl().GetShareMeetingChatStartedLegalNoticeContent();
	v8::Local<v8::String> bret = v8::String::NewFromUtf8(isolate, zs2s(zn_chatStartedLegalNoticeContent).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingChatCtrlWrap::GetShareMeetingChatStoppedLegalNoticeContent(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZoomSTRING zn_chatStoppedLegalNoticeContent = _g_native_wrap.GetMeetingServiceWrap().GetMeetingChatCtrl().GetShareMeetingChatStoppedLegalNoticeContent();
	v8::Local<v8::String> bret = v8::String::NewFromUtf8(isolate, zs2s(zn_chatStoppedLegalNoticeContent).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingChatCtrlWrap::IsChatMessageCanBeDeleted(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool zn_bIs = false;
	do
	{
		com::electron::sdk::proto::ChatMessageIDParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::ChatMessageIDParams >(args, proto_params))
		{
			zn_bIs = false;
			break;
		}
		if (!proto_params.has_msgid())
		{
			zn_bIs = false;
			break;
		}
		ZoomSTRING _zn_msgID;
		_zn_msgID = s2zs(proto_params.msgid());

		zn_bIs = _g_native_wrap.GetMeetingServiceWrap().GetMeetingChatCtrl().IsChatMessageCanBeDeleted(_zn_msgID);
	} while (false);
	
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, zn_bIs);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingChatCtrlWrap::DeleteChatMessage(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::ChatMessageIDParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::ChatMessageIDParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_msgid())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		ZoomSTRING _zn_msgID;
		_zn_msgID = s2zs(proto_params.msgid());

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingChatCtrl().DeleteChatMessage(_zn_msgID);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingChatCtrlWrap::GetAllChatMessageID(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	ZNList<ZoomSTRING> zn_msgID_lst;
	zn_msgID_lst = _g_native_wrap.GetMeetingServiceWrap().GetMeetingChatCtrl().GetAllChatMessageID();

	v8::Local<v8::Array> chat_message_nodes = v8::Array::New(isolate);
	for (unsigned int i = 0; i < zn_msgID_lst.size(); ++i) {
		v8::HandleScope scope(isolate);
		v8::Local<v8::Object> node = v8::Object::New(isolate);
		node->Set(context, v8::String::NewFromUtf8(isolate, "msgID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_msgID_lst[i]).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		chat_message_nodes->Set(context, i, node);
	}
	args.GetReturnValue().Set(chat_message_nodes);
}
void ZoomNodeMeetingChatCtrlWrap::GetChatMessageById(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	v8::HandleScope scope(isolate);
	v8::Local<v8::Object> node = v8::Object::New(isolate);
	do
	{
		com::electron::sdk::proto::ChatMessageIDParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::ChatMessageIDParams >(args, proto_params))
		{
			break;
		}
		if (!proto_params.has_msgid())
		{
			break;
		}
		ZoomSTRING _zn_msgID = s2zs(proto_params.msgid());

		ZNChatMsgInfo zn_chatMsg = _g_native_wrap.GetMeetingServiceWrap().GetMeetingChatCtrl().GetChatMessageById(_zn_msgID);
		node = NodeWrapUtil::CreateChatMsgInfoObject(isolate, zn_chatMsg);
		
	} while (false);
	
	args.GetReturnValue().Set(node);
}
void ZoomNodeMeetingChatCtrlWrap::SetChatMessageBuilder(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::SetChatMessageBuilderParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::SetChatMessageBuilderParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_content() ||
			!proto_params.has_receiver() ||
			!proto_params.has_type()
			)
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		ZoomSTRING _zn_content;
		_zn_content = s2zs(proto_params.content());

		int _receiver = proto_params.receiver();

		ZoomSTRING _zn_threadId;
		if (proto_params.has_threadid())
		{
			_zn_threadId = s2zs(proto_params.threadid());
		}

		ZNSDKChatMessageType _zn_type = (ZNSDKChatMessageType)proto_params.type();
		
		int _positionStart = 0;
		if (proto_params.has_positionstart())
		{
			_positionStart = proto_params.positionstart();
		}
		int _positionEnd = 0;
		if (proto_params.has_positionend())
		{
			_positionEnd = proto_params.positionend();
		}

		bool _b_clearStyles = false;
		if (proto_params.has_bclearstyles())
		{
			convertBool(proto_params.bclearstyles(), _b_clearStyles);
		}
		bool _b_clear = false;
		if (proto_params.has_bclear())
		{
			convertBool(proto_params.bclear(), _b_clear);
		}

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingChatCtrl().SetChatMessageBuilder(_zn_content, _receiver, _zn_threadId, _zn_type, _positionStart, _positionEnd, _b_clearStyles, _b_clear);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingChatCtrlWrap::SendLastBuildChatMsgTo(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingChatCtrl().SendLastBuildChatMsgTo();
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingChatCtrlWrap::IsFileTransferEnabled(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	bool zn_bIs = _g_native_wrap.GetMeetingServiceWrap().GetMeetingChatCtrl().IsFileTransferEnabled();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, zn_bIs);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingChatCtrlWrap::TransferFile(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::TransferFileParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::TransferFileParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_filepath() ||
			!proto_params.has_userid()
			)
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		ZoomSTRING _zn_filePath;
		_zn_filePath = s2zs(proto_params.filepath());

		unsigned int _userid = proto_params.userid();
		
		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingChatCtrl().TransferFile(_zn_filePath, _userid);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingChatCtrlWrap::TransferFileToAll(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::TransferFileParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::TransferFileParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_filepath())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		ZoomSTRING _zn_filePath;
		_zn_filePath = s2zs(proto_params.filepath());

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingChatCtrl().TransferFileToAll(_zn_filePath);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingChatCtrlWrap::GetTransferFileTypeAllowList(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZoomSTRING zn_list = _g_native_wrap.GetMeetingServiceWrap().GetMeetingChatCtrl().GetTransferFileTypeAllowList();
	v8::Local<v8::String> bret = v8::String::NewFromUtf8(isolate, zs2s(zn_list).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingChatCtrlWrap::GetMaxTransferFileSizeBytes(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	unsigned long long max_size = _g_native_wrap.GetMeetingServiceWrap().GetMeetingChatCtrl().GetMaxTransferFileSizeBytes();
	v8::Local<v8::Number> bret = v8::Number::New(isolate, (unsigned long long)max_size);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingChatCtrlWrap::GetReceiver(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	unsigned int receiver_id = _g_native_wrap.GetMeetingServiceWrap().GetMeetingChatCtrl().GetReceiver();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (uint32_t)receiver_id);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingChatCtrlWrap::CancelSend(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingChatCtrl().CancelSend();
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingChatCtrlWrap::GetSender(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	unsigned int sender_id = _g_native_wrap.GetMeetingServiceWrap().GetMeetingChatCtrl().GetSender();
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (uint32_t)sender_id);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingChatCtrlWrap::CancelReceive(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingChatCtrl().CancelReceive();
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingChatCtrlWrap::StartReceive(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::StartReceiveParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::StartReceiveParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_path())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		ZoomSTRING _zn_path;
		_zn_path = s2zs(proto_params.path());

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingChatCtrl().StartReceive(_zn_path);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingChatCtrlWrap::SetOnChatMsgNotificationCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onChatMsgNotification.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onChatMsgNotification.Reset(isolate, cbfunc);

	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingChatCtrlWrap::SetOnChatStatusChangedNotificationCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onChatStatusChangedNotification.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onChatStatusChangedNotification.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingChatCtrlWrap::SetOnChatMsgDeleteNotificationCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onChatMsgDeleteNotification.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onChatMsgDeleteNotification.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingChatCtrlWrap::SetOnChatMessageEditNotificationCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onChatMessageEditNotification.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onChatMessageEditNotification.Reset(isolate, cbfunc);

	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingChatCtrlWrap::SetOnShareMeetingChatStatusChangedCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onShareMeetingChatStatusChanged.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onShareMeetingChatStatusChanged.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingChatCtrlWrap::SetOnFileSendStartCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onFileSendStart.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onFileSendStart.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingChatCtrlWrap::SetOnFileReceivedCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onFileReceived.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onFileReceived.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
void ZoomNodeMeetingChatCtrlWrap::SetOnFileTransferProgressCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onFileTransferProgress.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onFileTransferProgress.Reset(isolate, cbfunc);

	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}