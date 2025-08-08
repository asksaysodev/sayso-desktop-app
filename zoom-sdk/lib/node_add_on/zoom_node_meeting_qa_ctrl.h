#ifndef _ZOOM_NODE_MEETING_QA_CTRL_H_
#define _ZOOM_NODE_MEETING_QA_CTRL_H_
#include "zoom_node_common_include.h"
#include "zoom_native_sdk_wrap_core.h"
#include "zoom_singleton_wrap_class.h"

extern ZNativeSDKWrap _g_native_wrap;

class ZOOM_NODE_HIDE ZoomNodeMeetingQACtrlWrap :
	public ZoomWrapObject<ZoomNodeMeetingQACtrlWrap >
{
	friend class ZoomWrapObject<ZoomNodeMeetingQACtrlWrap >;
private:
	ZoomNodeMeetingQACtrlWrap();
	~ZoomNodeMeetingQACtrlWrap();
public:
	/// @brief Get question object by question ID.
	/// \param questionID is the question ID
	static void GetQuestion(const v8::FunctionCallbackInfo<v8::Value>& args);

	/// @brief Get answer object by answer ID.
	/// \param answerID is the answer ID
	static void GetAnswer(const v8::FunctionCallbackInfo<v8::Value>& args);

	/// @brief Get all question list.
	static void GetAllQuestionList(const v8::FunctionCallbackInfo<v8::Value>& args);

	/// @brief Answer questions in public.
	/// \param questionID is the question ID
	/// \param content is the answer content
	static void AnswerQuestionPublicWithQuestionID(const v8::FunctionCallbackInfo<v8::Value>& args);

	/// @brief Answer questions in private.
	/// \param questionID is the question ID
	/// \param content is the answer content
	static void AnswerQuestionPrivateWithQuestionID(const v8::FunctionCallbackInfo<v8::Value>& args);

	static void StartLiving(const v8::FunctionCallbackInfo<v8::Value>& args);

	static void EndLiving(const v8::FunctionCallbackInfo<v8::Value>& args);

	static void DismissQuestionWithQuestionID(const v8::FunctionCallbackInfo<v8::Value>& args);

	static void DeleteQuestion(const v8::FunctionCallbackInfo<v8::Value>& args);

	static void DeleteAnswer(const v8::FunctionCallbackInfo<v8::Value>& args);

	static void ReopenQuestionWithQuestionID(const v8::FunctionCallbackInfo<v8::Value>& args);

	static void VoteupQuestionWithQuestionID(const v8::FunctionCallbackInfo<v8::Value>& args);



	/// \brief Set the callback event when a question is added
	/// \param 1. callback(function)  Callback questions added.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnAddQuestionCB(const v8::FunctionCallbackInfo<v8::Value>& args);

	/// \brief Set the callback event when a question is received
	/// \param 1. callback(function)  Callback receive question.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnReceiveQuestionCB(const v8::FunctionCallbackInfo<v8::Value>& args);

	/// \brief Set the callback event when a question is deleted
	/// \param 1. callback(function)  Callback delete question.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnDeleteQuestionsCB(const v8::FunctionCallbackInfo<v8::Value>& args);

	/// \brief Set the callback event when a question is marked as dismissed
	/// \param 1. callback(function)  Callback dismissed question.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnQuestionMarkedAsDismissedCB(const v8::FunctionCallbackInfo<v8::Value>& args);

	/// \brief Set the callback event when a question is reopened
	/// \param 1. callback(function)  Callback reopened question.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnReopenQuestionCB(const v8::FunctionCallbackInfo<v8::Value>& args);

	/// \brief Set the callback event when a question is marked as answering live
	/// \param 1. callback(function)  Callback answered live question.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnUserLivingReplyCB(const v8::FunctionCallbackInfo<v8::Value>& args);

	/// \brief Set the callback event when a question is marked as answering live ended
	/// \param 1. callback(function)  Callback answered live question ended.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnUserEndLivingCB(const v8::FunctionCallbackInfo<v8::Value>& args);

	/// \brief Set the callback event when an answer is received
	/// \param 1. callback(function)  Callback answered receive.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnReceiveAnswerCB(const v8::FunctionCallbackInfo<v8::Value>& args);

	/// \brief Set the callback event when an answer is added
	/// \param 1. callback(function)  Callback answer added.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnAddAnswerCB(const v8::FunctionCallbackInfo<v8::Value>& args);

	/// \brief Set the callback event when an answer is deleted
	/// \param 1. callback(function)  Callback delete answer.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnDeleteAnswersCB(const v8::FunctionCallbackInfo<v8::Value>& args);

	/// \brief Set the callback event when a question is voted
	/// \param 1. callback(function)  Callback question voted.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnVoteupQuestionCB(const v8::FunctionCallbackInfo<v8::Value>& args);

	/// \brief Set the callback event when a question vote is revoked
	/// \param 1. callback(function)  Callback question vote revoked.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnRevokeVoteupQuestionCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	
	
	static v8::Persistent<v8::Function> constructor;
};
template<>
static void InitClassAttribute<ZoomNodeMeetingQACtrlWrap >(const v8::Local<v8::FunctionTemplate>& tpl, v8::Isolate* isolate)
{
	tpl->SetClassName(v8::String::NewFromUtf8(
		isolate, "ZoomNodeMeetingQACtrlWrap", v8::NewStringType::kInternalized).ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnAddQuestionCB", ZoomNodeMeetingQACtrlWrap::SetOnAddQuestionCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnReceiveQuestionCB", ZoomNodeMeetingQACtrlWrap::SetOnReceiveQuestionCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnDeleteQuestionsCB", ZoomNodeMeetingQACtrlWrap::SetOnDeleteQuestionsCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnQuestionMarkedAsDismissedCB", ZoomNodeMeetingQACtrlWrap::SetOnQuestionMarkedAsDismissedCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnReopenQuestionCB", ZoomNodeMeetingQACtrlWrap::SetOnReopenQuestionCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnUserLivingReplyCB", ZoomNodeMeetingQACtrlWrap::SetOnUserLivingReplyCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnUserEndLivingCB", ZoomNodeMeetingQACtrlWrap::SetOnUserEndLivingCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnAddAnswerCB", ZoomNodeMeetingQACtrlWrap::SetOnAddAnswerCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnReceiveAnswerCB", ZoomNodeMeetingQACtrlWrap::SetOnReceiveAnswerCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnDeleteAnswersCB", ZoomNodeMeetingQACtrlWrap::SetOnDeleteAnswersCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnVoteupQuestionCB", ZoomNodeMeetingQACtrlWrap::SetOnVoteupQuestionCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnRevokeVoteupQuestionCB", ZoomNodeMeetingQACtrlWrap::SetOnRevokeVoteupQuestionCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetQuestion", ZoomNodeMeetingQACtrlWrap::GetQuestion);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetAnswer", ZoomNodeMeetingQACtrlWrap::GetAnswer);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetAllQuestionList", ZoomNodeMeetingQACtrlWrap::GetAllQuestionList);
	NODE_SET_PROTOTYPE_METHOD(tpl, "AnswerQuestionPublicWithQuestionID", ZoomNodeMeetingQACtrlWrap::AnswerQuestionPublicWithQuestionID);
	NODE_SET_PROTOTYPE_METHOD(tpl, "AnswerQuestionPrivateWithQuestionID", ZoomNodeMeetingQACtrlWrap::AnswerQuestionPrivateWithQuestionID);	
	NODE_SET_PROTOTYPE_METHOD(tpl, "StartLiving", ZoomNodeMeetingQACtrlWrap::StartLiving);
	NODE_SET_PROTOTYPE_METHOD(tpl, "EndLiving", ZoomNodeMeetingQACtrlWrap::EndLiving);
	NODE_SET_PROTOTYPE_METHOD(tpl, "DismissQuestionWithQuestionID", ZoomNodeMeetingQACtrlWrap::DismissQuestionWithQuestionID);
	NODE_SET_PROTOTYPE_METHOD(tpl, "DeleteQuestion", ZoomNodeMeetingQACtrlWrap::DeleteQuestion);
	NODE_SET_PROTOTYPE_METHOD(tpl, "DeleteAnswer", ZoomNodeMeetingQACtrlWrap::DeleteAnswer);
	NODE_SET_PROTOTYPE_METHOD(tpl, "ReopenQuestionWithQuestionID", ZoomNodeMeetingQACtrlWrap::ReopenQuestionWithQuestionID);
	NODE_SET_PROTOTYPE_METHOD(tpl, "VoteupQuestionWithQuestionID", ZoomNodeMeetingQACtrlWrap::VoteupQuestionWithQuestionID);


}
template<>
static v8::Persistent<v8::Function>* GetConstructor<ZoomNodeMeetingQACtrlWrap >() {
	return &ZoomNodeMeetingQACtrlWrap::constructor;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif
