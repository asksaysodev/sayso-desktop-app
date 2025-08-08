
#include "meeting_live_stream_wrap_core.h"
#import "Header_include.h"
#include "sdk_native_error.h"
#include "meeting_service_wrap_core.h"
#import "liveStreamHelperDelegate.h"

void releaseLiveStreamItem(ZNList<int64_t> item_list)
{
    if(item_list.size() > 0)
    {
        for (int i = 0; i < item_list.size(); i++) {
            ZoomSDKLiveStreamItem* item = (ZoomSDKLiveStreamItem *)item_list[i];
            if (item) {
                [item release];
            }
        }
        item_list.clear();
    }
}

ZMeetingLiveStreamWrap& ZMeetingServiceWrap::GetMeetingLiveStreamCtrl()
{
    return m_meeting_live_stream_ctrl;
}

ZRequestRawLiveStreamPrivilegeHandlerWrap& ZMeetingServiceWrap::GetRequestRawLiveStreamPrivilegeHandler()
{
    return m_meeting_request_raw_live_stream_privilege_handler;
}

ZMeetingLiveStreamWrap::ZMeetingLiveStreamWrap()
{
    m_pSink = 0;
}

ZMeetingLiveStreamWrap::~ZMeetingLiveStreamWrap()
{
    m_pSink = 0;
    releaseLiveStreamItem(m_live_stream_items_list);
}

void ZMeetingLiveStreamWrap::Init()
{
    
}

void ZMeetingLiveStreamWrap::Uninit()
{

}

void ZMeetingLiveStreamWrap::SetSink(ZNativeSDKMeetingLiveStreamWrapSink* pSink)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (service)
    {
        ZoomSDKLiveStreamHelper *controller = [service getLiveStreamHelper];
        if(controller)
        {
            [controller setDelegate:[liveStreamHelperDelegate shared]];
        }
    }
       m_pSink = pSink;
}

ZNSDKError ZMeetingLiveStreamWrap::CanStartLiveStream()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
    
    ZoomSDKLiveStreamHelper *controller = [service getLiveStreamHelper];
    if (!controller)
        return ZNSDKERR_SERVICE_FAILED;
    ZoomSDKError ret = [controller canStartLiveStream];
    nativeErrorTypeHelp  Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingLiveStreamWrap::StartLiveStream(int64_t liveStreamHandle)
{
    if (!isValidItem(liveStreamHandle))
        return ZNSDKERR_INVALID_PARAMETER;
        
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
    
    ZoomSDKLiveStreamHelper *controller = [service getLiveStreamHelper];
    if (!controller)
        return ZNSDKERR_SERVICE_FAILED;
    
    ZoomSDKError ret = [controller startLiveStream:(ZoomSDKLiveStreamItem*)liveStreamHandle];
    
    nativeErrorTypeHelp  Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingLiveStreamWrap::StartLiveStreamWithSteamingURL(ZoomSTRING streamingURL, ZoomSTRING streamingKey, ZoomSTRING broadcastURL)
{
    if (streamingURL.empty() || streamingKey.empty() || broadcastURL.empty())
        return ZNSDKERR_INVALID_PARAMETER;
    
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
    
    ZoomSDKLiveStreamHelper *controller = [service getLiveStreamHelper];
    if (!controller)
        return ZNSDKERR_SERVICE_FAILED;
    
    NSString *streamURL = [NSString stringWithUTF8String:streamingURL.c_str()];
    NSString *streamKey = [NSString stringWithUTF8String:streamingKey.c_str()];
    NSString *url = [NSString stringWithUTF8String:broadcastURL.c_str()];
    
    ZoomSDKError ret = [controller startLiveStreamByURL:streamURL StreamKey:streamKey BroadcastURL:url];
    
    nativeErrorTypeHelp  Help_type;
    return Help_type.ZoomSDKErrorType(ret);
    
}

ZNSDKError ZMeetingLiveStreamWrap::StopLiveStream()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
    
    ZoomSDKLiveStreamHelper *controller = [service getLiveStreamHelper];
    if (!controller)
        return ZNSDKERR_SERVICE_FAILED;
    
    ZoomSDKError ret = [controller stopLiveStream];
    
    nativeErrorTypeHelp  Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNList<int64_t> ZMeetingLiveStreamWrap::GetSupportLiveStreamItems()
{
    if(m_live_stream_items_list.size() > 0)
    {
        releaseLiveStreamItem(m_live_stream_items_list);
    }
    
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return m_live_stream_items_list;
    
    ZoomSDKLiveStreamHelper *controller = [service getLiveStreamHelper];
    if (!controller)
        return m_live_stream_items_list;
    
    NSArray *arr = [controller getSupportLiveStreamItems];
    if (!arr || arr.count == 0)
        return m_live_stream_items_list;

    for (ZoomSDKLiveStreamItem* item in arr) {
        if (!item)
            break;
        [item retain];
        
        m_live_stream_items_list.push_back((int64_t)item);
        
    }
    return m_live_stream_items_list;
}

ZNLiveStreamItemInfo ZMeetingLiveStreamWrap::GetCurrentLiveStreamItem()
{
    ZNLiveStreamItemInfo zn_info;
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return zn_info;
    
    ZoomSDKLiveStreamHelper *controller = [service getLiveStreamHelper];
    if (!controller)
        return zn_info;
    
    ZoomSDKLiveStreamItem* item = [controller getCurrentLiveStreamItem];
    if (!item)
        return zn_info;
    
    NSString* liveStreamUrl = [item getLiveStreamURL];
    if (liveStreamUrl && liveStreamUrl.length > 0) {
        zn_info.liveStreamURL = [liveStreamUrl UTF8String];
    } else {
        zn_info.liveStreamURL = "";
    }
    
    NSString* liveStreamDes = [item getLiveStreamURLDescription];
    if (liveStreamDes && liveStreamDes.length > 0) {
        zn_info.liveStreamURLDescription = [liveStreamDes UTF8String];
    } else {
        zn_info.liveStreamURLDescription = "";
    }

    NSString* liveStreamViewerURL = [item getLiveStreamViewerURL];
    if (liveStreamViewerURL && liveStreamViewerURL.length > 0) {
        zn_info.liveStreamViewerURL = [liveStreamViewerURL UTF8String];
    } else {
        zn_info.liveStreamViewerURL = "";
    }
    return zn_info;
}

ZNLiveStreamStatus ZMeetingLiveStreamWrap::GetCurrentLiveStreamStatus()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZN_LiveStreamStatus_None;
    
    ZoomSDKLiveStreamHelper *controller = [service getLiveStreamHelper];
    if (!controller)
        return ZN_LiveStreamStatus_None;
    
    LiveStreamStatus status = [controller getLiveStreamStatus];
    
    nativeErrorTypeHelp  Help_type;
    return Help_type.ZNLiveStreamStatusMap(status);
}

