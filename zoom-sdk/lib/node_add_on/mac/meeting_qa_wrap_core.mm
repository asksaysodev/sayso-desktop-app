#include "Header_include.h"
#include "sdk_native_error.h"
#include "meeting_qa_wrap_core.h"
#include "meeting_service_wrap_core.h"
#import "meetingServiceDelegate.h"

ZMeetingQAWrap &ZMeetingServiceWrap::GetMeetingQACtrl()
{
    return m_meeting_qa_ctrl;
}
ZMeetingQAWrap::ZMeetingQAWrap()
{
   m_pSink = 0;
}

ZMeetingQAWrap::~ZMeetingQAWrap()
{
    m_pSink = 0;
    [[[[ZoomSDK sharedSDK] getMeetingService] getMeetingQAController] setDelegate:nil];
}

void ZMeetingQAWrap::SetSink(ZNativeSDKMeetingQAWrapSink *pSink)
{
    [[[[ZoomSDK sharedSDK] getMeetingService] getQAController] setDelegate:[[meetingServiceQADelegate alloc] init]];
    m_pSink= pSink;
}

void ZMeetingQAWrap::Init()
{

}
void ZMeetingQAWrap::Uninit()
{
    
}

ZNQAItemInfo ZMeetingQAWrap::GetQuestion(ZoomSTRING zn_questionID) {
    ZNQAItemInfo zn_question_info;
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return zn_question_info;
    
    ZoomSDKQAController *questionController = [service getQAController];
    if (!questionController)
        return zn_question_info;

    if (zn_questionID.empty())
        return zn_question_info;
    
    NSString *questionID = [NSString stringWithUTF8String:zn_questionID.c_str()];
    ZoomSDKQuestionInfo* question = [questionController getQuestionByQuestionID:questionID];
    if (question)
    {
        // time
        zn_question_info.timeStamp = [question getTimeStamp];

        // upvoteNum
        zn_question_info.upvoteNum = [question upVoteNum];

        // text
        ZoomSTRING zn_text;
        NSString *text = [question getQuestionContent];
        if (text)
            zn_text = [text UTF8String];
        zn_question_info.text = zn_text;

        // senderName
        ZoomSTRING zn_senderName;
        NSString *senderName = [question getSendName];
        if (senderName)
            zn_senderName = [senderName UTF8String];
        zn_question_info.senderName = zn_senderName;

        // questionID
        ZoomSTRING zn_q_ID;
        NSString *questionItemID = [question getQuestionId];
        if (questionItemID)
            zn_q_ID = [questionItemID UTF8String];
        zn_question_info.questionID = zn_q_ID;

        // isAnonymous
        zn_question_info.isAnonymous = [question isAnonymous];

        // isMarkedAsAnswered
        zn_question_info.isMarkedAsAnswered = [question isMarkedAsAnswered];

        // isMarkedAsDismissed
        zn_question_info.isMarkedAsDismissed = [question isMarkedAsDismissed];

        // isSenderMyself
        zn_question_info.isSenderMyself = [question isSenderMyself];

        // isMySelfUpvoted
        zn_question_info.isMySelfUpvoted = [question isMySelfUpvoted];

        // hasLiveAnswers
        zn_question_info.hasLiveAnswers = [question hasLiveAnswers];

        // hasTextAnswers
        zn_question_info.hasTextAnswers = [question hasTextAnswers];

        // amILiveAnswering
        zn_question_info.amILiveAnswering = [question amILiveAnswering];

        // liveAnswerName
        ZoomSTRING zn_liveAnswerName;
        NSString *liveAnswerName = [question getLiveAnswerName];
        if (liveAnswerName)
            zn_liveAnswerName = [liveAnswerName UTF8String];
        zn_question_info.liveAnswerName = zn_liveAnswerName;

        // isLiveAnswering
        zn_question_info.isLiveAnswering = [question isLiveAnswering];

        // answerList
        NSArray<ZoomSDKAnswerInfo *> * panswerList = [question getAnswerList];
        ZNList<ZNAnswerItem> answerList;
        for (ZoomSDKAnswerInfo *pAnswerItem in panswerList) {
            ZNAnswerItem zn_answer_item;
            if (pAnswerItem) {
                // time 
                zn_answer_item.timeStamp = [pAnswerItem getTimeStamp];

                // text
                ZoomSTRING zn_text_2;
                NSString *text2 = [pAnswerItem getAnswerContent];
                if (text2)
                    zn_text_2 = [text2 UTF8String];
                zn_answer_item.text = zn_text_2;

                // senderName
                ZoomSTRING zn_senderName_2;
                NSString *senderName2 = [pAnswerItem getSendName];
                if (senderName2)
                    zn_senderName_2 = [senderName2 UTF8String];
                zn_answer_item.senderName = zn_senderName_2;

                // questionID
                ZoomSTRING zn_q_ID_2;
                NSString *questionItemID2 = [pAnswerItem getQuestionId];
                if (questionItemID2)
                    zn_q_ID_2 = [questionItemID2 UTF8String];
                zn_answer_item.questionID = zn_q_ID_2;

                // answerID
                ZoomSTRING zn_a_ID;
                NSString *answerItemID = [pAnswerItem getAnswerID];
                if (answerItemID)
                    zn_a_ID = [answerItemID UTF8String];
                zn_answer_item.answerID = zn_a_ID;

                // isPrivate
                zn_answer_item.isPrivate = [pAnswerItem isPrivate];

                // isLiveAnswer
                zn_answer_item.isLiveAnswer = [pAnswerItem isLiveAnswer];

                // isSenderMyself
                zn_answer_item.isSenderMyself = [pAnswerItem isSenderMyself];
            }
            answerList.push_back(zn_answer_item);
        }
        zn_question_info.answerList = answerList;
    }
    return zn_question_info;
}

