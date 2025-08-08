#include "Header_include.h"
#include "sdk_native_error.h"
#include "meeting_chat_wrap_core.h"
#include "meeting_service_wrap_core.h"
#import "meetingServiceDelegate.h"
ZMeetingChatWrap &ZMeetingServiceWrap::GetMeetingChatCtrl()
{
    return m_meeting_chat_ctrl;
}
ZMeetingChatWrap::ZMeetingChatWrap()
{
   m_pSink = 0;
}

ZMeetingChatWrap::~ZMeetingChatWrap()
{
    m_pSink = 0;
    [[[[ZoomSDK sharedSDK] getMeetingService] getMeetingChatController] setDelegate:nil];
}

void ZMeetingChatWrap::Init()
{

}
void ZMeetingChatWrap::Uninit()
{
    
}

ZoomSDKChatInfo* lastBuildChatMsg;

void ZMeetingChatWrap::SetSink(ZNativeSDKMeetingChatWrapSink *pSink)
{
    [[[[ZoomSDK sharedSDK] getMeetingService] getMeetingChatController] setDelegate:[meetingServiceChatDelegate share]];
    m_pSink= pSink;
}

ZNChatStatus ZMeetingChatWrap::GetChatStatus()
{
    ZNChatStatus zn_chat_status;
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return zn_chat_status;
  
    ZoomSDKMeetingActionController *actionController = [service getMeetingActionController];
    if (!actionController)
        return zn_chat_status;

    ZoomSDKChatStatus* status = [actionController getChatStatus];
    if (status)
    {
        zn_chat_status.is_chat_off = !status.isSupportChat;
        zn_chat_status.is_webinar_attendee = status.isWebinarAttendee;
        zn_chat_status.is_webinar_meeting = status.isWebinarMeeting;
        if (status.isWebinarMeeting)
        {
            if (status.isWebinarAttendee) 
            {
                zn_chat_status.ut.webinar_attendee_status.can_chat = status.getWebinarAttendeePrivilege.canChat;
                zn_chat_status.ut.webinar_attendee_status.can_chat_to_all_panellist_and_attendee = status.getWebinarAttendeePrivilege.canChatToAllPanellistAndAttendee;
                zn_chat_status.ut.webinar_attendee_status.can_chat_to_all_panellist = status.getWebinarAttendeePrivilege.canChatToAllPanellist;
            } else {
                zn_chat_status.ut.webinar_other_status.can_chat_to_all_panellist = status.getWebinarPanelistPrivilege.canChatToAllPanellist;
                zn_chat_status.ut.webinar_other_status.can_chat_to_all_panellist_and_attendee = status.getWebinarPanelistPrivilege.canChatToAllPanellistAndAttendee;
                zn_chat_status.ut.webinar_other_status.can_chat_to_individual = status.getWebinarPanelistPrivilege.canChatToIndividual;
            }
        } else {
            zn_chat_status.ut.normal_meeting_status.can_chat = status.getNormalMeetingPrivilege.canChat;
            zn_chat_status.ut.normal_meeting_status.can_chat_to_all = status.getNormalMeetingPrivilege.canChatToAll;
            zn_chat_status.ut.normal_meeting_status.can_chat_to_individual = status.getNormalMeetingPrivilege.canChatToIndividual;
            zn_chat_status.ut.normal_meeting_status.is_only_can_chat_to_host = status.getNormalMeetingPrivilege.isOnlyCanChatToHost;
        }
    } 
    return zn_chat_status;
}

ZNSDKError ZMeetingChatWrap::SetParticipantsChatPrivilege(ZNSDKChatPrivilege zn_privilege)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
  
    ZoomSDKMeetingActionController *actionController = [service getMeetingActionController];
    if (!actionController)
        return ZNSDKERR_SERVICE_FAILED;

    nativeErrorTypeHelp Help_type;
    ZoomSDKChatPrivilegeType privilege = Help_type.ZoomSDKChatPrivilegeTypeMap(zn_privilege);
    ZoomSDKError ret = [actionController setParticipantsChatPrivilege:privilege];
    return Help_type.ZoomSDKErrorType(ret);
}

