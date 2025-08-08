

#import "meetingServiceDelegate.h"
#include "zoom_node_addon.h"
#include "sdk_native_error.h"
extern ZNativeSDKWrap _g_native_wrap;

@implementation meetingServiceDelegate

+(meetingServiceDelegate *)share
{
    static meetingServiceDelegate *delegate = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        
        delegate = [[meetingServiceDelegate alloc]init];
    });
    return delegate;
}

#pragma mark meeting delegate
- (void)onMeetingStatusChange:(ZoomSDKMeetingStatus)state meetingError:(ZoomSDKMeetingError)error EndReason:(EndMeetingReason)reason
{
    nativeErrorTypeHelp help;
    ZNMeetingStatus status = help.ZNSDKMeetingStatus(state);
    int  result = 0;
    if (error == ZoomSDKMeetingError_None) {
        result = help.ZNSDKMeetingEndReason(reason);
    }else{
        result = help.ZNSDKMeetingFailCode(error);
    }
    _g_native_wrap.GetMeetingServiceWrap().onMeetingStatusChanged(status, result);
}

- (void)onFreeMeetingNeedToUpgrade:(FreeMeetingNeedUpgradeType)type giftUpgradeURL:(NSString *)giftURL
{
    nativeErrorTypeHelp help;
    ZNFreeMeetingNeedUpgradeType  ZN_type = help.ZoomSDKFreeMeetingNeedUpgradeType(type);
    NSString *url = nil;
    if (giftURL == nil) {
        url = @"";
    }else{
        url = giftURL;
    }
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingConfigCtrl().onFreeMeetingNeedToUpgrade(ZN_type, url.UTF8String);
}

- (void)onFreeMeetingUpgradeToGiftFreeTrialStop
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingConfigCtrl().onFreeMeetingUpgradeToGiftFreeTrialStop();
}

- (void)onFreeMeetingUpgradeToGiftFreeTrialStart
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingConfigCtrl().onFreeMeetingUpgradeToGiftFreeTrialStart();
}

- (void)onFreeMeetingUpgradedToProMeeting
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingConfigCtrl().onFreeMeetingUpgradeToProMeeting();
}

- (void)onMeetingStatisticWarning:(StatisticWarningType)type
{
    nativeErrorTypeHelp help;
    ZNStatisticsWarningType warningType = help.ZNStatisticsWarningTypeMap(type);
    _g_native_wrap.GetMeetingServiceWrap().onMeetingStatisticsWarningNotification(warningType);
}

- (void)onMeetingParameterNotification:(ZoomSDKMeetingParameter *)meetingParameter
{
    ZNMeetingParameter param;
    nativeErrorTypeHelp help;
    MeetingType type = meetingParameter.meetingType;
    param.meeting_type = help.ZNSDKMeetingType(type);
    param.is_view_only = meetingParameter.isViewOnly;
    param.is_auto_recording_local = meetingParameter.isAutoRecordingLocal;
    param.is_auto_recording_cloud = meetingParameter.isAutoRecordingCloud;
    param.meeting_number = meetingParameter.meetingNumber;
    param.meeting_topic = (meetingParameter.meetingTopic?meetingParameter.meetingTopic.UTF8String:"");
    param.meeting_host = (meetingParameter.meetingHost?meetingParameter.meetingHost.UTF8String:"");
    _g_native_wrap.GetMeetingServiceWrap().onMeetingParameterNotification(param);
}

- (void)onMeetingTopicChanged:(NSString *)topic
{
    ZoomSTRING zn_topic = "";
    if (topic) 
        zn_topic = [topic UTF8String];
    _g_native_wrap.GetMeetingServiceWrap().onMeetingTopicChanged(zn_topic);
}

- (void)onMeetingFullToWatchLiveStream:(NSString*)liveStreamUrl
{
    ZoomSTRING zn_liveStreamUrl = "";
    if (liveStreamUrl) 
        zn_liveStreamUrl = [liveStreamUrl UTF8String];
    _g_native_wrap.GetMeetingServiceWrap().onMeetingFullToWatchLiveStream(zn_liveStreamUrl);
}

#pragma mark meeting action controller
- (void)onVideoStatusChange:(BOOL)videoOn UserID:(unsigned int)userID
{
    ZNVideoStatus videoStatus = ZN_Video_OFF;
    if (videoOn == YES) {
        videoStatus = ZN_Video_ON;
    }else{
        videoStatus = ZN_Video_OFF;
    }
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingVideoCtrl().onUserVideoStatusChange(userID, videoStatus);
}

-(void)onActiveVideoUserChanged:(unsigned int)userID
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingVideoCtrl().onActiveVideoUserChanged(userID);
}

-(void)onActiveSpeakerVideoUserChanged:(unsigned int)userID
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingVideoCtrl().onActiveSpeakerVideoUserChanged(userID);
}

-(void)onUserAudioStatusChange:(NSArray *)userAudioStatusArray
{
    if (!userAudioStatusArray || userAudioStatusArray.count == 0) {
        return;
    }
    ZNList<ZNUserAudioStatus> userAudioStatusList;
    nativeErrorTypeHelp help_type;
    ZNUserAudioStatus userAudioInfo;
    for (ZoomSDKUserAudioStatus *key in userAudioStatusArray) {
        unsigned int USERID = [key getUserID];
        ZoomSDKAudioStatus status = [key getStatus];
        ZNAudioStatus znStatus = help_type.ZNSDKUserAudioStatus(status);
        userAudioInfo.audioStauts = znStatus;
        userAudioInfo.userId = USERID;
        userAudioStatusList.push_back(userAudioInfo);
    }
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingAudioCtrl().onUserAudioStatusChange(userAudioStatusList, "");
    
}

-(void)onUserActiveAudioChange:(NSArray *)useridArray
{
    ZNList<unsigned int> list;
    for (NSNumber *num in useridArray) {
        unsigned int userid = [num unsignedIntValue];
        list.push_back(userid);
    }
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingAudioCtrl().onUserActiveAudioChange(list);
}

-(void)onMuteOnEntryStatusChange:(BOOL)enable
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingAudioCtrl().onMuteOnEntryStatusChange(enable);
}

-(void)onUserLeft:(NSArray *)array
{
    if (!array || array.count == 0) {
        return;
    }
    ZNList<unsigned int> list;
    for (NSNumber *num in array) {
        unsigned int userid = [num unsignedIntValue];
        list.push_back(userid);
    }
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingParticipantsCtrl().onUserLeft(list, "");
}

-(void)onUserJoin:(NSArray *)array
{
    if (!array || array.count == 0) {
        return;
    }
    ZNList<unsigned int> list;
    for (NSNumber *num in array) {
        unsigned int userid = [num unsignedIntValue];
        list.push_back(userid);
    }
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingParticipantsCtrl().onUserJoin(list, "");
}

-(void)onHostChange:(unsigned int)userID
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingParticipantsCtrl().onHostChangeNotification(userID);
}

-(void)onAllowParticipantsRequestCloudRecording:(bool)allow
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingParticipantsCtrl().onAllowParticipantsRequestCloudRecording(allow);
}

-(void)onBotAuthorizerRelationChanged:(unsigned int)authorizeUserID
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingParticipantsCtrl().onBotAuthorizerRelationChanged(authorizeUserID);
}

-(void)onJoinMeetingNeedUserInfo:(ZoomSDKMeetingInputUserInfoHandler *)handler
{
    self.meetingInputUserInfoHandler = handler;
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingConfigCtrl().onJoinMeetingNeedUserInfo();
}

-(void)onUserConfirmToStartArchive:(ZoomSDKMeetingArchiveConfirmHandler *)handler
{
    self.meetingArchiveConfirmHandler = handler;
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingConfigCtrl().onUserConfirmToStartArchive();
}

