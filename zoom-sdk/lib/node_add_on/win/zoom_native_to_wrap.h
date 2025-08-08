#ifndef _zoom_native_to_wrap_h_
#define _zoom_native_to_wrap_h_
#include "zoom_native_sdk_wrap_core_def.h"
#include "sdk_wrap.h"
ZNSDKError Map2WrapDefine(ZOOM_SDK_NAMESPACE::SDKError err);
ZNAuthResult Map2WrapDefine(ZOOM_SDK_NAMESPACE::AuthResult result);
ZNLOGINSTATUS Map2WrapDefine(ZOOM_SDK_NAMESPACE::LOGINSTATUS status);
ZNAccountInfo Map2WrapDefine(ZOOM_SDK_NAMESPACE::IAccountInfo* pAccountInfo);
ZNMeetingStatus Map2WrapDefine(ZOOM_SDK_NAMESPACE::MeetingStatus status);
ZNMeetingType Map2WrapDefine(ZOOM_SDK_NAMESPACE::MeetingType type);
ZNConnectionQuality Map2WrapDefine(ZOOM_SDK_NAMESPACE::ConnectionQuality quality);
ZNAudioStatus Map2WrapDefine(ZOOM_SDK_NAMESPACE::AudioStatus status);
ZNVideoStatus Map2WrapDefine(ZOOM_SDK_NAMESPACE::VideoStatus status);
ZNH323CalloutStatus Map2WrapDefine(ZOOM_SDK_NAMESPACE::H323CalloutStatus status);
ZNH323DeviceType Map2WrapDefine(ZOOM_SDK_NAMESPACE::H323DeviceType type);
ZNFreeMeetingNeedUpgradeType Map2WrapDefine(ZOOM_SDK_NAMESPACE::IMeetingConfigurationFreeMeetingEvent::FreeMeetingNeedUpgradeType type);
ZNDirectShareStatus Map2WrapDefine(ZOOM_SDK_NAMESPACE::DirectShareStatus status);
ZNShareStatus Map2WrapDefine(ZOOM_SDK_NAMESPACE::SharingStatus status);
ZNShareType Map2WrapDefine(ZOOM_SDK_NAMESPACE::ShareType type);
ZNAudioType Map2WrapDefine(ZOOM_SDK_NAMESPACE::AudioType type);
ZNUserRole Map2WrapDefine(ZOOM_SDK_NAMESPACE::UserRole role);
ZNSMSVerificationCodeErr Map2WrapDefine(SMSVerificationCodeErr err);
ZNLoginFailReason Map2WrapDefine(ZOOM_SDK_NAMESPACE::LoginFailReason reason);
ZNLiveStreamStatus Map2WrapDefine(ZOOM_SDK_NAMESPACE::LiveStreamStatus status);
ZNMeetingReminderType Map2WrapDefine(ZOOM_SDK_NAMESPACE::MeetingReminderType type);
ZNSDKChatMessageType Map2WrapDefine(ZOOM_SDK_NAMESPACE::SDKChatMessageType type);
ZNSDKChatMessageDeleteType Map2WrapDefine(ZOOM_SDK_NAMESPACE::SDKChatMessageDeleteType type);
ZNSDKFileTransferStatus Map2WrapDefine(ZOOM_SDK_NAMESPACE::SDKFileTransferStatus status);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
ZNSettingsNetWorkType Map2WrapDefine(ZOOM_SDK_NAMESPACE::SettingsNetWorkType role);
ZNSettingConnectionType Map2WrapDefine(ZOOM_SDK_NAMESPACE::SettingConnectionType role);
ZNRequiredInfoType Map2WrapDefine(ZOOM_SDK_NAMESPACE::IMeetingPasswordAndScreenNameHandler::RequiredInfoType type);
ZNWebinarNeedRegisterType Map2WrapDefine(ZOOM_SDK_NAMESPACE::IWebinarNeedRegisterHandler::WebinarNeedRegisterType type);
ZNAudioCallbackActionInfo Map2WrapDefine(ZOOM_SDK_NAMESPACE::AudioCallbackActionInfo info);
ZNRecordingStatus Map2WrapDefine(ZOOM_SDK_NAMESPACE::RecordingStatus info);
ZNRequestStartCloudRecordingStatus Map2WrapDefine(ZOOM_SDK_NAMESPACE::RequestStartCloudRecordingStatus status);
ZN_SDK_ECHO_CANCELLATION_LEVEL Map2WrapDefine(ZOOM_SDK_NAMESPACE::SDK_ECHO_CANCELLATION_LEVEL level);
ZNStatisticsWarningType Map2WrapDefine(ZOOM_SDK_NAMESPACE::StatisticsWarningType type);
ZNSDKEmojiFeedbackType Map2WrapDefine(ZOOM_SDK_NAMESPACE::SDKEmojiFeedbackType type);
ZNWaitingRoomLayoutType Map2WrapDefine(ZOOM_SDK_NAMESPACE::WaitingRoomLayoutType type);
ZNCustomWaitingRoomDataStatus Map2WrapDefine(ZOOM_SDK_NAMESPACE::CustomWaitingRoomDataStatus status);
ZNSDKPanelistChatPrivilege Map2WrapDefine(ZOOM_SDK_NAMESPACE::SDKPanelistChatPrivilege privilege);
ZNSDKAttendeeViewMode Map2WrapDefine(ZOOM_SDK_NAMESPACE::SDKAttendeeViewMode mode);
ZNZoomSDKWallpaperLayoutMode Map2WrapDefine(ZOOM_SDK_NAMESPACE::ZoomSDKWallpaperLayoutMode mode);
ZNZoomSDKWallpaperSettingStatus Map2WrapDefine(ZOOM_SDK_NAMESPACE::ZoomSDKWallpaperSettingStatus status);
ZNVBVideoError Map2WrapDefine(ZOOM_SDK_NAMESPACE::VBVideoError error);
ZNMeetingAICompanionQuerySettingOptions Map2WrapDefine(ZOOM_SDK_NAMESPACE::MeetingAICompanionQuerySettingOptions option);
ZNMeetingAICompanionQueryRequestError Map2WrapDefine(ZOOM_SDK_NAMESPACE::MeetingAICompanionQueryRequestError error);
ZNAICompanionFeature Map2WrapDefine(ZOOM_SDK_NAMESPACE::AICompanionFeature feature);
ZNSDKPollingType Map2WrapDefine(ZOOM_SDK_NAMESPACE::SDKPollingType type);
ZNSDKPollingStatus Map2WrapDefine(ZOOM_SDK_NAMESPACE::SDKPollingStatus status);
ZNSDKPollingQuestionType Map2WrapDefine(ZOOM_SDK_NAMESPACE::SDKPollingQuestionType type);
ZNSDKLiveTranscriptionOperationType Map2WrapDefine(ZOOM_SDK_NAMESPACE::SDKLiveTranscriptionOperationType messageType);
ZNCannotShareReasonType Map2WrapDefine(ZOOM_SDK_NAMESPACE::CannotShareReasonType reason);
ZNSDKWhiteboardShareOption Map2WrapDefine(ZOOM_SDK_NAMESPACE::SDKWhiteboardShareOption option);
ZNSDKWhiteboardCreateOption Map2WrapDefine(ZOOM_SDK_NAMESPACE::SDKWhiteboardCreateOption option);
ZNSDKWhiteboardStatus Map2WrapDefine(ZOOM_SDK_NAMESPACE::SDKWhiteboardStatus option);
ZNSDKDocsStatus Map2WrapDefine(ZOOM_SDK_NAMESPACE::SDKDocsStatus status);
ZNSDKDocsShareOption Map2WrapDefine(ZOOM_SDK_NAMESPACE::SDKDocsShareOption option);
ZNSDKDocsCreateOption Map2WrapDefine(ZOOM_SDK_NAMESPACE::SDKDocsCreateOption option);