bool ZMeetingChatWrap::IsMeetingChatLegalNoticeAvailable()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return false;
  
    ZoomSDKMeetingActionController *actionController = [service getMeetingActionController];
    if (!actionController)
        return false;
    
    return [actionController isMeetingChatLegalNoticeAvailable];
}

ZoomSTRING ZMeetingChatWrap::getChatLegalNoticesPrompt()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return "";
    
    ZoomSDKMeetingActionController *actionController = [service getMeetingActionController];
    if (!actionController)
        return "";

    NSString *noticesPrompt = [actionController getChatLegalNoticesPrompt];
    if (!noticesPrompt)
        return "";
    return [noticesPrompt UTF8String];
}

ZoomSTRING ZMeetingChatWrap::getChatLegalNoticesExplained()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return "";
    
    ZoomSDKMeetingActionController *actionController = [service getMeetingActionController];
    if (!actionController)
        return "";

    NSString *noticesExplained = [actionController getChatLegalNoticesExplained];
    if (!noticesExplained)
        return "";
    return [noticesExplained UTF8String];
}

bool ZMeetingChatWrap::IsShareMeetingChatLegalNoticeAvailable()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return false;
  
    ZoomSDKMeetingActionController *actionController = [service getMeetingActionController];
    if (!actionController)
        return false;
    
    return [actionController isShareMeetingChatLegalNoticeAvailable];
}

ZoomSTRING ZMeetingChatWrap::GetShareMeetingChatStartedLegalNoticeContent()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return "";
    
    ZoomSDKMeetingActionController *actionController = [service getMeetingActionController];
    if (!actionController)
        return "";

    NSString *noticesPrompt = [actionController getShareMeetingChatStartedLegalNoticeContent];
    if (!noticesPrompt)
        return "";
    return [noticesPrompt UTF8String];
}

ZoomSTRING ZMeetingChatWrap::GetShareMeetingChatStoppedLegalNoticeContent()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return "";
    
    ZoomSDKMeetingActionController *actionController = [service getMeetingActionController];
    if (!actionController)
        return "";

    NSString *noticesPrompt = [actionController getShareMeetingChatStoppedLegalNoticeContent];
    if (!noticesPrompt)
        return "";
    return [noticesPrompt UTF8String];
}

bool ZMeetingChatWrap::IsChatMessageCanBeDeleted(ZoomSTRING zn_msgID)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return false;
  
    ZoomSDKMeetingActionController *actionController = [service getMeetingActionController];
    if (!actionController)
        return false;

    if (zn_msgID.empty())
        return false;

    NSString *msgID = [NSString stringWithUTF8String:zn_msgID.c_str()];
    return [actionController isChatMessageCanBeDeleted:msgID];
}

ZNSDKError ZMeetingChatWrap::DeleteChatMessage(ZoomSTRING zn_msgID)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
  
    ZoomSDKMeetingActionController *actionController = [service getMeetingActionController];
    if (!actionController)
        return ZNSDKERR_SERVICE_FAILED;

    if (zn_msgID.empty())
        return ZNSDKERR_INVALID_PARAMETER;

    NSString *msgID = [NSString stringWithUTF8String:zn_msgID.c_str()];
    ZoomSDKError ret = [actionController deleteChatMessage:msgID];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNList<ZoomSTRING> ZMeetingChatWrap::GetAllChatMessageID()
{
    ZNList<ZoomSTRING> list;
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return list;
    
    ZoomSDKMeetingActionController *actionController = [service getMeetingActionController];
    if (!actionController)
        return list;

    NSArray *arr = [actionController getAllChatMessageID];
    if (arr && arr.count > 0) {
        for (NSString *msgID in arr) {
            list.push_back(msgID.UTF8String);
        }
    }
    return list;
}