ZNSDKError ZMeetingLiveStreamWrap::StopRawLiveStream()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
    
    ZoomSDKLiveStreamHelper *controller = [service getLiveStreamHelper];
    if (!controller)
        return ZNSDKERR_SERVICE_FAILED;
    
    ZoomSDKError ret = [controller stopRawLiveStream];
    
    nativeErrorTypeHelp  Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

bool ZMeetingLiveStreamWrap::isValidItem(int64_t liveStreamHandle)
{
    if (m_live_stream_items_list.size() == 0)
        return false;
    
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
    if (!isValidItem(liveStreamHandle))
        return zn_info;
    
    ZoomSDKLiveStreamItem* item = (ZoomSDKLiveStreamItem*)liveStreamHandle;
    
    NSString* liveStreamUrl = [item getLiveStreamURL];
    if (liveStreamUrl && liveStreamUrl.length > 0) {
        zn_info.liveStreamURL = [liveStreamUrl UTF8String];
    } else {
        zn_info.liveStreamURL = "";
    }
    
    NSString* liveStreamDes = [item getLiveStreamURLDescription];
    if (liveStreamDes && liveStreamDes.length > 0) {
        zn_info.liveStreamURLDescription = [liveStreamDes UTF8String];
    } else {
        zn_info.liveStreamURLDescription = "";
    }

    NSString* liveStreamViewerURL = [item getLiveStreamViewerURL];
    if (liveStreamViewerURL && liveStreamViewerURL.length > 0) {
        zn_info.liveStreamViewerURL = [liveStreamViewerURL UTF8String];
    } else {
        zn_info.liveStreamViewerURL = "";
    }
    return zn_info;
}

bool ZMeetingLiveStreamWrap::IsRawLiveStreamSupported()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return false;
    
    ZoomSDKLiveStreamHelper *controller = [service getLiveStreamHelper];
    if (!controller)
        return false;
    
    return [controller isRawLiveStreamSupported];
}

ZNSDKError ZMeetingLiveStreamWrap::CanStartRawLiveStream()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
    
    ZoomSDKLiveStreamHelper *controller = [service getLiveStreamHelper];
    if (!controller)
        return ZNSDKERR_SERVICE_FAILED;
    
    ZoomSDKError ret = [controller canStartRawLiveStream];
    
    nativeErrorTypeHelp  Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingLiveStreamWrap::RemoveRawLiveStreamPrivilege(unsigned int userid)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
    
    ZoomSDKLiveStreamHelper *controller = [service getLiveStreamHelper];
    if (!controller)
        return ZNSDKERR_SERVICE_FAILED;
    
    ZoomSDKError ret = [controller removeRawLiveStreamPrivilege:userid];
    
    nativeErrorTypeHelp  Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNList<ZNRawLiveStreamInfo> ZMeetingLiveStreamWrap::GetRawLiveStreamingInfoList()
{
    ZNList<ZNRawLiveStreamInfo> zn_list;
    
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return zn_list;
    
    ZoomSDKLiveStreamHelper *controller = [service getLiveStreamHelper];
    if (!controller)
        return zn_list;
    
    NSArray *arr = [controller getRawLiveStreamingInfoList];
    if(!arr || arr.count == 0)
        return zn_list;
    
    for (ZoomSDKRawLiveStreamInfo *info in arr) {
        if(!info)
            break;
        
        ZNRawLiveStreamInfo znInfo;
        znInfo.userId = info.userID;
        znInfo.broadcastUrl = (info.broadcastUrl ? [info.broadcastUrl UTF8String] : "");
        zn_list.push_back(znInfo);
    }
    return zn_list;
}