-(void)onJoinMeetingResponse:(ZoomSDKJoinMeetingHelper *)joinMeetingHelper
{
    self.joinMeetingHelper = joinMeetingHelper;
}

-(void)cleanUp
{
    if (_joinMeetingHelper) {
        [_joinMeetingHelper release];
        _joinMeetingHelper = nil;
    }
    if (_meetingInputUserInfoHandler) {
        [_meetingInputUserInfoHandler release];
        _meetingInputUserInfoHandler = nil;
    }
    if (_meetingArchiveConfirmHandler) {
        [_meetingArchiveConfirmHandler release];
        _meetingArchiveConfirmHandler = nil;
    }
}

-(void)dealloc
{
    [self cleanUp];
    [super dealloc];
}

-(void)onChatStatusChangedNotification:(ZoomSDKChatStatus *)chatStatus
{
    ZNChatStatus zn_chat_status;
    zn_chat_status.is_chat_off = !chatStatus.isSupportChat;
    zn_chat_status.is_webinar_attendee = chatStatus.isWebinarAttendee;
    zn_chat_status.is_webinar_meeting = chatStatus.isWebinarMeeting;
    if (chatStatus.isWebinarMeeting)
    {
        if (chatStatus.isWebinarAttendee) 
        {
            zn_chat_status.ut.webinar_attendee_status.can_chat = chatStatus.getWebinarAttendeePrivilege.canChat;
            zn_chat_status.ut.webinar_attendee_status.can_chat_to_all_panellist_and_attendee = chatStatus.getWebinarAttendeePrivilege.canChatToAllPanellistAndAttendee;
            zn_chat_status.ut.webinar_attendee_status.can_chat_to_all_panellist = chatStatus.getWebinarAttendeePrivilege.canChatToAllPanellist;
        } else {
            zn_chat_status.ut.webinar_other_status.can_chat_to_all_panellist = chatStatus.getWebinarPanelistPrivilege.canChatToAllPanellist;
            zn_chat_status.ut.webinar_other_status.can_chat_to_all_panellist_and_attendee = chatStatus.getWebinarPanelistPrivilege.canChatToAllPanellistAndAttendee;
            zn_chat_status.ut.webinar_other_status.can_chat_to_individual = chatStatus.getWebinarPanelistPrivilege.canChatToIndividual;
        }
    } else {
        zn_chat_status.ut.normal_meeting_status.can_chat = chatStatus.getNormalMeetingPrivilege.canChat;
        zn_chat_status.ut.normal_meeting_status.can_chat_to_all = chatStatus.getNormalMeetingPrivilege.canChatToAll;
        zn_chat_status.ut.normal_meeting_status.can_chat_to_individual = chatStatus.getNormalMeetingPrivilege.canChatToIndividual;
        zn_chat_status.ut.normal_meeting_status.is_only_can_chat_to_host = chatStatus.getNormalMeetingPrivilege.isOnlyCanChatToHost;
    }
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingChatCtrl().onChatStatusChangedNotification(zn_chat_status);
}

-(void)onChatMsgDeleteNotification:(NSString*)msgID messageDeleteType:(ZoomSDKChatMessageDeleteType)deleteBy
{
    ZoomSTRING zn_msgID = "";
    if (msgID) 
        zn_msgID = [msgID UTF8String];
    
    nativeErrorTypeHelp help;
    ZNSDKChatMessageDeleteType zn_deleteBy = help.ZNSDKChatMessageDeleteTypeMap(deleteBy);
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingChatCtrl().onChatMsgDeleteNotification(zn_msgID, zn_deleteBy);
}

-(void)onShareMeetingChatStatusChanged:(BOOL)isStart
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingChatCtrl().onShareMeetingChatStatusChanged(isStart);
}

@end

@implementation meetingServiceChatDelegate
+(meetingServiceChatDelegate *)share
{
    static meetingServiceChatDelegate *delegate = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        delegate = [[meetingServiceChatDelegate alloc] init];
    });
    return delegate;
}

-(instancetype)init
{
    self = [super init];
    if (self) {
        _fileSender = nil;
        _fileReceiver = nil;
        return self;
    }
    return nil;
}

-(void)dealloc
{
    if(_fileSender)
    {
        _fileSender = nil;
    }
    if(_fileReceiver)
    {
        _fileReceiver = nil;
    }
    [super dealloc];
}

#pragma mark chat delegate

-(void)onChatMessageNotification:(ZoomSDKChatInfo*)chatInfo
{
    ZoomSTRING zn_content = "";
    nativeErrorTypeHelp Help_type;
    ZNChatMsgInfo zn_chatInfo = Help_type.ConvertSDKChatMsgInfoToZNChatMsgInfo(chatInfo);

    _g_native_wrap.GetMeetingServiceWrap().GetMeetingChatCtrl().onChatMsgNotification(zn_chatInfo, zn_content);
}

-(void)onChatMessageEditNotification:(ZoomSDKChatInfo*)chatInfo
{
    nativeErrorTypeHelp Help_type;
    ZNChatMsgInfo zn_chatInfo = Help_type.ConvertSDKChatMsgInfoToZNChatMsgInfo(chatInfo);

    _g_native_wrap.GetMeetingServiceWrap().GetMeetingChatCtrl().onChatMessageEditNotification(zn_chatInfo);
}

-(void)onFileSendStart:(ZoomSDKFileSender *)sender
{
    self.fileSender = sender;
    
    ZoomSDKFileTransferInfo *transfer_info = sender.transferInfo;
    if (transfer_info)
    {
        ZNSDKFileTransferInfo zn_transfer_info;

        ZoomSTRING zn_messageID;
        NSString *messageId = transfer_info.messageId;
        if (messageId)
            zn_messageID = [messageId UTF8String];
        zn_transfer_info.messageID = zn_messageID;

        nativeErrorTypeHelp Help_type;
        ZoomSDKFileTransferStatus transfer_status = transfer_info.transferStatus;
        ZNSDKFileTransferStatus zn_transfer_status = Help_type.ZNSDKFileTransferStatusMap(transfer_status);
        zn_transfer_info.trans_status = zn_transfer_status;

        zn_transfer_info.time_stamp = transfer_info.timeStamp;
        zn_transfer_info.is_send_to_all = transfer_info.isSendToAll;
        zn_transfer_info.file_size = transfer_info.fileSizeBytes;

        ZoomSTRING zn_file_name;
        NSString *fileName = transfer_info.fileName;
        if (fileName)
            zn_file_name = [fileName UTF8String];
        zn_transfer_info.file_name = zn_file_name;

        zn_transfer_info.complete_percentage = transfer_info.completePercentage;
        zn_transfer_info.complete_size = transfer_info.completeSize;
        zn_transfer_info.bit_per_second = transfer_info.bitPerSecond;
        _g_native_wrap.GetMeetingServiceWrap().GetMeetingChatCtrl().onFileSendStart(zn_transfer_info);
    }
}

-(void)onFileReceived:(ZoomSDKFileReceiver *)receiver
{
    self.fileReceiver = receiver;
    
    ZoomSDKFileTransferInfo *transfer_info = receiver.transferInfo;
    if (transfer_info)
    {
        ZNSDKFileTransferInfo zn_transfer_info;

        ZoomSTRING zn_messageID;
        NSString *messageId = transfer_info.messageId;
        if (messageId)
            zn_messageID = [messageId UTF8String];
        zn_transfer_info.messageID = zn_messageID;

        nativeErrorTypeHelp Help_type;
        ZoomSDKFileTransferStatus transfer_status = transfer_info.transferStatus;
        ZNSDKFileTransferStatus zn_transfer_status = Help_type.ZNSDKFileTransferStatusMap(transfer_status);
        zn_transfer_info.trans_status = zn_transfer_status;

        zn_transfer_info.time_stamp = transfer_info.timeStamp;
        zn_transfer_info.is_send_to_all = transfer_info.isSendToAll;
        zn_transfer_info.file_size = transfer_info.fileSizeBytes;

        ZoomSTRING zn_file_name;
        NSString *fileName = transfer_info.fileName;
        if (fileName)
            zn_file_name = [fileName UTF8String];
        zn_transfer_info.file_name = zn_file_name;

        zn_transfer_info.complete_percentage = transfer_info.completePercentage;
        zn_transfer_info.complete_size = transfer_info.completeSize;
        zn_transfer_info.bit_per_second = transfer_info.bitPerSecond;
        _g_native_wrap.GetMeetingServiceWrap().GetMeetingChatCtrl().onFileReceived(zn_transfer_info);
    }
}