ZNAnswerItem ZMeetingQAWrap::GetAnswer(ZoomSTRING zn_answerID) {
    ZNAnswerItem zn_answer_info;
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return zn_answer_info;
    
    ZoomSDKQAController *questionController = [service getQAController];
    if (!questionController)
        return zn_answer_info;

    if (zn_answerID.empty())
        return zn_answer_info;
    
    NSString *answerID = [NSString stringWithUTF8String:zn_answerID.c_str()];
    ZoomSDKAnswerInfo* answer = [questionController getAnswerByAnswerID:answerID];

    if(answer) {
        // answerID
        ZoomSTRING zn_a_ID;
        NSString *answerItemID = [answer getAnswerID];
        if (answerItemID)
            zn_a_ID = [answerItemID UTF8String];
        zn_answer_info.answerID = zn_a_ID;

        // isSenderMyself
        zn_answer_info.isSenderMyself = [answer isSenderMyself];

        // time
        zn_answer_info.timeStamp = [answer getTimeStamp];

        // text
        ZoomSTRING zn_text;
        NSString *text = [answer getAnswerContent];
        if (text)
            zn_text = [text UTF8String];
        zn_answer_info.text = zn_text;

        // senderName
        ZoomSTRING zn_senderName;
        NSString *senderName = [answer getSendName];
        if (senderName)
            zn_senderName = [senderName UTF8String];
        zn_answer_info.senderName = zn_senderName;

        // questionID
        ZoomSTRING zn_q_ID;
        NSString *questionItemID = [answer getQuestionId];
        if (questionItemID)
            zn_q_ID = [questionItemID UTF8String];
        zn_answer_info.questionID = zn_q_ID;

        // isPrivate
        zn_answer_info.isPrivate = [answer isPrivate];

        // isLiveAnswer
        zn_answer_info.isLiveAnswer = [answer isLiveAnswer];
    }
    return zn_answer_info;
}

