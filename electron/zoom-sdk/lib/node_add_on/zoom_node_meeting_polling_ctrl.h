#ifndef _ZOOM_NODE_MEETING_POLLING_CTRL_H_
#define _ZOOM_NODE_MEETING_POLLING_CTRL_H_
#include "zoom_node_common_include.h"
#include "zoom_native_sdk_wrap_core.h"
#include "zoom_singleton_wrap_class.h"

extern ZNativeSDKWrap _g_native_wrap;

class ZOOM_NODE_HIDE ZoomNodePollingCtrlWrap :
	public ZoomWrapObject<ZoomNodePollingCtrlWrap >
{
	friend class ZoomWrapObject<ZoomNodePollingCtrlWrap >;
private:
	ZoomNodePollingCtrlWrap();
	~ZoomNodePollingCtrlWrap();
public:
	/// @brief Determine whether the current meeting can do the polling.
	static void CanDoPolling (const v8::FunctionCallbackInfo<v8::Value>& args);

	/// @brief Determine if the user can get the right answer list.
	static void CanGetRightAnswerItemList (const v8::FunctionCallbackInfo<v8::Value>& args);

	/// @brief Get the list of poll's question.
	/// \param pollingID is the polling ID
	static void GetPollingQuestionItemList (const v8::FunctionCallbackInfo<v8::Value>& args);

	/// @brief Get the list of polling question or subquestion's right answer.
	/// \param pollingID is the polling ID
	static void GetPollingRightAnswerItemList (const v8::FunctionCallbackInfo<v8::Value>& args);

	/// @brief Get the list of polling answer result item.
	/// \param pollingID is the polling ID
	static void GetPollingAnswerResultItemList (const v8::FunctionCallbackInfo<v8::Value>& args);

	/// @brief Get polling object by polling ID.
	/// \param pollingID is the polling ID
	static void GetPollingItemByID (const v8::FunctionCallbackInfo<v8::Value>& args);

	/// @brief Get the list of poll item.
	static void GetPollingItemList (const v8::FunctionCallbackInfo<v8::Value>& args);

	/// @brief Start the polling.
	/// \param pollingID is the polling ID
	static void StartPolling (const v8::FunctionCallbackInfo<v8::Value>& args);

	/// @brief Stop the polling.
	/// \param pollingID is the polling ID
	static void StopPolling (const v8::FunctionCallbackInfo<v8::Value>& args);

	/// @brief Restart the polling.
	/// \param pollingID is the polling ID
	static void RestartPolling (const v8::FunctionCallbackInfo<v8::Value>& args);

	/// @brief Determine if the question is case sensitive.
	/// \param pollingID is the polling ID
	/// \param questionID is the question's question ID
	static void IsQuestionCaseSensitive (const v8::FunctionCallbackInfo<v8::Value>& args);

	/// @brief Get the polling question's image path.
	/// \param pollingID is the answer's polling ID
	/// \param questionID is the answer's question ID
	static void GetPollingQuestionImagePath (const v8::FunctionCallbackInfo<v8::Value>& args);

	/// \brief Set the callback event when polling status changes
	/// \param 1. callback(function)  Callback polling status changed.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnPollingStatusChangedCB(const v8::FunctionCallbackInfo<v8::Value>& args);

	/// \brief Set the callback event when polling results updates
	/// \param 1. callback(function)  Callback polling results updated.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnPollingResultUpdatedCB(const v8::FunctionCallbackInfo<v8::Value>& args);

	/// \brief Set the callback event when a poll updates
	/// \param 1. callback(function)  Callback poll updated.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnPollingListUpdatedCB(const v8::FunctionCallbackInfo<v8::Value>& args);

	/// \brief Set the callback event when a a user perform an action for a poll
	/// \param 1. callback(function)  Callback poll action performed.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnPollingActionResultCB(const v8::FunctionCallbackInfo<v8::Value>& args);

	static v8::Persistent<v8::Function> constructor;
};
template<>
static void InitClassAttribute<ZoomNodePollingCtrlWrap >(const v8::Local<v8::FunctionTemplate>& tpl, v8::Isolate* isolate)
{
	tpl->SetClassName(v8::String::NewFromUtf8(
		isolate, "ZoomNodePollingCtrlWrap", v8::NewStringType::kInternalized).ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	NODE_SET_PROTOTYPE_METHOD(tpl, "CanGetRightAnswerItemList", ZoomNodePollingCtrlWrap::CanGetRightAnswerItemList);
	
	NODE_SET_PROTOTYPE_METHOD(tpl, "CanDoPolling", ZoomNodePollingCtrlWrap::CanDoPolling);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetPollingQuestionItemList", ZoomNodePollingCtrlWrap::GetPollingQuestionItemList);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetPollingRightAnswerItemList", ZoomNodePollingCtrlWrap::GetPollingRightAnswerItemList);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetPollingAnswerResultItemList", ZoomNodePollingCtrlWrap::GetPollingAnswerResultItemList);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetPollingItemByID", ZoomNodePollingCtrlWrap::GetPollingItemByID);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetPollingItemList", ZoomNodePollingCtrlWrap::GetPollingItemList);
	NODE_SET_PROTOTYPE_METHOD(tpl, "StartPolling", ZoomNodePollingCtrlWrap::StartPolling);
	NODE_SET_PROTOTYPE_METHOD(tpl, "StopPolling", ZoomNodePollingCtrlWrap::StopPolling);
	NODE_SET_PROTOTYPE_METHOD(tpl, "RestartPolling", ZoomNodePollingCtrlWrap::RestartPolling);
	NODE_SET_PROTOTYPE_METHOD(tpl, "IsQuestionCaseSensitive", ZoomNodePollingCtrlWrap::IsQuestionCaseSensitive);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetPollingQuestionImagePath", ZoomNodePollingCtrlWrap::GetPollingQuestionImagePath);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnPollingStatusChangedCB", ZoomNodePollingCtrlWrap::SetOnPollingStatusChangedCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnPollingResultUpdatedCB", ZoomNodePollingCtrlWrap::SetOnPollingResultUpdatedCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnPollingListUpdatedCB", ZoomNodePollingCtrlWrap::SetOnPollingListUpdatedCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnPollingActionResultCB", ZoomNodePollingCtrlWrap::SetOnPollingActionResultCB);
}
template<>
static v8::Persistent<v8::Function>* GetConstructor<ZoomNodePollingCtrlWrap >() {
	return &ZoomNodePollingCtrlWrap::constructor;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif