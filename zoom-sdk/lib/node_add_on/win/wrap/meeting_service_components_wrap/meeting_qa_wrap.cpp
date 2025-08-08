#include "meeting_qa_wrap.h"
#include "meeting_service_wrap.h"
BEGIN_ZOOM_SDK_NAMESPACE
IMeetingQAController *
InitIMeetingQAControllerFunc(IMeetingQAControllerEvent *pEvent, IMeetingServiceWrap *pOwner) 
{
	if (pOwner && pOwner->GetSDKObj()) {
		ZOOM_SDK_NAMESPACE::IMeetingQAController *pObj = pOwner->GetSDKObj()->GetMeetingQAController();
		if (pObj) {
			pObj->SetEvent(pEvent);
		}
		return pObj;
	}

	return NULL;
}

void UninitIMeetingQAControllerFunc(IMeetingQAController *obj) 
{
	if (obj) {
		obj->SetEvent(NULL);
	}
}

// virtual SDKError AddQuestion(const zchar_t *questionContent, bool
// bAskAnonymous) = 0;
IMPL_FUNC_2(IMeetingQAController, AddQuestion, SDKError, const zchar_t *, questionContent, bool, bAskAnonymous, SDKERR_UNINITIALIZE)
// virtual int GetMyQuestionCount() = 0;
IMPL_FUNC_0(IMeetingQAController, GetMyQuestionCount, int, NULL)
// virtual IList<IQAItemInfo*>* GetMyQuestionList() = 0;
IMPL_FUNC_0(IMeetingQAController, GetMyQuestionList, IList<IQAItemInfo *> *, NULL)
// virtual SDKError CommentQuestion(const zchar_t *questionID, const zchar_t*
// commentContent) = 0;
IMPL_FUNC_2(IMeetingQAController, CommentQuestion, SDKError, const zchar_t *, questionID, const zchar_t *, commentContent, SDKERR_UNINITIALIZE)
// virtual SDKError AnswerQuestionPrivate(const zchar_t *questionID, const
// zchar_t *answerContent) = 0;
IMPL_FUNC_2(IMeetingQAController, AnswerQuestionPrivate, SDKError, const zchar_t *, questionID, const zchar_t *, answerContent, SDKERR_UNINITIALIZE)
// virtual SDKError AnswerQuestionPublic(const zchar_t *questionID, const
// zchar_t *answerContent) = 0;
IMPL_FUNC_2(IMeetingQAController, AnswerQuestionPublic, SDKError, const zchar_t *, questionID, const zchar_t *, answerContent, SDKERR_UNINITIALIZE)
// virtual SDKError DismissQuestion(const zchar_t *questionID) = 0;
IMPL_FUNC_1(IMeetingQAController, DismissQuestion, SDKError, const zchar_t *, questionID, SDKERR_UNINITIALIZE)
// virtual SDKError DeleteQuestion(const zchar_t *questionID) = 0;
IMPL_FUNC_1(IMeetingQAController, DeleteQuestion, SDKError, const zchar_t *, questionID, SDKERR_UNINITIALIZE)
// virtual SDKError DeleteAnswer(const zchar_t *answerID) = 0;
IMPL_FUNC_1(IMeetingQAController, DeleteAnswer, SDKError, const zchar_t *, answerID, SDKERR_UNINITIALIZE)
// virtual SDKError ReopenQuestion(const zchar_t *questionID) = 0;
IMPL_FUNC_1(IMeetingQAController, ReopenQuestion, SDKError, const zchar_t *, questionID, SDKERR_UNINITIALIZE)
// virtual SDKError StartLiving(const zchar_t *questionID) = 0;
IMPL_FUNC_1(IMeetingQAController, StartLiving, SDKError, const zchar_t *, questionID, SDKERR_UNINITIALIZE)
// virtual SDKError EndLiving(const zchar_t *questionID) = 0;
IMPL_FUNC_1(IMeetingQAController, EndLiving, SDKError, const zchar_t *, questionID, SDKERR_UNINITIALIZE)
// virtual int GetOpenQuestionCount() = 0;
IMPL_FUNC_0(IMeetingQAController, GetOpenQuestionCount, int, NULL)
// virtual int GetDismissedQuestionCount() = 0;
IMPL_FUNC_0(IMeetingQAController, GetDismissedQuestionCount, int, NULL)
// virtual int GetAnsweredQuestionCount() = 0;
IMPL_FUNC_0(IMeetingQAController, GetAnsweredQuestionCount, int, NULL)
// virtual IList<IQAItemInfo*>* GetOpenQuestionList() = 0;
IMPL_FUNC_0(IMeetingQAController, GetOpenQuestionList, IList<IQAItemInfo *> *, NULL)
// virtual IList<IQAItemInfo*>* GetDismissedQuestionList() = 0;
IMPL_FUNC_0(IMeetingQAController, GetDismissedQuestionList, IList<IQAItemInfo *> *, NULL)
// virtual IList<IQAItemInfo*>* GetAnsweredQuestionList() = 0;
IMPL_FUNC_0(IMeetingQAController, GetAnsweredQuestionList, IList<IQAItemInfo *> *, NULL)

