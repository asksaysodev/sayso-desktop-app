
#include "meeting_live_stream_wrap_core.h"
#include "sdk_wrap.h"
#include "meeting_service_components_wrap/meeting_live_stream_wrap.h"
#include "zoom_native_to_wrap.h"
#include "sdk_events_wrap_class.h"
extern ZOOM_SDK_NAMESPACE::IMeetingServiceWrap& g_meeting_service_wrap;

ZMeetingLiveStreamWrap::ZMeetingLiveStreamWrap()
{
	SDKEventWrapMgr::GetInst().m_meetingLiveStreamWrapEvent.SetOwner(this);
	m_pSink = NULL;
}
ZMeetingLiveStreamWrap::~ZMeetingLiveStreamWrap()
{
	Uninit();
	m_pSink = NULL;
	m_live_stream_items_list.clear();
	SDKEventWrapMgr::GetInst().m_meetingLiveStreamWrapEvent.SetOwner(NULL);
}
void ZMeetingLiveStreamWrap::Init()
{
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingLiveStreamController().Init_Wrap(&g_meeting_service_wrap);
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingLiveStreamController().SetEvent(&SDKEventWrapMgr::GetInst().m_meetingLiveStreamWrapEvent);
}
void ZMeetingLiveStreamWrap::Uninit()
{
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingLiveStreamController().SetEvent(NULL);
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingLiveStreamController().Uninit_Wrap();
}
void ZMeetingLiveStreamWrap::SetSink(ZNativeSDKMeetingLiveStreamWrapSink* pSink)
{
	m_pSink = pSink;
}
//
ZNSDKError ZMeetingLiveStreamWrap::CanStartLiveStream()
{
	return Map2WrapDefine(ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingLiveStreamController().CanStartLiveStream());
}
ZNSDKError ZMeetingLiveStreamWrap::StartLiveStream(int64_t liveStreamHandle)
{
	if (!isValidItem(liveStreamHandle))
	{
		return ZNSDKERR_INVALID_PARAMETER;
	}
	ZOOM_SDK_NAMESPACE::IMeetingLiveStreamItem* pLiveStreamItem = (ZOOM_SDK_NAMESPACE::IMeetingLiveStreamItem*)liveStreamHandle;
	return Map2WrapDefine(ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingLiveStreamController().StartLiveStream(pLiveStreamItem));
}
ZNSDKError ZMeetingLiveStreamWrap::StartLiveStreamWithSteamingURL(ZoomSTRING streamingURL, ZoomSTRING streamingKey, ZoomSTRING broadcastURL)
{
	return Map2WrapDefine(ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingLiveStreamController().StartLiveStreamWithSteamingURL(streamingURL.c_str(), streamingKey.c_str(), broadcastURL.c_str()));
}
ZNSDKError ZMeetingLiveStreamWrap::StopLiveStream()
{
	return Map2WrapDefine(ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingLiveStreamController().StopLiveStream());
}
ZNList<int64_t> ZMeetingLiveStreamWrap::GetSupportLiveStreamItems()
{
	ZOOM_SDK_NAMESPACE::IList<ZOOM_SDK_NAMESPACE::IMeetingLiveStreamItem* >* pLiveStreamItemsLst(NULL);
	pLiveStreamItemsLst = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingLiveStreamController().GetSupportLiveStreamItems();
	m_live_stream_items_list.clear();
	if (pLiveStreamItemsLst && pLiveStreamItemsLst->GetCount() > 0)
	{
		int count = pLiveStreamItemsLst->GetCount();
		for (int i = 0; i < count; i++)
		{
			ZOOM_SDK_NAMESPACE::IMeetingLiveStreamItem* pItem = pLiveStreamItemsLst->GetItem(i);
			if (NULL == pItem)
				continue;
			int64_t itemHandle = (int64_t)pItem;
			m_live_stream_items_list.push_back(itemHandle);
		}
	}
	return m_live_stream_items_list;
}
ZNLiveStreamItemInfo ZMeetingLiveStreamWrap::GetCurrentLiveStreamItem()
{
	ZNLiveStreamItemInfo zn_info;
	ZOOM_SDK_NAMESPACE::IMeetingLiveStreamItem* pLiveStreamItem = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingLiveStreamController().GetCurrentLiveStreamItem();
	if (pLiveStreamItem) {
		if (pLiveStreamItem->GetLiveStreamURL())
		{
			zn_info.liveStreamURL = pLiveStreamItem->GetLiveStreamURL();
		}
		if (pLiveStreamItem->GetLiveStreamURLDescription())
		{
			zn_info.liveStreamURLDescription = pLiveStreamItem->GetLiveStreamURLDescription();
		}
		if (pLiveStreamItem->GetLiveStreamViewerURL())
		{
			zn_info.liveStreamViewerURL = pLiveStreamItem->GetLiveStreamViewerURL();
		}
	}
	return zn_info;
}
ZNLiveStreamStatus ZMeetingLiveStreamWrap::GetCurrentLiveStreamStatus()
{
	return Map2WrapDefine(ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingLiveStreamController().GetCurrentLiveStreamStatus());
}
ZNSDKError ZMeetingLiveStreamWrap::StopRawLiveStream()
{
	return Map2WrapDefine(ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingLiveStreamController().StopRawLiveStream());
}
bool ZMeetingLiveStreamWrap::isValidItem(int64_t liveStreamHandle)
{
	bool isValid = false;
	for (int i = 0; i < m_live_stream_items_list.size(); i++)
	{
		if (liveStreamHandle == m_live_stream_items_list[i])
		{
			isValid = true;
			break;
		}
	}
	return isValid;
}
ZNLiveStreamItemInfo ZMeetingLiveStreamWrap::GetLiveStreamItemInfo(int64_t liveStreamHandle)
{
	ZNLiveStreamItemInfo zn_info;
	ZOOM_SDK_NAMESPACE::IMeetingLiveStreamItem* pLiveStreamItem = (ZOOM_SDK_NAMESPACE::IMeetingLiveStreamItem*)liveStreamHandle;
	bool isValid = isValidItem(liveStreamHandle);
	if (isValid)
	{
		if (pLiveStreamItem->GetLiveStreamURL())
		{
			zn_info.liveStreamURL = pLiveStreamItem->GetLiveStreamURL();
		}
		if (pLiveStreamItem->GetLiveStreamURLDescription())
		{
			zn_info.liveStreamURLDescription = pLiveStreamItem->GetLiveStreamURLDescription();
		}
		if (pLiveStreamItem->GetLiveStreamViewerURL())
		{
			zn_info.liveStreamViewerURL = pLiveStreamItem->GetLiveStreamViewerURL();
		}
	}
	return zn_info;
}
bool ZMeetingLiveStreamWrap::IsRawLiveStreamSupported()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingLiveStreamController().IsRawLiveStreamSupported();
}
ZNSDKError ZMeetingLiveStreamWrap::CanStartRawLiveStream()
{
	return Map2WrapDefine(ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingLiveStreamController().CanStartRawLiveStream());
}
ZNSDKError ZMeetingLiveStreamWrap::RemoveRawLiveStreamPrivilege(unsigned int userid)
{
	return Map2WrapDefine(ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingLiveStreamController().RemoveRawLiveStreamPrivilege(userid));
}
ZNList<ZNRawLiveStreamInfo> ZMeetingLiveStreamWrap::GetRawLiveStreamingInfoList()
{
	ZOOM_SDK_NAMESPACE::IList<ZOOM_SDK_NAMESPACE::RawLiveStreamInfo >* pNativeLst(NULL);
	pNativeLst = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingLiveStreamController().GetRawLiveStreamingInfoList();
	ZNList<ZNRawLiveStreamInfo> zn_list;
	if (pNativeLst && pNativeLst->GetCount() > 0)
	{
		int count = pNativeLst->GetCount();
		for (int i = 0; i < count; i++)
		{
			ZOOM_SDK_NAMESPACE::RawLiveStreamInfo nativeInfo = pNativeLst->GetItem(i);
			ZNRawLiveStreamInfo znInfo;
			if (nativeInfo.broadcastUrl)
			{
				znInfo.broadcastUrl = nativeInfo.broadcastUrl;
			}
			znInfo.userId = nativeInfo.userId;
			zn_list.push_back(znInfo);
		}
	}
	return zn_list;
}
ZNList<unsigned int> ZMeetingLiveStreamWrap::GetRawLiveStreamPrivilegeUserList()
{
	ZOOM_SDK_NAMESPACE::IList<unsigned int >* pNativeLst(NULL);
	pNativeLst = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingLiveStreamController().GetRawLiveStreamPrivilegeUserList();
	ZNList<unsigned int> zn_list;
	if (pNativeLst && pNativeLst->GetCount() > 0)
	{
		int count = pNativeLst->GetCount();
		for (int i = 0; i < count; i++)
		{
			zn_list.push_back(pNativeLst->GetItem(i));
		}
	}
	return zn_list;
}
bool ZMeetingLiveStreamWrap::IsLiveStreamReminderEnabled()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingLiveStreamController().IsLiveStreamReminderEnabled();
}
bool ZMeetingLiveStreamWrap::CanEnableLiveStreamReminder()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingLiveStreamController().CanEnableLiveStreamReminder();
}
ZNSDKError ZMeetingLiveStreamWrap::EnableLiveStreamReminder(bool enable)
{
	return Map2WrapDefine(ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingLiveStreamController().EnableLiveStreamReminder(enable));
}

