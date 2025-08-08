#include "zoom_native_to_wrap.h"
#include <locale.h>
ZNSDKError Map2WrapDefine(ZOOM_SDK_NAMESPACE::SDKError err)
{
	ZNSDKError zn_err_(ZNSDKERR_UNKNOWN);
	switch (err)
	{
	case ZOOM_SDK_NAMESPACE::SDKERR_SUCCESS:
		zn_err_ = ZNSDKERR_SUCCESS;
		break;
	case ZOOM_SDK_NAMESPACE::SDKERR_NO_IMPL:
		zn_err_ = ZNSDKERR_NO_IMPL;
		break;
	case ZOOM_SDK_NAMESPACE::SDKERR_WRONG_USAGE:
		zn_err_ = ZNSDKERR_WRONG_USEAGE;
		break;
	case ZOOM_SDK_NAMESPACE::SDKERR_INVALID_PARAMETER:
		zn_err_ = ZNSDKERR_INVALID_PARAMETER;
		break;
	case ZOOM_SDK_NAMESPACE::SDKERR_MODULE_LOAD_FAILED:
		zn_err_ = ZNSDKERR_MODULE_LOAD_FAILED;
		break;
	case ZOOM_SDK_NAMESPACE::SDKERR_MEMORY_FAILED:
		zn_err_ = ZNSDKERR_MEMORY_FAILED;
		break;
	case ZOOM_SDK_NAMESPACE::SDKERR_SERVICE_FAILED:
		zn_err_ = ZNSDKERR_SERVICE_FAILED;
		break;
	case ZOOM_SDK_NAMESPACE::SDKERR_UNINITIALIZE:
		zn_err_ = ZNSDKERR_UNINITIALIZE;
		break;
	case ZOOM_SDK_NAMESPACE::SDKERR_UNAUTHENTICATION:
		zn_err_ = ZNSDKERR_UNAUTHENTICATION;
		break;
	case ZOOM_SDK_NAMESPACE::SDKERR_NORECORDINGINPROCESS:
		zn_err_ = ZNSDKERR_NORECORDINGINPROCESS;
		break;
	case ZOOM_SDK_NAMESPACE::SDKERR_TRANSCODER_NOFOUND:
		zn_err_ = ZNSDKERR_TRANSCODER_NOFOUND;
		break;	
	case ZOOM_SDK_NAMESPACE::SDKERR_VIDEO_NOTREADY:
		zn_err_ = ZNSDKERR_VIDEO_NOTREADY;
		break;
	case ZOOM_SDK_NAMESPACE::SDKERR_NO_PERMISSION:
		zn_err_ = ZNSDKERR_NO_PERMISSION;
		break;
	case ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN:
		zn_err_ = ZNSDKERR_UNKNOWN;
		break;
	case ZOOM_SDK_NAMESPACE::SDKERR_OTHER_SDK_INSTANCE_RUNNING:
		zn_err_ = ZNSDKERR_OTHER_SDK_INSTANCE_RUNNING;
		break;
		
	case ZOOM_SDK_NAMESPACE::SDKERR_INTERNAL_ERROR:	
		zn_err_ = ZNSDKERR_INTERNAL_ERROR;
		break;
	case ZOOM_SDK_NAMESPACE::SDKERR_NO_AUDIODEVICE_ISFOUND:
		zn_err_ = ZNSDKERR_NO_AUDIODEVICE_ISFOUND;
		break;
	case ZOOM_SDK_NAMESPACE::SDKERR_NO_VIDEODEVICE_ISFOUND:
		zn_err_ = ZNSDKERR_NO_VIDEODEVICE_ISFOUND;
		break;
	case ZOOM_SDK_NAMESPACE::SDKERR_TOO_FREQUENT_CALL:
		zn_err_ = ZNSDKERR_TOO_FREQUENT_CALL;
		break;
	case ZOOM_SDK_NAMESPACE::SDKERR_FAIL_ASSIGN_USER_PRIVILEGE:
		zn_err_ = ZNSDKERR_FAIL_ASSIGN_USER_PRIVILEGE;
		break;
	case ZOOM_SDK_NAMESPACE::SDKERR_MEETING_DONT_SUPPORT_FEATURE:
		zn_err_ = ZNSDKERR_MEETING_DONT_SUPPORT_FEATURE;
		break;
	case ZOOM_SDK_NAMESPACE::SDKERR_MEETING_NOT_SHARE_SENDER:
		zn_err_ = ZNSDKERR_MEETING_NOT_SHARE_SENDER;
		break;
	case ZOOM_SDK_NAMESPACE::SDKERR_MEETING_YOU_HAVE_NO_SHARE:
		zn_err_ = ZNSDKERR_MEETING_YOU_HAVE_NO_SHARE;
		break;
	case ZOOM_SDK_NAMESPACE::SDKERR_MEETING_VIEWTYPE_PARAMETER_IS_WRONG:
		zn_err_ = ZNSDKERR_MEETING_VIEWTYPE_PARAMETER_IS_WRONG;
		break;
	case ZOOM_SDK_NAMESPACE::SDKERR_MEETING_ANNOTATION_IS_OFF:	
		zn_err_ = ZNSDKERR_MEETING_ANNOTATION_IS_OFF;
		break;
	case ZOOM_SDK_NAMESPACE::SDKERR_SETTING_OS_DONT_SUPPORT:
		zn_err_ = ZNSDKERR_SETTING_OS_DONT_SUPPORT;
		break;
	case ZOOM_SDK_NAMESPACE::SDKERR_EMAIL_LOGIN_IS_DISABLED:
		zn_err_ = ZNSDKERR_EMAIL_LOGIN_IS_DISABLED;
		break;
	case ZOOM_SDK_NAMESPACE::SDKERR_HARDWARE_NOT_MEET_FOR_VB:
		zn_err_ = ZNSDKERR_HARDWARE_NOT_MEET_FOR_VB;
		break;
	case ZOOM_SDK_NAMESPACE::SDKERR_NEED_USER_CONFIRM_RECORD_DISCLAIMER:
		zn_err_ = ZNSDKERR_NEED_USER_CONFIRM_RECORD_DISCLAIMER;
		break;
	case ZOOM_SDK_NAMESPACE::SDKERR_NO_SHARE_DATA:
		zn_err_ = ZNSDKERR_NO_SHARE_DATA;
		break;
	case ZOOM_SDK_NAMESPACE::SDKERR_SHARE_CANNOT_SUBSCRIBE_MYSELF:
		zn_err_ = ZNSDKERR_SHARE_CANNOT_SUBSCRIBE_MYSELF;
		break;
	case ZOOM_SDK_NAMESPACE::SDKERR_NOT_IN_MEETING:
		zn_err_ = ZNSDKERR_NOT_IN_MEETING;
		break;
	case ZOOM_SDK_NAMESPACE::SDKERR_NOT_JOIN_AUDIO:
		zn_err_ = ZNSDKERR_NOT_JOIN_AUDIO;
		break;
	case ZOOM_SDK_NAMESPACE::SDKERR_HARDWARE_DONT_SUPPORT:
		zn_err_ = ZNSDKERR_HARDWARE_DONT_SUPPORT;
		break;
	case ZOOM_SDK_NAMESPACE::SDKERR_DOMAIN_DONT_SUPPORT:
		zn_err_ = ZNSDKERR_DOMAIN_DONT_SUPPORT;
		break;
	case ZOOM_SDK_NAMESPACE::SDKERR_MEETING_REMOTE_CONTROL_IS_OFF:
		zn_err_ = ZNSDKERR_MEETING_REMOTE_CONTROL_IS_OFF;
		break;
	case ZOOM_SDK_NAMESPACE::SDKERR_FILETRANSFER_ERROR:
		zn_err_ = ZNSDKERR_FILETRANSFER_ERROR;
		break;
	default:
		break;
	}
	return zn_err_;
}

ZNAuthResult Map2WrapDefine(ZOOM_SDK_NAMESPACE::AuthResult result)
{
	ZNAuthResult zn_auth_result_ = ZNAUTHRET_UNKNOWN;
	switch (result)
	{
	case ZOOMSDK::AUTHRET_SUCCESS:
		zn_auth_result_ = ZNAUTHRET_SUCCESS;
		break;
	case ZOOMSDK::AUTHRET_KEYORSECRETEMPTY:
		zn_auth_result_ = ZNAUTHRET_KEYORSECRETEMPTY;
		break;
	case ZOOMSDK::AUTHRET_KEYORSECRETWRONG:
		zn_auth_result_ = ZNAUTHRET_KEYORSECRETWRONG;
		break;
	case ZOOMSDK::AUTHRET_ACCOUNTNOTSUPPORT:
		zn_auth_result_ = ZNAUTHRET_ACCOUNTNOTSUPPORT;
		break;
	case ZOOMSDK::AUTHRET_ACCOUNTNOTENABLESDK:
		zn_auth_result_ = ZNAUTHRET_ACCOUNTNOTENABLESDK;
		break;
	case ZOOMSDK::AUTHRET_UNKNOWN:
		zn_auth_result_ = ZNAUTHRET_UNKNOWN;
		break;
	case ZOOMSDK::AUTHRET_SERVICE_BUSY:
		zn_auth_result_ = ZNAUTHRET_SERVICE_BUSY;
		break;
	case ZOOMSDK::AUTHRET_NONE:
		zn_auth_result_ = ZNAUTHRET_NONE;
		break;
	case ZOOMSDK::AUTHRET_OVERTIME:
		zn_auth_result_ = ZNAUTHRET_OVERTIME;
		break;
	case ZOOMSDK::AUTHRET_NETWORKISSUE:
		zn_auth_result_ = ZNAUTHRET_NETWORKISSUE;
		break;
	case ZOOMSDK::AUTHRET_CLIENT_INCOMPATIBLE:
		zn_auth_result_ = ZNAUTHRET_CLIENT_INCOMPATIBLE;
		break;
	case ZOOMSDK::AUTHRET_JWTTOKENWRONG:
		zn_auth_result_ = ZNAUTHRET_JWTTOKENWRONG;
		break;
	case ZOOMSDK::AUTHRET_LIMIT_EXCEEDED_EXCEPTION:
		zn_auth_result_ = ZNAUTHRET_LIMIT_EXCEEDED_EXCEPTION;
		break;
	default:
		break;
	}
	
	return zn_auth_result_;
}
ZNLOGINSTATUS Map2WrapDefine(ZOOM_SDK_NAMESPACE::LOGINSTATUS status)
{
	ZNLOGINSTATUS zn_login_status;
	switch (status)
	{
	case ZOOM_SDK_NAMESPACE::LOGIN_IDLE:
		zn_login_status = ZNLOGIN_IDLE;
		break;
	case ZOOM_SDK_NAMESPACE::LOGIN_PROCESSING:
		zn_login_status = ZNLOGIN_PROCESSING;
		break;
	case ZOOM_SDK_NAMESPACE::LOGIN_SUCCESS:
		zn_login_status = ZNLOGIN_SUCCESS;
		break;
	case ZOOM_SDK_NAMESPACE::LOGIN_FAILED:
		zn_login_status = ZNLOGIN_FAILED;
		break;
	default:
		break;
	}
	return zn_login_status;
}
ZNAccountInfo Map2WrapDefine(ZOOM_SDK_NAMESPACE::IAccountInfo* pAccountInfo)
{
	ZNAccountInfo zn_account_info;
	zn_account_info.dispaly_name = pAccountInfo->GetDisplayName();
	return zn_account_info;
}
ZNMeetingStatus Map2WrapDefine(ZOOM_SDK_NAMESPACE::MeetingStatus status)
{
	ZNMeetingStatus zn_meeting_status;
	switch (status)
	{
	case ZOOMSDK::MEETING_STATUS_IDLE:
		zn_meeting_status = ZNMEETING_STATUS_IDLE;
		break;
	case ZOOMSDK::MEETING_STATUS_CONNECTING:
		zn_meeting_status = ZNMEETING_STATUS_CONNECTING;
		break;
	case ZOOMSDK::MEETING_STATUS_WAITINGFORHOST:
		zn_meeting_status = ZNMEETING_STATUS_WAITINGFORHOST;
		break;
	case ZOOMSDK::MEETING_STATUS_INMEETING:
		zn_meeting_status = ZNMEETING_STATUS_INMEETING;
		break;
	case ZOOMSDK::MEETING_STATUS_DISCONNECTING:
		zn_meeting_status = ZNMEETING_STATUS_DISCONNECTING;
		break;
	case ZOOMSDK::MEETING_STATUS_RECONNECTING:
		zn_meeting_status = ZNMEETING_STATUS_RECONNECTING;
		break;
	case ZOOMSDK::MEETING_STATUS_FAILED:
		zn_meeting_status = ZNMEETING_STATUS_FAILED;
		break;
	case ZOOMSDK::MEETING_STATUS_ENDED:
		zn_meeting_status = ZNMEETING_STATUS_ENDED;
		break;
	case ZOOMSDK::MEETING_STATUS_UNKNOWN:
		zn_meeting_status = ZNMEETING_STATUS_UNKNOWN;
		break;
	case ZOOMSDK::MEETING_STATUS_LOCKED:
		zn_meeting_status = ZNMEETING_STATUS_LOCKED;
		break;
	case ZOOMSDK::MEETING_STATUS_UNLOCKED:
		zn_meeting_status = ZNMEETING_STATUS_UNLOCKED;
		break;
	case ZOOMSDK::MEETING_STATUS_IN_WAITING_ROOM:
		zn_meeting_status = ZNMEETING_STATUS_IN_WAITING_ROOM;
		break;
	case ZOOMSDK::MEETING_STATUS_WEBINAR_PROMOTE:
		zn_meeting_status = ZNMEETING_STATUS_WEBINAR_PROMOTE;
		break;
	case ZOOMSDK::MEETING_STATUS_WEBINAR_DEPROMOTE:
		zn_meeting_status = ZNMEETING_STATUS_WEBINAR_DEPROMOTE;
		break;
	case ZOOMSDK::MEETING_STATUS_JOIN_BREAKOUT_ROOM:
		zn_meeting_status = ZNMEETING_STATUS_JOIN_BREAKOUT_ROOM;
		break;
	case ZOOMSDK::MEETING_STATUS_LEAVE_BREAKOUT_ROOM:
		zn_meeting_status = ZNMEETING_STATUS_LEAVE_BREAKOUT_ROOM;
		break;
	default:
		break;
	}
	return zn_meeting_status;
}
ZNMeetingType Map2WrapDefine(ZOOM_SDK_NAMESPACE::MeetingType type)
{
	ZNMeetingType zn_meeting_type;
	switch (type)
	{
	case ZOOMSDK::MEETING_TYPE_NONE:
		zn_meeting_type = ZNMEETING_TYPE_NONE;
		break;
	case ZOOMSDK::MEETING_TYPE_NORMAL:
		zn_meeting_type = ZNMEETING_TYPE_NORMAL;
		break;
	case ZOOMSDK::MEETING_TYPE_WEBINAR:
		zn_meeting_type = ZNMEETING_TYPE_WEBINAR;
		break;
	case ZOOMSDK::MEETING_TYPE_BREAKOUTROOM:
		zn_meeting_type = ZNMEETING_TYPE_BREAKOUTROOM;
		break;
	default:
		break;
	}
	return zn_meeting_type;
}
ZNConnectionQuality Map2WrapDefine(ZOOM_SDK_NAMESPACE::ConnectionQuality quality)
{
	ZNConnectionQuality zn_connection_quality;
	switch (quality)
	{
	case ZOOMSDK::Conn_Quality_Unknown:
		zn_connection_quality = ZNConn_Quality_Unknown;
		break;
	case ZOOMSDK::Conn_Quality_Very_Bad:
		zn_connection_quality = ZNConn_Quality_Very_Bad;
		break;
	case ZOOMSDK::Conn_Quality_Bad:
		zn_connection_quality = ZNConn_Quality_Bad;
		break;
	case ZOOMSDK::Conn_Quality_Not_Good:
		zn_connection_quality = ZNConn_Quality_Not_Good;
		break;
	case ZOOMSDK::Conn_Quality_Normal:
		zn_connection_quality = ZNConn_Quality_Normal;
		break;
	case ZOOMSDK::Conn_Quality_Good:
		zn_connection_quality = ZNConn_Quality_Good;
		break;
	case ZOOMSDK::Conn_Quality_Excellent:
		zn_connection_quality = ZNConn_Quality_Excellent;
		break;
	default:
		break;
	}
	return zn_connection_quality;
}
ZNAudioStatus Map2WrapDefine(ZOOM_SDK_NAMESPACE::AudioStatus status)
{
	ZNAudioStatus zn_audio_status;
	switch (status)
	{
	case ZOOMSDK::Audio_None:
		zn_audio_status = ZN_Audio_None;
		break;
	case ZOOMSDK::Audio_Muted:
		zn_audio_status = ZN_Audio_Muted;
		break;
	case ZOOMSDK::Audio_UnMuted:
		zn_audio_status = ZN_Audio_UnMuted;
		break;
	case ZOOMSDK::Audio_Muted_ByHost:
		zn_audio_status = ZN_Audio_Muted_ByHost;
		break;
	case ZOOMSDK::Audio_UnMuted_ByHost:
		zn_audio_status = ZN_Audio_UnMuted_ByHost;
		break;
	case ZOOMSDK::Audio_MutedAll_ByHost:
		zn_audio_status = ZN_Audio_MutedAll_ByHost;
		break;
	case ZOOMSDK::Audio_UnMutedAll_ByHost:
		zn_audio_status = ZN_Audio_UnMutedAll_ByHost;
		break;
	default:
		break;
	}
	return zn_audio_status;
}
ZNVideoStatus Map2WrapDefine(ZOOM_SDK_NAMESPACE::VideoStatus status)
{
	ZNVideoStatus zn_video_status;
	switch (status)
	{
	case ZOOMSDK::Video_ON:
		zn_video_status = ZN_Video_ON;
		break;
	case ZOOMSDK::Video_OFF:
		zn_video_status = ZN_Video_OFF;
		break;
	default:
		break;
	}
	return zn_video_status;
}
ZNH323CalloutStatus Map2WrapDefine(ZOOM_SDK_NAMESPACE::H323CalloutStatus status)
{
	ZNH323CalloutStatus zn_h323_callout_status;
	switch (status)
	{
	case ZOOMSDK::H323Callout_Unknown:
		zn_h323_callout_status = ZN_H323Callout_Unknown;
		break;
	case ZOOMSDK::H323Callout_Success:
		zn_h323_callout_status = ZN_H323Callout_Success;
		break;
	case ZOOMSDK::H323Callout_Ring:
		zn_h323_callout_status = ZN_H323Callout_Ring;
		break;
	case ZOOMSDK::H323Callout_Timeout:
		zn_h323_callout_status = ZN_H323Callout_Timeout;
		break;
	case ZOOMSDK::H323Callout_Failed:
		zn_h323_callout_status = ZN_H323Callout_Failed;
		break;
	case ZOOMSDK::H323Callout_Busy:
		zn_h323_callout_status = ZN_H323Callout_Busy;
		break;
	case ZOOMSDK::H323Callout_Decline:
		zn_h323_callout_status = ZN_H323Callout_Decline;
		break;
	default:
		break;
	}
	return zn_h323_callout_status;
}
ZNH323DeviceType Map2WrapDefine(ZOOM_SDK_NAMESPACE::H323DeviceType type)
{
	ZNH323DeviceType zn_h323_device_type;
	switch (type)
	{
	case ZOOMSDK::H323DeviceType_Unknown:
		zn_h323_device_type = ZN_H323DeviceType_Unknown;
		break;
	case ZOOMSDK::H323DeviceType_H323:
		zn_h323_device_type = ZN_H323DeviceType_H323;
		break;
	case ZOOMSDK::H323DeviceType_SIP:
		zn_h323_device_type = ZN_H323DeviceType_SIP;
		break;
	case ZOOMSDK::H323DeviceType_BOTH:
		zn_h323_device_type = ZN_H323DeviceType_BOTH;
		break;
	default:
		break;
	}
	return zn_h323_device_type;
}
ZNFreeMeetingNeedUpgradeType Map2WrapDefine(ZOOM_SDK_NAMESPACE::IMeetingConfigurationFreeMeetingEvent::FreeMeetingNeedUpgradeType type)
{
	ZNFreeMeetingNeedUpgradeType zn_free_meeting_need_upgrade_type;
	switch (type)
	{
	case ZOOMSDK::IMeetingConfigurationFreeMeetingEvent::FreeMeetingNeedUpgradeType_NONE:
		zn_free_meeting_need_upgrade_type = ZN_FreeMeetingNeedUpgradeType_NONE;
		break;
	case ZOOMSDK::IMeetingConfigurationFreeMeetingEvent::FreeMeetingNeedUpgradeType_BY_ADMIN:
		zn_free_meeting_need_upgrade_type = ZN_FreeMeetingNeedUpgradeType_BY_ADMIN;
		break;
	case ZOOMSDK::IMeetingConfigurationFreeMeetingEvent::FreeMeetingNeedUpgradeType_BY_GIFTURL:
		zn_free_meeting_need_upgrade_type = ZN_FreeMeetingNeedUpgradeType_BY_GIFTURL;
		break;
	default:
		break;
	}
	return zn_free_meeting_need_upgrade_type;
}

