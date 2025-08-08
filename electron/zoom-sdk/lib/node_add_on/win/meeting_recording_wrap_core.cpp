
#include "meeting_recording_wrap_core.h"
#include "sdk_wrap.h"
#include "meeting_service_components_wrap/meeting_recording_wrap.h"
#include "zoom_native_to_wrap.h"
#include "sdk_events_wrap_class.h"
extern ZOOM_SDK_NAMESPACE::IMeetingServiceWrap& g_meeting_service_wrap;

ZMeetingRecordingWrap::ZMeetingRecordingWrap()
{
	SDKEventWrapMgr::GetInst().m_meetingRecordingCtrlWrapEvent.SetOwner(this);
	m_pSink = NULL;
}
ZMeetingRecordingWrap::~ZMeetingRecordingWrap()
{
	Uninit();
	m_pSink = NULL;
	SDKEventWrapMgr::GetInst().m_meetingRecordingCtrlWrapEvent.SetOwner(NULL);
}
void ZMeetingRecordingWrap::Init()
{
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingRecordingController().Init_Wrap(&g_meeting_service_wrap);
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingRecordingController().SetEvent(&SDKEventWrapMgr::GetInst().m_meetingRecordingCtrlWrapEvent);
}
void ZMeetingRecordingWrap::Uninit()
{
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingRecordingController().SetEvent(NULL);
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingRecordingController().Uninit_Wrap();
}
void ZMeetingRecordingWrap::SetSink(ZNativeSDKMeetingRecordingWrapSink* pSink)
{
	m_pSink = pSink;
}
ZNSDKError ZMeetingRecordingWrap::StartRecording(time_t& startTimestamp)
{
	return Map2WrapDefine(ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingRecordingController().StartRecording(startTimestamp));
}
ZNSDKError ZMeetingRecordingWrap::StopRecording(time_t& stopTimestamp)
{
	return Map2WrapDefine(ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingRecordingController().StopRecording(stopTimestamp));
}
ZNSDKError ZMeetingRecordingWrap::CanStartRecording(bool cloud_recording, unsigned int userid)
{
	return Map2WrapDefine(ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingRecordingController().CanStartRecording(cloud_recording, userid));
}
ZNSDKError ZMeetingRecordingWrap::CanAllowDisAllowLocalRecording()
{
	return Map2WrapDefine(ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingRecordingController().CanAllowDisAllowLocalRecording());
}
ZNSDKError ZMeetingRecordingWrap::StartCloudRecording()
{
	return Map2WrapDefine(ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingRecordingController().StartCloudRecording());
}
ZNSDKError ZMeetingRecordingWrap::StopCloudRecording()
{
	return Map2WrapDefine(ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingRecordingController().StopCloudRecording());
}
ZNSDKError ZMeetingRecordingWrap::IsSupportLocalRecording(unsigned int userid)
{
	return Map2WrapDefine(ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingRecordingController().IsSupportLocalRecording(userid));
}
ZNSDKError ZMeetingRecordingWrap::AllowLocalRecording(unsigned int userid)
{
	return Map2WrapDefine(ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingRecordingController().AllowLocalRecording(userid));
}
ZNSDKError ZMeetingRecordingWrap::DisAllowLocalRecording(unsigned int userid)
{
	return Map2WrapDefine(ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingRecordingController().DisAllowLocalRecording(userid));
}
ZNSDKError ZMeetingRecordingWrap::RequestStartCloudRecording()
{
	return Map2WrapDefine(ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingRecordingController().RequestStartCloudRecording());
}
bool ZMeetingRecordingWrap::IsSmartRecordingEnabled()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingRecordingController().IsSmartRecordingEnabled();
}
bool ZMeetingRecordingWrap::CanEnableSmartRecordingFeature()
{
	return ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingRecordingController().CanEnableSmartRecordingFeature();
}
ZNSDKError ZMeetingRecordingWrap::EnableSmartRecording()
{
	return Map2WrapDefine(ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingRecordingController().EnableSmartRecording());
}
void ZMeetingRecordingWrap::onRecording2MP4Done(bool bsuccess, int iResult, ZoomSTRING szPath)
{
	if (m_pSink)
	{
		m_pSink->onRecording2MP4Done(bsuccess, iResult, szPath);
	}
}
void ZMeetingRecordingWrap::onRecording2MP4Processing(int iPercentage)
{
	if (m_pSink)
	{
		m_pSink->onRecording2MP4Processing(iPercentage);
	}
}
void ZMeetingRecordingWrap::onRecordingStatus(ZNRecordingStatus status)
{
	if (m_pSink)
	{
		m_pSink->onRecordingStatus(status);
	}
}
void ZMeetingRecordingWrap::onCloudRecordingStatus(ZNRecordingStatus status)
{
	if (m_pSink)
	{
		m_pSink->onCloudRecordingStatus(status);
	}
}

