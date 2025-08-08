


#include "sdk_native_error.h"

nativeErrorTypeHelp::nativeErrorTypeHelp()
{
    
}
nativeErrorTypeHelp::~nativeErrorTypeHelp()
{
    
}
#pragma mark  zoom error
ZNSDKError nativeErrorTypeHelp::ZoomSDKErrorType(ZoomSDKError &error)
{
    ZNSDKError ret = ZNSDKERR_UNKNOWN;
    switch (error) {
        case ZoomSDKError_Success:
            ret = ZNSDKERR_SUCCESS;
            break;
        case ZoomSDKError_Failed:
            ret = ZNSDKERR_MEMORY_FAILED;
            break;
        case ZoomSDKError_UnKnown:
            ret = ZNSDKERR_UNKNOWN;
            break;
        case ZoomSDKError_Uninit:
            ret = ZNSDKERR_UNINITIALIZE;
            break;
        case ZoomSDKError_ServiceFailed:
            ret = ZNSDKERR_SERVICE_FAILED;
            break;
        case ZoomSDKError_WrongUsage:
            ret = ZNSDKERR_WRONG_USEAGE;
            break;
        case ZoomSDKError_InvalidParameter:
            ret = ZNSDKERR_INVALID_PARAMETER;
            break;
        case ZoomSDKError_NoPermission:
            ret = ZNSDKERR_NO_PERMISSION;
            break;
        case ZoomSDKError_NoRecordingInProgress:
            ret = ZNSDKERR_NORECORDINGINPROCESS;
            break;
        case ZoomSDKError_TooFrequentCall:
            ret = ZNSDKERR_TOO_FREQUENT_CALL;
            break;     
        case ZoomSDKError_ModuleLoadFail:
            ret = ZNSDKERR_MODULE_LOAD_FAILED;
            break;
        case ZoomSDKError_NoVideoDeviceFound:
            ret = ZNSDKERR_NO_VIDEODEVICE_ISFOUND;
            break;
        case ZoomSDKError_NeedUserConfirmRecordDisclaimer:
            ret = ZNSDKERR_NEED_USER_CONFIRM_RECORD_DISCLAIMER;
            break;
        case ZoomSDKError_NotInMeeting:
            ret = ZNSDKERR_NOT_IN_MEETING;
            break;
        case ZoomSDKError_NotJoinAudio:
            ret = ZNSDKERR_NOT_JOIN_AUDIO;
            break;
        case ZoomSDKError_HardwareDontSupport:
            ret = ZNSDKERR_HARDWARE_DONT_SUPPORT;
            break;
        case ZoomSDKError_DomainDontSupport:
            ret = ZNSDKERR_DOMAIN_DONT_SUPPORT;
            break;
        case ZoomSDKError_FileTransferError:
            ret = ZNSDKERR_FILETRANSFER_ERROR;
            break;
        default:
            break;
    }
    return ret;
}

#pragma mark  auth error
ZNAuthResult nativeErrorTypeHelp::ZoomSDKAuthErrorTpye(ZoomSDKAuthError &error)
{
    ZNAuthResult ret = ZNAUTHRET_UNKNOWN;
    switch (error) {
        case ZoomSDKAuthError_Success:
            ret = ZNAUTHRET_SUCCESS;
            break;
        case ZoomSDKAuthError_KeyOrSecretWrong:
            ret = ZNAUTHRET_KEYORSECRETWRONG;
            break;
        case ZoomSDKAuthError_AccountNotSupport:
            ret = ZNAUTHRET_ACCOUNTNOTSUPPORT;
            break;
        case ZoomSDKAuthError_AccountNotEnableSDK:
            ret = ZNAUTHRET_ACCOUNTNOTENABLESDK;
            break;
        case ZoomSDKAuthError_Unknown:
            ret = ZNAUTHRET_UNKNOWN;
            break;
        case ZoomSDKAuthError_Timeout:
            ret = ZNAUTHRET_OVERTIME;
            break;
        case ZoomSDKAuthError_NetworkIssue:
            ret = ZNAUTHRET_NETWORKISSUE;
            break;
        case ZoomSDKAuthError_Client_Incompatible:
            ret = ZNAUTHRET_CLIENT_INCOMPATIBLE;
            break;
        case ZoomSDKAuthError_JwtTokenWrong:
            ret = ZNAUTHRET_JWTTOKENWRONG;
            break;
        case ZoomSDKAuthError_KeyOrSecretEmpty:
            ret = ZNAUTHRET_KEYORSECRETEMPTY;
            break;
        case ZoomSDKAuthError_LimitExceededException:
            ret = ZNAUTHRET_LIMIT_EXCEEDED_EXCEPTION;
            break;
        default:
            break;
    }
    return ret;
}

#pragma mark  login status
ZNLOGINSTATUS nativeErrorTypeHelp::ZoomSDKLoginStatusType(ZoomSDKLoginStatus &status)
{
    ZNLOGINSTATUS ret = ZNLOGIN_IDLE;
    switch (status) {
        case ZoomSDKLoginStatus_Idle:
            ret = ZNLOGIN_IDLE;
            break;
        case ZoomSDKLoginStatus_Success:
            ret = ZNLOGIN_SUCCESS;
            break;
        case ZoomSDKLoginStatus_Failed:
            ret = ZNLOGIN_FAILED;
            break;
        case ZoomSDKLoginStatus_Processing:
            ret = ZNLOGIN_PROCESSING;
            break;
        default:
            break;
    }
    return ret;
}
#pragma mark user type
ZNSDKUserType nativeErrorTypeHelp::ZoomSDKUserTypeChange(ZoomSDKUserType &type)
{
    ZNSDKUserType ret = ZNSDK_UT_NORMALUSER;
    switch (type) {
        case ZoomSDKUserType_ZoomUser:
            ret = ZNSDK_UT_NORMALUSER;
            break;
        case ZoomSDKUserType_WithoutLogin:
            ret = ZNSDK_UT_WITHOUT_LOGIN;
            break;
        default:
            break;
    }
    return ret;
}

ZoomSDKUserType nativeErrorTypeHelp::ZNSDKUserTypeChange(ZNSDKUserType &type)
{
    ZoomSDKUserType ret = ZoomSDKUserType_WithoutLogin;
    switch (type) {
        case ZNSDK_UT_WITHOUT_LOGIN:
            ret = ZoomSDKUserType_WithoutLogin;
            break;
        case ZNSDK_UT_NORMALUSER:
            ret = ZoomSDKUserType_ZoomUser;
            break;
        default:
            break;
    }
    return ret;
}

SDKUserType  nativeErrorTypeHelp::SDKUserTypeChange(ZNZoomUserType &type)
{
    SDKUserType ret = SDKUserType_Unknown;
    switch (type) {
        case ZNZoomUserType_APIUSER:
            ret = SDKUserType_APIUser;
            break;
        case ZNZoomUserType_EMAIL_LOGIN:
            ret = SDKUserType_EmailLogin;
            break;
        case ZNZoomUserType_FACEBOOK:
            ret = SDKUserType_FaceBook;
            break;
        case ZNZoomUserType_GoogleOAuth:
            ret = SDKUserType_GoogleOAuth;
            break;
        case ZNZoomUserType_SSO:
            ret = SDKUserType_SSO;
            break;
        case ZNZoomUserType_Unknown:
            ret = SDKUserType_Unknown;
            break;
        default:
            break;
    }
    return ret;
}

LeaveMeetingCmd nativeErrorTypeHelp::ZoomSDKLeaveMeetingCmd(ZNLeaveMeetingCmd &cmd)
{
    LeaveMeetingCmd ret = LeaveMeetingCmd_End;
    switch (cmd) {
        case ZNLEAVE_MEETING:
            ret = LeaveMeetingCmd_Leave;
            break;
        case ZNEND_MEETING:
            ret = LeaveMeetingCmd_End;
            break;
        default:
            break;
    }
    return ret;
}

#pragma mark meeting status
ZNMeetingStatus  nativeErrorTypeHelp::ZNSDKMeetingStatus(ZoomSDKMeetingStatus &status)
{
    ZNMeetingStatus ret = ZNMEETING_STATUS_IDLE;
    switch (status) {
        case ZoomSDKMeetingStatus_Idle:
            ret = ZNMEETING_STATUS_IDLE;
            break;
        case ZoomSDKMeetingStatus_Connecting:
            ret = ZNMEETING_STATUS_CONNECTING;
            break;
        case ZoomSDKMeetingStatus_WaitingForHost:
            ret = ZNMEETING_STATUS_WAITINGFORHOST;
            break;
        case ZoomSDKMeetingStatus_InMeeting:
            ret = ZNMEETING_STATUS_INMEETING;
            break;
        case ZoomSDKMeetingStatus_Disconnecting:
            ret = ZNMEETING_STATUS_DISCONNECTING;
            break;
        case ZoomSDKMeetingStatus_Reconnecting:
            ret = ZNMEETING_STATUS_RECONNECTING;
            break;
        case ZoomSDKMeetingStatus_Failed:
            ret = ZNMEETING_STATUS_FAILED;
            break;
        case ZoomSDKMeetingStatus_Ended:
            ret = ZNMEETING_STATUS_ENDED;
            break;
        case ZoomSDKMeetingStatus_AudioReady:
            ret = ZNMEETING_STATUS_AUDIO_READY;
            break;
        case ZoomSDKMeetingStatus_OtherMeetingInProgress:
            ret = ZNMEETING_STATUS_OTHER_MEETING_INPROGRESS;
            break;
        case ZoomSDKMeetingStatus_InWaitingRoom:
            ret = ZNMEETING_STATUS_IN_WAITING_ROOM;
            break;
        case ZoomSDKMeetingStatus_Webinar_Promote:
            ret = ZNMEETING_STATUS_WEBINAR_PROMOTE;
            break;
        case ZoomSDKMeetingStatus_Webinar_Depromote:
            ret = ZNMEETING_STATUS_WEBINAR_DEPROMOTE;
            break;
        default:
            break;
    }
    return ret;
}

ZoomSDKMeetingStatus nativeErrorTypeHelp::ZoomSDKMeetingStatusC(ZNMeetingStatus &status)
{
    ZoomSDKMeetingStatus ret = ZoomSDKMeetingStatus_Idle;
    switch (status) {
        case ZNMEETING_STATUS_IDLE:
            ret = ZoomSDKMeetingStatus_Idle;
            break;
        case ZNMEETING_STATUS_CONNECTING:
            ret = ZoomSDKMeetingStatus_Connecting;
            break;
        case ZNMEETING_STATUS_WAITINGFORHOST:
            ret = ZoomSDKMeetingStatus_WaitingForHost;
            break;
        case ZNMEETING_STATUS_INMEETING:
            ret = ZoomSDKMeetingStatus_InMeeting;
            break;
        case ZNMEETING_STATUS_DISCONNECTING:
            ret = ZoomSDKMeetingStatus_Disconnecting;
            break;
        case ZNMEETING_STATUS_RECONNECTING:
            ret = ZoomSDKMeetingStatus_Reconnecting;
            break;
        case ZNMEETING_STATUS_FAILED:
            ret = ZoomSDKMeetingStatus_Failed;
            break;
        case ZNMEETING_STATUS_ENDED:
            ret = ZoomSDKMeetingStatus_Ended;
            break;
        case ZNMEETING_STATUS_AUDIO_READY:
            ret = ZoomSDKMeetingStatus_AudioReady;
            break;
        case ZNMEETING_STATUS_OTHER_MEETING_INPROGRESS:
            ret = ZoomSDKMeetingStatus_OtherMeetingInProgress;
            break;
        case ZNMEETING_STATUS_IN_WAITING_ROOM:
            ret = ZoomSDKMeetingStatus_InWaitingRoom;
            break;
        case ZNMEETING_STATUS_WEBINAR_PROMOTE:
            ret = ZoomSDKMeetingStatus_Webinar_Promote;
            break;
        case ZNMEETING_STATUS_WEBINAR_DEPROMOTE:
            ret = ZoomSDKMeetingStatus_Webinar_Depromote;
            break;
        default:
            break;
    }
    return ret;
}
#pragma mark lanaguage
static NSString* const kZMFollowSystemIdentify = @"follow system language";
NSString *nativeErrorTypeHelp::ZNSDKLanaguageChange(ZNSDK_LANGUAGE_ID &lanaguageid)
{
    NSString *ret = kZMFollowSystemIdentify;
    switch (lanaguageid) {
        case ZNLANGUAGE_Unknown:
            ret = kZMFollowSystemIdentify;
            break;
        case ZNLANGUAGE_English:
            ret = @"en";
            break;
        case ZNLANGUAGE_Chinese_Simplified:
            ret = @"zh-Hans";
            break;
        case ZNLANGUAGE_Chinese_Traditional:
            ret = @"zh-Hant";
            break;
        case ZNLANGUAGE_Japanese:
            ret = @"ja";
            break;
        case ZNLANGUAGE_Spanish:
            ret = @"es";
            break;
        case ZNLANGUAGE_German:
            ret = @"de";
            break;
        case ZNLANGUAGE_French:
            ret = @"fr";
            break;
        case ZNLANGUAGE_Portuguese:
            ret = @"pt-PT";
            break;
        case ZNLANGUAGE_Russian:
            ret = @"ru";
            break;
        case ZNLANGUAGE_Korean:
            ret = @"ko-KR";
            break;
        case ZNLANGUAGE_Vietnamese:
            ret = @"vi";
            break;
        case ZNLANGUAGE_Italian:
            ret = @"it";
            break;
        case ZNLANGUAGE_Polish:
            ret = @"pl";
            break;
        case ZNLANGUAGE_Turkish:
            ret = @"tr";
            break;
        case ZNLANGUAGE_Indonesian:
            ret = @"id";
            break;
        case ZNLANGUAGE_Dutch:
            ret = @"nl";
            break;
        default:
            break;
    }
    return ret;
}

ZNMeetingType nativeErrorTypeHelp::ZNSDKMeetingType(MeetingType &type)
{
    ZNMeetingType ret = ZNMEETING_TYPE_NONE;
    switch (type) {
        case MeetingType_None:
            ret = ZNMEETING_TYPE_NONE;
            break;
        case MeetingType_Normal:
            ret = ZNMEETING_TYPE_NORMAL;
            break;
        case MeetingType_BreakoutRoom:
            ret = ZNMEETING_TYPE_BREAKOUTROOM;
            break;
        case MeetingType_Webinar:
            ret = ZNMEETING_TYPE_WEBINAR;
            break;
        default:
            break;
    }
    return ret;
}

ZNConnectionQuality nativeErrorTypeHelp::ZNSDKConnectionQuatity(ZoomSDKConnectionQuality &quatity)
{
    ZNConnectionQuality ret = ZNConn_Quality_Unknown;
    switch (quatity) {
        case ZoomSDKConnectionQuality_Unknown:
            ret = ZNConn_Quality_Unknown;
            break;
        case ZoomSDKConnectionQuality_VeryBad:
            ret = ZNConn_Quality_Very_Bad;
            break;
        case ZoomSDKConnectionQuality_Bad:
            ret = ZNConn_Quality_Bad;
            break;
        case ZoomSDKConnectionQuality_NotGood:
            ret = ZNConn_Quality_Not_Good;
            break;
        case ZoomSDKConnectionQuality_Normal:
            ret = ZNConn_Quality_Normal;
            break;
        case ZoomSDKConnectionQuality_Good:
            ret = ZNConn_Quality_Good;
            break;
        case ZoomSDKConnectionQuality_Excellent:
            ret = ZNConn_Quality_Excellent;
            break;
        default:
            break;
    }
    return ret;
}

#pragma mark screenViewType
ZNSDKViewType nativeErrorTypeHelp::ZNSDKViewTypeC(ScreenType &type)
{
    ZNSDKViewType ret = ZNSDK_FIRST_VIEW;
    switch (type) {
        case ScreenType_First:
            ret = ZNSDK_FIRST_VIEW;
            break;
        case ScreenType_Second:
            ret = ZNSDK_SECOND_VIEW;
        default:
            break;
    }
    return ret;
}

ScreenType nativeErrorTypeHelp::ZoomSDKScreenViewType(ZNSDKViewType &type)
{
    ScreenType ret = ScreenType_First;
    switch (type) {
        case ZNSDK_FIRST_VIEW:
            ret = ScreenType_First;
            break;
        case ZNSDK_SECOND_VIEW:
            ret = ScreenType_Second;
            break;
        default:
            break;
    }
    return ret;
}

AnnotationToolType nativeErrorTypeHelp::ZoomSDKAnnotationToolType(ZNAnnotationToolType &type)
{
    AnnotationToolType ret = AnnotationToolType_None;
    switch (type) {
        case ZN_ANNOTOOL_NONE_DRAWING:
            ret = AnnotationToolType_None;
            break;
        case ZN_ANNOTOOL_PEN:
            ret = AnnotationToolType_Pen;
            break;
        case ZN_ANNOTOOL_HIGHLIGHTER:
            ret = AnnotationToolType_HighLighter;
            break;
        case ZN_ANNOTOOL_AUTO_LINE:
            ret = AnnotationToolType_AutoLine;
            break;
        case ZN_ANNOTOOL_AUTO_RECTANGLE:
            ret = AnnotationToolType_AutoRectangle;
            break;
        case ZN_ANNOTOOL_AUTO_ELLIPSE:
            ret = AnnotationToolType_AutoEllipse;
            break;
        case ZN_ANNOTOOL_AUTO_ARROW:
            ret = AnnotationToolType_AutoArrow;
            break;
        case ZN_ANNOTOOL_AUTO_RECTANGLE_FILL:
            ret = AnnotationToolType_AutoRectangleFill;
            break;
        case ZN_ANNOTOOL_AUTO_ELLIPSE_FILL:
            ret = AnnotationToolType_AutoEllipseFill;
            break;
        case ZN_ANNOTOOL_SPOTLIGHT:
            ret = AnnotationToolType_SpotLight;
            break;
        case ZN_ANNOTOOL_ARROW:
            ret = AnnotationToolType_Arrow;
            break;
        case ZN_ANNOTOOL_ERASER:
            ret = AnnotationToolType_ERASER;
            break;
        default:
            break;
    }
    return ret;
}

AnnotationClearType nativeErrorTypeHelp::ZoomSDKAnnotationClearType(ZNAnnotationClearType &type)
{
    AnnotationClearType ret = AnnotationClearType_Self;
    switch (type) {
        case ZN_ANNOCLEAR_ALL:
            ret =  AnnotationClearType_All;
            break;
        case ZN_ANNOCLEAR_SELF:
            ret = AnnotationClearType_Self;
            break;
        case ZN_ANNOCLEAR_OTHER:
            ret = AnnotationClearType_Other;
            break;
        default:
            break;
    }
    return ret;
}

ZoomSDKMeetingFeatureEnableOption nativeErrorTypeHelp::FeatureEnableOptionMap(ZNFeatureEnableOption &option)
{
    ZoomSDKMeetingFeatureEnableOption meeting_enable_option = ZoomSDKMeetingFeatureEnableOption_None;
    switch (option) {
        case ZNEnableOption_None:
            meeting_enable_option = ZoomSDKMeetingFeatureEnableOption_None;
            break;
        case ZNEnableOption_Once:
            meeting_enable_option = ZoomSDKMeetingFeatureEnableOption_Once;
            break;
        case ZNEnableOption_Always:
            meeting_enable_option = ZoomSDKMeetingFeatureEnableOption_Always;
            break;
        default:
            break;
    }
    return meeting_enable_option;
}


ZNAudioStatus nativeErrorTypeHelp::ZNSDKUserAudioStatus(ZoomSDKAudioStatus &status)
{
    ZNAudioStatus ret = ZN_Audio_None;
    switch (status) {
        case ZoomSDKAudioStatus_None:
            ret = ZN_Audio_None;
            break;
        case ZoomSDKAudioStatus_Muted:
            ret = ZN_Audio_Muted;
            break;
        case ZoomSDKAudioStatus_UnMuted:
            ret = ZN_Audio_UnMuted;
            break;
        case ZoomSDKAudioStatus_MutedByHost:
            ret = ZN_Audio_Muted_ByHost;
            break;
        case ZoomSDKAudioStatus_UnMutedByHost:
            ret = ZN_Audio_UnMuted_ByHost;
            break;
        case ZoomSDKAudioStatus_MutedAllByHost:
            ret = ZN_Audio_MutedAll_ByHost;
            break;
        case ZoomSDKAudioStatus_UnMutedAllByHost:
            ret = ZN_Audio_UnMutedAll_ByHost;
            break;
        default:
            break;
    }
    return ret;
}


H323DeviceType nativeErrorTypeHelp::ZoomSDKH323DeviceType(ZNH323DeviceType &type)
{
    H323DeviceType ret = H323DeviceType_Unknown;
    switch (type) {
        case ZN_H323DeviceType_Unknown:
            ret = H323DeviceType_Unknown;
            break;
        case ZN_H323DeviceType_SIP:
            ret = H323DeviceType_SIP;
            break;
        case ZN_H323DeviceType_H323:
            ret = H323DeviceType_H323;
            break;
            
        default:
            break;
    }
    return ret;
}

ZNH323DeviceType nativeErrorTypeHelp::ZNSDKH323DeviceType(H323DeviceType &type)
{
    ZNH323DeviceType ret = ZN_H323DeviceType_Unknown;
    switch (type) {
        case H323DeviceType_Unknown:
            ret = ZN_H323DeviceType_Unknown;
            break;
        case H323DeviceType_SIP:
            ret = ZN_H323DeviceType_SIP;
            break;
        case H323DeviceType_H323:
            ret = ZN_H323DeviceType_H323;
            break;
        default:
            break;
    }
    return ret;
}

ZNH323CalloutStatus nativeErrorTypeHelp::ZoomSDKH323Status(H323CalloutStatus &status)
{
    ZNH323CalloutStatus ret = ZN_H323Callout_Unknown;
    switch (status) {
        case H323CalloutStatus_Unknown:
            ret = ZN_H323Callout_Unknown;
            break;
        case H323CalloutStatus_Success:
            ret = ZN_H323Callout_Success;
            break;
        case H323CalloutStatus_Ring:
            ret = ZN_H323Callout_Ring;
            break;
        case H323CalloutStatus_Timeout:
            ret = ZN_H323Callout_Timeout;
            break;
        case H323CalloutStatus_Failed:
            ret = ZN_H323Callout_Failed;
            break;
        case H323CalloutStatus_Busy:
            ret = ZN_H323Callout_Busy;
            break;
        case H323CalloutStatus_Decline:
            ret = ZN_H323Callout_Decline;
            break;
        default:
            break;
    }
    return ret;
}



ZNFreeMeetingNeedUpgradeType nativeErrorTypeHelp::ZoomSDKFreeMeetingNeedUpgradeType(FreeMeetingNeedUpgradeType &type)
{
    ZNFreeMeetingNeedUpgradeType ret = ZN_FreeMeetingNeedUpgradeType_NONE;
    switch (type) {
        case FreeMeetingNeedUpgradeType_NONE:
            ret = ZN_FreeMeetingNeedUpgradeType_NONE;
            break;
        case FreeMeetingNeedUpgradeType_BY_ADMIN:
            ret = ZN_FreeMeetingNeedUpgradeType_BY_ADMIN;
            break;
        case FreeMeetingNeedUpgradeType_BY_GIFTURL:
            ret = ZN_FreeMeetingNeedUpgradeType_BY_GIFTURL;
            break;
        default:
            break;
    }
    return ret;
}

ZNPremeetingAPIResult nativeErrorTypeHelp::ZNSDKPreMeetingError(ZoomSDKPremeetingError &error)
{
    ZNPremeetingAPIResult ret = ZN_PREMETAPIRET_UNKNOW;
    switch (error) {
        case ZoomSDKPremeetingError_Success:
            ret = ZN_PREMETAPIRET_SUCCESS;
            break;
        case ZoomSDKPremeetingError_Failed:
            ret = ZN_PREMETAPIRET_FAILED;
            break;
        case ZoomSDKPremeetingError_TimeOut:
            ret = ZN_PREMETAPIRET_TIMEOUT;
            break;
        case ZoomSDKPremeetingError_Unknown:
            ret = ZN_PREMETAPIRET_UNKNOW;
            break;
        default:
            break;
    }
    return ret;
}

ZNDirectShareStatus nativeErrorTypeHelp::ZNSDKDirectShareStatus(DirectShareStatus &status)
{
    ZNDirectShareStatus ret = ZN_DirectShare_Unknown;
    switch (status) {
        case DirectShareStatus_None:
            ret = ZN_DirectShare_Unknown;
            break;
        case DirectShareStatus_Connecting:
            ret = ZN_DirectShare_Connecting;
            break;
        case DirectShareStatus_InProgress:
            ret = ZN_DirectShare_In_Direct_Share_Mode;
            break;
        case DirectShareStatus_Ended:
            ret = ZN_DirectShare_Ended;
            break;
        case DirectShareStatus_NeedMeetingIDOrSharingKey:
            ret = ZN_DirectShare_Need_MeetingID_Or_PairingCode;
            break;
        case DirectShareStatus_WrongMeetingIDOrSharingKey:
            ret = ZN_DirectShare_WrongMeetingID_Or_SharingKey;
            break;
        case DirectShareStatus_NetworkError:
            ret = ZN_DirectShare_NetWork_Error;
            break;
        case DirectShareStatus_Unknown:
            ret = ZN_DirectShare_Unknown;
            break;
        case DirectShareStatus_NeedInputNewPairingCode:
            ret = ZN_DirectShare_InputNewParingCode;
            break;
        case DirectShareStatus_Prepared:
            ret = ZN_DirectShare_Prepared;
            break;

        default:
            break;
    }
    return ret;
}

ZNShareStatus nativeErrorTypeHelp::ZNSDKShareStatus(ZoomSDKShareStatus &status)
{
    ZNShareStatus ret = ZN_Sharing_None;
    switch (status) {
        case ZoomSDKShareStatus_None:
            ret = ZN_Sharing_None;
            break;
        case ZoomSDKShareStatus_SelfBegin:
            ret = ZN_Sharing_Self_Send_Begin;
            break;
        case ZoomSDKShareStatus_SelfEnd:
            ret = ZN_Sharing_Self_Send_End;
            break;
        case ZoomSDKShareStatus_OtherBegin:
            ret = ZN_Sharing_Other_Share_Begin;
            break;
        case ZoomSDKShareStatus_OtherEnd:
            ret = ZN_Sharing_Other_Share_End;
            break;
        case ZoomSDKShareStatus_ViewOther:
            ret = ZN_Sharing_View_Other_Sharing;
            break;
        case ZoomSDKShareStatus_Pause:
            ret = ZN_Sharing_Pause;
            break;
        case ZoomSDKShareStatus_Resume:
            ret = ZN_Sharing_Resume;
            break;
        case ZoomSDKShareStatus_SelfStartAudioShare:
            ret = ZN_Sharing_SelfStartAudioShare;
            break;
        case ZoomSDKShareStatus_SelfStopAudioShare:
            ret = ZN_Sharing_SelfStopAudioShare;
            break;
        case ZoomSDKShareStatus_OtherStartAudioShare:
            ret = ZN_Sharing_OtherStartAudioShare;
            break;
        case ZoomSDKShareStatus_OtherStopAudioShare:
            ret = ZN_Sharing_OtherStopAudioShare;
            break;
        case ZoomSDKShareStatus_Disconnected:
            ret = ZN_Sharing_Disconnected;
            break;
        default:
            break;
    }
    return ret;
}