ZNDirectShareStatus Map2WrapDefine(ZOOM_SDK_NAMESPACE::DirectShareStatus status)
{
	ZNDirectShareStatus zn_direct_share_status;
	switch (status)
	{
	case ZOOMSDK::DirectShare_Unknown:
		zn_direct_share_status = ZN_DirectShare_Unknown;
		break;
	case ZOOMSDK::DirectShare_Connecting:
		zn_direct_share_status = ZN_DirectShare_Connecting;
		break;
	case ZOOMSDK::DirectShare_In_Direct_Share_Mode:
		zn_direct_share_status = ZN_DirectShare_In_Direct_Share_Mode;
		break;
	case ZOOMSDK::DirectShare_Ended:
		zn_direct_share_status = ZN_DirectShare_Ended;
		break;
	case ZOOMSDK::DirectShare_Need_MeetingID_Or_PairingCode:
		zn_direct_share_status = ZN_DirectShare_Need_MeetingID_Or_PairingCode;
		break;
	case ZOOMSDK::DirectShare_NetWork_Error:
		zn_direct_share_status = ZN_DirectShare_NetWork_Error;
		break;
	case ZOOMSDK::DirectShare_Other_Error:
		zn_direct_share_status = ZN_DirectShare_Other_Error;
		break;
	case ZOOMSDK::DirectShare_WrongMeetingID_Or_SharingKey:
		zn_direct_share_status = ZN_DirectShare_WrongMeetingID_Or_SharingKey;
		break; 
	case ZOOMSDK::DirectShare_InputNewParingCode:
		zn_direct_share_status = ZN_DirectShare_InputNewParingCode;
		break;
	case ZOOMSDK::DirectShare_Prepared:
		zn_direct_share_status = ZN_DirectShare_Prepared;
		break;
	default:
		break;
	}
	return zn_direct_share_status;
}
ZNShareStatus Map2WrapDefine(ZOOM_SDK_NAMESPACE::SharingStatus status)
{
	ZNShareStatus zn_sharing_status = ZN_Sharing_None;
	switch (status)
	{
	case ZOOMSDK::Sharing_Self_Send_Begin:
		zn_sharing_status = ZN_Sharing_Self_Send_Begin;
		break;
	case ZOOMSDK::Sharing_Self_Send_End:
		zn_sharing_status = ZN_Sharing_Self_Send_End;
		break;
	case ZOOMSDK::Sharing_Self_Send_Pure_Audio_Begin:
		zn_sharing_status = ZN_Sharing_SelfStartAudioShare;
		break;
	case ZOOMSDK::Sharing_Self_Send_Pure_Audio_End:
		zn_sharing_status = ZN_Sharing_SelfStopAudioShare;
		break;
	case ZOOMSDK::Sharing_Other_Share_Begin:
		zn_sharing_status = ZN_Sharing_Other_Share_Begin;
		break;
	case ZOOMSDK::Sharing_Other_Share_End:
		zn_sharing_status = ZN_Sharing_Other_Share_End;
		break;
	case ZOOMSDK::Sharing_Other_Share_Pure_Audio_Begin:
		zn_sharing_status = ZN_Sharing_OtherStartAudioShare;
		break;
	case ZOOMSDK::Sharing_Other_Share_Pure_Audio_End:
		zn_sharing_status = ZN_Sharing_OtherStopAudioShare;
		break;
	case ZOOMSDK::Sharing_View_Other_Sharing:
		zn_sharing_status = ZN_Sharing_View_Other_Sharing;
		break;
	case ZOOMSDK::Sharing_Pause:
		zn_sharing_status = ZN_Sharing_Pause;
		break;
	case ZOOMSDK::Sharing_Resume:
		zn_sharing_status = ZN_Sharing_Resume;
		break;
	default:
		break;
	}
	return zn_sharing_status;
}
ZNShareType Map2WrapDefine(ZOOM_SDK_NAMESPACE::ShareType type)
{
	ZNShareType zn_type = ZNSHARE_TYPE_UNKNOWN;
	switch (type)
	{
	case ZOOMSDK::SHARE_TYPE_UNKNOWN:
		zn_type = ZNSHARE_TYPE_UNKNOWN;
		break;
	case ZOOMSDK::SHARE_TYPE_AS:
		zn_type = ZNSHARE_TYPE_AS;
		break;
	case ZOOMSDK::SHARE_TYPE_DS:
		zn_type = ZNSHARE_TYPE_DS;
		break;
	case ZOOMSDK::SHARE_TYPE_WB:
		zn_type = ZNSHARE_TYPE_WB;
		break;
	case ZOOMSDK::SHARE_TYPE_AIRHOST:
		zn_type = ZNSHARE_TYPE_AIRHOST;
		break;
	case ZOOMSDK::SHARE_TYPE_CAMERA:
		zn_type = ZNSHARE_TYPE_CAMERA;
		break;
	case ZOOMSDK::SHARE_TYPE_DATA:
		zn_type = ZNSHARE_TYPE_DATA;
		break;
	case ZOOMSDK::SHARE_TYPE_VIDEO_FILE:
		zn_type = ZNSHARE_TYPE_VIDEO_FILE;
		break;
	case ZOOMSDK::SHARE_TYPE_FRAME:
		zn_type = ZNSHARE_TYPE_FRAME;
		break;
	case ZOOMSDK::SHARE_TYPE_DOCUMENT:
		zn_type = ZNSHARE_TYPE_DOCUMENT;
		break;
	case ZOOMSDK::SHARE_TYPE_COMPUTER_AUDIO:
		zn_type = ZNSHARE_TYPE_COMPUTER_AUDIO;
		break;
	default:
		break;
	}
	return zn_type;
}
ZNAudioType Map2WrapDefine(ZOOM_SDK_NAMESPACE::AudioType type)
{
	ZNAudioType zn_audio_type;
	switch (type)
	{
	case ZOOMSDK::AUDIOTYPE_NONE:
		zn_audio_type = ZN_AUDIOTYPE_NONE;
		break;
	case ZOOMSDK::AUDIOTYPE_VOIP:
		zn_audio_type = ZN_AUDIOTYPE_VOIP;
		break;
	case ZOOMSDK::AUDIOTYPE_PHONE:
		zn_audio_type = ZN_AUDIOTYPE_PHONE;
		break;
	case ZOOMSDK::AUDIOTYPE_UNKNOWN:
		zn_audio_type = ZN_AUDIOTYPE_UNKNOWN;
		break;
	default:
		break;
	}
	return zn_audio_type;
}
ZNUserRole Map2WrapDefine(ZOOM_SDK_NAMESPACE::UserRole role)
{
	ZNUserRole zn_user_role;
	switch (role)
	{
	case ZOOMSDK::USERROLE_NONE:
		zn_user_role = ZN_USERROLE_NONE;
		break;
	case ZOOMSDK::USERROLE_HOST:
		zn_user_role = ZN_USERROLE_HOST;
		break;
	case ZOOMSDK::USERROLE_COHOST:
		zn_user_role = ZN_USERROLE_COHOST;
		break;
	case ZOOMSDK::USERROLE_PANELIST:
		zn_user_role = ZN_USERROLE_PANELIST;
		break;
	case ZOOMSDK::USERROLE_BREAKOUTROOM_MODERATOR:
		zn_user_role = ZN_USERROLE_BREAKOUTROOM_MODERATOR;
		break;
	case ZOOMSDK::USERROLE_ATTENDEE:
		zn_user_role = ZN_USERROLE_ATTENDEE;
		break;
	default:
		break;
	}
	return zn_user_role;
}
ZNSMSVerificationCodeErr Map2WrapDefine(SMSVerificationCodeErr err)
{
	ZNSMSVerificationCodeErr zn_err = ZNSMSVerificationCodeErr_Unknown;
	switch (err)
	{
	case SMSVerificationCodeErr_Unknown:
		zn_err = ZNSMSVerificationCodeErr_Unknown;
		break;
	case SMSVerificationCodeErr_Success:
		zn_err = ZNSMSVerificationCodeErr_Success;
		break;
	case SMSVerificationCodeErr_Retrieve_SendSMSFailed:
		zn_err = ZNSMSVerificationCodeErr_Retrieve_SendSMSFailed;
		break;
	case SMSVerificationCodeErr_Retrieve_InvalidPhoneNum:
		zn_err = ZNSMSVerificationCodeErr_Retrieve_InvalidPhoneNum;
		break;
	case SMSVerificationCodeErr_Retrieve_PhoneNumAlreadyBound:
		zn_err = ZNSMSVerificationCodeErr_Retrieve_PhoneNumAlreadyBound;
		break;
	case SMSVerificationCodeErr_Retrieve_PhoneNumSendTooFrequent:
		zn_err = ZNSMSVerificationCodeErr_Retrieve_PhoneNumSendTooFrequent;
		break;
	case SMSVerificationCodeErr_Verify_CodeIncorrect:
		zn_err = ZNSMSVerificationCodeErr_Verify_CodeIncorrect;
		break;
	case SMSVerificationCodeErr_Verify_CodeExpired:
		zn_err = ZNSMSVerificationCodeErr_Verify_CodeExpired;
		break;
	case SMSVerificationCodeErr_Verify_UnknownError:
		zn_err = ZNSMSVerificationCodeErr_Verify_UnknownError;
		break;
	default:
		break;
	}
	return zn_err;
}
ZNLiveStreamStatus Map2WrapDefine(ZOOM_SDK_NAMESPACE::LiveStreamStatus status)
{
	ZNLiveStreamStatus zn_live_stream_status = ZN_LiveStreamStatus_None;
	switch (status)
	{
	case ZOOMSDK::LiveStreamStatus_None:
		zn_live_stream_status = ZN_LiveStreamStatus_None;
		break;
	case ZOOMSDK::LiveStreamStatus_InProgress:
		zn_live_stream_status = ZN_LiveStreamStatus_InProgress;
		break;
	case ZOOMSDK::LiveStreamStatus_Connecting:
		zn_live_stream_status = ZN_LiveStreamStatus_Connecting;
		break;
	case ZOOMSDK::LiveStreamStatus_Start_Failed_Timeout:
		zn_live_stream_status = ZN_LiveStreamStatus_Start_Failed_Timeout;
		break;
	case ZOOMSDK::LiveStreamStatus_Start_Failed:
		zn_live_stream_status = ZN_LiveStreamStatus_Start_Failed;
		break;
	case ZOOMSDK::LiveStreamStatus_Ended:
		zn_live_stream_status = ZN_LiveStreamStatus_Ended;
		break;
	default:
		break;
	}
	return zn_live_stream_status;
}
ZNMeetingReminderType Map2WrapDefine(ZOOM_SDK_NAMESPACE::MeetingReminderType type)
{
	ZNMeetingReminderType zn_reminder_type = ZNTYPE_LOGIN_REQUIRED;
	switch (type)
	{
	case ZOOMSDK::TYPE_LOGIN_REQUIRED:
		zn_reminder_type = ZNTYPE_LOGIN_REQUIRED;
		break;
	case ZOOMSDK::TYPE_START_OR_JOIN_MEETING:
		zn_reminder_type = ZNTYPE_START_OR_JOIN_MEETING;
		break;
	case ZOOMSDK::TYPE_RECORD_REMINDER:
		zn_reminder_type = ZNTYPE_RECORD_REMINDER;
		break;
	case ZOOMSDK::TYPE_RECORD_DISCLAIMER:
		zn_reminder_type = ZNTYPE_RECORD_DISCLAIMER;
		break;
	case ZOOMSDK::TYPE_LIVE_STREAM_DISCLAIMER:
		zn_reminder_type = ZNTYPE_LIVE_STREAM_DISCLAIMER;
		break;
	case ZOOMSDK::TYPE_ARCHIVE_DISCLAIMER:
		zn_reminder_type = ZNTYPE_ARCHIVE_DISCLAIMER;
		break;
	case ZOOMSDK::TYPE_WEBINAR_AS_PANELIST_JOIN:
		zn_reminder_type = ZNTYPE_WEBINAR_AS_PANELIST_JOIN;
		break;
	case ZOOMSDK::TYPE_TERMS_OF_SERVICE:
		zn_reminder_type = ZNTYPE_TERMS_OF_SERVICE;
		break;
	case ZOOMSDK::TYPE_SMART_SUMMARY_DISCLAIMER:
		zn_reminder_type = ZNTYPE_SMART_SUMMARY_DISCLAIMER;
		break;
	case ZOOMSDK::TYPE_QUERY_DISCLAIMER:
		zn_reminder_type = ZNTYPE_QUERY_DISCLAIMER;
		break;
	case ZOOMSDK::TYPE_WEBINAR_ATTENDEE_PROMOTE_REMINDER:
		zn_reminder_type = ZNTYPE_WEBINAR_ATTENDEE_PROMOTE_REMINDER;
		break;
	case ZOOMSDK::TYPE_JOIN_PRIVATE_MODE_MEETING_REMINDER:
		zn_reminder_type = ZNTYPE_JOIN_PRIVATE_MODE_MEETING_REMINDER;
		break;
	case ZOOMSDK::TYPE_CLOSED_CAPTION_DISCLAIMER:
		zn_reminder_type = ZNTYPE_CLOSED_CAPTION_DISCLAIMER;
		break;
	case ZOOMSDK::TYPE_MULTI_DISCLAIMER:
		zn_reminder_type = ZNTYPE_MULTI_DISCLAIMER;
		break;
	case ZOOMSDK::TYPE_JOIN_MEETING_CONNECTOR_AS_GUEST_REMINDER:
		zn_reminder_type = ZNTYPE_JOIN_MEETING_CONNECTOR_AS_GUEST_REMINDER;
		break;
	case ZOOMSDK::TYPE_COMMON_DISCLAIMER:
		zn_reminder_type = ZNTYPE_COMMON_DISCLAIMER;
		break;
	default:
		break;
	}
	return zn_reminder_type;
}
ZNSDKChatMessageType Map2WrapDefine(ZOOM_SDK_NAMESPACE::SDKChatMessageType type)
{
	ZNSDKChatMessageType zn_chat_message_type = ZNSDKChatMessageType_To_None;
	switch (type)
	{
	case ZOOMSDK::SDKChatMessageType_To_None:
		zn_chat_message_type = ZNSDKChatMessageType_To_None;
		break;
	case ZOOMSDK::SDKChatMessageType_To_All:
		zn_chat_message_type = ZNSDKChatMessageType_To_All;
		break;
	case ZOOMSDK::SDKChatMessageType_To_All_Panelist:
		zn_chat_message_type = ZNSDKChatMessageType_To_All_Panelist;
		break;
	case ZOOMSDK::SDKChatMessageType_To_Individual_Panelist:
		zn_chat_message_type = ZNSDKChatMessageType_To_Individual_Panelist;
		break;
	case ZOOMSDK::SDKChatMessageType_To_Individual:
		zn_chat_message_type = ZNSDKChatMessageType_To_Individual;
		break;
	case ZOOMSDK::SDKChatMessageType_To_WaitingRoomUsers:
		zn_chat_message_type = ZNSDKChatMessageType_To_WaitingRoomUsers;
		break;
	default:
		break;
	}
	return zn_chat_message_type;
}
ZNSDKChatMessageDeleteType Map2WrapDefine(ZOOM_SDK_NAMESPACE::SDKChatMessageDeleteType type)
{
	ZNSDKChatMessageDeleteType zn_chat_message_delete_type = ZNSDK_CHAT_DELETE_BY_NONE;
	switch (type)
	{
	case ZOOMSDK::SDK_CHAT_DELETE_BY_NONE:
		zn_chat_message_delete_type = ZNSDK_CHAT_DELETE_BY_NONE;
		break;
	case ZOOMSDK::SDK_CHAT_DELETE_BY_SELF:
		zn_chat_message_delete_type = ZNSDK_CHAT_DELETE_BY_SELF;
		break;
	case ZOOMSDK::SDK_CHAT_DELETE_BY_HOST:
		zn_chat_message_delete_type = ZNSDK_CHAT_DELETE_BY_HOST;
		break;
	case ZOOMSDK::SDK_CHAT_DELETE_BY_DLP:
		zn_chat_message_delete_type = ZNSDK_CHAT_DELETE_BY_DLP;
		break;
	default:
		break;
	}
	return zn_chat_message_delete_type;
}
ZNSDKFileTransferStatus Map2WrapDefine(ZOOM_SDK_NAMESPACE::SDKFileTransferStatus status)
{
	ZNSDKFileTransferStatus zn_status = ZNSDKFileTransferState_None;
	switch (status)
	{
	case ZOOMSDK::SDKFileTransferState_None:
		zn_status = ZNSDKFileTransferState_None;
		break;
	case ZOOMSDK::SDKFileTransferState_ReadyToTransfer:
		zn_status = ZNSDKFileTransferState_ReadyToTransfer;
		break;
	case ZOOMSDK::SDKFileTransferState_Transfering:
		zn_status = ZNSDKFileTransferState_Transfering;
		break;
	case ZOOMSDK::SDKFileTransferState_TransferFailed:
		zn_status = ZNSDKFileTransferState_TransferFailed;
		break;
	case ZOOMSDK::SDKFileTransferState_TransferDone:
		zn_status = ZNSDKFileTransferState_TransferDone;
		break;
	default:
		break;
	}
	return zn_status;
}
ZNSettingsNetWorkType Map2WrapDefine(ZOOM_SDK_NAMESPACE::SettingsNetWorkType role)
{
	ZNSettingsNetWorkType zn_type = ZNSETTINGS_NETWORK_UNKNOWN;
	switch (role)
	{
	case ZOOMSDK::SETTINGS_NETWORK_WIRED:
		zn_type = ZNSETTINGS_NETWORK_WIRED;
		break;
	case ZOOMSDK::SETTINGS_NETWORK_WIFI:
		zn_type = ZNSETTINGS_NETWORK_WIFI;
		break;
	case ZOOMSDK::SETTINGS_NETWORK_PPP:
		zn_type = ZNSETTINGS_NETWORK_PPP;
		break;
	case ZOOMSDK::SETTINGS_NETWORK_3G:
		zn_type = ZNSETTINGS_NETWORK_3G;
		break;
	case ZOOMSDK::SETTINGS_NETWORK_OTHERS:
		zn_type = ZNSETTINGS_NETWORK_OTHERS;
		break;
	case ZOOMSDK::SETTINGS_NETWORK_UNKNOWN:
		zn_type = ZNSETTINGS_NETWORK_UNKNOWN;
		break;
	default:
		break;
	}
	return zn_type;
}
ZNStatisticsWarningType Map2WrapDefine(ZOOM_SDK_NAMESPACE::StatisticsWarningType type)
{
	ZNStatisticsWarningType zn_type = ZNStatistics_Warning_None;
	switch (type)
	{
	case ZOOMSDK::Statistics_Warning_None:
		zn_type = ZNStatistics_Warning_None;
		break;
	case ZOOMSDK::Statistics_Warning_Network_Quality_Bad:
		zn_type = ZNStatistics_Warning_Network_Quality_Bad;
		break;
	case ZOOMSDK::Statistics_Warning_Busy_System:
		zn_type = ZNStatistics_Warning_Busy_System;
		break;
	default:
		break;
	}
	return zn_type;
}
ZNSDKEmojiFeedbackType Map2WrapDefine(ZOOM_SDK_NAMESPACE::SDKEmojiFeedbackType type)
{
	ZNSDKEmojiFeedbackType zn_type = ZNSDKEmojiFeedbackType_None;
	switch (type)
	{
	case ZOOMSDK::SDKEmojiFeedbackType_None:
		zn_type = ZNSDKEmojiFeedbackType_None;
		break;
	case ZOOMSDK::SDKEmojiFeedbackType_Yes:
		zn_type = ZNSDKEmojiFeedbackType_Yes;
		break;
	case ZOOMSDK::SDKEmojiFeedbackType_No:
		zn_type = ZNSDKEmojiFeedbackType_No;
		break;
	case ZOOMSDK::SDKEmojiFeedbackType_SpeedUp:
		zn_type = ZNSDKEmojiFeedbackType_SpeedUp;
		break;
	case ZOOMSDK::SDKEmojiFeedbackType_SlowDown:
		zn_type = ZNSDKEmojiFeedbackType_SlowDown;
		break;
	case ZOOMSDK::SDKEmojiFeedbackType_Away:
		zn_type = ZNSDKEmojiFeedbackType_Away;
		break;
	default:
		break;
	}
	return zn_type;
}
ZNWaitingRoomLayoutType Map2WrapDefine(ZOOM_SDK_NAMESPACE::WaitingRoomLayoutType type)
{
	ZNWaitingRoomLayoutType zn_type = ZNWaitingRoomLayoutType_Default;
	switch (type)
	{
	case ZOOMSDK::WaitingRoomLayoutType_Default:
		zn_type = ZNWaitingRoomLayoutType_Default;
		break;
	case ZOOMSDK::WaitingRoomLayoutType_Logo:
		zn_type = ZNWaitingRoomLayoutType_Logo;
		break;
	case ZOOMSDK::WaitingRoomLayoutType_Video:
		zn_type = ZNWaitingRoomLayoutType_Video;
		break;
	default:
		break;
	}
	return zn_type;
}
ZNCustomWaitingRoomDataStatus Map2WrapDefine(ZOOM_SDK_NAMESPACE::CustomWaitingRoomDataStatus status)
{
	ZNCustomWaitingRoomDataStatus zn_status = ZNCustomWaitingRoomDataStatus_Init;
	switch (status)
	{
	case ZOOMSDK::CustomWaitingRoomDataStatus_Init:
		zn_status = ZNCustomWaitingRoomDataStatus_Init;
		break;
	case ZOOMSDK::CustomWaitingRoomDataStatus_Downloading:
		zn_status = ZNCustomWaitingRoomDataStatus_Downloading;
		break;
	case ZOOMSDK::CustomWaitingRoomDataStatus_Download_OK:
		zn_status = ZNCustomWaitingRoomDataStatus_Download_OK;
		break;
	case ZOOMSDK::CustomWaitingRoomDataStatus_Download_Failed:
		zn_status = ZNCustomWaitingRoomDataStatus_Download_Failed;
		break;
	default:
		break;
	}
	return zn_status;
}
ZNSettingConnectionType Map2WrapDefine(ZOOM_SDK_NAMESPACE::SettingConnectionType role)
{
	ZNSettingConnectionType zn_type = ZNSETTINGS_CONNECTION_TYPE_UNKNOWN;
	switch (role)
	{
	case ZOOMSDK::SETTINGS_CONNECTION_TYPE_CLOUD:
		zn_type = ZNSETTINGS_CONNECTION_TYPE_CLOUD;
		break;
	case ZOOMSDK::SETTINGS_CONNECTION_TYPE_DIRECT:
		zn_type = ZNSETTINGS_CONNECTION_TYPE_DIRECT;
		break;
	case ZOOMSDK::SETTINGS_CONNECTION_TYPE_UNKNOWN:
		zn_type = ZNSETTINGS_CONNECTION_TYPE_UNKNOWN;
		break;
	default:
		break;
	}
	return zn_type;
}