IMPL_FUNC_1(IMeetingQAController, EnableAnonymousQuestion, SDKError, bool, bEnable, SDKERR_UNINITIALIZE)
// virtual SDKError EnableAttendeeViewAllQuestion(bool bEnable) = 0;
IMPL_FUNC_1(IMeetingQAController, EnableAttendeeViewAllQuestion, SDKError, bool, bEnable, SDKERR_UNINITIALIZE)
// virtual SDKError EnableQAComment(bool bEnable) = 0;
IMPL_FUNC_1(IMeetingQAController, EnableQAComment, SDKError, bool, bEnable, SDKERR_UNINITIALIZE)
// virtual SDKError EnableQAVoteup(bool bEnable) = 0;
IMPL_FUNC_1(IMeetingQAController, EnableQAVoteup, SDKError, bool, bEnable, SDKERR_UNINITIALIZE)
// virtual bool IsQAEnabled() = 0;
IMPL_FUNC_0(IMeetingQAController, IsQAEnabled, bool, false)
// virtual bool IsQACommentEnabled() = 0;
IMPL_FUNC_0(IMeetingQAController, IsQACommentEnabled, bool, false)
// virtual bool IsQAVoteupEnabled() = 0;
IMPL_FUNC_0(IMeetingQAController, IsQAVoteupEnabled, bool, false)
// virtual bool IsAskQuestionAnonymouslyEnabled() = 0;
IMPL_FUNC_0(IMeetingQAController, IsAskQuestionAnonymouslyEnabled, bool, false)
// virtual bool IsAttendeeCanViewAllQuestions() = 0;
IMPL_FUNC_0(IMeetingQAController, IsAttendeeCanViewAllQuestions, bool, false)
// virtual IList<IQAItemInfo*>* GetAllQuestionList() = 0;
IMPL_FUNC_0(IMeetingQAController, GetAllQuestionList, IList<IQAItemInfo *> *, NULL)
// virtual IQAItemInfo* GetQuestion(const zchar_t *questionID) = 0;
IMPL_FUNC_1(IMeetingQAController, GetQuestion, IQAItemInfo *, const zchar_t *, questionID, NULL)
// virtual IAnswerItem* GetAnswer(const zchar_t *answerID) = 0;
IMPL_FUNC_1(IMeetingQAController, GetAnswer, IAnswerItem *, const zchar_t *, answerID, NULL)
// virtual SDKError VoteupQuestion(const zchar_t *questionID, bool bVokeup) = 0;
// (typo is present in SDK)
IMPL_FUNC_2(IMeetingQAController, VoteupQuestion, SDKError, const zchar_t *, questionID, bool, bVokeup, SDKERR_UNINITIALIZE)
// virtual bool IsQALegalNoticeAvailable() = 0;
IMPL_FUNC_0(IMeetingQAController, IsQALegalNoticeAvailable, bool, false)
// virtual const zchar_t* getQALegalNoticesPrompt() = 0;
IMPL_FUNC_0(IMeetingQAController, getQALegalNoticesPrompt, const zchar_t *, NULL)
// virtual const zchar_t* getQALegalNoticesExplained() = 0;
IMPL_FUNC_0(IMeetingQAController, getQALegalNoticesExplained, const zchar_t *, NULL)
// virtual SDKError EnableMeetingQAFeature(bool bEnable) = 0;
IMPL_FUNC_1(IMeetingQAController, EnableMeetingQAFeature, SDKError, bool, bEnable, SDKERR_UNINITIALIZE)
// virtual bool IsMeetingQAFeatureOn() = 0;
IMPL_FUNC_0(IMeetingQAController, IsMeetingQAFeatureOn, bool, false)
// virtual SDKError EnableAskQuestion(bool bEnable) = 0;
IMPL_FUNC_1(IMeetingQAController, EnableAskQuestion, SDKError, bool, bEnable, SDKERR_UNINITIALIZE)
// virtual bool IsAskQuestionEnabled() = 0;
IMPL_FUNC_0(IMeetingQAController, IsAskQuestionEnabled, bool, false)



END_ZOOM_SDK_NAMESPACE