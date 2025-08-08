#ifndef _zoom_sinks_wrap_class_h_
#define _zoom_sinks_wrap_class_h_
#include "zoom_node_common_include.h"
#include "zoom_singleton_wrap_class.h"
#include "zoom_node_wrap_util.h"

class ZNativeSDKAuthWrapSink
{
public:
	/// \brief Authentication result callback.
	/// \param authResult Authentication result value.  For more details, see \link ZNAuthResult \endlink enum.
	virtual void onAuthenticationReturn(ZNAuthResult authResult)
	{
		if (ZoomNodeSinkHelper::GetInst().onAuthenticationReturn.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Integer> v8_authresult = v8::Integer::New(isolate, (int32_t)authResult);
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_authresult };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onAuthenticationReturn);

		fn->Call(context, global, argc, argv);
	}
	/// \brief Logout result callback.
	virtual void onLogout()
	{
		if (ZoomNodeSinkHelper::GetInst().onLogout.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onLogout);
		fn->Call(context, global, 0, NULL);
	}
	/// \brief Zoom identity has expired, please re-login or generate a new zoom access token.
	virtual void onZoomIdentityExpired()
	{
		if (ZoomNodeSinkHelper::GetInst().onZoomIdentityExpired.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onZoomIdentityExpired);
		fn->Call(context, global, 0, NULL);
	}
	virtual void onZoomAuthIdentityExpired()
	{
		if (ZoomNodeSinkHelper::GetInst().onZoomAuthIdentityExpired.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onZoomAuthIdentityExpired);
		fn->Call(context, global, 0, NULL);
	}
	virtual void onLoginReturnWithReason(ZNLOGINSTATUS ret, ZNLoginFailReason reason)
	{
		if (ZoomNodeSinkHelper::GetInst().onLoginReturnWithReason.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Integer> v8_loginstatus = v8::Integer::New(isolate, (int32_t)ret);
		v8::Local<v8::Integer> v8_loginfailreason = v8::Integer::New(isolate, (int32_t)reason);

		int argc = 2;
		v8::Local<v8::Value> argv[2] = { v8_loginstatus, v8_loginfailreason };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onLoginReturnWithReason);
		fn->Call(context, global, argc, argv);
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZNativeSDKMeetingWrapSink
{
public:
	/// \brief Meeting status changed callback.
	/// \param status The value of meeting. For more details, see \link ZNMeetingStatus \endlink.
	/// \param iResult Detailed reasons for special meeting status.
	///If the status is ZNMEETING_STATUS_FAILED, the value of iResult is one of those listed in \link ZNMeetingFailCode \endlink enum. 
	///If the status is ZNMEETING_STATUS_ENDED, the value of iResult is one of those listed in \link ZNMeetingEndReason \endlink enum.
	virtual void onMeetingStatusChanged(ZNMeetingStatus meetingStatus, int iResult)
	{
		if (ZoomNodeSinkHelper::GetInst().onMeetingStatusChanged.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Integer > v8_meeting_status = v8::Integer::New(isolate, (int32_t)meetingStatus);
		v8::Local<v8::Integer > v8_result = v8::Integer::New(isolate, (int32_t)iResult);
		int argc = 2;
		v8::Local<v8::Value> argv[2] = { v8_meeting_status, v8_result };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onMeetingStatusChanged);

		fn->Call(context, global, argc, argv);
	}
	virtual void onMeetingStatisticsWarningNotification(ZNStatisticsWarningType type)
	{
		if (ZoomNodeSinkHelper::GetInst().onMeetingStatisticsWarningNotification.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Integer > v8_type = v8::Integer::New(isolate, (int32_t)type);
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_type };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onMeetingStatisticsWarningNotification);

		fn->Call(context, global, argc, argv);
	}
	virtual void onMeetingParameterNotification(ZNMeetingParameter parameter)
	{
		if (ZoomNodeSinkHelper::GetInst().onMeetingParameterNotification.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();


		v8::Local<v8::Object> node = v8::Object::New(isolate);
		node->Set(context, v8::String::NewFromUtf8(isolate, "meetingType", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int32_t)parameter.meeting_type));
		node->Set(context, v8::String::NewFromUtf8(isolate, "isViewOnly", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, parameter.is_view_only));
		node->Set(context, v8::String::NewFromUtf8(isolate, "isAutoRecordingLocal", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, parameter.is_auto_recording_local));
		node->Set(context, v8::String::NewFromUtf8(isolate, "isAutoRecordingCloud", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, parameter.is_auto_recording_cloud));
		node->Set(context, v8::String::NewFromUtf8(isolate, "meetingNumber", v8::NewStringType::kInternalized).ToLocalChecked(), v8::BigInt::NewFromUnsigned(isolate, parameter.meeting_number));
		node->Set(context, v8::String::NewFromUtf8(isolate, "meetingTopic", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(parameter.meeting_topic).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "meetingHost", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(parameter.meeting_host).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());

		int argc = 1;
		v8::Local<v8::Value> argv[1] = { node };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onMeetingParameterNotification);

		fn->Call(context, global, argc, argv);
	}
	virtual void onMeetingTopicChanged(ZoomSTRING sTopic)
	{
		if (ZoomNodeSinkHelper::GetInst().onMeetingTopicChanged.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::String> v8_sTopic = v8::String::NewFromUtf8(isolate, zs2s(sTopic).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_sTopic };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onMeetingTopicChanged);

		fn->Call(context, global, argc, argv);
	}
	virtual void onMeetingFullToWatchLiveStream(ZoomSTRING sLiveStreamUrl)
	{
		if (ZoomNodeSinkHelper::GetInst().onMeetingFullToWatchLiveStream.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::String> v8_sLiveStreamUrl = v8::String::NewFromUtf8(isolate, zs2s(sLiveStreamUrl).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_sLiveStreamUrl };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onMeetingFullToWatchLiveStream);

		fn->Call(context, global, argc, argv);
	}
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZNativeSDKMeetingUICtrlWrapSink
{
public:
	/// \brief Callback event to click the INVITE button.
	/// \param [out] bHandled True indicates to show the user's own custom dialog interface. Default value: FALSE.
	/// \remarks If the value of bHandled is not set to TRUE, the default interface will pop up. 
	virtual void onInviteBtnClicked(bool& bHandled)
	{
		if (ZoomNodeSinkHelper::GetInst().onInviteBtnClicked.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Boolean > v8_bHandled = v8::Boolean::New(isolate, bHandled);

		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_bHandled };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onInviteBtnClicked);
		v8::MaybeLocal<v8::Value> bReturnedValue = fn->Call(context, global, argc, argv);
#ifdef USING_V8_NEW_STRING
		bHandled = bReturnedValue.ToLocalChecked()->BooleanValue(isolate);
#else
		bHandled = bReturnedValue.ToLocalChecked()->BooleanValue();
#endif 
	}

	virtual void onStartShareBtnClicked()
	{
		if (ZoomNodeSinkHelper::GetInst().onStartShareBtnClicked.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onStartShareBtnClicked);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onEndMeetingBtnClicked()
	{
		if (ZoomNodeSinkHelper::GetInst().onEndMeetingBtnClicked.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onEndMeetingBtnClicked);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onParticipantListBtnClicked()
	{
		if (ZoomNodeSinkHelper::GetInst().onParticipantListBtnClicked.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onParticipantListBtnClicked);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onCustomLiveStreamMenuClicked()
	{
		if (ZoomNodeSinkHelper::GetInst().onCustomLiveStreamMenuClicked.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onCustomLiveStreamMenuClicked);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onZoomInviteDialogFailed()
	{
		if (ZoomNodeSinkHelper::GetInst().onZoomInviteDialogFailed.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onZoomInviteDialogFailed);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onCCBTNClicked()
	{
		if (ZoomNodeSinkHelper::GetInst().onCCBTNClicked.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onCCBTNClicked);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onAudioBtnClicked(ZNAudioBtnClickedCallbackInfo info)
	{
		if (ZoomNodeSinkHelper::GetInst().onAudioBtnClicked.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Integer > v8_audio_clicked_action = v8::Integer::New(isolate, (int32_t)info.audio_clicked_action);
		v8::Local<v8::Integer > v8_userid_MuteUnmute = v8::Integer::New(isolate, (uint32_t)info.userid_MuteUnmute);

		int argc = 2;
		v8::Local<v8::Value> argv[2] = { v8_audio_clicked_action, v8_userid_MuteUnmute };

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onAudioBtnClicked);

		fn->Call(context, global, argc, argv);
	}
	virtual void onAudioMenuBtnClicked()
	{
		if (ZoomNodeSinkHelper::GetInst().onAudioMenuBtnClicked.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onAudioMenuBtnClicked);

		fn->Call(context, global, 0, NULL);
	}
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZNativeSDKMeetingAudioWrapSink
{
public:
	/// \brief User's audio status changed callback.
	/// \param lstAudioStatusChange List of the user information with audio status changed. The list will be emptied once the function calls end. 
	///For more user audio status infomation, see \link ZNAudioStatus \endlink enum.
	virtual void onUserAudioStatusChange(ZNList<ZNUserAudioStatus> lstAudioStatusChange, ZoomSTRING strAudioStatusList)
	{

		if (ZoomNodeSinkHelper::GetInst().onUserAudioStatusChange.IsEmpty())
		{
			return;
		}


		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Array> nodes = v8::Array::New(isolate);
		for (unsigned int i = 0; i < lstAudioStatusChange.size(); ++i) {
			v8::HandleScope scope(isolate);
			v8::Local<v8::Object> node = v8::Object::New(isolate);
			node->Set(context, v8::String::NewFromUtf8(isolate, "userid", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int32_t)lstAudioStatusChange[i].userId));
			node->Set(context, v8::String::NewFromUtf8(isolate, "audioStauts", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int32_t)lstAudioStatusChange[i].audioStauts));
			nodes->Set(context, i, node);
		}
		//
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { nodes };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onUserAudioStatusChange);

		fn->Call(context, global, argc, argv);
	}
	/// \brief The callback event that users whose audio is active changed.
	/// \param lstActiveAudio List to store the ID of user whose audio is active.
	virtual void onUserActiveAudioChange(ZNList<unsigned int > lstActiveAudio)
	{

		if (ZoomNodeSinkHelper::GetInst().onUserActiveAudioChange.IsEmpty())
		{
			return;
		}

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Array> nodes = v8::Array::New(isolate);
		for (unsigned int i = 0; i < lstActiveAudio.size(); ++i) {
			v8::HandleScope scope(isolate);
			v8::Local<v8::Object> node = v8::Object::New(isolate);
			node->Set(context, v8::String::NewFromUtf8(isolate, "userid", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int32_t)lstActiveAudio[i]));
			nodes->Set(context, i, node);
		}
		//
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { nodes };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onUserActiveAudioChange);

		fn->Call(context, global, argc, argv);
	}
	virtual void onMuteOnEntryStatusChange(bool bEnabled)
	{
		if (ZoomNodeSinkHelper::GetInst().onMuteOnEntryStatusChange.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Boolean> v8_bEnabled = v8::Boolean::New(isolate, bEnabled);
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_bEnabled };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onMuteOnEntryStatusChange);

		fn->Call(context, global, argc, argv);
	}
};



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ZNativeSDKMeetingVideoWrapSink
{
public:
	/// \brief Callback event of the user video status changes.
	/// \param userId The user ID whose video status changes
	/// \param status New video status. For more details, see \link ZNVideoStatus \endlink enum.
	virtual void onUserVideoStatusChange(unsigned int userId, ZNVideoStatus status)
	{
		if (ZoomNodeSinkHelper::GetInst().onUserVideoStatusChange.IsEmpty())
		{
			return;
		}

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Object> node = v8::Object::New(isolate);
		node->Set(context, v8::String::NewFromUtf8(isolate, "userId", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int32_t)userId));
		node->Set(context, v8::String::NewFromUtf8(isolate, "videoStatus", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int32_t)status));

		int argc = 1;
		v8::Local<v8::Value> argv[1] = { node };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onUserVideoStatusChange);

		fn->Call(context, global, argc, argv);
	}
	/// \brief Callback event of the active speaker video user changes. 
	/// \param userid The ID of user who becomes the new active speaker.
	virtual void onActiveSpeakerVideoUserChanged(unsigned int userId)
	{
		if (ZoomNodeSinkHelper::GetInst().onActiveSpeakerVideoUserChanged.IsEmpty())
		{
			return;
		}
		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();
		v8::Local<v8::Integer> zn_userid = v8::Integer::New(isolate, (int32_t)userId);

		int argc = 1;
		v8::Local<v8::Value> argv[1] = { zn_userid };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onActiveSpeakerVideoUserChanged);

		fn->Call(context, global, argc, argv);
	}
	/// \brief Callback event of the active video user changes. 
	/// \param userid The ID of user who becomes the new active speaker.
	virtual void onActiveVideoUserChanged(unsigned int userId)
	{
		if (ZoomNodeSinkHelper::GetInst().onActiveVideoUserChanged.IsEmpty())
		{
			return;
		}
		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();
		v8::Local<v8::Integer> zn_userid = v8::Integer::New(isolate, (int32_t)userId);

		int argc = 1;
		v8::Local<v8::Value> argv[1] = { zn_userid };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onActiveVideoUserChanged);

		fn->Call(context, global, argc, argv);
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZNativeSDKMeetingShareWrapSink
{
public:
	/// \brief Callback event of the changed sharing status. 
	/// \param status The values of sharing status. For more details, see \link ZNShareStatus \endlink enum.
	/// \param userId Sharer ID. 
	/// \remarks The userId changes according to the status value. When the status value is the ZN_Sharing_Self_Send_Begin or ZN_Sharing_Self_Send_End, the userId is the user own ID. Otherwise, the value of userId is the sharer ID.
	virtual void onSharingStatus(ZNZoomSDKSharingSourceInfo shareInfo)
	{
		if (ZoomNodeSinkHelper::GetInst().onSharingStatus.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Object> node = NodeWrapUtil::CreateSharingSourceInfoObject(isolate, shareInfo);

		int argc = 1;
		v8::Local<v8::Value> argv[1] = { node };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onSharingStatus);

		fn->Call(context, global, argc, argv);
	}
	/// \brief Callback event of failed to start sharing. 
	virtual void onFailedToStartShare()
	{
		if (ZoomNodeSinkHelper::GetInst().onFailedToStartShare.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onFailedToStartShare);

		fn->Call(context, global, 0, NULL);
	}
	/// \brief Callback event of the changed optimizing video status. 
	/// \param shareInfo Sharing information. For more details, see \link ZoomSDKSharingSourceInfo \endlink structure.
	virtual void onOptimizingShareForVideoClipStatusChanged(ZNZoomSDKSharingSourceInfo shareInfo)
	{
		if (ZoomNodeSinkHelper::GetInst().onOptimizingShareForVideoClipStatusChanged.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Object> node = NodeWrapUtil::CreateSharingSourceInfoObject(isolate, shareInfo);

		int argc = 1;
		v8::Local<v8::Value> argv[1] = { node };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onOptimizingShareForVideoClipStatusChanged);

		fn->Call(context, global, argc, argv);
	}
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZNativeSDKMeetingAnnotationWrapSink
{
public:
	virtual void onSupportAnnotationStatus(unsigned int nShareSourceID, bool bSupportAnnotation)
	{
		if (ZoomNodeSinkHelper::GetInst().onSupportAnnotationStatus.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Integer> zn_nShareSourceID = v8::Integer::New(isolate, (int32_t)nShareSourceID);
		v8::Local<v8::Boolean> zn_bSupportAnnotation = v8::Boolean::New(isolate, bSupportAnnotation);
		int argc = 2;
		v8::Local<v8::Value> argv[2] = { zn_nShareSourceID, zn_bSupportAnnotation };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onSupportAnnotationStatus);

		fn->Call(context, global, argc, argv);
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZNativeSDKMeetingParticipantsWrapSink
{
public:
	/// \brief Callback event of notification of users who are in the meeting.
	/// \param lstUserID List of the user ID. 
	/// \param strUserList List of user in json format. This function is currently invalid, hereby only for reservations.
	virtual void onUserJoin(ZNList<unsigned int> lstUserID, ZoomSTRING strUserList)
	{

		if (ZoomNodeSinkHelper::GetInst().onUserJoin.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Array> nodes = v8::Array::New(isolate);
		for (unsigned int i = 0; i < lstUserID.size(); ++i) {
			v8::HandleScope scope(isolate);
			v8::Local<v8::Object> node = v8::Object::New(isolate);
			node->Set(context, v8::String::NewFromUtf8(isolate, "userid", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int32_t)lstUserID[i]));
			nodes->Set(context, i, node);
		}
		//
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { nodes };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onUserJoin);

		fn->Call(context, global, argc, argv);
	}
	/// \brief Callback event of notification of user who leaves the meeting.
	/// \param lstUserID List of the user ID who leaves the meeting.
	/// \param strUserList List of the user in json format. This function is currently invalid, hereby only for reservations.
	virtual void onUserLeft(ZNList<unsigned int> lstUserID, ZoomSTRING strUserList)
	{

		if (ZoomNodeSinkHelper::GetInst().onUserLeft.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Array> nodes = v8::Array::New(isolate);
		for (unsigned int i = 0; i < lstUserID.size(); ++i) {
			v8::HandleScope scope(isolate);
			v8::Local<v8::Object> node = v8::Object::New(isolate);
			node->Set(context, v8::String::NewFromUtf8(isolate, "userid", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int32_t)lstUserID[i]));
			nodes->Set(context, i, node);
		}
		//
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { nodes };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onUserLeft);

		fn->Call(context, global, argc, argv);
	}
	/// \brief Callback event of notification of the new host. 
	/// \param userId Specify the ID of the new host.
	virtual void onHostChangeNotification(unsigned int userId)
	{

		if (ZoomNodeSinkHelper::GetInst().onHostChangeNotification.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Integer> v8_userid = v8::Integer::New(isolate, (int32_t)userId);
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_userid };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onHostChangeNotification);

		fn->Call(context, global, argc, argv);
	}
	/// \brief Callback event that lets participants request that the host starts cloud recording.
	/// \param bAllow True allow. If false, disallow.
	virtual void onAllowParticipantsRequestCloudRecording(bool bAllow)
	{
		if (ZoomNodeSinkHelper::GetInst().onAllowParticipantsRequestCloudRecording.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Boolean> v8_bAllow = v8::Boolean::New(isolate, bAllow);
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_bAllow };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onAllowParticipantsRequestCloudRecording);

		fn->Call(context, global, argc, argv);
	}
	/// \brief Callback event that the bot relationship changed in the meeting.
	/// \param authorizeUserID Specify the authorizer user ID.
	virtual void onBotAuthorizerRelationChanged(unsigned int authorizeUserID)
	{

		if (ZoomNodeSinkHelper::GetInst().onBotAuthorizerRelationChanged.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Integer> v8_authorizeUserID = v8::Integer::New(isolate, (int32_t)authorizeUserID);
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_authorizeUserID };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onBotAuthorizerRelationChanged);

		fn->Call(context, global, argc, argv);
	}
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZNativeSDKMeetingH323WrapSink
{
public:
	/// \brief Callback event when the calling status of H.323 device changes.
	/// \param status H.323 device calling out status value. For more details, see \link ZNH323CalloutStatus \endlink enum.
	virtual void onH323CalloutStatusNotify(ZNH323CalloutStatus status)
	{
		if (ZoomNodeSinkHelper::GetInst().onH323CalloutStatusNotify.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Integer> v8_h323CalloutStatus = v8::Integer::New(isolate, (int32_t)status);
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_h323CalloutStatus };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onH323CalloutStatusNotify);

		fn->Call(context, global, argc, argv);
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZNativeSDKMeetingConfigWrapFreemeetingSink
{
public:
	/// \brief The callback of upgrading the free meeting.
	/// \param type Type of upgrading the free meeting, see \link ZNFreeMeetingNeedUpgradeType \endlink enum.
	/// \param gift_url Upgrade the free meeting by the gift link. When and only when the value of type is ZN_FreeMeetingNeedUpgradeType_BY_GIFTURL, this parameter is meaningful.
	virtual void onFreeMeetingNeedToUpgrade(ZNFreeMeetingNeedUpgradeType type, ZoomSTRING gift_url)
	{
		if (ZoomNodeSinkHelper::GetInst().onFreeMeetingNeedToUpgrade.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Object> node = v8::Object::New(isolate);
		node->Set(context, v8::String::NewFromUtf8(isolate, "giftUrl", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(gift_url).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "freeMeetingNeedUpgradeType", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int32_t)type));

		int argc = 1;
		v8::Local<v8::Value> argv[1] = { node };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onFreeMeetingNeedToUpgrade);

		fn->Call(context, global, argc, argv);
	}
	/// \brief Callback function of starting to upgrade the free meeting by the gift link.
	virtual void onFreeMeetingUpgradeToGiftFreeTrialStart()
	{
		if (ZoomNodeSinkHelper::GetInst().onFreeMeetingUpgradeToGiftFreeTrialStart.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onFreeMeetingUpgradeToGiftFreeTrialStart);

		fn->Call(context, global, 0, NULL);
	}
	/// \brief Callback function of ending upgrade the free meeting by the gift link.
	virtual void onFreeMeetingUpgradeToGiftFreeTrialStop()
	{
		if (ZoomNodeSinkHelper::GetInst().onFreeMeetingUpgradeToGiftFreeTrialStop.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onFreeMeetingUpgradeToGiftFreeTrialStop);

		fn->Call(context, global, 0, NULL);
	}
	/// \brief Callback function of free meting upgrades successfully.
	virtual void onFreeMeetingUpgradeToProMeeting()
	{
		if (ZoomNodeSinkHelper::GetInst().onFreeMeetingUpgradeToProMeeting.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onFreeMeetingUpgradeToProMeeting);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onInputMeetingPasswordAndScreenNameNotification()
	{
		if (ZoomNodeSinkHelper::GetInst().onInputMeetingPasswordAndScreenNameNotification.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onInputMeetingPasswordAndScreenNameNotification);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onAirPlayInstructionWndNotification(bool bShow, ZoomSTRING airhostName)
	{
		if (ZoomNodeSinkHelper::GetInst().onAirPlayInstructionWndNotification.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Boolean> zn_bShow = v8::Boolean::New(isolate, bShow);
		v8::Local<v8::String> zn_airhostName = v8::String::NewFromUtf8(isolate, zs2s(airhostName).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();

		int argc = 2;
		v8::Local<v8::Value> argv[2] = { zn_bShow, zn_airhostName };

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onAirPlayInstructionWndNotification);

		fn->Call(context, global, argc, argv);
	}
	
	virtual void onWebinarNeedRegisterNotification()
	{
		if (ZoomNodeSinkHelper::GetInst().onWebinarNeedRegisterNotification.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onWebinarNeedRegisterNotification);

		fn->Call(context, global, 0, NULL);
	}
	
	virtual void onEndOtherMeetingToJoinMeetingNotification()
	{
		if (ZoomNodeSinkHelper::GetInst().onEndOtherMeetingToJoinMeetingNotification.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onEndOtherMeetingToJoinMeetingNotification);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onFreeMeetingRemainTime(unsigned int leftTime)
	{
		if (ZoomNodeSinkHelper::GetInst().onFreeMeetingRemainTime.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Integer> v8_leftTime = v8::Integer::New(isolate, (int32_t)leftTime);
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_leftTime };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onFreeMeetingRemainTime);

		fn->Call(context, global, argc, argv);
	}
	virtual void onFreeMeetingRemainTimeStopCountDown()
	{
		if (ZoomNodeSinkHelper::GetInst().onFreeMeetingRemainTimeStopCountDown.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onFreeMeetingRemainTimeStopCountDown);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onWebinarNeedInputScreenName()
	{
		if (ZoomNodeSinkHelper::GetInst().onWebinarNeedInputScreenName.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onWebinarNeedInputScreenName);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onJoinMeetingNeedUserInfo()
	{
		if (ZoomNodeSinkHelper::GetInst().onJoinMeetingNeedUserInfo.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onJoinMeetingNeedUserInfo);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onUserConfirmToStartArchive()
	{
		if (ZoomNodeSinkHelper::GetInst().onUserConfirmToStartArchive.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onUserConfirmToStartArchive);

		fn->Call(context, global, 0, NULL);
	}
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZNativeSDKDirectShareHelperWrapSink
{
public:
	/// \brief The callback event will be triggered if the status of direct sharing changes.
	/// \param status Specifies the status of direct sharing. For more details, see \link ZNDirectShareStatus \endlink enum.
	///When the value of status is ZN_DirectShare_Need_MeetingID_Or_PairingCode or ZN_DirectShare_WrongMeetingID_Or_SharingKey, 
	///the SDK user must set the value of the _paring_code or _meeting_number via the functions TryWithPairingCode or TryWithMeetingNumber to start direct sharing.
	virtual void OnDirectShareStatusUpdate(ZNDirectShareStatus status)
	{
		if (ZoomNodeSinkHelper::GetInst().OnDirectShareStatusUpdate.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();


		v8::Local<v8::Object> node = v8::Object::New(isolate);
		node->Set(context, v8::String::NewFromUtf8(isolate, "DirectShareStatus", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int32_t)status));

		int argc = 1;
		v8::Local<v8::Value> argv[1] = { node };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().OnDirectShareStatusUpdate);

		fn->Call(context, global, argc, argv);


	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZNativeSDKSMSHelperWrapSink
{
public:
	virtual void onNeedRealNameAuthMeetingNotification(ZNList<ZNZoomRealNameAuthCountryInfo> support_country_list, ZoomSTRING privacy_url)
	{
		if (ZoomNodeSinkHelper::GetInst().onNeedRealNameAuthMeetingNotification.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Array> zn_support_country_list = v8::Array::New(isolate);
		for (unsigned int i = 0; i < support_country_list.size(); ++i)
		{
			v8::HandleScope scope(isolate);
			v8::Local<v8::Object> node = v8::Object::New(isolate);
			node->Set(context, v8::String::NewFromUtf8(isolate, "countryCode", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(support_country_list[i].countryCode).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
			node->Set(context, v8::String::NewFromUtf8(isolate, "countryID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(support_country_list[i].countryID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
			node->Set(context, v8::String::NewFromUtf8(isolate, "countryName", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(support_country_list[i].countryName).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
			zn_support_country_list->Set(context, i, node);
		}
		
		v8::Local<v8::String> zn_privacy_url = v8::String::NewFromUtf8(isolate, zs2s(privacy_url).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();

		int argc = 2;
		v8::Local<v8::Value> argv[2] = { zn_support_country_list, zn_privacy_url };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onNeedRealNameAuthMeetingNotification);

		fn->Call(context, global, argc, argv);
	}
	virtual void onRetrieveSMSVerificationCodeResultNotification(ZNSMSVerificationCodeErr result)
	{
		if (ZoomNodeSinkHelper::GetInst().onRetrieveSMSVerificationCodeResultNotification.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Integer> zn_smsVerificationCodeErr = v8::Integer::New(isolate, (int32_t)result);

		int argc = 1;
		v8::Local<v8::Value> argv[1] = { zn_smsVerificationCodeErr };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onRetrieveSMSVerificationCodeResultNotification);

		fn->Call(context, global, argc, argv);
	}
	virtual void onVerifySMSVerificationCodeResultNotification(ZNSMSVerificationCodeErr result)
	{
		if (ZoomNodeSinkHelper::GetInst().onVerifySMSVerificationCodeResultNotification.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Integer> zn_smsVerificationCodeErr = v8::Integer::New(isolate, (int32_t)result);

		int argc = 1;
		v8::Local<v8::Value> argv[1] = { zn_smsVerificationCodeErr };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onVerifySMSVerificationCodeResultNotification);

		fn->Call(context, global, argc, argv);
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZNativeSDKAudioSettingContextWrapSink
{
public:
	/// \brief Callback event if the SDK detects that the computer mic devices have been changed.
	/// \param newMicList The new list of all mic devices plugged into the computer.
	virtual void onComputerMicDeviceChanged(ZNList<ZNMicInfo> newMicList)
	{
		if (ZoomNodeSinkHelper::GetInst().onComputerMicDeviceChanged.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Array> nodes = v8::Array::New(isolate);
		for (unsigned int i = 0; i < newMicList.size(); ++i) {
			v8::HandleScope scope(isolate);
			v8::Local<v8::Object> node = v8::Object::New(isolate);
			node->Set(context, v8::String::NewFromUtf8(isolate, "deviceId", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(newMicList[i].deviceId).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
			node->Set(context, v8::String::NewFromUtf8(isolate, "deviceName", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(newMicList[i].deviceName).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
			node->Set(context, v8::String::NewFromUtf8(isolate, "isSelected", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, newMicList[i].isSelectedDevice));
			nodes->Set(context, i, node);
		}

		int argc = 1;
		v8::Local<v8::Value> argv[1] = { nodes };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onComputerMicDeviceChanged);

		fn->Call(context, global, argc, argv);
	}
	/// \brief Callback event if the SDK detects that the computer speaker devices have been changed.
	/// \param newSpeakerList The new list of all speaker devices plugged into the computer. 
	virtual void onComputerSpeakerDeviceChanged(ZNList<ZNSpeakerInfo> newSpeakerList)
	{
		if (ZoomNodeSinkHelper::GetInst().onComputerSpeakerDeviceChanged.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Array> nodes = v8::Array::New(isolate);
		for (unsigned int i = 0; i < newSpeakerList.size(); ++i) {
			v8::HandleScope scope(isolate);
			v8::Local<v8::Object> node = v8::Object::New(isolate);
			node->Set(context, v8::String::NewFromUtf8(isolate, "deviceId", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(newSpeakerList[i].deviceId).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
			node->Set(context, v8::String::NewFromUtf8(isolate, "deviceName", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(newSpeakerList[i].deviceName).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
			node->Set(context, v8::String::NewFromUtf8(isolate, "isSelected", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, newSpeakerList[i].isSelectedDevice));
			nodes->Set(context, i, node);
		}

		int argc = 1;
		v8::Local<v8::Value> argv[1] = { nodes };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onComputerSpeakerDeviceChanged);

		fn->Call(context, global, argc, argv);
	}
	/// \brief Notify the user that a microphone device is selected.
	/// \param deviceId Specify a device to be selected.
	/// \param deviceName Specify the device name assigned by deviceId.
	virtual void onDefaultMicDeviceChanged(ZoomSTRING deviceId, ZoomSTRING deviceName)
	{
		if (ZoomNodeSinkHelper::GetInst().onDefaultMicDeviceChanged.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::String> zn_deviceId = v8::String::NewFromUtf8(isolate, zs2s(deviceId).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
		v8::Local<v8::String> zn_deviceName = v8::String::NewFromUtf8(isolate, zs2s(deviceName).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();

		int argc = 2;
		v8::Local<v8::Value> argv[2] = { zn_deviceId, zn_deviceName };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onDefaultMicDeviceChanged);

		fn->Call(context, global, argc, argv);
	}
	/// \brief Notify the user that a speaker device is selected.
	/// \param deviceId Specify a device to be selected.
	/// \param deviceName Specify the device name assigned by deviceId.
	virtual void onDefaultSpeakerDeviceChanged(ZoomSTRING deviceId, ZoomSTRING deviceName)
	{
		if (ZoomNodeSinkHelper::GetInst().onDefaultSpeakerDeviceChanged.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::String> zn_deviceId = v8::String::NewFromUtf8(isolate, zs2s(deviceId).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
		v8::Local<v8::String> zn_deviceName = v8::String::NewFromUtf8(isolate, zs2s(deviceName).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();

		int argc = 2;
		v8::Local<v8::Value> argv[2] = { zn_deviceId, zn_deviceName };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onDefaultSpeakerDeviceChanged);

		fn->Call(context, global, argc, argv);
	}

};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZNativeSDKVideoSettingContextWrapSink
{
public:
	virtual void onComputerCamDeviceChanged(ZNList<ZNCameraInfo> newCameraList)
	{
		if (ZoomNodeSinkHelper::GetInst().onComputerCamDeviceChanged.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Array> nodes = v8::Array::New(isolate);
		for (unsigned int i = 0; i < newCameraList.size(); ++i) {
			v8::HandleScope scope(isolate);
			v8::Local<v8::Object> node = v8::Object::New(isolate);
			node->Set(context, v8::String::NewFromUtf8(isolate, "deviceId", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(newCameraList[i].deviceId).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
			node->Set(context, v8::String::NewFromUtf8(isolate, "deviceName", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(newCameraList[i].deviceName).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
			node->Set(context, v8::String::NewFromUtf8(isolate, "isSelected", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, newCameraList[i].isSelectedDevice));
			nodes->Set(context, i, node);
		}

		int argc = 1;
		v8::Local<v8::Value> argv[1] = { nodes };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onComputerCamDeviceChanged);

		fn->Call(context, global, argc, argv);
	}
	virtual void onDefaultCamDeviceChanged(ZoomSTRING deviceId, ZoomSTRING deviceName)
	{
		if (ZoomNodeSinkHelper::GetInst().onDefaultCamDeviceChanged.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::String> zn_deviceId = v8::String::NewFromUtf8(isolate, zs2s(deviceId).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
		v8::Local<v8::String> zn_deviceName = v8::String::NewFromUtf8(isolate, zs2s(deviceName).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();

		int argc = 2;
		v8::Local<v8::Value> argv[2] = { zn_deviceId, zn_deviceName };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onDefaultCamDeviceChanged);

		fn->Call(context, global, argc, argv);
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZNativeSDKRecordingSettingContextWrapSink
{
public:
	void onCloudRecordingStorageInfo(signed long long storage_total_size, signed long long storage_used_size, bool allow_exceed_storage)
	{
		if (ZoomNodeSinkHelper::GetInst().onCloudRecordingStorageInfo.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Integer> zn_storageTotalSize = v8::Integer::New(isolate, (int64_t)storage_total_size);
		v8::Local<v8::Integer> zn_storageUsedSize = v8::Integer::New(isolate, (int64_t)storage_used_size);
		v8::Local<v8::Boolean> zn_allowExceedStorage = v8::Boolean::New(isolate, allow_exceed_storage);


		int argc = 3;
		v8::Local<v8::Value> argv[3] = { zn_storageTotalSize, zn_storageUsedSize, zn_allowExceedStorage };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onCloudRecordingStorageInfo);

		fn->Call(context, global, argc, argv);
	}

};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZNativeSDKWallpaperSettingContextWrapSink
{
public:
	virtual void onMeetingWallpaperChanged(ZNWallpaperItem item)
	{
		if (ZoomNodeSinkHelper::GetInst().onMeetingWallpaperChanged.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Object> node = NodeWrapUtil::CreateWallpaperItemObject(isolate, item);

		int argc = 1;
		v8::Local<v8::Value> argv[1] = { node };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onMeetingWallpaperChanged);

		fn->Call(context, global, argc, argv);
	}
	void onMeetingWallpaperImageDownloadStatus(ZNZoomSDKWallpaperSettingStatus status, ZoomSTRING wallpaperId)
	{
		if (ZoomNodeSinkHelper::GetInst().onMeetingWallpaperImageDownloadStatus.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Integer> v8_status = v8::Integer::New(isolate, (int32_t)status);
		v8::Local<v8::String> v8_wallpaperId = v8::String::NewFromUtf8(isolate, zs2s(wallpaperId).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();

		int argc = 2;
		v8::Local<v8::Value> argv[2] = { v8_status, v8_wallpaperId };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onMeetingWallpaperImageDownloadStatus);

		fn->Call(context, global, argc, argv);
	}
	virtual void onPersonalWallpaperChanged(ZNWallpaperItem item)
	{
		if (ZoomNodeSinkHelper::GetInst().onPersonalWallpaperChanged.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Object> node = NodeWrapUtil::CreateWallpaperItemObject(isolate, item);

		int argc = 1;
		v8::Local<v8::Value> argv[1] = { node };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onPersonalWallpaperChanged);

		fn->Call(context, global, argc, argv);
	}
	void onPersonalWallpaperImageDownloadStatus(ZNZoomSDKWallpaperSettingStatus status, ZoomSTRING wallpaperId)
	{
		if (ZoomNodeSinkHelper::GetInst().onPersonalWallpaperImageDownloadStatus.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Integer> v8_status = v8::Integer::New(isolate, (int32_t)status);
		v8::Local<v8::String> v8_wallpaperId = v8::String::NewFromUtf8(isolate, zs2s(wallpaperId).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();

		int argc = 2;
		v8::Local<v8::Value> argv[2] = { v8_status, v8_wallpaperId };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onPersonalWallpaperImageDownloadStatus);

		fn->Call(context, global, argc, argv);
	}
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZNativeSDKVirtualBGSettingContextWrapSink
{
public:
	virtual void onVBImageDidDownloaded()
	{
		if (ZoomNodeSinkHelper::GetInst().onVBImageDidDownloaded.IsEmpty())
		{
			return;
		}

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onVBImageDidDownloaded);
		fn->Call(context, global, 0, NULL);
	}
	virtual void onGreenVBDidUpdateWithReplaceColor(ZNZoomSDKColor selectedColor)
	{
		if (ZoomNodeSinkHelper::GetInst().onGreenVBDidUpdateWithReplaceColor.IsEmpty())
		{
			return;
		}

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Object> node = v8::Object::New(isolate);
		node->Set(context, v8::String::NewFromUtf8(isolate, "red", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, selectedColor.red));
		node->Set(context, v8::String::NewFromUtf8(isolate, "green", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, selectedColor.green));
		node->Set(context, v8::String::NewFromUtf8(isolate, "blue", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, selectedColor.blue));

		int argc = 1;
		v8::Local<v8::Value> argv[1] = { node };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onGreenVBDidUpdateWithReplaceColor);
		fn->Call(context, global, argc, argv);
	}
	virtual void onSelectedVBImageChanged()
	{
		if (ZoomNodeSinkHelper::GetInst().onSelectedVBImageChanged.IsEmpty())
		{
			return;
		}

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onSelectedVBImageChanged);
		fn->Call(context, global, 0, NULL);
	}
	virtual void onVBVideoUploadedResult(bool success, ZNVBVideoError error)
	{
		if (ZoomNodeSinkHelper::GetInst().onVBVideoUploadedResult.IsEmpty())
		{
			return;
		}

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();
		
		v8::Local<v8::Boolean> v8_success = v8::Boolean::New(isolate, success);
		v8::Local<v8::Integer> v8_error = v8::Integer::NewFromUnsigned(isolate, error);
		int argc = 2;
		v8::Local<v8::Value> argv[2] = { v8_success, v8_error };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onVBVideoUploadedResult);
		fn->Call(context, global, argc, argv);
	}
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZNativeSDKMeetingRecordingWrapSink
{
public:

	virtual void onRecording2MP4Done(bool bsuccess, int iResult, ZoomSTRING szPath)
	{
		if (ZoomNodeSinkHelper::GetInst().onRecording2MP4Done.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Boolean> v8_bsuccess = v8::Boolean::New(isolate, bsuccess);
		v8::Local<v8::Integer > v8_result = v8::Integer::New(isolate, (int32_t)iResult);
		v8::Local<v8::String> v8_szPath = v8::String::NewFromUtf8(isolate, zs2s(szPath).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();

		int argc = 3;
		v8::Local<v8::Value> argv[3] = { v8_bsuccess, v8_result, v8_szPath };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onRecording2MP4Done);

		fn->Call(context, global, argc, argv);
	}

	virtual void onRecording2MP4Processing(int iPercentage)
	{
		if (ZoomNodeSinkHelper::GetInst().onRecording2MP4Processing.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Integer > v8_iPercentage = v8::Integer::New(isolate, (int32_t)iPercentage);

		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_iPercentage };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onRecording2MP4Processing);

		fn->Call(context, global, argc, argv);
	}

	virtual void onRecordingStatus(ZNRecordingStatus status)
	{
		if (ZoomNodeSinkHelper::GetInst().onRecordingStatus.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Integer> v8_recordingStatus = v8::Integer::New(isolate, (int32_t)status);
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_recordingStatus };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onRecordingStatus);

		fn->Call(context, global, argc, argv);
	}
	virtual void onCloudRecordingStatus(ZNRecordingStatus status)
	{
		if (ZoomNodeSinkHelper::GetInst().onCloudRecordingStatus.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Integer> v8_recordingStatus = v8::Integer::New(isolate, (int32_t)status);
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_recordingStatus };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onCloudRecordingStatus);

		fn->Call(context, global, argc, argv);
	}

	virtual void onRecordPrivilegeChanged(bool bCanRec)
	{
		if (ZoomNodeSinkHelper::GetInst().onRecordPrivilegeChanged.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Boolean> v8_bCanRec = v8::Boolean::New(isolate, bCanRec);
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_bCanRec };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onRecordPrivilegeChanged);

		fn->Call(context, global, argc, argv);
	}

	virtual void onCloudRecordingStorageFull(time_t gracePeriodDate)
	{
		if (ZoomNodeSinkHelper::GetInst().onCloudRecordingStorageFull.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Integer> v8_gracePeriodDate = v8::Integer::New(isolate, (int64_t)gracePeriodDate);
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_gracePeriodDate };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onCloudRecordingStorageFull);

		fn->Call(context, global, argc, argv);
	}
	/// \brief Callback event for when the host responds to a cloud recording permission request.
	/// \param status Value of request host to start cloud recording response status. For more details, see \link RequestStartCloudRecordingStatus \endlink enum.
	virtual void onRequestCloudRecordingResponse(ZNRequestStartCloudRecordingStatus status)
	{
		if (ZoomNodeSinkHelper::GetInst().onRequestCloudRecordingResponse.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Integer> v8_status = v8::Integer::New(isolate, (int32_t)status);
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_status };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onRequestCloudRecordingResponse);

		fn->Call(context, global, argc, argv);
	}
	/// \brief Callback event received only by the host when a user requests to start cloud recording.
	/// \param handler A pointer to the IRequestStartCloudRecordingHandler. For more details, see \link IRequestStartCloudRecordingHandler \endlink.
	virtual void onStartCloudRecordingRequested()
	{
		if (ZoomNodeSinkHelper::GetInst().onStartCloudRecordingRequested.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onStartCloudRecordingRequested);

		fn->Call(context, global, 0, NULL);
	}
	/// \brief Callback event received only by the host when a user requests to enable and start smart cloud recording.
	/// \param handler A pointer to the IRequestEnableAndStartSmartRecordingHandler. For more details, see \link IRequestEnableAndStartSmartRecordingHandler \endlink.
	virtual void onEnableAndStartSmartRecordingRequested()
	{
		if (ZoomNodeSinkHelper::GetInst().onEnableAndStartSmartRecordingRequested.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onEnableAndStartSmartRecordingRequested);

		fn->Call(context, global, 0, NULL);
	}
	/// \brief Callback event received when you call \link EnableSmartRecording \endlink. You can use the handler to confirm or cancel to enable the smart recording.
	/// \param handler A pointer to the ISmartRecordingEnableActionHandler. For more details, see \link ISmartRecordingEnableActionHandler \endlink.
	virtual void onSmartRecordingEnableActionCallback()
	{
		if (ZoomNodeSinkHelper::GetInst().onSmartRecordingEnableActionCallback.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onSmartRecordingEnableActionCallback);

		fn->Call(context, global, 0, NULL);
	}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZNativeSDKMeetingLiveStreamWrapSink
{
public:
	virtual void onLiveStreamStatusChange(ZNLiveStreamStatus status)
	{

		if (ZoomNodeSinkHelper::GetInst().onLiveStreamStatusChange.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Integer> v8_status = v8::Integer::New(isolate, (int32_t)status);
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_status };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onLiveStreamStatusChange);

		fn->Call(context, global, argc, argv);
	}
	virtual void onRawLiveStreamPrivilegeChanged(bool bHasPrivilege)
	{
		if (ZoomNodeSinkHelper::GetInst().onRawLiveStreamPrivilegeChanged.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Boolean> v8_bHasPrivilege = v8::Boolean::New(isolate, bHasPrivilege);
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_bHasPrivilege };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onRawLiveStreamPrivilegeChanged);

		fn->Call(context, global, argc, argv);
	}
	virtual void onUserRawLiveStreamPrivilegeChanged(unsigned int userid, bool bHasPrivilege)
	{
		if (ZoomNodeSinkHelper::GetInst().onUserRawLiveStreamPrivilegeChanged.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Boolean> v8_bHasPrivilege = v8::Boolean::New(isolate, bHasPrivilege);
		v8::Local<v8::Integer > v8_userid = v8::Integer::NewFromUnsigned(isolate, userid);

		int argc = 2;
		v8::Local<v8::Value> argv[3] = { v8_bHasPrivilege, v8_userid };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onUserRawLiveStreamPrivilegeChanged);

		fn->Call(context, global, argc, argv);
	}
	virtual void onRawLiveStreamPrivilegeRequested()
	{
		if (ZoomNodeSinkHelper::GetInst().onRawLiveStreamPrivilegeRequested.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onRawLiveStreamPrivilegeRequested);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onUserRawLiveStreamingStatusChanged(ZNList<ZNRawLiveStreamInfo> LivingList)
	{
		if (ZoomNodeSinkHelper::GetInst().onUserRawLiveStreamingStatusChanged.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Array> nodes = v8::Array::New(isolate);
		for (unsigned int i = 0; i < LivingList.size(); ++i) {
			v8::HandleScope scope(isolate);
			v8::Local<v8::Object> node = v8::Object::New(isolate);
			node->Set(context, v8::String::NewFromUtf8(isolate, "broadcastUrl", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(LivingList[i].broadcastUrl).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
			node->Set(context, v8::String::NewFromUtf8(isolate, "userId", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, LivingList[i].userId));
			nodes->Set(context, i, node);
		}

		int argc = 1;
		v8::Local<v8::Value> argv[1] = { nodes };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onUserRawLiveStreamingStatusChanged);

		fn->Call(context, global, argc, argv);
	}
	virtual void onRawLiveStreamPrivilegeRequestTimeout()
	{
		if (ZoomNodeSinkHelper::GetInst().onRawLiveStreamPrivilegeRequestTimeout.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onRawLiveStreamPrivilegeRequestTimeout);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onLiveStreamReminderStatusChanged(bool enable)
	{
		if (ZoomNodeSinkHelper::GetInst().onLiveStreamReminderStatusChanged.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Boolean> v8_enable = v8::Boolean::New(isolate, enable);
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_enable };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onLiveStreamReminderStatusChanged);

		fn->Call(context, global, argc, argv);
	}
	virtual void onLiveStreamReminderStatusChangeFailed()
	{
		if (ZoomNodeSinkHelper::GetInst().onLiveStreamReminderStatusChangeFailed.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onLiveStreamReminderStatusChangeFailed);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onUserThresholdReachedForLiveStream(int percent)
	{
		if (ZoomNodeSinkHelper::GetInst().onUserThresholdReachedForLiveStream.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Integer > v8_percent = v8::Integer::New(isolate, (int32_t)percent);

		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_percent };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onUserThresholdReachedForLiveStream);

		fn->Call(context, global, argc, argv);
	}
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZNativeSDKMeetingReminderWrapSink
{
public:
	/// \brief Callback event of the reminder dialog show.
	/// \param content The detail content in the reminder dialog. For more details, see \link IMeetingReminderContent \endlink enum.
	virtual void onReminderNotify(ZNMeetingReminderContent reminderContent)
	{
		if (ZoomNodeSinkHelper::GetInst().onReminderNotify.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();


		v8::Local<v8::Object> node = v8::Object::New(isolate);
		node->Set(context, v8::String::NewFromUtf8(isolate, "type", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int32_t)reminderContent.type));

		node->Set(context, v8::String::NewFromUtf8(isolate, "title", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(reminderContent.title).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "content", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(reminderContent.content).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "is_blocking", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, reminderContent.is_blocking));

		int argc = 1;
		v8::Local<v8::Value> argv[1] = { node };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onReminderNotify);

		fn->Call(context, global, argc, argv);
	}
	/// \brief Callback event of the enable reminder dialog show.
	/// \param content The detail content in the reminder dialog. For more details, see \link IMeetingEnableReminderHandler \endlink enum.
	/// \param handle A pointer to the IMeetingReminderHandler. For more details, see \link IMeetingEnableReminderHandler \endlink.
	virtual void onEnableReminderNotify(ZNMeetingReminderContent reminderContent)
	{
		if (ZoomNodeSinkHelper::GetInst().onEnableReminderNotify.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();


		v8::Local<v8::Object> node = v8::Object::New(isolate);
		node->Set(context, v8::String::NewFromUtf8(isolate, "type", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, (int32_t)reminderContent.type));

		node->Set(context, v8::String::NewFromUtf8(isolate, "title", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(reminderContent.title).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "content", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(reminderContent.content).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "is_blocking", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, reminderContent.is_blocking));

		int argc = 1;
		v8::Local<v8::Value> argv[1] = { node };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onEnableReminderNotify);

		fn->Call(context, global, argc, argv);
	}
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZNativeSDKMeetingChatWrapSink
{
public:
	virtual void onChatMsgNotification(ZNChatMsgInfo chatMsg, ZoomSTRING content)
	{
		if (ZoomNodeSinkHelper::GetInst().onChatMsgNotification.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Object> node = NodeWrapUtil::CreateChatMsgInfoObject(isolate, chatMsg);
		
		v8::Local<v8::String> v8_content = v8::String::NewFromUtf8(isolate, zs2s(content).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();

		int argc = 2;
		v8::Local<v8::Value> argv[2] = { node, v8_content };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onChatMsgNotification);

		fn->Call(context, global, argc, argv);
	}
	virtual void onChatStatusChangedNotification(ZNChatStatus zn_chatStatus)
	{
		if (ZoomNodeSinkHelper::GetInst().onChatStatusChangedNotification.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Object> node = v8::Object::New(isolate);
		node->Set(context, v8::String::NewFromUtf8(isolate, "is_chat_off", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_chatStatus.is_chat_off));
		node->Set(context, v8::String::NewFromUtf8(isolate, "is_webinar_attendee", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_chatStatus.is_webinar_attendee));
		node->Set(context, v8::String::NewFromUtf8(isolate, "is_webinar_meeting", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_chatStatus.is_webinar_meeting));
		if (zn_chatStatus.is_webinar_meeting) {
			if (zn_chatStatus.is_webinar_attendee) {
				node->Set(context, v8::String::NewFromUtf8(isolate, "can_chat", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_chatStatus.ut.webinar_attendee_status.can_chat));
				node->Set(context, v8::String::NewFromUtf8(isolate, "can_chat_to_all_panellist_and_attendee", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_chatStatus.ut.webinar_attendee_status.can_chat_to_all_panellist_and_attendee));
				node->Set(context, v8::String::NewFromUtf8(isolate, "can_chat_to_all_panellist", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_chatStatus.ut.webinar_attendee_status.can_chat_to_all_panellist));
			} else {
				node->Set(context, v8::String::NewFromUtf8(isolate, "can_chat_to_all_panellist", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_chatStatus.ut.webinar_other_status.can_chat_to_all_panellist));
				node->Set(context, v8::String::NewFromUtf8(isolate, "can_chat_to_all_panellist_and_attendee", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_chatStatus.ut.webinar_other_status.can_chat_to_all_panellist_and_attendee));
				node->Set(context, v8::String::NewFromUtf8(isolate, "can_chat_to_individual", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_chatStatus.ut.webinar_other_status.can_chat_to_individual));
			}
		} else {
			node->Set(context, v8::String::NewFromUtf8(isolate, "can_chat", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_chatStatus.ut.normal_meeting_status.can_chat));
			node->Set(context, v8::String::NewFromUtf8(isolate, "can_chat_to_all", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_chatStatus.ut.normal_meeting_status.can_chat_to_all));
			node->Set(context, v8::String::NewFromUtf8(isolate, "can_chat_to_individual", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_chatStatus.ut.normal_meeting_status.can_chat_to_individual));
			node->Set(context, v8::String::NewFromUtf8(isolate, "is_only_can_chat_to_host", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, zn_chatStatus.ut.normal_meeting_status.is_only_can_chat_to_host));
		}

		int argc = 1;
		v8::Local<v8::Value> argv[1] = { node };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onChatStatusChangedNotification);

		fn->Call(context, global, argc, argv);
	}
	virtual void onChatMsgDeleteNotification(ZoomSTRING msgID, ZNSDKChatMessageDeleteType deleteBy)
	{
		if (ZoomNodeSinkHelper::GetInst().onChatMsgDeleteNotification.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::String> v8_msgID = v8::String::NewFromUtf8(isolate, zs2s(msgID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
		v8::Local<v8::Integer> v8_deleteBy = v8::Integer::New(isolate, (int32_t)deleteBy);
		int argc = 2;
		v8::Local<v8::Value> argv[2] = { v8_msgID, v8_deleteBy };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onChatMsgDeleteNotification);

		fn->Call(context, global, argc, argv);
	}
	virtual void onChatMessageEditNotification(ZNChatMsgInfo chatMsg)
	{
		if (ZoomNodeSinkHelper::GetInst().onChatMessageEditNotification.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Object> node = NodeWrapUtil::CreateChatMsgInfoObject(isolate, chatMsg);

		int argc = 1;
		v8::Local<v8::Value> argv[1] = { node };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onChatMessageEditNotification);

		fn->Call(context, global, argc, argv);
	}
	virtual void onShareMeetingChatStatusChanged(bool isStart)
	{
		if (ZoomNodeSinkHelper::GetInst().onShareMeetingChatStatusChanged.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Boolean> v8_isStart = v8::Boolean::New(isolate, isStart);
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_isStart };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onShareMeetingChatStatusChanged);

		fn->Call(context, global, argc, argv);
	}
	virtual void onFileSendStart(ZNSDKFileTransferInfo senderInfo)
	{
		if (ZoomNodeSinkHelper::GetInst().onFileSendStart.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Object> node = v8::Object::New(isolate);
		node->Set(context, v8::String::NewFromUtf8(isolate, "messageID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(senderInfo.messageID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "trans_status", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, senderInfo.trans_status));
		node->Set(context, v8::String::NewFromUtf8(isolate, "time_stamp", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, senderInfo.time_stamp));
		node->Set(context, v8::String::NewFromUtf8(isolate, "is_send_to_all", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, senderInfo.is_send_to_all));
		node->Set(context, v8::String::NewFromUtf8(isolate, "file_size", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, senderInfo.file_size));
		node->Set(context, v8::String::NewFromUtf8(isolate, "file_name", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(senderInfo.file_name).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "complete_percentage", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, senderInfo.complete_percentage));
		node->Set(context, v8::String::NewFromUtf8(isolate, "complete_size", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, senderInfo.complete_size));
		node->Set(context, v8::String::NewFromUtf8(isolate, "bit_per_second", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, senderInfo.bit_per_second));

		int argc = 1;
		v8::Local<v8::Value> argv[1] = { node };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onFileSendStart);

		fn->Call(context, global, argc, argv);
	}
	virtual void onFileReceived(ZNSDKFileTransferInfo receiverInfo)
	{
		if (ZoomNodeSinkHelper::GetInst().onFileReceived.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Object> node = v8::Object::New(isolate);
		node->Set(context, v8::String::NewFromUtf8(isolate, "messageID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(receiverInfo.messageID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "trans_status", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, receiverInfo.trans_status));
		node->Set(context, v8::String::NewFromUtf8(isolate, "time_stamp", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, receiverInfo.time_stamp));
		node->Set(context, v8::String::NewFromUtf8(isolate, "is_send_to_all", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, receiverInfo.is_send_to_all));
		node->Set(context, v8::String::NewFromUtf8(isolate, "file_size", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, receiverInfo.file_size));
		node->Set(context, v8::String::NewFromUtf8(isolate, "file_name", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(receiverInfo.file_name).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "complete_percentage", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, receiverInfo.complete_percentage));
		node->Set(context, v8::String::NewFromUtf8(isolate, "complete_size", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, receiverInfo.complete_size));
		node->Set(context, v8::String::NewFromUtf8(isolate, "bit_per_second", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, receiverInfo.bit_per_second));

		int argc = 1;
		v8::Local<v8::Value> argv[1] = { node };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onFileReceived);

		fn->Call(context, global, argc, argv);
	}
	virtual void onFileTransferProgress(ZNSDKFileTransferInfo transferInfo)
	{
		if (ZoomNodeSinkHelper::GetInst().onFileTransferProgress.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Object> node = v8::Object::New(isolate);
		node->Set(context, v8::String::NewFromUtf8(isolate, "messageID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(transferInfo.messageID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "trans_status", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, transferInfo.trans_status));
		node->Set(context, v8::String::NewFromUtf8(isolate, "time_stamp", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, transferInfo.time_stamp));
		node->Set(context, v8::String::NewFromUtf8(isolate, "is_send_to_all", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Boolean::New(isolate, transferInfo.is_send_to_all));
		node->Set(context, v8::String::NewFromUtf8(isolate, "file_size", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, transferInfo.file_size));
		node->Set(context, v8::String::NewFromUtf8(isolate, "file_name", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(transferInfo.file_name).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "complete_percentage", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, transferInfo.complete_percentage));
		node->Set(context, v8::String::NewFromUtf8(isolate, "complete_size", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, transferInfo.complete_size));
		node->Set(context, v8::String::NewFromUtf8(isolate, "bit_per_second", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, transferInfo.bit_per_second));

		int argc = 1;
		v8::Local<v8::Value> argv[1] = { node };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onFileTransferProgress);

		fn->Call(context, global, argc, argv);
	}
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZNativeSDKMeetingWaitingRoomWrapSink
{
public:
	virtual void onWaitingRoomUserJoin(unsigned int userID)
	{
		if (ZoomNodeSinkHelper::GetInst().onWaitingRoomUserJoin.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Integer > v8_userID = v8::Integer::NewFromUnsigned(isolate, userID);
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_userID };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onWaitingRoomUserJoin);

		fn->Call(context, global, argc, argv);
	}
	virtual void onWaitingRoomUserLeft(unsigned int userID)
	{
		if (ZoomNodeSinkHelper::GetInst().onWaitingRoomUserLeft.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Integer > v8_userID = v8::Integer::NewFromUnsigned(isolate, userID);
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_userID };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onWaitingRoomUserLeft);

		fn->Call(context, global, argc, argv);
	}
	virtual void onWaitingRoomPresetAudioStatusChanged(bool bAudioCanTurnOn)
	{
		if (ZoomNodeSinkHelper::GetInst().onWaitingRoomPresetAudioStatusChanged.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Boolean> v8_bAudioCanTurnOn = v8::Boolean::New(isolate, bAudioCanTurnOn);
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_bAudioCanTurnOn };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onWaitingRoomPresetAudioStatusChanged);

		fn->Call(context, global, argc, argv);
	}
	virtual void onWaitingRoomPresetVideoStatusChanged(bool bVideoCanTurnOn)
	{
		if (ZoomNodeSinkHelper::GetInst().onWaitingRoomPresetVideoStatusChanged.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Boolean> v8_bVideoCanTurnOn = v8::Boolean::New(isolate, bVideoCanTurnOn);
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_bVideoCanTurnOn };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onWaitingRoomPresetVideoStatusChanged);

		fn->Call(context, global, argc, argv);
	}
	virtual void onCustomWaitingRoomDataUpdated(ZNCustomWaitingRoomData bData)
	{
		if (ZoomNodeSinkHelper::GetInst().onCustomWaitingRoomDataUpdated.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Object> node = v8::Object::New(isolate);

		node->Set(context, v8::String::NewFromUtf8(isolate, "title", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(bData.title).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "description", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(bData.description).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "logo_path", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(bData.logo_path).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "video_path", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(bData.video_path).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "image_path", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(bData.image_path).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "type", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, bData.type));
		node->Set(context, v8::String::NewFromUtf8(isolate, "status", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, bData.status));

		v8::Local<v8::Object> pageBackgroundColorObj = v8::Object::New(isolate);
		pageBackgroundColorObj->Set(context, v8::String::NewFromUtf8(isolate, "red", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, bData.page_color.background_color.red));
		pageBackgroundColorObj->Set(context, v8::String::NewFromUtf8(isolate, "green", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, bData.page_color.background_color.green));
		pageBackgroundColorObj->Set(context, v8::String::NewFromUtf8(isolate, "blue", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, bData.page_color.background_color.blue));

		v8::Local<v8::Object> pageColorObj = v8::Object::New(isolate);
		pageColorObj->Set(context, v8::String::NewFromUtf8(isolate, "background_color", v8::NewStringType::kInternalized).ToLocalChecked(), pageBackgroundColorObj);

		v8::Local<v8::Object> textPrimaryColorObj = v8::Object::New(isolate);
		textPrimaryColorObj->Set(context, v8::String::NewFromUtf8(isolate, "red", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, bData.text_color.primary_color.red));
		textPrimaryColorObj->Set(context, v8::String::NewFromUtf8(isolate, "green", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, bData.text_color.primary_color.green));
		textPrimaryColorObj->Set(context, v8::String::NewFromUtf8(isolate, "blue", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, bData.text_color.primary_color.blue));
		
		v8::Local<v8::Object> textSecondaryColorObj = v8::Object::New(isolate);
		textSecondaryColorObj->Set(context, v8::String::NewFromUtf8(isolate, "red", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, bData.text_color.secondary_color.red));
		textSecondaryColorObj->Set(context, v8::String::NewFromUtf8(isolate, "green", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, bData.text_color.secondary_color.green));
		textSecondaryColorObj->Set(context, v8::String::NewFromUtf8(isolate, "blue", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, bData.text_color.secondary_color.blue));
		
		v8::Local<v8::Object> textParagraphColorObj = v8::Object::New(isolate);
		textParagraphColorObj->Set(context, v8::String::NewFromUtf8(isolate, "red", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, bData.text_color.paragraph_color.red));
		textParagraphColorObj->Set(context, v8::String::NewFromUtf8(isolate, "green", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, bData.text_color.paragraph_color.green));
		textParagraphColorObj->Set(context, v8::String::NewFromUtf8(isolate, "blue", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, bData.text_color.paragraph_color.blue));
		
		v8::Local<v8::Object> textHyperlinkColorObj = v8::Object::New(isolate);
		textHyperlinkColorObj->Set(context, v8::String::NewFromUtf8(isolate, "red", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, bData.text_color.hyperlink_color.red));
		textHyperlinkColorObj->Set(context, v8::String::NewFromUtf8(isolate, "green", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, bData.text_color.hyperlink_color.green));
		textHyperlinkColorObj->Set(context, v8::String::NewFromUtf8(isolate, "blue", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, bData.text_color.hyperlink_color.blue));
		
		v8::Local<v8::Object> textColorObj = v8::Object::New(isolate);
		textColorObj->Set(context, v8::String::NewFromUtf8(isolate, "primary_color", v8::NewStringType::kInternalized).ToLocalChecked(),textPrimaryColorObj);
		textColorObj->Set(context, v8::String::NewFromUtf8(isolate, "secondary_color", v8::NewStringType::kInternalized).ToLocalChecked(),textSecondaryColorObj);
		textColorObj->Set(context, v8::String::NewFromUtf8(isolate, "paragraph_color", v8::NewStringType::kInternalized).ToLocalChecked(),textParagraphColorObj);
		textColorObj->Set(context, v8::String::NewFromUtf8(isolate, "hyperlink_color", v8::NewStringType::kInternalized).ToLocalChecked(),textHyperlinkColorObj);
		
		v8::Local<v8::Object> buttonPrimaryColorObj = v8::Object::New(isolate);
		buttonPrimaryColorObj->Set(context, v8::String::NewFromUtf8(isolate, "red", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, bData.button_color.primary_button_color.red));
		buttonPrimaryColorObj->Set(context, v8::String::NewFromUtf8(isolate, "green", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, bData.button_color.primary_button_color.green));
		buttonPrimaryColorObj->Set(context, v8::String::NewFromUtf8(isolate, "blue", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, bData.button_color.primary_button_color.blue));

		v8::Local<v8::Object> buttonColorObj = v8::Object::New(isolate);
		buttonColorObj->Set(context, v8::String::NewFromUtf8(isolate, "primary_button_color", v8::NewStringType::kInternalized).ToLocalChecked(), buttonPrimaryColorObj);
		
		node->Set(context, v8::String::NewFromUtf8(isolate, "page_color", v8::NewStringType::kInternalized).ToLocalChecked(), pageColorObj);
		node->Set(context, v8::String::NewFromUtf8(isolate, "text_color", v8::NewStringType::kInternalized).ToLocalChecked(), textColorObj);
		node->Set(context, v8::String::NewFromUtf8(isolate, "button_color", v8::NewStringType::kInternalized).ToLocalChecked(), buttonColorObj);

		int argc = 1;
		v8::Local<v8::Value> argv[1] = { node };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onCustomWaitingRoomDataUpdated);

		fn->Call(context, global, argc, argv);
	}
	virtual void onWaitingRoomUserNameChanged(unsigned int userID, ZoomSTRING userName)
	{
		if (ZoomNodeSinkHelper::GetInst().onWaitingRoomUserNameChanged.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Integer > v8_userID = v8::Integer::NewFromUnsigned(isolate, userID);
		v8::Local<v8::String> v8_userName = v8::String::NewFromUtf8(isolate, zs2s(userName).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
		int argc = 2;
		v8::Local<v8::Value> argv[2] = { v8_userID, v8_userName };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onWaitingRoomUserNameChanged);

		fn->Call(context, global, argc, argv);
	}
	virtual void onWaitingRoomEntranceEnabled(bool bIsEnabled)
	{
		if (ZoomNodeSinkHelper::GetInst().onWaitingRoomEntranceEnabled.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Boolean> v8_bIsEnabled = v8::Boolean::New(isolate, bIsEnabled);
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_bIsEnabled };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onWaitingRoomEntranceEnabled);

		fn->Call(context, global, argc, argv);
	}
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZNativeSDKMeetingAICompanionWrapSink
{
public:
	virtual void onAICompanionFeatureTurnOffByParticipant()
	{
		if (ZoomNodeSinkHelper::GetInst().onAICompanionFeatureTurnOffByParticipant.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onAICompanionFeatureTurnOffByParticipant);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onAICompanionFeatureSwitchRequested()
	{
		if (ZoomNodeSinkHelper::GetInst().onAICompanionFeatureSwitchRequested.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onAICompanionFeatureSwitchRequested);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onAICompanionFeatureSwitchRequestResponse(bool bTimeout, bool bAgree, bool bTurnOn)
	{
		if (ZoomNodeSinkHelper::GetInst().onAICompanionFeatureSwitchRequestResponse.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Boolean> v8_timeout = v8::Boolean::New(isolate, bTimeout);
		v8::Local<v8::Boolean> v8_agree = v8::Boolean::New(isolate, bAgree);
		v8::Local<v8::Boolean> v8_turnOn = v8::Boolean::New(isolate, bTurnOn);
		int argc = 3;
		v8::Local<v8::Value> argv[3] = { v8_timeout, v8_agree, v8_turnOn };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onAICompanionFeatureSwitchRequestResponse);

		fn->Call(context, global, argc, argv);
	}
	virtual void onAICompanionFeatureCanNotBeTurnedOff(ZNList<ZNAICompanionFeature> features)
	{
		if (ZoomNodeSinkHelper::GetInst().onAICompanionFeatureCanNotBeTurnedOff.IsEmpty())
		{
			return;
		}

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Array> nodes = v8::Array::New(isolate);
		for (unsigned int i = 0; i < features.size(); ++i) {
			nodes->Set(context, i, v8::Integer::New(isolate, (int32_t)features[i]));
		}
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { nodes };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onAICompanionFeatureCanNotBeTurnedOff);

		fn->Call(context, global, argc, argv);
	}
	virtual void onSmartSummaryStateNotSupported()
	{
		if (ZoomNodeSinkHelper::GetInst().onSmartSummaryStateNotSupported.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onSmartSummaryStateNotSupported);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onSmartSummaryStateSupportedButDisabled()
	{
		if (ZoomNodeSinkHelper::GetInst().onSmartSummaryStateSupportedButDisabled.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onSmartSummaryStateSupportedButDisabled);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onSmartSummaryStateEnabledButNotStarted()
	{
		if (ZoomNodeSinkHelper::GetInst().onSmartSummaryStateEnabledButNotStarted.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onSmartSummaryStateEnabledButNotStarted);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onSmartSummaryStateStarted()
	{
		if (ZoomNodeSinkHelper::GetInst().onSmartSummaryStateStarted.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onSmartSummaryStateStarted);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onFailedToStartSmartSummary(bool bTimeout)
	{
		if (ZoomNodeSinkHelper::GetInst().onFailedToStartSmartSummary.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Boolean> v8_timeout = v8::Boolean::New(isolate, bTimeout);
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_timeout };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onFailedToStartSmartSummary);

		fn->Call(context, global, argc, argv);
	}
	virtual void onSmartSummaryEnableRequestReceived()
	{
		if (ZoomNodeSinkHelper::GetInst().onSmartSummaryEnableRequestReceived.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onSmartSummaryEnableRequestReceived);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onSmartSummaryStartRequestReceived()
	{
		if (ZoomNodeSinkHelper::GetInst().onSmartSummaryStartRequestReceived.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onSmartSummaryStartRequestReceived);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onSmartSummaryEnableActionCallback()
	{
		if (ZoomNodeSinkHelper::GetInst().onSmartSummaryEnableActionCallback.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onSmartSummaryEnableActionCallback);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onQueryStateNotSupported()
	{
		if (ZoomNodeSinkHelper::GetInst().onQueryStateNotSupported.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onQueryStateNotSupported);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onQueryStateSupportedButDisabled()
	{
		if (ZoomNodeSinkHelper::GetInst().onQueryStateSupportedButDisabled.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onQueryStateSupportedButDisabled);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onQueryStateEnabledButNotStarted()
	{
		if (ZoomNodeSinkHelper::GetInst().onQueryStateEnabledButNotStarted.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onQueryStateEnabledButNotStarted);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onQueryStateStarted()
	{
		if (ZoomNodeSinkHelper::GetInst().onQueryStateStarted.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onQueryStateStarted);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onQuerySettingChanged(ZNMeetingAICompanionQuerySettingOptions eSetting)
	{
		if (ZoomNodeSinkHelper::GetInst().onQuerySettingChanged.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Integer> v8_eSetting = v8::Integer::New(isolate, (int32_t)eSetting);
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_eSetting };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onQuerySettingChanged);

		fn->Call(context, global, argc, argv);
	}
	virtual void onFailedToStartQuery(bool bTimeout)
	{
		if (ZoomNodeSinkHelper::GetInst().onFailedToStartQuery.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Boolean> v8_bTimeout = v8::Boolean::New(isolate, bTimeout);
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_bTimeout };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onFailedToStartQuery);

		fn->Call(context, global, argc, argv);
	}
	virtual void onReceiveRequestToEnableQuery()
	{
		if (ZoomNodeSinkHelper::GetInst().onReceiveRequestToEnableQuery.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onReceiveRequestToEnableQuery);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onReceiveRequestToStartQuery()
	{
		if (ZoomNodeSinkHelper::GetInst().onReceiveRequestToStartQuery.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onReceiveRequestToStartQuery);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onReceiveQueryAnswer(ZNMeetingAICompanionQueryItem queryItem)
	{
		if (ZoomNodeSinkHelper::GetInst().onReceiveQueryAnswer.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Object> node = v8::Object::New(isolate);
		node->Set(context, v8::String::NewFromUtf8(isolate, "queryID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(queryItem.queryID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "qustionContent", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(queryItem.qustionContent).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "answerContent", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(queryItem.answerContent).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "errorCode", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, queryItem.errorCode));
		node->Set(context, v8::String::NewFromUtf8(isolate, "errorMsg", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(queryItem.errorMsg).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "timeStamp", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, queryItem.timeStamp));

		int argc = 1;
		v8::Local<v8::Value> argv[1] = { node };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onReceiveQueryAnswer);

		fn->Call(context, global, argc, argv);
	}
	virtual void onQueryEnableActionCallback()
	{
		if (ZoomNodeSinkHelper::GetInst().onQueryEnableActionCallback.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onQueryEnableActionCallback);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onSendQueryPrivilegeChanged(bool canSendQuery)
	{
		if (ZoomNodeSinkHelper::GetInst().onSendQueryPrivilegeChanged.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Boolean> v8_canSendQuery = v8::Boolean::New(isolate, canSendQuery);
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_canSendQuery };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onSendQueryPrivilegeChanged);

		fn->Call(context, global, argc, argv);
	}
	virtual void onFailedToRequestSendQuery(bool bTimeout)
	{
		if (ZoomNodeSinkHelper::GetInst().onFailedToRequestSendQuery.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Boolean> v8_bTimeout = v8::Boolean::New(isolate, bTimeout);
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_bTimeout };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onFailedToRequestSendQuery);

		fn->Call(context, global, argc, argv);
	}
	virtual void onReceiveRequestToSendQuery()
	{
		if (ZoomNodeSinkHelper::GetInst().onReceiveRequestToSendQuery.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onReceiveRequestToSendQuery);

		fn->Call(context, global, 0, NULL);
	}
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZNativeSDKMeetingIndicatorWrapSink
{
public:
	virtual void onIndicatorItemReceived()
	{
		if (ZoomNodeSinkHelper::GetInst().onIndicatorItemReceived.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onIndicatorItemReceived);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onIndicatorItemRemoved()
	{
		if (ZoomNodeSinkHelper::GetInst().onIndicatorItemRemoved.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onIndicatorItemRemoved);

		fn->Call(context, global, 0, NULL);
	}
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZNativeSDKMeetingWebinarWrapSink
{
public:
	virtual void onPromptAttendee2PanelistResult(int result)
	{
		if (ZoomNodeSinkHelper::GetInst().onPromptAttendee2PanelistResult.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Integer> v8_result = v8::Integer::NewFromUnsigned(isolate, result);
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_result };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onPromptAttendee2PanelistResult);

		fn->Call(context, global, argc, argv);
	}
	virtual void onDepromptPanelist2AttendeeResult(int result)
	{
		if (ZoomNodeSinkHelper::GetInst().onDepromptPanelist2AttendeeResult.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Integer> v8_result = v8::Integer::NewFromUnsigned(isolate, result);
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_result };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onDepromptPanelist2AttendeeResult);

		fn->Call(context, global, argc, argv);
	}
	virtual void onAllowPanelistStartVideoNotification()
	{
		if (ZoomNodeSinkHelper::GetInst().onAllowPanelistStartVideoNotification.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onAllowPanelistStartVideoNotification);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onDisallowPanelistStartVideoNotification()
	{
		if (ZoomNodeSinkHelper::GetInst().onDisallowPanelistStartVideoNotification.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onDisallowPanelistStartVideoNotification);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onSelfAllowTalkNotification()
	{
		if (ZoomNodeSinkHelper::GetInst().onSelfAllowTalkNotification.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onSelfAllowTalkNotification);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onSelfDisallowTalkNotification()
	{
		if (ZoomNodeSinkHelper::GetInst().onSelfDisallowTalkNotification.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onSelfDisallowTalkNotification);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onAllowAttendeeChatNotification()
	{
		if (ZoomNodeSinkHelper::GetInst().onAllowAttendeeChatNotification.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onAllowAttendeeChatNotification);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onDisallowAttendeeChatNotification()
	{
		if (ZoomNodeSinkHelper::GetInst().onDisallowAttendeeChatNotification.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onDisallowAttendeeChatNotification);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onAllowWebinarReactionStatusChanged(bool can_reaction)
	{
		if (ZoomNodeSinkHelper::GetInst().onAllowWebinarReactionStatusChanged.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Boolean> v8_can_reaction = v8::Boolean::New(isolate, can_reaction);
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_can_reaction };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onAllowWebinarReactionStatusChanged);

		fn->Call(context, global, argc, argv);
	}
	virtual void onAllowAttendeeRaiseHandStatusChanged(bool can_raiseHand)
	{
		if (ZoomNodeSinkHelper::GetInst().onAllowAttendeeRaiseHandStatusChanged.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Boolean> v8_can_raiseHand = v8::Boolean::New(isolate, can_raiseHand);
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_can_raiseHand };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onAllowAttendeeRaiseHandStatusChanged);

		fn->Call(context, global, argc, argv);
	}
	virtual void onAllowAttendeeViewTheParticipantCountStatusChanged(bool can_viewParticipantCount)
	{
		if (ZoomNodeSinkHelper::GetInst().onAllowAttendeeViewTheParticipantCountStatusChanged.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Boolean> v8_can_viewParticipantCount = v8::Boolean::New(isolate, can_viewParticipantCount);
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_can_viewParticipantCount };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onAllowAttendeeViewTheParticipantCountStatusChanged);

		fn->Call(context, global, argc, argv);
	}
	virtual void onAttendeeAudioStatusNotification(unsigned int userid, bool can_talk, bool is_muted)
	{
		if (ZoomNodeSinkHelper::GetInst().onAttendeeAudioStatusNotification.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Integer> v8_userid = v8::Integer::NewFromUnsigned(isolate, userid);
		v8::Local<v8::Boolean> v8_can_talk = v8::Boolean::New(isolate, can_talk);
		v8::Local<v8::Boolean> v8_is_muted = v8::Boolean::New(isolate, is_muted);
		int argc = 3;
		v8::Local<v8::Value> argv[3] = { v8_userid, v8_can_talk, v8_is_muted };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onAttendeeAudioStatusNotification);

		fn->Call(context, global, argc, argv);
	}
	virtual void onAttendeePromoteConfirmResult(bool agree, unsigned int userid)
	{
		if (ZoomNodeSinkHelper::GetInst().onAttendeePromoteConfirmResult.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Boolean> v8_agree = v8::Boolean::New(isolate, agree);
		v8::Local<v8::Integer> v8_userid = v8::Integer::NewFromUnsigned(isolate, userid);
		int argc = 2;
		v8::Local<v8::Value> argv[2] = { v8_agree, v8_userid };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onAttendeePromoteConfirmResult);

		fn->Call(context, global, argc, argv);
	}
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZNativeSDKMeetingQAWrapSink {
public:
	virtual void onAddQuestion(ZoomSTRING questionID, bool bSuccess) {
		if (ZoomNodeSinkHelper::GetInst().onAddQuestion.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::String> v8_questionID = v8::String::NewFromUtf8(isolate, zs2s(questionID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
		v8::Local<v8::Boolean> v8_bSuccess = v8::Boolean::New(isolate, bSuccess);
		int argc = 2;
		v8::Local<v8::Value> argv[2] = { v8_questionID, v8_bSuccess};
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onAddQuestion);

		fn->Call(context, global, argc, argv);
	}
	virtual void onReceiveQuestion(ZoomSTRING questionID) {
		if (ZoomNodeSinkHelper::GetInst().onReceiveQuestion.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::String> v8_questionID = v8::String::NewFromUtf8(isolate, zs2s(questionID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_questionID};
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onReceiveQuestion);

		fn->Call(context, global, argc, argv);
	}
	virtual void onDeleteQuestions(ZNList<ZoomSTRING> questions) {
		if (ZoomNodeSinkHelper::GetInst().onDeleteQuestions.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Array> nodes = v8::Array::New(isolate);
		for (unsigned int i = 0; i < questions.size(); ++i) {
			v8::HandleScope scope(isolate);
			v8::Local<v8::Object> node = v8::Object::New(isolate);
			node->Set(context, v8::String::NewFromUtf8(isolate, "questionID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(questions[i]).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
			nodes->Set(context, i, node);
		}

		int argc = 1;
		v8::Local<v8::Value> argv[1] = { nodes };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onDeleteQuestions);

		fn->Call(context, global, argc, argv);
	}
	virtual void onQuestionMarkedAsDismissed(ZoomSTRING questionID) {
		if (ZoomNodeSinkHelper::GetInst().onQuestionMarkedAsDismissed.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::String> v8_questionID = v8::String::NewFromUtf8(isolate, zs2s(questionID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_questionID};
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onQuestionMarkedAsDismissed);

		fn->Call(context, global, argc, argv);
	}
	virtual void onReopenQuestion(ZoomSTRING questionID) {
		if (ZoomNodeSinkHelper::GetInst().onReopenQuestion.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::String> v8_questionID = v8::String::NewFromUtf8(isolate, zs2s(questionID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_questionID};
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onReopenQuestion);

		fn->Call(context, global, argc, argv);
	}
	virtual void onUserLivingReply(ZoomSTRING questionID) {
		if (ZoomNodeSinkHelper::GetInst().onUserLivingReply.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::String> v8_questionID = v8::String::NewFromUtf8(isolate, zs2s(questionID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_questionID};
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onUserLivingReply);

		fn->Call(context, global, argc, argv);
	}
	virtual void onUserEndLiving(ZoomSTRING questionID) {
		if (ZoomNodeSinkHelper::GetInst().onUserEndLiving.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::String> v8_questionID = v8::String::NewFromUtf8(isolate, zs2s(questionID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_questionID};
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onUserEndLiving);

		fn->Call(context, global, argc, argv);
	}
	virtual void onAddAnswer(ZoomSTRING answerID, bool bSuccess) {
		if (ZoomNodeSinkHelper::GetInst().onAddAnswer.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::String> v8_answerID = v8::String::NewFromUtf8(isolate, zs2s(answerID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
		v8::Local<v8::Boolean> v8_bSuccess = v8::Boolean::New(isolate, bSuccess);
		int argc = 2;
		v8::Local<v8::Value> argv[2] = { v8_answerID, v8_bSuccess};
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onAddAnswer);

		fn->Call(context, global, argc, argv);
	}
	virtual void onReceiveAnswer(ZoomSTRING answerID) {
		if (ZoomNodeSinkHelper::GetInst().onReceiveAnswer.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::String> v8_answerID = v8::String::NewFromUtf8(isolate, zs2s(answerID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_answerID};
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onReceiveAnswer);

		fn->Call(context, global, argc, argv);
	}
	virtual void onDeleteAnswers(ZNList<ZoomSTRING> answer) {
		if (ZoomNodeSinkHelper::GetInst().onDeleteAnswers.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Array> nodes = v8::Array::New(isolate);
		for (unsigned int i = 0; i < answer.size(); ++i) {
			v8::HandleScope scope(isolate);
			v8::Local<v8::Object> node = v8::Object::New(isolate);
			node->Set(context, v8::String::NewFromUtf8(isolate, "answerID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(answer[i]).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
			nodes->Set(context, i, node);
		}

		int argc = 1;
		v8::Local<v8::Value> argv[1] = { nodes };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onDeleteAnswers);

		fn->Call(context, global, argc, argv);
	}
	virtual void onVoteupQuestion(ZoomSTRING questionID, bool isChanged) {
		if (ZoomNodeSinkHelper::GetInst().onVoteupQuestion.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::String> v8_questionID = v8::String::NewFromUtf8(isolate, zs2s(questionID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
		v8::Local<v8::Boolean> v8_isChanged = v8::Boolean::New(isolate, isChanged);
		int argc = 2;
		v8::Local<v8::Value> argv[2] = { v8_questionID, v8_isChanged};
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onVoteupQuestion);

		fn->Call(context, global, argc, argv);
	}
	virtual void onRevokeVoteupQuestion(ZoomSTRING questionID, bool isChanged) {
		if (ZoomNodeSinkHelper::GetInst().onRevokeVoteupQuestion.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::String> v8_questionID = v8::String::NewFromUtf8(isolate, zs2s(questionID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
		v8::Local<v8::Boolean> v8_isChanged = v8::Boolean::New(isolate, isChanged);
		int argc = 2;
		v8::Local<v8::Value> argv[2] = { v8_questionID, v8_isChanged};
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onRevokeVoteupQuestion);

		fn->Call(context, global, argc, argv);
	}
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZNativeSDKMeetingReactionWrapSink {
public:
	virtual void onEmojiReactionReceived(unsigned int userid, ZNSDKEmojiReactionType type, ZNSDKEmojiReactionSkinTone skinTone) {
		if (ZoomNodeSinkHelper::GetInst().onEmojiReactionReceived.IsEmpty())
		return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Integer> v8_userid = v8::Integer::NewFromUnsigned(isolate, userid);
		v8::Local<v8::Integer> v8_type = v8::Integer::New(isolate, (int32_t)type);
		v8::Local<v8::Integer> v8_skinTone = v8::Integer::New(isolate, (int32_t)skinTone);
		int argc = 3;
		v8::Local<v8::Value> argv[3] = { v8_userid, v8_type, v8_skinTone };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onEmojiReactionReceived);

		fn->Call(context, global, argc, argv);
	}
	virtual void onEmojiReactionReceivedInWebinar(ZNSDKEmojiReactionType type) {
		if (ZoomNodeSinkHelper::GetInst().onEmojiReactionReceivedInWebinar.IsEmpty())
		return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Integer> v8_type = v8::Integer::New(isolate, (int32_t)type);
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_type };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onEmojiReactionReceivedInWebinar);

		fn->Call(context, global, argc, argv);
	}
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZNativeSDKCloseCaptionWrapSink {
public:
	virtual void onLiveTranscriptionMsgInfoReceived(ZNLiveTranscriptionMessageInfo messageInfo) {
		if (ZoomNodeSinkHelper::GetInst().onLiveTranscriptionMsgInfoReceived.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Object> node = v8::Object::New(isolate);
		node->Set(context, v8::String::NewFromUtf8(isolate, "messageID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(messageInfo.messageID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "speakerID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, messageInfo.speakerID));
		node->Set(context, v8::String::NewFromUtf8(isolate, "speakerName", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(messageInfo.speakerName).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "messageContent", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(messageInfo.messageContent).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "timeStamp", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, messageInfo.timeStamp));
		node->Set(context, v8::String::NewFromUtf8(isolate, "messageType", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, messageInfo.messageType));

		int argc = 1;
		v8::Local<v8::Value> argv[1] = { node };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onLiveTranscriptionMsgInfoReceived);

		fn->Call(context, global, argc, argv);
	}
	virtual void onOriginalLanguageMsgReceived(ZNLiveTranscriptionMessageInfo messageInfo) {
		if (ZoomNodeSinkHelper::GetInst().onOriginalLanguageMsgReceived.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Object> node = v8::Object::New(isolate);
		node->Set(context, v8::String::NewFromUtf8(isolate, "messageID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(messageInfo.messageID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "speakerID", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, messageInfo.speakerID));
		node->Set(context, v8::String::NewFromUtf8(isolate, "speakerName", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(messageInfo.speakerName).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "messageContent", v8::NewStringType::kInternalized).ToLocalChecked(), v8::String::NewFromUtf8(isolate, zs2s(messageInfo.messageContent).c_str(), v8::NewStringType::kInternalized).ToLocalChecked());
		node->Set(context, v8::String::NewFromUtf8(isolate, "timeStamp", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, messageInfo.timeStamp));
		node->Set(context, v8::String::NewFromUtf8(isolate, "messageType", v8::NewStringType::kInternalized).ToLocalChecked(), v8::Integer::New(isolate, messageInfo.messageType));

		int argc = 1;
		v8::Local<v8::Value> argv[1] = { node };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onOriginalLanguageMsgReceived);

		fn->Call(context, global, argc, argv);
	}
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZNativeSDKPollingWrapSink {
public:
	virtual void onPollingStatusChanged(ZoomSTRING pollingID, ZNSDKPollingStatus status) {
		if (ZoomNodeSinkHelper::GetInst().onPollingStatusChanged.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::String> v8_pollingID = v8::String::NewFromUtf8(isolate, zs2s(pollingID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
		v8::Local<v8::Integer> v8_status = v8::Integer::New(isolate, (int32_t)status);

		int argc = 2;
		v8::Local<v8::Value> argv[2] = { v8_pollingID, v8_status};
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onPollingStatusChanged);

		fn->Call(context, global, argc, argv);
	}
	virtual void onPollingResultUpdated(ZoomSTRING pollingID) {
		if (ZoomNodeSinkHelper::GetInst().onPollingResultUpdated.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::String> v8_pollingID = v8::String::NewFromUtf8(isolate, zs2s(pollingID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();

		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_pollingID};
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onPollingResultUpdated);

		fn->Call(context, global, argc, argv);
	}
	virtual void onPollingListUpdated() {
		if (ZoomNodeSinkHelper::GetInst().onPollingListUpdated.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		/*int argc = 0;
		v8::Local<v8::Value> argv[0] = {};*/
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onPollingListUpdated);

		fn->Call(context, global, 0, NULL);
	}
	virtual void onPollingActionResult(ZNSDKPollingActionType actionType, ZoomSTRING pollingID, bool bSuccess, ZoomSTRING errorMsg) {
		if (ZoomNodeSinkHelper::GetInst().onPollingActionResult.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Integer> v8_actionType = v8::Integer::New(isolate, (int32_t)actionType);
		v8::Local<v8::String> v8_pollingID = v8::String::NewFromUtf8(isolate, zs2s(pollingID).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
		v8::Local<v8::Boolean> v8_bSuccess = v8::Boolean::New(isolate, bSuccess);
		v8::Local<v8::String> v8_errorMsg = v8::String::NewFromUtf8(isolate, zs2s(errorMsg).c_str(), v8::NewStringType::kInternalized).ToLocalChecked();

		int argc = 4;
		v8::Local<v8::Value> argv[4] = { v8_actionType, v8_pollingID, v8_bSuccess, v8_errorMsg};
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onPollingActionResult);

		fn->Call(context, global, argc, argv);
	}
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZNativeSDKMeetingWhiteboardWrapSink
{
public:
	virtual void onWhiteboardStatusChanged(ZNSDKWhiteboardStatus status)
	{
		if (ZoomNodeSinkHelper::GetInst().onWhiteboardStatusChanged.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Integer> v8_status = v8::Integer::NewFromUnsigned(isolate, status);
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { v8_status };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onWhiteboardStatusChanged);

		fn->Call(context, global, argc, argv);
	}
	virtual void onWhiteboardSettingsChanged(ZNSDKWhiteboardShareOption shareOption, ZNSDKWhiteboardCreateOption createOption, bool bEnable)
	{
		if (ZoomNodeSinkHelper::GetInst().onWhiteboardSettingsChanged.IsEmpty())
			return;

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Integer> v8_shareOption = v8::Integer::NewFromUnsigned(isolate, shareOption);
		v8::Local<v8::Integer> v8_createOption = v8::Integer::NewFromUnsigned(isolate, createOption);
		v8::Local<v8::Boolean> v8_bEnable = v8::Boolean::New(isolate, bEnable);
		int argc = 3;
		v8::Local<v8::Value> argv[3] = { v8_shareOption, v8_createOption, v8_bEnable };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onWhiteboardSettingsChanged);

		fn->Call(context, global, argc, argv);
	}
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZNativeSDKMeetingDocsWrapSink
{
public:
	virtual void onDocsSharingSourceInfoChanged(ZNZoomSDKDocSharingSourceInfo zn_sourceinfo)
	{
		if (ZoomNodeSinkHelper::GetInst().onDocsSharingSourceInfoChanged.IsEmpty())
		{
			return;
		}

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Object> node = NodeWrapUtil::CreateZoomSDKDocSharingSourceInfoObject(isolate, zn_sourceinfo);
		int argc = 1;
		v8::Local<v8::Value> argv[1] = { node };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onDocsSharingSourceInfoChanged);
		fn->Call(context, global, argc, argv);
	}
	virtual void onDocsPermissionChanged(ZNSDKDocsCreateOption createOption, ZNSDKDocsShareOption option)
	{
		if (ZoomNodeSinkHelper::GetInst().onDocsPermissionChanged.IsEmpty())
		{
			return;
		}

		auto isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();
		auto global = context->Global();

		v8::Local<v8::Integer> v8_createOption = v8::Integer::NewFromUnsigned(isolate, createOption);
		v8::Local<v8::Integer> v8_option = v8::Integer::NewFromUnsigned(isolate, option);
		int argc = 2;
		v8::Local<v8::Value> argv[2] = { v8_createOption, v8_option };
		auto fn = v8::Local<v8::Function>::New(isolate, ZoomNodeSinkHelper::GetInst().onDocsPermissionChanged);
		fn->Call(context, global, argc, argv);
	}
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ZoomNodeSinkWrapMgr
{
public:
	//auth_service_cb	
	ZNativeSDKAuthWrapSink m_authServiceWrapSink;

	//meeting_service_cb
	ZNativeSDKMeetingWrapSink m_meetingServiceWrapSink;

	//meeting_ui_ctrl_cb
	ZNativeSDKMeetingUICtrlWrapSink m_meetingUICtrlWrapSink;

	//meeting_audio_ctrl_cb
	ZNativeSDKMeetingAudioWrapSink m_meetingAudioCtrlWrapSink;
	
	//meeting_video_ctrl_cb
	ZNativeSDKMeetingVideoWrapSink m_meetingVideoWrapSink;

	//meeting_recording_ctrl_cb
	ZNativeSDKMeetingRecordingWrapSink m_meetingRecordingWrapSink;

	//meeting_share_ctrl_cb
	ZNativeSDKMeetingShareWrapSink m_meetingShareWrapSink;

	//meeting_annotation_ctrl_cb
	ZNativeSDKMeetingAnnotationWrapSink m_meetingAnnotationWrapSink;

	//meeting_participants_ctrl_cb
	ZNativeSDKMeetingParticipantsWrapSink m_meetingParticipantsWrapSink;

	//meeting_h323_ctrl_cb
	ZNativeSDKMeetingH323WrapSink m_meetingH323WrapSink;

	//meeting_config_ctrl_cb
	ZNativeSDKMeetingConfigWrapFreemeetingSink m_meetingConfigWrapFreeMeetingSink;

	//direct_share_helper_cb
	ZNativeSDKDirectShareHelperWrapSink m_directShareHelperWrapSink;

	//sms_helper_cb
	ZNativeSDKSMSHelperWrapSink m_smsHelperWrapSink;

	//audio_setting_context_cb
	ZNativeSDKAudioSettingContextWrapSink m_audioSettingContextWrapSink;

	//video_setting_context_cb
	ZNativeSDKVideoSettingContextWrapSink m_videoSettingContextWrapSink;

	//recording_setting_context_cb
	ZNativeSDKRecordingSettingContextWrapSink m_recordingSettingContextWrapSink;

	//wallpaper_setting_context_cb
	ZNativeSDKWallpaperSettingContextWrapSink m_wallpaperSettingContextWrapSink;

	//virtual_bg_setting_context_cb
	ZNativeSDKVirtualBGSettingContextWrapSink m_virtualBGSettingContextWrapSink;

	//meeting_live_stream_ctrl_cb
	ZNativeSDKMeetingLiveStreamWrapSink m_meetingLiveStreamWrapSink;

	//meeting_reminder_ctrl_cb
	ZNativeSDKMeetingReminderWrapSink m_meetingReminderWrapSink;

	//meeting_chat_ctrl_cb
	ZNativeSDKMeetingChatWrapSink m_meetingChatWrapSink;

	//meeting_waiting_room_ctrl_cb
	ZNativeSDKMeetingWaitingRoomWrapSink m_meetingWaitingRoomWrapSink;

	//meeting_ai_companion_ctrl_cb
	ZNativeSDKMeetingAICompanionWrapSink m_meetingAICompanionWrapSink;

	//meeting_indicator_ctrl_cb
	ZNativeSDKMeetingIndicatorWrapSink m_meetingIndicatorWrapSink;

	//meeting_webinar_ctrl_cb
	ZNativeSDKMeetingWebinarWrapSink m_meetingWebinarWrapSink;

	//meeting_qa_ctrl_cb
	ZNativeSDKMeetingQAWrapSink m_meetingQAWrapSink;

	//meeting_reaction_ctrl_cb
	ZNativeSDKMeetingReactionWrapSink m_meetingReactionWrapSink;

	//meeting_close_caption_ctrl_cb
	ZNativeSDKCloseCaptionWrapSink m_closeCaptionWrapSink;

	//polling_ctrl_cb
	ZNativeSDKPollingWrapSink m_pollingWrapSink;

	//meeting_whiteboard_ctrl_cb
	ZNativeSDKMeetingWhiteboardWrapSink m_meetingWhiteboardWrapSink;

	//meeting_docs_ctrl_cb
	ZNativeSDKMeetingDocsWrapSink m_meetingDocsWrapSink;

	static ZoomNodeSinkWrapMgr& GetInst()
	{
		static ZoomNodeSinkWrapMgr inst;
		return inst;
	}
	~ZoomNodeSinkWrapMgr() {}
private:
	ZoomNodeSinkWrapMgr() {}
};
#endif // !_zoom_sinks_wrap_class_h_
