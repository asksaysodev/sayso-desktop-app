#pragma once
#include "common_include.h"

BEGIN_ZOOM_SDK_NAMESPACE

class IMeetingLiveStreamItemImpl : public IMeetingLiveStreamItem
{
public:
	IMeetingLiveStreamItemImpl(const zchar_t* liveStreamURL, const zchar_t* liveStreamURLDescription, const zchar_t* liveStreamViewerURL);
	virtual ~IMeetingLiveStreamItemImpl() {};

	virtual void SetLiveStreamURL(const zchar_t* liveStreamURL);
	virtual const zchar_t* GetLiveStreamURL();
	virtual void SetLiveStreamURLDescription(const zchar_t* liveStreamURLDescription);
	virtual const zchar_t* GetLiveStreamURLDescription();
	virtual void SetLiveStreamViewerURL(const zchar_t* liveStreamViewerURL);
	virtual const zchar_t* GetLiveStreamViewerURL();

private:
	std::wstring _liveStreamURL;
	std::wstring _liveStreamURLDescription;
	std::wstring _liveStreamViewerURL;
};


class IMeetingServiceWrap;
IMeetingLiveStreamController* InitIMeetingLiveStreamControllerFunc(IMeetingLiveStreamCtrlEvent* pEvent, IMeetingServiceWrap* pOwner);
void UninitIMeetingLiveStreamControllerFunc(IMeetingLiveStreamController* obj);
BEGIN_CLASS_DEFINE_WITHCALLBACK(IMeetingLiveStreamController, IMeetingLiveStreamCtrlEvent)
NORMAL_CLASS(IMeetingLiveStreamController)
INIT_UNINIT_WITHEVENT_AND_OWNSERVICE(IMeetingLiveStreamController, IMeetingServiceWrap)
virtual SDKError SetEvent(IMeetingLiveStreamCtrlEvent* pEvent)
{
	external_cb = pEvent;
	return SDKERR_SUCCESS;
}
// virtual SDKError CanStartLiveStream() = 0;
DEFINE_FUNC_0(CanStartLiveStream, SDKError)
// virtual SDKError StartLiveStream(IMeetingLiveStreamItem* item_) = 0;
DEFINE_FUNC_1(StartLiveStream, SDKError, IMeetingLiveStreamItem*, item_)
// virtual SDKError StartLiveStreamWithSteamingURL(const zchar_t* streamingURL, const zchar_t* streamingKey, const zchar_t* broadcastURL) = 0;
DEFINE_FUNC_3(StartLiveStreamWithSteamingURL, SDKError, const zchar_t*, streamingURL, const zchar_t*, streamingKey, const zchar_t*, broadcastURL)
// virtual SDKError StopLiveStream() = 0;
DEFINE_FUNC_0(StopLiveStream, SDKError)
// virtual IList<IMeetingLiveStreamItem* >* GetSupportLiveStreamURL() = 0;
DEFINE_FUNC_0(GetSupportLiveStreamURL, IList<IMeetingLiveStreamItem*>*)
// virtual IList<IMeetingLiveStreamItem* >* GetSupportLiveStreamItems() = 0;
DEFINE_FUNC_0(GetSupportLiveStreamItems, IList<IMeetingLiveStreamItem*>*)
// virtual IMeetingLiveStreamItem* GetCurrentLiveStreamItem() = 0;
DEFINE_FUNC_0(GetCurrentLiveStreamItem, IMeetingLiveStreamItem*)
// virtual LiveStreamStatus GetCurrentLiveStreamStatus() = 0;
DEFINE_FUNC_0(GetCurrentLiveStreamStatus, LiveStreamStatus)
//virtual SDKError StopRawLiveStream() = 0;
DEFINE_FUNC_0(StopRawLiveStream, SDKError)
//virtual bool IsRawLiveStreamSupported() = 0;
DEFINE_FUNC_0(IsRawLiveStreamSupported, bool)
//virtual SDKError CanStartRawLiveStream() = 0;
DEFINE_FUNC_0(CanStartRawLiveStream, SDKError)
//virtual SDKError RemoveRawLiveStreamPrivilege(unsigned int userid) = 0;
DEFINE_FUNC_1(RemoveRawLiveStreamPrivilege, SDKError, unsigned int, userid)
//virtual IList<RawLiveStreamInfo>* GetRawLiveStreamingInfoList() = 0;
DEFINE_FUNC_0(GetRawLiveStreamingInfoList, IList<RawLiveStreamInfo>*)
//virtual IList<unsigned int>* GetRawLiveStreamPrivilegeUserList() = 0;
DEFINE_FUNC_0(GetRawLiveStreamPrivilegeUserList, IList<unsigned int>*)
//virtual bool IsLiveStreamReminderEnabled() = 0;
DEFINE_FUNC_0(IsLiveStreamReminderEnabled, bool)
//virtual bool CanEnableLiveStreamReminder() = 0;
DEFINE_FUNC_0(CanEnableLiveStreamReminder, bool)
//virtual SDKError EnableLiveStreamReminder(bool enable) = 0;
DEFINE_FUNC_1(EnableLiveStreamReminder, SDKError, bool, enable)

// virtual void onLiveStreamStatusChange(LiveStreamStatus status) = 0;
CallBack_FUNC_1(onLiveStreamStatusChange, LiveStreamStatus, status)
//virtual void onRawLiveStreamPrivilegeChanged(bool bHasPrivilege) = 0;
CallBack_FUNC_1(onRawLiveStreamPrivilegeChanged, bool, bHasPrivilege)
//virtual void onRawLiveStreamPrivilegeRequestTimeout() = 0;
CallBack_FUNC_0(onRawLiveStreamPrivilegeRequestTimeout)
//virtual void onUserRawLiveStreamPrivilegeChanged(unsigned int userid, bool bHasPrivilege) = 0;
CallBack_FUNC_2(onUserRawLiveStreamPrivilegeChanged, unsigned int, userid, bool, bHasPrivilege)
//virtual void onRawLiveStreamPrivilegeRequested(IRequestRawLiveStreamPrivilegeHandler* handler) = 0;
CallBack_FUNC_1(onRawLiveStreamPrivilegeRequested, IRequestRawLiveStreamPrivilegeHandler*, handler)
//virtual void onUserRawLiveStreamingStatusChanged(IList<RawLiveStreamInfo>* liveStreamList) = 0;
CallBack_FUNC_1(onUserRawLiveStreamingStatusChanged, IList<RawLiveStreamInfo>*, liveStreamList)
//virtual void onLiveStreamReminderStatusChanged(bool enable) = 0;
CallBack_FUNC_1(onLiveStreamReminderStatusChanged, bool, enable)
//virtual void onLiveStreamReminderStatusChangeFailed() = 0;
CallBack_FUNC_0(onLiveStreamReminderStatusChangeFailed)
//virtual void onUserThresholdReachedForLiveStream(int percent) = 0;
CallBack_FUNC_1(onUserThresholdReachedForLiveStream, int, percent)

END_CLASS_DEFINE(IMeetingLiveStreamController)
END_ZOOM_SDK_NAMESPACE