-(void)onFileTransferProgress:(ZoomSDKFileTransferInfo *)info
{
    if (info)
    {
        ZNSDKFileTransferInfo zn_transfer_info;

        ZoomSTRING zn_messageID;
        NSString *messageId = info.messageId;
        if (messageId)
            zn_messageID = [messageId UTF8String];
        zn_transfer_info.messageID = zn_messageID;

        nativeErrorTypeHelp Help_type;
        ZoomSDKFileTransferStatus transfer_status = info.transferStatus;
        ZNSDKFileTransferStatus zn_transfer_status = Help_type.ZNSDKFileTransferStatusMap(transfer_status);
        zn_transfer_info.trans_status = zn_transfer_status;

        zn_transfer_info.time_stamp = info.timeStamp;
        zn_transfer_info.is_send_to_all = info.isSendToAll;
        zn_transfer_info.file_size = info.fileSizeBytes;

        ZoomSTRING zn_file_name;
        NSString *fileName = info.fileName;
        if (fileName)
            zn_file_name = [fileName UTF8String];
        zn_transfer_info.file_name = zn_file_name;

        zn_transfer_info.complete_percentage = info.completePercentage;
        zn_transfer_info.complete_size = info.completeSize;
        zn_transfer_info.bit_per_second = info.bitPerSecond;
        _g_native_wrap.GetMeetingServiceWrap().GetMeetingChatCtrl().onFileTransferProgress(zn_transfer_info);
    }
}

@end

@implementation meetingServiceRecordDelegate
+(meetingServiceRecordDelegate *)share
{
    static meetingServiceRecordDelegate *delegate = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        delegate = [[meetingServiceRecordDelegate alloc] init];
    });
    return delegate;
}

-(instancetype)init
{
    self = [super init];
    if (self) {
        _cloudRecordingHandler = nil;
        _enableAndStartSmartRecordingHandler = nil;
        _smartRecordingEnableActionHandler = nil;
        return self;
    }
    return nil;
}

-(void)dealloc
{
    if(_cloudRecordingHandler)
    {
        _cloudRecordingHandler = nil;
        _enableAndStartSmartRecordingHandler = nil;
        _smartRecordingEnableActionHandler = nil;
    }
    [super dealloc];
}

#pragma mark recording delegate

-(void)onRecord2MP4Done:(BOOL)success Path:(NSString *)recordPath
{
    ZoomSTRING path = "";
    if (recordPath)
        path = [recordPath UTF8String];
    
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingRecordingCtrl().onRecording2MP4Done(success, 0, path);
}

-(void)onRecord2MP4Progressing:(int)percentage
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingRecordingCtrl().onRecording2MP4Processing(percentage);
}

- (void)onLocalRecordStatus:(ZoomSDKRecordingStatus)status userID:(unsigned int)userID
{
    nativeErrorTypeHelp help;
    ZNRecordingStatus znStatus = help.ZNRecordingStatusChange(status);
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingRecordingCtrl().onRecordingStatus(znStatus);
}

-(void)onRecordPrivilegeChange:(BOOL)canRec
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingRecordingCtrl().onRecordPrivilegeChanged(canRec);
}

-(void)onCloudRecordingStatus:(ZoomSDKRecordingStatus)status
{
    nativeErrorTypeHelp help;
    ZNRecordingStatus znStatus = help.ZNRecordingStatusChange(status);
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingRecordingCtrl().onCloudRecordingStatus(znStatus);
}

-(void)onCloudRecordingStorageFull:(time_t)gracePeriodDate
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingRecordingCtrl().onCloudRecordingStorageFull(gracePeriodDate);
}


-(void)onRequestCloudRecordingResponse:(ZoomSDKRequestStartCloudRecordingStatus)status
{
    nativeErrorTypeHelp help;
    ZNRequestStartCloudRecordingStatus znStatus = help.ZNRequestStartCloudRecordingStatusMap(status);
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingRecordingCtrl().onRequestCloudRecordingResponse(znStatus);
}

-(void)onStartCloudRecordingRequested:(ZoomSDKRequestStartCloudRecordingHandler *)handler
{
    self.cloudRecordingHandler = handler;
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingRecordingCtrl().onStartCloudRecordingRequested();
}

-(void)onEnableAndStartSmartRecordingRequested:(ZoomSDKRequestEnableAndStartSmartRecordingHandler *)handler
{
    self.enableAndStartSmartRecordingHandler = handler;
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingRecordingCtrl().onEnableAndStartSmartRecordingRequested();
}

-(void)onSmartRecordingEnableActionCallback:(ZoomSDKSmartRecordingEnableActionHandler *)handler
{
    self.smartRecordingEnableActionHandler = handler;
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingRecordingCtrl().onSmartRecordingEnableActionCallback();
}

@end

@implementation meetingServiceWaitingRoomDelegate
+(meetingServiceWaitingRoomDelegate *)share
{
    static meetingServiceWaitingRoomDelegate *delegate = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        delegate = [[meetingServiceWaitingRoomDelegate alloc] init];
    });
    return delegate;
}

-(instancetype)init
{
    self = [super init];
    if (self) {
        _waitingRoomHandler = nil;
        return self;
    }
    return nil;
}

-(void)dealloc
{
    if(_waitingRoomHandler)
    {
        _waitingRoomHandler = nil;
    }
    [super dealloc];
}

#pragma mark waiting room delegate

-(void)onUserJoinWaitingRoom:(unsigned int)userid
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingWaitingRoomCtrl().onWaitingRoomUserJoin(userid);
}

-(void)onUserLeftWaitingRoom:(unsigned int)userid
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingWaitingRoomCtrl().onWaitingRoomUserLeft(userid);
}

-(void)onWaitingRoomPresetAudioStatusChanged:(BOOL)audioCanTurnOn
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingWaitingRoomCtrl().onWaitingRoomPresetAudioStatusChanged(audioCanTurnOn);
}

-(void)onWaitingRoomPresetVideoStatusChanged:(BOOL)videoCanTurnOn
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingWaitingRoomCtrl().onWaitingRoomPresetVideoStatusChanged(videoCanTurnOn);
}

