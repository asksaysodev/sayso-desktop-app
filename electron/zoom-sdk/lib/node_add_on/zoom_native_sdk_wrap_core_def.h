#ifndef _zoom_native_sdk_wrap_core_def_h_
#define _zoom_native_sdk_wrap_core_def_h_
#include <string>
#include <vector>
#if (defined BUILD_WIN)
#define ZoomSTRING std::wstring
#else
#define ZoomSTRING std::string
#endif
#ifdef BUILD_WIN
#define ZOOM_HIDE_SYMBOL
#else
#define ZOOM_HIDE_SYMBOL __attribute__((visibility("hidden")))
#endif

enum ZNSDKError
{
	
	ZNSDKERR_SUCCESS = 0,///<Success.
	ZNSDKERR_NO_IMPL,///<This feature is currently invalid. 
	ZNSDKERR_WRONG_USEAGE,///<Incorrect usage of the feature. 
	ZNSDKERR_INVALID_PARAMETER,///<Wrong parameter.
	ZNSDKERR_MODULE_LOAD_FAILED,///<Loading module failed.
	ZNSDKERR_MEMORY_FAILED,///<No memory is allocated. 
	ZNSDKERR_SERVICE_FAILED,///<Internal service error.
	ZNSDKERR_UNINITIALIZE,///<Not initialized before the usage.
	ZNSDKERR_UNAUTHENTICATION,///<Not authorized before the usage.
	ZNSDKERR_NORECORDINGINPROCESS,///<No recording in process.
	ZNSDKERR_TRANSCODER_NOFOUND,///<Transcoder module is not found.
	ZNSDKERR_VIDEO_NOTREADY,///<The video service is not ready.
	ZNSDKERR_NO_PERMISSION,///<No permission.
	ZNSDKERR_UNKNOWN,///<Unknown error.
	ZNSDKERR_OTHER_SDK_INSTANCE_RUNNING,///<The other instance of the SDK is in process.
	ZNSDKERR_INTERNAL_ERROR,///<SDK internal error.
	ZNSDKERR_NO_AUDIODEVICE_ISFOUND,///<No audio device found.
	ZNSDKERR_NO_VIDEODEVICE_ISFOUND,///<No video device found.
	ZNSDKERR_TOO_FREQUENT_CALL,///<API calls too frequently.
	ZNSDKERR_FAIL_ASSIGN_USER_PRIVILEGE, ///<User can't be assigned with new privilege.
	ZNSDKERR_MEETING_DONT_SUPPORT_FEATURE,///<The current meeting doesn't support the feature.
	ZNSDKERR_MEETING_NOT_SHARE_SENDER,///<The current user is not the presenter.
	ZNSDKERR_MEETING_YOU_HAVE_NO_SHARE,///<There is no sharing.
	ZNSDKERR_MEETING_VIEWTYPE_PARAMETER_IS_WRONG, ///<Incorrect ViewType parameters.
	ZNSDKERR_MEETING_ANNOTATION_IS_OFF, ///<Annotation is disabled.
	ZNSDKERR_SETTING_OS_DONT_SUPPORT, ///<Current OS doesn't support the setting.
	ZNSDKERR_EMAIL_LOGIN_IS_DISABLED, ///<Email login is disable
	ZNSDKERR_HARDWARE_NOT_MEET_FOR_VB, ///<Computer doesn't meet the minimum requirements to use virtual background feature.
	ZNSDKERR_NEED_USER_CONFIRM_RECORD_DISCLAIMER,  ///<Need process disclaimer.
	ZNSDKERR_NO_SHARE_DATA,///<There is no raw data of sharing.
	ZNSDKERR_SHARE_CANNOT_SUBSCRIBE_MYSELF,
	ZNSDKERR_NOT_IN_MEETING,
	ZNSDKERR_NOT_JOIN_AUDIO,
	ZNSDKERR_HARDWARE_DONT_SUPPORT, ///<The current device doesn't support the feature.
	ZNSDKERR_DOMAIN_DONT_SUPPORT,
	ZNSDKERR_MEETING_REMOTE_CONTROL_IS_OFF, ///<Remote control is disabled.
	ZNSDKERR_FILETRANSFER_ERROR,
};
enum ZNSDK_LANGUAGE_ID
{
	ZNLANGUAGE_Unknown = 0,///<For initialization.
	ZNLANGUAGE_English,///<In English.
	ZNLANGUAGE_Chinese_Simplified,///<In simplified Chinese.
	ZNLANGUAGE_Chinese_Traditional,///<In traditional Chinese.
	ZNLANGUAGE_Japanese,///<In Japanese.
	ZNLANGUAGE_Spanish,///<In Spanish.
	ZNLANGUAGE_German,///<In German.
	ZNLANGUAGE_French,///<In French.
	ZNLANGUAGE_Portuguese,///<In Portuguese.
	ZNLANGUAGE_Russian,///<In Russian.
	ZNLANGUAGE_Korean,///<In Korean.
	ZNLANGUAGE_Vietnamese,///<In Vietnamese.
	ZNLANGUAGE_Italian,///<In Italian.
	ZNLANGUAGE_Polish,///<In Polish.
	ZNLANGUAGE_Turkish,///<In Turkish.
	ZNLANGUAGE_Indonesian,///<In Indonesian.
	ZNLANGUAGE_Dutch,///<In Dutch.
};

enum ZNAuthResult
{
	ZNAUTHRET_SUCCESS = 0,///<Authentication is successful.
	ZNAUTHRET_KEYORSECRETEMPTY,///<The key or secret to authenticate is empty.
	ZNAUTHRET_KEYORSECRETWRONG,///<The key or secret to authenticate is wrong.
	ZNAUTHRET_ACCOUNTNOTSUPPORT,///<The user account does not support.
	ZNAUTHRET_ACCOUNTNOTENABLESDK,///<The user account is not enabled for SDK.
	ZNAUTHRET_UNKNOWN,///<Unknown error.
	ZNAUTHRET_SERVICE_BUSY,///<Service is busy.
	ZNAUTHRET_NONE,///<Initial status.
	ZNAUTHRET_OVERTIME,///<Time out.
	ZNAUTHRET_NETWORKISSUE,///<Network issues.
	ZNAUTHRET_CLIENT_INCOMPATIBLE,///Account does not support this SDK version
	ZNAUTHRET_JWTTOKENWRONG,///<The jwt token to authenticate is wrong.
	ZNAUTHRET_LIMIT_EXCEEDED_EXCEPTION,///<The authentication rate limit is exceeded.
};
enum ZNCustomizedLanguageType
{
	ZN_CustomizedLanguage_None,///<No use of the custom resource.
	ZN_CustomizedLanguage_FilePath,///<Use the specified file path to assign the custom resource.
	ZN_CustomizedLanguage_Content,///<Use the specified content to assign the custom resource.
};
typedef struct _ZNCustomizedLanguageInfo
{
	ZoomSTRING langName;///<Resource name.
	ZoomSTRING langInfo;///<The value should be the full path of the resource file when the langType value is CustomizedLanguage_FilePath, including the file name. When the langType value is CustomizedLanguage_Content, the value saves the content of the resource.
	ZNCustomizedLanguageType langType;///<Use the custom resource type.
	_ZNCustomizedLanguageInfo()
	{
		langType = ZN_CustomizedLanguage_None;
	}

}ZNCustomizedLanguageInfo;
typedef struct _ZNConfigurableOptions
{
	ZNCustomizedLanguageInfo customizedLang;///<The custom resource information.
	bool useCustomUI;///<Additional functional configuration. The function currently supports only whether to use the custom UI mode. When the value of the optionalFeatures&ENABLE_CUSTOMIZED_UI_FLAG is TRUE, it means to the Use the custom UI mode. Otherwise use the traditional interface mode.
	_ZNConfigurableOptions()
	{
		useCustomUI = false;
	}

}ZNConfigurableOptions;

enum ZNSDK_APP_Locale
{
	ZNSDK_APP_Locale_Default,
	ZNSDK_APP_Locale_CN,
};
enum ZNZoomSDKVideoRenderMode
{
	ZNSDKVideoRenderMode_None = 0,
	ZNSDKVideoRenderMode_Auto,
	ZNSDKVideoRenderMode_D3D11EnableFLIP,
	ZNSDKVideoRenderMode_D3D11,
	ZNSDKVideoRenderMode_D3D9,
	ZNSDKVideoRenderMode_GDI,
};
enum ZNZoomSDKRenderPostProcessing
{
	ZNZoomSDKRenderPostProcessing_None = 0,
	ZNZoomSDKRenderPostProcessing_Auto,
	ZNZoomSDKRenderPostProcessing_Enable,
	ZNZoomSDKRenderPostProcessing_Disable,
};
enum ZNZoomSDKVideoCaptureMethod
{
	ZNZoomSDKVideoCaptureMethod_None = 0,
	ZNZoomSDKVideoCaptureMethod_Auto,
	ZNZoomSDKVideoCaptureMethod_DirectSHow,
	ZNZoomSDKVideoCaptureMethod_MediaFoundation,
};

typedef struct tagZNZoomSDKRenderOptions
{
	ZNZoomSDKVideoRenderMode    videoRenderMode;
	ZNZoomSDKRenderPostProcessing renderPostProcessing;
	ZNZoomSDKVideoCaptureMethod videoCaptureMethod;
	tagZNZoomSDKRenderOptions()
	{
		videoRenderMode = ZNSDKVideoRenderMode_None;
		renderPostProcessing = ZNZoomSDKRenderPostProcessing_Auto;
		videoCaptureMethod = ZNZoomSDKVideoCaptureMethod_Auto;
	}
}ZNZoomSDKRenderOptions;

enum ZNSDKRawDataMemoryMode
{
	ZNSDKRawDataMemoryModeStack,
	ZNSDKRawDataMemoryModeHeap
};
typedef struct tagZNRawDataOptions
{
	bool enableRawdataIntermediateMode; ///<false -- YUV420data, true -- intermediate data
	ZNSDKRawDataMemoryMode  videoRawdataMemoryMode;
	ZNSDKRawDataMemoryMode  shareRawdataMemoryMode;
	ZNSDKRawDataMemoryMode  audioRawdataMemoryMode;
	tagZNRawDataOptions()
	{
#if (defined _WIN32)
		enableRawdataIntermediateMode = true;
#else
		enableRawdataIntermediateMode = false;
#endif
		videoRawdataMemoryMode = ZNSDKRawDataMemoryModeStack;
		shareRawdataMemoryMode = ZNSDKRawDataMemoryModeStack;
		audioRawdataMemoryMode = ZNSDKRawDataMemoryModeStack;
	}
}ZNRawDataOptions;
typedef struct _ZNInitParam
{
	ZoomSTRING path;///<The path of sdk.dll
	ZoomSTRING domain;///<Web domain.
	bool enable_log;///<Enable log feature.
	ZNSDK_LANGUAGE_ID langid;///<The ID of the language.
	ZoomSTRING strSupportUrl;///<Support URL.
	ZoomSTRING strBrandingName;///<Branding name.
	ZNConfigurableOptions obConfigOpts;///<The configuration options of the SDK.
	unsigned int logFileSize; ///<Size of a log file in M(megabyte). The default size is 5M. There are 5 log files in total and the file size varies from 1M to 50M.
	bool enableGeneratDump; ///<Enable generate dump file if the app crashed.
	ZNSDK_APP_Locale locale;
	bool permonitor_awareness_mode;
	ZNZoomSDKRenderOptions renderOpts;
	ZNRawDataOptions rawdataOpts;
	ZoomSTRING customLocalizationFilePath;///<Only for Mac.
	unsigned int wrapperType;
	_ZNInitParam(){
		enable_log = true;
		langid = ZNLANGUAGE_Unknown;
		locale = ZNSDK_APP_Locale_Default;
		logFileSize = 5;
		enableGeneratDump = false;
		permonitor_awareness_mode = true;
		wrapperType = 3;
	}
}ZNInitParam;

typedef struct _ZNAuthContext
{
	ZoomSTRING sdk_jwt_token;///<Jwt token.
}ZNAuthContext;

enum ZNLOGINSTATUS
{
	ZNLOGIN_IDLE = 0,///<Unlogged in.
	ZNLOGIN_PROCESSING,///<In process of login.
	ZNLOGIN_SUCCESS,///<Login successful.
	ZNLOGIN_FAILED,///<Login failed.
};

enum ZNLoginFailReason
{
	ZNLoginFail_None = 0,
	ZNLoginFail_EmailLoginDisable,
	ZNLoginFail_UserNotExist,
	ZNLoginFail_WrongPassword,
	ZNLoginFail_AccountLocked,
	ZNLoginFail_SDKNeedUpdate,
	ZNLoginFail_TooManyFailedAttempts,
	ZNLoginFail_SMSCodeError,
	ZNLoginFail_SMSCodeExpired,
	ZNLoginFail_PhoneNumberFormatInValid,
	ZNLoginFail_LoginTokenInvalid,
	ZNLoginFail_OtherIssue = 100,
};

