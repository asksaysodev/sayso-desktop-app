#include "zoom_node_meeting_polling_ctrl.h"

ZoomNodePollingCtrlWrap::ZoomNodePollingCtrlWrap()
{
	_g_native_wrap.GetMeetingServiceWrap().GetMeetingPollingCtrl().SetSink(&ZoomNodeSinkWrapMgr::GetInst().m_pollingWrapSink);
}

ZoomNodePollingCtrlWrap::~ZoomNodePollingCtrlWrap()
{
	_g_native_wrap.GetMeetingServiceWrap().GetMeetingPollingCtrl().SetSink(NULL);
}

void ZoomNodePollingCtrlWrap::CanGetRightAnswerItemList (const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = args.GetIsolate();
	bool zn_bIs = false;
	do {
		com::electron::sdk::proto::PollingIDParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::PollingIDParams >(args, proto_params))
		{
			zn_bIs = false;
			break;
		}
		if (!proto_params.has_pid())
		{
			zn_bIs = false;
			break;
		}
		ZoomSTRING _zn_pollingID;
		_zn_pollingID = s2zs(proto_params.pid());

		zn_bIs = _g_native_wrap.GetMeetingServiceWrap().GetMeetingPollingCtrl().CanGetRightAnswerItemList(_zn_pollingID);
	} while (false);

	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, zn_bIs);
	args.GetReturnValue().Set(bret);
}

void ZoomNodePollingCtrlWrap::CanDoPolling (const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = args.GetIsolate();
	bool zn_bIs = _g_native_wrap.GetMeetingServiceWrap().GetMeetingPollingCtrl().CanDoPolling();
	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, zn_bIs);
	args.GetReturnValue().Set(bret);
}