ZNShareType nativeErrorTypeHelp::ZNSDKShareType(ZoomSDKShareContentType &type)
{
    ZNShareType zn_type = ZNSHARE_TYPE_UNKNOWN;
    switch (type)
    {
        case ZoomSDKShareContentType_UNKNOWN:
            zn_type = ZNSHARE_TYPE_UNKNOWN;
            break;
        case ZoomSDKShareContentType_AS:
            zn_type = ZNSHARE_TYPE_AS;
            break;
        case ZoomSDKShareContentType_DS:
            zn_type = ZNSHARE_TYPE_DS;
            break;
        case ZoomSDKShareContentType_WB:
            zn_type = ZNSHARE_TYPE_WB;
            break;
        case ZoomSDKShareContentType_AIRHOST:
            zn_type = ZNSHARE_TYPE_AIRHOST;
            break;
        case ZoomSDKShareContentType_CAMERA:
            zn_type = ZNSHARE_TYPE_CAMERA;
            break;
        case ZoomSDKShareContentType_DATA:
            zn_type = ZNSHARE_TYPE_DATA;
            break;
        case ZoomSDKShareContentType_VIDEO_FILE:
            zn_type = ZNSHARE_TYPE_VIDEO_FILE;
            break;
        case ZoomSDKShareContentType_FRAME:
            zn_type = ZNSHARE_TYPE_FRAME;
            break;
        case ZoomSDKShareContentType_DOCUMENT:
            zn_type = ZNSHARE_TYPE_DOCUMENT;
            break;
        case ZoomSDKShareContentType_COMPUTER_AUDIO:
            zn_type = ZNSHARE_TYPE_COMPUTER_AUDIO;
            break;
        default:
            break;
    }
    return zn_type;
}

ZNUserRole nativeErrorTypeHelp::ZNSDKUserRole(UserRole &role)
{
    ZNUserRole ret = ZN_USERROLE_NONE;
    switch (role) {
        case UserRole_None:
            ret = ZN_USERROLE_NONE;
            break;
        case UserRole_Host:
            ret = ZN_USERROLE_HOST;
            break;
        case UserRole_CoHost:
            ret = ZN_USERROLE_COHOST;
            break;
        case UserRole_Attendee:
            ret = ZN_USERROLE_ATTENDEE;
            break;
        case UserRole_Panelist:
            ret = ZN_USERROLE_PANELIST;
            break;
        case UserRole_BreakoutRoom_Moderator:
            ret = ZN_USERROLE_BREAKOUTROOM_MODERATOR;
            break;
        default:
            break;
    }
    return ret;
}

ZNMeetingEndReason  nativeErrorTypeHelp::ZNSDKMeetingEndReason(EndMeetingReason &reason)
{
    ZNMeetingEndReason ret = ZNEndMeetingReason_None;
    switch (reason) {
        case EndMeetingReason_None:
            ret = ZNEndMeetingReason_None;
            break;
        case EndMeetingReason_KickByHost:
            ret = ZNEndMeetingReason_KickByHost;
            break;
        case EndMeetingReason_EndByHost:
            ret = ZNEndMeetingReason_EndByHost;
            break;
        case EndMeetingReason_JBHTimeOut:
            ret = ZNEndMeetingReason_JBHTimeOut;
            break;
        case EndMeetingReason_NoAttendee:
            ret = ZNEndMeetingReason_NoAttendee;
            break;
        case EndMeetingReason_HostStartAnotherMeeting:
            ret = ZNEndMeetingReason_HostStartAnotherMeeting;
            break;
        case EndMeetingReason_FreeMeetingTimeOut:
            ret = ZNEndMeetingReason_FreeMeetingTimeOut;
            break;
        case EndMeetingReason_Undefined:
            ret = ZNEndMeetingReason_Undefined;
            break;
        default:
            break;
    }
    return ret;
}

ZNMeetingFailCode  nativeErrorTypeHelp::ZNSDKMeetingFailCode(ZoomSDKMeetingError &error)
{
    ZNMeetingFailCode ret = ZNMEETING_SUCCESS;
    switch (error) {
        case ZoomSDKMeetingError_Success:
            ret = ZNMEETING_SUCCESS;
            break;
        case ZoomSDKMeetingError_ConnectionError:
            ret = ZNMEETING_FAIL_CONNECTION_ERR;
            break;
        case ZoomSDKMeetingError_ReconnectFailed:
            ret =  ZNMEETING_FAIL_RECONNECT_ERR;
            break;
        case ZoomSDKMeetingError_MMRError:
            ret = ZNMEETING_FAIL_MMR_ERR;
            break;
        case ZoomSDKMeetingError_PasswordError:
            ret = ZNMEETING_FAIL_PASSWORD_ERR;
            break;
        case ZoomSDKMeetingError_SessionError:
            ret = ZNMEETING_FAIL_SESSION_ERR;
            break;
        case ZoomSDKMeetingError_MeetingOver:
            ret = ZNMEETING_FAIL_MEETING_OVER;
            break;
        case ZoomSDKMeetingError_MeetingNotStart:
            ret = ZNMEETING_FAIL_MEETING_NOT_START;
            break;
        case ZoomSDKMeetingError_MeetingNotExist:
            ret = ZNMEETING_FAIL_MEETING_NOT_EXIST;
            break;
        case ZoomSDKMeetingError_UserFull:
            ret = ZNMEETING_FAIL_MEETING_USER_FULL;
            break;
        case ZoomSDKMeetingError_ClientIncompatible:
            ret = ZNMEETING_FAIL_CLIENT_INCOMPATIBLE;
            break;
        case ZoomSDKMeetingError_NoMMR:
            ret = ZNMEETING_FAIL_NO_MMR;
            break;
        case ZoomSDKMeetingError_MeetingLocked:
            ret = ZNMEETING_FAIL_CONFLOCKED;
            break;
        case ZoomSDKMeetingError_MeetingRestricted:
            ret = ZNMEETING_FAIL_MEETING_RESTRICTED;
            break;
        case ZoomSDKMeetingError_MeetingJBHRestricted:
            ret = ZNMEETING_FAIL_MEETING_RESTRICTED_JBH;
            break;
        case ZoomSDKMeetingError_EmitWebRequestFailed:
            ret = ZNMEETING_FAIL_CANNOT_EMIT_WEBREQUEST;
            break;
        case ZoomSDKMeetingError_StartTokenExpired:
            ret = ZNMEETING_FAIL_CANNOT_START_TOKENEXPIRE;
            break;
        case ZoomSDKMeetingError_VideoSessionError:
            ret = ZNSESSION_VIDEO_ERR;
            break;
        case ZoomSDKMeetingError_AudioAutoStartError:
            ret = ZNSESSION_AUDIO_AUTOSTARTERR;
            break;
        case ZoomSDKMeetingError_RegisterWebinarFull:
            ret = ZNMEETING_FAIL_REGISTERWEBINAR_FULL;
            break;
        case ZoomSDKMeetingError_RegisterWebinarHostRegister:
            ret = ZNMEETING_FAIL_REGISTERWEBINAR_HOSTREGISTER;
            break;
        case ZoomSDKMeetingError_RegisterWebinarPanelistRegister:
            ret = ZNMEETING_FAIL_REGISTERWEBINAR_PANELISTREGISTER;
            break;
        case ZoomSDKMeetingError_RegisterWebinarDeniedEmail:
            ret = ZNMEETING_FAIL_REGISTERWEBINAR_DENIED_EMAIL;
            break;
        case ZoomSDKMeetingError_RegisterWebinarEnforceLogin:
            ret = ZNMEETING_FAIL_ENFORCE_LOGIN;
            break;
        case ZoomSDKMeetingError_ZCCertificateChanged:
            ret = ZNCONF_FAIL_ZC_CERTIFICATE_CHANGED;
            break;
        case ZoomSDKMeetingError_ConfigFileWriteFailed:
            ret = ZNMEETING_FAIL_WRITE_CONFIG_FILE;
            break;
        case ZoomSDKMeetingError_RemovedByHost:
            ret = ZNCONF_FAIL_REMOVED_BY_HOST;
            break;
        case ZoomSDKMeetingError_Unknown:
            ret = ZNMEETING_SUCCESS;
            break;
        case ZoomSDKMeetingError_None:
            ret = ZNMEETING_SUCCESS;
            break;
        case ZoomSDKMeetingError_vanityNotExist:
            ret = ZNCONF_FAIL_VANITY_NOT_EXIST;
            break;
        case ZoomSDKMeetingError_joinWebinarWithSameEmail:
            ret = ZNCONF_FAIL_JOIN_WEBINAR_WITHSAMEEMAIL;
            break;
        case ZoomSDKMeetingError_disallowHostMeeting:
            ret = ZNCONF_FAIL_DISALLOW_HOST_MEETING;
            break;
        case ZoomSDKMeetingError_forbidToJoinInternalMeeting:
            ret = ZNMEETING_FAIL_FORBID_TO_JOIN_INTERNAL_MEETING;
            break;
        case ZoomSDKMeetingError_HostDisallowOutsideUserJoin:
            ret = ZNMEETING_FAIL_HOST_DISALLOW_OUTSIDE_USER_JOIN;
            break;
        case ZoomSDKMeetingError_UnableToJoinExternalMeeting:
            ret = ZNMEETING_FAIL_UNABLE_TO_JOIN_EXTERNAL_MEETING;
            break;
        case ZoomSDKMeetingError_BlockedByAccountAdmin:
            ret = ZNMEETING_FAIL_BLOCKED_BY_ACCOUNT_ADMIN;
            break;
        case ZoomSDKMeetingError_NeedSigninForPrivateMeeting:
            ret = ZNMEETING_FAIL_NEED_SIGN_IN_FOR_PRIVATE_MEETING;
            break;
        case ZoomSDKMeetingError_AppPrivilegeTokenError:
            ret = ZNMEETING_FAIL_APP_PRIVILEGE_TOKEN_ERROR;
            break;
        case ZoomSDKMeetingError_JmakUserEmailNotMatch:
            ret = ZNMEETING_FAIL_JMAK_USER_EMAIL_NOT_MATCH;
            break;
        default:
            break;
    }
    return ret;
}

ZoomSDKLocale nativeErrorTypeHelp::ZNSDKAPPLocalType(ZNSDK_APP_Locale &local)
{
    ZoomSDKLocale applocal = ZoomSDKLocale_Def;
    switch (local) {
        case ZNSDK_APP_Locale_CN:
            applocal = ZoomSDKLocale_CN;
            break;
        case ZNSDK_APP_Locale_Default:
            applocal = ZoomSDKLocale_Def;
            break;
        default:
            break;
    }
    return applocal;
}

ZNSMSVerificationCodeErr nativeErrorTypeHelp::ZNSDKSMSVerificationCodeError(ZoomSDKSMSVerificationCodeError &code)
{
    ZNSMSVerificationCodeErr ret = ZNSMSVerificationCodeErr_Unknown;
    switch (code) {
        case ZoomSDKSMSError_Unknown:
            ret = ZNSMSVerificationCodeErr_Unknown;
            break;
        case ZoomSDKSMSError_Success:
            ret = ZNSMSVerificationCodeErr_Success;
            break;
        case ZoomSDKSMSError_Retrieve_SendSMSFailed:
            ret = ZNSMSVerificationCodeErr_Retrieve_SendSMSFailed;
            break;
        case ZoomSDKSMSError_Retrieve_InvalidPhoneNum:
            ret = ZNSMSVerificationCodeErr_Retrieve_InvalidPhoneNum;
            break;
        case ZoomSDKSMSError_Retrieve_PhoneNumAlreadyBound:
            ret = ZNSMSVerificationCodeErr_Retrieve_PhoneNumAlreadyBound;
            break;
        case ZoomSDKSMSError_Retrieve_PhoneNumSendTooFrequent:
            ret = ZNSMSVerificationCodeErr_Retrieve_PhoneNumSendTooFrequent;
            break;
        case ZoomSDKSMSError_Verify_CodeIncorrect:
            ret = ZNSMSVerificationCodeErr_Verify_CodeIncorrect;
            break;
        case ZoomSDKSMSError_Verify_CodeExpired:
            ret = ZNSMSVerificationCodeErr_Verify_CodeExpired;
            break;
        case ZoomSDKSMSError_Verify_UnknownError:
            ret = ZNSMSVerificationCodeErr_Verify_UnknownError;
            break;
        default:
            break;
    }
    return ret;
}

ZNAudioCallbackActionInfo nativeErrorTypeHelp::ZNSDKAudioCallbackAction(ZoomSDKAudioActionInfo &info)
{
    ZNAudioCallbackActionInfo ret = ZNACTION_INFO_NONE;
    switch (info) {
        case ZoomSDKAudioActionInfo_none:
            ret = ZNACTION_INFO_NONE;
            break;
        case ZoomSDKAudioActionInfo_needJoinVoip:
            ret = ZNACTION_INFO_NEED_JOIN_VOIP;
            break;
        case ZoomSDKAudioActionInfo_muteOrUnmenuAudio:
            ret = ZNACTION_INFO_MUTE_UNMUTE_AUDIO;
            break;
        case ZoomSDKAudioActionInfo_noAudioDeviceConnected:
            ret = ZNACTION_INFO_CHOOSE_AUDIO_DEVICE_NOAUDIODEVICECONNECTTED;
            break;
        case ZoomSDKAudioActionInfo_computerAudioDeviceError:
            ret = ZNACTION_INFO_CHOOSE_AUDIO_DEVICE_COMPUTERAUDIODEVICEERROR;
            break;
        default:
            break;
    }
    return ret;
}