enum ZNLoginType
{
	ZNLoginType_Unknown = 0,///<Unknown type.
	ZNLoginType_SSO,///<Login with SSO token.
};

typedef struct _ZNAccountInfo {
	ZoomSTRING dispaly_name;///<The screen name of user.
	ZNLoginType login_type;///<The login type.
}ZNAccountInfo;
enum ZNMeetingFailCode
{
	ZNMEETING_SUCCESS = 0,///<Start meeting successfully.
	ZNMEETING_FAIL_CONNECTION_ERR = 1,///<The connection with the backend service has errors.
	ZNMEETING_FAIL_RECONNECT_ERR = 2,///<Reconnect error.
	ZNMEETING_FAIL_MMR_ERR = 3,///<Multi-media Router error.
	ZNMEETING_FAIL_PASSWORD_ERR = 4,///<Password is wrong.
	ZNMEETING_FAIL_SESSION_ERR = 5,///<Session error.
	ZNMEETING_FAIL_MEETING_OVER = 6,///<Meeting is over.
	ZNMEETING_FAIL_MEETING_NOT_START = 7,///<Meeting has not begun.
	ZNMEETING_FAIL_MEETING_NOT_EXIST = 8,///<Meeting does not exist.
	ZNMEETING_FAIL_MEETING_USER_FULL = 9,///<The capacity of meeting is full. For users that can't join meeting, they can go to watch live stream with the callback IMeetingServiceEvent::onMeetingFullToWatchLiveStream if the host has started.
	ZNMEETING_FAIL_CLIENT_INCOMPATIBLE = 10,///<The client is incompatible.
	ZNMEETING_FAIL_NO_MMR = 11,///<The Multi-media router is not founded. 
	ZNMEETING_FAIL_CONFLOCKED = 12,///<The meeting is locked.
	ZNMEETING_FAIL_MEETING_RESTRICTED = 13,///<The meeting is failed because of the restriction by the same account.
	ZNMEETING_FAIL_MEETING_RESTRICTED_JBH = 14,///<The meeting is restricted by the same account while the attendee is allowed to join before the host.
	ZNMEETING_FAIL_CANNOT_EMIT_WEBREQUEST = 15,///<Unable to send web request.
	ZNMEETING_FAIL_CANNOT_START_TOKENEXPIRE = 16,///<The token is expired.
	ZNSESSION_VIDEO_ERR = 17,///<Video hardware or software error.
	ZNSESSION_AUDIO_AUTOSTARTERR = 18,///<Audio autostart error.
	ZNMEETING_FAIL_REGISTERWEBINAR_FULL = 19,///<The number of webinar registered has reached the upper limit.
	ZNMEETING_FAIL_REGISTERWEBINAR_HOSTREGISTER = 20,///<Register webinar with the role of webinar host.
	ZNMEETING_FAIL_REGISTERWEBINAR_PANELISTREGISTER = 21,///<Register webinar with the role of panelist member.
	ZNMEETING_FAIL_REGISTERWEBINAR_DENIED_EMAIL = 22,///<Register webinar with the denied email.
	ZNMEETING_FAIL_ENFORCE_LOGIN = 23,///<Webinar request to login.
	ZNCONF_FAIL_ZC_CERTIFICATE_CHANGED = 24,  ///<Invalid for Windows SDK.
	ZNCONF_FAIL_VANITY_NOT_EXIST = 27, ///<Vanity conference ID does not exist.
	ZNCONF_FAIL_JOIN_WEBINAR_WITHSAMEEMAIL = 28, ///<Join webinar with the same email.
	ZNCONF_FAIL_DISALLOW_HOST_MEETING = 29, ///<Meeting settings is not allowed to start a meeting.
	ZNMEETING_FAIL_WRITE_CONFIG_FILE = 50,	///<Disabled to write the configure file.
	ZNMEETING_FAIL_FORBID_TO_JOIN_INTERNAL_MEETING = 60, ///<Forbidden to join the internal meeting.
	ZNCONF_FAIL_REMOVED_BY_HOST = 61, ///<Removed by the host. 
	ZNMEETING_FAIL_HOST_DISALLOW_OUTSIDE_USER_JOIN = 62,   ///<Forbidden to join meeting
	ZNMEETING_FAIL_UNABLE_TO_JOIN_EXTERNAL_MEETING = 63,  ///<To join a meeting hosted by an external Zoom account, your SDK app has to be published on Zoom Marketplace. You can refer to Section 6.1 of Zoom's API License Terms of Use.
	ZNMEETING_FAIL_BLOCKED_BY_ACCOUNT_ADMIN = 64,  ///<Join failed because this Meeting SDK key is blocked by the host's account admin.
	ZNMEETING_FAIL_NEED_SIGN_IN_FOR_PRIVATE_MEETING = 82,  ///<Need sign in using the same account as the meeting organizer.
	ZNMEETING_FAIL_APP_PRIVILEGE_TOKEN_ERROR = 500,  ///<App join token error.
	ZNMEETING_FAIL_JMAK_USER_EMAIL_NOT_MATCH = 1143, ///<Jmak user email not match
};
enum ZNMeetingEndReason
{
	ZNEndMeetingReason_None = 0,///<For initialization.
	ZNEndMeetingReason_KickByHost = 1,///<Kicked by host.
	ZNEndMeetingReason_EndByHost = 2,///<Ended by host.
	ZNEndMeetingReason_JBHTimeOut = 3,///<JBH times out.
	ZNEndMeetingReason_NoAttendee = 4,///<No attendee.
	ZNEndMeetingReason_HostStartAnotherMeeting = 5,///<Host starts another meeting.
	ZNEndMeetingReason_FreeMeetingTimeOut = 6,///<Free meeting times out.
	ZNEndMeetingReason_Undefined,///<Represents an undefined end meeting reason, typically used for new error codes introduced by the backend after client release
};
enum ZNMeetingStatus
{
	ZNMEETING_STATUS_IDLE = 0,///<No meeting is running.
	ZNMEETING_STATUS_CONNECTING,///<Connect to the meeting server status.
	ZNMEETING_STATUS_WAITINGFORHOST,///<Waiting for the host to start the meeting.
	ZNMEETING_STATUS_INMEETING,///<Meeting is ready, in meeting status.
	ZNMEETING_STATUS_DISCONNECTING,///<Disconnect the meeting server, leave meeting status.
	ZNMEETING_STATUS_RECONNECTING,///<Reconnecting meeting server status.
	ZNMEETING_STATUS_FAILED,///<Failed to connect the meeting server.
	ZNMEETING_STATUS_ENDED,///<Meeting ends.
	ZNMEETING_STATUS_UNKNOWN,///<Unknown status.
	ZNMEETING_STATUS_LOCKED,///<Meeting is locked to prevent the further participants to join the meeting.
	ZNMEETING_STATUS_UNLOCKED,///<Meeting is open and participants can join the meeting. 
	ZNMEETING_STATUS_IN_WAITING_ROOM,///<Participants who join the meeting before the start are in the waiting room.
	ZNMEETING_STATUS_WEBINAR_PROMOTE,///<Upgrade the attendees to panelist in webinar.
	ZNMEETING_STATUS_WEBINAR_DEPROMOTE,///<Downgrade the attendees from the panelist.
	ZNMEETING_STATUS_JOIN_BREAKOUT_ROOM,///<Join the breakout room.
	ZNMEETING_STATUS_LEAVE_BREAKOUT_ROOM,///<Leave the breakout room.
    ZNMEETING_STATUS_AUDIO_READY,///<Audio is connected.
    ZNMEETING_STATUS_OTHER_MEETING_INPROGRESS,///<There is another ongoing meeting on the server.
};
enum ZNSDKUserType
{
	ZNSDK_UT_NORMALUSER = 100,///<Type of ordinary user who needs to login.
	ZNSDK_UT_WITHOUT_LOGIN,///<Start meeting without login.
};
enum ZNZoomUserType
{
	ZNZoomUserType_APIUSER,///<API user.
	ZNZoomUserType_EMAIL_LOGIN,///<User logged in with email.
	ZNZoomUserType_FACEBOOK,///<User logged in with Facebook.
	ZNZoomUserType_GoogleOAuth,///<User logged in with Google.
	ZNZoomUserType_SSO,///<User logged in with SSO.
	ZNZoomUserType_Unknown,///<User of unknown type.
};
typedef struct _ZNStartParam
{
	ZNSDKUserType userType;///<SDK user type. For more details, see \link ZNSDKUserType \endlink enum.
	unsigned long long meetingNumber;///<Meeting number.
	ZoomSTRING hDirectShareAppWnd;///<The window handle of the direct sharing application.
	ZoomSTRING customer_key;///<The customer key that need the app intergrated with sdk to specify. The SDK will set this value when the associated settings are turned on.
	ZoomSTRING username;///<Username when logged in.
	ZoomSTRING sdkVanityID;///<Meeting vanity ID.
	ZoomSTRING userZAK;///<ZOOM access token.
	ZNZoomUserType zoomUserType;///<Zoom user type. For more details, see \link ZNZoomUserType \endlink enum.
	bool isVideoOff;///<Turn off the video or not. True indicates to turn off. In addition, this flag is affected by meeting attributes.
	bool isAudioOff;///<Turn off the audio or not. True indicates to turn off. In addition, this flag is affected by meeting attributes.
	bool isDirectShareDesktop;///<Share the desktop directly or not. True indicates to share.
	_ZNStartParam() {
		userType = ZNSDK_UT_WITHOUT_LOGIN;
	}
}ZNStartParam;
typedef struct _ZNJoinParam
{
	ZNSDKUserType userType;///<SDK user type. For more details, see \link ZNSDKUserType \endlink enum.
	unsigned long long meetingNumber;///<Meeting number.
	ZoomSTRING app_privilege_token; ///<app_privilege_token.
	ZoomSTRING hDirectShareAppWnd;///<The window handle of the direct sharing application.
	ZoomSTRING customer_key;///<The customer key that need the app intergrated with sdk to specify. The SDK will set this value when the associated settings are turned on.
	ZoomSTRING userId;///<User ID.
	ZoomSTRING username;///<Username when logged in.
	ZoomSTRING psw;///<Meeting password.
	ZoomSTRING vanityID;///<Meeting vanity ID.
	ZoomSTRING webinarToken;///<Webinar token.
	ZoomSTRING userZAK;///<ZOOM access token.
	ZoomSTRING displayID;
	ZoomSTRING join_token;///<Join token.
	ZoomSTRING onBehalfToken;///<On behalf token.
	bool isVideoOff;///<Turn off the video or not. True indicates to turn off. In addition, this flag is affected by meeting attributes.
	bool isAudioOff;///<Turn off the audio or not. True indicates to turn off. In addition, this flag is affected by meeting attributes.
	bool isDirectShareDesktop;///<Share the desktop directly or not. True indicates to share.
	_ZNJoinParam() {
		userType = ZNSDK_UT_WITHOUT_LOGIN;
	}
}ZNJoinParam;
enum ZNLeaveMeetingCmd
{
	ZNLEAVE_MEETING,///<Leave meeting
	ZNEND_MEETING,///<End meeting
};
enum ZNMeetingType
{
	ZNMEETING_TYPE_NONE,///<For initialization.
	ZNMEETING_TYPE_NORMAL,///<Ordinary meeting.
	ZNMEETING_TYPE_WEBINAR,///<Webinar.
	ZNMEETING_TYPE_BREAKOUTROOM,///<Breakout meeting.
};
enum ZNConnectionQuality
{
	ZNConn_Quality_Unknown,///<Unknown connection status
	ZNConn_Quality_Very_Bad,///<The connection quality is very poor.
	ZNConn_Quality_Bad,///<The connection quality is poor. 
	ZNConn_Quality_Not_Good,///<The connection quality is not good.
	ZNConn_Quality_Normal,///<The connection quality is normal.
	ZNConn_Quality_Good,///<The connection quality is good.
	ZNConn_Quality_Excellent,///<The connection quality is excellent.
};
typedef struct _ZNShowChatDlgParam
{
	ZoomSTRING hParent;///<Parent window handle.
	ZoomSTRING rect_top;
	ZoomSTRING rect_bottom;
	ZoomSTRING rect_left;
	ZoomSTRING rect_right;
	ZoomSTRING hChatWnd;///<Chat dialog handle.
}ZNShowChatDlgParam;
enum ZNSDKFloatVideoType
{
	ZN_FLOATVIDEO_List,///<Type of list.
	ZN_FLOATVIDEO_Small,///<Small. 
	ZN_FLOATVIDEO_Large,///<Large.
	ZN_FLOATVIDEO_Minimize,///<Minimized.
};
typedef struct _ZNVideoWallPageInfoParam
{
	ZoomSTRING currentPage;///<The page in video wall mode for the moment.
	ZoomSTRING totalPages;///<The total number of pages in video wall mode.
}ZNVideoWallPageInfoParam;
typedef struct _ZNSplitScreenInfo
{
	bool bZNSupportSplitScreen;///<The page in video wall mode for the moment.
	bool bZNInSplitScreenMode;///<The total number of pages in video wall mode.
	_ZNSplitScreenInfo()
	{
		bZNSupportSplitScreen = false;
		bZNInSplitScreenMode = false;
	}
}ZNSplitScreenInfo;
enum ZNSDKViewType
{
	ZNSDK_FIRST_VIEW,///<Primary displayer.
	ZNSDK_SECOND_VIEW,///<Secondary displayer.
	ZNSDK_SEND_SHARE_VIEW,
};
enum ZNAnnotationToolType
{
	ZN_ANNOTOOL_NONE_DRAWING,///<Switch to mouse cursor.