void ZMeetingLiveStreamWrap::onLiveStreamStatusChange(ZNLiveStreamStatus status)
{
	if (m_pSink)
		m_pSink->onLiveStreamStatusChange(status);
}
void ZMeetingLiveStreamWrap::onRawLiveStreamPrivilegeChanged(bool bHasPrivilege)
{
	if (m_pSink)
		m_pSink->onRawLiveStreamPrivilegeChanged(bHasPrivilege);
}
void ZMeetingLiveStreamWrap::onUserRawLiveStreamPrivilegeChanged(unsigned int userid, bool bHasPrivilege)
{
	if (m_pSink)
		m_pSink->onUserRawLiveStreamPrivilegeChanged(userid, bHasPrivilege);
}
void ZMeetingLiveStreamWrap::onRawLiveStreamPrivilegeRequested()
{
	if (m_pSink)
		m_pSink->onRawLiveStreamPrivilegeRequested();
}
void ZMeetingLiveStreamWrap::onUserRawLiveStreamingStatusChanged(ZNList<ZNRawLiveStreamInfo> pLivingList)
{
	if (m_pSink)
		m_pSink->onUserRawLiveStreamingStatusChanged(pLivingList);
}
void ZMeetingLiveStreamWrap::onRawLiveStreamPrivilegeRequestTimeout()
{
	if (m_pSink)
		m_pSink->onRawLiveStreamPrivilegeRequestTimeout();
}
void ZMeetingLiveStreamWrap::onLiveStreamReminderStatusChanged(bool enable)
{
	if (m_pSink)
		m_pSink->onLiveStreamReminderStatusChanged(enable);
}
void ZMeetingLiveStreamWrap::onLiveStreamReminderStatusChangeFailed()
{
	if (m_pSink)
		m_pSink->onLiveStreamReminderStatusChangeFailed();
}
void ZMeetingLiveStreamWrap::onUserThresholdReachedForLiveStream(int percent)
{
	if (m_pSink)
		m_pSink->onUserThresholdReachedForLiveStream(percent);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZRequestRawLiveStreamPrivilegeHandlerWrap::ZRequestRawLiveStreamPrivilegeHandlerWrap()
{
	
}
ZRequestRawLiveStreamPrivilegeHandlerWrap::~ZRequestRawLiveStreamPrivilegeHandlerWrap()
{
	
}
ZoomSTRING ZRequestRawLiveStreamPrivilegeHandlerWrap::GetRequestId()
{
	return ZNRequestRawLiveStreamPrivilegeHandler::GetInst().GetRequestId();
}
unsigned int ZRequestRawLiveStreamPrivilegeHandlerWrap::GetRequesterId()
{
	return ZNRequestRawLiveStreamPrivilegeHandler::GetInst().GetRequesterId();
}
ZoomSTRING ZRequestRawLiveStreamPrivilegeHandlerWrap::GetRequesterName()
{
	return ZNRequestRawLiveStreamPrivilegeHandler::GetInst().GetRequesterName();
}
ZoomSTRING ZRequestRawLiveStreamPrivilegeHandlerWrap::GetBroadcastUrl()
{
	return ZNRequestRawLiveStreamPrivilegeHandler::GetInst().GetBroadcastUrl();
}
ZNSDKError ZRequestRawLiveStreamPrivilegeHandlerWrap::GrantRawLiveStreamPrivilege()
{
	return ZNRequestRawLiveStreamPrivilegeHandler::GetInst().GrantRawLiveStreamPrivilege();
}
ZNSDKError ZRequestRawLiveStreamPrivilegeHandlerWrap::DenyRawLiveStreamPrivilege()
{
	return ZNRequestRawLiveStreamPrivilegeHandler::GetInst().DenyRawLiveStreamPrivilege();
}