ZNUserInfomation ConvertSDKUserInfoToZnUserInfo(ZOOM_SDK_NAMESPACE::IUserInfo* userInfo);
ZNWallpaperItem ConvertSDKIWallpaperItemToZNWallpaperItem(ZOOM_SDK_NAMESPACE::IWallpaperItem* wallpaperItem);
ZNVirtualBGImageInfo ConvertSDKVirtualBGImageInfoToZNVirtualBGImageInfo(ZOOM_SDK_NAMESPACE::IVirtualBGImageInfo* imageinfo);
ZNZoomSDKSharingSourceInfo ConvertSDKSharingSourceInfoToZNSharingSourceInfo(ZOOM_SDK_NAMESPACE::ZoomSDKSharingSourceInfo shareInfo);
ZNChatMsgInfo ConvertSDKChatMsgInfoToZNChatMsgInfo(ZOOM_SDK_NAMESPACE::IChatMsgInfo* chatMsg);
ZNSDKPollingItem ConvertSDKIPollingItemToZNSDKPollingItem(ZOOM_SDK_NAMESPACE::IPollingItem* pollItem);
ZNSDKPollingAnswerItem ConvertSDKIPollingAnswerItemToZNSDKPollingAnswerItem(ZOOM_SDK_NAMESPACE::IPollingAnswerItem* answerItem);
ZNSDKPollingAnswerResultItem ConvertSDKIPollingAnswerResultItemToZNSDKPollingAnswerResultItem(ZOOM_SDK_NAMESPACE::IPollingAnswerResultItem* answerResultItem);
ZNSDKPollingQuestionItem ConvertSDKIPollingQuestionItemToZNSDKPollingQuestionItem(ZOOM_SDK_NAMESPACE::IPollingQuestionItem* questionItem);
ZNLiveTranscriptionMessageInfo ConvertSDKILiveTranscriptionMessageInfoToZNLiveTranscriptionMessageInfo(ZOOM_SDK_NAMESPACE::ILiveTranscriptionMessageInfo* messageInfo);
ZNZoomSDKDocSharingSourceInfo ConvertSDKZoomSDKDocSharingSourceInfoToZNZoomSDKDocSharingSourceInfo(ZOOM_SDK_NAMESPACE::IZoomSDKDocSharingSourceInfo* sourceinfo);