	ZN_ANNOTOOL_PEN,///<Pen.
	ZN_ANNOTOOL_HIGHLIGHTER,///<Highlighter.
	ZN_ANNOTOOL_AUTO_LINE,///<A straight line changes automatically in pace with the mouse cursor.
	ZN_ANNOTOOL_AUTO_RECTANGLE,///<A rectangle changes automatically in pace with the mouse cursor.
	ZN_ANNOTOOL_AUTO_ELLIPSE,///<An ellipse changes automatically in pace with the mouse cursor.
	ZN_ANNOTOOL_AUTO_ARROW,///<An arrow changes automatically in pace with the mouse cursor.
	ZN_ANNOTOOL_AUTO_RECTANGLE_FILL,///<A filled rectangle.
	ZN_ANNOTOOL_AUTO_ELLIPSE_FILL,///<A filled ellipse.
	ZN_ANNOTOOL_SPOTLIGHT,///<Laser pointer.
	ZN_ANNOTOOL_ARROW,///<An arrow showing the name of whom click on the sharing content.
	ZN_ANNOTOOL_ERASER,///<An eraser.

	ZN_ANNOTOOL_TEXTBOX, ///<Insert a textbox in order to input letters.
	ZN_ANNOTOOL_PICKER, ///<Select the annotations.
	ZN_ANNOTOOL_AUTO_RECTANGLE_SEMI_FILL, ///<A fair rectangle changes automatically in pace with the mouse cursor.
	ZN_ANNOTOOL_AUTO_ELLIPSE_SEMI_FILL, ///<A fair ellipse changes automatically in pace with the mouse cursor.
	ZN_ANNOTOOL_AUTO_DOUBLE_ARROW, ///<A line with double-arrow. 
	ZN_ANNOTOOL_AUTO_DIAMOND, ///<An unfilled rhombus.
	ZN_ANNOTOOL_AUTO_STAMP_ARROW, ///<A fixed-size arrow for marking.
	ZN_ANNOTOOL_AUTO_STAMP_CHECK, ///<A sign marking that something is correct.
	ZN_ANNOTOOL_AUTO_STAMP_X, ///<A sign marking that something is wrong.
	ZN_ANNOTOOL_AUTO_STAMP_STAR, ///<A star for marking.
	ZN_ANNOTOOL_AUTO_STAMP_HEART, ///<A heart for marking.
	ZN_ANNOTOOL_AUTO_STAMP_QM, ///<A sign for interrogation.
};
enum ZNAnnotationClearType
{
	ZN_ANNOCLEAR_ALL,///<Clear all annotations.
	ZN_ANNOCLEAR_SELF,///<Clear only your own annotations.
	ZN_ANNOCLEAR_OTHER,///<Clear only the others' annotations.
};
enum ZNAudioStatus
{
	ZN_Audio_None,///<Initialization.
	ZN_Audio_Muted,///<Muted status.
	ZN_Audio_UnMuted,///<Unmuted status.
	ZN_Audio_Muted_ByHost,///<Muted by the host.
	ZN_Audio_UnMuted_ByHost,///<Unmuted by the host.
	ZN_Audio_MutedAll_ByHost,///<The host mutes all.
	ZN_Audio_UnMutedAll_ByHost,///<The host unmutes all.
};
typedef struct _ZNUserAudioStatus
{
	unsigned int userId;///<The page in video wall mode for the moment.
	ZNAudioStatus audioStauts;///<The total number of pages in video wall mode.
}ZNUserAudioStatus;
enum ZNVideoStatus
{
	ZN_Video_ON, ///<Video is on.
	ZN_Video_OFF, ///<Video is off.
};
/*! \enum RecordingStatus
\brief Recording status.
Here are more detailed structural descriptions.
*/
enum ZNRecordingStatus
{
	ZNRecording_Start,///<Start recording on local computer or on cloud.
	ZNRecording_Stop,///<Stop recording on local computer or on cloud.
	ZNRecording_DiskFull,///<There is no more space to store both local and cloud recording.
	ZNRecording_Pause,///<Pause recording on local or on cloud.
	ZNRecording_Connecting,///<Connecting, only for cloud recording.
	ZNRecording_Fail,///<Saving the recording failed.
};

/*! \enum RequestStartCloudRecordingStatus
	\brief Request host to start cloud recording response status.
*/
enum ZNRequestStartCloudRecordingStatus
{
	ZNRequestStartCloudRecording_Granted,///<host grants the request.
	ZNRequestStartCloudRecording_Denied,///<host denies the request.
	ZNRequestStartCloudRecording_TimedOut,///<the request for cloud recording timed out.	
};

enum ZNShareStatus
{
	ZN_Sharing_None,///<For initialization.
	ZN_Sharing_Self_Send_Begin,///<Begin to share by the user himself.
	ZN_Sharing_Self_Send_End,///<Stop sharing by the user.
	ZN_Sharing_Other_Share_Begin,///<Others begin to share.
	ZN_Sharing_Other_Share_End,///<Others stop sharing.
	ZN_Sharing_View_Other_Sharing,///<View the sharing of others.
	ZN_Sharing_Pause,///<Pause sharing.
	ZN_Sharing_Resume,///<Resume sharing.
	ZN_Sharing_ContentTypeChange,///<Sharing content changes. This status is marked as deprecated.
	ZN_Sharing_SelfStartAudioShare,///<The current user begins to share the sounds of computer audio.
	ZN_Sharing_SelfStopAudioShare,///<The current user stops to share the sounds of computer audio.
	ZN_Sharing_OtherStartAudioShare,///<Other user begins to share the sounds of computer audio.
	ZN_Sharing_OtherStopAudioShare,///<Other user stops to share the sounds of computer audio.
	ZN_Sharing_Disconnected,///<The share is disconnected.
};

/*! \enum ZNShareType
	Type of current sharing received by the user.
	Here are more detailed structural descriptions..
*/
enum ZNShareType
{
	ZNSHARE_TYPE_UNKNOWN,///<Type unknown.
	ZNSHARE_TYPE_AS,///<Type of sharing the application.
	ZNSHARE_TYPE_DS,///<Type of sharing the desktop.
	ZNSHARE_TYPE_WB,///<Type of sharing the white-board.
	ZNSHARE_TYPE_AIRHOST,///<Type of sharing data from the device connected WIFI. 
	ZNSHARE_TYPE_CAMERA,///<Type of sharing the camera.
	ZNSHARE_TYPE_DATA,///<Type of sharing the data.
	ZNSHARE_TYPE_VIDEO_FILE,///<Type of sharing the video file.
	ZNSHARE_TYPE_FRAME,///<Type of sharing the frame.
	ZNSHARE_TYPE_DOCUMENT,///<Type of sharing the document.
	ZNSHARE_TYPE_COMPUTER_AUDIO///<Type of sharing the computer audio.
};

enum ZNLiveStreamStatus
{
	ZN_LiveStreamStatus_None,///<Only for initialization.
	ZN_LiveStreamStatus_InProgress,///<In progress.
	ZN_LiveStreamStatus_Connecting,///<Be connecting.
	ZN_LiveStreamStatus_Start_Failed_Timeout,///<Connect timeout.
	ZN_LiveStreamStatus_Start_Failed,///<Failed to start live streaming. 
	ZN_LiveStreamStatus_Ended,///<Live stream ends.
};
typedef struct _ZNLiveStreamItemInfo
{
	ZoomSTRING liveStreamURL; ///< The URL of the live stream meeting.
	ZoomSTRING liveStreamURLDescription;///< The descriptions of live stream.
	ZoomSTRING liveStreamViewerURL;///< The viewer URL of the live stream meeting.
}ZNLiveStreamItemInfo;
struct ZNRawLiveStreamInfo
{
	unsigned int userId;///<User ID.
	ZoomSTRING broadcastUrl;///<Broadcast URL
	ZNRawLiveStreamInfo()
	{
		userId = 0xffff;
	}
};
typedef struct _ZNCameraInfo
{
	ZoomSTRING deviceId; ///<The camera device ID.
	ZoomSTRING deviceName;///<The camera device name.
	bool isSelectedDevice;///<Is the current device is selected to use or not.
}ZNCameraInfo;
typedef struct _ZNMicInfo
{
	ZoomSTRING deviceId;///<The microphone device ID.
	ZoomSTRING deviceName;///<The microphone device name.
	bool isSelectedDevice;///<Is the current device is selected to use or not.
}ZNMicInfo;
typedef struct _ZNSpeakerInfo
{
	ZoomSTRING deviceId;///<The speaker device ID.
	ZoomSTRING deviceName;///<The speaker device name.
	bool isSelectedDevice;///<Is the current device is selected to use or not.
}ZNSpeakerInfo;
enum ZNSettingTabPage
{
	ZN_SettingTabPage_General,///<General setting page.
	ZN_SettingTabPage_Audio,///<Audio setting page.
	ZN_SettingTabPage_Video,///<Video setting page.
};
typedef struct _ZNShowSettingDlgParam
{
	ZoomSTRING hParent;///<Parent window handle.
	ZoomSTRING top;///<The Y-axis value of the top-left corner of the dialog uses the coordinate system of the monitor.
	ZoomSTRING left;///<The X-axis value of the top-left corner of the dialog uses the coordinate system of the monitor.
	ZoomSTRING hSettingWnd;///<Window handle of the dialog setting.
	bool bShow;///<Enable to display or nor.
	ZNSettingTabPage eTabPageType; ///<The tab page shown at the top of the displayed setting dialog.
	_ZNShowSettingDlgParam()
	{
		bShow = true;
		eTabPageType = ZN_SettingTabPage_General;
	}
}ZNShowSettingDlgParam;
enum ZNSDKCustomizedStringType
{
	ZN_SDK_Customized_LiveStream_MenuString_LiveOn_String = 0,///<The new string must end up with "%s" so that the menu item can show correctly. This type is used to define a string to replace the menu item ON %S on live streaming. 
	ZN_SDK_Customized_LiveStream_MenuString_LiveView_String,///<The new string must end up with "%s" so that the menu item can show correctly. This type is used to define a string to replace the menu item VIEW STREAM ON %S on live streaming.
	ZN_SDK_Customized_LiveStream_MenuString_LiveStop_String,///<The new string must be a pure string so that it can show correctly. This type is used to define a string to replace the menu item STOP LIVE STREAM on live streaming.
	ZN_SDK_Customized_LiveStream_MenuString_CopyURL_String,///<The new string must be a pure string so that it can show correctly. This type is used to define a string to replace the menu item COPY STREAMING LINK on live streaming.
	ZN_SDK_Customized_Title_App,	///<The new string must be a pure string so that it can show correctly. This type is used to define a string to replace the title of the meeting video UI.
	ZN_SDK_Customized_Title_ZoomVideo,  ///<The new string must be the same format as "Zoom Participant ID: %s   Meeting ID: %s" so that it can show correctly. This type is used to define a string to replace the title of the meeting video UI.
	ZN_SDK_Customized_Title_FreeZoomVideo, ///<The new string must be the same format as "Zoom Participant ID: %s  %d-Minutes Meeting ID:%s" so that it can show correctly. This type is used to define a string to replace the title of the meeting video UI when the user is free user and in view-only status. 
	ZN_SDK_Customized_Title_ViewOnly_FreeZoomVideo, ///<The new string must be the same format as "Zoom %d-Minutes Meeting ID: %s" so that it can show correctly. This type is used to define a string to replace the title of the meeting video UI when the user is free user and in view-only status. 
};
enum ZNSDKCustomizedURLType
{
	ZN_SDKCustomizedURL_VITRULBG_HELP, ///<Set the custom help URL in the virtual background tab page.
	ZN_SDKCustomizedURL_VITRULBG_LEARN_MORE,///<Set the custom Learn More URL in the virtual background tab page.
	ZN_SDKCustomizedURL_SUPPORTURL, ///<Set the Support URL in the meeting.
};
enum ZNH323CalloutStatus
{
	ZN_H323Callout_Unknown, ///<Used only for initialization.
	ZN_H323Callout_Success, ///<Call out successfully.
	ZN_H323Callout_Ring,   ///<Bell during the call.
	ZN_H323Callout_Timeout, ///<Call timeout.
	ZN_H323Callout_Failed, ///<Call fails.
	ZN_H323Callout_Busy,	///<Busy
	ZN_H323Callout_Decline, ///<Decline
};
enum ZNH323DeviceType
{
	ZN_H323DeviceType_Unknown,///<Unknown device, only for initialization.
	ZN_H323DeviceType_H323,///<H.323 device.
	ZN_H323DeviceType_SIP,///<SIP device.
	ZN_H323DeviceType_BOTH,///<H.323 device and SIP device.
};
typedef struct _ZNH323DeviecInfo
{
	ZoomSTRING h323_deviceName;///<The H.323 device ID.
	ZoomSTRING h323_deviceIP;///<The H.323 device IP.
	ZoomSTRING h323_deviceE164Name;///<The H.323 device E164 name.
	ZNH323DeviceType h323_DeviceType;///<The H.323 device type. For more infomation, see \link ZNH323DeviceType \endlink enum.
}ZNH323DeviecInfo;
typedef struct _ZNWndPosition
{
	ZoomSTRING z_left;///<Specifies the X-axis coordinate of the top-left corner of the window
	ZoomSTRING z_top;///<Specifies the Y-axis coordinate of the top-left of the window.
#if (defined BUILD_WIN)
	ZoomSTRING z_hSelfWnd;///<Specifies the window handle of the window itself.
	ZoomSTRING z_hParent;///<Specifies the window handle of the parent window. If the value is NULL, the position coordinate is the monitor coordinate.
#else
	ZoomSTRING z_height;
	ZoomSTRING z_width;
#endif
}ZNWndPosition;
enum ZNFreeMeetingEndingReminderType
{
	ZN_FreeMeetingEndingReminder_NONE,///<Initialization.
	ZN_FreeMeetingEndingReminder_Can_UpgradeMeeting_ToPro_Once,///<Upgrade to the priority meeting at once. 
	ZN_FreeMeetingEndingReminder_Can_UpgradeAccount,///<Upgrade the account.
	ZN_FreeMeetingEndingReminder_Reminder,///<Remind to pay.
	ZN_FreeMeetingEndingReminder_UpgradeMeeting_ToPro_Failed,///<Upgrade failed.
	ZN_FreeMeetingEndingReminder_UpgradeMeeting_ToPro_Success,///<Upgrade successfully.
};
enum ZNFreeMeetingNeedUpgradeType
{
	ZN_FreeMeetingNeedUpgradeType_NONE,///<Initialization.
	ZN_FreeMeetingNeedUpgradeType_BY_ADMIN,///<It is necessary for administrator to upgrade the free meeting.
	ZN_FreeMeetingNeedUpgradeType_BY_GIFTURL,///<Upgrade the free meeting by the gift link.
};
enum ZNPremeetingAPIResult
{
	ZN_PREMETAPIRET_UNKNOW,///<API returns unknown error.
	ZN_PREMETAPIRET_SUCCESS,///<Calls API successfully.
	ZN_PREMETAPIRET_FAILED,
	ZN_PREMETAPIRET_TIMEOUT,
};
enum ZNDirectShareStatus
{
	ZN_DirectShare_Unknown = 0,///<Only for initialization.
	ZN_DirectShare_Connecting,///<Waiting for enabling the direct sharing.
	ZN_DirectShare_In_Direct_Share_Mode,///<In direct sharing mode.
	ZN_DirectShare_Ended,///<End the direct sharing.
	ZN_DirectShare_Need_MeetingID_Or_PairingCode,///<Re-enter the meeting ID/paring code.
	ZN_DirectShare_NetWork_Error,///<Network error. Please try again later.
	ZN_DirectShare_Other_Error,///<Other errors. Mainly occur in SIP call mode.
	ZN_DirectShare_WrongMeetingID_Or_SharingKey,
	ZN_DirectShare_InputNewParingCode,///<Please input new paring code.
	ZN_DirectShare_Prepared, ///Prepare to share data
};
enum ZNAudioType
{
	ZN_AUDIOTYPE_NONE,///<Normal audio type.
	ZN_AUDIOTYPE_VOIP,///<In VoIP mode.
	ZN_AUDIOTYPE_PHONE,///<In telephone mode.
	ZN_AUDIOTYPE_UNKNOWN,///<Unknown mode.
};
enum ZNUserRole
{
	ZN_USERROLE_NONE,///<For initialization.
	ZN_USERROLE_HOST,///<Role of the host.
	ZN_USERROLE_COHOST,///<Role of co-host.
	ZN_USERROLE_PANELIST,///<Role of the panelist, valid only in webinar.
	ZN_USERROLE_BREAKOUTROOM_MODERATOR,///<Host role in breakout room.
	ZN_USERROLE_ATTENDEE,///<Role of attendee.
};
enum ZNSDKUserInfoType
{
	ZN_REAL_USERINFO,
	ZN_FAKE_USERINFO,
};
/*! \struct tagWebinarAttendeeStatus
    \brief Status of webinar attendee.
    Here are more detailed structural descriptions.
*/
typedef struct tagWebinarAttendeeStatus
{
	bool allow_talk;///<TRUE indicates that it is able to talk.
	tagWebinarAttendeeStatus()
	{
		allow_talk = false;
	}
}ZNWebinarAttendeeStatus;
/*! \enum ZNSDKEmojiFeedbackType
	\brief Specify the emoji feedback type.
	Here are more detailed enum descriptions.
*/
typedef enum tagSDKEmojiFeedbackType
{
	ZNSDKEmojiFeedbackType_None,
	ZNSDKEmojiFeedbackType_Yes,
	ZNSDKEmojiFeedbackType_No,
	ZNSDKEmojiFeedbackType_SpeedUp,
	ZNSDKEmojiFeedbackType_SlowDown,
	ZNSDKEmojiFeedbackType_Away
}ZNSDKEmojiFeedbackType;
typedef struct _ZNUserInfomation
{
	ZoomSTRING userName;///<Current user name.
	bool isHost; ///<Whether the member corresponding with the current information is the host or not.
	unsigned int userID;///<Current user's ID.
	ZoomSTRING avatarPath;
	ZoomSTRING persistentId;
	ZoomSTRING customerKey;
	bool isVideoOn;///<The video status of the user specified by the current information.
	bool isAudioMuted;///<The audio status of the user specified by the current information.
	bool isMySelf;///<Whether the current information corresponds to the user himself or not.
	ZNUserRole userRole;///<The type of role of the user specified by the current information. For more infomation, see \link ZNUserRole \endlink enum.
	bool isPurePhoneUser;///<Whether the user corresponding to the current information joins the meeting by telephone or not.
	bool isTalking;
	bool isH323User; ///<Whether the member corresponding with the current information is the h323 user or not.
	ZNWebinarAttendeeStatus webinarAttendeeStatus;///<The webinar status of the user specified by the current information. TRUE indicates that it is able to talk.
	ZNRecordingStatus localRecordingStatus;
	ZNSDKUserInfoType userInfoType;
	bool isRawLiveStreaming;
	bool HasRawLiveStreamPrivilege;
	bool hasCamera;
	ZNAudioType audioJoinType;///<the audio type of the user specified by the current information when joins the meeting. For more infomation, see \link ZNAudioType \endlink enum.
	bool isRaiseHand;///<Whether the user specified by the current information raises hand or not.
	bool isInterpreter;
	bool isSignLanguageInterpreter;
	ZoomSTRING interpreterActiveLanguage;
	ZNSDKEmojiFeedbackType emojiFeedbackType;
	bool isBotUser;///<Whether the user specified by the current information is bot user or not.
	ZoomSTRING botAppName;///<the bot app name.
	bool isInWebinarBackstage;
	bool isInWaitingRoom;///<Whether the user specified by the current information is in the waiting room or not.
	bool isClosedCaptionSender;///<Whether the user corresponding to the current information is the sender of Closed Caption or not.
#if (!defined TARGET_OS_MAC)
	int audioVoiceLevel;///<The Mic level of the user corresponding to the current information.
#endif
	_ZNUserInfomation()
	{
		userInfoType = ZN_FAKE_USERINFO;
		isHost = false;
		isVideoOn = false;
		isAudioMuted = false;
		isMySelf = false;
		userRole = ZN_USERROLE_NONE;
		isPurePhoneUser = false;
		isTalking = false;
		isH323User = false;
		isRawLiveStreaming = false;
		HasRawLiveStreamPrivilege = false;
		hasCamera = false;
		audioJoinType = ZN_AUDIOTYPE_NONE;
		isRaiseHand = false;
		isInterpreter = false;
		isSignLanguageInterpreter = false;
		emojiFeedbackType = ZNSDKEmojiFeedbackType_None;
		isBotUser = false;
		isInWebinarBackstage = false;
		isInWaitingRoom = false;
		isClosedCaptionSender = false;
	}
}ZNUserInfomation;

/*! \enum SDKMinimizeUIMode
\brief The type of minimize user interface mode.
Here are more detailed structural descriptions.
*/
enum ZNSDKMinimizeUIMode
{
	ZN_MinimizeUIMode_NONE,///<For initialization.
	ZN_MinimizeUIMode_SHARE,///<Minimized mode for sharing.
	ZN_MinimizeUIMode_VIDEO,///<Minimized mode for video.
	ZN_MinimizeUIMode_ACTIVESPEAKER,///<Minimized mode for speaking.
};
typedef struct _ZNSettingDlgShowTabPageOption
{
	bool z_bShowGeneral;///<True indicates to show general page
	bool z_bShowVideo; ///<True indicates to show video page
	bool z_bShowAudio;///<True indicates to show audio page
	bool z_bShowVirtualBackGround;///<True indicates to show virtual background page
	bool z_bSHowRecording;///<True indicates to show recording page
	bool z_bShowAdvancedFeature;///<True indicates to show advance feature page
	bool z_bShowStatistics;///<True indicates to show staticstics page
	bool z_bShowFeedback;///<True indicates to show feed back page
	bool z_bShowAccessibility;///<True indicates to show accessibility page
	_ZNSettingDlgShowTabPageOption()
	{
		z_bShowGeneral = true;
		z_bShowVideo = true;
		z_bShowAudio = true;
		z_bShowVirtualBackGround = true;
		z_bSHowRecording = true;
		z_bShowStatistics = true;
		z_bShowAccessibility = true;
		z_bShowAdvancedFeature = false;
		z_bShowFeedback = false;
	}

}ZNSettingDlgShowTabPageOption;

enum ZNSMSVerificationCodeErr
{
	ZNSMSVerificationCodeErr_Unknown,
	ZNSMSVerificationCodeErr_Success,
	ZNSMSVerificationCodeErr_Retrieve_SendSMSFailed,
	ZNSMSVerificationCodeErr_Retrieve_InvalidPhoneNum,
	ZNSMSVerificationCodeErr_Retrieve_PhoneNumAlreadyBound,
	ZNSMSVerificationCodeErr_Retrieve_PhoneNumSendTooFrequent,
	ZNSMSVerificationCodeErr_Verify_CodeIncorrect,
	ZNSMSVerificationCodeErr_Verify_CodeExpired,
	ZNSMSVerificationCodeErr_Verify_UnknownError,
};
typedef struct _ZNZoomRealNameAuthCountryInfo
{
	ZoomSTRING countryID;
	ZoomSTRING countryName;
	ZoomSTRING countryCode;
}ZNZoomRealNameAuthCountryInfo;

enum ZNSDKInviteDlgTabPage
{
	SDK_INVITEDLG_TAB_EMAILCONTACT = 0, ///<'Invite by Email' tab page
	SDK_INVITEDLG_TAB_PHONECONTACT, ///<'Invite by Phone' tab pag
	SDK_INVITEDLG_TAB_ROOMSYSTEM, ///<'Invite a Room System' tab page
};

enum ZNSDKH323TabPage
{
	SDK_INVITEDLG_H323_DIALIN = 0, ///<'Dial In' sub-tab page under Room System invitation tab page
	SDK_INVITEDLG_H323_CALLOUT, ///<'Call Out' sub-tab page under Room System invitation tab page
};

typedef struct _ZNZoomRedirectWarningMsgOption
{
	bool bRedirectBadNetwork;
	bool bRedirectWarnHighCPU;
	_ZNZoomRedirectWarningMsgOption()
	{
		bRedirectBadNetwork = false;
		bRedirectWarnHighCPU = false;
	}
}ZNZoomRedirectWarningMsgOption;

//SettingDlgShowTabPageOptionDef
#define SETTING_DLG_SHOW_VIRTUAL_BACK_GROUND_TABPAGE 1UL
#define SETTING_DLG_SHOW_VIDEO_TABPAGE (1UL << 1)
#define SETTING_DLG_SHOW_STATISTICS_TABPAGE (1UL << 2)
#define SETTING_DLG_SHOW_RECORDING_TABPAGE (1UL << 3)
#define SETTING_DLG_SHOW_GENERAL_TABPAGE (1UL << 4)
#define SETTING_DLG_SHOW_FEEDBACK_TABPAGE (1UL << 5)
#define SETTING_DLG_SHOW_AUDIO_TABPAGE (1UL << 6)
#define SETTING_DLG_SHOW_ADVANCED_FEATURE_TABPAGE (1UL << 7)
#define SETTING_DLG_SHOW_ACCESSIBILITY_TABPAGE (1UL << 8)

