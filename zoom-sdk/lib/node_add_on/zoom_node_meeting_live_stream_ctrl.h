#ifndef _ZOOM_NODE_MEETING_LIVE_STREAM_CTRL_H_
#define _ZOOM_NODE_MEETING_LIVE_STREAM_CTRL_H_
#include "zoom_node_common_include.h"
#include "zoom_native_sdk_wrap_core.h"
#include "zoom_singleton_wrap_class.h"

extern ZNativeSDKWrap _g_native_wrap;
class ZOOM_NODE_HIDE ZoomNodeMeetingLiveStreamCtrlWrap :
	public ZoomWrapObject<ZoomNodeMeetingLiveStreamCtrlWrap >
{
	friend class ZoomWrapObject<ZoomNodeMeetingLiveStreamCtrlWrap >;
private:
	ZoomNodeMeetingLiveStreamCtrlWrap();
	~ZoomNodeMeetingLiveStreamCtrlWrap();
public:

	static void CanStartLiveStream(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void StartLiveStream(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void StartLiveStreamWithSteamingURL(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void StopLiveStream(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void GetSupportLiveStreamItems(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void GetCurrentLiveStreamItem(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void GetCurrentLiveStreamStatus(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void StopRawLiveStream(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void GetLiveStreamItemInfo(const v8::FunctionCallbackInfo<v8::Value>& args);

	static void IsRawLiveStreamSupported(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void CanStartRawLiveStream(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void RemoveRawLiveStreamPrivilege(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void GetRawLiveStreamingInfoList(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void GetRawLiveStreamPrivilegeUserList(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void IsLiveStreamReminderEnabled(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void CanEnableLiveStreamReminder(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void EnableLiveStreamReminder(const v8::FunctionCallbackInfo<v8::Value>& args);

	static void SetOnLiveStreamStatusChangeCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void SetOnRawLiveStreamPrivilegeChangedCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void SetOnUserRawLiveStreamPrivilegeChangedCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void SetOnRawLiveStreamPrivilegeRequestedCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void SetOnUserRawLiveStreamingStatusChangedCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void SetOnRawLiveStreamPrivilegeRequestTimeoutCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void SetOnLiveStreamReminderStatusChangedCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void SetOnLiveStreamReminderStatusChangeFailedCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void SetOnUserThresholdReachedForLiveStreamCB(const v8::FunctionCallbackInfo<v8::Value>& args);

	static v8::Persistent<v8::Function> constructor;
};
template<>
static void InitClassAttribute<ZoomNodeMeetingLiveStreamCtrlWrap >(const v8::Local<v8::FunctionTemplate>& tpl, v8::Isolate* isolate)
{
	tpl->SetClassName(v8::String::NewFromUtf8(
		isolate, "ZoomNodeMeetingLiveStreamCtrlWrap", v8::NewStringType::kInternalized).ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	NODE_SET_PROTOTYPE_METHOD(tpl, "CanStartLiveStream", ZoomNodeMeetingLiveStreamCtrlWrap::CanStartLiveStream);
	NODE_SET_PROTOTYPE_METHOD(tpl, "StartLiveStream", ZoomNodeMeetingLiveStreamCtrlWrap::StartLiveStream);
	NODE_SET_PROTOTYPE_METHOD(tpl, "StartLiveStreamWithSteamingURL", ZoomNodeMeetingLiveStreamCtrlWrap::StartLiveStreamWithSteamingURL);
	NODE_SET_PROTOTYPE_METHOD(tpl, "StopLiveStream", ZoomNodeMeetingLiveStreamCtrlWrap::StopLiveStream);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetSupportLiveStreamItems", ZoomNodeMeetingLiveStreamCtrlWrap::GetSupportLiveStreamItems);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetCurrentLiveStreamItem", ZoomNodeMeetingLiveStreamCtrlWrap::GetCurrentLiveStreamItem);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetCurrentLiveStreamStatus", ZoomNodeMeetingLiveStreamCtrlWrap::GetCurrentLiveStreamStatus);
	NODE_SET_PROTOTYPE_METHOD(tpl, "StopRawLiveStream", ZoomNodeMeetingLiveStreamCtrlWrap::StopRawLiveStream);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetLiveStreamItemInfo", ZoomNodeMeetingLiveStreamCtrlWrap::GetLiveStreamItemInfo);

	NODE_SET_PROTOTYPE_METHOD(tpl, "IsRawLiveStreamSupported", ZoomNodeMeetingLiveStreamCtrlWrap::IsRawLiveStreamSupported);
	NODE_SET_PROTOTYPE_METHOD(tpl, "CanStartRawLiveStream", ZoomNodeMeetingLiveStreamCtrlWrap::CanStartRawLiveStream);
	NODE_SET_PROTOTYPE_METHOD(tpl, "RemoveRawLiveStreamPrivilege", ZoomNodeMeetingLiveStreamCtrlWrap::RemoveRawLiveStreamPrivilege);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetRawLiveStreamingInfoList", ZoomNodeMeetingLiveStreamCtrlWrap::GetRawLiveStreamingInfoList);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetRawLiveStreamPrivilegeUserList", ZoomNodeMeetingLiveStreamCtrlWrap::GetRawLiveStreamPrivilegeUserList);
	NODE_SET_PROTOTYPE_METHOD(tpl, "IsLiveStreamReminderEnabled", ZoomNodeMeetingLiveStreamCtrlWrap::IsLiveStreamReminderEnabled);
	NODE_SET_PROTOTYPE_METHOD(tpl, "CanEnableLiveStreamReminder", ZoomNodeMeetingLiveStreamCtrlWrap::CanEnableLiveStreamReminder);
	NODE_SET_PROTOTYPE_METHOD(tpl, "EnableLiveStreamReminder", ZoomNodeMeetingLiveStreamCtrlWrap::EnableLiveStreamReminder);

	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnLiveStreamStatusChangeCB", ZoomNodeMeetingLiveStreamCtrlWrap::SetOnLiveStreamStatusChangeCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnRawLiveStreamPrivilegeChangedCB", ZoomNodeMeetingLiveStreamCtrlWrap::SetOnRawLiveStreamPrivilegeChangedCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnUserRawLiveStreamPrivilegeChangedCB", ZoomNodeMeetingLiveStreamCtrlWrap::SetOnUserRawLiveStreamPrivilegeChangedCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnRawLiveStreamPrivilegeRequestedCB", ZoomNodeMeetingLiveStreamCtrlWrap::SetOnRawLiveStreamPrivilegeRequestedCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnUserRawLiveStreamingStatusChangedCB", ZoomNodeMeetingLiveStreamCtrlWrap::SetOnUserRawLiveStreamingStatusChangedCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnRawLiveStreamPrivilegeRequestTimeoutCB", ZoomNodeMeetingLiveStreamCtrlWrap::SetOnRawLiveStreamPrivilegeRequestTimeoutCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnLiveStreamReminderStatusChangedCB", ZoomNodeMeetingLiveStreamCtrlWrap::SetOnLiveStreamReminderStatusChangedCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnLiveStreamReminderStatusChangeFailedCB", ZoomNodeMeetingLiveStreamCtrlWrap::SetOnLiveStreamReminderStatusChangeFailedCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnUserThresholdReachedForLiveStreamCB", ZoomNodeMeetingLiveStreamCtrlWrap::SetOnUserThresholdReachedForLiveStreamCB);
}
template<>
static v8::Persistent<v8::Function>* GetConstructor<ZoomNodeMeetingLiveStreamCtrlWrap >() {
	return &ZoomNodeMeetingLiveStreamCtrlWrap::constructor;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ZOOM_NODE_HIDE ZoomNodeRequestRawLiveStreamPrivilegeHandlerWrap :
	public ZoomWrapObject<ZoomNodeRequestRawLiveStreamPrivilegeHandlerWrap >
{
	friend class ZoomWrapObject<ZoomNodeRequestRawLiveStreamPrivilegeHandlerWrap >;
private:
	ZoomNodeRequestRawLiveStreamPrivilegeHandlerWrap();
	~ZoomNodeRequestRawLiveStreamPrivilegeHandlerWrap();
public:

	static void GetRequestId(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void GetRequesterId(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void GetRequesterName(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void GetBroadcastUrl(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void GrantRawLiveStreamPrivilege(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void DenyRawLiveStreamPrivilege(const v8::FunctionCallbackInfo<v8::Value>& args);


	static v8::Persistent<v8::Function> constructor;
};
template<>
static void InitClassAttribute<ZoomNodeRequestRawLiveStreamPrivilegeHandlerWrap >(const v8::Local<v8::FunctionTemplate>& tpl, v8::Isolate* isolate)
{
	tpl->SetClassName(v8::String::NewFromUtf8(
		isolate, "ZoomNodeRequestRawLiveStreamPrivilegeHandlerWrap", v8::NewStringType::kInternalized).ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetRequestId", ZoomNodeRequestRawLiveStreamPrivilegeHandlerWrap::GetRequestId);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetRequesterId", ZoomNodeRequestRawLiveStreamPrivilegeHandlerWrap::GetRequesterId);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetRequesterName", ZoomNodeRequestRawLiveStreamPrivilegeHandlerWrap::GetRequesterName);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetBroadcastUrl", ZoomNodeRequestRawLiveStreamPrivilegeHandlerWrap::GetBroadcastUrl);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GrantRawLiveStreamPrivilege", ZoomNodeRequestRawLiveStreamPrivilegeHandlerWrap::GrantRawLiveStreamPrivilege);
	NODE_SET_PROTOTYPE_METHOD(tpl, "DenyRawLiveStreamPrivilege", ZoomNodeRequestRawLiveStreamPrivilegeHandlerWrap::DenyRawLiveStreamPrivilege);
}
template<>
static v8::Persistent<v8::Function>* GetConstructor<ZoomNodeRequestRawLiveStreamPrivilegeHandlerWrap >() {
	return &ZoomNodeRequestRawLiveStreamPrivilegeHandlerWrap::constructor;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
