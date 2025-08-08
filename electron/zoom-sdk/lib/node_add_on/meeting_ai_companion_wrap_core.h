#pragma once
#include "zoom_native_sdk_wrap_core_def.h"
#include "zoom_sinks_wrap_class.h"

class ZMeetingAICompanionWrap
{
public:
	ZMeetingAICompanionWrap();
	virtual ~ZMeetingAICompanionWrap();
	void Init();
	void Uninit();
	void SetSink(ZNativeSDKMeetingAICompanionWrapSink* pSink);

	// IMeetingAICompanionController
	bool IsTurnoffAllAICompanionsSupported();
	bool IsTurnOnAllAICompanionsSupported();
	bool CanTurnOffAllAICompanions();
	bool CanTurnOnAllAICompanions();
	ZNSDKError TurnOffAllAICompanions(bool deleteAssets);
	ZNSDKError TurnOnAllAICompanions();
	bool CanRequestTurnoffAllAICompanions();
	bool CanRequestTurnOnAllAICompanions();
	ZNSDKError RequestTurnoffAllAICompanions();
	ZNSDKError RequestTurnOnAllAICompanions();

	ZNList<ZNAICompanionFeature> GetFeatureList();
	ZNList<ZNAICompanionFeature> GetAssetsDeletedFeatureList();
	ZNSDKError TurnOnAgain();
	ZNSDKError AgreeTurnOff();

	unsigned int GetRequestUserID();
	bool IsTurnOn();
	ZNSDKError Agree(bool deleteAssets);
	ZNSDKError Decline();

	//callback
	void onAICompanionFeatureTurnOffByParticipant();
	void onAICompanionFeatureSwitchRequested();
	void onAICompanionFeatureSwitchRequestResponse(bool bTimeout, bool bAgree, bool bTurnOn);
	void onAICompanionFeatureCanNotBeTurnedOff(ZNList<ZNAICompanionFeature> features);

	// IMeetingAICompanionSmartSummaryHelper
	ZNSDKError EnableSmartSummary();
	bool IsForRequestEnableSmartSummary();

	ZNSDKError StartSmartSummary();
	bool IsForRequestStartSmartSummary();

	ZNSDKError StopSmartSummary();

	unsigned int GetSenderEnableSmartSummaryUserID();
	ZNSDKError ContinueApproveEnableSmartSummary();

	unsigned int GetSenderStartSmartSummaryUserID();
	ZNSDKError ApproveStartSmartSummary();
	ZNSDKError DeclineStartSmartSummary();

	ZoomSTRING GetEnableSmartSummaryActionTipTitle();
	ZoomSTRING GetEnableSmartSummaryActionTipString();
	ZNSDKError ConfirmEnableSmartSummaryAction();
	ZNSDKError CancelEnableSmartSummaryAction();

	//callback
	void onSmartSummaryStateNotSupported();
	void onSmartSummaryStateSupportedButDisabled();
	void onSmartSummaryStateEnabledButNotStarted();
	void onSmartSummaryStateStarted();
	void onFailedToStartSmartSummary(bool timeout);
	void onSmartSummaryEnableRequestReceived();
	void onSmartSummaryStartRequestReceived();
	void onSmartSummaryEnableActionCallback();


	// IMeetingAICompanionQueryHelper
	ZNSDKError CanChangeQuerySetting(bool& bCan);
	ZNSDKError ChangeQuerySettings(ZNMeetingAICompanionQuerySettingOptions setting);
	ZNMeetingAICompanionQuerySettingOptions GetSelectedQuerySetting();
	ZNSDKError IsAICompanionQueryLegalNoticeAvailable(bool& bAvailable);
	ZoomSTRING GetAICompanionQueryLegalNoticesPrompt();
	ZoomSTRING GetAICompanionQueryLegalNoticesExplained();
	ZNSDKError IsAICompanionQueryPrivacyLegalNoticeAvailable(bool& bAvailable);
	ZoomSTRING GetAICompanionQueryPrivacyLegalNoticesPrompt();

	ZNSDKError EnableQuery();
	bool IsForRequestEnableQuery();

	ZNSDKError StartMeetingQuery();
	bool IsForRequestStartMeetingQuery();

	ZNList<ZoomSTRING> GetDefaultQueryQuestions();
	ZNSDKError SendQueryQuestion(ZoomSTRING question);
	ZNSDKError StopMeetingQuery();
	bool CanSendQuery();
	ZNSDKError RequestSendQueryPrivilege();

	unsigned int GetSenderEnableQueryUserID();
	ZNSDKError ContinueApproveEnableQuery();

	unsigned int GetSenderStartQueryUserID();
	ZNSDKError ApproveStartQuery();
	ZNSDKError DeclineStartQuery();

	ZNSDKError AICompanionQueryFeedback(ZNMeetingAICompanionQueryFeedbackType feedbackType);

	ZoomSTRING GetEnableQueryActionTipTitle();
	ZoomSTRING GetEnableQueryActionTipString();
	ZNSDKError ConfirmEnableQueryAction();
	ZNSDKError CancelEnableQueryAction();

	unsigned int GetSenderQueryUserID();
	ZNSDKError ApproveSendQuery();
	ZNSDKError DeclineSendQuery(bool bDeclineAll);

	//callback
	void onQueryStateNotSupported();
	void onQueryStateSupportedButDisabled();
	void onQueryStateEnabledButNotStarted();
	void onQueryStateStarted();
	void onQuerySettingChanged(ZNMeetingAICompanionQuerySettingOptions eSetting);
	void onFailedToStartQuery(bool bTimeout);
	void onReceiveRequestToEnableQuery();
	void onReceiveRequestToStartQuery();
	void onReceiveQueryAnswer(ZNMeetingAICompanionQueryItem queryItem);
	void onQueryEnableActionCallback();
	void onSendQueryPrivilegeChanged(bool canSendQuery);
	void onFailedToRequestSendQuery(bool bTimeout);
	void onReceiveRequestToSendQuery();
	
private:
	ZNativeSDKMeetingAICompanionWrapSink* m_pSink;
};