ZNWebinarNeedRegisterType nativeErrorTypeHelp::ZNSDKWebinarNeedRegisterType(WebinarRegisterType &type)
{
    ZNWebinarNeedRegisterType ret = ZNWebinarReg_NONE;
    switch (type) {
        case WebinarRegisterType_None:
            ret = ZNWebinarReg_NONE;
            break;
        case WebinarRegisterType_URL:
            ret = ZNWebinarReg_By_Register_Url;
            break;
        case WebinarRegisterType_Email:
            ret = ZNWebinarReg_By_Email_and_DisplayName;
            break;
        default:
            break;
    }
    return ret;
}

ZNRequiredInfoType nativeErrorTypeHelp::ZNSDKRequiredInfoType(JoinMeetingReqInfoType &type)
{
    ZNRequiredInfoType ret = ZNREQUIRED_INFO_TYPE_NONE;
    switch (type) {
        case JoinMeetingReqInfoType_None:
            ret = ZNREQUIRED_INFO_TYPE_NONE;
            break;
        case JoinMeetingReqInfoType_Password:
            ret = ZNREQUIRED_INFO_TYPE_Password;
            break;
        case JoinMeetingReqInfoType_Password_Wrong:
            ret = ZNREQUIRED_INFO_TYPE_Password4WrongPassword;
            break;
        default:
            break;
    }
    return ret;
}


ZNRecordingStatus nativeErrorTypeHelp::ZNRecordingStatusChange(ZoomSDKRecordingStatus &status)
{
    ZNRecordingStatus ZNStatus = ZNRecording_Stop;
    switch (status) {
        case ZoomSDKRecordingStatus_Stop:
            ZNStatus = ZNRecording_Stop;
            break;
        case ZoomSDKRecordingStatus_Pause:
            ZNStatus = ZNRecording_Pause;
            break;
        case ZoomSDKRecordingStatus_DiskFull:
            ZNStatus = ZNRecording_DiskFull;
            break;
        case ZoomSDKRecordingStatus_Start:
            ZNStatus = ZNRecording_Start;
            break;
        default:
            break;
    }
    return ZNStatus;
}

ZNRequestStartCloudRecordingStatus nativeErrorTypeHelp::ZNRequestStartCloudRecordingStatusMap(ZoomSDKRequestStartCloudRecordingStatus &status)
{
    ZNRequestStartCloudRecordingStatus zn_status = ZNRequestStartCloudRecording_Granted;
    switch (status) {
        case ZoomSDKRequestStartCloudRecordingStatus_Granted:
            zn_status = ZNRequestStartCloudRecording_Granted;
            break;
        case ZoomSDKRequestStartCloudRecordingStatus_Denied:
            zn_status = ZNRequestStartCloudRecording_Denied;
            break;
        case ZoomSDKRequestStartCloudRecordingStatus_TimedOut:
            zn_status = ZNRequestStartCloudRecording_TimedOut;
            break;
        default:
            break;
    }
    return zn_status;
}

ZNLoginFailReason nativeErrorTypeHelp::ZNLoginFailReasonMap(ZoomSDKLoginFailReason &reason)
{
    ZNLoginFailReason failReason = ZNLoginFail_None;
    switch(reason){
        case ZoomSDKLoginFailReason_None:
            failReason = ZNLoginFail_None;
            break;
        case ZoomSDKLoginFailReason_EmailLoginDisabled:
            failReason = ZNLoginFail_EmailLoginDisable;
            break;
        case ZoomSDKLoginFailReason_UserNotExist:
            failReason = ZNLoginFail_UserNotExist;
            break;
        case ZoomSDKLoginFailReason_WrongPassword:
            failReason = ZNLoginFail_WrongPassword;
            break;
        case ZoomSDKLoginFailReason_AccountLocked:
            failReason = ZNLoginFail_AccountLocked;
            break;
        case ZoomSDKLoginFailReason_SDKNeedUpdate:
            failReason = ZNLoginFail_SDKNeedUpdate;
            break;
        case ZoomSDKLoginFailReason_TooManyFailedAttempts:
            failReason = ZNLoginFail_TooManyFailedAttempts;
            break;
        case ZoomSDKLoginFailReason_SMSCodeError:
            failReason = ZNLoginFail_SMSCodeError;
            break;
        case ZoomSDKLoginFailReason_SMSCodeExpired:
            failReason = ZNLoginFail_SMSCodeExpired;
            break;
        case ZoomSDKLoginFailReason_PhoneNumberFormatInValid:
            failReason = ZNLoginFail_PhoneNumberFormatInValid;
            break;
        case ZoomSDKLoginFailReason_LoginTokenInvalid:
            failReason = ZNLoginFail_LoginTokenInvalid;
            break;
        case ZoomSDKLoginFailReason_Other_Issue:
            failReason = ZNLoginFail_OtherIssue;
            break;
        default:
            failReason = ZNLoginFail_OtherIssue;
            break;
    }
    return failReason;
}

ZNStatisticsWarningType nativeErrorTypeHelp::ZNStatisticsWarningTypeMap(StatisticWarningType &type)
{
    ZNStatisticsWarningType warningType = ZNStatistics_Warning_None;
    switch (type) {
        case StatisticWarningType_None:
            warningType = ZNStatistics_Warning_None;
            break;
        case StatisticWarningType_NetworkBad:
            warningType = ZNStatistics_Warning_Network_Quality_Bad;
            break;
        case StatisticWarningType_CPUHigh:
        case StatisticWarningType_SystemBusy:
            warningType = ZNStatistics_Warning_Busy_System;
            break;
        default:
            break;
    }
    return warningType;
}

ZNAudioShareMode nativeErrorTypeHelp::ZNAudioShareModeMap(ZoomSDKAudioShareMode &mode)
{
    ZNAudioShareMode shareMode = ZNAudioShareMode_Mono;
    switch (mode) {
        case ZoomSDKAudioShareMode_Mono:
            shareMode = ZNAudioShareMode_Mono;
            break;
        case ZoomSDKAudioShareMode_Stereo:
            shareMode = ZNAudioShareMode_Stereo;
            break;
            
        default:
            break;
    }
    return shareMode;
}

ZoomSDKAudioShareMode nativeErrorTypeHelp::ZoomSDKAudioShareModeMap(ZNAudioShareMode &mode)
{
    ZoomSDKAudioShareMode shareMode = ZoomSDKAudioShareMode_Mono;
    switch (mode) {
        case ZNAudioShareMode_Mono:
            shareMode = ZoomSDKAudioShareMode_Mono;
            break;
        case ZNAudioShareMode_Stereo:
            shareMode = ZoomSDKAudioShareMode_Stereo;
            break;
            
        default:
            break;
    }
    return shareMode;
}

ZNLiveStreamStatus nativeErrorTypeHelp::ZNLiveStreamStatusMap(LiveStreamStatus &status)
{
    ZNLiveStreamStatus liveStreamStatus = ZN_LiveStreamStatus_None;
    switch (status) {
        case LiveStreamStatus_None:
            liveStreamStatus = ZN_LiveStreamStatus_None;
            break;
        case LiveStreamStatus_InProgress:
            liveStreamStatus = ZN_LiveStreamStatus_InProgress;
            break;
        case LiveStreamStatus_Connecting:
            liveStreamStatus = ZN_LiveStreamStatus_Connecting;
            break;
        case LiveStreamStatus_StartFailedTimeout:
            liveStreamStatus = ZN_LiveStreamStatus_Start_Failed_Timeout;
            break;
        case LiveStreamStatus_StartFailed:
            liveStreamStatus = ZN_LiveStreamStatus_Start_Failed;
            break;
        case LiveStreamStatus_Ended:
            liveStreamStatus = ZN_LiveStreamStatus_Ended;
            break;
        default:
            break;
    }
    return liveStreamStatus;
}

ZNMeetingReminderType nativeErrorTypeHelp::ZNReminderTypeMap(ZoomSDKReminderType &type)
{
    ZNMeetingReminderType reminder_type = ZNTYPE_LOGIN_REQUIRED;
    switch (type) {
        case ZoomSDKReminderType_LoginRequired:
            reminder_type = ZNTYPE_LOGIN_REQUIRED;
            break;
        case ZoomSDKReminderType_StartOrJoinMeeting:
            reminder_type = ZNTYPE_START_OR_JOIN_MEETING;
            break;
        case ZoomSDKReminderType_RecordReminder:
            reminder_type = ZNTYPE_RECORD_REMINDER;
            break;
        case ZoomSDKReminderType_LiveStreamDisclaimer:
            reminder_type = ZNTYPE_LIVE_STREAM_DISCLAIMER;
            break;
        case ZoomSDKReminderType_ArchiveDisclaimer:
            reminder_type = ZNTYPE_ARCHIVE_DISCLAIMER;
            break;
        case ZoomSDKReminderType_WebinarAsPanelistJoin:
            reminder_type = ZNTYPE_WEBINAR_AS_PANELIST_JOIN;
            break;
        case ZoomSDKReminderType_TermsService:
            reminder_type = ZNTYPE_TERMS_OF_SERVICE;
            break;
        case ZoomSDKReminderType_SmartSummaryDisclaimer:
            reminder_type = ZNTYPE_SMART_SUMMARY_DISCLAIMER;
            break;
        case ZoomSDKReminderType_QueryDisclaimer:
            reminder_type = ZNTYPE_QUERY_DISCLAIMER;
            break;
        case ZoomSDKReminderType_WebinarAttendeePromoteReminder:
            reminder_type = ZNTYPE_WEBINAR_ATTENDEE_PROMOTE_REMINDER;
            break;
        case ZoomSDKReminderType_JoinMeetingPrivateModeReminder:
            reminder_type = ZNTYPE_JOIN_PRIVATE_MODE_MEETING_REMINDER;
            break;
        case ZoomSDKReminderType_ClosedCaptionDisclamier:
            reminder_type = ZNTYPE_CLOSED_CAPTION_DISCLAIMER;
            break;
        case ZoomSDKReminderType_MultiDisclamier:
            reminder_type = ZNTYPE_MULTI_DISCLAIMER;
            break;
        case ZoomSDKReminderType_JoinMeetingConnectorAsGuestReminder:
            reminder_type = ZNTYPE_JOIN_MEETING_CONNECTOR_AS_GUEST_REMINDER;
            break;
        case ZoomSDKReminderType_CommonDisclaimer:
            reminder_type = ZNTYPE_COMMON_DISCLAIMER;
            break;
        default:
            break;
    }
    return reminder_type;
}

ZoomSDKChatPrivilegeType nativeErrorTypeHelp::ZoomSDKChatPrivilegeTypeMap(ZNSDKChatPrivilege &privilege)
{
    ZoomSDKChatPrivilegeType ret = ZoomSDKChatPrivilegeType_To_Everyone;
    switch (privilege) {
        case ZNSDK_CHAT_PRIVILEGE_ALL:
            ret = ZoomSDKChatPrivilegeType_To_Everyone;
            break;
        case ZNSDK_CHAT_PRIVILEGE_ALL_PANELIST:
            ret = ZoomSDKChatPrivilegeType_To_All_Panelist;
            break;
        case ZNSDK_CHAT_PRIVILEGE_HOST:
            ret = ZoomSDKChatPrivilegeType_To_Host;
            break;
        case ZNSDK_CHAT_PRIVILEGE_DISABLE_ATTENDEE_CHAT:
            ret = ZoomSDKChatPrivilegeType_Disable_Attendee_Chat;
            break;
        case ZNSDK_CHAT_PRIVILEGE_HOST_PUBLIC:
            ret = ZoomSDKChatPrivilegeType_Host_Public;
            break;
        default:
            break;
    }
    return ret;
}

ZoomSDKChatMessageType nativeErrorTypeHelp::ZoomSDKChatMessageTypeMap(ZNSDKChatMessageType &type)
{
    ZoomSDKChatMessageType ret = ZoomSDKChatMessageType_To_None;
    switch (type) {
        case ZNSDKChatMessageType_To_None:
            ret = ZoomSDKChatMessageType_To_None;
            break;
        case ZNSDKChatMessageType_To_All:
            ret = ZoomSDKChatMessageType_To_All;
            break;
        case ZNSDKChatMessageType_To_All_Panelist:
            ret = ZoomSDKChatMessageType_To_All_Panelist;
            break;
        case ZNSDKChatMessageType_To_Individual_Panelist:
            ret = ZoomSDKChatMessageType_To_Individual_Panelist;
            break;
        case ZNSDKChatMessageType_To_Individual:
            ret = ZoomSDKChatMessageType_To_Individual;
            break;
        case ZNSDKChatMessageType_To_WaitingRoomUsers:
            ret = ZoomSDKChatMessageType_To_WaitingRoomUsers;
            break;
        default:
            break;
    }
    return ret;
}

ZNSDKChatMessageType nativeErrorTypeHelp::ZNSDKChatMessageTypeMap(ZoomSDKChatMessageType &type)
{
    ZNSDKChatMessageType ret = ZNSDKChatMessageType_To_None;
    switch (type) {
        case ZoomSDKChatMessageType_To_None:
            ret = ZNSDKChatMessageType_To_None;
            break;
        case ZoomSDKChatMessageType_To_All:
            ret = ZNSDKChatMessageType_To_All;
            break;
        case ZoomSDKChatMessageType_To_All_Panelist:
            ret = ZNSDKChatMessageType_To_All_Panelist;
            break;
        case ZoomSDKChatMessageType_To_Individual_Panelist:
            ret = ZNSDKChatMessageType_To_Individual_Panelist;
            break;
        case ZoomSDKChatMessageType_To_Individual:
            ret = ZNSDKChatMessageType_To_Individual;
            break;
        case ZoomSDKChatMessageType_To_WaitingRoomUsers:
            ret = ZNSDKChatMessageType_To_WaitingRoomUsers;
            break;
        default:
            break;
    }
    return ret;
}