//SettingDlgShowURLOptionDef
#define ZNSettingDlgURL_General_ViewMoreSettings 1UL
#define ZNSettingDlgURL_Video_SupportCenter (1UL << 1)
#define ZNSettingDlgURL_Audio_LearnMore (1UL << 2)
#define ZNSettingDlgURL_Share_and_VB_LearnMore (1UL << 3)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \enum ZNSettingsNetWorkType
\brief Notify network type.
Here are more detailed structural descriptions.
*/
enum ZNSettingsNetWorkType
{
	ZNSETTINGS_NETWORK_WIRED = 0,///<Wired LAN
	ZNSETTINGS_NETWORK_WIFI = 1,///<WIFI
	ZNSETTINGS_NETWORK_PPP = 2,///<PPP
	ZNSETTINGS_NETWORK_3G = 3,///<3G
	ZNSETTINGS_NETWORK_OTHERS = 4,///<Others

	ZNSETTINGS_NETWORK_UNKNOWN = -1,///<Unknown network.
};
/*! \enum ZNSettingConnectionType
\brief Notify connection type.
Here are more detailed structural descriptions.
*/
enum ZNSettingConnectionType
{
	ZNSETTINGS_CONNECTION_TYPE_CLOUD,///<Cloud connection.
	ZNSETTINGS_CONNECTION_TYPE_DIRECT,///<Direct connection.
	ZNSETTINGS_CONNECTION_TYPE_UNKNOWN = -1,///<Unknown connection.
};
/*! \struct ZNOverallStatisticInfo
\brief Notify overall statistic information.
Here are more detailed structural descriptions.
*/
typedef struct _ZNOverallStatisticInfo
{
	ZNSettingsNetWorkType net_work_type;///<Network type.
	ZNSettingConnectionType connection_type;///<Connection type
	ZoomSTRING proxy_addr;///<Proxy address.
	_ZNOverallStatisticInfo()
	{
		net_work_type = ZNSETTINGS_NETWORK_UNKNOWN;
		connection_type = ZNSETTINGS_CONNECTION_TYPE_UNKNOWN;
	}
}ZNOverallStatisticInfo;
/*! \struct ZNAudioSessionStatisticInfo
\brief Notify the audio status information.
Here are more detailed structural descriptions.
*/
typedef struct _ZNAudioSessionStatisticInfo
{
	int frequency_send;///<Sending frequency, unit: KHz.
	int frequency_recv;///<Receiving frequency, unit: KHz.
	int latency_send;///<Sending latency, unit: ms.
	int latency_recv;///<Receiving latency, unit: ms.
	int jitter_send;///<Sending jitter, unit: ms.
	int jitter_recv;///<Receiving jitter, unit: ms.
	float packetloss_send;///<Sending packet loss, unit: %.
	float packetloss_recv;///<Receiving packet loss, unit: %.

}ZNAudioSessionStatisticInfo;
/*! \struct ZNASVSessionStatisticInfo
\brief Notify video status information.
Here are more detailed structural descriptions.
*/
typedef struct ZNASVSessionStatisticInfo
{
	int latency_send;///<Sending latency, unit: ms.
	int latency_recv;///<Receiving latency, unit: ms.
	int jitter_send;///<Sending jitter, unit: ms.
	int jitter_recv;///<Receiving jitter, unit: ms.
	float packetloss_send_max;///<Sending max packet loss, unit: %.
	float packetloss_recv_max;///<Receiving max packet loss, unit: %.
	float packetloss_send_avg;///<Sending average packet loss, unit: %.
	float packetloss_recv_avg;///<Receiving average packet loss, unit: %.
	int resolution_send;///<HIWORD->height, LOWORD->width.
	int resolution_recv;///<HIWORD->height, LOWORD->width. 
	int fps_send;///<Frame per second sending.
	int fps_recv;///<Frame per second receiving.

}ZNASVSessionStatisticInfo;

typedef enum
{
	ZNZoomSDKWallpaperLayoutMode_None,
	ZNZoomSDKWallpaperLayoutMode_Fill,
	ZNZoomSDKWallpaperLayoutMode_Fit
}ZNZoomSDKWallpaperLayoutMode;

typedef enum
{
	ZNZoomSDKWallpaperSettingStatus_None,
	ZNZoomSDKWallpaperSettingStatus_Downloading,
	ZNZoomSDKWallpaperSettingStatus_Downloaded,
	ZNZoomSDKWallpaperSettingStatus_DownloadFail
}ZNZoomSDKWallpaperSettingStatus;

typedef struct tagWallpaperItem
{
	ZNZoomSDKWallpaperLayoutMode wallpaperLayoutMode;///<the layout mode of the wallpaper.
	ZoomSTRING wallpaperId;///<the wallpaper id.
	ZoomSTRING title;///<the wallpaper title.
	ZoomSTRING thumbnailPath;///<the wallpaper thumbnail path.
	ZoomSTRING path;///<the full image path of the wallpaper.
	int32_t transparency;///<the transparency of the wallpaper.
	tagWallpaperItem()
	{
		wallpaperLayoutMode = ZNZoomSDKWallpaperLayoutMode_None;
		transparency = -1;
	}

}ZNWallpaperItem;

/// \brief Virtual background image information interface.
typedef struct tagVirtualBGImageInfo
{
	bool isSelected; ///<Determine the usage of current image
	bool isAllowDelete; ///<Determine the current image can be deleted from the list
	ZoomSTRING imageFilePath; ///<the file path of current image
	ZoomSTRING imageName; ///<the name of current image
	int64_t imageHandle;
	tagVirtualBGImageInfo()
	{
		isSelected = false;
		isAllowDelete = false;
		imageHandle = -1;
	}
}ZNVirtualBGImageInfo;

enum ZNVBVideoError
{
	ZNVB_VideoError_None = 0, 
	ZNVB_VideoError_UnknownFormat, 
	ZNVB_VideoError_ResolutionHigh1080P, 
	ZNVB_VideoError_ResolutionHigh720P, 
	ZNVB_VideoError_ResolutionLow, 
	ZNVB_VideoError_PlayError, 
	ZNVB_VideoError_OpenError, 
};

enum ZNRequiredInfoType
{
	ZNREQUIRED_INFO_TYPE_NONE,///<Initialization.
	ZNREQUIRED_INFO_TYPE_Password, ///<The user needs to enter the password when joins the meeting. Via the InputMeetingPasswordAndScreenName() to specify the password information.
	ZNREQUIRED_INFO_TYPE_Password4WrongPassword,///<If the password is invalid, the user needs to re-enter it. Via the InputMeetingPasswordAndScreenName() to specify the password information. 
	ZNREQUIRED_INFO_TYPE_PasswordAndScreenName,///<The user needs to enter the screen name and the password,via the InputMeetingPasswordAndScreenName() to specify the necessary information.
	ZNREQUIRED_INFO_TYPE_ScreenName,///<The user needs to enter the screen name. Via the InputMeetingScreenName() to specify the screen name information.
	ZNREQUIRED_INFO_TYPE_MeetingIDAndScreenName,///<The user needs to enter the screen name and the meeting id,via the InputMeetingMeetingIDAndScreenName() to specify the necessary information.
};
/*! \enum WebinarNeedRegisterType
\brief Register type of webinar.
Here are more detailed structural descriptions.
*/
enum ZNWebinarNeedRegisterType
{
	ZNWebinarReg_NONE,///<Initialization.
	ZNWebinarReg_By_Register_Url,///<Register webinar account by URL.
	ZNWebinarReg_By_Email_and_DisplayName,///<Register webinar account by email and the screen name.
};

enum ZNAudioCallbackActionInfo
{
	ZNACTION_INFO_NONE = 0,
	ZNACTION_INFO_CHOOSE_AUDIO_DEVICE_NOAUDIODEVICECONNECTTED,
	ZNACTION_INFO_CHOOSE_AUDIO_DEVICE_COMPUTERAUDIODEVICEERROR,
	ZNACTION_INFO_CHOOSE_AUDIO_DEVICE_PHONECALLDEVICEERROR,
	ZNACTION_INFO_NEED_JOIN_VOIP,
	ZNACTION_INFO_MUTE_UNMUTE_AUDIO,
	ZNACTION_INFO_SHOW_AUDIO_SETTING_WINDOW,
};

enum ZNZoomSDKResolution
{
	ZNZoomSDKResolution_90P = 0,
	ZNZoomSDKResolution_180P,
	ZNZoomSDKResolution_360P,
	ZNZoomSDKResolution_720P,
	ZNZoomSDKResolution_1080P,
	ZNZoomSDKResolution_NoUse = 100
};
enum ZNZoomSDKRawDataType
{
	ZNRAW_DATA_TYPE_VIDEO = 0,
	ZNRAW_DATA_TYPE_SHARE,
};

enum ZNRawDataStatus
{
	ZNRawData_On,
	ZNRawData_Off,
};

enum ZNZoomSDKVideoSubscribeFailReason
{
	ZNZoomSDKVideoSubscribe_Fail_None = 0, 
	ZNZoomSDKVideoSubscribe_Fail_ViewOnly, 
	ZNZoomSDKVideoSubscribe_Fail_NotInMeeting, 
	ZNZoomSDKVideoSubscribe_Fail_HasSubscribe1080POr720, 
	ZNZoomSDKVideoSubscribe_Fail_HasSubscribeTwo720P, 
	ZNZoomSDKVideoSubscribe_Fail_HasSubscribeExceededLimit, 
	ZNZoomSDKVideoSubscribe_Fail_TooFrequentCall, 
};

typedef struct tagZNAudioBtnClickedCallbackInfo
{
	unsigned int userid_MuteUnmute;
	ZNAudioCallbackActionInfo audio_clicked_action;
	tagZNAudioBtnClickedCallbackInfo()
	{
		userid_MuteUnmute = 0;
		audio_clicked_action = ZNACTION_INFO_NONE;
	}

}ZNAudioBtnClickedCallbackInfo;

typedef enum
{
	ZN_VIDEO_HARDWARE_ENCODE_RECEIVING = 0,
	ZN_VIDEO_HARDWARE_ENCODE_SENDING,
	ZN_VIDEO_HARDWARE_ENCODE_PROCESSING,
}ZN_VIDEO_HARDWARE_ENCODE_TYPE;

/**
 * @brief Enumeration for echo cancellation.  For more information, please visit <https://support.zoom.com/hc/en/article?id=zm_kb&sysparm_article=KB0066398>. 
 */
typedef enum {
	/** Automatically adjust echo cancellation, balancing CPU and performance. */
	ZN_SDK_ECHO_CANCELLATION_DEFAULT = 0,
	/** Better echo limitation, taking into account multiple people talking at the same time, low CPU utilization. */
	ZN_SDK_ECHO_CANCELLATION_LOW,
	/** Best experience when multiple people are talking at the same time. Enabling this option may increase CPU utilization. */
	ZN_SDK_ECHO_CANCELLATION_HIGH
}ZN_SDK_ECHO_CANCELLATION_LEVEL;

typedef struct _ZNWebinarLegalNoticesExplainedInfo
{
	ZoomSTRING explained_content;
	ZoomSTRING url_register_account_owner;
	ZoomSTRING url_register_terms;
	ZoomSTRING url_register_privacy_policy;
}ZNWebinarLegalNoticesExplainedInfo;

enum ZNStatisticsWarningType
{
	ZNStatistics_Warning_None,///<No warning.
	ZNStatistics_Warning_Network_Quality_Bad,///<The network connection quality is bad.
	ZNStatistics_Warning_Busy_System,///<The system is busy.
};

typedef struct tagZNMeetingParameter
{
	ZNMeetingType meeting_type;///<Meeting type.
	bool is_view_only;///<View only or not. True indicates to view only.
	bool is_auto_recording_local;///<Auto local recording or not. True indicates to auto local recording.
	bool is_auto_recording_cloud;///<Auto cloud recording or not. True indicates to auto cloud recording.
	uint64_t meeting_number;///<Meeting number.
	ZoomSTRING meeting_topic;///<Meeting topic.
	ZoomSTRING meeting_host;///<Meeting host.
	tagZNMeetingParameter()
	{
		meeting_type = ZNMEETING_TYPE_NONE;
		is_view_only = true;
		is_auto_recording_local = false;
		is_auto_recording_cloud = false;
		meeting_number = 0;
	}

}ZNMeetingParameter;

/*! \enum AudioShareMode
	\brief Audio share mode.
	Here are more detailed structural descriptions.
*/
enum ZNAudioShareMode
{
	ZNAudioShareMode_Mono,		///<Mono mode.
	ZNAudioShareMode_Stereo		///<Stereo mode
};

/*! \struct tagZoomSDKSharingSourceInfo
    \brief Visible shared source information.
    Here are more detailed structural descriptions..
*/
typedef struct tagZoomSDKSharingSourceInfo
{
	unsigned int userid;			///<User ID.
	unsigned int shareSourceID;		///<Share source ID.
	ZNShareStatus status;			///<The values of sharing status, see \link ZNShareStatus \endlink enum.
	bool isShowingInFirstView;		///<Display or not on the primary view. Valid for ZOOM style only.
	bool isShowingInSecondView;		///<Display or not on the secondary view. Valid for ZOOM style only.
	bool isCanBeRemoteControl;		///<Enable or disable the remote control.
	bool bEnableOptimizingVideoSharing;    ///<Enable of disable the optimizing video.
	ZNShareType contentType;		///<Type of sharing, see \link ZNShareType \endlink enum.
#if (defined __MACOS__)
	uint32_t windowID;				///<Handle of sharing application or white-board. It is invalid unless the value of the eShareType is SHARE_TYPE_AS or SHARE_TYPE_WB.	
	uint32_t monitorID;				///<The ID of screen to be shared. It is invalid unless the value of the eShareType is SHARE_TYPE_DS.
#else
	uint64_t windowID;				///<Handle of sharing application or white-board. It is invalid unless the value of the eShareType is SHARE_TYPE_AS or SHARE_TYPE_WB.
	ZoomSTRING monitorID;			///<The ID of screen to be shared. It is invalid unless the value of the eShareType is SHARE_TYPE_DS.
#endif

	tagZoomSDKSharingSourceInfo()
	{
		userid = 0;
		shareSourceID = 0;
		contentType = ZNSHARE_TYPE_UNKNOWN;
		status = ZN_Sharing_None;
		isShowingInFirstView = false;
		isShowingInSecondView = false;
		isCanBeRemoteControl = false;
		bEnableOptimizingVideoSharing = false;
	}
}ZNZoomSDKSharingSourceInfo;

/*! \enum MeetingReminderType
	\brief The type for disclaimer dialog.
	Here are more detailed structural descriptions.
*/
enum ZNMeetingReminderType
{
	ZNTYPE_LOGIN_REQUIRED,///Reminder type of login.
	ZNTYPE_START_OR_JOIN_MEETING,///<Reminder type of start or join meeting.
	ZNTYPE_RECORD_REMINDER,///<Reminder type of record reminder.
	ZNTYPE_RECORD_DISCLAIMER,///<Reminder type of record disclaimer
	ZNTYPE_LIVE_STREAM_DISCLAIMER,///<Reminder type of live stream disclaimer.
	ZNTYPE_ARCHIVE_DISCLAIMER,///<Reminder type of archive disclaimer.
	ZNTYPE_WEBINAR_AS_PANELIST_JOIN,///<Reminder type of join webinar as panelist.
	ZNTYPE_TERMS_OF_SERVICE, ///<Reminder type of Terms of service or privacy statement changed.
	ZNTYPE_SMART_SUMMARY_DISCLAIMER, ///<Reminder type of smart summary disclaimer.
	ZNTYPE_SMART_SUMMARY_ENABLE_REQUEST_REMINDER, ///<Reminder type of smart summary enable request. This type is marked as deprecated. Replaced with callback \link IMeetingAICompanionSmartSummaryHelperEvent::onSmartSummaryEnableActionCallback \endlink
	ZNTYPE_QUERY_DISCLAIMER, ///<Reminder type of query disclaimer.
	ZNTYPE_QUERY_ENABLE_REQUEST_REMINDER, ///<Reminder type of query enable request. This type is marked as deprecated.
	ZNTYPE_ENABLE_SMART_SUMMARY_REMINDER, ///<Reminder type of enable smart summary. This type is marked as deprecated. Replaced with callback \link IMeetingAICompanionSmartSummaryHelperEvent::onSmartSummaryEnableActionCallback \endlink
 	ZNTYPE_WEBINAR_ATTENDEE_PROMOTE_REMINDER, ///<Reminder type of webinar promote attendee.
	ZNTYPE_JOIN_PRIVATE_MODE_MEETING_REMINDER,///<Reminder type of joining a meeting with private mode.
	ZNTYPE_SMART_RECORDING_ENABLE_REQUEST_REMINDER,///<Reminder type to enable smart recording request. This type is marked as deprecated.
	ZNTYPE_ENABLE_SMART_RECORDING_REMINDER,///<Reminder type to enable smart recording. This type is marked as deprecated.
	ZNTYPE_AI_COMPANION_PLUS_DISCLAIMER, ///<Reminder type of AICompanionPlus disclaimer. This type is marked as Deprecated.
	ZNTYPE_CLOSED_CAPTION_DISCLAIMER,///<Reminder type of Close Caption disclaimer.
	ZNTYPE_MULTI_DISCLAIMER,///<Reminder type of disclaimers combination. 
	ZNTYPE_JOIN_MEETING_CONNECTOR_AS_GUEST_REMINDER,///<Reminder type for a join meeting connector with guest mode.
	ZNTYPE_COMMON_DISCLAIMER,///<Reminder type of common disclaimer.
};

/*! \enum FeatureEnableOption
	\brief The option for meeting feature.
*/
enum ZNFeatureEnableOption
{
	ZNEnableOption_None,///do not enable.
	ZNEnableOption_Once,///enable for this meeting.
	ZNEnableOption_Always,///enable for this and all future meetings on this account.
};

typedef struct _ZNMeetingReminderContent
{
	ZNMeetingReminderType type;///<The type of the reminder.
	ZoomSTRING title;///<The title of the reminder dialog content.
	ZoomSTRING content;///<The detail content of the reminder dialog content.
	bool is_blocking;///<Determine whether block the user join or stay in the meeting, True indicates block the user join or stay in the meeting. Otherwise False.
}ZNMeetingReminderContent;

/*! \struct NormalMeetingChatStatus
    \brief The authority to chat in the normal meeting.  
    Here are more detailed structural descriptions..
*/
typedef struct _ZNNormalMeetingChatStatus
{
	bool can_chat;///<TRUE indicates that the user owns the authority to send message to chat.
	bool can_chat_to_all;///<TRUE indicates that the user owns the authority to send message to all.
	bool can_chat_to_individual;///<TRUE indicates that the user owns the authority to send message to an individual attendee in the meeting.
	bool is_only_can_chat_to_host;///<TRUE indicates that the user owns the authority to send message only to the host. 
}ZNNormalMeetingChatStatus;

/*! \struct tagWebinarAttendeeChatStatus
    \brief The authority to chat for the normal attendee in the webinar.
    Here are more detailed structural descriptions..
*/
typedef struct _ZNWebinarAttendeeChatStatus
{
	bool can_chat;///<TRUE indicates that the attendee can send message to chat. 
	bool can_chat_to_all_panellist_and_attendee;///<TRUE indicates that the user owns the authority to send message to all the panelists and attendees.
	bool can_chat_to_all_panellist;///<TRUE indicates that the user owns the authority to send message to all the panelists.
}ZNWebinarAttendeeChatStatus;

/*! \struct tagWebinarOtherUserRoleChatStatus
    \brief The authority to chat for the host, co-host and panelist to chat in webinar.
    Here are more detailed structural descriptions..
*/
typedef struct _ZNWebinarOtherUserRoleChatStatus
{
	bool can_chat_to_all_panellist;///<TRUE indicates that the user owns the authority to send message to all the panelists.
	bool can_chat_to_all_panellist_and_attendee;///<TRUE indicates that the user owns the authority to send message to all.
	bool can_chat_to_individual;///<TRUE indicates that the user owns the authority to send message to individual attendee.
}ZNWebinarOtherUserRoleChatStatus;

/*! \struct ZNChatStatus
    \brief The authority to chat in the specified meeting.
    Here are more detailed structural descriptions..
*/
typedef struct _ZNChatStatus
{
	union
	{
		ZNNormalMeetingChatStatus normal_meeting_status;
		ZNWebinarAttendeeChatStatus webinar_attendee_status;
		ZNWebinarOtherUserRoleChatStatus webinar_other_status;
	}ut;///<The ut value depends on the value of the other members in the structure. When the value of is_webinar_meeting is false, the ut value is the NormalMeetingChatStatusnormal_meeting_status. When the values of the is_webinar_meeting and the is_webinar_attendee is true, the ut value is WebinarAttendeeChatStatus webinar_attendee_status. The value of is_webinar_meeting is true while the is_webinar_attendee is false, the ut value is WebinarOtherUserRoleChatStatus webinar_other_status.
	bool is_chat_off;///<TRUE indicates that it is disabled to chat in the specified meeting. 
	bool is_webinar_attendee;///<TRUE indicates that the owner of the current message is the attendee of the webinar. 
	bool is_webinar_meeting;///<TRUE indicates that the current meeting is webinar.

	_ZNChatStatus()
	{
		Reset();
	}

	void Reset()
	{
		memset(this, 0, sizeof(_ZNChatStatus));  //checked safe
	}
}ZNChatStatus;

/**
 * @brief Enumerations of the chat privilege.
 */
typedef enum {
	ZNSDK_CHAT_PRIVILEGE_ALL = 1,                    	///<allow attendee to chat with everyone [meeting & webinar]
	ZNSDK_CHAT_PRIVILEGE_ALL_PANELIST = 2,		        ///<allow attendee to chat with all panelists only, but cannot to "all panelists and attendees" [webinar]
	ZNSDK_CHAT_PRIVILEGE_HOST = 3,	                    ///<allow attendee to chat with host only [meeting]
	ZNSDK_CHAT_PRIVILEGE_DISABLE_ATTENDEE_CHAT = 4,     ///<allow attendee to chat with no one [meeting & webinar]
	ZNSDK_CHAT_PRIVILEGE_HOST_PUBLIC = 5,               ///<allow attendee to chat with host and public [meeting]
	ZNSDK_CHAT_PRIVILEGE_END
} ZNSDKChatPrivilege;

/**
 * @brief Enumerations of the type for chat message.
 */
typedef enum
{
	ZNSDKChatMessageType_To_None, ///<For initialize	
	ZNSDKChatMessageType_To_All,///<Chat message is send to all.	
	ZNSDKChatMessageType_To_All_Panelist,///<Chat message is send to all panelists.	
	ZNSDKChatMessageType_To_Individual_Panelist,///<Chat message is send to individual attendee and cc panelists.	
	ZNSDKChatMessageType_To_Individual,///<Chat message is send to individual user.	
	ZNSDKChatMessageType_To_WaitingRoomUsers,///<Chat message is send to waiting room user.
}ZNSDKChatMessageType;

/**
 * @brief Enumerations of the chat message delete type.
 */
typedef enum
{
	ZNSDK_CHAT_DELETE_BY_NONE,	///<none
	ZNSDK_CHAT_DELETE_BY_SELF,	///<delete by self
	ZNSDK_CHAT_DELETE_BY_HOST,	///<delete by host
	ZNSDK_CHAT_DELETE_BY_DLP,	///<delete by dlp when the message goes against the host organization's compliance policies.
}ZNSDKChatMessageDeleteType;

/**
 * @brief Enumerations of the content rich text style type for chat message.
 */
typedef enum
{
	ZNTextStyle_None, ///<Chat message rich text style normal.
	ZNTextStyle_Bold, ///<Chat message rich text style bold.
	ZNTextStyle_Italic, ///<Chat message rich text style italic.
	ZNTextStyle_Strikethrough, ///<Chat message rich text style strike through.
	ZNTextStyle_BulletedList, ///<Chat message rich text style bulleted list.
	ZNTextStyle_NumberedList, ///<Chat message rich text style numbered list.
	ZNTextStyle_Underline, ///<Chat message rich text style underline.
	ZNTextStyle_FontSize, ///<Chat message rich text style font size.
	ZNTextStyle_FontColor, ///<Chat message rich text style font color.
	ZNTextStyle_BackgroundColor, ///<Chat message rich text style background color.
	ZNTextStyle_Indent, ///<Chat message rich text style indent.
	ZNTextStyle_Paragraph, ///<Chat message rich text style paragraph.
	ZNTextStyle_Quote, ///<Chat message rich text style quote.
	ZNTextStyle_InsertLink ///<Chat message rich text style insert link.
}ZNRichTextStyle;

typedef struct _ZNRichTextStyleOffset
{
	unsigned int positionStart;///<certain rich-text style's start position.
	unsigned int positionEnd;///<the end position of a certain style in rich text.
	ZoomSTRING reserve;///<certain rich-text style's supplementary information.
}ZNRichTextStyleOffset;

typedef struct _ZNRichTextStyleItem
{
	ZNRichTextStyle textStyle;///<the rich text type of a portion of the current message.
	std::vector<ZNRichTextStyleOffset> textStyleOffsetList;///<the current message's rich text position info list of a certain style.
}ZNRichTextStyleItem;