ZNChatMsgInfo ZMeetingChatWrap::GetChatMessageById(ZoomSTRING zn_msgID)
{
    ZNChatMsgInfo zn_chat_msg_info;
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return zn_chat_msg_info;
    
    ZoomSDKMeetingChatController *chatController = [service getMeetingChatController];
    if (!chatController)
        return zn_chat_msg_info;

    if (zn_msgID.empty())
        return zn_chat_msg_info;
    
    NSString *messageID = [NSString stringWithUTF8String:zn_msgID.c_str()];
    ZoomSDKChatInfo* chatMsg = [chatController getChatMessageById:messageID];
    if (chatMsg)
    {
        ZoomSTRING zn_messageID;
        NSString *chatMessageID = [chatMsg getMessageID];
        if (chatMessageID)
            zn_messageID = [chatMessageID UTF8String];
        zn_chat_msg_info.messageID = zn_messageID;
        zn_chat_msg_info.senderUserId = [chatMsg getSenderUserID];

        ZoomSTRING zn_senderDisplayName;
        NSString *senderDisplayName = [chatMsg getSenderDisplayName];
        if (senderDisplayName)
            zn_senderDisplayName = [senderDisplayName UTF8String];
        zn_chat_msg_info.senderDisplayName = zn_senderDisplayName;
        zn_chat_msg_info.receiverUserId = [chatMsg getReceiverUserID];

        ZoomSTRING zn_receiverDisplayName;
        NSString *receiverDisplayName = [chatMsg getReceiverDisplayName];
        if (receiverDisplayName)
            zn_receiverDisplayName = [receiverDisplayName UTF8String];
        zn_chat_msg_info.receiverDisplayName = zn_receiverDisplayName;

        ZoomSTRING zn_content;
        NSString *content = [chatMsg getMsgContent];
        if (content)
            zn_content = [content UTF8String];
        zn_chat_msg_info.content = zn_content;
        zn_chat_msg_info.timeStamp = [chatMsg getTimeStamp];
        zn_chat_msg_info.isChatToWaitingroom = chatMsg.isChatToWaitingRoom;

        nativeErrorTypeHelp Help_type;
        ZoomSDKChatMessageType chatMessageType = [chatMsg getChatMessageType];
        ZNSDKChatMessageType zn_chatMessageType = Help_type.ZNSDKChatMessageTypeMap(chatMessageType);
        zn_chat_msg_info.chatMessageType = zn_chatMessageType;

        if (chatMessageType == ZoomSDKChatMessageType_To_All)
            zn_chat_msg_info.isChatToAll = true;

        if (chatMessageType == ZoomSDKChatMessageType_To_All_Panelist)
            zn_chat_msg_info.isChatToAllPanelist = true;

        zn_chat_msg_info.isComment = chatMsg.isComment;
        zn_chat_msg_info.isThread = chatMsg.isThread;

        ZoomSTRING zn_threadID;
        NSString *threadID = [chatMsg getThreadID];
        if (threadID)
            zn_threadID = [threadID UTF8String];
        zn_chat_msg_info.threadID = zn_threadID;
        NSArray<ZoomSDKRichTextStyleItem *> * plstRichTextStyleItem = [chatMsg getTextStyleItemList];
        ZNList<ZNRichTextStyleItem> text_style_item_list;
        for (ZoomSDKRichTextStyleItem *pRichTextStyleItem in plstRichTextStyleItem)
        {
            ZNRichTextStyleItem zn_rich_text_style_item;
            if (pRichTextStyleItem)
            {
                zn_rich_text_style_item.textStyle = (ZNRichTextStyle)pRichTextStyleItem.textStyle;
                NSArray<ZoomSDKRichTextStyleOffset *> * plstTextStyleOffset = pRichTextStyleItem.getTextStyleOffsetList;
                ZNList<ZNRichTextStyleOffset> rich_text_style_offset_list;
                for (ZoomSDKRichTextStyleOffset *pTextStyleOffset in plstTextStyleOffset)
                {
                    ZNRichTextStyleOffset zn_rich_text_style_offset;
                    if (pTextStyleOffset)
                    {
                        zn_rich_text_style_offset.positionStart = pTextStyleOffset.positionStart;
                        zn_rich_text_style_offset.positionEnd = pTextStyleOffset.positionEnd;

                        ZoomSTRING zn_reserve;
                        NSString *reserve = pTextStyleOffset.reserve;
                        if (reserve)
                            zn_reserve = [reserve UTF8String];
                        zn_rich_text_style_offset.reserve = zn_reserve;
                    }
                    rich_text_style_offset_list.push_back(zn_rich_text_style_offset);
                }
                zn_rich_text_style_item.textStyleOffsetList = rich_text_style_offset_list;
            }
            text_style_item_list.push_back(zn_rich_text_style_item);
        }
        zn_chat_msg_info.textStyleItemList = text_style_item_list;
    }
    return zn_chat_msg_info;
}