//convert js to sdk
ZOOM_SDK_NAMESPACE::SDK_LANGUAGE_ID Map2SDKDefine(ZNSDK_LANGUAGE_ID id);
ZOOM_SDK_NAMESPACE::SDKUserType Map2SDKDefine(ZNSDKUserType type);
ZOOM_SDK_NAMESPACE::LeaveMeetingCmd Map2SDKDefine(ZNLeaveMeetingCmd cmd);
ZOOM_SDK_NAMESPACE::ZoomUserType Map2SDKDefine(ZNZoomUserType type);
ZOOM_SDK_NAMESPACE::SDKFloatVideoType Map2SDKDefine(ZNSDKFloatVideoType type);
ZOOM_SDK_NAMESPACE::SDKViewType Map2SDKDefine(ZNSDKViewType type);
ZOOM_SDK_NAMESPACE::AnnotationToolType Map2SDKDefine(ZNAnnotationToolType type);
ZOOM_SDK_NAMESPACE::AnnotationClearType Map2SDKDefine(ZNAnnotationClearType type);
ZOOM_SDK_NAMESPACE::SettingTabPage Map2SDKDefine(ZNSettingTabPage tabPage);
ZOOM_SDK_NAMESPACE::SDKCustomizedStringType Map2SDKDefine(ZNSDKCustomizedStringType type);
ZOOM_SDK_NAMESPACE::SDKCustomizedURLType Map2SDKDefine(ZNSDKCustomizedURLType type);
ZOOM_SDK_NAMESPACE::H323DeviceType Map2SDKDefine(ZNH323DeviceType type);
ZOOM_SDK_NAMESPACE::SDKMinimizeUIMode Map2SDKDefine(ZNSDKMinimizeUIMode mode);
ZOOM_SDK_NAMESPACE::SDK_APP_Locale Map2SDKDefine(ZNSDK_APP_Locale mode);
ZOOM_SDK_NAMESPACE::CustomizedLanguageType Map2SDKDefine(ZNCustomizedLanguageType customizedLanguageType);