ZNRequiredInfoType Map2WrapDefine(ZOOM_SDK_NAMESPACE::IMeetingPasswordAndScreenNameHandler::RequiredInfoType type)
{
	ZNRequiredInfoType zn_type = ZNREQUIRED_INFO_TYPE_NONE;
	switch (type)
	{
	case ZOOMSDK::IMeetingPasswordAndScreenNameHandler::REQUIRED_INFO_TYPE_NONE:
		zn_type = ZNREQUIRED_INFO_TYPE_NONE;
		break;
	case ZOOMSDK::IMeetingPasswordAndScreenNameHandler::REQUIRED_INFO_TYPE_Password:
		zn_type = ZNREQUIRED_INFO_TYPE_Password;
		break;
	case ZOOMSDK::IMeetingPasswordAndScreenNameHandler::REQUIRED_INFO_TYPE_Password4WrongPassword:
		zn_type = ZNREQUIRED_INFO_TYPE_Password4WrongPassword;
		break;
	case ZOOMSDK::IMeetingPasswordAndScreenNameHandler::REQUIRED_INFO_TYPE_PasswordAndScreenName:
		zn_type = ZNREQUIRED_INFO_TYPE_PasswordAndScreenName;
		break;
	case ZOOMSDK::IMeetingPasswordAndScreenNameHandler::REQUIRED_INFO_TYPE_ScreenName:
		zn_type = ZNREQUIRED_INFO_TYPE_ScreenName;
		break;
	case ZOOMSDK::IMeetingPasswordAndScreenNameHandler::REQUIRED_INFO_TYPE_MeetingIDAndScreenName:
		zn_type = ZNREQUIRED_INFO_TYPE_MeetingIDAndScreenName;
		break;
	default:
		break;
	}
	return zn_type;
}
ZNWebinarNeedRegisterType Map2WrapDefine(ZOOM_SDK_NAMESPACE::IWebinarNeedRegisterHandler::WebinarNeedRegisterType type)
{
	ZNWebinarNeedRegisterType zn_type = ZNWebinarReg_NONE;
	switch (type)
	{
	case ZOOMSDK::IWebinarNeedRegisterHandler::WebinarReg_NONE:
		zn_type = ZNWebinarReg_NONE;
		break;
	case ZOOMSDK::IWebinarNeedRegisterHandler::WebinarReg_By_Register_Url:
		zn_type = ZNWebinarReg_By_Register_Url;
		break;
	case ZOOMSDK::IWebinarNeedRegisterHandler::WebinarReg_By_Email_and_DisplayName:
		zn_type = ZNWebinarReg_By_Email_and_DisplayName;
		break;
	default:
		break;
	}
	return zn_type;
}
ZNAudioCallbackActionInfo Map2WrapDefine(ZOOM_SDK_NAMESPACE::AudioCallbackActionInfo info)
{
	ZNAudioCallbackActionInfo zn_info;
	switch (info)
	{
	case ZOOMSDK::ACTION_NONE:
		zn_info = ZNACTION_INFO_NONE;
		break;
	case ZOOMSDK::ACTION_CHOOSE_AUDIO_DEVICE_NOAUDIODEVICECONNECTTED:
		zn_info = ZNACTION_INFO_CHOOSE_AUDIO_DEVICE_NOAUDIODEVICECONNECTTED;
		break;
	case ZOOMSDK::ACTION_CHOOSE_AUDIO_DEVICE_COMPUTERAUDIODEVICEERROR:
		zn_info = ZNACTION_INFO_CHOOSE_AUDIO_DEVICE_COMPUTERAUDIODEVICEERROR;
		break;
	case ZOOMSDK::ACTION_CHOOSE_AUDIO_DEVICE_PHONECALLDEVICEERROR:
		zn_info = ZNACTION_INFO_CHOOSE_AUDIO_DEVICE_PHONECALLDEVICEERROR;
		break;
	case ZOOMSDK::ACTION_NEED_JOIN_VOIP:
		zn_info = ZNACTION_INFO_NEED_JOIN_VOIP;
		break;
	case ZOOMSDK::ACTION_MUTE_UNMUTE_AUDIO:
		zn_info = ZNACTION_INFO_MUTE_UNMUTE_AUDIO;
		break;
	case ZOOMSDK::ACTION_SHOW_AUDIO_SETTING_WINDOW:
		zn_info = ZNACTION_INFO_SHOW_AUDIO_SETTING_WINDOW;
		break;
	default:
		break;
	}
	return zn_info;
}
ZNRecordingStatus Map2WrapDefine(ZOOM_SDK_NAMESPACE::RecordingStatus status)
{
	ZNRecordingStatus zn_status;
	switch (status)
	{
	case ZOOMSDK::Recording_Start:
		zn_status = ZNRecording_Start;
		break;
	case ZOOMSDK::Recording_Stop:
		zn_status = ZNRecording_Stop;
		break;
	case ZOOMSDK::Recording_DiskFull:
		zn_status = ZNRecording_DiskFull;
		break;
	case ZOOMSDK::Recording_Pause:
		zn_status = ZNRecording_Pause;
		break;
	case ZOOMSDK::Recording_Connecting:
		zn_status = ZNRecording_Connecting;
		break;
	case ZOOMSDK::Recording_Fail:
		zn_status = ZNRecording_Fail;
		break;	
	default:
		break;
	}
	return zn_status;
}
ZNRequestStartCloudRecordingStatus Map2WrapDefine(ZOOM_SDK_NAMESPACE::RequestStartCloudRecordingStatus status)
{
	ZNRequestStartCloudRecordingStatus zn_status = ZNRequestStartCloudRecording_Granted;
	switch (status)
	{
	case ZOOMSDK::RequestStartCloudRecording_Granted:
		zn_status = ZNRequestStartCloudRecording_Granted;
		break;
	case ZOOMSDK::RequestStartCloudRecording_Denied:
		zn_status = ZNRequestStartCloudRecording_Denied;
		break;
	case ZOOMSDK::RequestStartCloudRecording_TimedOut:
		zn_status = ZNRequestStartCloudRecording_TimedOut;
		break;
	default:
		break;
	}
	return zn_status;
}
ZN_SDK_ECHO_CANCELLATION_LEVEL Map2WrapDefine(ZOOM_SDK_NAMESPACE::SDK_ECHO_CANCELLATION_LEVEL level)
{
	ZN_SDK_ECHO_CANCELLATION_LEVEL zn_level = ZN_SDK_ECHO_CANCELLATION_DEFAULT;
	switch (level)
	{
	case ZOOM_SDK_NAMESPACE::SDK_ECHO_CANCELLATION_DEFAULT:
		zn_level = ZN_SDK_ECHO_CANCELLATION_DEFAULT;
		break;
	case ZOOM_SDK_NAMESPACE::SDK_ECHO_CANCELLATION_LOW:
		zn_level = ZN_SDK_ECHO_CANCELLATION_LOW;
		break;
	case ZOOM_SDK_NAMESPACE::SDK_ECHO_CANCELLATION_HIGH:
		zn_level = ZN_SDK_ECHO_CANCELLATION_HIGH;
		break;
	default:
		break;
	}
	return zn_level;
}
ZNLoginFailReason Map2WrapDefine(ZOOM_SDK_NAMESPACE::LoginFailReason reason)
{
	ZNLoginFailReason zn_reason = ZNLoginFail_None;
	switch (reason)
	{
	case ZOOMSDK::LoginFail_EmailLoginDisable:
		zn_reason = ZNLoginFail_EmailLoginDisable;
		break;
	case ZOOMSDK::LoginFail_UserNotExist:
		zn_reason = ZNLoginFail_UserNotExist;
		break;
	case ZOOMSDK::LoginFail_WrongPassword:
		zn_reason = ZNLoginFail_WrongPassword;
		break;
	case ZOOMSDK::LoginFail_AccountLocked:
		zn_reason = ZNLoginFail_AccountLocked;
		break;
	case ZOOMSDK::LoginFail_SDKNeedUpdate:
		zn_reason = ZNLoginFail_SDKNeedUpdate;
		break;
	case ZOOMSDK::LoginFail_TooManyFailedAttempts:
		zn_reason = ZNLoginFail_TooManyFailedAttempts;
		break;
	case ZOOMSDK::LoginFail_SMSCodeError:
		zn_reason = ZNLoginFail_SMSCodeError;
		break;
	case ZOOMSDK::LoginFail_SMSCodeExpired:
		zn_reason = ZNLoginFail_SMSCodeExpired;
		break;
	case ZOOMSDK::LoginFail_PhoneNumberFormatInValid:
		zn_reason = ZNLoginFail_PhoneNumberFormatInValid;
		break;
	case ZOOMSDK::LoginFail_OtherIssue:
		zn_reason = ZNLoginFail_OtherIssue;
		break;
	case ZOOMSDK::LoginFail_None:
		zn_reason = ZNLoginFail_None;
		break;
	case ZOOMSDK::LoginFail_LoginTokenInvalid:
		zn_reason = ZNLoginFail_LoginTokenInvalid;
		break;
	default:
		break;
	}
	return zn_reason;
}
ZNSDKPanelistChatPrivilege Map2WrapDefine(ZOOM_SDK_NAMESPACE::SDKPanelistChatPrivilege privilege)
{
	ZNSDKPanelistChatPrivilege zn_privilege = ZNSDKPanelistChatPrivilege_PanelistOnly;
	switch (privilege)
	{
	case ZOOM_SDK_NAMESPACE::SDKPanelistChatPrivilege_PanelistOnly:
		zn_privilege = ZNSDKPanelistChatPrivilege_PanelistOnly;
		break;
	case ZOOM_SDK_NAMESPACE::SDKPanelistChatPrivilege_All:
		zn_privilege = ZNSDKPanelistChatPrivilege_All;
		break;
	default:
		break;
	}
	return zn_privilege;
}
ZNSDKAttendeeViewMode Map2WrapDefine(ZOOM_SDK_NAMESPACE::SDKAttendeeViewMode mode)
{
	ZNSDKAttendeeViewMode zn_mode = ZNSDKAttendeeViewMode_None;
	switch (mode)
	{
	case ZOOM_SDK_NAMESPACE::SDKAttendeeViewMode_None:
		zn_mode = ZNSDKAttendeeViewMode_None;
		break;
	case ZOOM_SDK_NAMESPACE::SDKAttendeeViewMode_FollowHost:
		zn_mode = ZNSDKAttendeeViewMode_FollowHost;
		break;
	case ZOOM_SDK_NAMESPACE::SDKAttendeeViewMode_Speaker:
		zn_mode = ZNSDKAttendeeViewMode_Speaker;
		break;
	case ZOOM_SDK_NAMESPACE::SDKAttendeeViewMode_Gallery:
		zn_mode = ZNSDKAttendeeViewMode_Gallery;
		break;
	case ZOOM_SDK_NAMESPACE::SDKAttendeeViewMode_Standard:
		zn_mode = ZNSDKAttendeeViewMode_Standard;
		break;
	case ZOOM_SDK_NAMESPACE::SDKAttendeeViewMode_SidebysideSpeaker:
		zn_mode = ZNSDKAttendeeViewMode_SidebysideSpeaker;
		break;
	case ZOOM_SDK_NAMESPACE::SDKAttendeeViewMode_SidebysideGallery:
		zn_mode = ZNSDKAttendeeViewMode_SidebysideGallery;
		break;
	default:
		break;
	}
	return zn_mode;
}
ZNZoomSDKWallpaperLayoutMode Map2WrapDefine(ZOOM_SDK_NAMESPACE::ZoomSDKWallpaperLayoutMode mode)
{
	ZNZoomSDKWallpaperLayoutMode zn_mode = ZNZoomSDKWallpaperLayoutMode_None;
	switch (mode)
	{
	case ZOOM_SDK_NAMESPACE::ZoomSDKWallpaperLayoutMode_None:
		zn_mode = ZNZoomSDKWallpaperLayoutMode_None;
		break;
	case ZOOM_SDK_NAMESPACE::ZoomSDKWallpaperLayoutMode_Fill:
		zn_mode = ZNZoomSDKWallpaperLayoutMode_Fill;
		break;
	case ZOOM_SDK_NAMESPACE::ZoomSDKWallpaperLayoutMode_Fit:
		zn_mode = ZNZoomSDKWallpaperLayoutMode_Fit;
		break;
	default:
		break;
	}
	return zn_mode;
}
ZNZoomSDKWallpaperSettingStatus Map2WrapDefine(ZOOM_SDK_NAMESPACE::ZoomSDKWallpaperSettingStatus status)
{
	ZNZoomSDKWallpaperSettingStatus zn_status = ZNZoomSDKWallpaperSettingStatus_None;
	switch (status)
	{
	case ZOOM_SDK_NAMESPACE::ZoomSDKWallpaperSettingStatus_None:
		zn_status = ZNZoomSDKWallpaperSettingStatus_None;
		break;
	case ZOOM_SDK_NAMESPACE::ZoomSDKWallpaperSettingStatus_Downloading:
		zn_status = ZNZoomSDKWallpaperSettingStatus_Downloading;
		break;
	case ZOOM_SDK_NAMESPACE::ZoomSDKWallpaperSettingStatus_Downloaded:
		zn_status = ZNZoomSDKWallpaperSettingStatus_Downloaded;
		break;
	case ZOOM_SDK_NAMESPACE::ZoomSDKWallpaperSettingStatus_DownloadFail:
		zn_status = ZNZoomSDKWallpaperSettingStatus_DownloadFail;
		break;
	default:
		break;
	}
	return zn_status;
}
ZNVBVideoError Map2WrapDefine(ZOOM_SDK_NAMESPACE::VBVideoError error)
{
	ZNVBVideoError zn_error = ZNVB_VideoError_None;
	switch (error)
	{
	case ZOOM_SDK_NAMESPACE::VB_VideoError_None:
		zn_error = ZNVB_VideoError_None;
		break;
	case ZOOM_SDK_NAMESPACE::VB_VideoError_UnknownFormat:
		zn_error = ZNVB_VideoError_UnknownFormat;
		break;
	case ZOOM_SDK_NAMESPACE::VB_VideoError_ResolutionHigh1080P:
		zn_error = ZNVB_VideoError_ResolutionHigh1080P;
		break;
	case ZOOM_SDK_NAMESPACE::VB_VideoError_ResolutionHigh720P:
		zn_error = ZNVB_VideoError_ResolutionHigh720P;
		break;
	case ZOOM_SDK_NAMESPACE::VB_VideoError_ResolutionLow:
		zn_error = ZNVB_VideoError_ResolutionLow;
		break;
	case ZOOM_SDK_NAMESPACE::VB_VideoError_PlayError:
		zn_error = ZNVB_VideoError_PlayError;
		break;
	case ZOOM_SDK_NAMESPACE::VB_VideoError_OpenError:
		zn_error = ZNVB_VideoError_OpenError;
		break;
	default:
		break;
	}
	return zn_error;
}
ZNMeetingAICompanionQuerySettingOptions Map2WrapDefine(ZOOM_SDK_NAMESPACE::MeetingAICompanionQuerySettingOptions option)
{
	ZNMeetingAICompanionQuerySettingOptions zn_option = ZNMeetingAICompanionQuerySettingOptions_None;
	switch (option)
	{
	case ZOOM_SDK_NAMESPACE::MeetingAICompanionQuerySettingOptions_None:
		zn_option = ZNMeetingAICompanionQuerySettingOptions_None;
		break;
	case ZOOM_SDK_NAMESPACE::MeetingAICompanionQuerySettingOptions_WhenQueryStarted:
		zn_option = ZNMeetingAICompanionQuerySettingOptions_WhenQueryStarted;
		break;
	case ZOOM_SDK_NAMESPACE::MeetingAICompanionQuerySettingOptions_WhenParticipantsJoin:
		zn_option = ZNMeetingAICompanionQuerySettingOptions_WhenParticipantsJoin;
		break;
	case ZOOM_SDK_NAMESPACE::MeetingAICompanionQuerySettingOptions_OnlyHost:
		zn_option = ZNMeetingAICompanionQuerySettingOptions_OnlyHost;
		break;
	case ZOOM_SDK_NAMESPACE::MeetingAICompanionQuerySettingOptions_ParticipantsAndInviteesInOurOrganization:
		zn_option = ZNMeetingAICompanionQuerySettingOptions_ParticipantsAndInviteesInOurOrganization;
		break;
	case ZOOM_SDK_NAMESPACE::MeetingAICompanionQuerySettingOptions_WhenParticipantsAndOrganizationJoin:
		zn_option = ZNMeetingAICompanionQuerySettingOptions_WhenParticipantsAndOrganizationJoin;
		break;
	case ZOOM_SDK_NAMESPACE::MeetingAICompanionQuerySettingOptions_HostAndPanelists:
		zn_option = ZNMeetingAICompanionQuerySettingOptions_HostAndPanelists;
		break;
	case ZOOM_SDK_NAMESPACE::MeetingAICompanionQuerySettingOptions_HostAndPanelistsInOurOrganization:
		zn_option = ZNMeetingAICompanionQuerySettingOptions_HostAndPanelistsInOurOrganization;
		break;
	default:
		break;
	}
	return zn_option;
}
ZNMeetingAICompanionQueryRequestError Map2WrapDefine(ZOOM_SDK_NAMESPACE::MeetingAICompanionQueryRequestError error)
{
	ZNMeetingAICompanionQueryRequestError zn_error = ZNMeetingAICompanionQueryRequestError_OK;
	switch (error)
	{
		case ZOOM_SDK_NAMESPACE::MeetingAICompanionQueryRequestError_OK:
			zn_error = ZNMeetingAICompanionQueryRequestError_OK;
			break;
		case ZOOM_SDK_NAMESPACE::MeetingAICompanionQueryRequestError_InvalidParam:
			zn_error = ZNMeetingAICompanionQueryRequestError_InvalidParam;
			break;
		case ZOOM_SDK_NAMESPACE::MeetingAICompanionQueryRequestError_SendFailed:
			zn_error = ZNMeetingAICompanionQueryRequestError_SendFailed;
			break;
		case ZOOM_SDK_NAMESPACE::MeetingAICompanionQueryRequestError_WebUnAvailable:
			zn_error = ZNMeetingAICompanionQueryRequestError_WebUnAvailable;
			break;
		case ZOOM_SDK_NAMESPACE::MeetingAICompanionQueryRequestError_PermissionVerifyFailed:
			zn_error = ZNMeetingAICompanionQueryRequestError_PermissionVerifyFailed;
			break;
		case ZOOM_SDK_NAMESPACE::MeetingAICompanionQueryRequestError_QueryRateLimitError:
			zn_error = ZNMeetingAICompanionQueryRequestError_QueryRateLimitError;
			break;
		case ZOOM_SDK_NAMESPACE::MeetingAICompanionQueryRequestError_Timeout:
			zn_error = ZNMeetingAICompanionQueryRequestError_Timeout;
			break;
		case ZOOM_SDK_NAMESPACE::MeetingAICompanionQueryRequestError_Unknown:
			zn_error = ZNMeetingAICompanionQueryRequestError_Unknown;
			break;
		default:
			break;
	}
	return zn_error;
}
ZNAICompanionFeature Map2WrapDefine(ZOOM_SDK_NAMESPACE::AICompanionFeature feature)
{
	ZNAICompanionFeature zn_feature = ZNSMART_SUMMARY;
	switch (feature)
	{
		case ZOOM_SDK_NAMESPACE::SMART_SUMMARY:
			zn_feature = ZNSMART_SUMMARY;
			break;
		case ZOOM_SDK_NAMESPACE::QUERY:
			zn_feature = ZNQUERY;
			break;
		case ZOOM_SDK_NAMESPACE::SMART_RECORDING:
			zn_feature = ZNSMART_RECORDING;
			break;
		default:
			break;
	}
	return zn_feature;
}
ZNSDKPollingType Map2WrapDefine(ZOOM_SDK_NAMESPACE::SDKPollingType type)
{
	ZNSDKPollingType zn_type = ZNSDKPollingType_Unknown;
	switch (type)
	{
	case ZOOM_SDK_NAMESPACE::SDKPollingType_Unknown:
		zn_type = ZNSDKPollingType_Unknown;
		break;
	case ZOOM_SDK_NAMESPACE::SDKPollingType_Poll:
		zn_type = ZNSDKPollingType_Poll;
		break;
	case ZOOM_SDK_NAMESPACE::SDKPollingType_Quiz:
		zn_type = ZNSDKPollingType_Quiz;
		break;
	default:
		break;
	}
	return zn_type;
}
ZNSDKPollingStatus Map2WrapDefine(ZOOM_SDK_NAMESPACE::SDKPollingStatus status)
{
	ZNSDKPollingStatus zn_status = ZNSDKPollingStatus_Initial;
	switch (status)
	{
	case ZOOM_SDK_NAMESPACE::SDKPollingStatus_Initial:
		zn_status = ZNSDKPollingStatus_Initial;
		break;
	case ZOOM_SDK_NAMESPACE::SDKPollingStatus_Started:
		zn_status = ZNSDKPollingStatus_Started;
		break;
	case ZOOM_SDK_NAMESPACE::SDKPollingStatus_ShareResult:
		zn_status = ZNSDKPollingStatus_ShareResult;
		break;
	case ZOOM_SDK_NAMESPACE::SDKPollingStatus_Stopped:
		zn_status = ZNSDKPollingStatus_Stopped;
		break;
	default:
		break;
	}
	return zn_status;
}
ZNSDKPollingQuestionType Map2WrapDefine(ZOOM_SDK_NAMESPACE::SDKPollingQuestionType type)
{
	ZNSDKPollingQuestionType zn_type = ZNSDKPollingQuestionType_Unknown;
	switch (type)
	{
	case ZOOM_SDK_NAMESPACE::SDKPollingQuestionType_Unknown:
		zn_type = ZNSDKPollingQuestionType_Unknown;
		break;
	case ZOOM_SDK_NAMESPACE::SDKPollingQuestionType_Single:
		zn_type = ZNDKPollingQuestionType_Single;
		break;
	case ZOOM_SDK_NAMESPACE::SDKPollingQuestionType_Multi:
		zn_type = ZNSDKPollingQuestionType_Multi;
		break;
	case ZOOM_SDK_NAMESPACE::SDKPollingQuestionType_Matching:
		zn_type = ZNSDKPollingQuestionType_Matching;
		break;
	case ZOOM_SDK_NAMESPACE::SDKPollingQuestionType_RankOrder:
		zn_type = ZNSDKPollingQuestionType_RankOrder;
		break;
	case ZOOM_SDK_NAMESPACE::SDKPollingQuestionType_ShortAnswer:
		zn_type = ZNSDKPollingQuestionType_ShortAnswer;
		break;
	case ZOOM_SDK_NAMESPACE::SDKPollingQuestionType_LongAnswer:
		zn_type = ZNSDKPollingQuestionType_LongAnswer;
		break;
	case ZOOM_SDK_NAMESPACE::SDKPollingQuestionType_FillBlank:
		zn_type = ZNSDKPollingQuestionType_FillBlank;
		break;
	case ZOOM_SDK_NAMESPACE::SDKPollingQuestionType_RatingScale:
		zn_type = ZNSDKPollingQuestionType_NPS;
		break;
	case ZOOM_SDK_NAMESPACE::SDKPollingQuestionType_Dropdown:
		zn_type = ZNSDKPollingQuestionType_Dropdown;
		break;
	default:
		break;
	}
	return zn_type;
}
ZNSDKLiveTranscriptionOperationType Map2WrapDefine(ZOOM_SDK_NAMESPACE::SDKLiveTranscriptionOperationType messageType)
{
	ZNSDKLiveTranscriptionOperationType zn_messageType = ZNSDK_LiveTranscription_OperationType_None;
	switch (messageType)
	{
	case ZOOM_SDK_NAMESPACE::SDK_LiveTranscription_OperationType_None:
		zn_messageType = ZNSDK_LiveTranscription_OperationType_None;
		break;
	case ZOOM_SDK_NAMESPACE::SDK_LiveTranscription_OperationType_Add:
		zn_messageType = ZNSDK_LiveTranscription_OperationType_Add;
		break;
	case ZOOM_SDK_NAMESPACE::SDK_LiveTranscription_OperationType_Update:
		zn_messageType = ZNSDK_LiveTranscription_OperationType_Update;
		break;
	case ZOOM_SDK_NAMESPACE::SDK_LiveTranscription_OperationType_Delete:
		zn_messageType = ZNSDK_LiveTranscription_OperationType_Delete;
		break;
	case ZOOM_SDK_NAMESPACE::SDK_LiveTranscription_OperationType_Complete:
		zn_messageType = ZNSDK_LiveTranscription_OperationType_Complete;
		break;
	case ZOOM_SDK_NAMESPACE::SDK_LiveTranscription_OperationType_NotSupported:
		zn_messageType = ZNSDK_LiveTranscription_OperationType_NotSupported;
		break;
	default:
		break;
	}
	return zn_messageType;
}
ZNCannotShareReasonType Map2WrapDefine(ZOOM_SDK_NAMESPACE::CannotShareReasonType reason)
{
	ZNCannotShareReasonType zn_reason = ZNCannotShareReasonType_None;
	switch (reason)
	{
		case ZOOM_SDK_NAMESPACE::CannotShareReasonType_None:
			zn_reason = ZNCannotShareReasonType_None;
			break;
		case ZOOM_SDK_NAMESPACE::CannotShareReasonType_Locked:
			zn_reason = ZNCannotShareReasonType_Locked;
			break;
		case ZOOM_SDK_NAMESPACE::CannotShareReasonType_Disabled:
			zn_reason = ZNCannotShareReasonType_Disabled;
			break;
		case ZOOM_SDK_NAMESPACE::CannotShareReasonType_Other_Screen_Sharing:
			zn_reason = ZNCannotShareReasonType_Other_Screen_Sharing;
			break;
		case ZOOM_SDK_NAMESPACE::CannotShareReasonType_Other_WB_Sharing:
			zn_reason = ZNCannotShareReasonType_Other_WB_Sharing;
			break;
		case ZOOM_SDK_NAMESPACE::CannotShareReasonType_Need_Grab_Myself_Screen_Sharing:
			zn_reason = ZNCannotShareReasonType_Need_Grab_Myself_Screen_Sharing;
			break;
		case ZOOM_SDK_NAMESPACE::CannotShareReasonType_Need_Grab_Other_Screen_Sharing:
			zn_reason = ZNCannotShareReasonType_Need_Grab_Other_Screen_Sharing;
			break;
		case ZOOM_SDK_NAMESPACE::CannotShareReasonType_Need_Grab_Audio_Sharing:
			zn_reason = ZNCannotShareReasonType_Need_Grab_Audio_Sharing;
			break;
		case ZOOM_SDK_NAMESPACE::CannotShareReasonType_Need_Grap_WB_Sharing:
			zn_reason = ZNCannotShareReasonType_Need_Grap_WB_Sharing;
			break;
		case ZOOM_SDK_NAMESPACE::CannotShareReasonType_Reach_Maximum:
			zn_reason = ZNCannotShareReasonType_Reach_Maximum;
			break;
		case ZOOM_SDK_NAMESPACE::CannotShareReasonType_Have_Share_From_Mainsession:
			zn_reason = ZNCannotShareReasonType_Have_Share_From_Mainsession;
			break;
		case ZOOM_SDK_NAMESPACE::CannotShareReasonType_Other_DOCS_Sharing:
			zn_reason = ZNCannotShareReasonType_Other_DOCS_Sharing;
			break;
		case ZOOM_SDK_NAMESPACE::CannotShareReasonType_Need_Grab_DOCS_Sharing:
			zn_reason = ZNCannotShareReasonType_Need_Grab_DOCS_Sharing;
			break;
		case ZOOM_SDK_NAMESPACE::CannotShareReasonType_UnKnown:
			zn_reason = ZNCannotShareReasonType_UnKnown;
			break;
		default:
			break;
	}
	return zn_reason;
}
ZNSDKWhiteboardShareOption Map2WrapDefine(ZOOM_SDK_NAMESPACE::SDKWhiteboardShareOption option)
{
	ZNSDKWhiteboardShareOption zn_option = ZNSDKWhiteboardShareOption_HostShare;
	switch (option)
	{
		case ZOOM_SDK_NAMESPACE::SDKWhiteboardShareOption_HostShare:
			zn_option = ZNSDKWhiteboardShareOption_HostShare;
			break;
		case ZOOM_SDK_NAMESPACE::SDKWhiteboardShareOption_HostGrabShare:
			zn_option = ZNSDKWhiteboardShareOption_HostGrabShare;
			break;
		case ZOOM_SDK_NAMESPACE::SDKWhiteboardShareOption_AllGrabShare:
			zn_option = ZNSDKWhiteboardShareOption_AllGrabShare;
			break;
		default:
			break;
	}
	return zn_option;
}
ZNSDKWhiteboardCreateOption Map2WrapDefine(ZOOM_SDK_NAMESPACE::SDKWhiteboardCreateOption option)
{
	ZNSDKWhiteboardCreateOption zn_option = ZNSDKWhiteboardCreateOption_HostOnly;
	switch (option)
	{
		case ZOOM_SDK_NAMESPACE::SDKWhiteboardCreateOption_HostOnly:
			zn_option = ZNSDKWhiteboardCreateOption_HostOnly;
			break;
		case ZOOM_SDK_NAMESPACE::SDKWhiteboardCreateOption_AccountUsers:
			zn_option = ZNSDKWhiteboardCreateOption_AccountUsers;
			break;
		case ZOOM_SDK_NAMESPACE::SDKWhiteboardCreateOption_All:
			zn_option = ZNSDKWhiteboardCreateOption_All;
			break;
		default:
			break;
	}
	return zn_option;
}
ZNSDKWhiteboardStatus Map2WrapDefine(ZOOM_SDK_NAMESPACE::SDKWhiteboardStatus status)
{
	ZNSDKWhiteboardStatus zn_status = ZNSDKWhiteboardStatus_Started;
	switch (status)
	{
		case ZOOM_SDK_NAMESPACE::SDKWhiteboardStatus_Started:
			zn_status = ZNSDKWhiteboardStatus_Started;
			break;
		case ZOOM_SDK_NAMESPACE::SDKWhiteboardStatus_Stopped:
			zn_status = ZNSDKWhiteboardStatus_Stopped;
			break;
		default:
			break;
	}
	return zn_status;
}
ZNSDKDocsStatus Map2WrapDefine(ZOOM_SDK_NAMESPACE::SDKDocsStatus status)
{
	ZNSDKDocsStatus zn_status = ZNSDKDocsStatus_None;
	switch (status)
	{
		case ZOOM_SDK_NAMESPACE::SDKDocsStatus_None:
			zn_status = ZNSDKDocsStatus_None;
			break;
		case ZOOM_SDK_NAMESPACE::SDKDocsStatus_Start:
			zn_status = ZNSDKDocsStatus_Start;
			break;
		case ZOOM_SDK_NAMESPACE::SDKDocsStatus_Stop:
			zn_status = ZNSDKDocsStatus_Stop;
			break;
		default:
			break;
	}
	return zn_status;
}
ZNSDKDocsShareOption Map2WrapDefine(ZOOM_SDK_NAMESPACE::SDKDocsShareOption option)
{
	ZNSDKDocsShareOption zn_option = ZNSDKDocsShareOption_None;
	switch (option)
	{
		case ZOOM_SDK_NAMESPACE::SDKDocsShareOption_None:
			zn_option = ZNSDKDocsShareOption_None;
			break;
		case ZOOM_SDK_NAMESPACE::SDKDocsShareOption_HostShare:
			zn_option = ZNSDKDocsShareOption_HostShare;
			break;
		case ZOOM_SDK_NAMESPACE::SDKDocsShareOption_HostGrabShare:
			zn_option = ZNSDKDocsShareOption_HostGrabShare;
			break;
		case ZOOM_SDK_NAMESPACE::SDKDocsShareOption_AllGrabShare:
			zn_option = ZNSDKDocsShareOption_AllGrabShare;
			break;
		default:
			break;
	}
	return zn_option;
}
ZNSDKDocsCreateOption Map2WrapDefine(ZOOM_SDK_NAMESPACE::SDKDocsCreateOption option)
{
	ZNSDKDocsCreateOption zn_option = ZNSDKDocsCreateOption_None;
	switch (option)
	{
		case ZOOM_SDK_NAMESPACE::SDKDocsCreateOption_None:
			zn_option = ZNSDKDocsCreateOption_None;
			break;
		case ZOOM_SDK_NAMESPACE::SDKDocsCreateOption_HostOnly:
			zn_option = ZNSDKDocsCreateOption_HostOnly;
			break;
		case ZOOM_SDK_NAMESPACE::SDKDocsCreateOption_AccountUsers:
			zn_option = ZNSDKDocsCreateOption_AccountUsers;
			break;
		case ZOOM_SDK_NAMESPACE::SDKDocsCreateOption_All:
			zn_option = ZNSDKDocsCreateOption_All;
			break;
		default:
			break;
	}
	return zn_option;
}

ZNUserInfomation ConvertSDKUserInfoToZnUserInfo(ZOOM_SDK_NAMESPACE::IUserInfo* userInfo)
{
	ZNUserInfomation zn_userInfo;
	if (userInfo)
	{
		if (userInfo->GetUserName())
		{
			zn_userInfo.userName = userInfo->GetUserName();
		}
		zn_userInfo.isHost = userInfo->IsHost();
		zn_userInfo.userID = userInfo->GetUserID();
		if (userInfo->GetAvatarPath())
		{
			zn_userInfo.avatarPath = userInfo->GetAvatarPath();
		}
		if (userInfo->GetPersistentId())
		{
			zn_userInfo.persistentId = userInfo->GetPersistentId();
		}
		if (userInfo->GetCustomerKey())
		{
			zn_userInfo.customerKey = userInfo->GetCustomerKey();
		}
		zn_userInfo.isVideoOn = userInfo->IsVideoOn();
		zn_userInfo.isAudioMuted = userInfo->IsAudioMuted();
		zn_userInfo.isMySelf = userInfo->IsMySelf();
		zn_userInfo.userRole = Map2WrapDefine(userInfo->GetUserRole());
		zn_userInfo.isPurePhoneUser = userInfo->IsPurePhoneUser();
		zn_userInfo.isTalking = userInfo->IsTalking();
		zn_userInfo.isH323User = userInfo->IsH323User();
		if (userInfo->GetWebinarAttendeeStatus())
		{
			zn_userInfo.webinarAttendeeStatus.allow_talk = userInfo->GetWebinarAttendeeStatus()->allow_talk;
		}
		zn_userInfo.localRecordingStatus = Map2WrapDefine(userInfo->GetLocalRecordingStatus());
		zn_userInfo.userInfoType = ZN_REAL_USERINFO;
		zn_userInfo.isRawLiveStreaming = userInfo->IsRawLiveStreaming();
		zn_userInfo.HasRawLiveStreamPrivilege = userInfo->HasRawLiveStreamPrivilege();
		zn_userInfo.hasCamera = userInfo->HasCamera();
		zn_userInfo.audioJoinType = Map2WrapDefine(userInfo->GetAudioJoinType());
		zn_userInfo.isInWaitingRoom = userInfo->IsInWaitingRoom();
		zn_userInfo.isRaiseHand = userInfo->IsRaiseHand();
		zn_userInfo.audioVoiceLevel = userInfo->GetAudioVoiceLevel();
		zn_userInfo.isClosedCaptionSender = userInfo->IsClosedCaptionSender();
		zn_userInfo.isBotUser = userInfo->IsBotUser();
		if (userInfo->GetBotAppName())
		{
			zn_userInfo.botAppName = userInfo->GetBotAppName();
		}
		zn_userInfo.isInWebinarBackstage = userInfo->IsInWebinarBackstage();
		zn_userInfo.isInterpreter = userInfo->IsInterpreter();
		zn_userInfo.isSignLanguageInterpreter = userInfo->IsSignLanguageInterpreter();
		if (userInfo->GetInterpreterActiveLanguage())
		{
			zn_userInfo.interpreterActiveLanguage = userInfo->GetInterpreterActiveLanguage();
		}
		zn_userInfo.emojiFeedbackType = Map2WrapDefine(userInfo->GetEmojiFeedbackType());
	}
	return zn_userInfo;
}
ZNWallpaperItem ConvertSDKIWallpaperItemToZNWallpaperItem(ZOOM_SDK_NAMESPACE::IWallpaperItem* wallpaperItem)
{
	ZNWallpaperItem zn_wallpaperItem;
	if (wallpaperItem)
	{
		zn_wallpaperItem.wallpaperLayoutMode = Map2WrapDefine(wallpaperItem->GetWallpaperLayoutMode());
		if (wallpaperItem->GetWallpaperID())
		{
			zn_wallpaperItem.wallpaperId = wallpaperItem->GetWallpaperID();
		}
		if (wallpaperItem->GetTitle())
		{
			zn_wallpaperItem.title = wallpaperItem->GetTitle();
		}
		if (wallpaperItem->GetThumbnailPath())
		{
			zn_wallpaperItem.thumbnailPath = wallpaperItem->GetThumbnailPath();
		}
		if (wallpaperItem->GetPath())
		{
			zn_wallpaperItem.path = wallpaperItem->GetPath();
		}
		zn_wallpaperItem.transparency = wallpaperItem->GetTransparency();
	}
	return zn_wallpaperItem;
}
ZNVirtualBGImageInfo ConvertSDKVirtualBGImageInfoToZNVirtualBGImageInfo(ZOOM_SDK_NAMESPACE::IVirtualBGImageInfo* imageInfo)
{
	ZNVirtualBGImageInfo zn_imageInfo;
	zn_imageInfo.isSelected = imageInfo->isSelected();
	zn_imageInfo.isAllowDelete = imageInfo->isAllowDelete();
	if (imageInfo->GetImageFilePath())
	{
		zn_imageInfo.imageFilePath = imageInfo->GetImageFilePath();
	}
	if (imageInfo->GetImageName())
	{
		zn_imageInfo.imageName = imageInfo->GetImageName();
	}
	zn_imageInfo.imageHandle = (int64_t)imageInfo;
	return zn_imageInfo;
}
ZNZoomSDKSharingSourceInfo ConvertSDKSharingSourceInfoToZNSharingSourceInfo(ZOOM_SDK_NAMESPACE::ZoomSDKSharingSourceInfo shareInfo)
{
	ZNZoomSDKSharingSourceInfo zn_shareInfo;
	zn_shareInfo.userid = shareInfo.userid;
	zn_shareInfo.shareSourceID = shareInfo.shareSourceID;
	zn_shareInfo.status = Map2WrapDefine(shareInfo.status);
	zn_shareInfo.isShowingInFirstView = shareInfo.isShowingInFirstView;
	zn_shareInfo.isShowingInSecondView = shareInfo.isShowingInSecondView;
	zn_shareInfo.isCanBeRemoteControl = shareInfo.isCanBeRemoteControl;
	zn_shareInfo.bEnableOptimizingVideoSharing = shareInfo.bEnableOptimizingVideoSharing;
	zn_shareInfo.contentType = Map2WrapDefine(shareInfo.contentType);
	zn_shareInfo.windowID = reinterpret_cast<uint64_t>(shareInfo.hwndSharedApp);
	if (shareInfo.monitorID)
	{
		zn_shareInfo.monitorID = shareInfo.monitorID;
	}
	return zn_shareInfo;
}
ZNChatMsgInfo ConvertSDKChatMsgInfoToZNChatMsgInfo(ZOOM_SDK_NAMESPACE::IChatMsgInfo* chatMsg)
{
	ZNChatMsgInfo zn_chatMsg;
	if (chatMsg)
	{
		zn_chatMsg.messageID = chatMsg->GetMessageID();
		zn_chatMsg.senderUserId = chatMsg->GetSenderUserId();
		zn_chatMsg.senderDisplayName = chatMsg->GetSenderDisplayName();
		zn_chatMsg.receiverUserId = chatMsg->GetReceiverUserId();
		zn_chatMsg.receiverDisplayName = chatMsg->GetReceiverDisplayName();
		zn_chatMsg.content = chatMsg->GetContent();
		zn_chatMsg.timeStamp = chatMsg->GetTimeStamp();
		zn_chatMsg.isChatToAll = chatMsg->IsChatToAll();
		zn_chatMsg.isChatToAllPanelist = chatMsg->IsChatToAllPanelist();
		zn_chatMsg.isChatToWaitingroom = chatMsg->IsChatToWaitingroom();
		zn_chatMsg.chatMessageType = Map2WrapDefine(chatMsg->GetChatMessageType());
		zn_chatMsg.isComment = chatMsg->IsComment();
		zn_chatMsg.isThread = chatMsg->IsThread();
		zn_chatMsg.threadID = chatMsg->GetThreadID();
		ZOOM_SDK_NAMESPACE::IList<ZOOM_SDK_NAMESPACE::IRichTextStyleItem*>* plstRichTextStyleItem = chatMsg->GetTextStyleItemList();
		ZNList<ZNRichTextStyleItem> text_style_item_list;
		for (int i = 0; i < plstRichTextStyleItem->GetCount(); ++i)
		{
			ZOOM_SDK_NAMESPACE::IRichTextStyleItem* pRichTextStyleItem = plstRichTextStyleItem->GetItem(i);
			if (pRichTextStyleItem)
			{
				ZNRichTextStyleItem zn_rich_text_style_item;
				zn_rich_text_style_item.textStyle = (ZNRichTextStyle)pRichTextStyleItem->GetTextStyle();
				ZOOM_SDK_NAMESPACE::IList<ZOOM_SDK_NAMESPACE::IRichTextStyleOffset*>* plstTextStyleOffset = pRichTextStyleItem->GetTextStyleOffsetList();
				ZNList<ZNRichTextStyleOffset> rich_text_style_offset_list;
				for (int j = 0; j < plstTextStyleOffset->GetCount(); ++j)
				{
					ZOOM_SDK_NAMESPACE::IRichTextStyleOffset* pTextStyleOffset = plstTextStyleOffset->GetItem(j);
					if (pTextStyleOffset)
					{
						ZNRichTextStyleOffset zn_rich_text_style_offset;
						zn_rich_text_style_offset.positionStart = pTextStyleOffset->GetPositionStart();
						zn_rich_text_style_offset.positionEnd = pTextStyleOffset->GetPositionEnd();
						zn_rich_text_style_offset.reserve = pTextStyleOffset->GetReserve();
						rich_text_style_offset_list.push_back(zn_rich_text_style_offset);
					}
				}
				zn_rich_text_style_item.textStyleOffsetList = rich_text_style_offset_list;
				text_style_item_list.push_back(zn_rich_text_style_item);
			}
		}
		zn_chatMsg.textStyleItemList = text_style_item_list;
	}
	return zn_chatMsg;
}
ZNSDKPollingItem ConvertSDKIPollingItemToZNSDKPollingItem(ZOOM_SDK_NAMESPACE::IPollingItem* pollItem) 
{
	ZNSDKPollingItem zn_polling_info;
	if (pollItem->GetPollingID()) {
		zn_polling_info.pollingID = pollItem->GetPollingID();
	}
	if (pollItem->GetPollingName()) {
		zn_polling_info.pollingName = pollItem->GetPollingName();
	}
	zn_polling_info.pollingType = Map2WrapDefine(pollItem->GetPollingType());
	zn_polling_info.pollingStatus = Map2WrapDefine(pollItem->GetPollingStatus());
	zn_polling_info.pollingQuestionCount = pollItem->GetPollingQuestionCount();
	zn_polling_info.totalVotedUserCount = pollItem->GetTotalVotedUserCount();
	zn_polling_info.isLibraryPolling = pollItem->IsLibraryPolling();

	return zn_polling_info;
}
ZNSDKPollingAnswerItem ConvertSDKIPollingAnswerItemToZNSDKPollingAnswerItem(ZOOM_SDK_NAMESPACE::IPollingAnswerItem* answerItem) 
{
	ZNSDKPollingAnswerItem zn_answer_info;
	if (answerItem->GetPollingID()) {
		zn_answer_info.pollingID = answerItem->GetPollingID();
	}
	if (answerItem->GetPollingQuestionID()) {
		zn_answer_info.pollingQuestionID = answerItem->GetPollingQuestionID();
	}
	if (answerItem->GetPollingSubQuestionID()) {
		zn_answer_info.pollingSubQuestionID = answerItem->GetPollingSubQuestionID();
	}
	if (answerItem->GetPollingAnswerID()) {
		zn_answer_info.pollingAnswerID = answerItem->GetPollingAnswerID();
	}
	if (answerItem->GetPollingAnswerName()) {
		zn_answer_info.pollingAnswerName = answerItem->GetPollingAnswerName();
	}
	if (answerItem->GetPollingAnsweredContent()) {
		zn_answer_info.pollingAnsweredContent = answerItem->GetPollingAnsweredContent();
	}
	zn_answer_info.isChecked = answerItem->IsChecked();
	return zn_answer_info;
}
ZNSDKPollingAnswerResultItem ConvertSDKIPollingAnswerResultItemToZNSDKPollingAnswerResultItem(ZOOM_SDK_NAMESPACE::IPollingAnswerResultItem* answerResultItem)
{
	ZNSDKPollingAnswerResultItem zn_answer_result_info;
	if (answerResultItem->GetPollingID()) {
		zn_answer_result_info.pollingID = answerResultItem->GetPollingID();
	}
	if (answerResultItem->GetPollingQuestionID()) {
		zn_answer_result_info.pollingQuestionID = answerResultItem->GetPollingQuestionID();
	}
	if (answerResultItem->GetPollingSubQuestionID()) {
		zn_answer_result_info.pollingSubQuestionID = answerResultItem->GetPollingSubQuestionID();
	}
	if (answerResultItem->GetPollingAnswerID()) {
		zn_answer_result_info.pollingAnswerID = answerResultItem->GetPollingAnswerID();
	}
	if (answerResultItem->GetPollingAnswerName()) {
		zn_answer_result_info.pollingAnswerName = answerResultItem->GetPollingAnswerName();
	}
	zn_answer_result_info.selectedCount = answerResultItem->GetSelectedCount();
	return zn_answer_result_info;
}
ZNSDKPollingQuestionItem ConvertSDKIPollingQuestionItemToZNSDKPollingQuestionItem(ZOOM_SDK_NAMESPACE::IPollingQuestionItem* questionItem)
{
	ZNSDKPollingQuestionItem zn_question_info;
	if (questionItem->GetPollingID()) {
		zn_question_info.pollingID = questionItem->GetPollingID();
	}
	if (questionItem->GetPollingQuestionID()) {
		zn_question_info.pollingQuestionID = questionItem->GetPollingQuestionID();
	}
	if (questionItem->GetPollingQuestionName()) {
		zn_question_info.pollingQuestionName = questionItem->GetPollingQuestionName();
	}
	zn_question_info.pollingQuestionType = Map2WrapDefine(questionItem->GetPollingQuestionType());
	zn_question_info.answeredCount = questionItem->GetAnsweredCount();
	zn_question_info.isRequired = questionItem->IsRequired();
	return zn_question_info;
}
ZNLiveTranscriptionMessageInfo ConvertSDKILiveTranscriptionMessageInfoToZNLiveTranscriptionMessageInfo(ZOOM_SDK_NAMESPACE::ILiveTranscriptionMessageInfo* messageInfo)
{
	ZNLiveTranscriptionMessageInfo zn_message_info;
	if (messageInfo->GetMessageID()) {
		zn_message_info.messageID = messageInfo->GetMessageID();
	}
	if (messageInfo->GetSpeakerID()) {
		zn_message_info.speakerID = messageInfo->GetSpeakerID();
	}
	if (messageInfo->GetSpeakerName()) {
		zn_message_info.speakerName = messageInfo->GetSpeakerName();
	}
	if (messageInfo->GetMessageContent()) {
		zn_message_info.messageContent = messageInfo->GetMessageContent();
	}
	zn_message_info.timeStamp = messageInfo->GetTimeStamp();
	zn_message_info.messageType =	Map2WrapDefine(messageInfo->GetMessageOperationType());
	return zn_message_info;
}
ZNZoomSDKDocSharingSourceInfo ConvertSDKZoomSDKDocSharingSourceInfoToZNZoomSDKDocSharingSourceInfo(ZOOM_SDK_NAMESPACE::IZoomSDKDocSharingSourceInfo* sourceinfo)
{
    ZNZoomSDKDocSharingSourceInfo zn_sourceinfo;
    zn_sourceinfo.status = Map2WrapDefine(sourceinfo->GetStatus());
    if (sourceinfo->GetDocTitle())
    {
        zn_sourceinfo.docTitle = sourceinfo->GetDocTitle();
    }
    zn_sourceinfo.shareSourceID = sourceinfo->GetShareSourceID();
    zn_sourceinfo.userID = sourceinfo->GetUserID();
    return zn_sourceinfo;
}
/////Covert JS define to SDK
ZOOM_SDK_NAMESPACE::SDK_LANGUAGE_ID Map2SDKDefine(ZNSDK_LANGUAGE_ID id)
{
	ZOOM_SDK_NAMESPACE::SDK_LANGUAGE_ID sdk_id = ZOOM_SDK_NAMESPACE::LANGUAGE_Unknown;
	switch (id)
	{
	case ZNLANGUAGE_Unknown:
		sdk_id = ZOOM_SDK_NAMESPACE::LANGUAGE_Unknown;
		break;
	case ZNLANGUAGE_English:
		sdk_id = ZOOM_SDK_NAMESPACE::LANGUAGE_English;
		break;
	case ZNLANGUAGE_Chinese_Simplified:
		sdk_id = ZOOM_SDK_NAMESPACE::LANGUAGE_Chinese_Simplified;
		break;
	case ZNLANGUAGE_Chinese_Traditional:
		sdk_id = ZOOM_SDK_NAMESPACE::LANGUAGE_Chinese_Traditional;
		break;
	case ZNLANGUAGE_Japanese:
		sdk_id = ZOOM_SDK_NAMESPACE::LANGUAGE_Japanese;
		break;
	case ZNLANGUAGE_Spanish:
		sdk_id = ZOOM_SDK_NAMESPACE::LANGUAGE_Spanish;
		break;
	case ZNLANGUAGE_German:
		sdk_id = ZOOM_SDK_NAMESPACE::LANGUAGE_German;
		break;
	case ZNLANGUAGE_French:
		sdk_id = ZOOM_SDK_NAMESPACE::LANGUAGE_French;
		break;
	case ZNLANGUAGE_Portuguese:
		sdk_id = ZOOM_SDK_NAMESPACE::LANGUAGE_Portuguese;
		break;
	case ZNLANGUAGE_Russian:
		sdk_id = ZOOM_SDK_NAMESPACE::LANGUAGE_Russian;
		break;
	case ZNLANGUAGE_Korean:
		sdk_id = ZOOM_SDK_NAMESPACE::LANGUAGE_Korean;
		break;
	case ZNLANGUAGE_Vietnamese:
		sdk_id = ZOOM_SDK_NAMESPACE::LANGUAGE_Vietnamese;
		break;
	case ZNLANGUAGE_Italian:
		sdk_id = ZOOM_SDK_NAMESPACE::LANGUAGE_Italian;
		break;
	case ZNLANGUAGE_Polish:
		sdk_id = ZOOM_SDK_NAMESPACE::LANGUAGE_Polish;
		break;
	case ZNLANGUAGE_Turkish:
		sdk_id = ZOOM_SDK_NAMESPACE::LANGUAGE_Turkish;
		break;
	case ZNLANGUAGE_Indonesian:
		sdk_id = ZOOM_SDK_NAMESPACE::LANGUAGE_Indonesian;
		break;
	case ZNLANGUAGE_Dutch:
		sdk_id = ZOOM_SDK_NAMESPACE::LANGUAGE_Dutch;
		break;
	default:
		break;
	}

	return sdk_id;
}
ZOOM_SDK_NAMESPACE::SDKUserType Map2SDKDefine(ZNSDKUserType type)
{
	ZOOM_SDK_NAMESPACE::SDKUserType sdk_user_type= ZOOM_SDK_NAMESPACE::SDK_UT_WITHOUT_LOGIN;
	switch (type)
	{
	case ZNSDK_UT_NORMALUSER:
		sdk_user_type = ZOOM_SDK_NAMESPACE::SDK_UT_NORMALUSER;
		break;
	case ZNSDK_UT_WITHOUT_LOGIN:
		sdk_user_type = ZOOM_SDK_NAMESPACE::SDK_UT_WITHOUT_LOGIN;
		break;
	default:
		break;
	}
	return sdk_user_type;
}
ZOOM_SDK_NAMESPACE::LeaveMeetingCmd Map2SDKDefine(ZNLeaveMeetingCmd cmd)
{
	ZOOM_SDK_NAMESPACE::LeaveMeetingCmd sdk_leave_meeting_cmd;
	switch (cmd)
	{
	case ZNLEAVE_MEETING:
		sdk_leave_meeting_cmd = ZOOM_SDK_NAMESPACE::LEAVE_MEETING;
		break;
	case ZNEND_MEETING:
		sdk_leave_meeting_cmd = ZOOM_SDK_NAMESPACE::END_MEETING;
		break;
	default:
		break;
	}
	return sdk_leave_meeting_cmd;
}
ZOOM_SDK_NAMESPACE::ZoomUserType Map2SDKDefine(ZNZoomUserType type)
{
	ZOOM_SDK_NAMESPACE::ZoomUserType sdk_zoom_user_type;
	switch (type)
	{
	case ZNZoomUserType_APIUSER:
		sdk_zoom_user_type = ZOOM_SDK_NAMESPACE::ZoomUserType_APIUSER;
		break;
	case ZNZoomUserType_EMAIL_LOGIN:
		sdk_zoom_user_type = ZOOM_SDK_NAMESPACE::ZoomUserType_EMAIL_LOGIN;
		break;
	case ZNZoomUserType_FACEBOOK:
		sdk_zoom_user_type = ZOOM_SDK_NAMESPACE::ZoomUserType_FACEBOOK;
		break;
	case ZNZoomUserType_GoogleOAuth:
		sdk_zoom_user_type = ZOOM_SDK_NAMESPACE::ZoomUserType_GoogleOAuth;
		break;
	case ZNZoomUserType_SSO:
		sdk_zoom_user_type = ZOOM_SDK_NAMESPACE::ZoomUserType_SSO;
		break;
	case ZNZoomUserType_Unknown:
		sdk_zoom_user_type = ZOOM_SDK_NAMESPACE::ZoomUserType_Unknown;
		break;
	default:
		break;
	}
	return sdk_zoom_user_type;
}
ZOOM_SDK_NAMESPACE::SDKFloatVideoType Map2SDKDefine(ZNSDKFloatVideoType type)
{
	ZOOM_SDK_NAMESPACE::SDKFloatVideoType sdk_float_video_type;
	switch (type)
	{
	case ZN_FLOATVIDEO_List:
		sdk_float_video_type = ZOOM_SDK_NAMESPACE::FLOATVIDEO_List;
		break;
	case ZN_FLOATVIDEO_Small:
		sdk_float_video_type = ZOOM_SDK_NAMESPACE::FLOATVIDEO_Small;
		break;
	case ZN_FLOATVIDEO_Large:
		sdk_float_video_type = ZOOM_SDK_NAMESPACE::FLOATVIDEO_Large;
		break;
	case ZN_FLOATVIDEO_Minimize:
		sdk_float_video_type = ZOOM_SDK_NAMESPACE::FLOATVIDEO_Minimize;
		break;
	default:
		break;
	}
	return sdk_float_video_type;
}
ZOOM_SDK_NAMESPACE::SDKViewType Map2SDKDefine(ZNSDKViewType type)
{
	ZOOM_SDK_NAMESPACE::SDKViewType sdk_view_type;
	switch (type)
	{
	case ZNSDK_FIRST_VIEW:
		sdk_view_type = ZOOM_SDK_NAMESPACE::SDK_FIRST_VIEW;
		break;
	case ZNSDK_SECOND_VIEW:
		sdk_view_type = ZOOM_SDK_NAMESPACE::SDK_SECOND_VIEW;
		break;
	case ZNSDK_SEND_SHARE_VIEW:
		sdk_view_type = ZOOM_SDK_NAMESPACE::SDK_SEND_SHARE_VIEW;
		break;
	default:
		break;
	}
	return sdk_view_type;
}
ZOOM_SDK_NAMESPACE::AnnotationToolType Map2SDKDefine(ZNAnnotationToolType type)
{
	ZOOM_SDK_NAMESPACE::AnnotationToolType sdk_annotation_tool_type;
	switch (type)
	{
	case ZN_ANNOTOOL_NONE_DRAWING:
		sdk_annotation_tool_type = ZOOM_SDK_NAMESPACE::ANNOTOOL_NONE_DRAWING;
		break;
	case ZN_ANNOTOOL_PEN:
		sdk_annotation_tool_type = ZOOM_SDK_NAMESPACE::ANNOTOOL_PEN;
		break;
	case ZN_ANNOTOOL_HIGHLIGHTER:
		sdk_annotation_tool_type = ZOOM_SDK_NAMESPACE::ANNOTOOL_HIGHLIGHTER;
		break;
	case ZN_ANNOTOOL_AUTO_LINE:
		sdk_annotation_tool_type = ZOOM_SDK_NAMESPACE::ANNOTOOL_AUTO_LINE;
		break;
	case ZN_ANNOTOOL_AUTO_RECTANGLE:
		sdk_annotation_tool_type = ZOOM_SDK_NAMESPACE::ANNOTOOL_AUTO_RECTANGLE;
		break;
	case ZN_ANNOTOOL_AUTO_ELLIPSE:
		sdk_annotation_tool_type = ZOOM_SDK_NAMESPACE::ANNOTOOL_AUTO_ELLIPSE;
		break;
	case ZN_ANNOTOOL_AUTO_ARROW:
		sdk_annotation_tool_type = ZOOM_SDK_NAMESPACE::ANNOTOOL_AUTO_ARROW;
		break;
	case ZN_ANNOTOOL_AUTO_RECTANGLE_FILL:
		sdk_annotation_tool_type = ZOOM_SDK_NAMESPACE::ANNOTOOL_AUTO_RECTANGLE_FILL;
		break;
	case ZN_ANNOTOOL_AUTO_ELLIPSE_FILL:
		sdk_annotation_tool_type = ZOOM_SDK_NAMESPACE::ANNOTOOL_AUTO_ELLIPSE_FILL;
		break;
	case ZN_ANNOTOOL_SPOTLIGHT:
		sdk_annotation_tool_type = ZOOM_SDK_NAMESPACE::ANNOTOOL_SPOTLIGHT;
		break;
	case ZN_ANNOTOOL_ARROW:
		sdk_annotation_tool_type = ZOOM_SDK_NAMESPACE::ANNOTOOL_ARROW;
		break;
	case ZN_ANNOTOOL_ERASER:
		sdk_annotation_tool_type = ZOOM_SDK_NAMESPACE::ANNOTOOL_ERASER;
		break;
	case ZN_ANNOTOOL_TEXTBOX:
		sdk_annotation_tool_type = ZOOM_SDK_NAMESPACE::ANNOTOOL_TEXTBOX;
		break;
	case ZN_ANNOTOOL_PICKER:
		sdk_annotation_tool_type = ZOOM_SDK_NAMESPACE::ANNOTOOL_PICKER;
		break;
	case ZN_ANNOTOOL_AUTO_RECTANGLE_SEMI_FILL:
		sdk_annotation_tool_type = ZOOM_SDK_NAMESPACE::ANNOTOOL_AUTO_RECTANGLE_SEMI_FILL;
		break;
	case ZN_ANNOTOOL_AUTO_ELLIPSE_SEMI_FILL:
		sdk_annotation_tool_type = ZOOM_SDK_NAMESPACE::ANNOTOOL_AUTO_ELLIPSE_SEMI_FILL;
		break;
	case ZN_ANNOTOOL_AUTO_DOUBLE_ARROW:
		sdk_annotation_tool_type = ZOOM_SDK_NAMESPACE::ANNOTOOL_AUTO_DOUBLE_ARROW;
		break;
	case ZN_ANNOTOOL_AUTO_DIAMOND:
		sdk_annotation_tool_type = ZOOM_SDK_NAMESPACE::ANNOTOOL_AUTO_DIAMOND;
		break;
	case ZN_ANNOTOOL_AUTO_STAMP_ARROW:
		sdk_annotation_tool_type = ZOOM_SDK_NAMESPACE::ANNOTOOL_AUTO_STAMP_ARROW;
		break;
	case ZN_ANNOTOOL_AUTO_STAMP_CHECK:
		sdk_annotation_tool_type = ZOOM_SDK_NAMESPACE::ANNOTOOL_AUTO_STAMP_CHECK;
		break;
	case ZN_ANNOTOOL_AUTO_STAMP_X:
		sdk_annotation_tool_type = ZOOM_SDK_NAMESPACE::ANNOTOOL_AUTO_STAMP_X;
		break;
	case ZN_ANNOTOOL_AUTO_STAMP_STAR:
		sdk_annotation_tool_type = ZOOM_SDK_NAMESPACE::ANNOTOOL_AUTO_STAMP_STAR;
		break;
	case ZN_ANNOTOOL_AUTO_STAMP_HEART:
		sdk_annotation_tool_type = ZOOM_SDK_NAMESPACE::ANNOTOOL_AUTO_STAMP_HEART;
		break;
	case ZN_ANNOTOOL_AUTO_STAMP_QM:
		sdk_annotation_tool_type = ZOOM_SDK_NAMESPACE::ANNOTOOL_AUTO_STAMP_QM;
		break;
	default:
		break;
	}
	return sdk_annotation_tool_type;
}
ZOOM_SDK_NAMESPACE::AnnotationClearType Map2SDKDefine(ZNAnnotationClearType type)
{
	ZOOM_SDK_NAMESPACE::AnnotationClearType sdk_annotation_clear_type;
	switch (type)
	{
	case ZN_ANNOCLEAR_ALL:
		sdk_annotation_clear_type = ZOOM_SDK_NAMESPACE::ANNOCLEAR_ALL;
		break;
	case ZN_ANNOCLEAR_SELF:
		sdk_annotation_clear_type = ZOOM_SDK_NAMESPACE::ANNOCLEAR_SELF;
		break;
	case ZN_ANNOCLEAR_OTHER:
		sdk_annotation_clear_type = ZOOM_SDK_NAMESPACE::ANNOCLEAR_OTHER;
		break;
	default:
		break;
	}
	return sdk_annotation_clear_type;
}
ZOOM_SDK_NAMESPACE::SettingTabPage Map2SDKDefine(ZNSettingTabPage tabPage)
{
	ZOOM_SDK_NAMESPACE::SettingTabPage sdk_tabPage;
	switch (tabPage)
	{
	case ZN_SettingTabPage_General:
		sdk_tabPage = ZOOM_SDK_NAMESPACE::SettingTabPage_General;
		break;
	case ZN_SettingTabPage_Audio:
		sdk_tabPage = ZOOM_SDK_NAMESPACE::SettingTabPage_Audio;
		break;
	case ZN_SettingTabPage_Video:
		sdk_tabPage = ZOOM_SDK_NAMESPACE::SettingTabPage_Video;
		break;
	default:
		break;
	}
	return sdk_tabPage;
}
ZOOM_SDK_NAMESPACE::SDKCustomizedStringType Map2SDKDefine(ZNSDKCustomizedStringType type)
{
	ZOOM_SDK_NAMESPACE::SDKCustomizedStringType sdk_type;
	switch (type)
	{
	case ZN_SDK_Customized_LiveStream_MenuString_LiveOn_String:
		sdk_type = ZOOM_SDK_NAMESPACE::SDK_Customized_LiveStream_MenuString_LiveOn_String;
		break;
	case ZN_SDK_Customized_LiveStream_MenuString_LiveView_String:
		sdk_type = ZOOM_SDK_NAMESPACE::SDK_Customized_LiveStream_MenuString_LiveView_String;
		break;
	case ZN_SDK_Customized_LiveStream_MenuString_LiveStop_String:
		sdk_type = ZOOM_SDK_NAMESPACE::SDK_Customized_LiveStream_MenuString_LiveStop_String;
		break;
	case ZN_SDK_Customized_Title_App:
		sdk_type = ZOOM_SDK_NAMESPACE::SDK_Customized_Title_App;
		break;
	case ZN_SDK_Customized_Title_ZoomVideo:
		sdk_type = ZOOM_SDK_NAMESPACE::SDK_Customized_Title_ZoomVideo;
		break;
	case ZN_SDK_Customized_Title_FreeZoomVideo:
		sdk_type = ZOOM_SDK_NAMESPACE::SDK_Customized_Title_FreeZoomVideo;
		break;
	case ZN_SDK_Customized_Title_ViewOnly_FreeZoomVideo:
		sdk_type = ZOOM_SDK_NAMESPACE::SDK_Customized_Title_ViewOnly_FreeZoomVideo;
		break;
	default:
		break;
	}
	return sdk_type;
}
ZOOM_SDK_NAMESPACE::SDKCustomizedURLType Map2SDKDefine(ZNSDKCustomizedURLType type)
{
	ZOOM_SDK_NAMESPACE::SDKCustomizedURLType sdk_type;
	switch (type)
	{
	case ZN_SDKCustomizedURL_VITRULBG_HELP:
		sdk_type = ZOOM_SDK_NAMESPACE::SDKCustomizedURL_VITRULBG_HELP;
		break;
	case ZN_SDKCustomizedURL_VITRULBG_LEARN_MORE:
		sdk_type = ZOOM_SDK_NAMESPACE::SDKCustomizedURL_VITRULBG_LEARN_MORE;
		break;
	case ZN_SDKCustomizedURL_SUPPORTURL:
		sdk_type = ZOOM_SDK_NAMESPACE::SDKCustomizedURL_SUPPORTURL;
		break;
	default:
		break;
	}
	return sdk_type;
}
ZOOM_SDK_NAMESPACE::H323DeviceType Map2SDKDefine(ZNH323DeviceType type)
{
	ZOOM_SDK_NAMESPACE::H323DeviceType sdk_h323_type;
	switch (type)
	{
	case ZN_H323DeviceType_Unknown:
		sdk_h323_type = ZOOM_SDK_NAMESPACE::H323DeviceType_Unknown;
		break;
	case ZN_H323DeviceType_H323:
		sdk_h323_type = ZOOM_SDK_NAMESPACE::H323DeviceType_H323;
		break;
	case ZN_H323DeviceType_SIP:
		sdk_h323_type = ZOOM_SDK_NAMESPACE::H323DeviceType_SIP;
		break;
	case ZN_H323DeviceType_BOTH:
		sdk_h323_type = ZOOM_SDK_NAMESPACE::H323DeviceType_BOTH;
		break;
	default:
		break;
	}
	return sdk_h323_type;
}
ZOOM_SDK_NAMESPACE::SDKMinimizeUIMode Map2SDKDefine(ZNSDKMinimizeUIMode mode)
{
	ZOOM_SDK_NAMESPACE::SDKMinimizeUIMode sdk_mode;
	switch (mode)
	{
	case ZN_MinimizeUIMode_NONE:
		sdk_mode = ZOOM_SDK_NAMESPACE::MinimizeUIMode_NONE;
		break;
	case ZN_MinimizeUIMode_SHARE:
		sdk_mode = ZOOM_SDK_NAMESPACE::MinimizeUIMode_SHARE;
		break;
	case ZN_MinimizeUIMode_VIDEO:
		sdk_mode = ZOOM_SDK_NAMESPACE::MinimizeUIMode_VIDEO;
		break;
	case ZN_MinimizeUIMode_ACTIVESPEAKER:
		sdk_mode = ZOOM_SDK_NAMESPACE::MinimizeUIMode_ACTIVESPEAKER;
		break;
	default:
		break;
	}
	return sdk_mode;
}
ZOOM_SDK_NAMESPACE::SDK_APP_Locale Map2SDKDefine(ZNSDK_APP_Locale locale)
{
	ZOOM_SDK_NAMESPACE::SDK_APP_Locale sdk_locale;
	switch (locale)
	{
	case ZNSDK_APP_Locale_Default:
		sdk_locale = ZOOM_SDK_NAMESPACE::SDK_APP_Locale_Default;
		break;
	case ZNSDK_APP_Locale_CN:
		sdk_locale = ZOOM_SDK_NAMESPACE::SDK_APP_Locale_CN;
		break;
	default:
		break;
	}
	return sdk_locale;
}
ZOOM_SDK_NAMESPACE::CustomizedLanguageType Map2SDKDefine(ZNCustomizedLanguageType customizedLanguageType)
{
	ZOOM_SDK_NAMESPACE::CustomizedLanguageType sdk_customizedLanguageType = ZOOM_SDK_NAMESPACE::CustomizedLanguage_None;
	switch (customizedLanguageType)
	{
	case ZN_CustomizedLanguage_None:
		sdk_customizedLanguageType = ZOOM_SDK_NAMESPACE::CustomizedLanguage_None;
		break;
	case ZN_CustomizedLanguage_FilePath:
		sdk_customizedLanguageType = ZOOM_SDK_NAMESPACE::CustomizedLanguage_FilePath;
		break;
	case ZN_CustomizedLanguage_Content:
		sdk_customizedLanguageType = ZOOM_SDK_NAMESPACE::CustomizedLanguage_Content;
		break;
	default:
		break;
	}
	return sdk_customizedLanguageType;
}