ZNSDKError ZMeetingChatWrap::SetChatMessageBuilder(ZoomSTRING zn_content, unsigned int receiver, ZoomSTRING zn_threadId, ZNSDKChatMessageType zn_type, unsigned int positionStart, unsigned int positionEnd, bool bClearStyles, bool bClear)
{
    lastBuildChatMsg = nil;
    if (bClearStyles || bClear)
    {
        return ZNSDKERR_NO_IMPL;
    }

    ZoomSDKChatMsgInfoBuilder* builder = [[ZoomSDKChatMsgInfoBuilder alloc] init];
    if (!builder)
    {
        return ZNSDKERR_SERVICE_FAILED;
    }

    if (zn_content.empty())
    {
        return ZNSDKERR_INVALID_PARAMETER;
    }

    NSString *content = [NSString stringWithUTF8String:zn_content.c_str()];
    [builder setContent:content];
    [builder setReceiver:receiver];

    if (!zn_threadId.empty())
    {
        NSString *threadId = [NSString stringWithUTF8String:zn_threadId.c_str()];
        [builder setThreadId:threadId];
    }

    nativeErrorTypeHelp Help_type;
    ZoomSDKChatMessageType type = Help_type.ZoomSDKChatMessageTypeMap(zn_type);
    [builder setMessageType:type];
    
    if (positionStart < positionEnd)
    {
        [builder setQuotePosition:positionStart positionEnd:positionEnd];
    }
    lastBuildChatMsg = [builder build];
    return ZNSDKERR_SUCCESS;
}

ZNSDKError ZMeetingChatWrap::SendLastBuildChatMsgTo()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
    
    ZoomSDKMeetingChatController *chatController = [service getMeetingChatController];
    if (!chatController)
        return ZNSDKERR_SERVICE_FAILED;

    nativeErrorTypeHelp Help_type;
    ZoomSDKChatInfo* message = lastBuildChatMsg;
    ZoomSDKError ret = [chatController sendChatMsgTo:message];
    lastBuildChatMsg = nil;
    return Help_type.ZoomSDKErrorType(ret);
}

bool ZMeetingChatWrap::IsFileTransferEnabled()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return false;
  
    ZoomSDKMeetingChatController *chatController = [service getMeetingChatController];
    if (!chatController)
        return false;
    
    return [chatController isFileTransferEnabled];
}

ZNSDKError ZMeetingChatWrap::TransferFile(ZoomSTRING zn_filePath, unsigned int userID)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
  
    ZoomSDKMeetingChatController *chatController = [service getMeetingChatController];
    if (!chatController)
        return ZNSDKERR_SERVICE_FAILED;

    if (zn_filePath.empty())
        return ZNSDKERR_INVALID_PARAMETER;

    NSString *filePath = [NSString stringWithUTF8String:zn_filePath.c_str()];
    ZoomSDKError ret = [chatController transferFile:filePath toUser:userID];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingChatWrap::TransferFileToAll(ZoomSTRING zn_filePath)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
  
    ZoomSDKMeetingChatController *chatController = [service getMeetingChatController];
    if (!chatController)
        return ZNSDKERR_SERVICE_FAILED;

    if (zn_filePath.empty())
        return ZNSDKERR_INVALID_PARAMETER;

    NSString *filePath = [NSString stringWithUTF8String:zn_filePath.c_str()];
    ZoomSDKError ret = [chatController transferFileToAll:filePath];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZoomSTRING ZMeetingChatWrap::GetTransferFileTypeAllowList()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return "";
    
    ZoomSDKMeetingChatController *chatController = [service getMeetingChatController];
    if (!chatController)
        return "";

    NSString *list = [chatController getTransferFileTypeAllowList];
    if (!list)
        return "";
    return [list UTF8String];
}