ZNSDKChatMessageDeleteType nativeErrorTypeHelp::ZNSDKChatMessageDeleteTypeMap(ZoomSDKChatMessageDeleteType &type)
{
    ZNSDKChatMessageDeleteType ret = ZNSDK_CHAT_DELETE_BY_NONE;
    switch (type) {
        case ZoomSDK_Chat_Delete_By_None:
            ret = ZNSDK_CHAT_DELETE_BY_NONE;
            break;
        case ZoomSDK_Chat_Delete_By_Self:
            ret = ZNSDK_CHAT_DELETE_BY_SELF;
            break;
        case ZoomSDK_Chat_Delete_By_Host:
            ret = ZNSDK_CHAT_DELETE_BY_HOST;
            break;
        case ZoomSDK_Chat_Delete_By_Dlp:
            ret = ZNSDK_CHAT_DELETE_BY_DLP;
            break;
        default:
            break;
    }
    return ret;
}

ZNSDKFileTransferStatus nativeErrorTypeHelp::ZNSDKFileTransferStatusMap(ZoomSDKFileTransferStatus &status)
{
    ZNSDKFileTransferStatus zn_status = ZNSDKFileTransferState_None;
    switch (status) {
        case ZoomSDKFileTransferStatus_None:
            zn_status = ZNSDKFileTransferState_None;
            break;
        case ZoomSDKFileTransferStatus_ReadyToTransfer:
            zn_status = ZNSDKFileTransferState_ReadyToTransfer;
            break;
        case ZoomSDKFileTransferStatus_Transfering:
            zn_status = ZNSDKFileTransferState_Transfering;
            break;
        case ZoomSDKFileTransferStatus_TransferFailed:
            zn_status = ZNSDKFileTransferState_TransferFailed;
            break;
        case ZoomSDKFileTransferStatus_TransferDone:
            zn_status = ZNSDKFileTransferState_TransferDone;
            break;
        default:
            break;
    }
    return zn_status;
}

ZNAudioType nativeErrorTypeHelp::ZNAudioTypeMap(ZoomSDKAudioType &type)
{
    ZNAudioType ret = ZN_AUDIOTYPE_NONE;
    switch (type) {
        case ZoomSDKAudioType_None:
            ret = ZN_AUDIOTYPE_NONE;
            break;
        case ZoomSDKAudioType_Voip:
            ret = ZN_AUDIOTYPE_VOIP;
            break;
        case ZoomSDKAudioType_Phone:
            ret = ZN_AUDIOTYPE_PHONE;
            break;
        case ZoomSDKAudioType_Unknown:
            ret = ZN_AUDIOTYPE_UNKNOWN;
            break;
        default:
            break;
    }
    return ret;
}

ZNSDKEmojiFeedbackType nativeErrorTypeHelp::ZNSDKEmojiFeedbackTypeMap(ZoomSDKEmojiFeedbackType &type)
{
    ZNSDKEmojiFeedbackType ret = ZNSDKEmojiFeedbackType_None;
    switch (type) {
        case ZoomSDKEmojiFeedbackType_None:
            ret = ZNSDKEmojiFeedbackType_None;
            break;
        case ZoomSDKEmojiFeedbackType_Yes:
            ret = ZNSDKEmojiFeedbackType_Yes;
            break;
        case ZoomSDKEmojiFeedbackType_No:
            ret = ZNSDKEmojiFeedbackType_No;
            break;
        case ZoomSDKEmojiFeedbackType_SpeedUp:
            ret = ZNSDKEmojiFeedbackType_SpeedUp;
            break;
        case ZoomSDKEmojiFeedbackType_SlowDown:
            ret = ZNSDKEmojiFeedbackType_SlowDown;
            break;
        case ZoomSDKEmojiFeedbackType_Away:
            ret = ZNSDKEmojiFeedbackType_Away;
            break;
        default:
            break;
    }
    return ret;
}

ZNWaitingRoomLayoutType nativeErrorTypeHelp::ZNWaitingRoomLayoutTypeMap(ZoomSDKWaitingRoomLayoutType &type)
{
    ZNWaitingRoomLayoutType ret = ZNWaitingRoomLayoutType_Default;
    switch (type) {
        case ZoomSDKWaitingRoomLayoutType_Default:
            ret = ZNWaitingRoomLayoutType_Default;
            break;
        case ZoomSDKWaitingRoomLayoutType_Logo:
            ret = ZNWaitingRoomLayoutType_Logo;
            break;
        case ZoomSDKWaitingRoomLayoutType_Video:
            ret = ZNWaitingRoomLayoutType_Video;
            break;
        default:
            break;
    }
    return ret;
}

ZNCustomWaitingRoomDataStatus nativeErrorTypeHelp::ZNCustomWaitingRoomDataStatusMap(ZoomSDKCustomWaitingRoomDataStatus &status)
{
    ZNCustomWaitingRoomDataStatus ret = ZNCustomWaitingRoomDataStatus_Init;
    switch (status) {
        case ZoomSDKCustomWaitingRoomDataStatus_Init:
            ret = ZNCustomWaitingRoomDataStatus_Init;
            break;
        case ZoomSDKCustomWaitingRoomDataStatus_Downloading:
            ret = ZNCustomWaitingRoomDataStatus_Downloading;
            break;
        case ZoomSDKCustomWaitingRoomDataStatus_Download_OK:
            ret = ZNCustomWaitingRoomDataStatus_Download_OK;
            break;
        case ZoomSDKCustomWaitingRoomDataStatus_Download_Failed:
            ret = ZNCustomWaitingRoomDataStatus_Download_Failed;
            break;
        default:
            break;
        }
    return ret;
}


ZoomSDKPanelistChatPrivilege nativeErrorTypeHelp::ZoomSDKPanelistChatPrivilegeMap(ZNSDKPanelistChatPrivilege &privilege)
{
    ZoomSDKPanelistChatPrivilege ret = ZoomSDKPanelistChatPrivilege_PanelistOnly;
    switch (privilege) {
        case ZNSDKPanelistChatPrivilege_PanelistOnly:
            ret = ZoomSDKPanelistChatPrivilege_PanelistOnly;
            break;
        case ZNSDKPanelistChatPrivilege_All:
            ret = ZoomSDKPanelistChatPrivilege_All;
            break;
        default:
            break;
    }
    return ret;
}

ZNSDKPanelistChatPrivilege nativeErrorTypeHelp::ZNSDKPanelistChatPrivilegeMap(ZoomSDKPanelistChatPrivilege &privilege)
{
    ZNSDKPanelistChatPrivilege ret = ZNSDKPanelistChatPrivilege_PanelistOnly;
    switch (privilege) {
        case ZoomSDKPanelistChatPrivilege_PanelistOnly:
            ret = ZNSDKPanelistChatPrivilege_PanelistOnly;
            break;
        case ZoomSDKPanelistChatPrivilege_All:
            ret = ZNSDKPanelistChatPrivilege_All;
            break;
        default:
            break;
    }
    return ret;
}

ZoomSDKAttendeeViewMode nativeErrorTypeHelp::ZoomSDKAttendeeViewModeMap(ZNSDKAttendeeViewMode &mode)
{
    ZoomSDKAttendeeViewMode ret = ZoomSDKAttendeeViewMode_None;
    switch (mode) {
        case ZNSDKAttendeeViewMode_None:
            ret = ZoomSDKAttendeeViewMode_None;
            break;
        case ZNSDKAttendeeViewMode_FollowHost:
            ret = ZoomSDKAttendeeViewMode_FollowHost;
            break;
        case ZNSDKAttendeeViewMode_Speaker:
            ret = ZoomSDKAttendeeViewMode_Speaker;
            break;
        case ZNSDKAttendeeViewMode_Gallery:
            ret = ZoomSDKAttendeeViewMode_Gallery;
            break;
        case ZNSDKAttendeeViewMode_Standard:
            ret = ZoomSDKAttendeeViewMode_Sharing_Standard;
            break;
        case ZNSDKAttendeeViewMode_SidebysideSpeaker:
            ret = ZoomSDKAttendeeViewMode_Sharing_SidebysideSpeaker;
            break;
        case ZNSDKAttendeeViewMode_SidebysideGallery:
            ret = ZoomSDKAttendeeViewMode_Sharing_SidebysideGallery;
            break;
        default:
            break;
    }
    return ret;
}

ZNSDKAttendeeViewMode nativeErrorTypeHelp::ZNSDKAttendeeViewModeMap(ZoomSDKAttendeeViewMode &mode)
{
    ZNSDKAttendeeViewMode ret = ZNSDKAttendeeViewMode_None;
    switch (mode) {
        case ZoomSDKAttendeeViewMode_None:
            ret = ZNSDKAttendeeViewMode_None;
            break;
        case ZoomSDKAttendeeViewMode_FollowHost:
            ret = ZNSDKAttendeeViewMode_FollowHost;
            break;
        case ZoomSDKAttendeeViewMode_Speaker:
            ret = ZNSDKAttendeeViewMode_Speaker;
            break;
        case ZoomSDKAttendeeViewMode_Gallery:
            ret = ZNSDKAttendeeViewMode_Gallery;
            break;
        case ZoomSDKAttendeeViewMode_Sharing_Standard:
            ret = ZNSDKAttendeeViewMode_Standard;
            break;
        case ZoomSDKAttendeeViewMode_Sharing_SidebysideSpeaker:
            ret = ZNSDKAttendeeViewMode_SidebysideSpeaker;
            break;
        case ZoomSDKAttendeeViewMode_Sharing_SidebysideGallery:
            ret = ZNSDKAttendeeViewMode_SidebysideGallery;
            break;
        default:
            break;
    }
    return ret;
}

ZNZoomSDKWallpaperLayoutMode nativeErrorTypeHelp::ZNSDKWallpaperLayoutModeMap(ZoomSDKWallpaperLayoutMode &mode)
{
    ZNZoomSDKWallpaperLayoutMode zn_mode = ZNZoomSDKWallpaperLayoutMode_None;
    switch (mode) {
        case ZoomSDKWallpaperLayoutMode_None:
            zn_mode = ZNZoomSDKWallpaperLayoutMode_None;
            break;
        case ZoomSDKWallpaperLayoutMode_Fill:
            zn_mode = ZNZoomSDKWallpaperLayoutMode_Fill;
            break;
        case ZoomSDKWallpaperLayoutMode_Fit:
            zn_mode = ZNZoomSDKWallpaperLayoutMode_Fit;
            break;
        default:
            break;
    }
    return zn_mode;
}

ZoomSDKWallpaperLayoutMode nativeErrorTypeHelp::ZoomSDKWallpaperLayoutModeMap(ZNZoomSDKWallpaperLayoutMode &zn_mode)
{
    ZoomSDKWallpaperLayoutMode mode = ZoomSDKWallpaperLayoutMode_None;
    switch (zn_mode) {
        case ZNSDKAttendeeViewMode_None:
            mode = ZoomSDKWallpaperLayoutMode_None;
            break;
        case ZNZoomSDKWallpaperLayoutMode_Fill:
            mode = ZoomSDKWallpaperLayoutMode_Fill;
            break;
        case ZNZoomSDKWallpaperLayoutMode_Fit:
            mode = ZoomSDKWallpaperLayoutMode_Fit;
            break;
        default:
            break;
    }
    return mode;
}

ZNZoomSDKWallpaperSettingStatus nativeErrorTypeHelp::ZNSDKWallpaperSettingStatusMap(ZoomSDKWallpaperSettingStatus &status)
{
    ZNZoomSDKWallpaperSettingStatus zn_status = ZNZoomSDKWallpaperSettingStatus_None;
    switch (status) {
        case ZoomSDKWallpaperSettingStatus_None:
            zn_status = ZNZoomSDKWallpaperSettingStatus_None;
            break;
        case ZoomSDKWallpaperSettingStatus_Downloading:
            zn_status = ZNZoomSDKWallpaperSettingStatus_Downloading;
            break;
        case ZoomSDKWallpaperSettingStatus_Downloaded:
            zn_status = ZNZoomSDKWallpaperSettingStatus_Downloaded;
            break;
        case ZoomSDKWallpaperSettingStatus_DownloadFail:
            zn_status = ZNZoomSDKWallpaperSettingStatus_DownloadFail;
            break;
        default:
            break;
    }
    return zn_status;
}

ZNVBVideoError nativeErrorTypeHelp::ZNVBVideoErrorMap(ZoomSDKSettingVBVideoError &error) {
    ZNVBVideoError zn_error = ZNVB_VideoError_None;
    switch (error) {
        case ZoomSDKSettingVBVideoError_None:
            zn_error = ZNVB_VideoError_None;
            break;
        case ZoomSDKSettingVBVideoError_UnknownFormat:
            zn_error = ZNVB_VideoError_UnknownFormat;
            break;
        case ZoomSDKSettingVBVideoError_ResolutionBig:
            zn_error = ZNVB_VideoError_ResolutionHigh1080P;
            break;
        case ZoomSDKSettingVBVideoError_ResolutionHigh720P:
            zn_error = ZNVB_VideoError_ResolutionHigh720P;
            break;
        case ZoomSDKSettingVBVideoError_ResolutionLow:
            zn_error = ZNVB_VideoError_ResolutionLow;
            break;
        case ZoomSDKSettingVBVideoError_PlayError:
            zn_error = ZNVB_VideoError_PlayError;
            break;
        case ZoomSDKSettingVBVideoError_OpenError:
            zn_error = ZNVB_VideoError_OpenError;
            break;
        default:
            break;
    }
    return zn_error;
}

