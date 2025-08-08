#pragma once
#include "zoom_native_sdk_wrap_core_def.h"
#include "zoom_sinks_wrap_class.h"

class ZMeetingAnnotationWrap
{
public:
	ZMeetingAnnotationWrap();
	virtual ~ZMeetingAnnotationWrap();
	void Init();
	void Uninit();
	void SetSink(ZNativeSDKMeetingAnnotationWrapSink* pSink);

	//callback
	void onSupportAnnotationStatus(unsigned int nShareSourceID, bool bSupportAnnotation);

	bool IsAnnotationDisable();
	ZNSDKError StartAnnotation(ZNSDKViewType type, ZoomSTRING left, ZoomSTRING top);
	ZNSDKError StopAnnotation(ZNSDKViewType type);
	ZNSDKError SetTool(ZNSDKViewType viewType, ZNAnnotationToolType toolType);
	ZNSDKError Clear(ZNSDKViewType viewType, ZNAnnotationClearType toolType);
	ZNSDKError SetColor(ZNSDKViewType viewType, ZoomSTRING color);
	ZNSDKError SetLineWidth(ZNSDKViewType viewType, ZoomSTRING lineWidth);
	ZNSDKError Undo(ZNSDKViewType viewType);
	ZNSDKError Redo(ZNSDKViewType viewType);
	bool IsAnnotationLegalNoticeAvailable();
	ZoomSTRING GetAnnotationLegalNoticesPrompt();
	ZoomSTRING GetAnnotationLegalNoticesExplained();

private:
	ZNativeSDKMeetingAnnotationWrapSink* m_pSink;
};