ZOOM_SDK_NAMESPACE::ZoomSDKVideoRenderMode Map2SDKDefine(ZNZoomSDKVideoRenderMode videoRenderType)
{
	ZOOM_SDK_NAMESPACE::ZoomSDKVideoRenderMode sdk_videoRenderType = ZOOM_SDK_NAMESPACE::ZoomSDKVideoRenderMode_None;
	switch (videoRenderType)
	{
	case ZNSDKVideoRenderMode_None:
		sdk_videoRenderType = ZOOM_SDK_NAMESPACE::ZoomSDKVideoRenderMode_None;
		break;
	case ZNSDKVideoRenderMode_Auto:
		sdk_videoRenderType = ZOOM_SDK_NAMESPACE::ZoomSDKVideoRenderMode_Auto;
		break;
	case ZNSDKVideoRenderMode_D3D11EnableFLIP:
		sdk_videoRenderType = ZOOM_SDK_NAMESPACE::ZoomSDKVideoRenderMode_D3D11EnableFLIP;
		break;
	case ZNSDKVideoRenderMode_D3D11:
		sdk_videoRenderType = ZOOM_SDK_NAMESPACE::ZoomSDKVideoRenderMode_D3D11;
		break;
	case ZNSDKVideoRenderMode_D3D9:
		sdk_videoRenderType = ZOOM_SDK_NAMESPACE::ZoomSDKVideoRenderMode_D3D9;
		break;
	case ZNSDKVideoRenderMode_GDI:
		sdk_videoRenderType = ZOOM_SDK_NAMESPACE::ZoomSDKVideoRenderMode_GDI;
		break;
	default:
		break;
	}
	return sdk_videoRenderType;
}
ZOOM_SDK_NAMESPACE::ZoomSDKRawDataMemoryMode Map2SDKDefine(ZNSDKRawDataMemoryMode rawdataMemoryMode)
{
	ZOOM_SDK_NAMESPACE::ZoomSDKRawDataMemoryMode sdk_rawdataMemoryMode = ZOOM_SDK_NAMESPACE::ZoomSDKRawDataMemoryModeStack;
	switch (rawdataMemoryMode)
	{
	case ZNSDKRawDataMemoryModeStack:
		sdk_rawdataMemoryMode = ZOOM_SDK_NAMESPACE::ZoomSDKRawDataMemoryModeStack;
		break;
	case ZNSDKRawDataMemoryModeHeap:
		sdk_rawdataMemoryMode = ZOOM_SDK_NAMESPACE::ZoomSDKRawDataMemoryModeHeap;
		break;
	default:
		break;
	}
	return sdk_rawdataMemoryMode;
}
ZOOM_SDK_NAMESPACE::ZoomSDKRawDataType Map2SDKDefine(ZNZoomSDKRawDataType type)
{
	ZOOM_SDK_NAMESPACE::ZoomSDKRawDataType sdk_Type = ZOOM_SDK_NAMESPACE::RAW_DATA_TYPE_VIDEO;
	switch (type)
	{
	case ZNRAW_DATA_TYPE_VIDEO:
		sdk_Type = ZOOM_SDK_NAMESPACE::RAW_DATA_TYPE_VIDEO;
		break;
	case ZNRAW_DATA_TYPE_SHARE:
		sdk_Type = ZOOM_SDK_NAMESPACE::RAW_DATA_TYPE_SHARE;
		break;
	default:
		break;
	}
	return sdk_Type;
}

