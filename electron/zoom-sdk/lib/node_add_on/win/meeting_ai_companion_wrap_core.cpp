#include "meeting_ai_companion_wrap_core.h"
#include "sdk_wrap.h"
#include "meeting_service_components_wrap/meeting_ai_companion_wrap.h"
#include "zoom_native_to_wrap.h"
#include "sdk_events_wrap_class.h"

extern ZOOM_SDK_NAMESPACE::IMeetingServiceWrap& g_meeting_service_wrap;

ZMeetingAICompanionWrap::ZMeetingAICompanionWrap()
{
	SDKEventWrapMgr::GetInst().m_meetingAICompanionCtrlEvent.SetOwner(this);
	SDKEventWrapMgr::GetInst().m_meetingAICompanionSmartSummaryEvent.SetOwner(this);
	SDKEventWrapMgr::GetInst().m_meetingAICompanionQueryEvent.SetOwner(this);
	m_pSink = NULL;
}
ZMeetingAICompanionWrap::~ZMeetingAICompanionWrap()
{
	Uninit();
	m_pSink = NULL;
	SDKEventWrapMgr::GetInst().m_meetingAICompanionCtrlEvent.SetOwner(NULL);
	SDKEventWrapMgr::GetInst().m_meetingAICompanionSmartSummaryEvent.SetOwner(NULL);
	SDKEventWrapMgr::GetInst().m_meetingAICompanionQueryEvent.SetOwner(NULL);
}
void ZMeetingAICompanionWrap::Init()
{
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingAICompanionController().Init_Wrap(&g_meeting_service_wrap);
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingAICompanionController().SetEvent(&SDKEventWrapMgr::GetInst().m_meetingAICompanionCtrlEvent);
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingAICompanionController().GetMeetingAICompanionSmartSummaryHelper().SetEvent(&SDKEventWrapMgr::GetInst().m_meetingAICompanionSmartSummaryEvent);
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingAICompanionController().GetMeetingAICompanionQueryHelper().SetEvent(&SDKEventWrapMgr::GetInst().m_meetingAICompanionQueryEvent);
}
void ZMeetingAICompanionWrap::Uninit()
{
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingAICompanionController().SetEvent(NULL);
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingAICompanionController().GetMeetingAICompanionSmartSummaryHelper().SetEvent(NULL);
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingAICompanionController().GetMeetingAICompanionQueryHelper().SetEvent(NULL);
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingAICompanionController().Uninit_Wrap();
}
void ZMeetingAICompanionWrap::SetSink(ZNativeSDKMeetingAICompanionWrapSink* pSink)
{
	m_pSink = pSink;
}

// IMeetingAICompanionController
bool ZMeetingAICompanionWrap::IsTurnoffAllAICompanionsSupported()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingAICompanionController().IsTurnoffAllAICompanionsSupported();
}
bool ZMeetingAICompanionWrap::IsTurnOnAllAICompanionsSupported()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingAICompanionController().IsTurnOnAllAICompanionsSupported();
}
bool ZMeetingAICompanionWrap::CanTurnOffAllAICompanions()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingAICompanionController().CanTurnOffAllAICompanions();
}
bool ZMeetingAICompanionWrap::CanTurnOnAllAICompanions()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingAICompanionController().CanTurnOnAllAICompanions();
}
ZNSDKError ZMeetingAICompanionWrap::TurnOffAllAICompanions(bool bDeleteAssets)
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingAICompanionController().TurnOffAllAICompanions(bDeleteAssets);
	return Map2WrapDefine(sdkerr);
}
ZNSDKError ZMeetingAICompanionWrap::TurnOnAllAICompanions()
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingAICompanionController().TurnOnAllAICompanions();
	return Map2WrapDefine(sdkerr);
}
bool ZMeetingAICompanionWrap::CanRequestTurnoffAllAICompanions()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingAICompanionController().CanRequestTurnoffAllAICompanions();
}
bool ZMeetingAICompanionWrap::CanRequestTurnOnAllAICompanions()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingAICompanionController().CanRequestTurnOnAllAICompanions();
}
ZNSDKError ZMeetingAICompanionWrap::RequestTurnoffAllAICompanions()
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingAICompanionController().RequestTurnoffAllAICompanions();
	return Map2WrapDefine(sdkerr);
}
ZNSDKError ZMeetingAICompanionWrap::RequestTurnOnAllAICompanions()
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingAICompanionController().RequestTurnOnAllAICompanions();
	return Map2WrapDefine(sdkerr);
}

void ZMeetingAICompanionWrap::onAICompanionFeatureTurnOffByParticipant()
{
	if (m_pSink)
	{
		m_pSink->onAICompanionFeatureTurnOffByParticipant();
	}
}
ZNList<ZNAICompanionFeature> ZMeetingAICompanionWrap::GetFeatureList()
{
	return ZAICompanionFeatureTurnOnAgainHandler::GetInst().GetFeatureList();
}
ZNList<ZNAICompanionFeature> ZMeetingAICompanionWrap::GetAssetsDeletedFeatureList()
{
	return ZAICompanionFeatureTurnOnAgainHandler::GetInst().GetAssetsDeletedFeatureList();
}
ZNSDKError ZMeetingAICompanionWrap::TurnOnAgain()
{
	return ZAICompanionFeatureTurnOnAgainHandler::GetInst().TurnOnAgain();
}
ZNSDKError ZMeetingAICompanionWrap::AgreeTurnOff()
{
	return ZAICompanionFeatureTurnOnAgainHandler::GetInst().AgreeTurnOff();
}

void ZMeetingAICompanionWrap::onAICompanionFeatureSwitchRequested()
{
	if (m_pSink)
	{
		m_pSink->onAICompanionFeatureSwitchRequested();
	}
}
unsigned int ZMeetingAICompanionWrap::GetRequestUserID()
{
	return ZAICompanionFeatureSwitchHandler::GetInst().GetRequestUserID();
}
bool ZMeetingAICompanionWrap::IsTurnOn()
{
	return ZAICompanionFeatureSwitchHandler::GetInst().IsTurnOn();
}
ZNSDKError ZMeetingAICompanionWrap::Agree(bool deleteAssets)
{
	return ZAICompanionFeatureSwitchHandler::GetInst().Agree(deleteAssets);
}
ZNSDKError ZMeetingAICompanionWrap::Decline()
{
	return ZAICompanionFeatureSwitchHandler::GetInst().Decline();
}

void ZMeetingAICompanionWrap::onAICompanionFeatureSwitchRequestResponse(bool bTimeout, bool bAgree, bool bTurnOn)
{
	if (m_pSink)
	{
		m_pSink->onAICompanionFeatureSwitchRequestResponse(bTimeout, bAgree, bTurnOn);
	}
}
void ZMeetingAICompanionWrap::onAICompanionFeatureCanNotBeTurnedOff(ZNList<ZNAICompanionFeature> features)
{
	if (m_pSink)
	{
		m_pSink->onAICompanionFeatureCanNotBeTurnedOff(features);
	}
}

void ZMeetingAICompanionWrap::onSmartSummaryStateNotSupported()
{
	if (m_pSink)
	{
		m_pSink->onSmartSummaryStateNotSupported();
	}
}

void ZMeetingAICompanionWrap::onSmartSummaryStateSupportedButDisabled()
{
	if (m_pSink)
	{
		m_pSink->onSmartSummaryStateSupportedButDisabled();
	}
}
ZNSDKError ZMeetingAICompanionWrap::EnableSmartSummary()
{
	return ZMeetingEnableSmartSummaryHandler::GetInst().EnableSmartSummary();
}
bool ZMeetingAICompanionWrap::IsForRequestEnableSmartSummary()
{
	return ZMeetingEnableSmartSummaryHandler::GetInst().IsForRequestEnableSmartSummary();
}

void ZMeetingAICompanionWrap::onSmartSummaryStateEnabledButNotStarted()
{
	if (m_pSink)
	{
		m_pSink->onSmartSummaryStateEnabledButNotStarted();
	}
}
ZNSDKError ZMeetingAICompanionWrap::StartSmartSummary()
{
	return ZMeetingStartSmartSummaryHandler::GetInst().StartSmartSummary();
}
bool ZMeetingAICompanionWrap::IsForRequestStartSmartSummary()
{
	return ZMeetingStartSmartSummaryHandler::GetInst().IsForRequestStartSmartSummary();
}

void ZMeetingAICompanionWrap::onSmartSummaryStateStarted()
{
	if (m_pSink)
	{
		m_pSink->onSmartSummaryStateStarted();
	}
}
ZNSDKError ZMeetingAICompanionWrap::StopSmartSummary()
{
	return ZMeetingStopSmartSummaryHandler::GetInst().StopSmartSummary();
}

void ZMeetingAICompanionWrap::onFailedToStartSmartSummary(bool bTimeout)
{
	if (m_pSink)
	{
		m_pSink->onFailedToStartSmartSummary(bTimeout);
	}
}

void ZMeetingAICompanionWrap::onSmartSummaryEnableRequestReceived()
{
	if (m_pSink)
	{
		m_pSink->onSmartSummaryEnableRequestReceived();
	}
}
unsigned int ZMeetingAICompanionWrap::GetSenderEnableSmartSummaryUserID()
{
	return ZMeetingApproveEnableSmartSummaryHandler::GetInst().GetSenderEnableSmartSummaryUserID();
}
ZNSDKError ZMeetingAICompanionWrap::ContinueApproveEnableSmartSummary()
{
	return ZMeetingApproveEnableSmartSummaryHandler::GetInst().ContinueApproveEnableSmartSummary();
}

void ZMeetingAICompanionWrap::onSmartSummaryStartRequestReceived()
{
	if (m_pSink)
	{
		m_pSink->onSmartSummaryStartRequestReceived();
	}
}
unsigned int ZMeetingAICompanionWrap::GetSenderStartSmartSummaryUserID()
{
	return ZMeetingApproveStartSmartSummaryHandler::GetInst().GetSenderStartSmartSummaryUserID();
}
ZNSDKError ZMeetingAICompanionWrap::ApproveStartSmartSummary()
{
	return ZMeetingApproveStartSmartSummaryHandler::GetInst().ApproveStartSmartSummary();
}
ZNSDKError ZMeetingAICompanionWrap::DeclineStartSmartSummary()
{
	return ZMeetingApproveStartSmartSummaryHandler::GetInst().DeclineStartSmartSummary();
}

void ZMeetingAICompanionWrap::onSmartSummaryEnableActionCallback()
{
	if (m_pSink)
	{
		m_pSink->onSmartSummaryEnableActionCallback();
	}
}

ZoomSTRING ZMeetingAICompanionWrap::GetEnableSmartSummaryActionTipTitle()
{
	return ZMeetingEnableSmartSummaryActionHandler::GetInst().GetEnableSmartSummaryActionTipTitle();
}
ZoomSTRING ZMeetingAICompanionWrap::GetEnableSmartSummaryActionTipString()
{
	return ZMeetingEnableSmartSummaryActionHandler::GetInst().GetEnableSmartSummaryActionTipString();
}
ZNSDKError ZMeetingAICompanionWrap::ConfirmEnableSmartSummaryAction()
{
	return ZMeetingEnableSmartSummaryActionHandler::GetInst().ConfirmEnableSmartSummaryAction();
}
ZNSDKError ZMeetingAICompanionWrap::CancelEnableSmartSummaryAction()
{
	return ZMeetingEnableSmartSummaryActionHandler::GetInst().CancelEnableSmartSummaryAction();
}