typedef struct _ZNChatMsgInfo
{
	ZoomSTRING messageID;///<the message ID of the current message.
	unsigned int senderUserId;///<the sender ID of the current message.
	ZoomSTRING senderDisplayName;///<the sender screen name of the current message.
	unsigned int receiverUserId;///<the receiver ID of the current message.
	ZoomSTRING receiverDisplayName;///<the receiver screen name of the current message.
	ZoomSTRING content;///<the content of the current message.
	time_t timeStamp;///<the timestamps of the current message.
	bool isChatToAll;///<if the current message is sent to all.
	bool isChatToAllPanelist;///<if the current message is sent to all the panelists.
	bool isChatToWaitingroom;///<if the current message is sent to waiting room.
	ZNSDKChatMessageType chatMessageType;///<the chat message type of the current message.
	bool isComment;///<if the current message is a reply to another message.
	bool isThread;///<if the current message is part of a message thread, and can be directly replied to.
	std::vector<ZNRichTextStyleItem> textStyleItemList;///<the current message's chat message font style list.
	ZoomSTRING threadID;///<the current message's thread ID.
}ZNChatMsgInfo;

typedef enum
{
	ZNSDKFileTransferState_None = 0,         ///< The file transfer has no state.
	ZNSDKFileTransferState_ReadyToTransfer,  ///< The file transfer is ready to start.
	ZNSDKFileTransferState_Transfering,      ///< The file transfer is in progress.
	ZNSDKFileTransferState_TransferFailed,   ///< The file transfer failed.
	ZNSDKFileTransferState_TransferDone,     ///< The file transfer completed successfully.
}ZNSDKFileTransferStatus;

///<\brief The basic information of transfer file
typedef struct tagSDKFileTransferInfo
{
	ZoomSTRING messageID;///<The message identify of transfer file.
	ZNSDKFileTransferStatus trans_status;///< he status of the file transfer.
	time_t time_stamp;///<The time stamp of the file.
	bool is_send_to_all;///<Is the file send to all user in meeting?
	unsigned int file_size;///<The bytes of transfer file size.
	ZoomSTRING file_name;///<the file name of transfer file.
	unsigned int complete_percentage;///<The percentage of the file transfer completed.
	unsigned int complete_size;///<The size of the file transferred so far in bytes.
	unsigned int bit_per_second;///<The speed of the file transfer in bits per second.
	tagSDKFileTransferInfo()
	{
		Reset();
	}

	void Reset()
	{
		memset(this, 0, sizeof(tagSDKFileTransferInfo));  //checked safe
	}
}ZNSDKFileTransferInfo;

/*! \enum ZNWaitingRoomLayoutType
	\brief WaitingRoom LayoutType.
	Here are more detailed structural descriptions.
*/
enum ZNWaitingRoomLayoutType
{
	ZNWaitingRoomLayoutType_Default,
	ZNWaitingRoomLayoutType_Logo,
	ZNWaitingRoomLayoutType_Video
};

/*! \enum ZNCustomWaitingRoomDataStatus
	\brief Download Status of CustomWaitingRoomData.
	Here are more detailed structural descriptions.
*/
enum ZNCustomWaitingRoomDataStatus
{
	ZNCustomWaitingRoomDataStatus_Init,
	ZNCustomWaitingRoomDataStatus_Downloading,
	ZNCustomWaitingRoomDataStatus_Download_OK,
	ZNCustomWaitingRoomDataStatus_Download_Failed
};

/*! \struct ZoomSDKColor
	\brief Zoom SDK color.
	The standard RGB color model has a value range of 0-255.
*/
struct ZNZoomSDKColor {
	unsigned int red = 0; // Font color R value.
	unsigned int green = 0; // Font color G value.
	unsigned int blue = 0; // Font color B value.
};

/*! \struct WaitingRoomBrandingPageColor
	\brief The waiting room page color.
*/
struct ZNWaitingRoomBrandingPageColor {
	ZNZoomSDKColor background_color;
};

/*! \struct WaitingRoomBrandingTextColor
	\brief The waiting room text color.
*/
struct ZNWaitingRoomBrandingTextColor {
	ZNZoomSDKColor primary_color;
	ZNZoomSDKColor secondary_color;
	ZNZoomSDKColor paragraph_color;
	ZNZoomSDKColor hyperlink_color;
};

/*! \struct WaitingRoomBrandingButtonColor
	\brief The waiting room button color.
*/
struct ZNWaitingRoomBrandingButtonColor {
	ZNZoomSDKColor primary_button_color;
};

/*! \struct ZNWaitingRoomCustomizeData
	\brief The WaitingRoom Customize Data Info.
	Here are more detailed structural descriptions..
*/
typedef struct CustomWaitingRoomData_s
{
	ZoomSTRING title;
	ZoomSTRING description;
	ZoomSTRING logo_path;
	ZoomSTRING video_path;
	ZoomSTRING image_path;
	ZNWaitingRoomLayoutType type;
	ZNCustomWaitingRoomDataStatus status;

	ZNWaitingRoomBrandingPageColor page_color;
	ZNWaitingRoomBrandingTextColor text_color;
	ZNWaitingRoomBrandingButtonColor button_color;
	CustomWaitingRoomData_s()
	{
		type = ZNWaitingRoomLayoutType_Default;
		status = ZNCustomWaitingRoomDataStatus_Init;
	}
}ZNCustomWaitingRoomData;

/*! \struct tagWebinarMeetingStatus
    \brief Webinar Meeting Status.
    Here are more detailed structural descriptions.
*/
typedef struct tagWebinarMeetingStatus
{
	bool allow_panellist_start_video;///<TRUE indicates that the panelist is able to turn on the video. FALSE not.
	bool allow_attendee_chat;///<TRUE indicates that the attendee is able to chat. FALSE not.
	bool allow_emoji_reaction;///<TRUE indicates that the attendee is able to emojireaction. FALSE not.
	bool allow_attendee_raise_hand;///<TRUE indicates that the attendee is able to raise hand. FALSE not.
	bool allow_attendee_view_participant_count;///<TRUE indicates that the attendee is able to view participant count. FALSE not.
	tagWebinarMeetingStatus()
	{
		Reset();
	}

	void Reset()
	{
		allow_panellist_start_video = false;
		allow_attendee_chat = false;
		allow_emoji_reaction = false;
		allow_attendee_raise_hand = false;
		allow_attendee_view_participant_count = false;
	}
}ZNWebinarMeetingStatus;

/**
 * @brief Enumerations of the panelist chat privilege.
 */
enum ZNSDKPanelistChatPrivilege
{
	ZNSDKPanelistChatPrivilege_PanelistOnly = 1,	///<Allow panelists only to chat with each other.
	ZNSDKPanelistChatPrivilege_All = 2			///<Allow panelist to chat with everyone.
};

/**
 * @brief Enumerations of the attendee view display mode.
 */
enum ZNSDKAttendeeViewMode
{
	ZNSDKAttendeeViewMode_None,              ///<attendee view display mode is invaild
	ZNSDKAttendeeViewMode_FollowHost,        ///<follow host
	ZNSDKAttendeeViewMode_Speaker,           ///<always view active speaker
	ZNSDKAttendeeViewMode_Gallery,           ///<always view gallery
	ZNSDKAttendeeViewMode_Standard,          ///<attendee can manually switch between gallery and active speaker
	ZNSDKAttendeeViewMode_SidebysideSpeaker, ///<attendee view sharing side by side speaker
	ZNSDKAttendeeViewMode_SidebysideGallery  ///<attendee view sharing side by side gallery
};

enum ZNAICompanionFeature
{
	ZNSMART_SUMMARY,    ///<Meeting summary with AI Companion generates summary assets.
	ZNQUERY,            ///<Meeting questions with AI Companion generates transcript assets.
	ZNSMART_RECORDING,  ///<Smart recording with AI Companion generates recording assets.
};

/*! \enum ZNMeetingAICompanionQueryFeedbackType
 * \brief Enumerations of the type for query feedback.
 */
enum ZNMeetingAICompanionQueryFeedbackType
{
	ZNMeetingAICompanionQueryFeedbackType_None = 0,	///<Initialization.
	ZNMeetingAICompanionQueryFeedbackType_Good,		///<Good.
	ZNMeetingAICompanionQueryFeedbackType_Bad		///<Bad.
};

/*! \enum ZNMeetingAICompanionQueryRequestError
 * \brief Enumerations of the type for query request error.
 */
enum ZNMeetingAICompanionQueryRequestError
{
	ZNMeetingAICompanionQueryRequestError_OK = 0,					///<OK.
	ZNMeetingAICompanionQueryRequestError_InvalidParam,				///<InvalidParam.
	ZNMeetingAICompanionQueryRequestError_SendFailed,				///<SendFailed.
	ZNMeetingAICompanionQueryRequestError_WebUnAvailable,			///<WebUnAvailable.
	ZNMeetingAICompanionQueryRequestError_PermissionVerifyFailed,	///<PermissionVerifyFailed.
	ZNMeetingAICompanionQueryRequestError_QueryRateLimitError,		///<QueryRateLimitError.
	ZNMeetingAICompanionQueryRequestError_Timeout,					///<Timeout.
	ZNMeetingAICompanionQueryRequestError_Unknown = 100				///<Unknown.
};

/*! \enum ZNMeetingAICompanionQuerySettingOptions
 * \brief Config who in the meeting can interact with AI Companion by asking questions about the meeting. please visit https://support.zoom.com/hc/en/article?id=zm_kb&sysparm_article=KB0057749
 */
enum ZNMeetingAICompanionQuerySettingOptions
{
	ZNMeetingAICompanionQuerySettingOptions_None = 0, ///<Initialization.
	ZNMeetingAICompanionQuerySettingOptions_WhenQueryStarted, ///<Allows all participants and attendee to ask questions about all discussions since AI Companion was active.
	ZNMeetingAICompanionQuerySettingOptions_WhenParticipantsJoin, ///<Allows all participants to ask questions about the discussion since they joined the current meeting.
	ZNMeetingAICompanionQuerySettingOptions_OnlyHost, ///<Allow only the host, alternative host, and users with host privileges assigned before the meeting starts to ask questions about all discussions since AI Companion is active.
	ZNMeetingAICompanionQuerySettingOptions_ParticipantsAndInviteesInOurOrganization, ///<Allows only participants and invitees from your organization to ask questions about all discussions since AI Companion was active.
	ZNMeetingAICompanionQuerySettingOptions_WhenParticipantsAndOrganizationJoin, ///<Allows only participants from your organization to ask questions about the discussion since they joined the current meeting.
	ZNMeetingAICompanionQuerySettingOptions_HostAndPanelists = 10, ///<Hosts and all panelists can ask question, and answers are based on the meeting's start until now.
	ZNMeetingAICompanionQuerySettingOptions_HostAndPanelistsInOurOrganization ///<Hosts and all panelists in our organization can ask question, and answers are based on the meeting's start until now.
};

typedef struct _ZNMeetingAICompanionQueryItem
{
	ZoomSTRING queryID;///<the query question ID.
	ZoomSTRING qustionContent;///<the query question content.
	ZoomSTRING answerContent;///<the query answer content.
	ZNMeetingAICompanionQueryRequestError errorCode;///<the error code.
	ZoomSTRING errorMsg;///<the error message.
	time_t timeStamp;///<the timestamps of the current message.
}ZNMeetingAICompanionQueryItem;

/*!  \enum ZNSDKVideoPreferenceMode
	\brief Select and use any of the defined preference mode below when initializing the SDKVideoPreferenceSetting.
	Video preference modes determined the video frame rate and resolution based on the user's bandwidth.
	Here are more detailed structural descriptions.
*/
typedef enum
{
	ZNSDKVideoPreferenceMode_Balance, ///<Balance mode. Default Preference, no additional parameters needed. Zoom will do what is best under the current bandwidth situation and make adjustments as needed.
	ZNSDKVideoPreferenceMode_Sharpness, ///<Sharpness mode. Prioritizes a smooth video frame transition by preserving the frame rate as much as possible.
	ZNSDKVideoPreferenceMode_Smoothness, ///<Smoothness mode. Prioritizes a sharp video image by preserving the resolution as much as possible.
	ZNSDKVideoPreferenceMode_Custom	///<Custom mode. Allows customization by providing the minimum and maximum frame rate. Use this mode if you have an understanding of your network behavior and a clear idea on how to adjust the frame rate to achieve the desired video quality.
}ZNSDKVideoPreferenceMode;

/*! \struct ZNSDKVideoPreferenceSetting
	\brief When setting custom modes, the developer provides the maximum and minimum frame rates.
	If the current bandwidth cannot maintain the minimum frame rate, the video system will drop to the next lower resolution.
	The default maximum and minimum frame rates for other modes are 0.
*/
typedef struct tagSDKVideoPreferenceSetting
{
	ZNSDKVideoPreferenceMode mode;  ///<0: Balance mode; 1: Smoothness mode; 2: Sharpness mode; 3: Custom mode
	uint32_t minimumFrameRate; ///<0 for the default value,minimum_frame_rate should be less than maximum_frame_rate, range: from 0 to 30 .out of range for frame-rate will use default frame-rate of Zoom	
	uint32_t maximumFrameRate; ///<0 for the default value,maximum_frame_rate should be less and equal than 30, range: from 0 to 30.out of range for frame-rate will use default frame-rate of Zoom
	tagSDKVideoPreferenceSetting()
	{
		mode = ZNSDKVideoPreferenceMode_Balance;
		minimumFrameRate = 0;
		maximumFrameRate = 0;
	}
}ZNSDKVideoPreferenceSetting;

