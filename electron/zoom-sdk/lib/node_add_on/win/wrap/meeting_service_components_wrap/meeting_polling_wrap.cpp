#include "meeting_polling_wrap.h"
#include "meeting_service_wrap.h"
BEGIN_ZOOM_SDK_NAMESPACE
IMeetingPollingController* InitIMeetingPollingControllerFunc(IMeetingPollingCtrlEvent* pEvent, IMeetingServiceWrap* pOwner)
{
	if (pOwner && pOwner->GetSDKObj())
	{
		ZOOM_SDK_NAMESPACE::IMeetingPollingController* pObj = pOwner->GetSDKObj()->GetMeetingPollingController();
		if (pObj)
		{
			pObj->SetEvent(pEvent);
		}
		return pObj;
	}

	return NULL;
}

void UninitIMeetingPollingControllerFunc(IMeetingPollingController* obj) {
	if (obj)
	{
		obj->SetEvent(NULL);
	}
}

//virtual bool CanDoPolling() = 0;
IMPL_FUNC_0(IMeetingPollingController, CanDoPolling, bool, NULL)
//virtual const zchar_t* GetActivePollingID() = 0;
IMPL_FUNC_0(IMeetingPollingController, GetActivePollingID, const zchar_t*, NULL)
//virtual IList<IPollingQuestionItem*>* GetPollingQuestionItemList(const zchar_t* pollingID) = 0;
IMPL_FUNC_1(IMeetingPollingController, GetPollingQuestionItemList, IList<IPollingQuestionItem*>*, const zchar_t*, pollingID, NULL)
//virtual bool CanGetRightAnswerItemList(const zchar_t* pollingID) = 0;
IMPL_FUNC_1(IMeetingPollingController, CanGetRightAnswerItemList, bool, const zchar_t*, pollingID, false)
//virtual IList<IPollingAnswerItem*>* GetPollingRightAnswerItemList(const zchar_t* pollingID) = 0;
IMPL_FUNC_1(IMeetingPollingController, GetPollingRightAnswerItemList, IList<IPollingAnswerItem*>*, const zchar_t*, pollingID, NULL)
//virtual bool CanShowAnswerResultList(const zchar_t* pollingID) = 0;
IMPL_FUNC_1(IMeetingPollingController, CanShowAnswerResultList, bool, const zchar_t*, pollingID, false)
//virtual IList<IPollingAnswerResultItem*>* GetPollingAnswerResultItemList(const zchar_t* pollingID) = 0;
IMPL_FUNC_1(IMeetingPollingController, GetPollingAnswerResultItemList, IList<IPollingAnswerResultItem*>*, const zchar_t*, pollingID, NULL)
//virtual IPollingItem* GetPollingItemByID(const zchar_t* pollingID) = 0;
IMPL_FUNC_1(IMeetingPollingController, GetPollingItemByID, IPollingItem*, const zchar_t*, pollingID, NULL)
//virtual const zchar_t* GetPollingQuestionImagePath(const zchar_t* pollingID, const zchar_t* questionID) = 0;
IMPL_FUNC_2(IMeetingPollingController, GetPollingQuestionImagePath, const zchar_t*, const zchar_t*, pollingID, const zchar_t*, questionID, NULL)
//virtual SDKError GetQuestionCharactersLengthRange(const zchar_t* pollingID, const zchar_t* questionID, unsigned int& minLen, unsigned int& maxLen) = 0;
IMPL_FUNC_4(IMeetingPollingController, GetQuestionCharactersLengthRange, SDKError, const zchar_t*, pollingID, const zchar_t*, questionID, unsigned int&, minLen, unsigned int&, maxLen, SDKERR_UNINITIALIZE)
//virtual bool IsQuestionCaseSensitive(const zchar_t* pollingID, const zchar_t* questionID) = 0;
IMPL_FUNC_2(IMeetingPollingController, IsQuestionCaseSensitive, bool, const zchar_t*, pollingID, const zchar_t*, questionID, NULL)