ZNMeetingAICompanionQuerySettingOptions nativeErrorTypeHelp::ZNMeetingAICompanionQuerySettingOptionsMap(ZoomSDKAICompanionQuerySettingOptions &option) {
    ZNMeetingAICompanionQuerySettingOptions zn_option = ZNMeetingAICompanionQuerySettingOptions_None;
    switch (option) {
        case ZoomSDKAICompanionQuerySettingOptions_None:
            zn_option = ZNMeetingAICompanionQuerySettingOptions_None;
            break;
        case ZoomSDKAICompanionQuerySettingOptions_WhenQueryStarted:
            zn_option = ZNMeetingAICompanionQuerySettingOptions_WhenQueryStarted;
            break;
        case ZoomSDKAICompanionQuerySettingOptions_WhenParticipantsJoin:
            zn_option = ZNMeetingAICompanionQuerySettingOptions_WhenParticipantsJoin;
            break;
        case ZoomSDKAICompanionQuerySettingOptions_OnlyHost:
            zn_option = ZNMeetingAICompanionQuerySettingOptions_OnlyHost;
            break;
        case ZoomSDKAICompanionQuerySettingOptions_ParticipantsAndInviteesInOurOrganization:
            zn_option = ZNMeetingAICompanionQuerySettingOptions_ParticipantsAndInviteesInOurOrganization;
            break;
        case ZoomSDKAICompanionQuerySettingOptions_WhenParticipantsAndOrganizationJoin:
            zn_option = ZNMeetingAICompanionQuerySettingOptions_WhenParticipantsAndOrganizationJoin;
            break;
        case ZoomSDKAICompanionQuerySettingOptions_HostAndPanelists:
            zn_option = ZNMeetingAICompanionQuerySettingOptions_HostAndPanelists;
            break;
        case ZoomSDKAICompanionQuerySettingOptions_HostAndPanelistsInOurOrganization:
            zn_option = ZNMeetingAICompanionQuerySettingOptions_HostAndPanelistsInOurOrganization;
            break;
        default:
            break;
    }
    return zn_option;
}

ZoomSDKAICompanionQuerySettingOptions nativeErrorTypeHelp::ZoomSDKMeetingAICompanionQuerySettingOptionsMap(ZNMeetingAICompanionQuerySettingOptions &zn_option) {
    ZoomSDKAICompanionQuerySettingOptions option = ZoomSDKAICompanionQuerySettingOptions_None;
    switch (zn_option) {
        case ZNMeetingAICompanionQuerySettingOptions_None:
            option = ZoomSDKAICompanionQuerySettingOptions_None;
            break;
        case ZNMeetingAICompanionQuerySettingOptions_WhenQueryStarted:
            option = ZoomSDKAICompanionQuerySettingOptions_WhenQueryStarted;
            break;
        case ZNMeetingAICompanionQuerySettingOptions_WhenParticipantsJoin:
            option = ZoomSDKAICompanionQuerySettingOptions_WhenParticipantsJoin;
            break;
        case ZNMeetingAICompanionQuerySettingOptions_OnlyHost:
            option = ZoomSDKAICompanionQuerySettingOptions_OnlyHost;
            break;
        case ZNMeetingAICompanionQuerySettingOptions_ParticipantsAndInviteesInOurOrganization:
            option = ZoomSDKAICompanionQuerySettingOptions_ParticipantsAndInviteesInOurOrganization;
            break;
        case ZNMeetingAICompanionQuerySettingOptions_WhenParticipantsAndOrganizationJoin:
            option = ZoomSDKAICompanionQuerySettingOptions_WhenParticipantsAndOrganizationJoin;
            break;
        case ZNMeetingAICompanionQuerySettingOptions_HostAndPanelists:
            option = ZoomSDKAICompanionQuerySettingOptions_HostAndPanelists;
            break;
        case ZNMeetingAICompanionQuerySettingOptions_HostAndPanelistsInOurOrganization:
            option = ZoomSDKAICompanionQuerySettingOptions_HostAndPanelistsInOurOrganization;
            break;
        default:
            break;
    }
    return option;
}

ZNMeetingAICompanionQueryRequestError nativeErrorTypeHelp::ZNMeetingAICompanionQueryRequestErrorMap(ZoomSDKAICompanionQueryRequestError &error)
{
    ZNMeetingAICompanionQueryRequestError zn_error = ZNMeetingAICompanionQueryRequestError_OK;
    switch (error) {
        case ZoomSDKAICompanionQueryRequestError_OK:
            zn_error = ZNMeetingAICompanionQueryRequestError_OK;
            break;
        case ZoomSDKAICompanionQueryRequestError_InvalidParam:
            zn_error = ZNMeetingAICompanionQueryRequestError_InvalidParam;
            break;
        case ZoomSDKAICompanionQueryRequestError_SendFailed:
            zn_error = ZNMeetingAICompanionQueryRequestError_SendFailed;
            break;
        case ZoomSDKAICompanionQueryRequestError_WebUnAvailable:
            zn_error = ZNMeetingAICompanionQueryRequestError_WebUnAvailable;
            break;
        case ZoomSDKAICompanionQueryRequestError_PermissionVerifyFailed:
            zn_error = ZNMeetingAICompanionQueryRequestError_PermissionVerifyFailed;
            break;
        case ZoomSDKAICompanionQueryRequestError_QueryRateLimitError:
            zn_error = ZNMeetingAICompanionQueryRequestError_QueryRateLimitError;
            break;
        case ZoomSDKAICompanionQueryRequestError_Timeout:
            zn_error = ZNMeetingAICompanionQueryRequestError_Timeout;
            break;
        case ZoomSDKAICompanionQueryRequestError_Unknown:
            zn_error = ZNMeetingAICompanionQueryRequestError_Unknown;
            break;
        default:
            break;
    }
    return zn_error;
}

ZoomSDKAICompanionQueryFeedbackType nativeErrorTypeHelp::ZoomSDKAICompanionQueryFeedbackTypeMap(ZNMeetingAICompanionQueryFeedbackType &zn_type)
{
    ZoomSDKAICompanionQueryFeedbackType type = ZoomSDKAICompanionQueryFeedbackType_None;
    switch (zn_type) {
        case ZNMeetingAICompanionQueryFeedbackType_None:
            type = ZoomSDKAICompanionQueryFeedbackType_None;
            break;
        case ZNMeetingAICompanionQueryFeedbackType_Good:
            type = ZoomSDKAICompanionQueryFeedbackType_Good;
            break;
        case ZNMeetingAICompanionQueryFeedbackType_Bad:
            type = ZoomSDKAICompanionQueryFeedbackType_Bad;
            break;
        default:
            break;
    }
    return type;
}

ZoomSDKVideoPreferenceMode nativeErrorTypeHelp::SDKVideoPreferenceModeMap(ZNSDKVideoPreferenceMode &zn_mode)
{
    ZoomSDKVideoPreferenceMode mode = ZoomSDKVideoPreferenceMode_Balance;
    switch (zn_mode)
    {
        case ZNSDKVideoPreferenceMode_Balance:
            mode = ZoomSDKVideoPreferenceMode_Balance;
            break;
        case ZNSDKVideoPreferenceMode_Sharpness:
            mode = ZoomSDKVideoPreferenceMode_Sharpness;
            break;
        case ZNSDKVideoPreferenceMode_Smoothness:
            mode = ZoomSDKVideoPreferenceMode_Smoothness;
            break;
        case ZNSDKVideoPreferenceMode_Custom:
            mode = ZoomSDKVideoPreferenceMode_Custom;
            break;
        default:
            break;
    }
    return mode;
}

ZNAICompanionFeature nativeErrorTypeHelp::ZNAICompanionFeatureMap(ZoomSDKAICompanionFeature &feature)
{
    ZNAICompanionFeature zn_feature = ZNSMART_SUMMARY;
    switch (feature)
    {
        case ZoomSDKAICompanionFeature_SmartSummary:
            zn_feature = ZNSMART_SUMMARY;
            break;
        case ZoomSDKAICompanionFeature_Query:
            zn_feature = ZNQUERY;
            break;
        case ZoomSDKAICompanionFeature_SmartRecording:
            zn_feature = ZNSMART_RECORDING;
            break;
        default:
            break;
    }
    return zn_feature;
}

ZNSDKEmojiReactionType nativeErrorTypeHelp::ZNSDKEmojiReactionTypeMap(ZoomSDKEmojiReactionType &type)
{
    ZNSDKEmojiReactionType ret = ZNSDKEmojiReactionType_Unknow;
    switch (type) {
        case ZoomSDKEmojiReactionType_Clap:
            ret = ZNSDKEmojiReactionType_Clap;
            break;
        case ZoomSDKEmojiReactionType_Thumbsup:
            ret = ZNSDKEmojiReactionType_Thumbsup;
            break;
        case ZoomSDKEmojiReactionType_Heart:
            ret = ZNSDKEmojiReactionType_Heart;
            break;
        case ZoomSDKEmojiReactionType_Joy:
            ret = ZNSDKEmojiReactionType_Joy;
            break;
        case ZoomSDKEmojiReactionType_Openmouth:
            ret = ZNSDKEmojiReactionType_Openmouth;
            break;
        case ZoomSDKEmojiReactionType_Tada:
            ret = ZNSDKEmojiReactionType_Tada;
            break;
        default:
            break;
    }
    return ret;
}

ZNSDKEmojiReactionSkinTone nativeErrorTypeHelp::ZNSDKEmojiReactionSkinToneMap(ZoomSDKEmojiReactionSkinTone &skinTone)
{
    ZNSDKEmojiReactionSkinTone ret = ZNSDKEmojiReactionSkinTone_Unknow;
    switch (skinTone) {
        case ZoomSDKEmojiReactionSkinTone_Default:
            ret = ZNSDKEmojiReactionSkinTone_Default;
            break;
        case ZoomSDKEmojiReactionSkinTone_Light:
            ret = ZNSDKEmojiReactionSkinTone_Light;
            break;
        case ZoomSDKEmojiReactionSkinTone_MediumLight:
            ret = ZNSDKEmojiReactionSkinTone_MediumLight;
            break;
        case ZoomSDKEmojiReactionSkinTone_Medium:
            ret = ZNSDKEmojiReactionSkinTone_Medium;
            break;
        case ZoomSDKEmojiReactionSkinTone_MediumDark:
            ret = ZNSDKEmojiReactionSkinTone_MediumDark;
            break;
        case ZoomSDKEmojiReactionSkinTone_Dark:
            ret = ZNSDKEmojiReactionSkinTone_Dark;
            break;
        default:
            break;
    }
    return ret;
}

ZNSDKLiveTranscriptionOperationType nativeErrorTypeHelp::ZNSDKLiveTranscriptionOperationTypeMap(ZoomSDKLiveTranscriptionOperationType &messageType)
{
    ZNSDKLiveTranscriptionOperationType ret = ZNSDK_LiveTranscription_OperationType_None;
    switch (messageType) {
        case ZoomSDK_LiveTranscription_OperationType_Add:
            ret = ZNSDK_LiveTranscription_OperationType_Add;
            break;
        case ZoomSDK_LiveTranscription_OperationType_Update:
            ret = ZNSDK_LiveTranscription_OperationType_Update;
            break;
        case ZoomSDK_LiveTranscription_OperationType_Delete:
            ret = ZNSDK_LiveTranscription_OperationType_Delete;
            break;
        case ZoomSDK_LiveTranscription_OperationType_Complete:
            ret = ZNSDK_LiveTranscription_OperationType_Complete;
            break;
        case ZoomSDK_LiveTranscription_OperationType_NotSupported:
            ret = ZNSDK_LiveTranscription_OperationType_NotSupported;
            break;
        default:
            break;
    }
    return ret;
}

ZNSDKPollingStatus nativeErrorTypeHelp::ZNSDKPollingStatusMap(ZoomSDKPollingStatus &status)
{
    ZNSDKPollingStatus ret = ZNSDKPollingStatus_Initial;
    switch (status) {
        case ZoomSDKPollingStatus_Started:
            ret = ZNSDKPollingStatus_Started;
            break;
        case ZoomSDKPollingStatus_ShareResult:
            ret = ZNSDKPollingStatus_ShareResult;
            break;
        case ZoomSDKPollingStatus_Stopped:
            ret = ZNSDKPollingStatus_Stopped;
            break;
        default:
            break;
    }
    return ret;
}

ZNSDKPollingQuestionType nativeErrorTypeHelp::ZNSDKPollingQuestionTypeMap(ZoomSDKPollingQuestionType &questionType)
{
    ZNSDKPollingQuestionType ret = ZNSDKPollingQuestionType_Unknown;
    switch (questionType) {
        case ZoomSDKPollingQuestionType_Single:
            ret = ZNDKPollingQuestionType_Single;
            break;
        case ZoomSDKPollingQuestionType_Multi:
            ret = ZNSDKPollingQuestionType_Multi;
            break;
        case ZoomSDKPollingQuestionType_Matching:
            ret = ZNSDKPollingQuestionType_Matching;
            break;
        case ZoomSDKPollingQuestionType_RankOrder:
            ret = ZNSDKPollingQuestionType_RankOrder;
            break;
        case ZoomSDKPollingQuestionType_ShortAnswer:
            ret = ZNSDKPollingQuestionType_ShortAnswer;
            break;
        case ZoomSDKPollingQuestionType_LongAnswer:
            ret = ZNSDKPollingQuestionType_LongAnswer;
            break;
        case ZoomSDKPollingQuestionType_FillBlank:
            ret = ZNSDKPollingQuestionType_FillBlank;
            break;
        case ZoomSDKPollingQuestionType_NPS:
            ret = ZNSDKPollingQuestionType_NPS;
            break;
        case ZoomSDKPollingQuestionType_Dropdown:
            ret =     ZNSDKPollingQuestionType_Dropdown;
            break;
        default:
            break;
    }
    return ret;
}

ZNSDKPollingType nativeErrorTypeHelp::ZNSDKPollingTypeMap(ZoomSDKPollingType &pollingType)
{
    ZNSDKPollingType ret = ZNSDKPollingType_Unknown;
    switch (pollingType) {
        case ZoomSDKPollingType_Poll:
            ret = ZNSDKPollingType_Poll;
            break;
        case ZoomSDKPollingType_Quiz:
            ret = ZNSDKPollingType_Quiz;
            break;
        default:
            break;
    }
    return ret;
}