void ZoomNodePollingCtrlWrap::GetPollingQuestionItemList (const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	v8::Local<v8::Array> all_question_nodes = v8::Array::New(isolate);

	do {
		com::electron::sdk::proto::PollingIDParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::PollingIDParams >(args, proto_params))
		{
			break;
		}
		if (!proto_params.has_pid())
		{
			break;
		}
		ZoomSTRING _zn_pollingID;
		_zn_pollingID = s2zs(proto_params.pid());
	
		ZNList<ZNSDKPollingQuestionItem> zn_polling_question_item_list;
		zn_polling_question_item_list = _g_native_wrap.GetMeetingServiceWrap().GetMeetingPollingCtrl().GetPollingQuestionItemList(_zn_pollingID);

		for (unsigned int i = 0; i < zn_polling_question_item_list.size(); ++i) {
			v8::HandleScope scope(isolate);
			v8::Local<v8::Object> node = v8::Object::New(isolate);

			node->Set(context, v8::String::NewFromUtf8(isolate, "pollingID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_polling_question_item_list[i].pollingID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
			node->Set(context, v8::String::NewFromUtf8(isolate, "pollingQuestionID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_polling_question_item_list[i].pollingQuestionID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
			node->Set(context, v8::String::NewFromUtf8(isolate, "pollingQuestionName", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_polling_question_item_list[i].pollingQuestionName).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
			node->Set(context, v8::String::NewFromUtf8(isolate, "pollingQuestionType", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, zn_polling_question_item_list[i].pollingQuestionType));
			node->Set(context, v8::String::NewFromUtf8(isolate, "answeredCount", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, zn_polling_question_item_list[i].answeredCount));
			node->Set(context, v8::String::NewFromUtf8(isolate, "isRequired", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_polling_question_item_list[i].isRequired));
			
			// Sub-question list
			v8::Local<v8::Array> sub_question_nodes = v8::Array::New(isolate);
			for (unsigned int j = 0; j < zn_polling_question_item_list[i].pollingSubQuestionItemList.size(); ++j) {
				v8::HandleScope scope(isolate);
				v8::Local<v8::Object> subItem = v8::Object::New(isolate);

				subItem->Set(context, v8::String::NewFromUtf8(isolate, "pollingID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_polling_question_item_list[i].pollingSubQuestionItemList[j].pollingID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
				subItem->Set(context, v8::String::NewFromUtf8(isolate, "pollingQuestionID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_polling_question_item_list[i].pollingSubQuestionItemList[j].pollingQuestionID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
				subItem->Set(context, v8::String::NewFromUtf8(isolate, "pollingQuestionName", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_polling_question_item_list[i].pollingSubQuestionItemList[j].pollingQuestionName).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
				subItem->Set(context, v8::String::NewFromUtf8(isolate, "pollingQuestionType", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, zn_polling_question_item_list[i].pollingSubQuestionItemList[j].pollingQuestionType));
				subItem->Set(context, v8::String::NewFromUtf8(isolate, "answeredCount", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, zn_polling_question_item_list[i].pollingSubQuestionItemList[j].answeredCount));
				subItem->Set(context, v8::String::NewFromUtf8(isolate, "isRequired", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_polling_question_item_list[i].pollingSubQuestionItemList[j].isRequired));

				v8::Local<v8::Array> sub_subQuestionNodes = v8::Array::New(isolate);

				v8::Local<v8::Array> sub_answerNodes = v8::Array::New(isolate);
				for (unsigned int k = 0; k < zn_polling_question_item_list[i].pollingSubQuestionItemList[j].pollingAnswerItemList.size(); ++k) {
					v8::HandleScope scope(isolate);
					v8::Local<v8::Object> sub_answer_item = v8::Object::New(isolate);

					sub_answer_item->Set(context, v8::String::NewFromUtf8(isolate, "pollingID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_polling_question_item_list[i].pollingSubQuestionItemList[j].pollingAnswerItemList[k].pollingID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
					sub_answer_item->Set(context, v8::String::NewFromUtf8(isolate, "pollingQuestionID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_polling_question_item_list[i].pollingSubQuestionItemList[j].pollingAnswerItemList[k].pollingQuestionID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
					sub_answer_item->Set(context, v8::String::NewFromUtf8(isolate, "pollingSubQuestionID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_polling_question_item_list[i].pollingSubQuestionItemList[j].pollingAnswerItemList[k].pollingSubQuestionID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
					sub_answer_item->Set(context, v8::String::NewFromUtf8(isolate, "pollingAnswerID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_polling_question_item_list[i].pollingSubQuestionItemList[j].pollingAnswerItemList[k].pollingAnswerID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
					sub_answer_item->Set(context, v8::String::NewFromUtf8(isolate, "pollingAnswerName", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_polling_question_item_list[i].pollingSubQuestionItemList[j].pollingAnswerItemList[k].pollingAnswerName).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
					sub_answer_item->Set(context, v8::String::NewFromUtf8(isolate, "pollingAnsweredContent", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_polling_question_item_list[i].pollingSubQuestionItemList[j].pollingAnswerItemList[k].pollingAnsweredContent).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
					sub_answer_item->Set(context, v8::String::NewFromUtf8(isolate, "isChecked", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_polling_question_item_list[i].pollingSubQuestionItemList[j].pollingAnswerItemList[k].isChecked));

					sub_answerNodes->Set(context, k, sub_answer_item);
				}

				v8::Local<v8::Array> sub_final_node = v8::Array::New(isolate);
				sub_final_node->Set(context, 0, subItem);
				sub_final_node->Set(context, 1, sub_subQuestionNodes);
				sub_final_node->Set(context, 2, sub_answerNodes);

				sub_question_nodes->Set(context, j, sub_final_node);
			}

			// Answer item list 
			v8::Local<v8::Array> answerNodes = v8::Array::New(isolate);
			for (unsigned int j = 0; j < zn_polling_question_item_list[i].pollingAnswerItemList.size(); ++j) {
				v8::HandleScope scope(isolate);
				v8::Local<v8::Object> item = v8::Object::New(isolate);

				item->Set(context, v8::String::NewFromUtf8(isolate, "pollingID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_polling_question_item_list[i].pollingAnswerItemList[j].pollingID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
				item->Set(context, v8::String::NewFromUtf8(isolate, "pollingQuestionID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_polling_question_item_list[i].pollingAnswerItemList[j].pollingQuestionID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
				item->Set(context, v8::String::NewFromUtf8(isolate, "pollingSubQuestionID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_polling_question_item_list[i].pollingAnswerItemList[j].pollingSubQuestionID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
				item->Set(context, v8::String::NewFromUtf8(isolate, "pollingAnswerID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_polling_question_item_list[i].pollingAnswerItemList[j].pollingAnswerID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
				item->Set(context, v8::String::NewFromUtf8(isolate, "pollingAnswerName", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_polling_question_item_list[i].pollingAnswerItemList[j].pollingAnswerName).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
				item->Set(context, v8::String::NewFromUtf8(isolate, "pollingAnsweredContent", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_polling_question_item_list[i].pollingAnswerItemList[j].pollingAnsweredContent).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
				item->Set(context, v8::String::NewFromUtf8(isolate, "isChecked", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_polling_question_item_list[i].pollingAnswerItemList[j].isChecked));
				
				answerNodes->Set(context, j, item);
			}
			
			v8::Local<v8::Array> final_node = v8::Array::New(isolate);
			final_node->Set(context, 0, node);
			final_node->Set(context, 1, sub_question_nodes);
			final_node->Set(context, 2, answerNodes);

			all_question_nodes->Set(context, i, final_node);
		}
	} while (false);
	args.GetReturnValue().Set(all_question_nodes);
}

void ZoomNodePollingCtrlWrap::GetPollingRightAnswerItemList (const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	v8::Local<v8::Array> all_poll_nodes = v8::Array::New(isolate);
	do {
		com::electron::sdk::proto::PollingIDParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::PollingIDParams >(args, proto_params))
		{
			break;
		}
		if (!proto_params.has_pid())
		{
			break;
		}
		ZoomSTRING _zn_pollingID;
		_zn_pollingID = s2zs(proto_params.pid());
	

		ZNList<ZNSDKPollingAnswerItem> zn_polling_answer_item_list;
		zn_polling_answer_item_list = _g_native_wrap.GetMeetingServiceWrap().GetMeetingPollingCtrl().GetPollingRightAnswerItemList(_zn_pollingID);

		for (unsigned int i = 0; i < zn_polling_answer_item_list.size(); ++i) {
			v8::HandleScope scope(isolate);
			v8::Local<v8::Object> node = v8::Object::New(isolate);

			node->Set(context, v8::String::NewFromUtf8(isolate, "pollingID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_polling_answer_item_list[i].pollingID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
			node->Set(context, v8::String::NewFromUtf8(isolate, "pollingQuestionID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_polling_answer_item_list[i].pollingQuestionID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
			node->Set(context, v8::String::NewFromUtf8(isolate, "pollingSubQuestionID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_polling_answer_item_list[i].pollingSubQuestionID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
			node->Set(context, v8::String::NewFromUtf8(isolate, "pollingAnswerID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_polling_answer_item_list[i].pollingAnswerID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
			node->Set(context, v8::String::NewFromUtf8(isolate, "pollingAnswerName", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_polling_answer_item_list[i].pollingAnswerName).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
			node->Set(context, v8::String::NewFromUtf8(isolate, "pollingAnsweredContent", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_polling_answer_item_list[i].pollingAnsweredContent).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
			node->Set(context, v8::String::NewFromUtf8(isolate, "isChecked", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_polling_answer_item_list[i].isChecked));
			
			all_poll_nodes->Set(context, i, node);
		}
	} while (false);
	args.GetReturnValue().Set(all_poll_nodes);
}

void ZoomNodePollingCtrlWrap::GetPollingAnswerResultItemList (const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	v8::Local<v8::Array> all_poll_nodes = v8::Array::New(isolate);
	do {
		com::electron::sdk::proto::PollingIDParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::PollingIDParams >(args, proto_params))
		{
			break;
		}
		if (!proto_params.has_pid())
		{
			break;
		}
		ZoomSTRING _zn_pollingID;
		_zn_pollingID = s2zs(proto_params.pid());
	

		ZNList<ZNSDKPollingAnswerResultItem> zn_polling_answer_result_item_list;
		zn_polling_answer_result_item_list = _g_native_wrap.GetMeetingServiceWrap().GetMeetingPollingCtrl().GetPollingAnswerResultItemList(_zn_pollingID);

		for (unsigned int i = 0; i < zn_polling_answer_result_item_list.size(); ++i) {
			v8::HandleScope scope(isolate);
			v8::Local<v8::Object> node = v8::Object::New(isolate);

			node->Set(context, v8::String::NewFromUtf8(isolate, "pollingID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_polling_answer_result_item_list[i].pollingID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
			node->Set(context, v8::String::NewFromUtf8(isolate, "pollingQuestionID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_polling_answer_result_item_list[i].pollingQuestionID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
			node->Set(context, v8::String::NewFromUtf8(isolate, "pollingSubQuestionID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_polling_answer_result_item_list[i].pollingSubQuestionID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
			node->Set(context, v8::String::NewFromUtf8(isolate, "pollingAnswerID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_polling_answer_result_item_list[i].pollingAnswerID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
			node->Set(context, v8::String::NewFromUtf8(isolate, "pollingAnswerName", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_polling_answer_result_item_list[i].pollingAnswerName).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
			node->Set(context, v8::String::NewFromUtf8(isolate, "selectedCount", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, zn_polling_answer_result_item_list[i].selectedCount));

			all_poll_nodes->Set(context, i, node);
		}
	} while (false);
	args.GetReturnValue().Set(all_poll_nodes);
}

void ZoomNodePollingCtrlWrap::GetPollingItemByID (const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	ZNSDKPollingItem zn_polling_item;
	v8::HandleScope scope(isolate);
	v8::Local<v8::Array> poll_nodes = v8::Array::New(isolate);

	do {
		com::electron::sdk::proto::PollingIDParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::PollingIDParams >(args, proto_params))
		{
			break;
		}
		if (!proto_params.has_pid())
		{
			break;
		}
		ZoomSTRING _zn_pollingID;
		_zn_pollingID = s2zs(proto_params.pid());

		zn_polling_item = _g_native_wrap.GetMeetingServiceWrap().GetMeetingPollingCtrl().GetPollingItemByID(_zn_pollingID);
		v8::Local<v8::Object> node = v8::Object::New(isolate);
		node->Set(context, v8::String::NewFromUtf8(isolate, "pollingID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_polling_item.pollingID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "pollingName", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_polling_item.pollingName).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "pollingType", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, zn_polling_item.pollingType));
		node->Set(context, v8::String::NewFromUtf8(isolate, "pollingStatus", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, zn_polling_item.pollingStatus));
		node->Set(context, v8::String::NewFromUtf8(isolate, "pollingQuestionCount", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, zn_polling_item.pollingQuestionCount));
		node->Set(context, v8::String::NewFromUtf8(isolate, "totalVotedUserCount", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, zn_polling_item.totalVotedUserCount));
		node->Set(context, v8::String::NewFromUtf8(isolate, "isLibraryPolling", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_polling_item.isLibraryPolling));
		poll_nodes->Set(context, 0, node);
	} while (false);

	args.GetReturnValue().Set(poll_nodes);
}

void ZoomNodePollingCtrlWrap::GetPollingItemList (const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();

	ZNList<ZNSDKPollingItem> zn_polling_item_list;
	zn_polling_item_list = _g_native_wrap.GetMeetingServiceWrap().GetMeetingPollingCtrl().GetPollingItemList();

	v8::Local<v8::Array> all_poll_nodes = v8::Array::New(isolate);
	for (unsigned int i = 0; i < zn_polling_item_list.size(); ++i) {
		v8::HandleScope scope(isolate);
		v8::Local<v8::Object> node = v8::Object::New(isolate);

		node->Set(context, v8::String::NewFromUtf8(isolate, "pollingID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_polling_item_list[i].pollingID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "pollingName", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_polling_item_list[i].pollingName).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "pollingType", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, zn_polling_item_list[i].pollingType));
		node->Set(context, v8::String::NewFromUtf8(isolate, "pollingStatus", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, zn_polling_item_list[i].pollingStatus));
		node->Set(context, v8::String::NewFromUtf8(isolate, "pollingQuestionCount", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, zn_polling_item_list[i].pollingQuestionCount));
		node->Set(context, v8::String::NewFromUtf8(isolate, "totalVotedUserCount", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, zn_polling_item_list[i].totalVotedUserCount));
		node->Set(context, v8::String::NewFromUtf8(isolate, "isLibraryPolling", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_polling_item_list[i].isLibraryPolling));

		all_poll_nodes->Set(context, i, node);
	}
	args.GetReturnValue().Set(all_poll_nodes);
}

void ZoomNodePollingCtrlWrap::StartPolling (const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do {
		com::electron::sdk::proto::PollingIDParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::PollingIDParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_pid())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		ZoomSTRING _zn_pollingID;
		_zn_pollingID = s2zs(proto_params.pid());

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingPollingCtrl().StartPolling(_zn_pollingID);
	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodePollingCtrlWrap::StopPolling (const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do {
		com::electron::sdk::proto::PollingIDParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::PollingIDParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_pid())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		ZoomSTRING _zn_pollingID;
		_zn_pollingID = s2zs(proto_params.pid());

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingPollingCtrl().StopPolling(_zn_pollingID);
	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodePollingCtrlWrap::RestartPolling (const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do {
		com::electron::sdk::proto::PollingIDParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::PollingIDParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_pid())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		ZoomSTRING _zn_pollingID;
		_zn_pollingID = s2zs(proto_params.pid());

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingPollingCtrl().RestartPolling(_zn_pollingID);
	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodePollingCtrlWrap::IsQuestionCaseSensitive (const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = args.GetIsolate();
	bool zn_bIs = false;
	do {
		com::electron::sdk::proto::QuestionCaseSensitiveParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::QuestionCaseSensitiveParams >(args, proto_params))
		{
			zn_bIs = false;
			break;
		}
		if (!proto_params.has_pid() || !proto_params.has_qid())
		{
			zn_bIs = false;
			break;
		}
		ZoomSTRING _zn_pollingID;
		_zn_pollingID = s2zs(proto_params.pid());

		ZoomSTRING _zn_questionID;
		_zn_questionID = s2zs(proto_params.qid());

		zn_bIs = _g_native_wrap.GetMeetingServiceWrap().GetMeetingPollingCtrl().IsQuestionCaseSensitive(_zn_pollingID, _zn_questionID);
	} while (false);

	v8::Local<v8::Boolean> bret = v8::Boolean::New(isolate, zn_bIs);
	args.GetReturnValue().Set(bret);
}

void ZoomNodePollingCtrlWrap::GetPollingQuestionImagePath (const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = args.GetIsolate();
#if (defined _WIN32)
	ZoomSTRING zn_imagePath = L"";
#else
	ZoomSTRING zn_imagePath = "";
#endif
	do {
		com::electron::sdk::proto::QuestionImagePathParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::QuestionImagePathParams >(args, proto_params))
		{
			break;
		}
		if (!proto_params.has_pid() || !proto_params.has_qid())
		{
			break;
		}
		ZoomSTRING _zn_pollingID;
		_zn_pollingID = s2zs(proto_params.pid());

		ZoomSTRING _zn_questionID;
		_zn_questionID = s2zs(proto_params.qid());

		zn_imagePath = _g_native_wrap.GetMeetingServiceWrap().GetMeetingPollingCtrl().GetPollingQuestionImagePath(_zn_pollingID, _zn_questionID);
	} while (false);

	v8::Local<v8::String> bret = v8::String::NewFromUtf8(isolate, zs2s(zn_imagePath).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
	args.GetReturnValue().Set(bret);
}

void ZoomNodePollingCtrlWrap::SetOnPollingStatusChangedCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onPollingStatusChanged.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onPollingStatusChanged.Reset(isolate, cbfunc);

	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodePollingCtrlWrap::SetOnPollingResultUpdatedCB(const v8::FunctionCallbackInfo<v8::Value>& args) {
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
			ZoomNodeSinkHelper::GetInst().onPollingResultUpdated.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onPollingResultUpdated.Reset(isolate, cbfunc);

	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodePollingCtrlWrap::SetOnPollingListUpdatedCB(const v8::FunctionCallbackInfo<v8::Value>& args) {
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
			ZoomNodeSinkHelper::GetInst().onPollingListUpdated.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onPollingListUpdated.Reset(isolate, cbfunc);

	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodePollingCtrlWrap::SetOnPollingActionResultCB(const v8::FunctionCallbackInfo<v8::Value>& args) {
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
			ZoomNodeSinkHelper::GetInst().onPollingActionResult.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onPollingActionResult.Reset(isolate, cbfunc);

	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}