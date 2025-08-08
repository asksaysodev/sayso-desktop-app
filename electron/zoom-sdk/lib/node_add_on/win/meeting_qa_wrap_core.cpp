#include "meeting_qa_wrap_core.h"
#include "meeting_service_components_wrap/meeting_qa_wrap.h"
#include "sdk_events_wrap_class.h"
#include "sdk_wrap.h"
#include "zoom_native_to_wrap.h"

extern ZOOM_SDK_NAMESPACE::IMeetingServiceWrap &g_meeting_service_wrap;

// Helper function to convert zchar_t* to ZoomSTRING.
// Note: Returns an empty string if zchar_t* is NULL.
ZoomSTRING ZCharToZoomSTRING(const zchar_t *zstr) 
{
	return zstr ? ZoomSTRING(zstr) : ZoomSTRING();
}

// Helper function to convert IAnswerItem to ZNAnswerItem.
// Note: answer_info should be a valid pointer.
ZNAnswerItem GetZNAnswerItemFromIAnswerItem(ZOOM_SDK_NAMESPACE::IAnswerItem *answer_info) 
{
	ZNAnswerItem zn_answer_item;
	if (answer_info == NULL) {
		return zn_answer_item;
	}

	zn_answer_item.timeStamp = answer_info->GetTimeStamp();
	zn_answer_item.text = ZCharToZoomSTRING(answer_info->GetText());
	zn_answer_item.senderName = ZCharToZoomSTRING(answer_info->GetSenderName());
	zn_answer_item.questionID = ZCharToZoomSTRING(answer_info->GetQuestionID());
	zn_answer_item.answerID = ZCharToZoomSTRING(answer_info->GetAnswerID());
	zn_answer_item.isPrivate = answer_info->IsPrivate();
	zn_answer_item.isLiveAnswer = answer_info->IsLiveAnswer();
	zn_answer_item.isSenderMyself = answer_info->IsSenderMyself();

	return zn_answer_item;
}

// Helper function to convert IQAItemInfo to ZNQAItemInfo.
// Note: question_info should be a valid pointer.
ZNQAItemInfo
GetZNQAItemInfoFromIQAItemInfo(ZOOM_SDK_NAMESPACE::IQAItemInfo *question_info) 
{
	ZNQAItemInfo zn_question_info;
	if (NULL == question_info) {
		return zn_question_info;
	}
	zn_question_info.timeStamp = question_info->GetTimeStamp();
	zn_question_info.upvoteNum = question_info->GetUpvoteNum();

	zn_question_info.text = ZCharToZoomSTRING(question_info->GetText());
	zn_question_info.senderName = ZCharToZoomSTRING(question_info->GetSenderName());
	zn_question_info.questionID = ZCharToZoomSTRING(question_info->GetQuestionID());
	zn_question_info.isAnonymous = question_info->IsAnonymous();
	zn_question_info.isMarkedAsAnswered = question_info->IsMarkedAsAnswered();
	zn_question_info.isMarkedAsDismissed = question_info->IsMarkedAsDismissed();
	zn_question_info.isSenderMyself = question_info->IsSenderMyself();
	zn_question_info.isMySelfUpvoted = question_info->IsMySelfUpvoted();
	zn_question_info.hasLiveAnswers = question_info->HasLiveAnswers();
	zn_question_info.hasTextAnswers = question_info->HasTextAnswers();
	zn_question_info.amILiveAnswering = question_info->AmILiveAnswering();
	zn_question_info.liveAnswerName = ZCharToZoomSTRING(question_info->GetLiveAnswerName());
	zn_question_info.isLiveAnswering = question_info->IsLiveAnswering();

	ZOOM_SDK_NAMESPACE::IList<ZOOM_SDK_NAMESPACE::IAnswerItem *> *answer_items = question_info->GetAnswerList();
	if (NULL == answer_items) {
		return zn_question_info;
	}

	for (int i = 0; i < answer_items->GetCount(); ++i) {
		ZOOM_SDK_NAMESPACE::IAnswerItem *answer_item = answer_items->GetItem(i);
		zn_question_info.answerList.push_back(GetZNAnswerItemFromIAnswerItem(answer_item));
	}
	return zn_question_info;
}

ZMeetingQAWrap::ZMeetingQAWrap() : m_pSink(nullptr) 
{
	SDKEventWrapMgr::GetInst().m_meetingQACtrlEvent.SetOwner(this);
	m_pSink = 0;
	/*SDKEventWrapMgr::GetInst().m_meetingQAController.SetOwner(this);*/
}

ZMeetingQAWrap::~ZMeetingQAWrap() 
{
	Uninit();
	m_pSink = 0;
	SDKEventWrapMgr::GetInst().m_meetingQACtrlEvent.SetOwner(NULL);
}

void ZMeetingQAWrap::SetSink(ZNativeSDKMeetingQAWrapSink *pSink) {
	m_pSink = pSink;
}

void ZMeetingQAWrap::Init() 
{
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingQAController().Init_Wrap(&g_meeting_service_wrap);
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingQAController().SetEvent(&SDKEventWrapMgr::GetInst().m_meetingQACtrlEvent);
}

void ZMeetingQAWrap::Uninit() {
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingQAController().SetEvent(NULL);
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingQAController().Uninit_Wrap();
}

// Callbacks
void ZMeetingQAWrap::onAddQuestion(ZoomSTRING questionID, bool bSuccess) 
{
	if (!m_pSink) {
		return;
	}
	m_pSink->onAddQuestion(questionID, bSuccess);
	return;
}

void ZMeetingQAWrap::onReceiveQuestion(ZoomSTRING questionID) 
{
	if (!m_pSink) {
		return;
	}
	m_pSink->onReceiveQuestion(questionID);
}

void ZMeetingQAWrap::onReopenQuestion(ZoomSTRING questionID) 
{
	if (!m_pSink) {
		return;
	}
	m_pSink->onReopenQuestion(questionID);
}

void ZMeetingQAWrap::onDeleteQuestions(ZNList<ZoomSTRING> questions) 
{
	if (!m_pSink) {
		return;
	}
	m_pSink->onDeleteQuestions(questions);
}

void ZMeetingQAWrap::onDeleteAnswers(ZNList<ZoomSTRING> answers) {
	if (!m_pSink) {
		return;
	}
	m_pSink->onDeleteAnswers(answers);
}

void ZMeetingQAWrap::onQuestionMarkedAsDismissed(ZoomSTRING questionID) 
{
	if (!m_pSink) {
		return;
	}
	m_pSink->onQuestionMarkedAsDismissed(questionID);
}

void ZMeetingQAWrap::onUserLivingReply(ZoomSTRING questionID) 
{
	if (!m_pSink) {
		return;
	}
	m_pSink->onUserLivingReply(questionID);
}

void ZMeetingQAWrap::onUserEndLiving(ZoomSTRING questionID) 
{
	if (!m_pSink) {
		return;
	}
	m_pSink->onUserEndLiving(questionID);
}

void ZMeetingQAWrap::onAddAnswer(ZoomSTRING answerID, bool bSuccess) 
{
	if (!m_pSink) {
		return;
	}
	m_pSink->onAddAnswer(answerID, bSuccess);
}

void ZMeetingQAWrap::onReceiveAnswer(ZoomSTRING answerID) 
{
	if (!m_pSink) {
		return;
	}
	m_pSink->onReceiveAnswer(answerID);
}

void ZMeetingQAWrap::onVoteupQuestion(ZoomSTRING questionID, bool isChanged) 
{
	if (!m_pSink) {
		return;
	}
	m_pSink->onVoteupQuestion(questionID, isChanged);
}

void ZMeetingQAWrap::onRevokeVoteupQuestion(ZoomSTRING questionID, bool isChanged) 
{
	if (!m_pSink) {
		return;
	}
	m_pSink->onRevokeVoteupQuestion(questionID, isChanged);
}

ZNSDKError ZMeetingQAWrap::StartLiving(ZoomSTRING questionID) 
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN;
	sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingQAController().StartLiving(questionID.c_str());
	return Map2WrapDefine(sdkerr);
}

ZNSDKError ZMeetingQAWrap::EndLiving(ZoomSTRING questionID) 
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN;
	sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingQAController().EndLiving(questionID.c_str());
	return Map2WrapDefine(sdkerr);
}

ZNSDKError ZMeetingQAWrap::DismissQuestionWithQuestionID(ZoomSTRING questionID) {
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN;
	sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingQAController().DismissQuestion(questionID.c_str());
	return Map2WrapDefine(sdkerr);
}

ZNSDKError ZMeetingQAWrap::DeleteQuestion(ZoomSTRING questionID) 
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN;
	sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingQAController().DeleteQuestion(questionID.c_str());
	return Map2WrapDefine(sdkerr);
}


ZNSDKError ZMeetingQAWrap::ReopenQuestionWithQuestionID(ZoomSTRING questionID) 
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN;
	sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingQAController().ReopenQuestion(questionID.c_str());
	return Map2WrapDefine(sdkerr);
}

ZNSDKError ZMeetingQAWrap::VoteupQuestionWithQuestionID(ZoomSTRING questionID, bool enabled) 
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN;
	sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingQAController().VoteupQuestion(questionID.c_str(), enabled);
	return Map2WrapDefine(sdkerr);
}

ZNSDKError ZMeetingQAWrap::answerQuestionPublicWithQuestionID(ZoomSTRING questionID, ZoomSTRING content) {
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN;
	sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingQAController().AnswerQuestionPublic(questionID.c_str(), content.c_str());
	return Map2WrapDefine(sdkerr);
}

ZNSDKError ZMeetingQAWrap::answerQuestionPrivateWithQuestionID(ZoomSTRING questionID, ZoomSTRING content) 
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN;
	sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingQAController().AnswerQuestionPrivate(questionID.c_str(), content.c_str());
	return Map2WrapDefine(sdkerr);
}

ZNSDKError ZMeetingQAWrap::DeleteAnswer(ZoomSTRING answerID) 
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN;
	sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingQAController().DeleteAnswer(answerID.c_str());
	return Map2WrapDefine(sdkerr);
}

ZNQAItemInfo ZMeetingQAWrap::GetQuestion(ZoomSTRING questionID) 
{
	ZOOM_SDK_NAMESPACE::IQAItemInfo* question_info = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingQAController().GetQuestion(questionID.c_str());
	return GetZNQAItemInfoFromIQAItemInfo(question_info);
}


ZNList<ZNQAItemInfo> ZMeetingQAWrap::getAllQuestionList() 
{
	ZOOM_SDK_NAMESPACE::IList<ZOOM_SDK_NAMESPACE::IQAItemInfo *> *question_list;
	question_list = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingQAController().GetAllQuestionList();
	
	ZNList<ZNQAItemInfo> zn_question_list;
	if (question_list == nullptr) {
		return zn_question_list;
	}
	for (int i = 0; i < question_list->GetCount(); ++i) {
		ZOOM_SDK_NAMESPACE::IQAItemInfo *question_info = question_list->GetItem(i);
		zn_question_list.push_back(GetZNQAItemInfoFromIQAItemInfo(question_info));
	}
	return zn_question_list;
}

ZNAnswerItem ZMeetingQAWrap::GetAnswer(ZoomSTRING answerID) 
{
	ZOOM_SDK_NAMESPACE::IAnswerItem* answer_item = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingQAController().GetAnswer(answerID.c_str());
	return GetZNAnswerItemFromIAnswerItem(answer_item);
}