ZNSDKPollingActionType nativeErrorTypeHelp::ZNSDKPollingActionTypeMap(ZoomSDKPollingActionType actionType)
{
    ZNSDKPollingActionType ret = ZNSDKPollingActionType_Unknown;
    switch (actionType) {
        case ZoomSDKPollingActionType_Start:
            ret = ZNSDKPollingActionType_Start;
            break;
        case ZoomSDKPollingActionType_Stop:
            ret = ZNSDKPollingActionType_Stop;
            break;
        case ZoomSDKPollingActionType_ShareResult:
            ret = ZNSDKPollingActionType_ShareResult;
            break;
        case ZoomSDKPollingActionType_StopShareResult:
            ret = ZNSDKPollingActionType_StopShareResult;
            break;
        case ZoomSDKPollingActionType_Duplicate:
            ret = ZNSDKPollingActionType_Duplicate;
            break;
        case ZoomSDKPollingActionType_Delete:
            ret = ZNSDKPollingActionType_Delete;
            break;
        case ZoomSDKPollingActionType_Submit:
            ret = ZNSDKPollingActionType_Submit;
            break;
        case ZoomSDKPollingActionType_Error:
            ret = ZNSDKPollingActionType_Error;
            break;
        default:
            break;
    }
    return ret;
}

ZNCannotShareReasonType nativeErrorTypeHelp::ZNCannotShareReasonTypeMap(ZoomSDKCannotShareReasonType &reason) {
    ZNCannotShareReasonType zn_reason = ZNCannotShareReasonType_None;
    switch (reason)
    {
        case ZoomSDKCannotShareReasonType_None:
            zn_reason = ZNCannotShareReasonType_None;
            break;
        case ZoomSDKCannotShareReasonType_Locked:
            zn_reason = ZNCannotShareReasonType_Locked;
            break;
        case ZoomSDKCannotShareReasonType_Disabled:
            zn_reason = ZNCannotShareReasonType_Disabled;
            break;
        case ZoomSDKCannotShareReasonType_Other_Screen_Sharing:
            zn_reason = ZNCannotShareReasonType_Other_Screen_Sharing;
            break;
        case ZoomSDKCannotShareReasonType_Other_WB_Sharing:
            zn_reason = ZNCannotShareReasonType_Other_WB_Sharing;
            break;
        case ZoomSDKCannotShareReasonType_Need_Grab_Myself_Screen_Sharing:
            zn_reason = ZNCannotShareReasonType_Need_Grab_Myself_Screen_Sharing;
            break;
        case ZoomSDKCannotShareReasonType_Need_Grab_Other_Screen_Sharing:
            zn_reason = ZNCannotShareReasonType_Need_Grab_Other_Screen_Sharing;
            break;
        case ZoomSDKCannotShareReasonType_Need_Grab_Audio_Sharing:
            zn_reason = ZNCannotShareReasonType_Need_Grab_Audio_Sharing;
            break;
        case ZoomSDKCannotShareReasonType_Need_Grap_WB_Sharing:
            zn_reason = ZNCannotShareReasonType_Need_Grap_WB_Sharing;
            break;
        case ZoomSDKCannotShareReasonType_Reach_Maximum:
            zn_reason = ZNCannotShareReasonType_Reach_Maximum;
            break;
        case ZoomSDKCannotShareReasonType_Have_Share_From_Mainsession:
            zn_reason = ZNCannotShareReasonType_Have_Share_From_Mainsession;
            break;
        case ZoomSDKCannotShareReasonType_Other_Docs_Sharing:
            zn_reason = ZNCannotShareReasonType_Other_DOCS_Sharing;
            break;
        case ZoomSDKCannotShareReasonType_Need_Grab_Docs_Sharing:
            zn_reason = ZNCannotShareReasonType_Need_Grab_DOCS_Sharing;
            break;
        case ZoomSDKCannotShareReasonType_UnKnown:
            zn_reason = ZNCannotShareReasonType_UnKnown;
            break;
       default:
            break;
    }
    return zn_reason;
}

ZoomSDKCannotShareReasonType nativeErrorTypeHelp::ZoomSDKCannotShareReasonTypeMap(ZNCannotShareReasonType &zn_reason) {
    ZoomSDKCannotShareReasonType reason = ZoomSDKCannotShareReasonType_None;
    switch (zn_reason)
    {
        case ZNCannotShareReasonType_None:
            reason = ZoomSDKCannotShareReasonType_None;
            break;
        case ZNCannotShareReasonType_Locked:
            reason = ZoomSDKCannotShareReasonType_Locked;
            break;
        case ZNCannotShareReasonType_Disabled:
            reason = ZoomSDKCannotShareReasonType_Disabled;
            break;
        case ZNCannotShareReasonType_Other_Screen_Sharing:
            reason = ZoomSDKCannotShareReasonType_Other_Screen_Sharing;
            break;
        case ZNCannotShareReasonType_Other_WB_Sharing:
            reason = ZoomSDKCannotShareReasonType_Other_WB_Sharing;
            break;
        case ZNCannotShareReasonType_Need_Grab_Myself_Screen_Sharing:
            reason = ZoomSDKCannotShareReasonType_Need_Grab_Myself_Screen_Sharing;
            break;
        case ZNCannotShareReasonType_Need_Grab_Other_Screen_Sharing:
            reason = ZoomSDKCannotShareReasonType_Need_Grab_Other_Screen_Sharing;
            break;
        case ZNCannotShareReasonType_Need_Grab_Audio_Sharing:
            reason = ZoomSDKCannotShareReasonType_Need_Grab_Audio_Sharing;
            break;
        case ZNCannotShareReasonType_Need_Grap_WB_Sharing:
            reason = ZoomSDKCannotShareReasonType_Need_Grap_WB_Sharing;
            break;
        case ZNCannotShareReasonType_Reach_Maximum:
            reason = ZoomSDKCannotShareReasonType_Reach_Maximum;
            break;
        case ZNCannotShareReasonType_Have_Share_From_Mainsession:
            reason = ZoomSDKCannotShareReasonType_Have_Share_From_Mainsession;
            break;
        case ZNCannotShareReasonType_Other_DOCS_Sharing:
            reason = ZoomSDKCannotShareReasonType_Other_Docs_Sharing;
            break;
        case ZNCannotShareReasonType_Need_Grab_DOCS_Sharing:
            reason = ZoomSDKCannotShareReasonType_Need_Grab_Docs_Sharing;
            break;
        case ZNCannotShareReasonType_UnKnown:
            reason = ZoomSDKCannotShareReasonType_UnKnown;
            break;
       default:
            break;
    }
    return reason;
}

ZNSDKWhiteboardStatus nativeErrorTypeHelp::ZNSDKWhiteboardStatusMap(ZoomSDKWhiteboardStatus &status)
{
    ZNSDKWhiteboardStatus zn_status = ZNSDKWhiteboardStatus_Started;
    switch (status)
    {
        case ZoomSDKWhiteboardStatus_Started:
            zn_status = ZNSDKWhiteboardStatus_Started;
            break;
        case ZoomSDKWhiteboardStatus_Stopped:
            zn_status = ZNSDKWhiteboardStatus_Stopped;
            break;
        default:
            break;
    }
    return zn_status;
}

ZNSDKWhiteboardShareOption nativeErrorTypeHelp::ZNSDKWhiteboardShareOptionMap(ZoomSDKWhiteboardShareOption &option) {
    ZNSDKWhiteboardShareOption zn_option = ZNSDKWhiteboardShareOption_HostShare;
    switch (option)
    {
       case ZoomSDKWhiteboardShareOption_HostShare:
            zn_option = ZNSDKWhiteboardShareOption_HostShare;
            break;
       case ZoomSDKWhiteboardShareOption_HostGrabShare:
            zn_option = ZNSDKWhiteboardShareOption_HostGrabShare;
            break;
       case ZoomSDKWhiteboardShareOption_AllGrabShare:
            zn_option = ZNSDKWhiteboardShareOption_AllGrabShare;
            break;
       default: break;
    }
    return zn_option;
}

ZoomSDKWhiteboardShareOption nativeErrorTypeHelp::ZoomSDKWhiteboardShareOptionMap(ZNSDKWhiteboardShareOption &zn_option) {
    ZoomSDKWhiteboardShareOption option = ZoomSDKWhiteboardShareOption_HostShare;
    switch (zn_option)
    {
       case ZNSDKWhiteboardShareOption_HostShare:
            option = ZoomSDKWhiteboardShareOption_HostShare;
            break;
       case ZNSDKWhiteboardShareOption_HostGrabShare:
            option = ZoomSDKWhiteboardShareOption_HostGrabShare;
            break;
       case ZNSDKWhiteboardShareOption_AllGrabShare:
            option = ZoomSDKWhiteboardShareOption_AllGrabShare;
            break;
       default: break;
    }
    return option;
}

ZNSDKWhiteboardCreateOption nativeErrorTypeHelp::ZNSDKWhiteboardCreateOptionMap(ZoomSDKWhiteboardCreateOption &option) {
    ZNSDKWhiteboardCreateOption zn_option = ZNSDKWhiteboardCreateOption_HostOnly;
    switch (option)
    {
        case ZoomSDKWhiteboardCreateOption_HostOnly:
            zn_option = ZNSDKWhiteboardCreateOption_HostOnly;
            break;
        case ZoomSDKWhiteboardCreateOption_AccountUsers:
            zn_option = ZNSDKWhiteboardCreateOption_AccountUsers;
            break;
        case ZoomSDKWhiteboardCreateOption_All:
            zn_option = ZNSDKWhiteboardCreateOption_All;
            break;
       default:
            break;
    }
    return zn_option;
}

ZoomSDKWhiteboardCreateOption nativeErrorTypeHelp::ZoomSDKWhiteboardCreateOptionMap(ZNSDKWhiteboardCreateOption &zn_option) {
    ZoomSDKWhiteboardCreateOption option = ZoomSDKWhiteboardCreateOption_HostOnly;
    switch (zn_option)
    {
        case ZNSDKWhiteboardCreateOption_HostOnly:
            option = ZoomSDKWhiteboardCreateOption_HostOnly;
            break;
        case ZNSDKWhiteboardCreateOption_AccountUsers:
            option = ZoomSDKWhiteboardCreateOption_AccountUsers;
            break;
        case ZNSDKWhiteboardCreateOption_All:
            option = ZoomSDKWhiteboardCreateOption_All;
            break;
       default:
            break;
    }
    return option;
}

ZNSDKDocsStatus nativeErrorTypeHelp::ZNSDKDocsStatusMap(ZoomSDKDocsStatus &option) {
    ZNSDKDocsStatus zn_option = ZNSDKDocsStatus_None;
    switch (option)
    {
        case ZoomSDKDocsStatus_None:
            zn_option = ZNSDKDocsStatus_None;
            break;
        case ZoomSDKDocsStatus_Start:
            zn_option = ZNSDKDocsStatus_Start;
            break;
        case ZoomSDKDocsStatus_Stop:
            zn_option = ZNSDKDocsStatus_Stop;
            break;
       default:
            break;
    }
    return zn_option;
}

ZoomSDKDocsStatus nativeErrorTypeHelp::ZoomSDKDocsStatusMap(ZNSDKDocsStatus &zn_option) {
    ZoomSDKDocsStatus option = ZoomSDKDocsStatus_None;
    switch (zn_option)
    {
        case ZNSDKDocsStatus_None:
            option = ZoomSDKDocsStatus_None;
            break;
        case ZNSDKDocsStatus_Start:
            option = ZoomSDKDocsStatus_Start;
            break;
        case ZNSDKDocsStatus_Stop:
            option = ZoomSDKDocsStatus_Stop;
            break;
       default:
            break;
    }
    return option;
}

ZNSDKDocsShareOption nativeErrorTypeHelp::ZNSDKDocsShareOptionMap(ZoomSDKDocsShareOption &option) {
    ZNSDKDocsShareOption zn_option = ZNSDKDocsShareOption_None;
    switch (option)
    {
        case ZoomSDKDocsShareOption_None:
            zn_option = ZNSDKDocsShareOption_None;
            break;
        case ZoomSDKDocsShareOption_HostCoHostShare:
            zn_option = ZNSDKDocsShareOption_HostShare;
            break;
        case ZoomSDKDocsShareOption_HostCoHostGrabShare:
            zn_option = ZNSDKDocsShareOption_HostGrabShare;
            break;
        case ZoomSDKDocsShareOption_AllGrabShare:
            zn_option = ZNSDKDocsShareOption_AllGrabShare;
            break;
       default:
            break;
    }
    return zn_option;
}

ZoomSDKDocsShareOption nativeErrorTypeHelp::ZoomSDKDocsShareOptionMap(ZNSDKDocsShareOption &zn_option) {
    ZoomSDKDocsShareOption option = ZoomSDKDocsShareOption_None;
    switch (zn_option)
    {
        case ZNSDKDocsShareOption_None:
            option = ZoomSDKDocsShareOption_None;
            break;
        case ZNSDKDocsShareOption_HostShare:
            option = ZoomSDKDocsShareOption_HostCoHostShare;
            break;
        case ZNSDKDocsShareOption_HostGrabShare:
            option = ZoomSDKDocsShareOption_HostCoHostGrabShare;
            break;
        case ZNSDKDocsShareOption_AllGrabShare:
            option = ZoomSDKDocsShareOption_AllGrabShare;
            break;
       default:
            break;
    }
    return option;
}

