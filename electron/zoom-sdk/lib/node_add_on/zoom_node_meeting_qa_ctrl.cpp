#include "zoom_node_meeting_qa_ctrl.h"
#include <cstdio>

ZoomNodeMeetingQACtrlWrap::ZoomNodeMeetingQACtrlWrap()
{
	_g_native_wrap.GetMeetingServiceWrap().GetMeetingQACtrl().SetSink(&ZoomNodeSinkWrapMgr::GetInst().m_meetingQAWrapSink);
}

ZoomNodeMeetingQACtrlWrap::~ZoomNodeMeetingQACtrlWrap()
{
	_g_native_wrap.GetMeetingServiceWrap().GetMeetingQACtrl().SetSink(NULL);
}

void ZoomNodeMeetingQACtrlWrap::GetQuestion(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	ZNQAItemInfo zn_question_info;
	v8::HandleScope scope(isolate);
	v8::Local<v8::Array> question_nodes = v8::Array::New(isolate);

	do {
		com::electron::sdk::proto::QuestionIDParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::QuestionIDParams >(args, proto_params))
		{
			break;
		}
		if (!proto_params.has_qid())
		{
			break;
		}
		ZoomSTRING _zn_questionID;
		_zn_questionID = s2zs(proto_params.qid());

		zn_question_info = _g_native_wrap.GetMeetingServiceWrap().GetMeetingQACtrl().GetQuestion(_zn_questionID);
		v8::Local<v8::Object> node = v8::Object::New(isolate);
		node->Set(context, v8::String::NewFromUtf8(isolate, "timeStamp", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, zn_question_info.timeStamp));
		node->Set(context, v8::String::NewFromUtf8(isolate, "text", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_question_info.text).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "upvoteNum", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, zn_question_info.upvoteNum));
		node->Set(context, v8::String::NewFromUtf8(isolate, "senderName", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_question_info.senderName).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "questionID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_question_info.questionID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "isAnonymous", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_question_info.isAnonymous));
		node->Set(context, v8::String::NewFromUtf8(isolate, "isMarkedAsAnswered", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_question_info.isMarkedAsAnswered));
		node->Set(context, v8::String::NewFromUtf8(isolate, "isMarkedAsDismissed", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_question_info.isMarkedAsDismissed));
		node->Set(context, v8::String::NewFromUtf8(isolate, "isSenderMyself", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_question_info.isSenderMyself));
		node->Set(context, v8::String::NewFromUtf8(isolate, "isMySelfUpvoted", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_question_info.isMySelfUpvoted));
		node->Set(context, v8::String::NewFromUtf8(isolate, "hasLiveAnswers", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_question_info.hasLiveAnswers));
		node->Set(context, v8::String::NewFromUtf8(isolate, "hasTextAnswers", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_question_info.hasTextAnswers));
		node->Set(context, v8::String::NewFromUtf8(isolate, "amILiveAnswering", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_question_info.amILiveAnswering));
		node->Set(context, v8::String::NewFromUtf8(isolate, "liveAnswerName", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_question_info.liveAnswerName).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "isLiveAnswering", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_question_info.isLiveAnswering));
		
		question_nodes->Set(context, 0, node);

		// answerList
		v8::Local<v8::Array> answerNodes = v8::Array::New(isolate);
		for (unsigned int i = 0; i < zn_question_info.answerList.size(); ++i) {
			v8::HandleScope scope(isolate);
			v8::Local<v8::Object> item = v8::Object::New(isolate);
			item->Set(context, v8::String::NewFromUtf8(isolate, "timeStamp", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, zn_question_info.answerList[i].timeStamp));
			item->Set(context, v8::String::NewFromUtf8(isolate, "text", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_question_info.answerList[i].text).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
			item->Set(context, v8::String::NewFromUtf8(isolate, "senderName", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_question_info.answerList[i].senderName).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
			item->Set(context, v8::String::NewFromUtf8(isolate, "questionID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_question_info.answerList[i].questionID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
			item->Set(context, v8::String::NewFromUtf8(isolate, "answerID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_question_info.answerList[i].answerID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
			item->Set(context, v8::String::NewFromUtf8(isolate, "isPrivate", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_question_info.answerList[i].isPrivate));
			item->Set(context, v8::String::NewFromUtf8(isolate, "isLiveAnswer", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_question_info.answerList[i].isLiveAnswer));
			item->Set(context, v8::String::NewFromUtf8(isolate, "isSenderMyself", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_question_info.answerList[i].isSenderMyself));

			answerNodes->Set(context, i, item);
		}

		question_nodes->Set(context, 1, answerNodes);
		
	} while (false);

	args.GetReturnValue().Set(question_nodes);
}

void ZoomNodeMeetingQACtrlWrap::GetAnswer(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	ZNAnswerItem zn_answer_info;
	v8::HandleScope scope(isolate);
	v8::Local<v8::Array> answer_nodes = v8::Array::New(isolate);

	do {
		com::electron::sdk::proto::AnswerIDParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::AnswerIDParams >(args, proto_params))
		{
			break;
		}
		if (!proto_params.has_answerid())
		{
			break;
		}
		ZoomSTRING _zn_answerID;
		_zn_answerID = s2zs(proto_params.answerid());

		zn_answer_info = _g_native_wrap.GetMeetingServiceWrap().GetMeetingQACtrl().GetAnswer(_zn_answerID);
		v8::Local<v8::Object> node = v8::Object::New(isolate);

		node->Set(context, v8::String::NewFromUtf8(isolate, "timeStamp", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, zn_answer_info.timeStamp));
		node->Set(context, v8::String::NewFromUtf8(isolate, "text", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_answer_info.text).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "senderName", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_answer_info.senderName).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "questionID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_answer_info.questionID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "answerID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_answer_info.answerID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "isPrivate", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_answer_info.isPrivate));
		node->Set(context, v8::String::NewFromUtf8(isolate, "isLiveAnswer", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_answer_info.isLiveAnswer));
		node->Set(context, v8::String::NewFromUtf8(isolate, "isSenderMyself", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_answer_info.isSenderMyself));

		answer_nodes->Set(context, 0, node);

	} while (false);

	args.GetReturnValue().Set(answer_nodes);
}

void ZoomNodeMeetingQACtrlWrap::GetAllQuestionList(const v8::FunctionCallbackInfo<v8::Value>& args) { 
	v8::Isolate* isolate = args.GetIsolate();
	auto context = isolate->GetCurrentContext();
	
	ZNList<ZNQAItemInfo> zn_qa_lst;
	zn_qa_lst = _g_native_wrap.GetMeetingServiceWrap().GetMeetingQACtrl().getAllQuestionList();

	v8::Local<v8::Array> all_question_nodes = v8::Array::New(isolate);
	for (unsigned int i = 0; i < zn_qa_lst.size(); ++i) {
		v8::HandleScope scope(isolate);
		v8::Local<v8::Object> node = v8::Object::New(isolate);


		node->Set(context, v8::String::NewFromUtf8(isolate, "timeStamp", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, zn_qa_lst[i].timeStamp));
		node->Set(context, v8::String::NewFromUtf8(isolate, "text", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_qa_lst[i].text).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "upvoteNum", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, zn_qa_lst[i].upvoteNum));
		node->Set(context, v8::String::NewFromUtf8(isolate, "senderName", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_qa_lst[i].senderName).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "questionID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_qa_lst[i].questionID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "isAnonymous", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_qa_lst[i].isAnonymous));
		node->Set(context, v8::String::NewFromUtf8(isolate, "isMarkedAsAnswered", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_qa_lst[i].isMarkedAsAnswered));
		node->Set(context, v8::String::NewFromUtf8(isolate, "isMarkedAsDismissed", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_qa_lst[i].isMarkedAsDismissed));
		node->Set(context, v8::String::NewFromUtf8(isolate, "isSenderMyself", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_qa_lst[i].isSenderMyself));
		node->Set(context, v8::String::NewFromUtf8(isolate, "isMySelfUpvoted", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_qa_lst[i].isMySelfUpvoted));
		node->Set(context, v8::String::NewFromUtf8(isolate, "hasLiveAnswers", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_qa_lst[i].hasLiveAnswers));
		node->Set(context, v8::String::NewFromUtf8(isolate, "hasTextAnswers", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_qa_lst[i].hasTextAnswers));
		node->Set(context, v8::String::NewFromUtf8(isolate, "amILiveAnswering", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_qa_lst[i].amILiveAnswering));
		node->Set(context, v8::String::NewFromUtf8(isolate, "liveAnswerName", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_qa_lst[i].liveAnswerName).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "isLiveAnswering", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_qa_lst[i].isLiveAnswering));

		// answerList
		v8::Local<v8::Array> answerNodes = v8::Array::New(isolate);
		for (unsigned int j = 0; j < zn_qa_lst[i].answerList.size(); ++j) {
			v8::HandleScope scope(isolate);
			v8::Local<v8::Object> item = v8::Object::New(isolate);
			item->Set(context, v8::String::NewFromUtf8(isolate, "timeStamp", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, zn_qa_lst[i].answerList[j].timeStamp));
			item->Set(context, v8::String::NewFromUtf8(isolate, "text", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_qa_lst[i].answerList[j].text).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
			item->Set(context, v8::String::NewFromUtf8(isolate, "senderName", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_qa_lst[i].answerList[j].senderName).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
			item->Set(context, v8::String::NewFromUtf8(isolate, "questionID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_qa_lst[i].answerList[j].questionID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
			item->Set(context, v8::String::NewFromUtf8(isolate, "answerID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(zn_qa_lst[i].answerList[j].answerID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
			item->Set(context, v8::String::NewFromUtf8(isolate, "isPrivate", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_qa_lst[i].answerList[j].isPrivate));
			item->Set(context, v8::String::NewFromUtf8(isolate, "isLiveAnswer", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_qa_lst[i].answerList[j].isLiveAnswer));
			item->Set(context, v8::String::NewFromUtf8(isolate, "isSenderMyself", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_qa_lst[i].answerList[j].isSenderMyself));

			answerNodes->Set(context, j, item);
		}

		v8::Local<v8::Array> final_node = v8::Array::New(isolate);

		final_node->Set(context, 0, node);
		final_node->Set(context, 1, answerNodes);

		all_question_nodes->Set(context, i, final_node);
	}
	args.GetReturnValue().Set(all_question_nodes);
}

void ZoomNodeMeetingQACtrlWrap::AnswerQuestionPublicWithQuestionID(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;

	do {
		com::electron::sdk::proto::AnswerQuestionParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::AnswerQuestionParams >(args, proto_params)) {
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_qid() || !proto_params.has_content()) {
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		
		ZoomSTRING _zn_questionID;
		ZoomSTRING _zn_content;

		_zn_questionID = s2zs(proto_params.qid());
		_zn_content = s2zs(proto_params.content());

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingQACtrl().answerQuestionPublicWithQuestionID(_zn_questionID, _zn_content);

	} while (false);
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodeMeetingQACtrlWrap::AnswerQuestionPrivateWithQuestionID(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;

	do {
		com::electron::sdk::proto::AnswerQuestionParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::AnswerQuestionParams >(args, proto_params)) {
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_qid() || !proto_params.has_content()) {
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		
		ZoomSTRING _zn_questionID;
		ZoomSTRING _zn_content;

		_zn_questionID = s2zs(proto_params.qid());
		_zn_content = s2zs(proto_params.content());

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingQACtrl().answerQuestionPrivateWithQuestionID(_zn_questionID, _zn_content);

	} while (false);
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodeMeetingQACtrlWrap::StartLiving(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::QuestionIDParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::QuestionIDParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_qid())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		ZoomSTRING _zn_questionID;
		_zn_questionID = s2zs(proto_params.qid());

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingQACtrl().StartLiving(_zn_questionID);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodeMeetingQACtrlWrap::EndLiving(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::QuestionIDParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::QuestionIDParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_qid())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		ZoomSTRING _zn_questionID;
		_zn_questionID = s2zs(proto_params.qid());

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingQACtrl().EndLiving(_zn_questionID);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodeMeetingQACtrlWrap::DismissQuestionWithQuestionID(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::QuestionIDParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::QuestionIDParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_qid())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		ZoomSTRING _zn_questionID;
		_zn_questionID = s2zs(proto_params.qid());

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingQACtrl().DismissQuestionWithQuestionID(_zn_questionID);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodeMeetingQACtrlWrap::DeleteQuestion(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::QuestionIDParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::QuestionIDParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_qid())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		ZoomSTRING _zn_questionID;
		_zn_questionID = s2zs(proto_params.qid());

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingQACtrl().DeleteQuestion(_zn_questionID);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodeMeetingQACtrlWrap::DeleteAnswer(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::AnswerIDParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::AnswerIDParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_answerid())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		ZoomSTRING _zn_answerID;
		_zn_answerID = s2zs(proto_params.answerid());

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingQACtrl().DeleteAnswer(_zn_answerID);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodeMeetingQACtrlWrap::ReopenQuestionWithQuestionID(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		com::electron::sdk::proto::QuestionIDParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::QuestionIDParams >(args, proto_params))
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_qid())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		ZoomSTRING _zn_questionID;
		_zn_questionID = s2zs(proto_params.qid());

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingQACtrl().ReopenQuestionWithQuestionID(_zn_questionID);
	} while (false);
	
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodeMeetingQACtrlWrap::VoteupQuestionWithQuestionID(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;

	do {
		com::electron::sdk::proto::VoteQuestionParams proto_params;
		if (!SetProtoParam<com::electron::sdk::proto::VoteQuestionParams >(args, proto_params)) {
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (!proto_params.has_qid()) {
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		
		ZoomSTRING _zn_questionID;
		bool enabled;

		_zn_questionID = s2zs(proto_params.qid());
		enabled = proto_params.enabled();

		err = _g_native_wrap.GetMeetingServiceWrap().GetMeetingQACtrl().VoteupQuestionWithQuestionID(_zn_questionID, enabled);

	} while (false);
	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);

}


void ZoomNodeMeetingQACtrlWrap::SetOnAddQuestionCB(const v8::FunctionCallbackInfo<v8::Value>& args){
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
			ZoomNodeSinkHelper::GetInst().onAddQuestion.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onAddQuestion.Reset(isolate, cbfunc);

	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodeMeetingQACtrlWrap::SetOnReceiveQuestionCB(const v8::FunctionCallbackInfo<v8::Value>& args){
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
			ZoomNodeSinkHelper::GetInst().onReceiveQuestion.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onReceiveQuestion.Reset(isolate, cbfunc);

	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodeMeetingQACtrlWrap::SetOnAddAnswerCB(const v8::FunctionCallbackInfo<v8::Value>& args){
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
			ZoomNodeSinkHelper::GetInst().onAddAnswer.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onAddAnswer.Reset(isolate, cbfunc);

	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodeMeetingQACtrlWrap::SetOnDeleteQuestionsCB(const v8::FunctionCallbackInfo<v8::Value>& args){
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
			ZoomNodeSinkHelper::GetInst().onDeleteQuestions.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onDeleteQuestions.Reset(isolate, cbfunc);

	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodeMeetingQACtrlWrap::SetOnQuestionMarkedAsDismissedCB(const v8::FunctionCallbackInfo<v8::Value>& args){
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
			ZoomNodeSinkHelper::GetInst().onQuestionMarkedAsDismissed.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onQuestionMarkedAsDismissed.Reset(isolate, cbfunc);

	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodeMeetingQACtrlWrap::SetOnReopenQuestionCB(const v8::FunctionCallbackInfo<v8::Value>& args){
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
			ZoomNodeSinkHelper::GetInst().onReopenQuestion.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onReopenQuestion.Reset(isolate, cbfunc);

	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodeMeetingQACtrlWrap::SetOnUserLivingReplyCB(const v8::FunctionCallbackInfo<v8::Value>& args){
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
			ZoomNodeSinkHelper::GetInst().onUserLivingReply.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onUserLivingReply.Reset(isolate, cbfunc);

	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodeMeetingQACtrlWrap::SetOnUserEndLivingCB(const v8::FunctionCallbackInfo<v8::Value>& args){
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
			ZoomNodeSinkHelper::GetInst().onUserEndLiving.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onUserEndLiving.Reset(isolate, cbfunc);

	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodeMeetingQACtrlWrap::SetOnReceiveAnswerCB(const v8::FunctionCallbackInfo<v8::Value>& args){
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
			ZoomNodeSinkHelper::GetInst().onReceiveAnswer.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onReceiveAnswer.Reset(isolate, cbfunc);

	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodeMeetingQACtrlWrap::SetOnDeleteAnswersCB(const v8::FunctionCallbackInfo<v8::Value>& args){
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
			ZoomNodeSinkHelper::GetInst().onDeleteAnswers.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onDeleteAnswers.Reset(isolate, cbfunc);

	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodeMeetingQACtrlWrap::SetOnVoteupQuestionCB(const v8::FunctionCallbackInfo<v8::Value>& args){
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
			ZoomNodeSinkHelper::GetInst().onVoteupQuestion.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onVoteupQuestion.Reset(isolate, cbfunc);

	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodeMeetingQACtrlWrap::SetOnRevokeVoteupQuestionCB(const v8::FunctionCallbackInfo<v8::Value>& args){
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
			ZoomNodeSinkHelper::GetInst().onRevokeVoteupQuestion.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onRevokeVoteupQuestion.Reset(isolate, cbfunc);

	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}