ZNList<unsigned int> ZMeetingLiveStreamWrap::GetRawLiveStreamPrivilegeUserList()
{
    ZNList<unsigned int> zn_list;
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return zn_list;
    
    ZoomSDKLiveStreamHelper *controller = [service getLiveStreamHelper];
    if (!controller)
        return zn_list;
    
    NSArray *arr = [controller getRawLiveStreamPrivilegeUserList];
    if(!arr || arr.count == 0)
        return zn_list;
    
    for (NSNumber*  number in arr) {
        if(!number)
            break;
        unsigned int userid = [number unsignedIntValue];
        zn_list.push_back(userid);
    }
    return zn_list;
}

bool ZMeetingLiveStreamWrap::IsLiveStreamReminderEnabled()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return false;
    
    ZoomSDKLiveStreamHelper *controller = [service getLiveStreamHelper];
    if (!controller)
        return false;
    
    return [controller isLiveStreamReminderEnabled];
}

bool ZMeetingLiveStreamWrap::CanEnableLiveStreamReminder()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return false;
    
    ZoomSDKLiveStreamHelper *controller = [service getLiveStreamHelper];
    if (!controller)
        return false;
    
    return [controller canEnableLiveStreamReminder];
}

ZNSDKError ZMeetingLiveStreamWrap::EnableLiveStreamReminder(bool enable)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
    
    ZoomSDKLiveStreamHelper *controller = [service getLiveStreamHelper];
    if (!controller)
        return ZNSDKERR_SERVICE_FAILED;
    
    ZoomSDKError ret = [controller enableLiveStreamReminder:enable];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
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
    ZoomSDKRequestRawLiveStreamPrivilegeHandler* handler = [liveStreamHelperDelegate shared].rawLiveStreamPrivilegeHandler;
    if(!handler)
        return "";
    return handler.requestId ? [handler.requestId UTF8String] : "";
}

unsigned int ZRequestRawLiveStreamPrivilegeHandlerWrap::GetRequesterId()
{
    ZoomSDKRequestRawLiveStreamPrivilegeHandler* handler = [liveStreamHelperDelegate shared].rawLiveStreamPrivilegeHandler;
    if(!handler)
        return 0;
    return handler.requesterId;
}

ZoomSTRING ZRequestRawLiveStreamPrivilegeHandlerWrap::GetRequesterName()
{
    ZoomSDKRequestRawLiveStreamPrivilegeHandler* handler = [liveStreamHelperDelegate shared].rawLiveStreamPrivilegeHandler;
    if(!handler)
        return "";
    return handler.requesterName ? [handler.requesterName UTF8String] : "";
}

ZoomSTRING ZRequestRawLiveStreamPrivilegeHandlerWrap::GetBroadcastUrl()
{
    ZoomSDKRequestRawLiveStreamPrivilegeHandler* handler = [liveStreamHelperDelegate shared].rawLiveStreamPrivilegeHandler;
    if(!handler)
        return "";
    return handler.broadcastUrl ? [handler.broadcastUrl UTF8String] : "";
}

ZNSDKError ZRequestRawLiveStreamPrivilegeHandlerWrap::GrantRawLiveStreamPrivilege()
{
    ZoomSDKRequestRawLiveStreamPrivilegeHandler* handler = [liveStreamHelperDelegate shared].rawLiveStreamPrivilegeHandler;
    if(!handler)
        return ZNSDKERR_SERVICE_FAILED;
    
    ZoomSDKError ret = [handler grantRawLiveStreamPrivilege];
    
    nativeErrorTypeHelp  Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZRequestRawLiveStreamPrivilegeHandlerWrap::DenyRawLiveStreamPrivilege()
{
    ZoomSDKRequestRawLiveStreamPrivilegeHandler* handler = [liveStreamHelperDelegate shared].rawLiveStreamPrivilegeHandler;
    if(!handler)
        return ZNSDKERR_SERVICE_FAILED;
    
    ZoomSDKError ret = [handler denyRawLiveStreamPrivilege];
    
    nativeErrorTypeHelp  Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}
