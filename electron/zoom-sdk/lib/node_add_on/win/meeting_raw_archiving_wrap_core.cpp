
#include "meeting_raw_archiving_wrap_core.h"
#include "sdk_wrap.h"
#include "meeting_service_components_wrap/meeting_raw_archiving_wrap.h"
#include "zoom_native_to_wrap.h"
#include "sdk_events_wrap_class.h"
extern ZOOM_SDK_NAMESPACE::IMeetingServiceWrap& g_meeting_service_wrap;

ZMeetingRawArchivingWrap::ZMeetingRawArchivingWrap()
{

}
ZMeetingRawArchivingWrap::~ZMeetingRawArchivingWrap()
{
	Uninit();
}
void ZMeetingRawArchivingWrap::Init()
{
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingRawArchivingController().Init_Wrap(&g_meeting_service_wrap);
}
void ZMeetingRawArchivingWrap::Uninit()
{
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingRawArchivingController().Uninit_Wrap();
}

ZNSDKError ZMeetingRawArchivingWrap::StartRawArchiving()
{
	return Map2WrapDefine(ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingRawArchivingController().StartRawArchiving());
}
ZNSDKError ZMeetingRawArchivingWrap::StopRawArchiving()
{
	return Map2WrapDefine(ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingRawArchivingController().StopRawArchiving());

}