unsigned long long ZMeetingChatWrap::GetMaxTransferFileSizeBytes()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return 0;
    
    ZoomSDKMeetingChatController *chatController = [service getMeetingChatController];
    if (!chatController)
        return 0;
    
    return [chatController getMaxTransferFileSizeBytes];
}

//callback
void ZMeetingChatWrap::onChatMsgNotification(ZNChatMsgInfo chatMsg, ZoomSTRING content)
{
    if (m_pSink) {
        m_pSink->onChatMsgNotification(chatMsg, content.c_str());
    }
}

void ZMeetingChatWrap::onChatStatusChangedNotification(ZNChatStatus status)
{
    if (m_pSink) {
        m_pSink->onChatStatusChangedNotification(status);
    }
}

void ZMeetingChatWrap::onChatMsgDeleteNotification(ZoomSTRING msgID, ZNSDKChatMessageDeleteType deleteBy)
{
    if (m_pSink) {
        m_pSink->onChatMsgDeleteNotification(msgID, deleteBy);
    }
}

void ZMeetingChatWrap::onChatMessageEditNotification(ZNChatMsgInfo chatMsg)
{
    if (m_pSink)
    {
        m_pSink->onChatMessageEditNotification(chatMsg);
    }
}

void ZMeetingChatWrap::onShareMeetingChatStatusChanged(bool isStart)
{
    if (m_pSink) {
        m_pSink->onShareMeetingChatStatusChanged(isStart);
    }
}

void ZMeetingChatWrap::onFileSendStart(ZNSDKFileTransferInfo senderInfo)
{
    if (m_pSink) {
        m_pSink->onFileSendStart(senderInfo);
    }
}

unsigned int ZMeetingChatWrap::GetReceiver()
{
    ZoomSDKFileSender *sender = [meetingServiceChatDelegate share].fileSender;
    if (!sender)
        return 0;

    return sender.receiverUserId;
}

ZNSDKError ZMeetingChatWrap::CancelSend()
{
    ZoomSDKFileSender *sender = [meetingServiceChatDelegate share].fileSender;
    if (!sender)
        return ZNSDKERR_WRONG_USEAGE;

    ZoomSDKError ret = [sender cancelSend];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

void ZMeetingChatWrap::onFileReceived(ZNSDKFileTransferInfo receiverInfo)
{
    if (m_pSink) {
        m_pSink->onFileReceived(receiverInfo);
    }
}

unsigned int ZMeetingChatWrap::GetSender()
{
    ZoomSDKFileReceiver *receiver = [meetingServiceChatDelegate share].fileReceiver;
    if (!receiver)
        return 0;

    return receiver.senderUserId;
}

ZNSDKError ZMeetingChatWrap::CancelReceive()
{
    ZoomSDKFileReceiver *receiver = [meetingServiceChatDelegate share].fileReceiver;
    if (!receiver)
        return ZNSDKERR_WRONG_USEAGE;

    ZoomSDKError ret = [receiver cancelReceive];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingChatWrap::StartReceive(ZoomSTRING zn_path)
{
    ZoomSDKFileReceiver *receiver = [meetingServiceChatDelegate share].fileReceiver;
    if (!receiver)
        return ZNSDKERR_WRONG_USEAGE;
    
    if (zn_path.empty())
        return ZNSDKERR_INVALID_PARAMETER;

    NSString *path = [NSString stringWithUTF8String:zn_path.c_str()];
    ZoomSDKError ret = [receiver startReceive:path];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

void ZMeetingChatWrap::onFileTransferProgress(ZNSDKFileTransferInfo transferInfo)
{
    if (m_pSink) {
        m_pSink->onFileTransferProgress(transferInfo);
    }
}