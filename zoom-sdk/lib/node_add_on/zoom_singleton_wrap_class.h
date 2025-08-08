#ifndef _zoom_singleton_wrap_class_h_
#define _zoom_singleton_wrap_class_h_
#include "zoom_node_common_include.h"
#include "zoom_sdk_napi_util_exporter.h"

template<class T>
static void InitClassAttribute(const v8::Local<v8::FunctionTemplate>& tpl, v8::Isolate* isolate)
{}

template<class T>
static v8::Persistent<v8::Function>* GetConstructor(){
	return NULL;
}

template<class T>
class ZoomWrapObject : public node::ObjectWrap
{
public:
	static void Init(v8::Isolate* isolate)
	{
		// Prepare constructor template
		v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(isolate, New);
		InitClassAttribute<T >(tpl, isolate);
		v8::Local<v8::Context> context = isolate->GetCurrentContext();
		(*GetConstructor<T >()).Reset(isolate, tpl->GetFunction(context).ToLocalChecked());
	}
	static void NewInstance(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		v8::Isolate* isolate = args.GetIsolate();

		const unsigned argc = 1;
		v8::Local<v8::Value> argv[argc] = { args[0] };
		v8::Local<v8::Function> cons = v8::Local<v8::Function>::New(isolate, (*GetConstructor<T >()));
		v8::Local<v8::Context> context = isolate->GetCurrentContext();
		v8::Local<v8::Object> instance =
			cons->NewInstance(context, argc, argv).ToLocalChecked();

		args.GetReturnValue().Set(instance);
	}

	static void New(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		v8::Isolate* isolate = args.GetIsolate();
		v8::Local<v8::Context> context = isolate->GetCurrentContext();

		if (args.IsConstructCall()) {
			T* obj = new T();
			obj->Wrap(args.This());
			args.GetReturnValue().Set(args.This());
		}
		else {
			const int argc = 1;
			v8::Local<v8::Value> argv[argc] = { args[0] };
			v8::Local<v8::Function> cons = v8::Local<v8::Function>::New(isolate, (*GetConstructor<T >()));
			v8::Local<v8::Object> instance =
				cons->NewInstance(context, argc, argv).ToLocalChecked();
			args.GetReturnValue().Set(instance);
		}
	}
	static v8::Local<v8::Object> GetNewInstance(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		v8::Isolate* isolate = args.GetIsolate();

		const unsigned argc = 1;
		v8::Local<v8::Value> argv[argc] = { args[0] };
		v8::Local<v8::Function> cons = v8::Local<v8::Function>::New(isolate, (*GetConstructor<T >()));
		v8::Local<v8::Context> context = isolate->GetCurrentContext();
		v8::Local<v8::Object> instance =
			cons->NewInstance(context, argc, argv).ToLocalChecked();
		return instance;
	}
protected:
	ZoomWrapObject() {};
	~ZoomWrapObject() {};
};

class ZoomNodeSinkHelper
{
public:
	//auth_service_cb
	v8::Persistent<v8::Function> onAuthenticationReturn;
	v8::Persistent<v8::Function> onLogout;
	v8::Persistent<v8::Function> onZoomIdentityExpired;
	v8::Persistent<v8::Function> onZoomAuthIdentityExpired;
	v8::Persistent<v8::Function> onLoginReturnWithReason;

	//meeting_service_cb
	v8::Persistent<v8::Function> onMeetingStatusChanged;
	v8::Persistent<v8::Function> onMeetingStatisticsWarningNotification;
	v8::Persistent<v8::Function> onMeetingParameterNotification;
	v8::Persistent<v8::Function> onMeetingTopicChanged;
	v8::Persistent<v8::Function> onMeetingFullToWatchLiveStream;
	
	//premeeting_service_cb
	v8::Persistent<v8::Function> onScheduleOrEditMeeting;
	v8::Persistent<v8::Function> onListMeeting;
	v8::Persistent<v8::Function> onDeleteMeeting;
	v8::Persistent<v8::Function> onGetInviteEmailContent;
	//meetingUI_Controller_cb
	v8::Persistent<v8::Function> onInviteBtnClicked;
	v8::Persistent<v8::Function> onStartShareBtnClicked;
	v8::Persistent<v8::Function> onEndMeetingBtnClicked;
	v8::Persistent<v8::Function> onParticipantListBtnClicked;
	v8::Persistent<v8::Function> onCustomLiveStreamMenuClicked;
	v8::Persistent<v8::Function> onZoomInviteDialogFailed;
	v8::Persistent<v8::Function> onCCBTNClicked;
	v8::Persistent<v8::Function> onAudioBtnClicked;
	v8::Persistent<v8::Function> onAudioMenuBtnClicked;

	//meeting_audio_Controller_cb
	v8::Persistent<v8::Function> onUserAudioStatusChange;
	v8::Persistent<v8::Function> onUserActiveAudioChange;
	v8::Persistent<v8::Function> onMuteOnEntryStatusChange;

	//meeting_video_Controller_cb
	v8::Persistent<v8::Function> onUserVideoStatusChange;
	v8::Persistent<v8::Function> onActiveSpeakerVideoUserChanged;
	v8::Persistent<v8::Function> onActiveVideoUserChanged;
	
	//meeting_recording_Controller_cb
	v8::Persistent<v8::Function> onRecording2MP4Done;
	v8::Persistent<v8::Function> onRecording2MP4Processing;
	v8::Persistent<v8::Function> onRecordingStatus;
	v8::Persistent<v8::Function> onCloudRecordingStatus;
	v8::Persistent<v8::Function> onRecordPrivilegeChanged;
	v8::Persistent<v8::Function> onCloudRecordingStorageFull;
	v8::Persistent<v8::Function> onRequestCloudRecordingResponse;
	v8::Persistent<v8::Function> onStartCloudRecordingRequested;
	v8::Persistent<v8::Function> onEnableAndStartSmartRecordingRequested;
	v8::Persistent<v8::Function> onSmartRecordingEnableActionCallback;

	//meeting_share_Controller_cb
	v8::Persistent<v8::Function> onSharingStatus;
	v8::Persistent<v8::Function> onFailedToStartShare;
	v8::Persistent<v8::Function> onOptimizingShareForVideoClipStatusChanged;

	//meeting_annotation_controller_cb
	v8::Persistent<v8::Function> onSupportAnnotationStatus;

	//meeting_participants_Controller_cb
	v8::Persistent<v8::Function> onUserJoin;
	v8::Persistent<v8::Function> onUserLeft;
	v8::Persistent<v8::Function> onHostChangeNotification;
	v8::Persistent<v8::Function> onAllowParticipantsRequestCloudRecording;
	v8::Persistent<v8::Function> onBotAuthorizerRelationChanged;

	//meeting_h323_Controller_cb
	v8::Persistent<v8::Function> onH323CalloutStatusNotify;

	//meeting_config_Controller_cb
	v8::Persistent<v8::Function> onFreeMeetingNeedToUpgrade;
	v8::Persistent<v8::Function> onFreeMeetingUpgradeToGiftFreeTrialStart;
	v8::Persistent<v8::Function> onFreeMeetingUpgradeToGiftFreeTrialStop;
	v8::Persistent<v8::Function> onFreeMeetingUpgradeToProMeeting;

	v8::Persistent<v8::Function> onInputMeetingPasswordAndScreenNameNotification;
	v8::Persistent<v8::Function> onAirPlayInstructionWndNotification;
	v8::Persistent<v8::Function> onWebinarNeedRegisterNotification;
	v8::Persistent<v8::Function> onEndOtherMeetingToJoinMeetingNotification;
	v8::Persistent<v8::Function> onFreeMeetingRemainTime;
	v8::Persistent<v8::Function> onFreeMeetingRemainTimeStopCountDown;
	v8::Persistent<v8::Function> onWebinarNeedInputScreenName;
	v8::Persistent<v8::Function> onJoinMeetingNeedUserInfo;
	v8::Persistent<v8::Function> onUserConfirmToStartArchive;
	

	//direct_share_helper_cb
	v8::Persistent<v8::Function> OnDirectShareStatusUpdate;

	//sdk_sms_helper_cb
	v8::Persistent<v8::Function> onNeedRealNameAuthMeetingNotification;
	v8::Persistent<v8::Function> onRetrieveSMSVerificationCodeResultNotification;
	v8::Persistent<v8::Function> onVerifySMSVerificationCodeResultNotification;

	//audio_setting_context_cb
	v8::Persistent<v8::Function> onComputerMicDeviceChanged;
	v8::Persistent<v8::Function> onComputerSpeakerDeviceChanged;
	v8::Persistent<v8::Function> onDefaultMicDeviceChanged;
	v8::Persistent<v8::Function> onDefaultSpeakerDeviceChanged;

	//video_setting_context_cb
	v8::Persistent<v8::Function> onComputerCamDeviceChanged;
	v8::Persistent<v8::Function> onDefaultCamDeviceChanged;

	//recording_setting_context_cb
	v8::Persistent<v8::Function> onCloudRecordingStorageInfo;

	//wallpaper_setting_context_cb
	v8::Persistent<v8::Function> onMeetingWallpaperChanged;
	v8::Persistent<v8::Function> onMeetingWallpaperImageDownloadStatus;
	v8::Persistent<v8::Function> onPersonalWallpaperChanged;
	v8::Persistent<v8::Function> onPersonalWallpaperImageDownloadStatus;

	//virtual_bg_setting_context_cb
	v8::Persistent<v8::Function> onVBImageDidDownloaded;
	v8::Persistent<v8::Function> onGreenVBDidUpdateWithReplaceColor;
	v8::Persistent<v8::Function> onSelectedVBImageChanged;
	v8::Persistent<v8::Function> onVBVideoUploadedResult;

	//rawdata_render_cb
	v8::Persistent<v8::Function> onRawDataStatusChanged;

	//meeting_live_stream_controller_cb
	v8::Persistent<v8::Function> onLiveStreamStatusChange;
	v8::Persistent<v8::Function> onRawLiveStreamPrivilegeChanged;
	v8::Persistent<v8::Function> onUserRawLiveStreamPrivilegeChanged;
	v8::Persistent<v8::Function> onRawLiveStreamPrivilegeRequested;
	v8::Persistent<v8::Function> onUserRawLiveStreamingStatusChanged;
	v8::Persistent<v8::Function> onRawLiveStreamPrivilegeRequestTimeout;
	v8::Persistent<v8::Function> onLiveStreamReminderStatusChanged;
	v8::Persistent<v8::Function> onLiveStreamReminderStatusChangeFailed;
	v8::Persistent<v8::Function> onUserThresholdReachedForLiveStream;

	//meeting_reminder_controller_cb
	v8::Persistent<v8::Function> onReminderNotify;
	v8::Persistent<v8::Function> onEnableReminderNotify;

	//meeting_chat_controller_cb
	v8::Persistent<v8::Function> onChatMsgNotification;
	v8::Persistent<v8::Function> onChatStatusChangedNotification;
	v8::Persistent<v8::Function> onChatMsgDeleteNotification;
	v8::Persistent<v8::Function> onShareMeetingChatStatusChanged;
	v8::Persistent<v8::Function> onFileSendStart;
	v8::Persistent<v8::Function> onFileReceived;
	v8::Persistent<v8::Function> onFileTransferProgress;
	v8::Persistent<v8::Function> onChatMessageEditNotification;

	//meeting_waiting_room_controller_cb
	v8::Persistent<v8::Function> onWaitingRoomUserJoin;
	v8::Persistent<v8::Function> onWaitingRoomUserLeft;
	v8::Persistent<v8::Function> onWaitingRoomPresetAudioStatusChanged;
	v8::Persistent<v8::Function> onWaitingRoomPresetVideoStatusChanged;
	v8::Persistent<v8::Function> onCustomWaitingRoomDataUpdated;
	v8::Persistent<v8::Function> onWaitingRoomUserNameChanged;
	v8::Persistent<v8::Function> onWaitingRoomEntranceEnabled;