// IMeetingAICompanionQueryHelper
ZNSDKError ZMeetingAICompanionWrap::CanChangeQuerySetting(bool& bCan)
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingAICompanionController().GetMeetingAICompanionQueryHelper().CanChangeQuerySetting(bCan);
	return Map2WrapDefine(sdkerr);
}
ZNSDKError ZMeetingAICompanionWrap::ChangeQuerySettings(ZNMeetingAICompanionQuerySettingOptions setting)
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingAICompanionController().GetMeetingAICompanionQueryHelper().ChangeQuerySettings(Map2SDKDefine(setting));
	return Map2WrapDefine(sdkerr);
}
ZNMeetingAICompanionQuerySettingOptions ZMeetingAICompanionWrap::GetSelectedQuerySetting()
{
	ZOOM_SDK_NAMESPACE::MeetingAICompanionQuerySettingOptions option = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingAICompanionController().GetMeetingAICompanionQueryHelper().GetSelectedQuerySetting();
	return Map2WrapDefine(option);
}
ZNSDKError ZMeetingAICompanionWrap::IsAICompanionQueryLegalNoticeAvailable(bool& bAvailable)
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingAICompanionController().GetMeetingAICompanionQueryHelper().IsAICompanionQueryLegalNoticeAvailable(bAvailable);
	return Map2WrapDefine(sdkerr);
}
ZoomSTRING ZMeetingAICompanionWrap::GetAICompanionQueryLegalNoticesPrompt()
{
	const zchar_t* prompt = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingAICompanionController().GetMeetingAICompanionQueryHelper().GetAICompanionQueryLegalNoticesPrompt();
	ZoomSTRING zn_prompt;
	if (prompt)
	{
		zn_prompt = prompt;
	}
	return zn_prompt;
}
ZoomSTRING ZMeetingAICompanionWrap::GetAICompanionQueryLegalNoticesExplained()
{
	const zchar_t* explained = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingAICompanionController().GetMeetingAICompanionQueryHelper().GetAICompanionQueryLegalNoticesExplained();
	ZoomSTRING zn_explained;
	if (explained)
	{
		zn_explained = explained;
	}
	return zn_explained;
}
ZNSDKError ZMeetingAICompanionWrap::IsAICompanionQueryPrivacyLegalNoticeAvailable(bool& bAvailable)
{
	ZOOM_SDK_NAMESPACE::SDKError sdkerr = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingAICompanionController().GetMeetingAICompanionQueryHelper().IsAICompanionQueryPrivacyLegalNoticeAvailable(bAvailable);
	return Map2WrapDefine(sdkerr);
}
ZoomSTRING ZMeetingAICompanionWrap::GetAICompanionQueryPrivacyLegalNoticesPrompt()
{
	const zchar_t* prompt = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingAICompanionController().GetMeetingAICompanionQueryHelper().GetAICompanionQueryPrivacyLegalNoticesPrompt();
	ZoomSTRING zn_prompt;
	if (prompt)
	{
		zn_prompt = prompt;
	}
	return zn_prompt;
}

void ZMeetingAICompanionWrap::onQueryStateNotSupported()
{
	if (m_pSink)
	{
		m_pSink->onQueryStateNotSupported();
	}
}

void ZMeetingAICompanionWrap::onQueryStateSupportedButDisabled()
{
	if (m_pSink)
	{
		m_pSink->onQueryStateSupportedButDisabled();
	}
}
ZNSDKError ZMeetingAICompanionWrap::EnableQuery()
{
	return ZMeetingEnableQueryHandler::GetInst().EnableQuery();
}
bool ZMeetingAICompanionWrap::IsForRequestEnableQuery()
{
	return ZMeetingEnableQueryHandler::GetInst().IsForRequestEnableQuery();
}

void ZMeetingAICompanionWrap::onQueryStateEnabledButNotStarted()
{
	if (m_pSink)
	{
		m_pSink->onQueryStateEnabledButNotStarted();
	}
}
ZNSDKError ZMeetingAICompanionWrap::StartMeetingQuery()
{
	return ZMeetingStartQueryHandler::GetInst().StartMeetingQuery();
}
bool ZMeetingAICompanionWrap::IsForRequestStartMeetingQuery()
{
	return ZMeetingStartQueryHandler::GetInst().IsForRequestStartMeetingQuery();
}

void ZMeetingAICompanionWrap::onQueryStateStarted()
{
	if (m_pSink)
	{
		m_pSink->onQueryStateStarted();
	}
}
ZNList<ZoomSTRING> ZMeetingAICompanionWrap::GetDefaultQueryQuestions()
{
	return ZMeetingSendQueryHandler::GetInst().GetDefaultQueryQuestions();
}
ZNSDKError ZMeetingAICompanionWrap::SendQueryQuestion(ZoomSTRING question)
{
	return ZMeetingSendQueryHandler::GetInst().SendQueryQuestion(question);
}
ZNSDKError ZMeetingAICompanionWrap::StopMeetingQuery()
{
	return ZMeetingSendQueryHandler::GetInst().StopMeetingQuery();
}
bool ZMeetingAICompanionWrap::CanSendQuery()
{
	return ZMeetingSendQueryHandler::GetInst().CanSendQuery();
}
ZNSDKError ZMeetingAICompanionWrap::RequestSendQueryPrivilege()
{
	return ZMeetingSendQueryHandler::GetInst().RequestSendQueryPrivilege();
}

void ZMeetingAICompanionWrap::onQuerySettingChanged(ZNMeetingAICompanionQuerySettingOptions eSetting)
{
	if (m_pSink)
	{
		m_pSink->onQuerySettingChanged(eSetting);
	}
}