ZNSDKDocsCreateOption nativeErrorTypeHelp::ZNSDKDocsCreateOptionMap(ZoomSDKDocsCreateOption &option) {
    ZNSDKDocsCreateOption zn_option = ZNSDKDocsCreateOption_None;
    switch (option)
    {
        case ZoomSDKDocsCreateOption_None:
            zn_option = ZNSDKDocsCreateOption_None;
            break;
        case ZoomSDKDocsCreateOption_HostOnly:
            zn_option = ZNSDKDocsCreateOption_HostOnly;
            break;
        case ZoomSDKDocsCreateOption_AccountUsers:
            zn_option = ZNSDKDocsCreateOption_AccountUsers;
            break;
        case ZoomSDKDocsCreateOption_All:
            zn_option = ZNSDKDocsCreateOption_All;
            break;
       default:
            break;
    }
    return zn_option;
}

ZoomSDKDocsCreateOption nativeErrorTypeHelp::ZoomSDKDocsCreateOptionMap(ZNSDKDocsCreateOption &zn_option) {
    ZoomSDKDocsCreateOption option = ZoomSDKDocsCreateOption_None;
    switch (zn_option)
    {
        case ZNSDKDocsCreateOption_None:
            option = ZoomSDKDocsCreateOption_None;
            break;
        case ZNSDKDocsCreateOption_HostOnly:
            option = ZoomSDKDocsCreateOption_HostOnly;
            break;
        case ZNSDKDocsCreateOption_AccountUsers:
            option = ZoomSDKDocsCreateOption_AccountUsers;
            break;
        case ZNSDKDocsCreateOption_All:
            option = ZoomSDKDocsCreateOption_All;
            break;
       default:
            break;
    }
    return option;
}

ZNUserInfomation nativeErrorTypeHelp::ConvertSDKUserInfoToZnUserInfo(ZoomSDKUserInfo *userInfo)
{
    ZNUserInfomation zn_userInfo;
    if (userInfo) {
        NSString *userName = [userInfo getUserName];
        zn_userInfo.userName = userName ? [userName UTF8String] : "";
        zn_userInfo.isHost = [userInfo isHost];
        zn_userInfo.userID = [userInfo getUserID];
        
        NSString *avatarPath = [userInfo getAvatarPath];
        zn_userInfo.avatarPath = avatarPath ? [avatarPath UTF8String] : "";
        
        NSString *persistentId = [userInfo getPersistentId];
        zn_userInfo.persistentId = persistentId ? [persistentId UTF8String] : "";
        
        zn_userInfo.isVideoOn = [userInfo isVideoOn];

        ZoomSDKAudioStatus audioStatus = [userInfo getAudioStatus];
        zn_userInfo.isAudioMuted = (audioStatus == ZoomSDKAudioStatus_Muted ? YES : NO);
        zn_userInfo.isMySelf = [userInfo isMySelf];

        UserRole roles = [userInfo getUserRole];
        zn_userInfo.userRole = ZNSDKUserRole(roles);

        zn_userInfo.isPurePhoneUser = [userInfo isPurePhoneUser];
        zn_userInfo.isTalking = [userInfo isTalking];
        zn_userInfo.isH323User = [userInfo isH323User];
        ZoomSDKWebinarAttendeeStatus *webinar = [userInfo GetWebinarAttendeeStatus];
        if (webinar)
        {
            zn_userInfo.webinarAttendeeStatus.allow_talk = webinar.isAttendeeCanTalk;
        }
       
        ZoomSDKRecordingStatus recordingStatus = [userInfo getLocalRecordingStatus];
        zn_userInfo.localRecordingStatus = ZNRecordingStatusChange(recordingStatus);
        zn_userInfo.userInfoType = ZN_REAL_USERINFO;
        NSString *participantID = [userInfo getCustomerKey];
        zn_userInfo.customerKey = participantID ? [participantID UTF8String] : "";

        zn_userInfo.isRawLiveStreaming = [userInfo isRawLiveStreaming];
        zn_userInfo.HasRawLiveStreamPrivilege = [userInfo hasRawLiveStreamPrivilege];

        ZoomSDKAudioType audioType = [userInfo getAudioType];
        zn_userInfo.audioJoinType = ZNAudioTypeMap(audioType);

        zn_userInfo.isRaiseHand = [userInfo isRaisingHand];
        zn_userInfo.isInWaitingRoom = [userInfo isInWaitingRoom];
        zn_userInfo.isClosedCaptionSender = [userInfo isClosedCaptionSender];
        zn_userInfo.isBotUser = [userInfo isBotUser];
        NSString *botAppName = [userInfo getBotAppName];
        zn_userInfo.botAppName = botAppName ? [botAppName UTF8String] : "";
        zn_userInfo.isInWebinarBackstage = [userInfo isInWebinarBackstage];
        zn_userInfo.isInterpreter = [userInfo isInterpreter];
        zn_userInfo.isSignLanguageInterpreter = [userInfo isSignLanguageInterpreter];
        NSString *interpreterActiveLanguage = [userInfo getInterpreterActiveLanguage];
        zn_userInfo.interpreterActiveLanguage = interpreterActiveLanguage ? [interpreterActiveLanguage UTF8String] : "";
        ZoomSDKEmojiFeedbackType feedbackType = [userInfo getEmojiFeedbackType];
        zn_userInfo.emojiFeedbackType = ZNSDKEmojiFeedbackTypeMap(feedbackType);
    }
    return zn_userInfo;
}

ZNWallpaperItem nativeErrorTypeHelp::ConvertSDKIWallpaperItemToZNWallpaperItem(ZoomSDKWallpaperItem *wallpaperItem)
{
    ZNWallpaperItem zn_wallpaperItem;
    if (wallpaperItem) {
        NSString *wallpaperId = wallpaperItem.wallpaperId;
        zn_wallpaperItem.wallpaperId = wallpaperId ? [wallpaperId UTF8String] : "";

        NSString *title = wallpaperItem.title;
        zn_wallpaperItem.title = title ? [title UTF8String] : "";

        NSString *thumbnailPath = wallpaperItem.thumbnailPath;
        zn_wallpaperItem.thumbnailPath = thumbnailPath ? [thumbnailPath UTF8String] : "";

        NSString *path = wallpaperItem.path;
        zn_wallpaperItem.path = path ? [path UTF8String] : "";

        zn_wallpaperItem.transparency = wallpaperItem.transparency;

        ZoomSDKWallpaperLayoutMode layoutMode = wallpaperItem.layoutMode;
        zn_wallpaperItem.wallpaperLayoutMode = ZNSDKWallpaperLayoutModeMap(layoutMode);
    }
    return zn_wallpaperItem;
}

ZNZoomSDKSharingSourceInfo nativeErrorTypeHelp::ConvertSDKSharingSourceInfoToZNSharingSourceInfo(ZoomSDKSharingSourceInfo *shareInfo)
{
    ZNZoomSDKSharingSourceInfo zn_shareInfo;
    if (shareInfo) {
        zn_shareInfo.userid = shareInfo.userID;
        zn_shareInfo.shareSourceID = shareInfo.shareSourceID;

        zn_shareInfo.windowID = shareInfo.windowID;
        zn_shareInfo.monitorID = shareInfo.displayID;

        zn_shareInfo.isShowingInFirstView = shareInfo.isShowInFirstScreen;
        zn_shareInfo.isShowingInSecondView = shareInfo.isShowInSecondScreen;
        zn_shareInfo.isCanBeRemoteControl = shareInfo.canBeRemoteControl;
        zn_shareInfo.bEnableOptimizingVideoSharing = shareInfo.bEnableOptimizingVideoSharing;
        
        ZoomSDKShareStatus status = shareInfo.status;
        ZNShareStatus zn_status = ZNSDKShareStatus(status);
        zn_shareInfo.status = zn_status;

        ZoomSDKShareContentType contentType = shareInfo.contentType;
        ZNShareType zn_type = ZNSDKShareType(contentType);
        zn_shareInfo.contentType = zn_type;
    }
    return zn_shareInfo;
}

ZNChatMsgInfo nativeErrorTypeHelp::ConvertSDKChatMsgInfoToZNChatMsgInfo(ZoomSDKChatInfo *chatInfo)
{
    ZNChatMsgInfo zn_chatInfo;
    if (chatInfo) {
        ZoomSTRING zn_messageID;
        NSString *messageID = [chatInfo getMessageID];
        if (messageID)
            zn_messageID = [messageID UTF8String];
        zn_chatInfo.messageID = zn_messageID;
        zn_chatInfo.senderUserId = [chatInfo getSenderUserID];

        ZoomSTRING zn_senderDisplayName;
        NSString *senderDisplayName = [chatInfo getSenderDisplayName];
        if (senderDisplayName)
            zn_senderDisplayName = [senderDisplayName UTF8String];
        zn_chatInfo.senderDisplayName = zn_senderDisplayName;
        zn_chatInfo.receiverUserId = [chatInfo getReceiverUserID];

        ZoomSTRING zn_receiverDisplayName;
        NSString *receiverDisplayName = [chatInfo getReceiverDisplayName];
        if (receiverDisplayName)
            zn_receiverDisplayName = [receiverDisplayName UTF8String];
        zn_chatInfo.receiverDisplayName = zn_receiverDisplayName;

        ZoomSTRING zn_msgContent;
        NSString *msgContent = [chatInfo getMsgContent];
        if (msgContent)
            zn_msgContent = [msgContent UTF8String];
        zn_chatInfo.content = zn_msgContent;
        zn_chatInfo.timeStamp = [chatInfo getTimeStamp];
        zn_chatInfo.isChatToWaitingroom = chatInfo.isChatToWaitingRoom;

        nativeErrorTypeHelp Help_type;
        ZoomSDKChatMessageType chatMessageType = [chatInfo getChatMessageType];
        ZNSDKChatMessageType zn_chatMessageType = Help_type.ZNSDKChatMessageTypeMap(chatMessageType);
        zn_chatInfo.chatMessageType = zn_chatMessageType;

        if (chatMessageType == ZoomSDKChatMessageType_To_All)
            zn_chatInfo.isChatToAll = true;

        if (chatMessageType == ZoomSDKChatMessageType_To_All_Panelist)
            zn_chatInfo.isChatToAllPanelist = true;

        zn_chatInfo.isComment = chatInfo.isComment;
        zn_chatInfo.isThread = chatInfo.isThread;

        ZoomSTRING zn_threadID;
        NSString *threadID = [chatInfo getThreadID];
        if (threadID)
            zn_threadID = [threadID UTF8String];
        zn_chatInfo.threadID = zn_threadID;
        NSArray<ZoomSDKRichTextStyleItem *> * plstRichTextStyleItem = [chatInfo getTextStyleItemList];
        ZNList<ZNRichTextStyleItem> text_style_item_list;
        for (ZoomSDKRichTextStyleItem *pRichTextStyleItem in plstRichTextStyleItem)
        {
            ZNRichTextStyleItem zn_rich_text_style_item;
            if (pRichTextStyleItem)
            {
                zn_rich_text_style_item.textStyle = (ZNRichTextStyle)pRichTextStyleItem.textStyle;
                NSArray<ZoomSDKRichTextStyleOffset *> * plstTextStyleOffset = pRichTextStyleItem.getTextStyleOffsetList;
                ZNList<ZNRichTextStyleOffset> rich_text_style_offset_list;
                for (ZoomSDKRichTextStyleOffset *pTextStyleOffset in plstTextStyleOffset)
                {
                    ZNRichTextStyleOffset zn_rich_text_style_offset;
                    if (pTextStyleOffset)
                    {
                        zn_rich_text_style_offset.positionStart = pTextStyleOffset.positionStart;
                        zn_rich_text_style_offset.positionEnd = pTextStyleOffset.positionEnd;

                        ZoomSTRING zn_reserve;
                        NSString *reserve = pTextStyleOffset.reserve;
                        if (reserve)
                            zn_reserve = [reserve UTF8String];
                        zn_rich_text_style_offset.reserve = zn_reserve;
                    }
                    rich_text_style_offset_list.push_back(zn_rich_text_style_offset);
                }
                zn_rich_text_style_item.textStyleOffsetList = rich_text_style_offset_list;
            }
            text_style_item_list.push_back(zn_rich_text_style_item);
        }
        zn_chatInfo.textStyleItemList = text_style_item_list;
    }
    return zn_chatInfo;
}

ZNVirtualBGImageInfo nativeErrorTypeHelp::ConvertSDKVirtualBGImageInfoToZNVirtualBGImageInfo(ZoomSDKVirtualBGImageInfo *imageInfo)
{
    ZNVirtualBGImageInfo zn_virtualBGImageInfo;
    if (imageInfo) {
        NSString *imageFilePath = [imageInfo getImageFilePath];
        zn_virtualBGImageInfo.imageFilePath = imageFilePath ? [imageFilePath UTF8String] : "";

        NSString *imageName = [imageInfo getImageName];
        zn_virtualBGImageInfo.imageName = imageName ? [imageName UTF8String] : "";

        zn_virtualBGImageInfo.isSelected = [imageInfo isSelected];
        zn_virtualBGImageInfo.isAllowDelete = [imageInfo isAllowDelete];
        zn_virtualBGImageInfo.imageHandle = (int64_t)imageInfo;
    }
    return zn_virtualBGImageInfo;
}

ZNZoomSDKDocSharingSourceInfo nativeErrorTypeHelp::ConvertSDKDocsSharingSourceInfoToZNDocsSharingSourceInfo(ZoomSDKDocsSharingSourceInfo *sourceInfo)
{
    ZNZoomSDKDocSharingSourceInfo zn_sourceInfo;
    if (sourceInfo) {
        zn_sourceInfo.shareSourceID = sourceInfo.shareSourceID;
        zn_sourceInfo.userID = sourceInfo.userID;

        ZoomSDKDocsStatus status = sourceInfo.status;
        zn_sourceInfo.status = ZNSDKDocsStatusMap(status);

        NSString *docTitle = sourceInfo.docTitle;
        zn_sourceInfo.docTitle = docTitle ? [docTitle UTF8String] : "";
    }
    return zn_sourceInfo;
}