//virtual bool CanAddPolling() = 0;
IMPL_FUNC_0(IMeetingPollingController, CanAddPolling, bool, false)
//virtual SDKError AddPolling() = 0;
IMPL_FUNC_0(IMeetingPollingController, AddPolling, SDKError, SDKERR_UNINITIALIZE)
//virtual bool CanEditPolling(const zchar_t* pollingID) = 0;
IMPL_FUNC_1(IMeetingPollingController, CanEditPolling, bool, zchar_t*, pollingID, false)
//virtual SDKError EditPolling(const zchar_t* pollingID) = 0;
IMPL_FUNC_1(IMeetingPollingController, EditPolling, SDKError, const zchar_t*, pollingID, SDKERR_UNINITIALIZE)
//virtual bool CanDeletePolling(const zchar_t* pollingID) = 0;
IMPL_FUNC_1(IMeetingPollingController, CanDeletePolling, bool, const zchar_t*, pollingID, false)
//virtual SDKError DeletePolling(const zchar_t* pollingID) = 0;
IMPL_FUNC_1(IMeetingPollingController, DeletePolling, SDKError, const zchar_t*, pollingID, SDKERR_UNINITIALIZE)
//virtual bool CanDuplicatePolling(const zchar_t* pollingID) = 0;
IMPL_FUNC_1(IMeetingPollingController, CanDuplicatePolling, bool, const zchar_t*, pollingID, false)
//virtual SDKError DuplicatePolling(const zchar_t* pollingID) = 0;
IMPL_FUNC_1(IMeetingPollingController, DuplicatePolling, SDKError, const zchar_t*, pollingID, SDKERR_UNINITIALIZE)
//virtual bool CanViewPollingResultFromBrowser(const zchar_t* pollingID) = 0;
IMPL_FUNC_1(IMeetingPollingController, CanViewPollingResultFromBrowser, bool, const zchar_t*, pollingID, false)
//virtual SDKError ViewPollingResultFromBrowser(const zchar_t* pollingID) = 0;
IMPL_FUNC_1(IMeetingPollingController, ViewPollingResultFromBrowser, SDKError, const zchar_t*, pollingID, SDKERR_UNINITIALIZE)
//virtual IList<IPollingItem*>* GetPollingItemList() = 0;
IMPL_FUNC_0(IMeetingPollingController, GetPollingItemList, IList<IPollingItem*>*, NULL)
//virtual bool CanStartPolling(const zchar_t* pollingID) = 0;
IMPL_FUNC_1(IMeetingPollingController, CanStartPolling, bool, const zchar_t*, pollingID, false)
//virtual SDKError StartPolling(const zchar_t* pollingID) = 0;
IMPL_FUNC_1(IMeetingPollingController, StartPolling, SDKError, const zchar_t*, pollingID, SDKERR_UNINITIALIZE)
//virtual SDKError StopPolling(const zchar_t* pollingID) = 0;
IMPL_FUNC_1(IMeetingPollingController, StopPolling, SDKError, const zchar_t*, pollingID, SDKERR_UNINITIALIZE)
//virtual bool CanRestartPolling(const zchar_t* pollingID) = 0;
IMPL_FUNC_1(IMeetingPollingController, CanRestartPolling, bool, const zchar_t*, pollingID, false)
//virtual SDKError RestartPolling(const zchar_t* pollingID) = 0;
IMPL_FUNC_1(IMeetingPollingController, RestartPolling, SDKError, const zchar_t*, pollingID, SDKERR_UNINITIALIZE)
//virtual bool CanSharePollingResult(const zchar_t* pollingID) = 0;
IMPL_FUNC_1(IMeetingPollingController, CanSharePollingResult, bool, const zchar_t*, pollingID, false)
//virtual SDKError StartSharePollingResult(const zchar_t* pollingID) = 0;
IMPL_FUNC_1(IMeetingPollingController, StartSharePollingResult, SDKError, const zchar_t*, pollingID, SDKERR_UNINITIALIZE)
//virtual SDKError StopSharePollingResult(const zchar_t* pollingID) = 0;
IMPL_FUNC_1(IMeetingPollingController, StopSharePollingResult, SDKError, const zchar_t*, pollingID, SDKERR_UNINITIALIZE)
//virtual SDKError EnableGetRightAnswerList(bool bEnable) = 0;
IMPL_FUNC_1(IMeetingPollingController, EnableGetRightAnswerList, SDKError, bool, bEnable, SDKERR_UNINITIALIZE)
//virtual bool CanDownloadResult() = 0;
IMPL_FUNC_0(IMeetingPollingController, CanDownloadResult, bool, false)
//virtual SDKError DownLoadResult() = 0;
IMPL_FUNC_0(IMeetingPollingController, DownLoadResult, SDKError, SDKERR_UNINITIALIZE)

//virtual SDKError SetAnswerCheck(IPollingAnswerItem* answerItem, bool bCheck) = 0;
IMPL_FUNC_2(IMeetingPollingController, SetAnswerCheck, SDKError, IPollingAnswerItem*, answerItem, bool, bCheck, SDKERR_UNINITIALIZE)
//virtual SDKError SetAnswerContent(IPollingAnswerItem* answerItem, const zchar_t* answerText) = 0;
IMPL_FUNC_2(IMeetingPollingController, SetAnswerContent, SDKError, IPollingAnswerItem*, answerItem, const zchar_t*, answerText, SDKERR_UNINITIALIZE)
//virtual bool CanSubmitPolling(const zchar_t* pollingID) = 0;
IMPL_FUNC_1(IMeetingPollingController, CanSubmitPolling, bool, const zchar_t*, pollingID, false)
//virtual SDKError SubmitPolling(const zchar_t* pollingID) = 0;
IMPL_FUNC_1(IMeetingPollingController, SubmitPolling, SDKError, const zchar_t*, pollingID, SDKERR_UNINITIALIZE)

END_ZOOM_SDK_NAMESPACE