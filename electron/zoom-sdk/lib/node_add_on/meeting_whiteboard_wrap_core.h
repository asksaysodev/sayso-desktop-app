#pragma once
#include "zoom_native_sdk_wrap_core_def.h"
#include "zoom_sinks_wrap_class.h"

class ZMeetingWhiteboardWrap
{
public:
	ZMeetingWhiteboardWrap();
	virtual ~ZMeetingWhiteboardWrap();
	void Init();
	void Uninit();
	void SetSink(ZNativeSDKMeetingWhiteboardWrapSink* pSink);

	ZNSDKError SetOwnerWindow(ZoomSTRING windowID);
	bool IsSupportWhiteBoard();
	bool CanStartShareWhiteboard(ZNCannotShareReasonType& reason);
	ZNSDKError ShowDashboardView(int x, int y);
	ZNSDKError SetDashboardViewSize(int width, int height);
	ZNSDKError SetWhiteboardViewPos(int x, int y);
	ZNSDKError SetWhiteboardViewSize(int width, int height);
	ZNSDKError SetWhiteboardShareOption(ZNSDKWhiteboardShareOption option);
	ZNSDKError GetWhiteboardShareOption(ZNSDKWhiteboardShareOption& option);
	ZNSDKError SetWhiteboardCreateOption(ZNSDKWhiteboardCreateOption option);
	ZNSDKError GetWhiteboardCreateOption(ZNSDKWhiteboardCreateOption& option);
	ZNSDKError EnableParticipantsCreateWithoutHost(bool bEnable);
	bool IsParticipantsCreateWithoutHostEnabled();
	bool IsOtherSharingWhiteboard();
	bool IsSharingWhiteboardOut();

	//callback
	void onWhiteboardStatusChanged(ZNSDKWhiteboardStatus status);
	void onWhiteboardSettingsChanged(ZNSDKWhiteboardShareOption shareOption, ZNSDKWhiteboardCreateOption createOption, bool bEnable);
	
private:
	ZNativeSDKMeetingWhiteboardWrapSink* m_pSink;
};