void ZMeetingRecordingWrap::onRecordPrivilegeChanged(bool bCanRec)
{
	if (m_pSink)
	{
		m_pSink->onRecordPrivilegeChanged(bCanRec);
	}
}

void ZMeetingRecordingWrap::onCloudRecordingStorageFull(time_t gracePeriodDate)
{
	if (m_pSink)
	{
		m_pSink->onCloudRecordingStorageFull(gracePeriodDate);
	}
}
void ZMeetingRecordingWrap::onRequestCloudRecordingResponse(ZNRequestStartCloudRecordingStatus status)
{
	if (m_pSink)
	{
		m_pSink->onRequestCloudRecordingResponse(status);
	}
}
void ZMeetingRecordingWrap::onStartCloudRecordingRequested()
{
	if (m_pSink)
	{
		m_pSink->onStartCloudRecordingRequested();
	}
}
void ZMeetingRecordingWrap::onEnableAndStartSmartRecordingRequested()
{
	if (m_pSink)
	{
		m_pSink->onEnableAndStartSmartRecordingRequested();
	}
}
void ZMeetingRecordingWrap::onSmartRecordingEnableActionCallback()
{
	if (m_pSink)
	{
		m_pSink->onSmartRecordingEnableActionCallback();
	}
}

unsigned int ZMeetingRecordingWrap::GetRequesterId()
{
	return ZRequestStartCloudRecordingHandler::GetInst().GetRequesterId();
}
ZoomSTRING ZMeetingRecordingWrap::GetRequesterName()
{
	return ZRequestStartCloudRecordingHandler::GetInst().GetRequesterName();
}
ZNSDKError ZMeetingRecordingWrap::Start()
{
	return ZRequestStartCloudRecordingHandler::GetInst().Start();
}
ZNSDKError ZMeetingRecordingWrap::Deny(bool bDenyAll)
{
	return ZRequestStartCloudRecordingHandler::GetInst().Deny(bDenyAll);
}

unsigned int ZMeetingRecordingWrap::GetRequestEnableAndStartSmartRecordingUserId()
{
	return ZRequestEnableAndStartSmartRecordingHandler::GetInst().GetRequestEnableAndStartSmartRecordingUserId();
}
ZoomSTRING ZMeetingRecordingWrap::GetRequestEnableAndStartSmartRecordingTipString()
{
	return ZRequestEnableAndStartSmartRecordingHandler::GetInst().GetRequestEnableAndStartSmartRecordingTipString();
}
ZNSDKError ZMeetingRecordingWrap::StartCloudRecordingWithoutEnableSmartRecording()
{
	return ZRequestEnableAndStartSmartRecordingHandler::GetInst().StartCloudRecordingWithoutEnableSmartRecording();
}
ZNSDKError ZMeetingRecordingWrap::AgreeToEnableAndStartSmartRecording(bool bAllMeetings)
{
	return ZRequestEnableAndStartSmartRecordingHandler::GetInst().AgreeToEnableAndStartSmartRecording(bAllMeetings);
}
ZNSDKError ZMeetingRecordingWrap::DeclineEnableAndStartSmartRecording(bool bDenyAll)
{
	return ZRequestEnableAndStartSmartRecordingHandler::GetInst().DeclineEnableAndStartSmartRecording(bDenyAll);
}


ZoomSTRING ZMeetingRecordingWrap::GetSmartRecordingEnableActionTipString()
{
	return ZSmartRecordingEnableActionHandler::GetInst().GetSmartRecordingEnableActionTipString();
}
ZNSDKError ZMeetingRecordingWrap::ActionConfirmEnableSmartRecording(bool bAllMeetings)
{
	return ZSmartRecordingEnableActionHandler::GetInst().ActionConfirmEnableSmartRecording(bAllMeetings);
}
ZNSDKError ZMeetingRecordingWrap::ActionCancelEnableSmartRecording()
{
	return ZSmartRecordingEnableActionHandler::GetInst().ActionCancelEnableSmartRecording();
}