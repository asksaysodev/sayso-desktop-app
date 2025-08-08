#pragma once
#include "common_include.h"
BEGIN_ZOOM_SDK_NAMESPACE
class IMeetingServiceWrap;
IMeetingPollingController* InitIMeetingPollingControllerFunc(IMeetingPollingCtrlEvent* pEvent, IMeetingServiceWrap* pOwner);
void UninitIMeetingPollingControllerFunc(IMeetingPollingController* obj);
BEGIN_CLASS_DEFINE_WITHCALLBACK(IMeetingPollingController, IMeetingPollingCtrlEvent)
NORMAL_CLASS(IMeetingPollingController)
INIT_UNINIT_WITHEVENT_AND_OWNSERVICE(IMeetingPollingController, IMeetingServiceWrap)
virtual SDKError SetEvent(IMeetingPollingCtrlEvent* pEvent)
{
	external_cb = pEvent;
	return SDKERR_SUCCESS;
}

//Common (for all)

//virtual bool CanDoPolling() = 0;
DEFINE_FUNC_0(CanDoPolling, bool)
//virtual const zchar_t* GetActivePollingID() = 0;
DEFINE_FUNC_0(GetActivePollingID, const zchar_t*)
//virtual IList<IPollingQuestionItem*>* GetPollingQuestionItemList(const zchar_t* pollingID) = 0;
DEFINE_FUNC_1(GetPollingQuestionItemList, IList<IPollingQuestionItem*>*, const zchar_t*, pollingID)
//virtual bool CanGetRightAnswerItemList(const zchar_t* pollingID) = 0;
DEFINE_FUNC_1(CanGetRightAnswerItemList, bool, const zchar_t*, pollingID)
//virtual IList<IPollingAnswerItem*>* GetPollingRightAnswerItemList(const zchar_t* pollingID) = 0;
DEFINE_FUNC_1(GetPollingRightAnswerItemList, IList<IPollingAnswerItem*>*, const zchar_t*, pollingID)
//virtual bool CanShowAnswerResultList(const zchar_t* pollingID) = 0;
DEFINE_FUNC_1(CanShowAnswerResultList, bool, const zchar_t*, pollingID)
//virtual IList<IPollingAnswerResultItem*>* GetPollingAnswerResultItemList(const zchar_t* pollingID) = 0;
DEFINE_FUNC_1(GetPollingAnswerResultItemList, IList<IPollingAnswerResultItem*>*, const zchar_t*, pollingID)
//virtual IPollingItem* GetPollingItemByID(const zchar_t* pollingID) = 0;
DEFINE_FUNC_1(GetPollingItemByID, IPollingItem*, const zchar_t*, pollingID)
//virtual const zchar_t* GetPollingQuestionImagePath(const zchar_t* pollingID, const zchar_t* questionID) = 0;
DEFINE_FUNC_2(GetPollingQuestionImagePath, const zchar_t*, const zchar_t*, pollingID, const zchar_t*, questionID)
//virtual SDKError GetQuestionCharactersLengthRange(const zchar_t* pollingID, const zchar_t* questionID, unsigned int& minLen, unsigned int& maxLen) = 0;
DEFINE_FUNC_4(GetQuestionCharactersLengthRange, SDKError, const zchar_t*, pollingID, const zchar_t*, questionID, unsigned int&, minLen, unsigned int&, maxLen)
//virtual bool IsQuestionCaseSensitive(const zchar_t* pollingID, const zchar_t* questionID) = 0;
DEFINE_FUNC_2(IsQuestionCaseSensitive, bool, const zchar_t*, pollingID, const zchar_t*, questionID)

//Host 

