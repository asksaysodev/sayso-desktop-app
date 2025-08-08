#pragma once
#include "zoom_native_sdk_wrap_core_def.h"
#include "zoom_sinks_wrap_class.h"

class ZMeetingWebinarWrap
{
public:
	ZMeetingWebinarWrap();
	virtual ~ZMeetingWebinarWrap();
	void Init();
	void Uninit();
	void SetSink(ZNativeSDKMeetingWebinarWrapSink* pSink);

	ZNSDKError PromptAttendee2Panelist(unsigned int userid);
	ZNSDKError DepromptPanelist2Attendee(unsigned int userid);
	bool IsSupportAttendeeTalk();
	ZNSDKError AllowAttendeeTalk(unsigned int userid);
	ZNSDKError DisallowAttendeeTalk(unsigned int userid);
	ZNSDKError AllowPanelistStartVideo();
	ZNSDKError DisallowPanelistStartVideo();
	ZNSDKError AllowWebinarEmojiReaction();
	ZNSDKError DisallowWebinarEmojiReaction();
	bool IsWebinarEmojiReactionSupported();
	ZNSDKError AllowAttendeeRaiseHand();
	ZNSDKError DisallowAttendeeRaiseHand();
	ZNSDKError AllowAttendeeViewTheParticipantCount();
	ZNSDKError DisallowAttendeeViewTheParticipantCount();
	int GetParticipantCount();
	ZNWebinarMeetingStatus GetWebinarMeetingStatus();
	ZNSDKError SetPanelistChatPrivilege(ZNSDKPanelistChatPrivilege privilege);
	ZNSDKError GetPanelistChatPrivilege(ZNSDKPanelistChatPrivilege& privilege);

	ZNSDKError SetAttendeeViewMode(ZNSDKAttendeeViewMode mode);
	ZNSDKAttendeeViewMode GetAttendeeViewMode();
	ZoomSTRING GetWebinarLegalNoticesPrompt();
	bool GetWebinarLegalNoticesExplained(ZNWebinarLegalNoticesExplainedInfo& zn_explained_info);

	//callback
	void onPromptAttendee2PanelistResult(int result);
	void onDepromptPanelist2AttendeeResult(int result);
	void onAllowPanelistStartVideoNotification();
	void onDisallowPanelistStartVideoNotification();
	void onSelfAllowTalkNotification();
	void onSelfDisallowTalkNotification();
	void onAllowAttendeeChatNotification();
	void onDisallowAttendeeChatNotification();
	void onAllowWebinarReactionStatusChanged(bool can_reaction);
	void onAllowAttendeeRaiseHandStatusChanged(bool can_raiseHand);
	void onAllowAttendeeViewTheParticipantCountStatusChanged(bool can_viewParticipantCount);
	void onAttendeeAudioStatusNotification(unsigned int userid, bool can_talk, bool is_muted);
	void onAttendeePromoteConfirmResult(bool agree, unsigned int userid);
	
private:
	ZNativeSDKMeetingWebinarWrapSink* m_pSink;
};