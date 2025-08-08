#pragma once
#include "zoom_native_sdk_wrap_core_def.h"
#include "zoom_sinks_wrap_class.h"

class ZMeetingQAWrap {
public:
	ZMeetingQAWrap();
	virtual ~ZMeetingQAWrap();
	void Init();
	void Uninit();
	void SetSink(ZNativeSDKMeetingQAWrapSink *pSink);

	// interface methods
	ZNQAItemInfo GetQuestion(ZoomSTRING questionID);
	ZNAnswerItem GetAnswer(ZoomSTRING answerID);
	ZNList<ZNQAItemInfo> getAllQuestionList();
	ZNSDKError answerQuestionPublicWithQuestionID(ZoomSTRING questionID, ZoomSTRING content);
	ZNSDKError answerQuestionPrivateWithQuestionID(ZoomSTRING questionID, ZoomSTRING content);
	ZNSDKError StartLiving(ZoomSTRING questionID);
	ZNSDKError EndLiving(ZoomSTRING questionID);
	ZNSDKError DismissQuestionWithQuestionID(ZoomSTRING questionID);
	ZNSDKError DeleteQuestion(ZoomSTRING questionID);
	ZNSDKError DeleteAnswer(ZoomSTRING answerID);
	ZNSDKError ReopenQuestionWithQuestionID(ZoomSTRING zn_questionID);
	ZNSDKError VoteupQuestionWithQuestionID(ZoomSTRING zn_questionID, bool enabled);


	// callbacks
	void onAddQuestion(ZoomSTRING questionID, bool bSuccess);
	void onReceiveQuestion(ZoomSTRING questionID);
	void onDeleteQuestions(ZNList<ZoomSTRING> questions);
	void onQuestionMarkedAsDismissed(ZoomSTRING questionID);
	void onReopenQuestion(ZoomSTRING questionID);
	void onUserLivingReply(ZoomSTRING questionID);
	void onUserEndLiving(ZoomSTRING questionID);
	void onAddAnswer(ZoomSTRING answerID, bool bSuccess);
	void onReceiveAnswer(ZoomSTRING answerID);
	void onDeleteAnswers(ZNList<ZoomSTRING> answer);
	void onVoteupQuestion(ZoomSTRING questionID, bool isChanged);

	// This callback is used for both MacOS and Windows platform. (Windows SDK Docs spells it as OnRevokeUpvoteQuestion)
	void onRevokeVoteupQuestion(ZoomSTRING questionID, bool isChanged);

private:
	ZNativeSDKMeetingQAWrapSink *m_pSink;
};