	//meeting_ai_companion_controller_cb
	v8::Persistent<v8::Function> onAICompanionFeatureTurnOffByParticipant;
	v8::Persistent<v8::Function> onAICompanionFeatureSwitchRequested;
	v8::Persistent<v8::Function> onAICompanionFeatureSwitchRequestResponse;
	v8::Persistent<v8::Function> onAICompanionFeatureCanNotBeTurnedOff;

	v8::Persistent<v8::Function> onSmartSummaryStateNotSupported;
	v8::Persistent<v8::Function> onSmartSummaryStateSupportedButDisabled;
	v8::Persistent<v8::Function> onSmartSummaryStateEnabledButNotStarted;
	v8::Persistent<v8::Function> onSmartSummaryStateStarted;
	v8::Persistent<v8::Function> onFailedToStartSmartSummary;
	v8::Persistent<v8::Function> onSmartSummaryEnableRequestReceived;
	v8::Persistent<v8::Function> onSmartSummaryStartRequestReceived;
	v8::Persistent<v8::Function> onSmartSummaryEnableActionCallback;

	v8::Persistent<v8::Function> onQueryStateNotSupported;
	v8::Persistent<v8::Function> onQueryStateSupportedButDisabled;
	v8::Persistent<v8::Function> onQueryStateEnabledButNotStarted;
	v8::Persistent<v8::Function> onQueryStateStarted;
	v8::Persistent<v8::Function> onQuerySettingChanged;
	v8::Persistent<v8::Function> onFailedToStartQuery;
	v8::Persistent<v8::Function> onReceiveRequestToEnableQuery;
	v8::Persistent<v8::Function> onReceiveRequestToStartQuery;
	v8::Persistent<v8::Function> onReceiveQueryAnswer;
	v8::Persistent<v8::Function> onQueryEnableActionCallback;
	v8::Persistent<v8::Function> onSendQueryPrivilegeChanged;
	v8::Persistent<v8::Function> onFailedToRequestSendQuery;
	v8::Persistent<v8::Function> onReceiveRequestToSendQuery;

	//meeting_indicator_controller_cb
	v8::Persistent<v8::Function> onIndicatorItemReceived;
	v8::Persistent<v8::Function> onIndicatorItemRemoved;

	//meeting_webinar_controller_cb
	v8::Persistent<v8::Function> onPromptAttendee2PanelistResult;
	v8::Persistent<v8::Function> onDepromptPanelist2AttendeeResult;
	v8::Persistent<v8::Function> onAllowPanelistStartVideoNotification;
	v8::Persistent<v8::Function> onDisallowPanelistStartVideoNotification;
	v8::Persistent<v8::Function> onSelfAllowTalkNotification;
	v8::Persistent<v8::Function> onSelfDisallowTalkNotification;
	v8::Persistent<v8::Function> onAllowAttendeeChatNotification;
	v8::Persistent<v8::Function> onDisallowAttendeeChatNotification;
	v8::Persistent<v8::Function> onAllowWebinarReactionStatusChanged;
	v8::Persistent<v8::Function> onAllowAttendeeRaiseHandStatusChanged;
	v8::Persistent<v8::Function> onAllowAttendeeViewTheParticipantCountStatusChanged;
	v8::Persistent<v8::Function> onAttendeeAudioStatusNotification;
	v8::Persistent<v8::Function> onAttendeePromoteConfirmResult;

	//meeting_qa_controller_cb
	v8::Persistent<v8::Function> onAddQuestion;
	v8::Persistent<v8::Function> onReceiveQuestion;
	v8::Persistent<v8::Function> onDeleteQuestions;
	v8::Persistent<v8::Function> onQuestionMarkedAsDismissed;
	v8::Persistent<v8::Function> onReopenQuestion;
	v8::Persistent<v8::Function> onAddAnswer;
	v8::Persistent<v8::Function> onReceiveAnswer;
	v8::Persistent<v8::Function> onDeleteAnswers;
	v8::Persistent<v8::Function> onUserLivingReply;
	v8::Persistent<v8::Function> onUserEndLiving;
	v8::Persistent<v8::Function> onVoteupQuestion;
	v8::Persistent<v8::Function> onRevokeVoteupQuestion;