//virtual bool CanAddPolling() = 0;
DEFINE_FUNC_0(CanAddPolling, bool)
//virtual SDKError AddPolling() = 0;
DEFINE_FUNC_0(AddPolling, SDKError)
//virtual bool CanEditPolling(const zchar_t* pollingID) = 0;
DEFINE_FUNC_1(CanEditPolling, bool, zchar_t*, pollingID)
//virtual SDKError EditPolling(const zchar_t* pollingID) = 0;
DEFINE_FUNC_1(EditPolling, SDKError, const zchar_t*, pollingID)
//virtual bool CanDeletePolling(const zchar_t* pollingID) = 0;
DEFINE_FUNC_1(CanDeletePolling, bool, const zchar_t*, pollingID)
//virtual SDKError DeletePolling(const zchar_t* pollingID) = 0;
DEFINE_FUNC_1(DeletePolling, SDKError, const zchar_t*, pollingID)
//virtual bool CanDuplicatePolling(const zchar_t* pollingID) = 0;
DEFINE_FUNC_1(CanDuplicatePolling, bool, const zchar_t*, pollingID)
//virtual SDKError DuplicatePolling(const zchar_t* pollingID) = 0;
DEFINE_FUNC_1(DuplicatePolling, SDKError, const zchar_t*, pollingID)
//virtual bool CanViewPollingResultFromBrowser(const zchar_t* pollingID) = 0;
DEFINE_FUNC_1(CanViewPollingResultFromBrowser, bool, const zchar_t*, pollingID)
//virtual SDKError ViewPollingResultFromBrowser(const zchar_t* pollingID) = 0;
DEFINE_FUNC_1(ViewPollingResultFromBrowser, SDKError, const zchar_t*, pollingID)
//virtual IList<IPollingItem*>* GetPollingItemList() = 0;
DEFINE_FUNC_0(GetPollingItemList, IList<IPollingItem*>*)
//virtual bool CanStartPolling(const zchar_t* pollingID) = 0;
DEFINE_FUNC_1(CanStartPolling, bool, const zchar_t*, pollingID)
//virtual SDKError StartPolling(const zchar_t* pollingID) = 0;
DEFINE_FUNC_1(StartPolling, SDKError, const zchar_t*, pollingID)
//virtual SDKError StopPolling(const zchar_t* pollingID) = 0;
DEFINE_FUNC_1(StopPolling, SDKError, const zchar_t*, pollingID)
//virtual bool CanRestartPolling(const zchar_t* pollingID) = 0;
DEFINE_FUNC_1(CanRestartPolling, bool, const zchar_t*, pollingID)
//virtual SDKError RestartPolling(const zchar_t* pollingID) = 0;
DEFINE_FUNC_1(RestartPolling, SDKError, const zchar_t*, pollingID)
//virtual bool CanSharePollingResult(const zchar_t* pollingID) = 0;
DEFINE_FUNC_1(CanSharePollingResult, bool, const zchar_t*, pollingID)
//virtual SDKError StartSharePollingResult(const zchar_t* pollingID) = 0;
DEFINE_FUNC_1(StartSharePollingResult, SDKError, const zchar_t*, pollingID)
//virtual SDKError StopSharePollingResult(const zchar_t* pollingID) = 0;
DEFINE_FUNC_1(StopSharePollingResult, SDKError, const zchar_t*, pollingID)
//virtual SDKError EnableGetRightAnswerList(bool bEnable) = 0;
DEFINE_FUNC_1(EnableGetRightAnswerList, SDKError, bool, bEnable)
//virtual bool CanDownloadResult() = 0;
DEFINE_FUNC_0(CanDownloadResult, bool)
//virtual SDKError DownLoadResult() = 0;
DEFINE_FUNC_0(DownLoadResult, SDKError)

//Attendee

//virtual SDKError SetAnswerCheck(IPollingAnswerItem* answerItem, bool bCheck) = 0;
DEFINE_FUNC_2(SetAnswerCheck, SDKError, IPollingAnswerItem*, answerItem, bool, bCheck)
//virtual SDKError SetAnswerContent(IPollingAnswerItem* answerItem, const zchar_t* answerText) = 0;
DEFINE_FUNC_2(SetAnswerContent, SDKError, IPollingAnswerItem*, answerItem, const zchar_t*, answerText)
//virtual bool CanSubmitPolling(const zchar_t* pollingID) = 0;
DEFINE_FUNC_1(CanSubmitPolling, bool, const zchar_t*, pollingID)
//virtual SDKError SubmitPolling(const zchar_t* pollingID) = 0;
DEFINE_FUNC_1(SubmitPolling, SDKError, const zchar_t*, pollingID)

//virtual void onPollingStatusChanged(const zchar_t* pollingID, SDKPollingStatus status) = 0;
CallBack_FUNC_2(onPollingStatusChanged, const zchar_t*, pollingID, SDKPollingStatus, status)
//virtual void onPollingResultUpdated(const zchar_t* pollingID) = 0;
CallBack_FUNC_1(onPollingResultUpdated, const zchar_t*, pollingID)
//virtual void onPollingListUpdated() = 0;
CallBack_FUNC_0(onPollingListUpdated)
//virtual void onPollingActionResult(SDKPollingActionType actionType, const zchar_t* pollingID, bool bSuccess, const zchar_t* errorMsg) = 0;
CallBack_FUNC_4(onPollingActionResult, SDKPollingActionType, actionType, const zchar_t*, pollingID, bool, bSuccess, const zchar_t*, errorMsg)
//virtual void onPollingQuestionImageDownloaded(const zchar_t* questionID, const zchar_t* path) = 0;
CallBack_FUNC_2(onPollingQuestionImageDownloaded, const zchar_t*, questionID, const zchar_t*, path)
//virtual void onPollingElapsedTime(const zchar_t* pollingID, unsigned int uElapsedtime) = 0;
CallBack_FUNC_2(onPollingElapsedTime, const zchar_t*, pollingID, unsigned int, uElapsedtime)
//virtual void onGetRightAnswerListPrivilege(bool bCan) = 0;
CallBack_FUNC_1(onGetRightAnswerListPrivilege, bool, bCan)
//virtual void onPollingInactive() = 0;
CallBack_FUNC_0(onPollingInactive)

END_CLASS_DEFINE(IMeetingPollingController)
END_ZOOM_SDK_NAMESPACE