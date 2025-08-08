#pragma once
#include "zoom_native_sdk_wrap_core_def.h"
#include "zoom_sinks_wrap_class.h"

class ZMeetingChatWrap
{
public:
	ZMeetingChatWrap();
	virtual ~ZMeetingChatWrap();
	void Init();
	void Uninit();
	void SetSink(ZNativeSDKMeetingChatWrapSink* pSink);

	ZNChatStatus GetChatStatus();
	ZNSDKError SetParticipantsChatPrivilege(ZNSDKChatPrivilege privilege);
	bool IsMeetingChatLegalNoticeAvailable();
	ZoomSTRING getChatLegalNoticesPrompt();
	ZoomSTRING getChatLegalNoticesExplained();
	bool IsShareMeetingChatLegalNoticeAvailable();
	ZoomSTRING GetShareMeetingChatStartedLegalNoticeContent();
	ZoomSTRING GetShareMeetingChatStoppedLegalNoticeContent();
	bool IsChatMessageCanBeDeleted(ZoomSTRING msgID);
	ZNSDKError DeleteChatMessage(ZoomSTRING msgID);
	ZNList<ZoomSTRING> GetAllChatMessageID();
	ZNChatMsgInfo GetChatMessageById(ZoomSTRING msgID);
	ZNSDKError SetChatMessageBuilder(ZoomSTRING content, unsigned int receiver, ZoomSTRING threadId, ZNSDKChatMessageType type, unsigned int positionStart, unsigned int positionEnd, bool bClearStyles, bool bClear);
	ZNSDKError SendLastBuildChatMsgTo();
	bool IsFileTransferEnabled();
	ZNSDKError TransferFile(ZoomSTRING filePath, unsigned int userid);
	ZNSDKError TransferFileToAll(ZoomSTRING filePath);
	ZoomSTRING GetTransferFileTypeAllowList();
	unsigned long long GetMaxTransferFileSizeBytes();

	unsigned int GetReceiver();
	ZNSDKError CancelSend();

	unsigned int GetSender();
	ZNSDKError CancelReceive();
	ZNSDKError StartReceive(ZoomSTRING path);

	//callback
	void onChatMsgNotification(ZNChatMsgInfo chatMsg, ZoomSTRING content);
	void onChatStatusChangedNotification(ZNChatStatus status);
	void onChatMsgDeleteNotification(ZoomSTRING msgID, ZNSDKChatMessageDeleteType deleteBy);
	void onShareMeetingChatStatusChanged(bool isStart);
	void onFileSendStart(ZNSDKFileTransferInfo senderInfo);
	void onFileReceived(ZNSDKFileTransferInfo receiverInfo);
	void onFileTransferProgress(ZNSDKFileTransferInfo transferInfo);
	void onChatMessageEditNotification(ZNChatMsgInfo chatMsg);
	
private:
	ZNativeSDKMeetingChatWrapSink* m_pSink;
};