	//meeting_reaction_controller_cb
	v8::Persistent<v8::Function> onEmojiReactionReceived;
	v8::Persistent<v8::Function> onEmojiReactionReceivedInWebinar;

	//close_caption_controller_cb
	v8::Persistent<v8::Function> onLiveTranscriptionMsgInfoReceived;
	v8::Persistent<v8::Function> onOriginalLanguageMsgReceived;

	//polling_controller_cb
	v8::Persistent<v8::Function> onPollingStatusChanged;
	v8::Persistent<v8::Function> onPollingResultUpdated;
	v8::Persistent<v8::Function> onPollingListUpdated;
	v8::Persistent<v8::Function> onPollingActionResult;

	//meeting_whiteboard_controller_cb
	v8::Persistent<v8::Function> onWhiteboardStatusChanged;
	v8::Persistent<v8::Function> onWhiteboardSettingsChanged;

	//meeting_docs_controller_cb
	v8::Persistent<v8::Function> onDocsSharingSourceInfoChanged;
	v8::Persistent<v8::Function> onDocsPermissionChanged;

	static ZoomNodeSinkHelper& GetInst()
	{
		static ZoomNodeSinkHelper inst;
		return inst;
	}

	void Reset()
	{
		onAuthenticationReturn.Reset();
		onLogout.Reset();
		onZoomIdentityExpired.Reset();
		onZoomAuthIdentityExpired.Reset();
		onMeetingStatusChanged.Reset();
		onMeetingStatisticsWarningNotification.Reset();
		onMeetingParameterNotification.Reset();
		onMeetingTopicChanged.Reset();
		onMeetingFullToWatchLiveStream.Reset();

		onScheduleOrEditMeeting.Reset();
		onListMeeting.Reset();
		onDeleteMeeting.Reset();
		onInviteBtnClicked.Reset();
		onStartShareBtnClicked.Reset();
		onEndMeetingBtnClicked.Reset();
		onParticipantListBtnClicked.Reset();
		onCustomLiveStreamMenuClicked.Reset();
		onZoomInviteDialogFailed.Reset();
		onCCBTNClicked.Reset();
		onAudioBtnClicked.Reset();
		onAudioMenuBtnClicked.Reset();

		onUserAudioStatusChange.Reset();
		onUserActiveAudioChange.Reset();
		onMuteOnEntryStatusChange.Reset();

		onUserVideoStatusChange.Reset();
		onActiveSpeakerVideoUserChanged.Reset();
		onActiveVideoUserChanged.Reset();

		onRecording2MP4Done.Reset();
		onRecording2MP4Processing.Reset();
		onRecordingStatus.Reset();
		onCloudRecordingStatus.Reset();
		onRecordPrivilegeChanged.Reset();
		onCloudRecordingStorageFull.Reset();
		onRequestCloudRecordingResponse.Reset();
		onStartCloudRecordingRequested.Reset();
		onEnableAndStartSmartRecordingRequested.Reset();
		onSmartRecordingEnableActionCallback.Reset();

		onSharingStatus.Reset();
		onFailedToStartShare.Reset();
		onOptimizingShareForVideoClipStatusChanged.Reset();

		onSupportAnnotationStatus.Reset();

		onUserJoin.Reset();
		onUserLeft.Reset();
		onHostChangeNotification.Reset();
		onAllowParticipantsRequestCloudRecording.Reset();
		onBotAuthorizerRelationChanged.Reset();

		onH323CalloutStatusNotify.Reset();

		onFreeMeetingNeedToUpgrade.Reset();
		onFreeMeetingUpgradeToGiftFreeTrialStart.Reset();
		onFreeMeetingUpgradeToGiftFreeTrialStop.Reset();
		onFreeMeetingUpgradeToProMeeting.Reset();

		onInputMeetingPasswordAndScreenNameNotification.Reset();
		onAirPlayInstructionWndNotification.Reset();
		onWebinarNeedRegisterNotification.Reset();
		onEndOtherMeetingToJoinMeetingNotification.Reset();
		onFreeMeetingRemainTime.Reset();
		onFreeMeetingRemainTimeStopCountDown.Reset();
		onWebinarNeedInputScreenName.Reset();
		onJoinMeetingNeedUserInfo.Reset();
		onUserConfirmToStartArchive.Reset();


		OnDirectShareStatusUpdate.Reset();

		onNeedRealNameAuthMeetingNotification.Reset();
		onRetrieveSMSVerificationCodeResultNotification.Reset();
		onVerifySMSVerificationCodeResultNotification.Reset();

		onComputerMicDeviceChanged.Reset();
		onComputerSpeakerDeviceChanged.Reset();
		onDefaultMicDeviceChanged.Reset();
		onDefaultSpeakerDeviceChanged.Reset();

		onComputerCamDeviceChanged.Reset();
		onDefaultCamDeviceChanged.Reset();

		onCloudRecordingStorageInfo.Reset();

		onMeetingWallpaperChanged.Reset();
		onMeetingWallpaperImageDownloadStatus.Reset();
		onPersonalWallpaperChanged.Reset();
		onPersonalWallpaperImageDownloadStatus.Reset();

		onVBImageDidDownloaded.Reset();
		onGreenVBDidUpdateWithReplaceColor.Reset();
		onSelectedVBImageChanged.Reset();
		onVBVideoUploadedResult.Reset();

		onRawDataStatusChanged.Reset();

		onLoginReturnWithReason.Reset();
		onGetInviteEmailContent.Reset();

		onLiveStreamStatusChange.Reset();
		onRawLiveStreamPrivilegeChanged.Reset();
		onUserRawLiveStreamPrivilegeChanged.Reset();
		onRawLiveStreamPrivilegeRequested.Reset();
		onUserRawLiveStreamingStatusChanged.Reset();
		onRawLiveStreamPrivilegeRequestTimeout.Reset();

		onLiveStreamReminderStatusChanged.Reset();
		onLiveStreamReminderStatusChangeFailed.Reset();
		onUserThresholdReachedForLiveStream.Reset();

		onReminderNotify.Reset();
		onEnableReminderNotify.Reset();

		onChatMsgNotification.Reset();
		onChatStatusChangedNotification.Reset();
		onChatMsgDeleteNotification.Reset();
		onShareMeetingChatStatusChanged.Reset();
		onFileSendStart.Reset();
		onFileReceived.Reset();
		onFileTransferProgress.Reset();
		onChatMessageEditNotification.Reset();

		onWaitingRoomUserJoin.Reset();
		onWaitingRoomUserLeft.Reset();
		onWaitingRoomPresetAudioStatusChanged.Reset();
		onWaitingRoomPresetVideoStatusChanged.Reset();
		onCustomWaitingRoomDataUpdated.Reset();
		onWaitingRoomUserNameChanged.Reset();
		onWaitingRoomEntranceEnabled.Reset();

		onAICompanionFeatureTurnOffByParticipant.Reset();
		onAICompanionFeatureSwitchRequested.Reset();
		onAICompanionFeatureSwitchRequestResponse.Reset();
		onAICompanionFeatureCanNotBeTurnedOff.Reset();

		onSmartSummaryStateNotSupported.Reset();
		onSmartSummaryStateSupportedButDisabled.Reset();
		onSmartSummaryStateEnabledButNotStarted.Reset();
		onSmartSummaryStateStarted.Reset();
		onFailedToStartSmartSummary.Reset();
		onSmartSummaryEnableRequestReceived.Reset();
		onSmartSummaryStartRequestReceived.Reset();
		onSmartSummaryEnableActionCallback.Reset();

		onQueryStateNotSupported.Reset();
		onQueryStateSupportedButDisabled.Reset();
		onQueryStateEnabledButNotStarted.Reset();
		onQueryStateStarted.Reset();
		onQuerySettingChanged.Reset();
		onFailedToStartQuery.Reset();
		onReceiveRequestToEnableQuery.Reset();
		onReceiveRequestToStartQuery.Reset();
		onReceiveQueryAnswer.Reset();
		onQueryEnableActionCallback.Reset();
		onSendQueryPrivilegeChanged.Reset();
		onFailedToRequestSendQuery.Reset();
		onReceiveRequestToSendQuery.Reset();

		onIndicatorItemReceived.Reset();
		onIndicatorItemRemoved.Reset();

		onPromptAttendee2PanelistResult.Reset();
		onDepromptPanelist2AttendeeResult.Reset();
		onAllowPanelistStartVideoNotification.Reset();
		onDisallowPanelistStartVideoNotification.Reset();
		onSelfAllowTalkNotification.Reset();
		onSelfDisallowTalkNotification.Reset();
		onAllowAttendeeChatNotification.Reset();
		onDisallowAttendeeChatNotification.Reset();
		onAllowWebinarReactionStatusChanged.Reset();
		onAllowAttendeeRaiseHandStatusChanged.Reset();
		onAllowAttendeeViewTheParticipantCountStatusChanged.Reset();
		onAttendeeAudioStatusNotification.Reset();
		onAttendeePromoteConfirmResult.Reset();

		onAddQuestion.Reset();
		onReceiveQuestion.Reset();
		onDeleteQuestions.Reset();
		onQuestionMarkedAsDismissed.Reset();
		onReopenQuestion.Reset();
		onAddAnswer.Reset();
		onReceiveAnswer.Reset();
		onDeleteAnswers.Reset();
		onUserLivingReply.Reset();
		onUserEndLiving.Reset();
		onVoteupQuestion.Reset();
		onRevokeVoteupQuestion.Reset();

		onEmojiReactionReceived.Reset();
		onEmojiReactionReceivedInWebinar.Reset();

		onLiveTranscriptionMsgInfoReceived.Reset();
		onOriginalLanguageMsgReceived.Reset();

		onPollingStatusChanged.Reset();
		onPollingResultUpdated.Reset();
		onPollingListUpdated.Reset();
		onPollingActionResult.Reset();

		onWhiteboardStatusChanged.Reset();
		onWhiteboardSettingsChanged.Reset();

		onDocsSharingSourceInfoChanged.Reset();
		onDocsPermissionChanged.Reset();
	}