-(void)onCustomWaitingRoomDataUpdated:(ZoomSDKCustomWaitingRoomData*)bData handle:(ZoomSDKWaitingRoomDataDownloadHandler*)handle
{
    self.waitingRoomHandler = handle;
    ZNCustomWaitingRoomData zn_custom_waiting_room_data;

    ZoomSTRING zn_title;
    NSString *title = bData.title;
    if (title)
        zn_title = [title UTF8String];
    zn_custom_waiting_room_data.title = zn_title;

    ZoomSTRING zn_description;
    NSString *dataDescription = bData.dataDescription;
    if (dataDescription)
        zn_description = [dataDescription UTF8String];
    zn_custom_waiting_room_data.description = zn_description;

    ZoomSTRING zn_logo_path;
    NSString *logoPath = bData.logoPath;
    if (logoPath)
        zn_logo_path = [logoPath UTF8String];
    zn_custom_waiting_room_data.logo_path = zn_logo_path;

    ZoomSTRING zn_video_path;
    NSString *videoPath = bData.videoPath;
    if (videoPath)
        zn_video_path = [videoPath UTF8String];
    zn_custom_waiting_room_data.video_path = zn_video_path;

    ZoomSTRING zn_image_path;
    NSString *imagePath = bData.imagePath;
    if (imagePath)
        zn_image_path = [imagePath UTF8String];
    zn_custom_waiting_room_data.image_path = zn_image_path;

    nativeErrorTypeHelp Help_type;
    ZoomSDKWaitingRoomLayoutType type = bData.type;
    ZNWaitingRoomLayoutType zn_type = Help_type.ZNWaitingRoomLayoutTypeMap(type);
    zn_custom_waiting_room_data.type = zn_type;

    ZoomSDKCustomWaitingRoomDataStatus status = bData.status;
    ZNCustomWaitingRoomDataStatus zn_status = Help_type.ZNCustomWaitingRoomDataStatusMap(status);
    zn_custom_waiting_room_data.status = zn_status;

    ZoomSDKWaitingRoomBrandingPageColor* pageColor = bData.pageColor;
    if (pageColor) {
        NSColor* backgroundColor = pageColor.backgroundColor;
        if (backgroundColor) {
            CGFloat red = 0, green = 0, blue = 0, alpha = 0;
            [backgroundColor getRed:&red green:&green blue:&blue alpha:&alpha];
            zn_custom_waiting_room_data.page_color.background_color.red = (int)(red * 255);
            zn_custom_waiting_room_data.page_color.background_color.green = (int)(green * 255);
            zn_custom_waiting_room_data.page_color.background_color.blue = (int)(blue * 255);
        }
    }

    ZoomSDKWaitingRoomBrandingTextColor* textColor = bData.textColor;
    if (textColor) {
        NSColor* primaryColor = textColor.primaryColor;
        if (primaryColor) {
            CGFloat red = 0, green = 0, blue = 0, alpha = 0;
            [primaryColor getRed:&red green:&green blue:&blue alpha:&alpha];
            zn_custom_waiting_room_data.text_color.primary_color.red = (int)(red * 255);
            zn_custom_waiting_room_data.text_color.primary_color.green = (int)(green * 255);
            zn_custom_waiting_room_data.text_color.primary_color.blue = (int)(blue * 255);
        }

        NSColor* secondaryColor = textColor.secondaryColor;
        if (secondaryColor) {
            CGFloat red = 0, green = 0, blue = 0, alpha = 0;
            [secondaryColor getRed:&red green:&green blue:&blue alpha:&alpha];
            zn_custom_waiting_room_data.text_color.secondary_color.red = (int)(red * 255);
            zn_custom_waiting_room_data.text_color.secondary_color.green = (int)(green * 255);
            zn_custom_waiting_room_data.text_color.secondary_color.blue = (int)(blue * 255);
        }

        NSColor* paragraphColor = textColor.paragraphColor;
        if (paragraphColor) {
            CGFloat red = 0, green = 0, blue = 0, alpha = 0;
            [paragraphColor getRed:&red green:&green blue:&blue alpha:&alpha];
            zn_custom_waiting_room_data.text_color.paragraph_color.red = (int)(red * 255);
            zn_custom_waiting_room_data.text_color.paragraph_color.green = (int)(green * 255);
            zn_custom_waiting_room_data.text_color.paragraph_color.blue = (int)(blue * 255);
        }

        NSColor* hyperlinkColor = textColor.hyperlinkColor;
        if (hyperlinkColor) {
            CGFloat red = 0, green = 0, blue = 0, alpha = 0;
            [hyperlinkColor getRed:&red green:&green blue:&blue alpha:&alpha];
            zn_custom_waiting_room_data.text_color.hyperlink_color.red = (int)(red * 255);
            zn_custom_waiting_room_data.text_color.hyperlink_color.green = (int)(green * 255);
            zn_custom_waiting_room_data.text_color.hyperlink_color.blue = (int)(blue * 255);
        }
    }

    ZoomSDKWaitingRoomBrandingButtonColor* buttonColor = bData.buttonColor;
    if (buttonColor) {
        NSColor* primaryButtonColor = buttonColor.primaryButtonColor;
        if (primaryButtonColor) {
            CGFloat red = 0, green = 0, blue = 0, alpha = 0;
            [primaryButtonColor getRed:&red green:&green blue:&blue alpha:&alpha];
            zn_custom_waiting_room_data.button_color.primary_button_color.red = (int)(red * 255);
            zn_custom_waiting_room_data.button_color.primary_button_color.green = (int)(green * 255);
            zn_custom_waiting_room_data.button_color.primary_button_color.blue = (int)(blue * 255);
        }
    }
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingWaitingRoomCtrl().onCustomWaitingRoomDataUpdated(zn_custom_waiting_room_data);
}

-(void)onWaitingRoomUserNameChanged:(unsigned int)userID userName:(NSString *)userName
{
    ZoomSTRING zn_userName = "";
    if (userName) 
        zn_userName = [userName UTF8String];
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingWaitingRoomCtrl().onWaitingRoomUserNameChanged(userID, zn_userName);
}

-(void)onWaitingRoomEntranceEnabled:(BOOL)enabled
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingWaitingRoomCtrl().onWaitingRoomEntranceEnabled(enabled);
}

@end

@implementation meetingUIControllerDelegate

#pragma mark meeting ui delegate
- (void)onToolbarInviteButtonClick
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    ZoomSDKMeetingConfiguration *config = [service getMeetingConfiguration];
    bool  isInvite = false;
    if (service && config) {
        isInvite = [config disableToolbarInviteButtonClickOriginAction];
    }
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingUICtrl().onInviteBtnClicked(isInvite);
}

-(void)onToolbarShareButtonClick
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingUICtrl().onStartShareBtnClicked();
}

-(void)onParticipantButtonClicked
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingUICtrl().onParticipantListBtnClicked();
}

-(void)onCustomLiveButtonClicked
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingUICtrl().onCustomLiveStreamMenuClicked();
}

-(void)onNeedShowLeaveMeetingWindow
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingUICtrl().onEndMeetingBtnClicked();
}

-(void)onAudioButtonClicked:(unsigned int)userid audioSession:(ZoomSDKAudioActionInfo)info
{
    nativeErrorTypeHelp help;
    ZNAudioCallbackActionInfo Actioninfo = help.ZNSDKAudioCallbackAction(info);
    ZNAudioBtnClickedCallbackInfo callbackInfo;
    callbackInfo.userid_MuteUnmute = userid;
    callbackInfo.audio_clicked_action = Actioninfo;
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingUICtrl().onAudioBtnClicked(callbackInfo);
}

-(void)onAudioSettingMenuButtonClicked
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingUICtrl().onAudioMenuBtnClicked();
}

-(void)onCloseCaptionButtonClick
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingUICtrl().onCCBTNClicked();
}

@end

@implementation meetingH323HelperDelegate

-(void)onCalloutStatusReceived:(H323CalloutStatus)calloutStatus
{
    nativeErrorTypeHelp help_type;
    ZNH323CalloutStatus status = help_type.ZoomSDKH323Status(calloutStatus);
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingH323Ctrl().onH323CalloutStatusNotify(status);
}
@end

@implementation meetingASControllerDelegate

+(meetingASControllerDelegate *)share
{
    static meetingASControllerDelegate *delegate = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        delegate = [[meetingASControllerDelegate alloc] init];
    });
    return delegate;
}

-(void)onSharingStatusChanged:(ZoomSDKSharingSourceInfo*)shareInfo
{
    nativeErrorTypeHelp Help_type;
    ZNZoomSDKSharingSourceInfo zn_shareInfo = Help_type.ConvertSDKSharingSourceInfoToZNSharingSourceInfo(shareInfo);

    _g_native_wrap.GetMeetingServiceWrap().GetMeetingShareCtrl().onSharingStatus(zn_shareInfo);
}

-(void)onFailedToStartShare
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingShareCtrl().onFailedToStartShare();
}

-(void)onAnnotationSupportPropertyChanged:(BOOL)isSupportAnnotation shareSourceID:(unsigned int)shareSourceID
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingAnnotation().onSupportAnnotationStatus(shareSourceID, isSupportAnnotation);
}

-(void)onOptimizingShareForVideoClipStatusChanged:(ZoomSDKSharingSourceInfo*)shareInfo
{
    nativeErrorTypeHelp Help_type;
    ZNZoomSDKSharingSourceInfo zn_shareInfo = Help_type.ConvertSDKSharingSourceInfoToZNSharingSourceInfo(shareInfo);

    _g_native_wrap.GetMeetingServiceWrap().GetMeetingShareCtrl().onOptimizingShareForVideoClipStatusChanged(zn_shareInfo);
}
@end

@implementation realNameAuthenticationDelegate
+(realNameAuthenticationDelegate *)share
{
    static realNameAuthenticationDelegate *delegate = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        delegate = [[realNameAuthenticationDelegate alloc] init];
    });
    return delegate;
}

-(instancetype)init
{
    self = [super init];
    if (self) {
        _retriveController = nil;
        _verifyCintroller = nil;
        return self;
    }
    return nil;
}

-(void)dealloc
{
    if (_retriveController) {
        [_retriveController release];
        _retriveController = nil;
    }
    
    if (_verifyCintroller) {
        [_verifyCintroller release];
        _verifyCintroller = nil;
    }
    [super dealloc];
}
#pragma mark  sms
-(void)onNeedRealNameAuthMeetingWithSupportCountryList:(NSArray *)supportCountryList withPrivacyURL:(NSString *)privacyURL withRetrieveSMSVerificationCodeHandler:(ZoomSDKRetrieveSMSVerificationCodeController *)handler
{
    self.retriveController = handler;
    ZNList<ZNZoomRealNameAuthCountryInfo> infoList;
    for (ZoomSDKRealNameAuthCountryInfo *info in supportCountryList) {
        ZNZoomRealNameAuthCountryInfo countryInfo;
        countryInfo.countryCode = [info getCountryCode].UTF8String;
        countryInfo.countryID = [info getCountryID].UTF8String;
        countryInfo.countryName = [info getCountryName].UTF8String;
        infoList.push_back(countryInfo);
    }
    _g_native_wrap.GetMeetingServiceWrap().GetSDKSMSHelper().onNeedRealNameAuthMeetingNotification(infoList, privacyURL.UTF8String);
}

-(void)onRetrieveSMSVerificationCodeResult:(ZoomSDKSMSVerificationCodeError)result withVerifySMSVerificationCodeHandle:(ZoomSDKVerifySMSVerificationCodeController *)handler
{
    self.verifyCintroller = handler;
    nativeErrorTypeHelp help;
    ZNSMSVerificationCodeErr ret = help.ZNSDKSMSVerificationCodeError(result);
    _g_native_wrap.GetMeetingServiceWrap().GetSDKSMSHelper().onRetrieveSMSVerificationCodeResultNotification(ret);
}

-(void)onVerifySMSVerificationCodeResult:(ZoomSDKSMSVerificationCodeError)result
{
    nativeErrorTypeHelp help;
    ZNSMSVerificationCodeErr ret = help.ZNSDKSMSVerificationCodeError(result);
    _g_native_wrap.GetMeetingServiceWrap().GetSDKSMSHelper().onVerifySMSVerificationCodeResultNotification(ret);
}

-(BOOL)getResendRetriveController
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) {
        return NO;
    }
    ZoomSDKRealNameAuthenticationController *controller = [service getRealNameController];
    if (!controller) {
        return NO;
    }
    self.retriveController = [controller resendSMSVerificationCodeController];
    if (self.retriveController) {
        return YES;
    }
    return NO;
}

-(BOOL)getReVerifyCintroller
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) {
        return NO;
    }
    ZoomSDKRealNameAuthenticationController *controller = [service getRealNameController];
    if (!controller) {
        return NO;
    }
    self.verifyCintroller = [controller reVerifySMSVerificationCodeController];
    if (self.verifyCintroller) {
        return YES;
    }
    return NO;
}
@end

@implementation WebinarControllerDelegate
+(WebinarControllerDelegate *)share
{
    static WebinarControllerDelegate *delegate = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        delegate = [[WebinarControllerDelegate alloc] init];
    });
    return delegate;
}

-(instancetype)init
{
    self = [super init];
    if (self) {
        _webinarRegisterHelper = nil;
        _webinarInputScreenNameHandler = nil;
        return self;
    }
    return nil;
}

-(void)dealloc
{
    if (_webinarRegisterHelper) {
        [_webinarRegisterHelper release];
        _webinarRegisterHelper = nil;
    }
    if (_webinarInputScreenNameHandler) {
        [_webinarInputScreenNameHandler release];
        _webinarInputScreenNameHandler = nil;
    }
    [super dealloc];
}

-(ZoomSDKError)onWebinarNeedRegisterResponse:(ZoomSDKWebinarRegisterHelper *)webinarRegisterHelper
{
    self.webinarRegisterHelper = webinarRegisterHelper;
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingConfigCtrl().onWebinarNeedRegisterNotification();
}


-(void)onWebinarNeedInputScreenName:(ZoomSDKWebinarInputScreenNameHandler*)handler
{
    self.webinarInputScreenNameHandler = handler;
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingConfigCtrl().onWebinarNeedInputScreenName();
}

-(void)onSelfAllowTalkNoti
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingWebinarCtrl().onSelfAllowTalkNotification();
}

-(void)onSelfDisallowTalkNoti
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingWebinarCtrl().onSelfDisallowTalkNotification();
}

-(void)onPromoteAttendee2PanelistResultNoti:(unsigned int)result
{    
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingWebinarCtrl().onPromptAttendee2PanelistResult(result);
}

-(void)onDepromotePanelist2AttendeeResultNoti:(unsigned int)result
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingWebinarCtrl().onDepromptPanelist2AttendeeResult(result);
}

-(void)onAllowPanelistStartVideoNoti
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingWebinarCtrl().onAllowPanelistStartVideoNotification();
}

-(void)onDisAllowPanelistStartVideoNoti
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingWebinarCtrl().onDisallowPanelistStartVideoNotification();
}

-(void)onAttendeeAudioStatusNoti:(unsigned int)userid canTalk:(BOOL)can_talk isMuted:(BOOL)is_muted
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingWebinarCtrl().onAttendeeAudioStatusNotification(userid, can_talk, is_muted);
}

-(void)onAttendeePromoteConfirmResult:(unsigned int)userid isAgree:(BOOL)agree
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingWebinarCtrl().onAttendeePromoteConfirmResult(agree, userid);
}

-(void)onAllowWebinarReactionStatusChanged:(BOOL)isAllowed
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingWebinarCtrl().onAllowWebinarReactionStatusChanged(isAllowed);
}

-(void)onAllowAttendeeRaiseHandStatusChanged:(BOOL)isAllowed
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingWebinarCtrl().onAllowAttendeeRaiseHandStatusChanged(isAllowed);
}

-(void)onAllowAttendeeViewTheParticipantCountStatusChanged:(BOOL)isAllowed
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingWebinarCtrl().onAllowAttendeeViewTheParticipantCountStatusChanged(isAllowed);
}
@end

@implementation meetingServiceAICompanionControllerDelegate
+(meetingServiceAICompanionControllerDelegate *)share
{
    static meetingServiceAICompanionControllerDelegate *delegate = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        delegate = [[meetingServiceAICompanionControllerDelegate alloc] init];
    });
    return delegate;
}

-(instancetype)init
{
    self = [super init];
    if (self) {
        _aiCompanionFeatureTurnOnAgainHandler = nil;
        _aiCompanionFeatureSwitchHandler = nil;
        return self;
    }
    return nil;
}

-(void)dealloc
{
    if(_aiCompanionFeatureTurnOnAgainHandler)
    {
        _aiCompanionFeatureTurnOnAgainHandler = nil;
    }
    if(_aiCompanionFeatureSwitchHandler)
    {
        _aiCompanionFeatureSwitchHandler = nil;
    }
    [super dealloc];
}

#pragma mark ai companion delegate

-(void)onAICompanionFeatureTurnOffByParticipant:(ZoomSDKAICompanionFeatureTurnOnAgainHandler*)handler
{
    self.aiCompanionFeatureTurnOnAgainHandler = handler;
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().onAICompanionFeatureTurnOffByParticipant();
}

-(void)onAICompanionFeatureSwitchRequested:(ZoomSDKAICompanionFeatureSwitchHandler*)handler
{
    self.aiCompanionFeatureSwitchHandler = handler;
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().onAICompanionFeatureSwitchRequested();
}

-(void)onAICompanionFeatureSwitchRequestResponse:(BOOL)timeout isAgree:(BOOL)agree isTurnOn:(BOOL)turnOn
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().onAICompanionFeatureSwitchRequestResponse(timeout, agree, turnOn);
}

-(void)onAICompanionFeatureCanNotBeTurnedOff:(NSArray*)features
{
    if (!features || features.count == 0) {
        return;
    }
    ZNList<ZNAICompanionFeature> zn_list;
    nativeErrorTypeHelp help;
    for (NSNumber *item in features) {
        ZoomSDKAICompanionFeature featureValue = (ZoomSDKAICompanionFeature)[item unsignedIntValue];
        ZNAICompanionFeature zn_feature = help.ZNAICompanionFeatureMap(featureValue);
        zn_list.push_back(zn_feature);
    }
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().onAICompanionFeatureCanNotBeTurnedOff(zn_list);
}
@end

@implementation meetingServiceAICompanionSmartSummaryDelegate
+(meetingServiceAICompanionSmartSummaryDelegate *)share
{
    static meetingServiceAICompanionSmartSummaryDelegate *delegate = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        delegate = [[meetingServiceAICompanionSmartSummaryDelegate alloc] init];
    });
    return delegate;
}

-(instancetype)init
{
    self = [super init];
    if (self) {
        _enableSmartSummaryHandler = nil;
        _startSmartSummaryHandler = nil;
        _stopSmartSummaryHandler = nil;
        _approveEnableSmartSummaryHandler = nil;
        _approveStartSmartSummaryHandler = nil;
        _enableSmartSummaryActionHandler = nil;
        return self;
    }
    return nil;
}

-(void)dealloc
{
    if(_enableSmartSummaryHandler)
    {
        _enableSmartSummaryHandler = nil;
    }
    if(_startSmartSummaryHandler)
    {
        _startSmartSummaryHandler = nil;
    }
    if(_stopSmartSummaryHandler)
    {
        _stopSmartSummaryHandler = nil;
    }
    if(_approveEnableSmartSummaryHandler)
    {
        _approveEnableSmartSummaryHandler = nil;
    }
    if(_approveStartSmartSummaryHandler)
    {
        _approveStartSmartSummaryHandler = nil;
    }
    if(_enableSmartSummaryActionHandler)
    {
        _enableSmartSummaryActionHandler = nil;
    }
    [super dealloc];
}

#pragma mark ai companion smart summary delegate

-(void)onSmartSummaryStateNotSupported
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().onSmartSummaryStateNotSupported();
}

-(void)onSmartSummaryStateSupportedButDisabled:(ZoomSDKEnableSmartSummaryHandler* _Nullable)handler
{
    self.enableSmartSummaryHandler = handler;
    
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().onSmartSummaryStateSupportedButDisabled();
}

-(void)onSmartSummaryStateEnabledButNotStarted:(ZoomSDKStartSmartSummaryHandler* _Nullable)handler
{
    self.startSmartSummaryHandler = handler;
    
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().onSmartSummaryStateEnabledButNotStarted();
}

-(void)onSmartSummaryStateStarted:(ZoomSDKStopSmartSummaryHandler* _Nullable)handler
{
    self.stopSmartSummaryHandler = handler;
    
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().onSmartSummaryStateStarted();
}

-(void)onFailedToStartSmartSummary:(BOOL)timeout
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().onFailedToStartSmartSummary(timeout);
}

-(void)onSmartSummaryEnableRequestReceived:(ZoomSDKApproveEnableSmartSummaryHandler* _Nullable)handler
{
    self.approveEnableSmartSummaryHandler = handler;
    
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().onSmartSummaryEnableRequestReceived();
}

-(void)onSmartSummaryStartRequestReceived:(ZoomSDKApproveStartSmartSummaryHandler* _Nullable)handler
{
    self.approveStartSmartSummaryHandler = handler;
    
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().onSmartSummaryStartRequestReceived();
}

-(void)onSmartSummaryEnableActionCallback:(ZoomSDKEnableSmartSummaryActionHandler* _Nullable)handler
{
    self.enableSmartSummaryActionHandler = handler;
    
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().onSmartSummaryEnableActionCallback();
}

@end

@implementation meetingServiceAICompanionQueryDelegate
+(meetingServiceAICompanionQueryDelegate *)share
{
    static meetingServiceAICompanionQueryDelegate *delegate = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        delegate = [[meetingServiceAICompanionQueryDelegate alloc] init];
    });
    return delegate;
}

-(instancetype)init
{
    self = [super init];
    if (self) {
        _enableQueryHandler = nil;
        _startQueryHandler = nil;
        _sendQueryHandler = nil;
        _approveEnableQueryHandler = nil;
        _approveStartQueryHandler = nil;
        _aiCompanionQueryItem = nil;
        _enableQueryActionHandler = nil;
        _approveSendQueryHandler = nil;
        return self;
    }
    return nil;
}

-(void)dealloc
{
    if (_enableQueryHandler)
    {
        _enableQueryHandler = nil;
    }
    if (_startQueryHandler)
    {
        _startQueryHandler = nil;
    }
    if (_sendQueryHandler)
    {
        _sendQueryHandler = nil;
    }
    if (_approveEnableQueryHandler)
    {
        _approveEnableQueryHandler = nil;
    }
    if (_approveStartQueryHandler)
    {
        _approveStartQueryHandler = nil;
    }
    if (_aiCompanionQueryItem)
    {
        _aiCompanionQueryItem = nil;
    }
    if (_enableQueryActionHandler)
    {
        _enableQueryActionHandler = nil;
    }
    if (_approveSendQueryHandler)
    {
        _approveSendQueryHandler = nil;
    }
    [super dealloc];
}

#pragma mark ai companion query delegate

-(void)onQueryStateNotSupported
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().onQueryStateNotSupported();
}

-(void)onQueryStateSupportedButDisabled:(ZoomSDKEnableQueryHandler*)handler
{
    self.enableQueryHandler = handler;
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().onQueryStateSupportedButDisabled();
}

-(void)onQueryStateEnabledButNotStarted:(ZoomSDKStartQueryHandler*)handler
{
    self.startQueryHandler = handler;
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().onQueryStateEnabledButNotStarted();
}

-(void)onQueryStateStarted:(ZoomSDKSendQueryHandler*)handler
{
    self.sendQueryHandler = handler;
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().onQueryStateStarted();
}

-(void)onQuerySettingChanged:(ZoomSDKAICompanionQuerySettingOptions)setting
{    
    nativeErrorTypeHelp help;
    ZNMeetingAICompanionQuerySettingOptions zn_setting = help.ZNMeetingAICompanionQuerySettingOptionsMap(setting);
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().onQuerySettingChanged(zn_setting);
}

-(void)onFailedToStartQuery:(BOOL)bTimeout
{    
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().onFailedToStartQuery(bTimeout);
}