ZOOM_SDK_NAMESPACE::VIDEO_HARDWARE_ENCODE_TYPE Map2SDKDefine(ZN_VIDEO_HARDWARE_ENCODE_TYPE encodeType)
{
	ZOOM_SDK_NAMESPACE::VIDEO_HARDWARE_ENCODE_TYPE sdk_encodeType = ZOOM_SDK_NAMESPACE::VIDEO_HARDWARE_ENCODE_RECEIVING;
	switch (encodeType)
	{
	case ZN_VIDEO_HARDWARE_ENCODE_RECEIVING:
		sdk_encodeType = ZOOM_SDK_NAMESPACE::VIDEO_HARDWARE_ENCODE_RECEIVING;
		break;
	case ZN_VIDEO_HARDWARE_ENCODE_SENDING:
		sdk_encodeType = ZOOM_SDK_NAMESPACE::VIDEO_HARDWARE_ENCODE_SENDING;
		break;
	case ZN_VIDEO_HARDWARE_ENCODE_PROCESSING:
		sdk_encodeType = ZOOM_SDK_NAMESPACE::VIDEO_HARDWARE_ENCODE_PROCESSING;
		break;
	default:
		break;
	}
	return sdk_encodeType;
}

ZOOM_SDK_NAMESPACE::ZoomSDKRenderPostProcessing Map2SDKDefine(ZNZoomSDKRenderPostProcessing postProcessing)
{
	ZOOM_SDK_NAMESPACE::ZoomSDKRenderPostProcessing sdk_postProcessing = ZOOM_SDK_NAMESPACE::ZoomSDKRenderPostProcessing_None;
	switch (postProcessing)
	{
	case ZNZoomSDKRenderPostProcessing_None:
		sdk_postProcessing = ZOOM_SDK_NAMESPACE::ZoomSDKRenderPostProcessing_None;
		break;
	case ZNZoomSDKRenderPostProcessing_Auto:
		sdk_postProcessing = ZOOM_SDK_NAMESPACE::ZoomSDKRenderPostProcessing_Auto;
		break;
	case ZNZoomSDKRenderPostProcessing_Enable:
		sdk_postProcessing = ZOOM_SDK_NAMESPACE::ZoomSDKRenderPostProcessing_Enable;
		break;
	case ZNZoomSDKRenderPostProcessing_Disable:
		sdk_postProcessing = ZOOM_SDK_NAMESPACE::ZoomSDKRenderPostProcessing_Disable;
		break;
	default:
		break;
	}
	return sdk_postProcessing;
}

