#ifndef _ZOOM_NODE_MEETING_CHAT_CTRL_H_
#define _ZOOM_NODE_MEETING_CHAT_CTRL_H_
#include "zoom_node_common_include.h"
#include "zoom_native_sdk_wrap_core.h"
#include "zoom_singleton_wrap_class.h"

extern ZNativeSDKWrap _g_native_wrap;

class ZOOM_NODE_HIDE ZoomNodeMeetingChatCtrlWrap :
	public ZoomWrapObject<ZoomNodeMeetingChatCtrlWrap >
{
	friend class ZoomWrapObject<ZoomNodeMeetingChatCtrlWrap >;
private:
	ZoomNodeMeetingChatCtrlWrap();
	~ZoomNodeMeetingChatCtrlWrap();
public:
	/// \brief Get the authority status to send current message. 
	/// \return If the function succeeds, the return value is a pointer to the structure of ChatStatus. For more details, see \link ChatStatus \endlink structure.
	///Otherwise failed, the return value is NULL. To get extended error information, see \link ChatStatus \endlink.
	static void GetChatStatus(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Set the chat privilege of participants.
	/// \param privilege The chat privilege of participants
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetParticipantsChatPrivilege(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Determine whether the legal notice for chat is available
	/// \return True indicates the legal notice for chat is available. Otherwise False.
	static void IsMeetingChatLegalNoticeAvailable(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Get the chat legal notices prompt.
	static void getChatLegalNoticesPrompt(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Get the chat legal notices explained.
	static void getChatLegalNoticesExplained(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Determine whether the legal notice for sharing in meeting chat is available
	/// \return True indicates the legal notice for chat is available. Otherwise False.
	static void IsShareMeetingChatLegalNoticeAvailable(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// Get the sharing in meeting chat started legal notices content.
	static void GetShareMeetingChatStartedLegalNoticeContent(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// Get the sharing in meeting chat stopped legal notices content.
	static void GetShareMeetingChatStoppedLegalNoticeContent(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Determine whether the message can be delete.
	/// \param msgID is the message id.
	/// \return True indicates the message can be delete. Otherwise False.	
	static void IsChatMessageCanBeDeleted(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// Delete chat message by message id.	
	/// \param msgID is the message id.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void DeleteChatMessage(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// Get all chat message id.	
	static void GetAllChatMessageID(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// Get chat message by message ID.	
	/// \param msgID is the message ID.
	static void GetChatMessageById(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// Get the chat message builder which can help construct the message entity.
	/// \param content chat message content.
	/// \param receiver who will receive the chat message.
	/// \param threadId the ID of the thread where the message will be posted.
	/// \param type the chat message type.
	/// \param positionStart The segment start position.
	/// \param positionEnd The segment end position.
	/// \param bClearStyles all set styles.
	/// \param bClear all set properties.
	static void SetChatMessageBuilder(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// Send a chat message.	
	static void SendLastBuildChatMsgTo(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Determine whether file transfer is enabled.
	/// \return True indicates file transfer is enabled, otherwise false.	
	static void IsFileTransferEnabled(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Send file to specify user in current session.
	/// \param filePath The absolute path of the file.
	/// \param user Send the file to this user.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	/// \remark this interface is related to chat privilege, see \link SDKChatPrivilege \endlink enum.	
	static void TransferFile(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Send file to all users in current session.
	/// \param filePath The absolute path of the file.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	/// \remark this interface is related to chat privilege, see \link SDKChatPrivilege \endlink enum.
	static void TransferFileToAll(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Get the list of allowed file types in transfer.
	/// \return The value of allowed file types in transfer, comma-separated if there are multiple values. Exe files are by default forbidden from being transferred.
	static void GetTransferFileTypeAllowList(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Get the maximum size for file transfer.
	/// \return The maximum number of bytes for file transfer.
	static void GetMaxTransferFileSizeBytes(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Get file receiver's user id.
	/// \return The receiver user id. -1 specify the internel error of get user id. 0 specify the file send to all.
	static void GetReceiver(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Cancel the file send.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	static void CancelSend(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Get file sender's user id.
	/// \return The receiver user id. -1 specify the internel error of get user id. 0 specify the file send to all.
	static void GetSender(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Cancel the file receive.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	static void CancelReceive(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Start receive the file.
	/// \param path The path to receive the file.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	static void StartReceive(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Set chat message callback.
	/// \param 1. callback(function) This function is used to inform the user once received the message sent by others.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnChatMsgNotificationCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Set the authority of chat changes callback.
	/// \param 1. callback(function) This function is used to inform the user when the authority of chat changes in the meeting or webinar.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnChatStatusChangedNotificationCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Set chat message be deleted callback.
	/// \param 1. callback(function) This function is used to inform the user host/myself the message be deleted.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnChatMsgDeleteNotificationCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Chat message be edited callback.
	/// \param chatMsg An object pointer to the chat message.
	static void SetOnChatMessageEditNotificationCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Set share meeting chat status changed callback.
	/// \param 1. callback(function) This function is used to inform the user when share meeting chat status changed.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnShareMeetingChatStatusChangedCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Invoked when start send file.
	/// \param sender The class to send file object.
	static void SetOnFileSendStartCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Invoked when receiving a file from another user.
	/// \param receiver The class to receive the file object.
	static void SetOnFileReceivedCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Invoked when send or receive file status change.
	/// \param info The class to basic transfer information.
	static void SetOnFileTransferProgressCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	
	static v8::Persistent<v8::Function> constructor;
};
template<>
void InitClassAttribute<ZoomNodeMeetingChatCtrlWrap >(const v8::Local<v8::FunctionTemplate>& tpl, v8::Isolate* isolate)
{
	tpl->SetClassName(v8::String::NewFromUtf8(
		isolate, "ZoomNodeMeetingChatCtrlWrap", v8::NewStringType::kInternalized).ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetChatStatus", ZoomNodeMeetingChatCtrlWrap::GetChatStatus);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetParticipantsChatPrivilege", ZoomNodeMeetingChatCtrlWrap::SetParticipantsChatPrivilege);
	NODE_SET_PROTOTYPE_METHOD(tpl, "IsMeetingChatLegalNoticeAvailable", ZoomNodeMeetingChatCtrlWrap::IsMeetingChatLegalNoticeAvailable);
	NODE_SET_PROTOTYPE_METHOD(tpl, "getChatLegalNoticesPrompt", ZoomNodeMeetingChatCtrlWrap::getChatLegalNoticesPrompt);
	NODE_SET_PROTOTYPE_METHOD(tpl, "getChatLegalNoticesExplained", ZoomNodeMeetingChatCtrlWrap::getChatLegalNoticesExplained);
	NODE_SET_PROTOTYPE_METHOD(tpl, "IsShareMeetingChatLegalNoticeAvailable", ZoomNodeMeetingChatCtrlWrap::IsShareMeetingChatLegalNoticeAvailable);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetShareMeetingChatStartedLegalNoticeContent", ZoomNodeMeetingChatCtrlWrap::GetShareMeetingChatStartedLegalNoticeContent);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetShareMeetingChatStoppedLegalNoticeContent", ZoomNodeMeetingChatCtrlWrap::GetShareMeetingChatStoppedLegalNoticeContent);
	NODE_SET_PROTOTYPE_METHOD(tpl, "IsChatMessageCanBeDeleted", ZoomNodeMeetingChatCtrlWrap::IsChatMessageCanBeDeleted);
	NODE_SET_PROTOTYPE_METHOD(tpl, "DeleteChatMessage", ZoomNodeMeetingChatCtrlWrap::DeleteChatMessage);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetAllChatMessageID", ZoomNodeMeetingChatCtrlWrap::GetAllChatMessageID);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetChatMessageById", ZoomNodeMeetingChatCtrlWrap::GetChatMessageById);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetChatMessageBuilder", ZoomNodeMeetingChatCtrlWrap::SetChatMessageBuilder);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SendLastBuildChatMsgTo", ZoomNodeMeetingChatCtrlWrap::SendLastBuildChatMsgTo);
	NODE_SET_PROTOTYPE_METHOD(tpl, "IsFileTransferEnabled", ZoomNodeMeetingChatCtrlWrap::IsFileTransferEnabled);
	NODE_SET_PROTOTYPE_METHOD(tpl, "TransferFile", ZoomNodeMeetingChatCtrlWrap::TransferFile);
	NODE_SET_PROTOTYPE_METHOD(tpl, "TransferFileToAll", ZoomNodeMeetingChatCtrlWrap::TransferFileToAll);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetTransferFileTypeAllowList", ZoomNodeMeetingChatCtrlWrap::GetTransferFileTypeAllowList);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetMaxTransferFileSizeBytes", ZoomNodeMeetingChatCtrlWrap::GetMaxTransferFileSizeBytes);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetReceiver", ZoomNodeMeetingChatCtrlWrap::GetReceiver);
	NODE_SET_PROTOTYPE_METHOD(tpl, "CancelSend", ZoomNodeMeetingChatCtrlWrap::CancelSend);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetSender", ZoomNodeMeetingChatCtrlWrap::GetSender);
	NODE_SET_PROTOTYPE_METHOD(tpl, "CancelReceive", ZoomNodeMeetingChatCtrlWrap::CancelReceive);
	NODE_SET_PROTOTYPE_METHOD(tpl, "StartReceive", ZoomNodeMeetingChatCtrlWrap::StartReceive);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnChatMsgNotificationCB", ZoomNodeMeetingChatCtrlWrap::SetOnChatMsgNotificationCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnChatStatusChangedNotificationCB", ZoomNodeMeetingChatCtrlWrap::SetOnChatStatusChangedNotificationCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnChatMsgDeleteNotificationCB", ZoomNodeMeetingChatCtrlWrap::SetOnChatMsgDeleteNotificationCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnChatMessageEditNotificationCB", ZoomNodeMeetingChatCtrlWrap::SetOnChatMessageEditNotificationCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnShareMeetingChatStatusChangedCB", ZoomNodeMeetingChatCtrlWrap::SetOnShareMeetingChatStatusChangedCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnFileSendStartCB", ZoomNodeMeetingChatCtrlWrap::SetOnFileSendStartCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnFileReceivedCB", ZoomNodeMeetingChatCtrlWrap::SetOnFileReceivedCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnFileTransferProgressCB", ZoomNodeMeetingChatCtrlWrap::SetOnFileTransferProgressCB);
}
template<>
v8::Persistent<v8::Function>* GetConstructor<ZoomNodeMeetingChatCtrlWrap >() {
	return &ZoomNodeMeetingChatCtrlWrap::constructor;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif
