#pragma once
#include "common_include.h"
BEGIN_ZOOM_SDK_NAMESPACE
class IMeetingServiceWrap;
IMeetingQAController *
InitIMeetingQAControllerFunc(IMeetingQAControllerEvent *pEvent, IMeetingServiceWrap *pOwner);
void UninitIMeetingQAControllerFunc(IMeetingQAController *obj);
BEGIN_CLASS_DEFINE_WITHCALLBACK(IMeetingQAController, IMeetingQAControllerEvent)
NORMAL_CLASS(IMeetingQAController)
INIT_UNINIT_WITHEVENT_AND_OWNSERVICE(IMeetingQAController, IMeetingServiceWrap)
virtual SDKError SetEvent(IMeetingQAControllerEvent *pEvent) {
	external_cb = pEvent;
	return SDKERR_SUCCESS;
}

// virtual SDKError AddQuestion(const zchar_t *questionContent, bool
// bAskAnonymous) = 0;
DEFINE_FUNC_2(AddQuestion, SDKError, const zchar_t *, questionContent, bool, bAskAnonymous)
// virtual int GetMyQuestionCount() = 0;
DEFINE_FUNC_0(GetMyQuestionCount, int)
// virtual IList<IQAItemInfo*>* GetMyQuestionList() = 0;
DEFINE_FUNC_0(GetMyQuestionList, IList<IQAItemInfo *> *)
// virtual SDKError CommentQuestion(const zchar_t *questionID, const zchar_t*
// commentContent) = 0;
DEFINE_FUNC_2(CommentQuestion, SDKError, const zchar_t *, questionID, const zchar_t *, commentContent)
// virtual SDKError AnswerQuestionPrivate(const zchar_t *questionID, const
// zchar_t *answerContent) = 0;
DEFINE_FUNC_2(AnswerQuestionPrivate, SDKError, const zchar_t *, questionID, const zchar_t *, answerContent)
// virtual SDKError AnswerQuestionPublic(const zchar_t *questionID, const
// zchar_t *answerContent) = 0;
DEFINE_FUNC_2(AnswerQuestionPublic, SDKError, const zchar_t *, questionID, const zchar_t *, answerContent)
// virtual SDKError DismissQuestion(const zhcar_t *questionID) = 0;
DEFINE_FUNC_1(DismissQuestion, SDKError, const zchar_t *, questionID)
// virtual SDKError DeleteQuestion(const zchar_t *questionID) = 0;
DEFINE_FUNC_1(DeleteQuestion, SDKError, const zchar_t *, questionID)
// virtual SDKError DeleteAnswer(const zchar_t *answerID) = 0;
DEFINE_FUNC_1(DeleteAnswer, SDKError, const zchar_t *, answerID)
// virtual SDKError ReopenQuestion(const zchar_t *questionID) = 0;
DEFINE_FUNC_1(ReopenQuestion, SDKError, const zchar_t *, questionID)
// virtual SDKError StartLiving(const zchar_t *questionID) = 0;
DEFINE_FUNC_1(StartLiving, SDKError, const zchar_t *, questionID)
// virtual SDKError EndLiving(const zchar_t *questionID) = 0;
DEFINE_FUNC_1(EndLiving, SDKError, const zchar_t *, questionID)
// virtual int GetOpenQuestionCount() = 0;
DEFINE_FUNC_0(GetOpenQuestionCount, int)
// virtual int GetDismissedQuestionCount() = 0;
DEFINE_FUNC_0(GetDismissedQuestionCount, int)
// virtual int GetAnsweredQuestionCount() = 0;
DEFINE_FUNC_0(GetAnsweredQuestionCount, int)
// virtual IList<IQAItemInfo*>* GetOpenQuestionList() = 0;
DEFINE_FUNC_0(GetOpenQuestionList, IList<IQAItemInfo *> *)
// virtual IList<IQAItemInfo*>* GetDismissedQuestionList() = 0;
DEFINE_FUNC_0(GetDismissedQuestionList, IList<IQAItemInfo *> *)
// virtual IList<IQAItemInfo*>* GetAnsweredQuestionList() = 0;
DEFINE_FUNC_0(GetAnsweredQuestionList, IList<IQAItemInfo *> *)
DEFINE_FUNC_1(EnableAnonymousQuestion, SDKError, bool, bEnable)
// virtual SDKError EnableAttendeeViewAllQuestion(bool bEnable) = 0;
DEFINE_FUNC_1(EnableAttendeeViewAllQuestion, SDKError, bool, bEnable)
// virtual SDKError EnableQAComment(bool bEnable) = 0;
DEFINE_FUNC_1(EnableQAComment, SDKError, bool, bEnable)
// virtual SDKError EnableQAVoteup(bool bEnable) = 0;
DEFINE_FUNC_1(EnableQAVoteup, SDKError, bool, bEnable)
// virtual bool IsQAEnabled() = 0;
DEFINE_FUNC_0(IsQAEnabled, bool)
// virtual bool IsQACommentEnabled() = 0;
DEFINE_FUNC_0(IsQACommentEnabled, bool)
// virtual bool IsQAVoteupEnabled() = 0;
DEFINE_FUNC_0(IsQAVoteupEnabled, bool)
// virtual bool IsAskQuestionAnonymouslyEnabled() = 0;
DEFINE_FUNC_0(IsAskQuestionAnonymouslyEnabled, bool)
// virtual bool IsAttendeeCanViewAllQuestions() = 0;
DEFINE_FUNC_0(IsAttendeeCanViewAllQuestions, bool);
// virtual IList<IQAItemInfo*>* GetAllQuestionList() = 0;
DEFINE_FUNC_0(GetAllQuestionList, IList<IQAItemInfo *> *)
// virtual IQAItemInfo* GetQuestion(const zchar_t *questionID) = 0;
DEFINE_FUNC_1(GetQuestion, IQAItemInfo *, const zchar_t *, questionID)
// virtual IAnswerItem* GetAnswer(const zchar_t *answerID) = 0;
DEFINE_FUNC_1(GetAnswer, IAnswerItem *, const zchar_t *, answerID)
// virtual SDKError VoteupQuestion(const zchar_t *questionID, bool bVokeup) = 0;
// (typo is present in SDK)
DEFINE_FUNC_2(VoteupQuestion, SDKError, const zchar_t *, questionID, bool, bVokeup)
// virtual bool IsQALegalNoticeAvailable() = 0;
DEFINE_FUNC_0(IsQALegalNoticeAvailable, bool)
// virtual const zchar_t* getQALegalNoticesPrompt() = 0;
DEFINE_FUNC_0(getQALegalNoticesPrompt, const zchar_t *)
// virtual const zchar_t* getQALegalNoticesExplained() = 0;
DEFINE_FUNC_0(getQALegalNoticesExplained, const zchar_t *)
// virtual SDKError EnableMeetingQAFeature(bool bEnable) = 0;
DEFINE_FUNC_1(EnableMeetingQAFeature, SDKError, bool, bEnable)
// virtual bool IsMeetingQAFeatureOn() = 0;
DEFINE_FUNC_0(IsMeetingQAFeatureOn, bool)
// virtual SDKError EnableAskQuestion(bool bEnable) = 0;
DEFINE_FUNC_1(EnableAskQuestion, SDKError, bool, bEnable)
// virtual bool IsAskQuestionEnabled() = 0;
DEFINE_FUNC_0(IsAskQuestionEnabled, bool)

// virtual void OnQAConnectStatus(ZOOM_SDK_NAMESPACE::QAConnectStatus
// connectStatus) = 0;
CallBack_FUNC_1(OnQAConnectStatus, ZOOM_SDK_NAMESPACE::QAConnectStatus, connectStatus)
// virtual void OnAddQuestion(const zchar_t* questionID, bool bSuccess) = 0;
CallBack_FUNC_2(OnAddQuestion, const zchar_t *, questionID, bool, bSuccess)
// virtual void OnAddAnswer(const zchar_t *answerID, bool bSuccess) = 0;
CallBack_FUNC_2(OnAddAnswer, const zchar_t *, answerID, bool, bSuccess)
// virtual void OnQuestionMarkedAsDismissed(const zchar_t *question_id) = 0;
CallBack_FUNC_1(OnQuestionMarkedAsDismissed, const zchar_t *, question_id)
// virtual void OnReopenQuestion(const zchar_t * question_id) = 0;
CallBack_FUNC_1(OnReopenQuestion, const zchar_t *, question_id)
// virtual void OnReceiveQuestion(const zchar_t* questionID) = 0;
CallBack_FUNC_1(OnReceiveQuestion, const zchar_t *, questionID)
// virtual void OnReceiveAnswer(const zchar_t *answerID) = 0;
CallBack_FUNC_1(OnReceiveAnswer, const zchar_t *, answerID)
// virtual void OnUserLivingReply(const zchar_t *questionID) = 0;
CallBack_FUNC_1(OnUserLivingReply, const zchar_t *, questionID)
// virtual void OnUserEndLiving(const zchar_t *questionID) = 0;
CallBack_FUNC_1(OnUserEndLiving, const zchar_t *, questionID)
// virtual void OnUpvoteQuestion(const zchar_t *question_id, bool
// order_changed) = 0;
CallBack_FUNC_2(OnUpvoteQuestion, const zchar_t *, question_id, bool, order_changed)
// virtual void OnRevokeUpvoteQuestion(const zchar_t *question_id, bool
// order_changed) = 0;
CallBack_FUNC_2(OnRevokeUpvoteQuestion, const zchar_t *, question_id, bool, order_changed)
// virtual void OnDeleteQuestion(IList<const zchar_t*>* lstQuestionID) = 0;
CallBack_FUNC_1(OnDeleteQuestion, IList<const zchar_t *> *, lstQuestionID)
// virtual void OnDeleteAnswer(IList<const zchar_t*>* lstAnswerID) = 0;
CallBack_FUNC_1(OnDeleteAnswer, IList<const zchar_t *> *, lstAnswerID)
// virtual void OnAllowAskQuestionAnonymousStatus(bool bEnabled) = 0;
CallBack_FUNC_1(OnAllowAskQuestionAnonymousStatus, bool, bEnabled)
// virtual void OnAllowAttendeeViewAllQuestionStatus(bool bEnabled) = 0;
CallBack_FUNC_1(OnAllowAttendeeViewAllQuestionStatus, bool, bEnabled)
// virtual void OnAllowAttendeeVoteupQuestionStatus(bool bEnabled) = 0;
CallBack_FUNC_1(OnAllowAttendeeVoteupQuestionStatus, bool, bEnabled)
// virtual void OnAllowAttendeeCommentQuestionStatus(bool bEnabled) = 0;
CallBack_FUNC_1(OnAllowAttendeeCommentQuestionStatus, bool, bEnabled)
// virtual void OnRefreshQAData() = 0;
CallBack_FUNC_0(OnRefreshQAData)
// virtual void onMeetingQAStatusChanged(bool bEnabled) = 0;
CallBack_FUNC_1(onMeetingQAStatusChanged, bool, bEnabled)
// virtual void onAllowAskQuestionStatus(bool bEnabled) = 0;
CallBack_FUNC_1(onAllowAskQuestionStatus, bool, bEnabled)

END_CLASS_DEFINE(IMeetingQAController) 
END_ZOOM_SDK_NAMESPACE