ZOOM_SDK_NAMESPACE::ZoomSDKVideoCaptureMethod Map2SDKDefine(ZNZoomSDKVideoCaptureMethod method)
{
	ZOOM_SDK_NAMESPACE::ZoomSDKVideoCaptureMethod sdk_method = ZOOM_SDK_NAMESPACE::ZoomSDKVideoCaptureMethod_None;
	switch (sdk_method)
	{
	case ZNZoomSDKVideoCaptureMethod_None:
		sdk_method = ZOOM_SDK_NAMESPACE::ZoomSDKVideoCaptureMethod_None;
		break;
	case ZNZoomSDKVideoCaptureMethod_Auto:
		sdk_method = ZOOM_SDK_NAMESPACE::ZoomSDKVideoCaptureMethod_Auto;
		break;
	case ZNZoomSDKVideoCaptureMethod_DirectSHow:
		sdk_method = ZOOM_SDK_NAMESPACE::ZoomSDKVideoCaptureMethod_DirectSHow;
		break;
	case ZNZoomSDKVideoCaptureMethod_MediaFoundation:
		sdk_method = ZOOM_SDK_NAMESPACE::ZoomSDKVideoCaptureMethod_MediaFoundation;
		break;
	default:
		break;
	}
	return sdk_method;
}

ZOOM_SDK_NAMESPACE::ZoomSDKResolution Map2SDKDefine(ZNZoomSDKResolution resolution)
{
	ZOOM_SDK_NAMESPACE::ZoomSDKResolution sdk_Resolution = ZOOM_SDK_NAMESPACE::ZoomSDKResolution_NoUse;
	switch (resolution)
	{
	case ZNZoomSDKResolution_90P:
		sdk_Resolution = ZOOM_SDK_NAMESPACE::ZoomSDKResolution_90P;
		break;
	case ZNZoomSDKResolution_180P:
		sdk_Resolution = ZOOM_SDK_NAMESPACE::ZoomSDKResolution_180P;
		break;
	case ZNZoomSDKResolution_360P:
		sdk_Resolution = ZOOM_SDK_NAMESPACE::ZoomSDKResolution_360P;
		break;
	case ZNZoomSDKResolution_720P:
		sdk_Resolution = ZOOM_SDK_NAMESPACE::ZoomSDKResolution_720P;
		break;
	case ZNZoomSDKResolution_1080P:
		sdk_Resolution = ZOOM_SDK_NAMESPACE::ZoomSDKResolution_1080P;
		break;
	case ZNZoomSDKResolution_NoUse:
		sdk_Resolution = ZOOM_SDK_NAMESPACE::ZoomSDKResolution_NoUse;
		break;
	default:
		break;
	}
	return sdk_Resolution;
}

