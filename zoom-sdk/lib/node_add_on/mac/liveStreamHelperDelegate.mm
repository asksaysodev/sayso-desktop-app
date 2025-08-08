//

#import "liveStreamHelperDelegate.h"
#include "zoom_node_addon.h"
#include "sdk_native_error.h"

extern ZNativeSDKWrap _g_native_wrap;
static liveStreamHelperDelegate *delegate = nil;

@implementation liveStreamHelperDelegate

+(liveStreamHelperDelegate *)shared
{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        
        delegate = [[liveStreamHelperDelegate alloc] init];
    });
    return delegate;
}

-(instancetype)init
{
    self = [super init];
    if (self) {
        self.rawLiveStreamPrivilegeHandler = nil;
        return  self;
    }
    return nil;
}

-(void)dealloc
{
    self.rawLiveStreamPrivilegeHandler = nil;
    [super dealloc];
}
#pragma mark live stream delegate
- (void)onLiveStreamStatusChange:(LiveStreamStatus)status
{
    nativeErrorTypeHelp  Help_type;
    ZNLiveStreamStatus liveStreamStatus = Help_type.ZNLiveStreamStatusMap(status);
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingLiveStreamCtrl().onLiveStreamStatusChange(liveStreamStatus);
}

- (void)onRawLiveStreamPrivilegeChanged:(BOOL)bHasPrivilege
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingLiveStreamCtrl().onRawLiveStreamPrivilegeChanged(bHasPrivilege);
}

- (void)onRawLiveStreamPrivilegeRequestTimeout
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingLiveStreamCtrl().onRawLiveStreamPrivilegeRequestTimeout();
}

- (void)onUserRawLiveStreamPrivilegeChanged:(unsigned int)userID hasPrivilege:(BOOL)bHasPrivilege
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingLiveStreamCtrl().onUserRawLiveStreamPrivilegeChanged(userID, bHasPrivilege);
}

- (void)onRawLiveStreamPrivilegeRequested:(ZoomSDKRequestRawLiveStreamPrivilegeHandler*)handler
{
    self.rawLiveStreamPrivilegeHandler = handler;
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingLiveStreamCtrl().onRawLiveStreamPrivilegeRequested();
}

- (void)onUserRawLiveStreamingStatusChanged:(NSArray<ZoomSDKRawLiveStreamInfo*>*)liveStreamList
{
    if (!liveStreamList || liveStreamList.count == 0)
        return;
    ZNList<ZNRawLiveStreamInfo> zn_rawLiveStreamInfolist;
    for (ZoomSDKRawLiveStreamInfo *info in liveStreamList) {
        if(!info)
            break;
        
        ZNRawLiveStreamInfo znInfo;
        znInfo.userId = info.userID;
        znInfo.broadcastUrl = (info.broadcastUrl ? [info.broadcastUrl UTF8String] : "");
        zn_rawLiveStreamInfolist.push_back(znInfo);
    }
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingLiveStreamCtrl().onUserRawLiveStreamingStatusChanged(zn_rawLiveStreamInfolist);
}

- (void)onLiveStreamReminderStatusChanged:(BOOL)enable
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingLiveStreamCtrl().onLiveStreamReminderStatusChanged(enable);
}

- (void)onLiveStreamReminderStatusChangedFailed
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingLiveStreamCtrl().onLiveStreamReminderStatusChangeFailed();
}

- (void)onUserThresholdReachedForLiveStream:(int)percent
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingLiveStreamCtrl().onUserThresholdReachedForLiveStream(percent);
}

@end
