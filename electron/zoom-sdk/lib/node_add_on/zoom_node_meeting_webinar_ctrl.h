#ifndef _ZOOM_NODE_MEETING_WEBINAR_CTRL_H_
#define _ZOOM_NODE_MEETING_WEBINAR_CTRL_H_
#include "zoom_node_common_include.h"
#include "zoom_native_sdk_wrap_core.h"
#include "zoom_singleton_wrap_class.h"

extern ZNativeSDKWrap _g_native_wrap;

class ZOOM_NODE_HIDE ZoomNodeMeetingWebinarCtrlWrap :
	public ZoomWrapObject<ZoomNodeMeetingWebinarCtrlWrap >
{
	friend class ZoomWrapObject<ZoomNodeMeetingWebinarCtrlWrap >;
private:
	ZoomNodeMeetingWebinarCtrlWrap();
	~ZoomNodeMeetingWebinarCtrlWrap();
public:
	/// \brief Promote the attendee to panelist. Available only for the meeting host.
	/// \param userid Specifies the user ID to promote.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	/// \remarks If the function succeeds, the user will receive the IMeetingWebinarCtrlEvent::onPromptAttendee2PanelistResult() callback event.
	static void PromptAttendee2Panelist(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Demote the panelist to attendee. Available only for the host.
	/// \param userid Specifies the user ID to demote.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	/// \remarks If the function succeeds, the user will receive the IMeetingWebinarCtrlEvent::onDepromptPanelist2AttendeeResult() callback event.
	static void DepromptPanelist2Attendee(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Query if the webinar supports the user to use the audio device.
	/// \return If it supports, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	static void IsSupportAttendeeTalk(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief The attendee is permitted to use the audio device.
	/// \param userid Specifies the permitted user ID.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	/// \remarks If the function succeeds, the user will receive the IMeetingWebinarCtrlEvent::onAllowAttendeeChatNotification() callback event. Available only for the host.
	static void AllowAttendeeTalk(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Forbid the attendee to use the audio device.
	/// \param userid Specifies the forbidden user ID.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	/// \remarks If the function succeeds, the user will receive the IMeetingWebinarCtrlEvent::onDisallowAttendeeChatNotification() callback event. Available only for the host.
	static void DisallowAttendeeTalk(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief The panelist is permitted to start the video.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	/// \remarks If the function succeeds, the user will receive the IMeetingWebinarCtrlEvent::onAllowPanelistStartVideoNotification() callback event. Available only for the host.
	static void AllowPanelistStartVideo(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Forbid the panelist to start video.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	/// \remarks If the function succeeds, the user will receive the IMeetingWebinarCtrlEvent::onDisallowPanelistStartVideoNotification() callback event. Available only for the host.
	static void DisallowPanelistStartVideo(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Permitted to use emoji reactions.
	/// \return If the function succeeds, the return value is SDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	/// \remarks If the function succeeds, the user will receive the IMeetingWebinarCtrlEvent::onAllowWebinarReactionStatusChanged(bool) callback event. Available only for the host.
	static void AllowWebinarEmojiReaction(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Forbid to use emoji reactions.
	/// \return If the function succeeds, the return value is SDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	/// \remarks If the function succeeds, the user will receive the IMeetingWebinarCtrlEvent::onAllowWebinarReactionStatusChanged(bool) callback event. Available only for the host.
	static void DisallowWebinarEmojiReaction(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Determine if current webinar support emoji reaction.
	/// \return True means the current webinar supports emoji reactions. Otherwise false.
	static void IsWebinarEmojiReactionSupported(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief The attendee is permitted to use the raise hand.
	/// \return If the function succeeds, the return value is SDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	/// \remarks If the function succeeds, the user will receive the IMeetingWebinarCtrlEvent::onAllowAttendeeRaiseHandStatusChanged(bool) callback event. Available only for the host.
	static void AllowAttendeeRaiseHand(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Forbid the attendee to use the raise hand.
	/// \return If the function succeeds, the return value is SDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	/// \remarks If the function succeeds, the user will receive the IMeetingWebinarCtrlEvent::onAllowAttendeeRaiseHandStatusChanged(bool) callback event. Available only for the host.
	static void DisallowAttendeeRaiseHand(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief The attendee is permitted to view the participant count.
	/// \return If the function succeeds, the return value is SDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	/// \remarks If the function succeeds, the user will receive the IMeetingWebinarCtrlEvent::onAllowAttendeeViewTheParticipantCountStatusChanged(bool) callback event. Available only for the host.
	static void AllowAttendeeViewTheParticipantCount(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Forbid the attendee to view the participant count.
	/// \return If the function succeeds, the return value is SDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	/// \remarks If the function succeeds, the user will receive the IMeetingWebinarCtrlEvent::onAllowAttendeeViewTheParticipantCountStatusChanged(bool) callback event. Available only for the host.
	static void DisallowAttendeeViewTheParticipantCount(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Get the participant count.
    /// \return The count of participant.
	static void GetParticipantCount(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Get the webinar status.
	/// \return The status of webinar. For more details, see \link WebinarMeetingStatus \endlink.
	static void GetWebinarMeetingStatus(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Set the chat privilege of the panelist.
	/// \param privilege The chat privilege of the panelist.
	/// \return If the function succeeds, the return value is SDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	static void SetPanelistChatPrivilege(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Get the chat privilege of the panelist.
	/// \param privilege The chat privilege of the panelist. It validates only when the return value is SDKErr_Success. 
	/// \return If the function succeeds, the return value is SDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	static void GetPanelistChatPrivilege(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Set the view mode of the attendee. Available only for zoom ui.
    /// \param mode The view mode of the attendee.
	/// \return If the function succeeds, the return value is SDKERR_SUCCESS..
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	static void SetAttendeeViewMode(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Get the view mode of the attendee.Available only for zoom ui.
    /// \return If the function succeeds, it will return the attendee's view mode. For more details, see \link ZoomSDKAttendeeViewMode \endlink.
	static void GetAttendeeViewMode(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Get the webinar legal notices prompt.
	/// \return The webinar legal notices prompt.
	static void GetWebinarLegalNoticesPrompt(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Get the webinar legal notices explained.
	/// \return The webinar legal notices explained.
	static void GetWebinarLegalNoticesExplained(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Set Callback to promote attendees to panelist.
	/// \param result If the promotion is successful, the result is zero(0). Otherwise it is an error code.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnPromptAttendee2PanelistResultCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Set Callback to demote attendees to panelist.
	/// \param result If the demotion is successful, the result is zero(0), otherwise an error code.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnDepromptPanelist2AttendeeResultCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Set Callback to enable the panelist to start the video.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnAllowPanelistStartVideoNotificationCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Set Callback to disable the panelist to start the video.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnDisallowPanelistStartVideoNotificationCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Set Callback event that attendees are required to enable the mic in the view-only mode of webinar.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnSelfAllowTalkNotificationCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Set Callback event that attendees are required to turn off the mic in the view-only mode of webinar.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnSelfDisallowTalkNotificationCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Set Callback to enable the attendees to chat. Available only for the host and the co-host.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnAllowAttendeeChatNotificationCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Set Callback to disable the attendees to chat. Available only for the host and the co-host.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnDisallowAttendeeChatNotificationCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Set Callback to emoji Reactions Status Changed.
	/// \param can_reaction If the Reaction is allow, the result is true, False not.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnAllowWebinarReactionStatusChangedCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Set Callback to attendee raise hand Status Changed.
	/// \param can_raiseHand If the raise hand is allow, the result is true, False not.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnAllowAttendeeRaiseHandStatusChangedCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Set Callback to attendee view the participant count status Changed.
	/// \param can_viewParticipantCount If attendee view the participant count is allow, the result is true, False not.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnAllowAttendeeViewTheParticipantCountStatusChangedCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Set Attendee will receive this callback if his audio status changes callback.
	/// \param userid The ID of the user whose audio status changes.
	/// \param can_talk True indicates that it is able to use the audio. False not.  
	/// \param is_muted TRUE indicates muted, FALSE not. This parameter works only when the value of can_talk is TRUE.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnAttendeeAudioStatusNotificationCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Set When attendee agree or decline the promote invitation, host will receive this callback.
	/// \param agree, if attendee agree return true, otherwise false.
	/// \param userid, The attendee user id.
	/// \return If the function succeeds, the return value is ZNSDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link ZNSDKError \endlink enum.
	static void SetOnAttendeePromoteConfirmResultCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	
	static v8::Persistent<v8::Function> constructor;
};
template<>
void InitClassAttribute<ZoomNodeMeetingWebinarCtrlWrap >(const v8::Local<v8::FunctionTemplate>& tpl, v8::Isolate* isolate)
{
	tpl->SetClassName(v8::String::NewFromUtf8(
		isolate, "ZoomNodeMeetingWebinarCtrlWrap", v8::NewStringType::kInternalized).ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	NODE_SET_PROTOTYPE_METHOD(tpl, "PromptAttendee2Panelist", ZoomNodeMeetingWebinarCtrlWrap::PromptAttendee2Panelist);
	NODE_SET_PROTOTYPE_METHOD(tpl, "DepromptPanelist2Attendee", ZoomNodeMeetingWebinarCtrlWrap::DepromptPanelist2Attendee);
	NODE_SET_PROTOTYPE_METHOD(tpl, "IsSupportAttendeeTalk", ZoomNodeMeetingWebinarCtrlWrap::IsSupportAttendeeTalk);
	NODE_SET_PROTOTYPE_METHOD(tpl, "AllowAttendeeTalk", ZoomNodeMeetingWebinarCtrlWrap::AllowAttendeeTalk);
	NODE_SET_PROTOTYPE_METHOD(tpl, "DisallowAttendeeTalk", ZoomNodeMeetingWebinarCtrlWrap::DisallowAttendeeTalk);
	NODE_SET_PROTOTYPE_METHOD(tpl, "AllowPanelistStartVideo", ZoomNodeMeetingWebinarCtrlWrap::AllowPanelistStartVideo);
	NODE_SET_PROTOTYPE_METHOD(tpl, "DisallowPanelistStartVideo", ZoomNodeMeetingWebinarCtrlWrap::DisallowPanelistStartVideo);
	NODE_SET_PROTOTYPE_METHOD(tpl, "AllowWebinarEmojiReaction", ZoomNodeMeetingWebinarCtrlWrap::AllowWebinarEmojiReaction);
	NODE_SET_PROTOTYPE_METHOD(tpl, "DisallowWebinarEmojiReaction", ZoomNodeMeetingWebinarCtrlWrap::DisallowWebinarEmojiReaction);
	NODE_SET_PROTOTYPE_METHOD(tpl, "IsWebinarEmojiReactionSupported", ZoomNodeMeetingWebinarCtrlWrap::IsWebinarEmojiReactionSupported);
	NODE_SET_PROTOTYPE_METHOD(tpl, "AllowAttendeeRaiseHand", ZoomNodeMeetingWebinarCtrlWrap::AllowAttendeeRaiseHand);
	NODE_SET_PROTOTYPE_METHOD(tpl, "DisallowAttendeeRaiseHand", ZoomNodeMeetingWebinarCtrlWrap::DisallowAttendeeRaiseHand);
	NODE_SET_PROTOTYPE_METHOD(tpl, "AllowAttendeeViewTheParticipantCount", ZoomNodeMeetingWebinarCtrlWrap::AllowAttendeeViewTheParticipantCount);
	NODE_SET_PROTOTYPE_METHOD(tpl, "DisallowAttendeeViewTheParticipantCount", ZoomNodeMeetingWebinarCtrlWrap::DisallowAttendeeViewTheParticipantCount);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetParticipantCount", ZoomNodeMeetingWebinarCtrlWrap::GetParticipantCount);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetWebinarMeetingStatus", ZoomNodeMeetingWebinarCtrlWrap::GetWebinarMeetingStatus);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetPanelistChatPrivilege", ZoomNodeMeetingWebinarCtrlWrap::SetPanelistChatPrivilege);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetPanelistChatPrivilege", ZoomNodeMeetingWebinarCtrlWrap::GetPanelistChatPrivilege);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetAttendeeViewMode", ZoomNodeMeetingWebinarCtrlWrap::SetAttendeeViewMode);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetAttendeeViewMode", ZoomNodeMeetingWebinarCtrlWrap::GetAttendeeViewMode);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetWebinarLegalNoticesPrompt", ZoomNodeMeetingWebinarCtrlWrap::GetWebinarLegalNoticesPrompt);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetWebinarLegalNoticesExplained", ZoomNodeMeetingWebinarCtrlWrap::GetWebinarLegalNoticesExplained);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnPromptAttendee2PanelistResultCB", ZoomNodeMeetingWebinarCtrlWrap::SetOnPromptAttendee2PanelistResultCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnDepromptPanelist2AttendeeResultCB", ZoomNodeMeetingWebinarCtrlWrap::SetOnDepromptPanelist2AttendeeResultCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnAllowPanelistStartVideoNotificationCB", ZoomNodeMeetingWebinarCtrlWrap::SetOnAllowPanelistStartVideoNotificationCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnDisallowPanelistStartVideoNotificationCB", ZoomNodeMeetingWebinarCtrlWrap::SetOnDisallowPanelistStartVideoNotificationCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnSelfAllowTalkNotificationCB", ZoomNodeMeetingWebinarCtrlWrap::SetOnSelfAllowTalkNotificationCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnSelfDisallowTalkNotificationCB", ZoomNodeMeetingWebinarCtrlWrap::SetOnSelfDisallowTalkNotificationCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnAllowAttendeeChatNotificationCB", ZoomNodeMeetingWebinarCtrlWrap::SetOnAllowAttendeeChatNotificationCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnDisallowAttendeeChatNotificationCB", ZoomNodeMeetingWebinarCtrlWrap::SetOnDisallowAttendeeChatNotificationCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnAllowWebinarReactionStatusChangedCB", ZoomNodeMeetingWebinarCtrlWrap::SetOnAllowWebinarReactionStatusChangedCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnAllowAttendeeRaiseHandStatusChangedCB", ZoomNodeMeetingWebinarCtrlWrap::SetOnAllowAttendeeRaiseHandStatusChangedCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnAllowAttendeeViewTheParticipantCountStatusChangedCB", ZoomNodeMeetingWebinarCtrlWrap::SetOnAllowAttendeeViewTheParticipantCountStatusChangedCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnAttendeeAudioStatusNotificationCB", ZoomNodeMeetingWebinarCtrlWrap::SetOnAttendeeAudioStatusNotificationCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnAttendeePromoteConfirmResultCB", ZoomNodeMeetingWebinarCtrlWrap::SetOnAttendeePromoteConfirmResultCB);
}
template<>
v8::Persistent<v8::Function>* GetConstructor<ZoomNodeMeetingWebinarCtrlWrap >() {
	return &ZoomNodeMeetingWebinarCtrlWrap::constructor;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif
