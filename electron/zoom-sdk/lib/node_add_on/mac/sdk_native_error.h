
#ifdef  __OBJC__
#import "Header_include.h"
#endif
#include "zoom_native_sdk_wrap_core_def.h"

class nativeErrorTypeHelp {
    
public:
    nativeErrorTypeHelp();
    ~nativeErrorTypeHelp();
    ZNSDKError ZoomSDKErrorType(ZoomSDKError &error);
    ZNAuthResult ZoomSDKAuthErrorTpye(ZoomSDKAuthError &error);
    ZNLOGINSTATUS ZoomSDKLoginStatusType(ZoomSDKLoginStatus &status);
    
    ZNSDKUserType ZoomSDKUserTypeChange(ZoomSDKUserType &type);
    ZoomSDKUserType ZNSDKUserTypeChange(ZNSDKUserType &type);
    SDKUserType  SDKUserTypeChange(ZNZoomUserType &type);
    LeaveMeetingCmd  ZoomSDKLeaveMeetingCmd(ZNLeaveMeetingCmd &cmd);
    
    NSString *ZNSDKLanaguageChange(ZNSDK_LANGUAGE_ID &lanaguageid);
    ZNMeetingStatus  ZNSDKMeetingStatus(ZoomSDKMeetingStatus &status);
    ZoomSDKMeetingStatus ZoomSDKMeetingStatusC(ZNMeetingStatus  &status);
    
    ZNMeetingType ZNSDKMeetingType(MeetingType &type);
    ZNConnectionQuality  ZNSDKConnectionQuatity(ZoomSDKConnectionQuality &quatity);
    ZNSDKViewType  ZNSDKViewTypeC(ScreenType &type);
    ScreenType  ZoomSDKScreenViewType(ZNSDKViewType &type);
    AnnotationToolType  ZoomSDKAnnotationToolType(ZNAnnotationToolType &type);
    AnnotationClearType  ZoomSDKAnnotationClearType(ZNAnnotationClearType &type);
    ZoomSDKMeetingFeatureEnableOption FeatureEnableOptionMap(ZNFeatureEnableOption &option);
    
    ZNAudioStatus  ZNSDKUserAudioStatus(ZoomSDKAudioStatus &status);
    H323DeviceType ZoomSDKH323DeviceType(ZNH323DeviceType &type);
    ZNH323DeviceType ZNSDKH323DeviceType(H323DeviceType &type);
    
    ZNH323CalloutStatus ZoomSDKH323Status(H323CalloutStatus &status);

    ZNFreeMeetingNeedUpgradeType  ZoomSDKFreeMeetingNeedUpgradeType(FreeMeetingNeedUpgradeType &type);
    ZNPremeetingAPIResult  ZNSDKPreMeetingError(ZoomSDKPremeetingError &error);
    ZNDirectShareStatus   ZNSDKDirectShareStatus(DirectShareStatus &status);
    ZNShareStatus  ZNSDKShareStatus(ZoomSDKShareStatus &status);
    ZNShareType ZNSDKShareType(ZoomSDKShareContentType &type);
    ZNUserRole ZNSDKUserRole(UserRole &role);
    ZNMeetingFailCode  ZNSDKMeetingFailCode(ZoomSDKMeetingError &error);
    ZNMeetingEndReason  ZNSDKMeetingEndReason(EndMeetingReason &reason);
    ZoomSDKLocale ZNSDKAPPLocalType(ZNSDK_APP_Locale &local);
    ZNSMSVerificationCodeErr ZNSDKSMSVerificationCodeError(ZoomSDKSMSVerificationCodeError &code);
    ZNAudioCallbackActionInfo ZNSDKAudioCallbackAction(ZoomSDKAudioActionInfo &info);
    ZNWebinarNeedRegisterType ZNSDKWebinarNeedRegisterType(WebinarRegisterType &type);
    ZNRequiredInfoType ZNSDKRequiredInfoType(JoinMeetingReqInfoType &type);
    ZNRecordingStatus ZNRecordingStatusChange(ZoomSDKRecordingStatus &status);
    ZNRequestStartCloudRecordingStatus ZNRequestStartCloudRecordingStatusMap(ZoomSDKRequestStartCloudRecordingStatus &status);
    ZNLoginFailReason ZNLoginFailReasonMap(ZoomSDKLoginFailReason &reason);
    ZNStatisticsWarningType ZNStatisticsWarningTypeMap(StatisticWarningType &type);
    ZNAudioShareMode ZNAudioShareModeMap(ZoomSDKAudioShareMode &mode);
    ZoomSDKAudioShareMode ZoomSDKAudioShareModeMap(ZNAudioShareMode &mode);
    ZNLiveStreamStatus ZNLiveStreamStatusMap(LiveStreamStatus &status);
    ZNMeetingReminderType ZNReminderTypeMap(ZoomSDKReminderType &type);

    ZoomSDKChatPrivilegeType ZoomSDKChatPrivilegeTypeMap(ZNSDKChatPrivilege &privilege);
    ZoomSDKChatMessageType ZoomSDKChatMessageTypeMap(ZNSDKChatMessageType &type);
    ZNSDKChatMessageType ZNSDKChatMessageTypeMap(ZoomSDKChatMessageType &type);
    ZNSDKChatMessageDeleteType ZNSDKChatMessageDeleteTypeMap(ZoomSDKChatMessageDeleteType &type);
    ZNSDKFileTransferStatus ZNSDKFileTransferStatusMap(ZoomSDKFileTransferStatus &status);

    ZNAudioType ZNAudioTypeMap(ZoomSDKAudioType &type);
    ZNSDKEmojiFeedbackType ZNSDKEmojiFeedbackTypeMap(ZoomSDKEmojiFeedbackType &type);
    ZNWaitingRoomLayoutType ZNWaitingRoomLayoutTypeMap(ZoomSDKWaitingRoomLayoutType &type);
    ZNCustomWaitingRoomDataStatus ZNCustomWaitingRoomDataStatusMap(ZoomSDKCustomWaitingRoomDataStatus &status);

    ZoomSDKPanelistChatPrivilege ZoomSDKPanelistChatPrivilegeMap(ZNSDKPanelistChatPrivilege &privilege);
    ZNSDKPanelistChatPrivilege ZNSDKPanelistChatPrivilegeMap(ZoomSDKPanelistChatPrivilege &privilege);
    ZoomSDKAttendeeViewMode ZoomSDKAttendeeViewModeMap(ZNSDKAttendeeViewMode &mode);
    ZNSDKAttendeeViewMode ZNSDKAttendeeViewModeMap(ZoomSDKAttendeeViewMode &mode);
    ZNZoomSDKWallpaperLayoutMode ZNSDKWallpaperLayoutModeMap(ZoomSDKWallpaperLayoutMode &mode);
    ZoomSDKWallpaperLayoutMode ZoomSDKWallpaperLayoutModeMap(ZNZoomSDKWallpaperLayoutMode &zn_mode);
    ZNZoomSDKWallpaperSettingStatus ZNSDKWallpaperSettingStatusMap(ZoomSDKWallpaperSettingStatus &status);
    ZNVBVideoError ZNVBVideoErrorMap(ZoomSDKSettingVBVideoError &error);
    ZNMeetingAICompanionQuerySettingOptions ZNMeetingAICompanionQuerySettingOptionsMap(ZoomSDKAICompanionQuerySettingOptions &option);
    ZoomSDKAICompanionQuerySettingOptions ZoomSDKMeetingAICompanionQuerySettingOptionsMap(ZNMeetingAICompanionQuerySettingOptions &zn_option);
    ZNMeetingAICompanionQueryRequestError ZNMeetingAICompanionQueryRequestErrorMap(ZoomSDKAICompanionQueryRequestError &error);
    ZoomSDKAICompanionQueryFeedbackType ZoomSDKAICompanionQueryFeedbackTypeMap(ZNMeetingAICompanionQueryFeedbackType &zn_type);
    ZoomSDKVideoPreferenceMode SDKVideoPreferenceModeMap(ZNSDKVideoPreferenceMode &zn_mode);
    ZNAICompanionFeature ZNAICompanionFeatureMap(ZoomSDKAICompanionFeature &feature);
    ZNSDKEmojiReactionType ZNSDKEmojiReactionTypeMap(ZoomSDKEmojiReactionType &type);
    ZNSDKEmojiReactionSkinTone ZNSDKEmojiReactionSkinToneMap(ZoomSDKEmojiReactionSkinTone &skinTone);
    ZNSDKLiveTranscriptionOperationType ZNSDKLiveTranscriptionOperationTypeMap(ZoomSDKLiveTranscriptionOperationType &messageType);
    ZNSDKPollingStatus ZNSDKPollingStatusMap(ZoomSDKPollingStatus &status);
    ZNSDKPollingType ZNSDKPollingTypeMap(ZoomSDKPollingType &pollingType);
    ZNSDKPollingQuestionType ZNSDKPollingQuestionTypeMap(ZoomSDKPollingQuestionType &questionType);
    ZNSDKPollingActionType ZNSDKPollingActionTypeMap(ZoomSDKPollingActionType actionType);
    ZNCannotShareReasonType ZNCannotShareReasonTypeMap(ZoomSDKCannotShareReasonType &reason);
    ZoomSDKCannotShareReasonType ZoomSDKCannotShareReasonTypeMap(ZNCannotShareReasonType &zn_reason);
    ZNSDKWhiteboardStatus ZNSDKWhiteboardStatusMap(ZoomSDKWhiteboardStatus &status);
    ZNSDKWhiteboardShareOption ZNSDKWhiteboardShareOptionMap(ZoomSDKWhiteboardShareOption &option);
    ZoomSDKWhiteboardShareOption ZoomSDKWhiteboardShareOptionMap(ZNSDKWhiteboardShareOption &zn_option);
    ZNSDKWhiteboardCreateOption ZNSDKWhiteboardCreateOptionMap(ZoomSDKWhiteboardCreateOption &option);
    ZoomSDKWhiteboardCreateOption ZoomSDKWhiteboardCreateOptionMap(ZNSDKWhiteboardCreateOption &zn_option);
    ZNSDKDocsStatus ZNSDKDocsStatusMap(ZoomSDKDocsStatus &option);
    ZoomSDKDocsStatus ZoomSDKDocsStatusMap(ZNSDKDocsStatus &zn_option);
    ZNSDKDocsShareOption ZNSDKDocsShareOptionMap(ZoomSDKDocsShareOption &option);
    ZoomSDKDocsShareOption ZoomSDKDocsShareOptionMap(ZNSDKDocsShareOption &zn_option);
    ZNSDKDocsCreateOption ZNSDKDocsCreateOptionMap(ZoomSDKDocsCreateOption &option);
    ZoomSDKDocsCreateOption ZoomSDKDocsCreateOptionMap(ZNSDKDocsCreateOption &zn_option);

    ZNUserInfomation ConvertSDKUserInfoToZnUserInfo(ZoomSDKUserInfo *userInfo);
    ZNWallpaperItem ConvertSDKIWallpaperItemToZNWallpaperItem(ZoomSDKWallpaperItem *wallpaperItem);
    ZNZoomSDKSharingSourceInfo ConvertSDKSharingSourceInfoToZNSharingSourceInfo(ZoomSDKSharingSourceInfo *shareInfo);
    ZNChatMsgInfo ConvertSDKChatMsgInfoToZNChatMsgInfo(ZoomSDKChatInfo *chatInfo);
    ZNVirtualBGImageInfo ConvertSDKVirtualBGImageInfoToZNVirtualBGImageInfo(ZoomSDKVirtualBGImageInfo *imageInfo);
    ZNZoomSDKDocSharingSourceInfo ConvertSDKDocsSharingSourceInfoToZNDocsSharingSourceInfo(ZoomSDKDocsSharingSourceInfo *sourceInfo);
};