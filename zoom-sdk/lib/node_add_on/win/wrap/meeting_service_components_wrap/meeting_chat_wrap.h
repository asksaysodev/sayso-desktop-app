#pragma once
#include "common_include.h"
BEGIN_ZOOM_SDK_NAMESPACE
class IMeetingServiceWrap;
IMeetingChatController* InitIMeetingChatControllerFunc(IMeetingChatCtrlEvent* pEvent, IMeetingServiceWrap* pOwner);
void UninitIMeetingChatControllerFunc(IMeetingChatController* obj);
BEGIN_CLASS_DEFINE_WITHCALLBACK(IMeetingChatController, IMeetingChatCtrlEvent)
NORMAL_CLASS(IMeetingChatController)
INIT_UNINIT_WITHEVENT_AND_OWNSERVICE(IMeetingChatController, IMeetingServiceWrap)
virtual SDKError SetEvent(IMeetingChatCtrlEvent* pEvent)
{
	external_cb = pEvent;
	return SDKERR_SUCCESS;
}

//virtual const ChatStatus* GetChatStatus() = 0;
DEFINE_FUNC_0(GetChatStatus, const ChatStatus*)
//virtual bool IsMeetingChatLegalNoticeAvailable() = 0;
DEFINE_FUNC_0(IsMeetingChatLegalNoticeAvailable, bool)
//virtual const zchar_t* getChatLegalNoticesPrompt() = 0;
DEFINE_FUNC_0(getChatLegalNoticesPrompt, const zchar_t*)
//virtual const zchar_t* getChatLegalNoticesExplained() = 0;
DEFINE_FUNC_0(getChatLegalNoticesExplained, const zchar_t*)
//virtual SDKError SetParticipantsChatPrivilege(SDKChatPrivilege privilege) = 0;
DEFINE_FUNC_1(SetParticipantsChatPrivilege, SDKError, SDKChatPrivilege, privilege)
//virtual bool IsChatMessageCanBeDeleted(const zchar_t* msgID) = 0;
DEFINE_FUNC_1(IsChatMessageCanBeDeleted, bool, const zchar_t*, msgID)
//virtual SDKError DeleteChatMessage(const zchar_t* msgID) = 0;
DEFINE_FUNC_1(DeleteChatMessage, SDKError, const zchar_t*, msgID)
//virtual IList<const zchar_t*>* GetAllChatMessageID() = 0;
DEFINE_FUNC_0(GetAllChatMessageID, IList<const zchar_t*>*)
//virtual bool IsShareMeetingChatLegalNoticeAvailable() = 0;
DEFINE_FUNC_0(IsShareMeetingChatLegalNoticeAvailable, bool)
//virtual const zchar_t* GetShareMeetingChatStartedLegalNoticeContent() = 0;
DEFINE_FUNC_0(GetShareMeetingChatStartedLegalNoticeContent, const zchar_t*)
//virtual const zchar_t* GetShareMeetingChatStoppedLegalNoticeContent() = 0;
DEFINE_FUNC_0(GetShareMeetingChatStoppedLegalNoticeContent, const zchar_t*)
//virtual IChatMsgInfo* GetChatMessageById(const zchar_t* msgID) = 0;
DEFINE_FUNC_1(GetChatMessageById, IChatMsgInfo*, const zchar_t*, msgID)
//virtual IChatMsgInfoBuilder* GetChatMessageBuilder() = 0;
DEFINE_FUNC_0(GetChatMessageBuilder, IChatMsgInfoBuilder*)
//virtual SDKError SendChatMsgTo(IChatMsgInfo* msg) = 0;
DEFINE_FUNC_1(SendChatMsgTo, SDKError, IChatMsgInfo*, msg)
//virtual bool IsFileTransferEnabled() = 0;
DEFINE_FUNC_0(IsFileTransferEnabled, bool)
//virtual SDKError TransferFile(const zchar_t* filePath, unsigned int userid) = 0;
DEFINE_FUNC_2(TransferFile, SDKError, const zchar_t*, filePath, unsigned int, userid)
//virtual SDKError TransferFileToAll(const zchar_t* filePath) = 0;
DEFINE_FUNC_1(TransferFileToAll, SDKError, const zchar_t*, filePath)
//virtual const zchar_t* GetTransferFileTypeAllowList() = 0;
DEFINE_FUNC_0(GetTransferFileTypeAllowList, const zchar_t*)
//virtual unsigned long long GetMaxTransferFileSizeBytes() = 0;
DEFINE_FUNC_0(GetMaxTransferFileSizeBytes, unsigned long long)
 
//virtual void onChatMsgNotification(IChatMsgInfo* chatMsg, const zchar_t* content = NULL) = 0;
CallBack_FUNC_2(onChatMsgNotification, IChatMsgInfo*, chatMsg, const zchar_t*, content)
//virtual void onChatStatusChangedNotification(ChatStatus* status_) = 0;
CallBack_FUNC_1(onChatStatusChangedNotification, ChatStatus*, status_)
//virtual void onChatMsgDeleteNotification(const zchar_t* msgID, SDKChatMessageDeleteType deleteBy) = 0;
CallBack_FUNC_2(onChatMsgDeleteNotification, const zchar_t*, msgID, SDKChatMessageDeleteType, deleteBy)
//virtual void onShareMeetingChatStatusChanged(bool isStart) = 0;
CallBack_FUNC_1(onShareMeetingChatStatusChanged, bool, isStart)
//virtual void onFileSendStart(ISDKFileSender* sender) = 0;
CallBack_FUNC_1(onFileSendStart, ISDKFileSender*, sender)
//virtual void onFileReceived(ISDKFileReceiver* receiver) = 0;
CallBack_FUNC_1(onFileReceived, ISDKFileReceiver*, receiver)
//virtual void onFileTransferProgress(SDKFileTransferInfo* info) = 0;
CallBack_FUNC_1(onFileTransferProgress, SDKFileTransferInfo*, info)
//virtual void onChatMessageEditNotification(IChatMsgInfo* chatMsg) = 0;
CallBack_FUNC_1(onChatMessageEditNotification, IChatMsgInfo*, chatMsg)

END_CLASS_DEFINE(IMeetingChatController)
END_ZOOM_SDK_NAMESPACE