-(void)onReceiveRequestToEnableQuery:(ZoomSDKApproveEnableQueryHandler *)handler
{
    self.approveEnableQueryHandler = handler;
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().onReceiveRequestToEnableQuery();
}

-(void)onReceiveRequestToStartQuery:(ZoomSDKApproveStartQueryHandler *_Nullable)handler
{
    self.approveStartQueryHandler = handler;
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().onReceiveRequestToStartQuery();
}

-(void)onReceiveQueryAnswer:(ZoomSDKAICompanionQueryItem*)queryItem
{
    self.aiCompanionQueryItem = queryItem;
    ZNMeetingAICompanionQueryItem zn_queryItem; 

    ZoomSTRING zn_queryID;
    NSString *queryID = queryItem.queryID;
    if (queryID)
        zn_queryID = [queryID UTF8String];
    zn_queryItem.queryID = zn_queryID;

    ZoomSTRING zn_qustionContent;
    NSString *qustionContent = queryItem.qustionContent;
    if (qustionContent)
        zn_qustionContent = [qustionContent UTF8String];
    zn_queryItem.qustionContent = zn_qustionContent;

    ZoomSTRING zn_answerContent;
    NSString *answerContent = queryItem.answerContent;
    if (answerContent)
        zn_answerContent = [answerContent UTF8String];
    zn_queryItem.answerContent = zn_answerContent;

    ZoomSTRING zn_errorMsg;
    NSString *errorMsg = queryItem.errorMsg;
    if (errorMsg)
        zn_errorMsg = [errorMsg UTF8String];
    zn_queryItem.errorMsg = zn_errorMsg;

    zn_queryItem.timeStamp = queryItem.timestamp;
    
    nativeErrorTypeHelp Help_type;
    ZoomSDKAICompanionQueryRequestError errorCode = queryItem.errorCode;
    zn_queryItem.errorCode = Help_type.ZNMeetingAICompanionQueryRequestErrorMap(errorCode);

    _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().onReceiveQueryAnswer(zn_queryItem);
}

-(void)onQueryEnableActionCallback:(ZoomSDKEnableQueryActionHandler *)handler
{
    self.enableQueryActionHandler = handler;
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().onQueryEnableActionCallback();
}

-(void)onReceiveRequestToSendQuery:(ZoomSDKApproveSendQueryHandler *_Nullable)handler
{
    self.approveSendQueryHandler = handler;
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingAICompanionCtrl().onReceiveRequestToSendQuery();
}

@end

@implementation meetingServiceIndicatorControllerDelegate
+(meetingServiceIndicatorControllerDelegate *)share
{
    static meetingServiceIndicatorControllerDelegate *delegate = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        delegate = [[meetingServiceIndicatorControllerDelegate alloc] init];
    });
    return delegate;
}

-(instancetype)init
{
    self = [super init];
    if (self) {
        _indicatorHandle = nil;
        return self;
    }
    return nil;
}

-(void)dealloc
{
    if(_indicatorHandle)
    {
        _indicatorHandle = nil;
    }
    [super dealloc];
}

#pragma mark ai companion delegate

-(void)onIndicatorItemReceived:(ZoomSDKMeetingIndicatorHandle *)handler
{
    self.indicatorHandle = handler;
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingIndicatorCtrl().onIndicatorItemReceived();
}

-(void)onIndicatorItemRemoved:(ZoomSDKMeetingIndicatorHandle *)handler
{
    self.indicatorHandle = handler;
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingIndicatorCtrl().onIndicatorItemRemoved();
}

@end

@implementation meetingServiceQADelegate

-(void)onAddQuestion:(NSString *)questionID  isSuccess:(BOOL)success
{
    ZoomSTRING zn_questionID = "";
    if (questionID)
        zn_questionID = [questionID UTF8String];

    _g_native_wrap.GetMeetingServiceWrap().GetMeetingQACtrl().onAddQuestion(zn_questionID, success);
}

-(void)onReceiveQuestion:(NSString *)questionID
{
    ZoomSTRING zn_questionID = "";
    if (questionID)
        zn_questionID = [questionID UTF8String];

    _g_native_wrap.GetMeetingServiceWrap().GetMeetingQACtrl().onReceiveQuestion(zn_questionID);
}

-(void)onDeleteQuestions:(NSArray *)questions
{
    if (!questions || questions.count == 0) {
        return;
    }
    ZNList<ZoomSTRING> zn_list;
    for (NSString *num in questions) {
        ZoomSTRING zn_questionID = [num UTF8String];
        zn_list.push_back(zn_questionID);
    }
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingQACtrl().onDeleteQuestions(zn_list);
}

-(void)onQuestionMarkedAsDismissed:(NSString *)questionID
{
    ZoomSTRING zn_questionID = "";
    if (questionID)
        zn_questionID = [questionID UTF8String];

    _g_native_wrap.GetMeetingServiceWrap().GetMeetingQACtrl().onQuestionMarkedAsDismissed(zn_questionID);
}

-(void)onReopenQuestion:(NSString *)questionID
{
    ZoomSTRING zn_questionID = "";
    if (questionID)
        zn_questionID = [questionID UTF8String];

    _g_native_wrap.GetMeetingServiceWrap().GetMeetingQACtrl().onReopenQuestion(zn_questionID);
}

-(void)onUserLivingReply:(NSString *)questionID
{
    ZoomSTRING zn_questionID = "";
    if (questionID)
        zn_questionID = [questionID UTF8String];

    _g_native_wrap.GetMeetingServiceWrap().GetMeetingQACtrl().onUserLivingReply(zn_questionID);
}

-(void)onUserEndLiving:(NSString *)questionID
{
    ZoomSTRING zn_questionID = "";
    if (questionID)
        zn_questionID = [questionID UTF8String];

    _g_native_wrap.GetMeetingServiceWrap().GetMeetingQACtrl().onUserEndLiving(zn_questionID);
}

-(void)onAddAnswer:(NSString *)answerID  isSuccess:(BOOL)success
{
    ZoomSTRING zn_answerID = "";
    if (answerID)
        zn_answerID = [answerID UTF8String];

    _g_native_wrap.GetMeetingServiceWrap().GetMeetingQACtrl().onAddAnswer(zn_answerID, success);
}

-(void)onReceiveAnswer:(NSString *_Nullable)answerID
{
    ZoomSTRING zn_answerID = "";
    if (answerID)
        zn_answerID = [answerID UTF8String];

    _g_native_wrap.GetMeetingServiceWrap().GetMeetingQACtrl().onReceiveAnswer(zn_answerID);
}

-(void)onDeleteAnswers:(NSArray *)answer
{
    if (!answer || answer.count == 0) {
        return;
    }
    ZNList<ZoomSTRING> zn_list;
    for (NSString *num in answer) {
        ZoomSTRING zn_answerID = [num UTF8String];
        zn_list.push_back(zn_answerID);
    }
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingQACtrl().onDeleteAnswers(zn_list);
}

-(void)onVoteupQuestion:(NSString *)questionID orderChanged:(BOOL)isChanged
{
    ZoomSTRING zn_questionID = "";
    if (questionID)
        zn_questionID = [questionID UTF8String];

    _g_native_wrap.GetMeetingServiceWrap().GetMeetingQACtrl().onVoteupQuestion(zn_questionID, isChanged);
}

-(void)onRevokeVoteupQuestion:(NSString *)questionID orderChanged:(BOOL)isChanged
{
    ZoomSTRING zn_questionID = "";
    if (questionID)
        zn_questionID = [questionID UTF8String];

    _g_native_wrap.GetMeetingServiceWrap().GetMeetingQACtrl().onRevokeVoteupQuestion(zn_questionID, isChanged);
}

@end

@implementation meetingServiceReactionDelegate
-(void)onEmojiReactionReceived:(unsigned int)userid reactionType:(ZoomSDKEmojiReactionType)type reactionSkinTone:(ZoomSDKEmojiReactionSkinTone)skinTone
{
    nativeErrorTypeHelp help;
    ZNSDKEmojiReactionType zn_type = help.ZNSDKEmojiReactionTypeMap(type);
    ZNSDKEmojiReactionSkinTone zn_skinTone = help.ZNSDKEmojiReactionSkinToneMap(skinTone);
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingReactionCtrl().onEmojiReactionReceived(userid, zn_type, zn_skinTone);
}

-(void)onEmojiReactionReceivedInWebinar:(ZoomSDKEmojiReactionType)type
{
    nativeErrorTypeHelp help;
    ZNSDKEmojiReactionType zn_type = help.ZNSDKEmojiReactionTypeMap(type);
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingReactionCtrl().onEmojiReactionReceivedInWebinar(zn_type);
}
@end

@implementation meetingServiceCloseCaptionDelegate
- (void)onLiveTranscriptionMsgInfoReceived:(ZoomSDKLiveTranscriptionMessageInfo*_Nullable)messageInfo
{
    ZNLiveTranscriptionMessageInfo zn_message_info;

    // messageID
    ZoomSTRING zn_messageID;
    NSString *messageID = messageInfo.messageID;
    if (messageID)
        zn_messageID = [messageID UTF8String];
    zn_message_info.messageID = zn_messageID;

    // speakerID
    zn_message_info.speakerID = messageInfo.speakerID;

    // speakerName
    ZoomSTRING zn_speakerName;
    NSString *speakerName = messageInfo.speakerName;
    if (speakerName)
        zn_speakerName = [speakerName UTF8String];
    zn_message_info.speakerName = zn_speakerName;

    // messageContent
    ZoomSTRING zn_messageContent;
    NSString *messageContent = messageInfo.messageContent;
    if (messageContent)
        zn_messageContent = [messageContent UTF8String];
    zn_message_info.messageContent = zn_messageContent;

    // timeStamp
    zn_message_info.timeStamp = messageInfo.timeStamp;

    // messageType
    nativeErrorTypeHelp Help_type;
    ZoomSDKLiveTranscriptionOperationType messageType = messageInfo.messageType;
    ZNSDKLiveTranscriptionOperationType zn_messageType = Help_type.ZNSDKLiveTranscriptionOperationTypeMap(messageType);
    zn_message_info.messageType = zn_messageType;

    _g_native_wrap.GetMeetingServiceWrap().GetMeetingCloseCaptionCtrl().onLiveTranscriptionMsgInfoReceived(zn_message_info);
}
- (void)onOriginalLanguageMsgReceived:(ZoomSDKLiveTranscriptionMessageInfo*_Nullable)messageInfo
{
    ZNLiveTranscriptionMessageInfo zn_message_info;

    ZoomSTRING zn_messageID;
    NSString *messageID = messageInfo.messageID;
    if (messageID)
        zn_messageID = [messageID UTF8String];
    zn_message_info.messageID = zn_messageID;

    zn_message_info.speakerID = messageInfo.speakerID;

    ZoomSTRING zn_speakerName;
    NSString *speakerName = messageInfo.speakerName;
    if (speakerName)
        zn_speakerName = [speakerName UTF8String];
    zn_message_info.speakerName = zn_speakerName;

    ZoomSTRING zn_messageContent;
    NSString *messageContent = messageInfo.messageContent;
    if (messageContent)
        zn_messageContent = [messageContent UTF8String];
    zn_message_info.messageContent = zn_messageContent;

    zn_message_info.timeStamp = messageInfo.timeStamp;

    nativeErrorTypeHelp Help_type;
    ZoomSDKLiveTranscriptionOperationType messageType = messageInfo.messageType;
    ZNSDKLiveTranscriptionOperationType zn_messageType = Help_type.ZNSDKLiveTranscriptionOperationTypeMap(messageType);
    zn_message_info.messageType = zn_messageType;

    _g_native_wrap.GetMeetingServiceWrap().GetMeetingCloseCaptionCtrl().onOriginalLanguageMsgReceived(zn_message_info);
}
@end

@implementation meetingServicePollingDelegate
- (void)onPollingStatusChanged:(NSString*)pollingID status:(ZoomSDKPollingStatus)status
{
    ZoomSTRING zn_pollingID = "";
    if (pollingID)
        zn_pollingID = [pollingID UTF8String];

    nativeErrorTypeHelp help;
    ZNSDKPollingStatus zn_status = help.ZNSDKPollingStatusMap(status);
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingPollingCtrl().onPollingStatusChanged(zn_pollingID, zn_status);
}
- (void)onPollingResultUpdated:(NSString *)pollingID
{
    ZoomSTRING zn_pollingID = "";
    if (pollingID)
        zn_pollingID = [pollingID UTF8String];
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingPollingCtrl().onPollingResultUpdated(zn_pollingID);
}
- (void)onPollingListUpdated
{
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingPollingCtrl().onPollingListUpdated();
}
- (void)onPollingActionResult:(ZoomSDKPollingActionType)actionType pollingID:(NSString*)pollingID bSuccess:(BOOL)bSuccess errorMsg:(NSString*_Nullable)errorMsg
{
    nativeErrorTypeHelp help;
    ZNSDKPollingActionType zn_actionType = help.ZNSDKPollingActionTypeMap(actionType);

    ZoomSTRING zn_pollingID = "";
    if (pollingID)
        zn_pollingID = [pollingID UTF8String];

    ZoomSTRING zn_errorMsg = "";
    if (errorMsg)
        zn_errorMsg = [errorMsg UTF8String];

    _g_native_wrap.GetMeetingServiceWrap().GetMeetingPollingCtrl().onPollingActionResult(zn_actionType, zn_pollingID, bSuccess, zn_errorMsg);
}
@end

@implementation meetingServiceWhiteboardControllerDelegate

-(void)onWhiteboardStatusChanged:(ZoomSDKWhiteboardStatus)status
{
    nativeErrorTypeHelp help_type;
    ZNSDKWhiteboardStatus zn_status = help_type.ZNSDKWhiteboardStatusMap(status);
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingWhiteboardCtrl().onWhiteboardStatusChanged(zn_status);
}

-(void)onWhiteboardSettingsChanged:(ZoomSDKWhiteboardShareOption)shareOption createOption:(ZoomSDKWhiteboardCreateOption)createOption enable:(BOOL)bEnable
{
    nativeErrorTypeHelp help_type;
    ZNSDKWhiteboardShareOption zn_shareOption = help_type.ZNSDKWhiteboardShareOptionMap(shareOption);
    ZNSDKWhiteboardCreateOption zn_createOption = help_type.ZNSDKWhiteboardCreateOptionMap(createOption);
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingWhiteboardCtrl().onWhiteboardSettingsChanged(zn_shareOption, zn_createOption, bEnable);
}
@end

@implementation meetingServiceMeetingDocsControllerDelegate

- (void)onDocsSharingSourceInfoChanged:(ZoomSDKDocsSharingSourceInfo *)shareSourceInfo;
{
    nativeErrorTypeHelp help_type;
    ZNZoomSDKDocSharingSourceInfo zn_info = help_type.ConvertSDKDocsSharingSourceInfoToZNDocsSharingSourceInfo(shareSourceInfo);
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingDocsCtrl().onDocsSharingSourceInfoChanged(zn_info);
}

- (void)onDocsPermissionChanged:(ZoomSDKDocsCreateOption)createOption shareOption:(ZoomSDKDocsShareOption)shareOption;
{
    nativeErrorTypeHelp help_type;
    ZNSDKDocsCreateOption zn_createOption = help_type.ZNSDKDocsCreateOptionMap(createOption);
    ZNSDKDocsShareOption zn_shareOption = help_type.ZNSDKDocsShareOptionMap(shareOption);
    _g_native_wrap.GetMeetingServiceWrap().GetMeetingDocsCtrl().onDocsPermissionChanged(zn_createOption, zn_shareOption);
}
@end