void ZMeetingAICompanionWrap::onFailedToStartQuery(bool bTimeout)
{
	if (m_pSink)
	{
		m_pSink->onFailedToStartQuery(bTimeout);
	}
}

void ZMeetingAICompanionWrap::onReceiveRequestToEnableQuery()
{
	if (m_pSink)
	{
		m_pSink->onReceiveRequestToEnableQuery();
	}
}
unsigned int ZMeetingAICompanionWrap::GetSenderEnableQueryUserID()
{
	return ZMeetingApproveEnableQueryHandler::GetInst().GetSenderEnableQueryUserID();
}
ZNSDKError ZMeetingAICompanionWrap::ContinueApproveEnableQuery()
{
	return ZMeetingApproveEnableQueryHandler::GetInst().ContinueApproveEnableQuery();
}

void ZMeetingAICompanionWrap::onReceiveRequestToStartQuery()
{
	if (m_pSink)
	{
		m_pSink->onReceiveRequestToStartQuery();
	}
}
unsigned int ZMeetingAICompanionWrap::GetSenderStartQueryUserID()
{
	return ZMeetingApproveStartQueryHandler::GetInst().GetSenderStartQueryUserID();
}
ZNSDKError ZMeetingAICompanionWrap::ApproveStartQuery()
{
	return ZMeetingApproveStartQueryHandler::GetInst().ApproveStartQuery();
}
ZNSDKError ZMeetingAICompanionWrap::DeclineStartQuery()
{
	return ZMeetingApproveStartQueryHandler::GetInst().DeclineStartQuery();
}

void ZMeetingAICompanionWrap::onReceiveQueryAnswer(ZNMeetingAICompanionQueryItem queryItem)
{
	if (m_pSink)
	{
		m_pSink->onReceiveQueryAnswer(queryItem);
	}
}
ZNSDKError ZMeetingAICompanionWrap::AICompanionQueryFeedback(ZNMeetingAICompanionQueryFeedbackType feedbackType)
{
	return ZMeetingAICompanionQueryItemHandler::GetInst().AICompanionQueryFeedback(feedbackType);
}

void ZMeetingAICompanionWrap::onQueryEnableActionCallback()
{
	if (m_pSink)
	{
		m_pSink->onQueryEnableActionCallback();
	}
}
ZoomSTRING ZMeetingAICompanionWrap::GetEnableQueryActionTipTitle()
{
	return ZMeetingEnableQueryActionHandler::GetInst().GetEnableQueryActionTipTitle();
}
ZoomSTRING ZMeetingAICompanionWrap::GetEnableQueryActionTipString()
{
	return ZMeetingEnableQueryActionHandler::GetInst().GetEnableQueryActionTipString();
}
ZNSDKError ZMeetingAICompanionWrap::ConfirmEnableQueryAction()
{
	return ZMeetingEnableQueryActionHandler::GetInst().ConfirmEnableQueryAction();
}
ZNSDKError ZMeetingAICompanionWrap::CancelEnableQueryAction()
{
	return ZMeetingEnableQueryActionHandler::GetInst().CancelEnableQueryAction();
}

void ZMeetingAICompanionWrap::onSendQueryPrivilegeChanged(bool canSendQuery)
{
	if (m_pSink)
	{
		m_pSink->onSendQueryPrivilegeChanged(canSendQuery);
	}
}
void ZMeetingAICompanionWrap::onFailedToRequestSendQuery(bool bTimeout)
{
	if (m_pSink)
	{
		m_pSink->onFailedToRequestSendQuery(bTimeout);
	}
}

void ZMeetingAICompanionWrap::onReceiveRequestToSendQuery()
{
	if (m_pSink)
	{
		m_pSink->onReceiveRequestToSendQuery();
	}
}
unsigned int ZMeetingAICompanionWrap::GetSenderQueryUserID()
{
	return ZMeetingApproveSendQueryHandler::GetInst().GetSenderQueryUserID();
}
ZNSDKError ZMeetingAICompanionWrap::ApproveSendQuery()
{
	return ZMeetingApproveSendQueryHandler::GetInst().ApproveSendQuery();
}
ZNSDKError ZMeetingAICompanionWrap::DeclineSendQuery(bool bDeclineAll)
{
	return ZMeetingApproveSendQueryHandler::GetInst().DeclineSendQuery(bDeclineAll);
}