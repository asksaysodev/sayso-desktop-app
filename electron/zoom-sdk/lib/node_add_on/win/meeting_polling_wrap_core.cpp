#include "meeting_polling_wrap_core.h"
#include "sdk_wrap.h"
#include "meeting_service_components_wrap/meeting_polling_wrap.h"
#include "zoom_native_to_wrap.h"
#include "sdk_events_wrap_class.h"

extern ZOOM_SDK_NAMESPACE::IMeetingServiceWrap& g_meeting_service_wrap;

ZMeetingPollingWrap::ZMeetingPollingWrap()
{
	SDKEventWrapMgr::GetInst().m_meetingPollingCtrlEvent.SetOwner(this);
	m_pSink = 0;
}
ZMeetingPollingWrap::~ZMeetingPollingWrap()
{
	Uninit();
	m_pSink = 0;
	SDKEventWrapMgr::GetInst().m_meetingPollingCtrlEvent.SetOwner(this);
}
void ZMeetingPollingWrap::Init()
{
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingPollingController().Init_Wrap(&g_meeting_service_wrap);
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingPollingController().SetEvent(&SDKEventWrapMgr::GetInst().m_meetingPollingCtrlEvent);
}
void ZMeetingPollingWrap::Uninit()
{
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingPollingController().SetEvent(NULL);
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingPollingController().Uninit_Wrap();
}
void ZMeetingPollingWrap::SetSink(ZNativeSDKPollingWrapSink* pSink)
{
	m_pSink = pSink;
}

bool ZMeetingPollingWrap::CanDoPolling() {
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingPollingController().CanDoPolling();
}