typedef struct _ZNAnswerItem {
    time_t timeStamp;
    ZoomSTRING text; ///<Get the text of the answer.
    ZoomSTRING senderName; ///<Get the sender's name of the answer.
    ZoomSTRING questionID; ///<Get the related question's id of the answer.
    ZoomSTRING answerID; ///<Get the answer id.
    bool isPrivate; ///<Determine if the answer is private or not.
    bool isLiveAnswer; ///<Determine if the answer is live or not.
    bool isSenderMyself; ///<Determine whether the answer's sender is the user himself or not.
}ZNAnswerItem;

typedef struct _ZNQAItemInfo {
    time_t timeStamp;
    unsigned int upvoteNum; ///<Get the number of the up_voters of the question.
    ZoomSTRING text; ///<Get the text of the question.
    ZoomSTRING senderName; ///<Get the sender's name of the question.
    ZoomSTRING questionID; ///<Get the question id.
    bool isAnonymous; ///<Determine if the question is anonymous.
    bool isMarkedAsAnswered; ///<Determine if the question is marked as answered.
    bool isMarkedAsDismissed; ///<Determine if the question is marked as dismissed.
    bool isSenderMyself; ///<Determine if the question's sender is the user himself or not.
    bool isMySelfUpvoted; ///<Determine if the user himself is an up_voter of the question or not.
    bool hasLiveAnswers; ///<Determine if the question has live answers or not.
    bool hasTextAnswers; ///<Determine if the question has text answers or not.
    bool amILiveAnswering; ///<Determine if the user himself is answering the question live or not.
    ZoomSTRING liveAnswerName; ///<Get all the users' names who answers the question live.
    bool isLiveAnswering; ///<Determine if the question is being answered live or not.
    std::vector<ZNAnswerItem> answerList; ///<Get the list of all the answers to the question.
}ZNQAItemInfo;

/**
 * @brief Enumerations of the emoji reaction type.
 */
typedef enum
{
    ZNSDKEmojiReactionType_Unknow,
    ZNSDKEmojiReactionType_Clap,
    ZNSDKEmojiReactionType_Thumbsup,
    ZNSDKEmojiReactionType_Heart,
    ZNSDKEmojiReactionType_Joy,
    ZNSDKEmojiReactionType_Openmouth,
    ZNSDKEmojiReactionType_Tada,
}ZNSDKEmojiReactionType;

/**
 * @brief Enumerations of the emoji reaction skintone.
 */
typedef enum
{
    ZNSDKEmojiReactionSkinTone_Unknow,
    ZNSDKEmojiReactionSkinTone_Default,
    ZNSDKEmojiReactionSkinTone_Light,
    ZNSDKEmojiReactionSkinTone_MediumLight,
    ZNSDKEmojiReactionSkinTone_Medium,
    ZNSDKEmojiReactionSkinTone_MediumDark,
    ZNSDKEmojiReactionSkinTone_Dark,
}ZNSDKEmojiReactionSkinTone;

/**
 * @brief Enumerations of the type for live transcription operation type.
 */
typedef enum
{
    ZNSDK_LiveTranscription_OperationType_None,
    ZNSDK_LiveTranscription_OperationType_Add,
    ZNSDK_LiveTranscription_OperationType_Update,
    ZNSDK_LiveTranscription_OperationType_Delete,
    ZNSDK_LiveTranscription_OperationType_Complete,
    ZNSDK_LiveTranscription_OperationType_NotSupported,
}ZNSDKLiveTranscriptionOperationType;

typedef struct _ZNLiveTranscriptionMessageInfo {
    ZoomSTRING messageID;
    unsigned int speakerID;
    ZoomSTRING speakerName;
    ZoomSTRING messageContent;
    time_t timeStamp;
    ZNSDKLiveTranscriptionOperationType messageType;
}ZNLiveTranscriptionMessageInfo;

typedef enum
{
    ZNSDKPollingStatus_Initial, ///<The initial status.
    ZNSDKPollingStatus_Started, ///<User started polling.
    ZNSDKPollingStatus_ShareResult, ///<User shared polling result.
    ZNSDKPollingStatus_Stopped, ///<User stopped polling.
}ZNSDKPollingStatus;

typedef enum
{
    ZNSDKPollingType_Unknown,
    ZNSDKPollingType_Poll,
    ZNSDKPollingType_Quiz,
}ZNSDKPollingType;

typedef enum
{
    ZNSDKPollingQuestionType_Unknown,
    ZNDKPollingQuestionType_Single,
    ZNSDKPollingQuestionType_Multi,
    ZNSDKPollingQuestionType_Matching,
    ZNSDKPollingQuestionType_RankOrder,
    ZNSDKPollingQuestionType_ShortAnswer,
    ZNSDKPollingQuestionType_LongAnswer,
    ZNSDKPollingQuestionType_FillBlank,
    ZNSDKPollingQuestionType_NPS,
    ZNSDKPollingQuestionType_Dropdown,
}ZNSDKPollingQuestionType;

typedef enum
{
    ZNSDKPollingActionType_Unknown,
    ZNSDKPollingActionType_Start,
    ZNSDKPollingActionType_Stop,
    ZNSDKPollingActionType_ShareResult,
    ZNSDKPollingActionType_StopShareResult,
    ZNSDKPollingActionType_Duplicate,
    ZNSDKPollingActionType_Delete,
    ZNSDKPollingActionType_Submit,
    ZNSDKPollingActionType_Error
}ZNSDKPollingActionType;

typedef struct _ZNSDKPollingItem {
    ZoomSTRING pollingID;
    ZoomSTRING pollingName;
    ZNSDKPollingType pollingType;
    ZNSDKPollingStatus pollingStatus;
    unsigned int pollingQuestionCount;
    unsigned int totalVotedUserCount;
    bool isLibraryPolling;
}ZNSDKPollingItem;

typedef struct _ZNSDKPollingAnswerItem {
    ZoomSTRING pollingID;
    ZoomSTRING pollingQuestionID;
    ZoomSTRING pollingSubQuestionID;
    ZoomSTRING pollingAnswerID;
    ZoomSTRING pollingAnswerName;
    ZoomSTRING pollingAnsweredContent;
    bool isChecked;
}ZNSDKPollingAnswerItem;

typedef struct _ZNSDKPollingAnswerResultItem {
    ZoomSTRING pollingID;
    ZoomSTRING pollingQuestionID;
    ZoomSTRING pollingSubQuestionID;
    ZoomSTRING pollingAnswerID;
    ZoomSTRING pollingAnswerName;
    unsigned int selectedCount;
}ZNSDKPollingAnswerResultItem;

typedef struct _ZNSDKPollingQuestionItem {
    ZoomSTRING pollingID;
    ZoomSTRING pollingQuestionID;
    ZoomSTRING pollingQuestionName;
    ZNSDKPollingQuestionType pollingQuestionType;
    unsigned int answeredCount;
    bool isRequired;
    std::vector<_ZNSDKPollingQuestionItem> pollingSubQuestionItemList;
    std::vector<ZNSDKPollingAnswerItem> pollingAnswerItemList;
}ZNSDKPollingQuestionItem;

enum ZNCannotShareReasonType
{
    ZNCannotShareReasonType_None,
    ZNCannotShareReasonType_Locked,                          ///<Only the host can share.
    ZNCannotShareReasonType_Disabled,                        ///<Sharing is disabled.
    ZNCannotShareReasonType_Other_Screen_Sharing,            ///<Another is sharing their screen.
    ZNCannotShareReasonType_Other_WB_Sharing,                ///<Another is sharing their whiteboard.
    ZNCannotShareReasonType_Need_Grab_Myself_Screen_Sharing, ///<The user is sharing their screen, and can grab. To grab, call EnableGrabShareWithoutReminder(true) before starting share.
    ZNCannotShareReasonType_Need_Grab_Other_Screen_Sharing,  ///<Another is sharing their screen, and can grab. To grab, call EnableGrabShareWithoutReminder(true) before starting share.
    ZNCannotShareReasonType_Need_Grab_Audio_Sharing,         ///<Another is sharing pure computer audio, and can grab. To grab, call EnableGrabShareWithoutReminder(true) before starting share.
    ZNCannotShareReasonType_Need_Grap_WB_Sharing,            ///<Other or myself is sharing whiteboard, and can Grab. To grab, call EnableGrabShareWithoutReminder(true) before starting share.
    ZNCannotShareReasonType_Reach_Maximum,                   ///<The meeting has reached the maximum allowed screen share sessions.
    ZNCannotShareReasonType_Have_Share_From_Mainsession,     ///<Other share screen in main session.
    ZNCannotShareReasonType_Other_DOCS_Sharing,			   	 ///<Another participant is sharing their zoom docs.
    ZNCannotShareReasonType_Need_Grab_DOCS_Sharing,          ///<Other or myself is sharing docs, and can grab. To grab, call EnableGrabShareWithoutReminder(true) before starting share.
    ZNCannotShareReasonType_UnKnown,
};

/*! \enum ZNSDKWhiteboardShareOption
	\brief Whiteboard who can share setting type.
*/
typedef enum
{
	ZNSDKWhiteboardShareOption_HostShare,         ///<Only the host can share a whiteboard.
	ZNSDKWhiteboardShareOption_HostGrabShare,     ///<Anyone can share a whiteboard, but only one can share at a time, and only the host can take another's sharing role.
	ZNSDKWhiteboardShareOption_AllGrabShare       ///<Anyone can share a whiteboard, but only one can share at a time, and anyone can take another's sharing role.
}ZNSDKWhiteboardShareOption;

/*! \enum ZNSDKWhiteboardCreateOption
	\brief Whiteboard who can initiate new whiteboard setting type.
*/
typedef enum
{
	ZNSDKWhiteboardCreateOption_HostOnly,         ///<Only the host can initiate a new whiteboard.
	ZNSDKWhiteboardCreateOption_AccountUsers,     ///<Users under the same account as the meeting owner can initiate a new whiteboard.
	ZNSDKWhiteboardCreateOption_All               ///<All participants can initiate a new whiteboard.
} ZNSDKWhiteboardCreateOption;

/*! \enum WhiteboardStatus
	\brief Status of whiteboard.
*/
typedef enum
{
	ZNSDKWhiteboardStatus_Started, ///< User stared sharing their whiteboard.
	ZNSDKWhiteboardStatus_Stopped, ///< User stopped sharing their whiteboard.
}ZNSDKWhiteboardStatus;

/*! \enum ZNSDKDocsStatus
	\brief Docs share status.
*/
typedef enum
{
    ZNSDKDocsStatus_None, 
    ZNSDKDocsStatus_Start, 		///<User starts sharing docs.
    ZNSDKDocsStatus_Stop 		///<User stops sharing docs.
}ZNSDKDocsStatus;

/*! \enum ZNSDKDocsShareOption
	\brief Docs share option
*/
typedef enum
{
    ZNSDKDocsShareOption_None, 				///<A wrong option, such as the meeting not supporting docs.
    ZNSDKDocsShareOption_HostShare, 		///<Only host can share docs.
    ZNSDKDocsShareOption_HostGrabShare, 	///<Anyone can share docs, but only one doc can be shared at a time, and only host can take over another's sharing.
    ZNSDKDocsShareOption_AllGrabShare 		///<Anyone can share docs, but only one doc can be shared at a time, and anyone can take over another's sharing.
}ZNSDKDocsShareOption;

/*! \enum ZNSDKDocsCreateOption
	\brief Docs create option
*/
typedef enum
{
    ZNSDKDocsCreateOption_None, 				///<A wrong option, maybe the meeting does not support docs.
    ZNSDKDocsCreateOption_HostOnly, 			///<Only the host can initiate new docs.
    ZNSDKDocsCreateOption_AccountUsers, 		///<Users under the same account can initiate new docs.
    ZNSDKDocsCreateOption_All 					///<All participants can initiate new docs.
}ZNSDKDocsCreateOption;


typedef struct _ZNZoomSDKDocSharingSourceInfo
{
	ZNSDKDocsStatus status; ///<Get the status of zoom docs sharing
	ZoomSTRING docTitle; ///<Get the title of the sharing zoom docs.
	unsigned int shareSourceID; ///<Get the share source id of the sharing zoom docs
	unsigned int userID; ///<Get the user id who is sharing zoom docs.
}ZNZoomSDKDocSharingSourceInfo;

////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ZNList std::vector
//////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
#if (defined BUILD_WIN)
#define JS_WRAP
//#define UserInterfaceClass
#endif