ZNSDKError ZMeetingQAWrap::answerQuestionPublicWithQuestionID(ZoomSTRING zn_questionID, ZoomSTRING zn_content) {
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) {
        return ZNSDKERR_SERVICE_FAILED;
    }
        
    ZoomSDKQAController *questionController = [service getQAController];
    if (!questionController) {
        return ZNSDKERR_SERVICE_FAILED;
    }
        
    if (zn_questionID.empty() || zn_content.empty()) {
        return ZNSDKERR_SERVICE_FAILED;
    }
    NSString *questionID = [NSString stringWithUTF8String:zn_questionID.c_str()];
    NSString *content = [NSString stringWithUTF8String:zn_content.c_str()];
    ZoomSDKError ret = [questionController answerQuestionPublicWithQuestionID:questionID answerContent:content];

    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingQAWrap::answerQuestionPrivateWithQuestionID(ZoomSTRING zn_questionID, ZoomSTRING zn_content) {
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) {
        return ZNSDKERR_SERVICE_FAILED;
    }
        
    ZoomSDKQAController *questionController = [service getQAController];
    if (!questionController) {
        return ZNSDKERR_SERVICE_FAILED;
    }
        
    if (zn_questionID.empty() || zn_content.empty()) {
        return ZNSDKERR_SERVICE_FAILED;
    }
    NSString *questionID = [NSString stringWithUTF8String:zn_questionID.c_str()];
    NSString *content = [NSString stringWithUTF8String:zn_content.c_str()];
    ZoomSDKError ret = [questionController answerQuestionPrivateWithQuestionID:questionID answerContent:content];

    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNList<ZNQAItemInfo> ZMeetingQAWrap::getAllQuestionList() {
    ZNList<ZNQAItemInfo> zn_question_info_list;
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) {
        return zn_question_info_list;
    }
    
    ZoomSDKQAController *questionController = [service getQAController];
    if (!questionController) {
        return zn_question_info_list;
    }

    NSArray<ZoomSDKQuestionInfo *> * arr = [questionController getAllQuestionList];

    for (ZoomSDKQuestionInfo *question in arr) {
        if (question) {
            ZNQAItemInfo zn_question_info;

            // time
            zn_question_info.timeStamp = [question getTimeStamp];

            // upvoteNum
            zn_question_info.upvoteNum = [question upVoteNum];

            // text
            ZoomSTRING zn_text;
            NSString *text = [question getQuestionContent];
            if (text)
                zn_text = [text UTF8String];
            zn_question_info.text = zn_text;

            // senderName
            ZoomSTRING zn_senderName;
            NSString *senderName = [question getSendName];
            if (senderName)
                zn_senderName = [senderName UTF8String];
            zn_question_info.senderName = zn_senderName;

            // questionID
            ZoomSTRING zn_q_ID;
            NSString *questionItemID = [question getQuestionId];
            if (questionItemID)
                zn_q_ID = [questionItemID UTF8String];
            zn_question_info.questionID = zn_q_ID;

            // isAnonymous
            zn_question_info.isAnonymous = [question isAnonymous];

            // isMarkedAsAnswered
            zn_question_info.isMarkedAsAnswered = [question isMarkedAsAnswered];

            // isMarkedAsDismissed
            zn_question_info.isMarkedAsDismissed = [question isMarkedAsDismissed];

            // isSenderMyself
            zn_question_info.isSenderMyself = [question isSenderMyself];

            // isMySelfUpvoted
            zn_question_info.isMySelfUpvoted = [question isMySelfUpvoted];

            // hasLiveAnswers
            zn_question_info.hasLiveAnswers = [question hasLiveAnswers];

            // hasTextAnswers
            zn_question_info.hasTextAnswers = [question hasTextAnswers];

            // amILiveAnswering
            zn_question_info.amILiveAnswering = [question amILiveAnswering];

            // liveAnswerName
            ZoomSTRING zn_liveAnswerName;
            NSString *liveAnswerName = [question getLiveAnswerName];
            if (liveAnswerName)
                zn_liveAnswerName = [liveAnswerName UTF8String];
            zn_question_info.liveAnswerName = zn_liveAnswerName;

            // isLiveAnswering
            zn_question_info.isLiveAnswering = [question isLiveAnswerin];

            // answerList
            NSArray<ZoomSDKAnswerInfo *> * panswerList = [question getAnswerList];
            ZNList<ZNAnswerItem> answerList;
            for (ZoomSDKAnswerInfo *pAnswerItem in panswerList) {
                ZNAnswerItem zn_answer_item;
                if (pAnswerItem) {
                    // time 
                    zn_answer_item.timeStamp = [pAnswerItem getTimeStamp];

                    // text
                    ZoomSTRING zn_text_2;
                    NSString *text2 = [pAnswerItem getAnswerContent];
                    if (text2)
                        zn_text_2 = [text2 UTF8String];
                    zn_answer_item.text = zn_text_2;

                    // senderName
                    ZoomSTRING zn_senderName_2;
                    NSString *senderName2 = [pAnswerItem getSendName];
                    if (senderName2)
                        zn_senderName_2 = [senderName2 UTF8String];
                    zn_answer_item.senderName = zn_senderName_2;

                    // questionID
                    ZoomSTRING zn_q_ID_2;
                    NSString *questionItemID2 = [pAnswerItem getQuestionId];
                    if (questionItemID2)
                        zn_q_ID_2 = [questionItemID2 UTF8String];
                    zn_answer_item.questionID = zn_q_ID_2;

                    // answerID
                    ZoomSTRING zn_a_ID;
                    NSString *answerItemID = [pAnswerItem getAnswerID];
                    if (answerItemID)
                        zn_a_ID = [answerItemID UTF8String];
                    zn_answer_item.answerID = zn_a_ID;

                    // isPrivate
                    zn_answer_item.isPrivate = [pAnswerItem isPrivate];

                    // isLiveAnswer
                    zn_answer_item.isLiveAnswer = [pAnswerItem isLiveAnswer];

                    // isSenderMyself
                    zn_answer_item.isSenderMyself = [pAnswerItem isSenderMyself];
                }
                answerList.push_back(zn_answer_item);
            }
            zn_question_info.answerList = answerList;

            zn_question_info_list.push_back(zn_question_info);
        }
    }
    return zn_question_info_list;  
}