ZOOM_SDK_NAMESPACE::ZoomSDKVideoRenderMode Map2SDKDefine(ZNZoomSDKVideoRenderMode videoRenderType);
ZOOM_SDK_NAMESPACE::ZoomSDKRawDataMemoryMode Map2SDKDefine(ZNSDKRawDataMemoryMode rawdataMemoryMode);
ZOOM_SDK_NAMESPACE::ZoomSDKRawDataType Map2SDKDefine(ZNZoomSDKRawDataType type);
ZOOM_SDK_NAMESPACE::ZoomSDKResolution Map2SDKDefine(ZNZoomSDKResolution resolution);
ZOOM_SDK_NAMESPACE::VIDEO_HARDWARE_ENCODE_TYPE Map2SDKDefine(ZN_VIDEO_HARDWARE_ENCODE_TYPE encodeType);
ZOOM_SDK_NAMESPACE::ZoomSDKRenderPostProcessing Map2SDKDefine(ZNZoomSDKRenderPostProcessing postProcessing);
ZOOM_SDK_NAMESPACE::ZoomSDKVideoCaptureMethod Map2SDKDefine(ZNZoomSDKVideoCaptureMethod method);
ZOOM_SDK_NAMESPACE::SDK_ECHO_CANCELLATION_LEVEL Map2SDKDefine(ZN_SDK_ECHO_CANCELLATION_LEVEL level);
ZOOM_SDK_NAMESPACE::SDKChatPrivilege Map2WrapDefine(ZNSDKChatPrivilege privilege);
ZOOM_SDK_NAMESPACE::SDKChatMessageType Map2WrapDefine(ZNSDKChatMessageType type);
ZOOM_SDK_NAMESPACE::FeatureEnableOption Map2WrapDefine(ZNFeatureEnableOption option);
ZOOM_SDK_NAMESPACE::SDKPanelistChatPrivilege Map2WrapDefine(ZNSDKPanelistChatPrivilege privilege);
ZOOM_SDK_NAMESPACE::SDKAttendeeViewMode Map2WrapDefine(ZNSDKAttendeeViewMode mode);
ZOOM_SDK_NAMESPACE::ZoomSDKWallpaperLayoutMode Map2SDKDefine(ZNZoomSDKWallpaperLayoutMode mode);
ZOOM_SDK_NAMESPACE::MeetingAICompanionQuerySettingOptions Map2SDKDefine(ZNMeetingAICompanionQuerySettingOptions option);
ZOOM_SDK_NAMESPACE::MeetingAICompanionQueryFeedbackType Map2SDKDefine(ZNMeetingAICompanionQueryFeedbackType type);
ZOOM_SDK_NAMESPACE::SDKVideoPreferenceMode Map2SDKDefine(ZNSDKVideoPreferenceMode mode);
ZOOM_SDK_NAMESPACE::SDKWhiteboardShareOption Map2SDKDefine(ZNSDKWhiteboardShareOption option);
ZOOM_SDK_NAMESPACE::SDKWhiteboardCreateOption Map2SDKDefine(ZNSDKWhiteboardCreateOption option);
ZOOM_SDK_NAMESPACE::SDKDocsStatus Map2SDKDefine(ZNSDKDocsStatus status);
ZOOM_SDK_NAMESPACE::SDKDocsShareOption Map2SDKDefine(ZNSDKDocsShareOption option);
ZOOM_SDK_NAMESPACE::SDKDocsCreateOption Map2SDKDefine(ZNSDKDocsCreateOption option);
//convert zoomstring to string
std::string wStr2Str(const ZoomSTRING& s);
//convert string to zoomstring
ZoomSTRING Str2WStr(const std::string& s);
#endif // !_zoom_native_to_wrap_h_