ZNList<ZNSDKPollingQuestionItem> ZMeetingPollingWrap::GetPollingQuestionItemList(ZoomSTRING pollingID)
{
	ZNList<ZNSDKPollingQuestionItem> zn_polling_question_info_list;
	ZOOM_SDK_NAMESPACE::IList<ZOOM_SDK_NAMESPACE::IPollingQuestionItem*>* questionList = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingPollingController().GetPollingQuestionItemList(pollingID.c_str());
	
	if (NULL == questionList) {
		return zn_polling_question_info_list;
	}
	for (int i = 0; i < questionList->GetCount(); ++i) {
		ZNSDKPollingQuestionItem zn_question_info;
		ZOOM_SDK_NAMESPACE::IPollingQuestionItem* question = questionList->GetItem(i);
		if (question) {
			zn_question_info = ConvertSDKIPollingQuestionItemToZNSDKPollingQuestionItem(question);
			// sub questions
			ZNList<ZNSDKPollingQuestionItem> zn_sub_polling_question_info_list;
			ZOOM_SDK_NAMESPACE::IList<ZOOM_SDK_NAMESPACE::IPollingQuestionItem*>* subQuestionList = question->GetPollingSubQuestionItemList();
			if (NULL != subQuestionList) {
				for (int j = 0; j < subQuestionList->GetCount(); ++j) {
					ZNSDKPollingQuestionItem sub_zn_question_info;
					ZOOM_SDK_NAMESPACE::IPollingQuestionItem* subQuestion = subQuestionList->GetItem(j);
					if (subQuestion) {
						sub_zn_question_info = ConvertSDKIPollingQuestionItemToZNSDKPollingQuestionItem(subQuestion);
						// sub's sub question list set to empty
						ZNList<ZNSDKPollingQuestionItem> zn_sub_sub_polling_question_info_list;
						sub_zn_question_info.pollingSubQuestionItemList = zn_sub_sub_polling_question_info_list;

						// sub's answer list
						ZNList<ZNSDKPollingAnswerItem> zn_sub_answer_info_list;
						ZOOM_SDK_NAMESPACE::IList<ZOOM_SDK_NAMESPACE::IPollingAnswerItem*>*subAnswerList = subQuestion->GetPollingAnswerItemList();
						if (NULL != subAnswerList) {
							for (int m = 0; m < subAnswerList->GetCount(); ++m) {
								ZNSDKPollingAnswerItem sub_zn_answer_info;
								ZOOM_SDK_NAMESPACE::IPollingAnswerItem* subAnswer = subAnswerList->GetItem(m);
								if (subAnswer) {
									sub_zn_answer_info = ConvertSDKIPollingAnswerItemToZNSDKPollingAnswerItem(subAnswer);
									zn_sub_answer_info_list.push_back(sub_zn_answer_info);
								}
							}
						}
						sub_zn_question_info.pollingAnswerItemList = zn_sub_answer_info_list;

						zn_sub_polling_question_info_list.push_back(sub_zn_question_info);
					}
				}
			}
			zn_question_info.pollingSubQuestionItemList = zn_sub_polling_question_info_list;


			// answers
			ZNList<ZNSDKPollingAnswerItem> zn_answer_info_list;
			ZOOM_SDK_NAMESPACE::IList<ZOOM_SDK_NAMESPACE::IPollingAnswerItem*>* answerList = question->GetPollingAnswerItemList();
			if (NULL != answerList) {
				for (int i = 0; i < answerList->GetCount(); ++i) {
					ZNSDKPollingAnswerItem zn_answer_info;
					ZOOM_SDK_NAMESPACE::IPollingAnswerItem* answer = answerList->GetItem(i);
					if (answer) {
						zn_answer_info = ConvertSDKIPollingAnswerItemToZNSDKPollingAnswerItem(answer);
						zn_answer_info_list.push_back(zn_answer_info);
					}

				}
			}
			zn_question_info.pollingAnswerItemList = zn_answer_info_list;

			zn_polling_question_info_list.push_back(zn_question_info);
		}
	}
	return zn_polling_question_info_list;
}
ZNList<ZNSDKPollingAnswerItem> ZMeetingPollingWrap::GetPollingRightAnswerItemList(ZoomSTRING pollingID)
{
	ZNList<ZNSDKPollingAnswerItem> zn_answer_info_list;
	ZOOM_SDK_NAMESPACE::IList<ZOOM_SDK_NAMESPACE::IPollingAnswerItem*>* answerList = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingPollingController().GetPollingRightAnswerItemList(pollingID.c_str());
	
	if (NULL == answerList) {
		return zn_answer_info_list;
	}
	for (int i = 0; i < answerList->GetCount(); ++i) {
		ZNSDKPollingAnswerItem zn_answer_info;
		ZOOM_SDK_NAMESPACE::IPollingAnswerItem* answer = answerList->GetItem(i);
		if (answer) {
			zn_answer_info = ConvertSDKIPollingAnswerItemToZNSDKPollingAnswerItem(answer);
			zn_answer_info_list.push_back(zn_answer_info);
		}
	}
	return zn_answer_info_list;
}
ZNList<ZNSDKPollingAnswerResultItem> ZMeetingPollingWrap::GetPollingAnswerResultItemList(ZoomSTRING pollingID)
{
	ZNList<ZNSDKPollingAnswerResultItem> zn_answer_result_info_list;
	ZOOM_SDK_NAMESPACE::IList<ZOOM_SDK_NAMESPACE::IPollingAnswerResultItem*>* answerResultList = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingPollingController().GetPollingAnswerResultItemList(pollingID.c_str());
	if (NULL == answerResultList) {
		return zn_answer_result_info_list;
	}
	for (int i = 0; i < answerResultList->GetCount(); ++i) {
		ZNSDKPollingAnswerResultItem zn_answer_result_info;
		ZOOM_SDK_NAMESPACE::IPollingAnswerResultItem* answerResult = answerResultList->GetItem(i);
		if (answerResult) {
			zn_answer_result_info = ConvertSDKIPollingAnswerResultItemToZNSDKPollingAnswerResultItem(answerResult);
			zn_answer_result_info_list.push_back(zn_answer_result_info);
		}
	}
	return zn_answer_result_info_list;
}
ZNSDKPollingItem ZMeetingPollingWrap::GetPollingItemByID(ZoomSTRING pollingID)
{
	ZNSDKPollingItem zn_polling_info;
	ZOOM_SDK_NAMESPACE::IPollingItem* poll = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingPollingController().GetPollingItemByID(pollingID.c_str());
	if (poll) {
		zn_polling_info = ConvertSDKIPollingItemToZNSDKPollingItem(poll);
	}
	return zn_polling_info;
}
ZNList<ZNSDKPollingItem> ZMeetingPollingWrap::GetPollingItemList()
{
	ZNList<ZNSDKPollingItem> zn_polling_info_list;
	ZOOM_SDK_NAMESPACE::IList<ZOOM_SDK_NAMESPACE::IPollingItem*>* pollList = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingPollingController().GetPollingItemList();
	if (NULL == pollList) {
		return zn_polling_info_list;
	}
	for (int i = 0; i < pollList->GetCount(); ++i) {
		ZNSDKPollingItem zn_polling_info;
		ZOOM_SDK_NAMESPACE::IPollingItem* poll = pollList->GetItem(i);
		if (poll) {
			zn_polling_info = ConvertSDKIPollingItemToZNSDKPollingItem(poll);
			zn_polling_info_list.push_back(zn_polling_info);
		}
	}
	return zn_polling_info_list;
}
ZNSDKError ZMeetingPollingWrap::StartPolling(ZoomSTRING pollingID)
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN;
	sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingPollingController().StartPolling(pollingID.c_str());
	return Map2WrapDefine(sdkerr);
}
ZNSDKError ZMeetingPollingWrap::StopPolling(ZoomSTRING pollingID)
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN;
	sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingPollingController().StopPolling(pollingID.c_str());
	return Map2WrapDefine(sdkerr);
}
ZNSDKError ZMeetingPollingWrap::RestartPolling(ZoomSTRING pollingID)
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN;
	sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingPollingController().RestartPolling(pollingID.c_str());
	return Map2WrapDefine(sdkerr);
}
bool ZMeetingPollingWrap::IsQuestionCaseSensitive(ZoomSTRING pollingID, ZoomSTRING questionID)
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingPollingController().IsQuestionCaseSensitive(pollingID.c_str(), questionID.c_str());
}
ZoomSTRING ZMeetingPollingWrap::GetPollingQuestionImagePath(ZoomSTRING pollingID, ZoomSTRING questionID)
{
	const zchar_t* questionImagePath = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingPollingController().GetPollingQuestionImagePath(pollingID.c_str(), questionID.c_str());
	ZoomSTRING zn_questionImagePath;
	if (questionImagePath)
	{
		zn_questionImagePath = questionImagePath;
	}
	return zn_questionImagePath;
}
bool ZMeetingPollingWrap::CanGetRightAnswerItemList(ZoomSTRING pollingID)
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingPollingController().CanGetRightAnswerItemList(pollingID.c_str());
}

void ZMeetingPollingWrap::onPollingStatusChanged(ZoomSTRING pollingID, ZNSDKPollingStatus status) 
{
	if (m_pSink)
	{
		m_pSink->onPollingStatusChanged(pollingID.c_str(), status);
	}
}
void ZMeetingPollingWrap::onPollingResultUpdated(ZoomSTRING pollingID) 
{
	if (m_pSink)
	{
		m_pSink->onPollingResultUpdated(pollingID.c_str());
	}
}
void ZMeetingPollingWrap::onPollingListUpdated() 
{
	if (m_pSink)
	{
		m_pSink->onPollingListUpdated();
	}
}
void ZMeetingPollingWrap::onPollingActionResult(ZNSDKPollingActionType actionType, ZoomSTRING pollingID, bool bSuccess, ZoomSTRING errorMsg) 
{
	if (m_pSink)
	{
		m_pSink->onPollingActionResult(actionType, pollingID.c_str(), bSuccess, errorMsg.c_str());
	}
}