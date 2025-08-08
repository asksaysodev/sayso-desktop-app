#pragma once
#include "zoom_native_sdk_wrap_core_def.h"
#include "zoom_sinks_wrap_class.h"

class ZMeetingDocsWrap
{
public:
	ZMeetingDocsWrap();
	virtual ~ZMeetingDocsWrap();
	void Init();
	void Uninit();
	void SetSink(ZNativeSDKMeetingDocsWrapSink* pSink);

	bool IsSupportDocs();
	bool CanStartShareDocs(ZNCannotShareReasonType& reason);
	bool CanSetDocsOption();
	ZNSDKError SetDocsShareOption(ZNSDKDocsShareOption option);
	ZNSDKError GetDocsShareOption(ZNSDKDocsShareOption& option);
	ZNSDKError SetDocsCreateOption(ZNSDKDocsCreateOption option);
	ZNSDKError GetDocsCreateOption(ZNSDKDocsCreateOption& option);
	bool IsOtherSharingDocs();
	bool IsSharingDocsOut();
	ZNSDKError SetOwnerWindow(ZoomSTRING windowID);
	ZNSDKError ShowDocsDashboard(int x, int y);
	ZNSDKError SetDocDashboardSize(int width, int height);
	ZNSDKError SetDocsViewPos(int x, int y);
	ZNSDKError SetDocsViewSize(int width, int height);
	ZNSDKError ShowDocsViewWindow(unsigned int sourceID);
	ZNSDKError CloseDocsViewWindow();
	ZNList<ZNZoomSDKDocSharingSourceInfo> GetDocsSharingSourceInfoList(unsigned int userID);
	ZNList<unsigned int> GetViewableDocsSharingUserList();

	// callback
	void onDocsSharingSourceInfoChanged(ZNZoomSDKDocSharingSourceInfo zn_info);
	void onDocsPermissionChanged(ZNSDKDocsCreateOption createOption, ZNSDKDocsShareOption option);

private:
	ZNativeSDKMeetingDocsWrapSink* m_pSink;
};