ZNSDKError ZMeetingQAWrap::StartLiving(ZoomSTRING zn_questionID) {
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
    
    ZoomSDKQAController *questionController = [service getQAController];
    if (!questionController)
        return ZNSDKERR_SERVICE_FAILED;

    NSString *questionID = [NSString stringWithUTF8String:zn_questionID.c_str()];

    ZoomSDKError ret = [questionController startLiving:questionID];

    nativeErrorTypeHelp  Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingQAWrap::EndLiving(ZoomSTRING zn_questionID) {
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
    
    ZoomSDKQAController *questionController = [service getQAController];
    if (!questionController)
        return ZNSDKERR_SERVICE_FAILED;

    NSString *questionID = [NSString stringWithUTF8String:zn_questionID.c_str()];

    ZoomSDKError ret = [questionController endLiving:questionID];

    nativeErrorTypeHelp  Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingQAWrap::DismissQuestionWithQuestionID(ZoomSTRING zn_questionID) {
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
    
    ZoomSDKQAController *questionController = [service getQAController];
    if (!questionController)
        return ZNSDKERR_SERVICE_FAILED;

    NSString *questionID = [NSString stringWithUTF8String:zn_questionID.c_str()];

    ZoomSDKError ret = [questionController dismissQuestionWithQuestionID:questionID];

    nativeErrorTypeHelp  Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingQAWrap::DeleteQuestion(ZoomSTRING zn_questionID) {
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
    
    ZoomSDKQAController *questionController = [service getQAController];
    if (!questionController)
        return ZNSDKERR_SERVICE_FAILED;

    NSString *questionID = [NSString stringWithUTF8String:zn_questionID.c_str()];

    ZoomSDKError ret = [questionController deleteQuestion:questionID];

    nativeErrorTypeHelp  Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingQAWrap::DeleteAnswer(ZoomSTRING zn_answerID) {
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;

    ZoomSDKQAController *questionController = [service getQAController];
    if (!questionController)
        return ZNSDKERR_SERVICE_FAILED;

    NSString *answerID = [NSString stringWithUTF8String:zn_answerID.c_str()];

    ZoomSDKError ret = [questionController deleteAnswer:answerID];

    nativeErrorTypeHelp  Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingQAWrap::ReopenQuestionWithQuestionID(ZoomSTRING zn_questionID) {
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
    
    ZoomSDKQAController *questionController = [service getQAController];
    if (!questionController)
        return ZNSDKERR_SERVICE_FAILED;

    NSString *questionID = [NSString stringWithUTF8String:zn_questionID.c_str()];

    ZoomSDKError ret = [questionController reopenQuestionWithQuestionID:questionID];

    nativeErrorTypeHelp  Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingQAWrap::VoteupQuestionWithQuestionID(ZoomSTRING zn_questionID, bool enabled) {
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) {
        return ZNSDKERR_SERVICE_FAILED;
    }
        
    ZoomSDKQAController *questionController = [service getQAController];
    if (!questionController) {
        return ZNSDKERR_SERVICE_FAILED;
    }
        
    if (zn_questionID.empty()) {
        return ZNSDKERR_SERVICE_FAILED;
    }
    NSString *questionID = [NSString stringWithUTF8String:zn_questionID.c_str()];
    ZoomSDKError ret = [questionController voteupQuestionWithQuestionID:questionID isEableVokeup:enabled];

    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}



void ZMeetingQAWrap::onAddQuestion(ZoomSTRING questionID, bool bSuccess)
{
    if (m_pSink) {
        m_pSink->onAddQuestion(questionID, bSuccess);
    }
}

void ZMeetingQAWrap::onReceiveQuestion(ZoomSTRING questionID)
{
    if (m_pSink) {
        m_pSink->onReceiveQuestion(questionID);
    }
}

void ZMeetingQAWrap::onDeleteQuestions(ZNList<ZoomSTRING> questions)
{
    if (m_pSink) {
        m_pSink->onDeleteQuestions(questions);
    }
}

void ZMeetingQAWrap::onQuestionMarkedAsDismissed(ZoomSTRING questionID)
{
    if (m_pSink) {
        m_pSink->onQuestionMarkedAsDismissed(questionID);
    }
}

void ZMeetingQAWrap::onReopenQuestion(ZoomSTRING questionID)
{
    if (m_pSink) {
        m_pSink->onReopenQuestion(questionID);
    }
}

void ZMeetingQAWrap::onUserLivingReply(ZoomSTRING questionID)
{
    if (m_pSink) {
        m_pSink->onUserLivingReply(questionID);
    }
}

void ZMeetingQAWrap::onUserEndLiving(ZoomSTRING questionID)
{
    if (m_pSink) {
        m_pSink->onUserEndLiving(questionID);
    }
}

void ZMeetingQAWrap::onReceiveAnswer(ZoomSTRING answerID)
{
    if (m_pSink) {
        m_pSink->onReceiveAnswer(answerID);
    }
}

void ZMeetingQAWrap::onDeleteAnswers(ZNList<ZoomSTRING> answer)
{
    if (m_pSink) {
        m_pSink->onDeleteAnswers(answer);
    }
}

void ZMeetingQAWrap::onVoteupQuestion(ZoomSTRING questionID, bool isChanged) 
{
    if (m_pSink) {
        m_pSink->onVoteupQuestion(questionID, isChanged);
    }
}

void ZMeetingQAWrap::onRevokeVoteupQuestion(ZoomSTRING questionID, bool isChanged) 
{
    if (m_pSink) {
        m_pSink->onRevokeVoteupQuestion(questionID, isChanged);
    }
}

void ZMeetingQAWrap::onAddAnswer(ZoomSTRING answerID, bool bSuccess) 
{
    if (m_pSink) {
        m_pSink->onAddAnswer(answerID, bSuccess);
    }
}