	~ZoomNodeSinkHelper() {}
private:
	ZoomNodeSinkHelper() {}
};

///////////////////////////////////////////////////////////////////////////////

template<class T>
static bool SetProtoParam(const v8::FunctionCallbackInfo<v8::Value>& args, T& proto_param) {

	v8::Isolate* isolate_temp = args.GetIsolate();

	if (args.Length() < 1 ||
		!args[0]->IsUint8Array()
		)
	{
		return false;
	}
	if (!ZoomNodeAPIUtilHelper::GetInst().m_bInit)
	{
		if (!ZoomNodeAPIUtilHelper::GetInst().ExportNAPIUtilFunc())
		{
			return false;
		}
	}
	v8::Local<v8::Uint8Array> uint8array_temp_param = args[0].As<v8::Uint8Array>();

	size_t sz_temp_param = 0;
	char* char_temp_param = nullptr;
	fnGetRawBufferPtr pGetRawBufferPtr = NULL;
	pGetRawBufferPtr = ZoomNodeAPIUtilHelper::GetInst().m_fnGetRawBufferPtr;
	if (!pGetRawBufferPtr)
	{
		return false;
	}
	pGetRawBufferPtr(uint8array_temp_param, (void**)(&char_temp_param), sz_temp_param);
	if (!char_temp_param)
	{
		return false;
	}

	if (!proto_param.ParseFromArray(char_temp_param, sz_temp_param))
	{
		return false;
	}
	return true;
}
#endif // !_zoom_singleton_wrap_class_h_