ZOOM_SDK_NAMESPACE::SDK_ECHO_CANCELLATION_LEVEL Map2SDKDefine(ZN_SDK_ECHO_CANCELLATION_LEVEL level)
{
	ZOOM_SDK_NAMESPACE::SDK_ECHO_CANCELLATION_LEVEL sdk_level = ZOOM_SDK_NAMESPACE::SDK_ECHO_CANCELLATION_DEFAULT;
	switch (level)
	{
	case ZN_SDK_ECHO_CANCELLATION_DEFAULT:
		sdk_level = ZOOM_SDK_NAMESPACE::SDK_ECHO_CANCELLATION_DEFAULT;
		break;
	case ZN_SDK_ECHO_CANCELLATION_LOW:
		sdk_level = ZOOM_SDK_NAMESPACE::SDK_ECHO_CANCELLATION_LOW;
		break;
	case ZN_SDK_ECHO_CANCELLATION_HIGH:
		sdk_level = ZOOM_SDK_NAMESPACE::SDK_ECHO_CANCELLATION_HIGH;
		break;
	default:
		break;
	}
	return sdk_level;
}
ZOOM_SDK_NAMESPACE::SDKChatPrivilege Map2WrapDefine(ZNSDKChatPrivilege privilege)
{
	ZOOM_SDK_NAMESPACE::SDKChatPrivilege chat_privilege = ZOOM_SDK_NAMESPACE::SDK_CHAT_PRIVILEGE_ALL;
	switch (privilege)
	{
	case ZNSDK_CHAT_PRIVILEGE_ALL:
		chat_privilege = ZOOM_SDK_NAMESPACE::SDK_CHAT_PRIVILEGE_ALL;
		break;
	case ZNSDK_CHAT_PRIVILEGE_ALL_PANELIST:
		chat_privilege = ZOOM_SDK_NAMESPACE::SDK_CHAT_PRIVILEGE_ALL_PANELIST;
		break;
	case ZNSDK_CHAT_PRIVILEGE_HOST:
		chat_privilege = ZOOM_SDK_NAMESPACE::SDK_CHAT_PRIVILEGE_HOST;
		break;
	case ZNSDK_CHAT_PRIVILEGE_DISABLE_ATTENDEE_CHAT:
		chat_privilege = ZOOM_SDK_NAMESPACE::SDK_CHAT_PRIVILEGE_DISABLE_ATTENDEE_CHAT;
		break;
	case ZNSDK_CHAT_PRIVILEGE_HOST_PUBLIC:
		chat_privilege = ZOOM_SDK_NAMESPACE::SDK_CHAT_PRIVILEGE_HOST_PUBLIC;
		break;
	case ZNSDK_CHAT_PRIVILEGE_END:
		chat_privilege = ZOOM_SDK_NAMESPACE::SDK_CHAT_PRIVILEGE_END;
		break;
	default:
		break;
	}
	return chat_privilege;
}
ZOOM_SDK_NAMESPACE::SDKChatMessageType Map2WrapDefine(ZNSDKChatMessageType type)
{
	ZOOM_SDK_NAMESPACE::SDKChatMessageType chat_message_type = ZOOM_SDK_NAMESPACE::SDKChatMessageType_To_None;
	switch (type)
	{
	case ZNSDKChatMessageType_To_None:
		chat_message_type = ZOOM_SDK_NAMESPACE::SDKChatMessageType_To_None;
		break;
	case ZNSDKChatMessageType_To_All:
		chat_message_type = ZOOM_SDK_NAMESPACE::SDKChatMessageType_To_All;
		break;
	case ZNSDKChatMessageType_To_All_Panelist:
		chat_message_type = ZOOM_SDK_NAMESPACE::SDKChatMessageType_To_All_Panelist;
		break;
	case ZNSDKChatMessageType_To_Individual_Panelist:
		chat_message_type = ZOOM_SDK_NAMESPACE::SDKChatMessageType_To_Individual_Panelist;
		break;
	case ZNSDKChatMessageType_To_Individual:
		chat_message_type = ZOOM_SDK_NAMESPACE::SDKChatMessageType_To_Individual;
		break;
	case ZNSDKChatMessageType_To_WaitingRoomUsers:
		chat_message_type = ZOOM_SDK_NAMESPACE::SDKChatMessageType_To_WaitingRoomUsers;
		break;
	default:
		break;
	}
	return chat_message_type;
}
ZOOM_SDK_NAMESPACE::FeatureEnableOption Map2WrapDefine(ZNFeatureEnableOption option)
{
	ZOOM_SDK_NAMESPACE::FeatureEnableOption meeting_enable_option = ZOOM_SDK_NAMESPACE::EnableOption_None;
	switch (option)
	{
	case ZNEnableOption_None:
		meeting_enable_option = ZOOM_SDK_NAMESPACE::EnableOption_None;
		break;
	case ZNEnableOption_Once:
		meeting_enable_option = ZOOM_SDK_NAMESPACE::EnableOption_Once;
		break;
	case ZNEnableOption_Always:
		meeting_enable_option = ZOOM_SDK_NAMESPACE::EnableOption_Always;
		break;
	default:
		break;
	}
	return meeting_enable_option;
}
ZOOM_SDK_NAMESPACE::SDKPanelistChatPrivilege Map2WrapDefine(ZNSDKPanelistChatPrivilege privilege)
{
	ZOOM_SDK_NAMESPACE::SDKPanelistChatPrivilege panelist_chat_privilege = ZOOM_SDK_NAMESPACE::SDKPanelistChatPrivilege_PanelistOnly;
	switch (privilege)
	{
	case ZNSDKPanelistChatPrivilege_PanelistOnly:
		panelist_chat_privilege = ZOOM_SDK_NAMESPACE::SDKPanelistChatPrivilege_PanelistOnly;
		break;
	case ZNSDKPanelistChatPrivilege_All:
		panelist_chat_privilege = ZOOM_SDK_NAMESPACE::SDKPanelistChatPrivilege_All;
		break;
	default:
		break;
	}
	return panelist_chat_privilege;
}
ZOOM_SDK_NAMESPACE::SDKAttendeeViewMode Map2WrapDefine(ZNSDKAttendeeViewMode mode)
{
	ZOOM_SDK_NAMESPACE::SDKAttendeeViewMode attendee_view_mode = ZOOM_SDK_NAMESPACE::SDKAttendeeViewMode_None;
	switch (mode)
	{
	case ZNSDKAttendeeViewMode_None:
		attendee_view_mode = ZOOM_SDK_NAMESPACE::SDKAttendeeViewMode_None;
		break;
	case ZNSDKAttendeeViewMode_FollowHost:
		attendee_view_mode = ZOOM_SDK_NAMESPACE::SDKAttendeeViewMode_FollowHost;
		break;
	case ZNSDKAttendeeViewMode_Speaker:
		attendee_view_mode = ZOOM_SDK_NAMESPACE::SDKAttendeeViewMode_Speaker;
		break;
	case ZNSDKAttendeeViewMode_Gallery:
		attendee_view_mode = ZOOM_SDK_NAMESPACE::SDKAttendeeViewMode_Gallery;
		break;
	case ZNSDKAttendeeViewMode_Standard:
		attendee_view_mode = ZOOM_SDK_NAMESPACE::SDKAttendeeViewMode_Standard;
		break;
	case ZNSDKAttendeeViewMode_SidebysideSpeaker:
		attendee_view_mode = ZOOM_SDK_NAMESPACE::SDKAttendeeViewMode_SidebysideSpeaker;
		break;
	case ZNSDKAttendeeViewMode_SidebysideGallery:
		attendee_view_mode = ZOOM_SDK_NAMESPACE::SDKAttendeeViewMode_SidebysideGallery;
		break;
	default:
		break;
	}
	return attendee_view_mode;
}
ZOOM_SDK_NAMESPACE::ZoomSDKWallpaperLayoutMode Map2SDKDefine(ZNZoomSDKWallpaperLayoutMode mode)
{
	ZOOM_SDK_NAMESPACE::ZoomSDKWallpaperLayoutMode layout_mode = ZOOM_SDK_NAMESPACE::ZoomSDKWallpaperLayoutMode_None;
	switch (mode)
	{
	case ZNZoomSDKWallpaperLayoutMode_None:
		layout_mode = ZOOM_SDK_NAMESPACE::ZoomSDKWallpaperLayoutMode_None;
		break;
	case ZNZoomSDKWallpaperLayoutMode_Fill:
		layout_mode = ZOOM_SDK_NAMESPACE::ZoomSDKWallpaperLayoutMode_Fill;
		break;
	case ZNZoomSDKWallpaperLayoutMode_Fit:
		layout_mode = ZOOM_SDK_NAMESPACE::ZoomSDKWallpaperLayoutMode_Fit;
		break;
	default:
		break;
	}
	return layout_mode;
}
ZOOM_SDK_NAMESPACE::MeetingAICompanionQuerySettingOptions Map2SDKDefine(ZNMeetingAICompanionQuerySettingOptions zn_option)
{
	ZOOM_SDK_NAMESPACE::MeetingAICompanionQuerySettingOptions option = ZOOM_SDK_NAMESPACE::MeetingAICompanionQuerySettingOptions_None;
	switch (zn_option)
	{
	case ZNMeetingAICompanionQuerySettingOptions_None:
		option = ZOOM_SDK_NAMESPACE::MeetingAICompanionQuerySettingOptions_None;
		break;
	case ZNMeetingAICompanionQuerySettingOptions_WhenQueryStarted:
		option = ZOOM_SDK_NAMESPACE::MeetingAICompanionQuerySettingOptions_WhenQueryStarted;
		break;
	case ZNMeetingAICompanionQuerySettingOptions_WhenParticipantsJoin:
		option = ZOOM_SDK_NAMESPACE::MeetingAICompanionQuerySettingOptions_WhenParticipantsJoin;
		break;
	case ZNMeetingAICompanionQuerySettingOptions_OnlyHost:
		option = ZOOM_SDK_NAMESPACE::MeetingAICompanionQuerySettingOptions_OnlyHost;
		break;
	case ZNMeetingAICompanionQuerySettingOptions_ParticipantsAndInviteesInOurOrganization:
		option = ZOOM_SDK_NAMESPACE::MeetingAICompanionQuerySettingOptions_ParticipantsAndInviteesInOurOrganization;
		break;
	case ZNMeetingAICompanionQuerySettingOptions_WhenParticipantsAndOrganizationJoin:
		option = ZOOM_SDK_NAMESPACE::MeetingAICompanionQuerySettingOptions_WhenParticipantsAndOrganizationJoin;
		break;
	case ZNMeetingAICompanionQuerySettingOptions_HostAndPanelists:
		option = ZOOM_SDK_NAMESPACE::MeetingAICompanionQuerySettingOptions_HostAndPanelists;
		break;
	case ZNMeetingAICompanionQuerySettingOptions_HostAndPanelistsInOurOrganization:
		option = ZOOM_SDK_NAMESPACE::MeetingAICompanionQuerySettingOptions_HostAndPanelistsInOurOrganization;
		break;
	default:
		break;
	}
	return option;
}
ZOOM_SDK_NAMESPACE::MeetingAICompanionQueryFeedbackType Map2SDKDefine(ZNMeetingAICompanionQueryFeedbackType zn_type)
{
	ZOOM_SDK_NAMESPACE::MeetingAICompanionQueryFeedbackType type = ZOOM_SDK_NAMESPACE::MeetingAICompanionQueryFeedbackType_None;
	switch (zn_type)
	{
	case ZNMeetingAICompanionQueryFeedbackType_None:
		type = ZOOM_SDK_NAMESPACE::MeetingAICompanionQueryFeedbackType_None;
		break;
	case ZNMeetingAICompanionQueryFeedbackType_Good:
		type = ZOOM_SDK_NAMESPACE::MeetingAICompanionQueryFeedbackType_Good;
		break;
	case ZNMeetingAICompanionQueryFeedbackType_Bad:
		type = ZOOM_SDK_NAMESPACE::MeetingAICompanionQueryFeedbackType_Bad;
		break;
	default:
		break;
	}
	return type;
}
ZOOM_SDK_NAMESPACE::SDKVideoPreferenceMode Map2SDKDefine(ZNSDKVideoPreferenceMode zn_mode)
{
	ZOOM_SDK_NAMESPACE::SDKVideoPreferenceMode mode = ZOOM_SDK_NAMESPACE::SDKVideoPreferenceMode_Balance;
	switch (zn_mode)
	{
		case ZNSDKVideoPreferenceMode_Balance:
			mode = ZOOM_SDK_NAMESPACE::SDKVideoPreferenceMode_Balance;
			break;
		case ZNSDKVideoPreferenceMode_Sharpness:
			mode = ZOOM_SDK_NAMESPACE::SDKVideoPreferenceMode_Sharpness;
			break;
		case ZNSDKVideoPreferenceMode_Smoothness:
			mode = ZOOM_SDK_NAMESPACE::SDKVideoPreferenceMode_Smoothness;
			break;
		case ZNSDKVideoPreferenceMode_Custom:
			mode = ZOOM_SDK_NAMESPACE::SDKVideoPreferenceMode_Custom;
			break;
		default:
			break;
	}
	return mode;
}
ZOOM_SDK_NAMESPACE::SDKWhiteboardShareOption Map2SDKDefine(ZNSDKWhiteboardShareOption zn_option)
{
	ZOOM_SDK_NAMESPACE::SDKWhiteboardShareOption option = ZOOM_SDK_NAMESPACE::SDKWhiteboardShareOption_HostShare;
	switch (zn_option)
	{
		case ZNSDKWhiteboardShareOption_HostShare:
			option = ZOOM_SDK_NAMESPACE::SDKWhiteboardShareOption_HostShare;
			break;
		case ZNSDKWhiteboardShareOption_HostGrabShare:
			option = ZOOM_SDK_NAMESPACE::SDKWhiteboardShareOption_HostGrabShare;
			break;
		case ZNSDKWhiteboardShareOption_AllGrabShare:
			option = ZOOM_SDK_NAMESPACE::SDKWhiteboardShareOption_AllGrabShare;
			break;
		default:
			break;
	}
	return option;
}
ZOOM_SDK_NAMESPACE::SDKWhiteboardCreateOption Map2SDKDefine(ZNSDKWhiteboardCreateOption zn_option)
{
	ZOOM_SDK_NAMESPACE::SDKWhiteboardCreateOption option = ZOOM_SDK_NAMESPACE::SDKWhiteboardCreateOption_HostOnly;
	switch (zn_option)
	{
		case ZNSDKWhiteboardCreateOption_HostOnly:
			option = ZOOM_SDK_NAMESPACE::SDKWhiteboardCreateOption_HostOnly;
			break;
		case ZNSDKWhiteboardCreateOption_AccountUsers:
			option = ZOOM_SDK_NAMESPACE::SDKWhiteboardCreateOption_AccountUsers;
			break;
		case ZNSDKWhiteboardCreateOption_All:
			option = ZOOM_SDK_NAMESPACE::SDKWhiteboardCreateOption_All;
			break;
		default:
			break;
	}
	return option;
}
ZOOM_SDK_NAMESPACE::SDKDocsStatus Map2SDKDefine(ZNSDKDocsStatus zn_status)
{
	ZOOM_SDK_NAMESPACE::SDKDocsStatus status = ZOOM_SDK_NAMESPACE::SDKDocsStatus_None;
	switch (zn_status)
	{
		case ZNSDKDocsStatus_None:
			status = ZOOM_SDK_NAMESPACE::SDKDocsStatus_None;
			break;
		case ZNSDKDocsStatus_Start:
			status = ZOOM_SDK_NAMESPACE::SDKDocsStatus_Start;
			break;
		case ZNSDKDocsStatus_Stop:
			status = ZOOM_SDK_NAMESPACE::SDKDocsStatus_Stop;
			break;
		default:
			break;
	}
	return status;
}
ZOOM_SDK_NAMESPACE::SDKDocsShareOption Map2SDKDefine(ZNSDKDocsShareOption zn_option)
{
	ZOOM_SDK_NAMESPACE::SDKDocsShareOption option = ZOOM_SDK_NAMESPACE::SDKDocsShareOption_None;
	switch (zn_option)
	{
		case ZNSDKDocsShareOption_None:
			option = ZOOM_SDK_NAMESPACE::SDKDocsShareOption_None;
			break;
		case ZNSDKDocsShareOption_HostShare:
			option = ZOOM_SDK_NAMESPACE::SDKDocsShareOption_HostShare;
			break;
		case ZNSDKDocsShareOption_HostGrabShare:
			option = ZOOM_SDK_NAMESPACE::SDKDocsShareOption_HostGrabShare;
			break;
		case ZNSDKDocsShareOption_AllGrabShare:
			option = ZOOM_SDK_NAMESPACE::SDKDocsShareOption_AllGrabShare;
			break;
		default:
			break;
	}
	return option;
}
ZOOM_SDK_NAMESPACE::SDKDocsCreateOption Map2SDKDefine(ZNSDKDocsCreateOption zn_option)
{
	ZOOM_SDK_NAMESPACE::SDKDocsCreateOption option = ZOOM_SDK_NAMESPACE::SDKDocsCreateOption_None;
	switch (zn_option)
	{
		case ZNSDKDocsCreateOption_None:
			option = ZOOM_SDK_NAMESPACE::SDKDocsCreateOption_None;
			break;
		case ZNSDKDocsCreateOption_HostOnly:
			option = ZOOM_SDK_NAMESPACE::SDKDocsCreateOption_HostOnly;
			break;
		case ZNSDKDocsCreateOption_AccountUsers:
			option = ZOOM_SDK_NAMESPACE::SDKDocsCreateOption_AccountUsers;
			break;
		case ZNSDKDocsCreateOption_All:
			option = ZOOM_SDK_NAMESPACE::SDKDocsCreateOption_All;
			break;
		default:
			break;
	}
	return option;
}

std::string wStr2Str(const ZoomSTRING& s)
{

	int len;
	int slength = (int)s.length() + 1;
	len = WideCharToMultiByte(CP_UTF8, 0, s.c_str(), slength, 0, 0, 0, 0);
	std::string r(len, '\0');
	WideCharToMultiByte(CP_UTF8, 0, s.c_str(), slength, &r[0], len, 0, 0);
	return r;
}
ZoomSTRING Str2WStr(const std::string& s)
{
	std::string strLocale = setlocale(LC_ALL, "");
	const char* chSrc = s.c_str();
	size_t nDestSize = mbstowcs(NULL, chSrc, 0) + 1;
	wchar_t* wchDest = new wchar_t[nDestSize];
	wmemset(wchDest, 0, nDestSize); //checked safe
	mbstowcs(wchDest, chSrc, nDestSize);
	std::wstring wstrResult = wchDest;
	delete[]wchDest;
	setlocale(LC_ALL, strLocale.c_str());
	return wstrResult;
}