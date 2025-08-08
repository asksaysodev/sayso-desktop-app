#ifndef _ZOOM_NODE_SETTING_WALLPAPER_CTRL_H_
#define _ZOOM_NODE_SETTING_WALLPAPER_CTRL_H_
#include "zoom_node_common_include.h"
#include "zoom_native_sdk_wrap_core.h"
#include "zoom_singleton_wrap_class.h"

extern ZNativeSDKWrap _g_native_wrap;

class ZOOM_NODE_HIDE ZoomNodeSettingWallpaperCtrlWrap :
	public ZoomWrapObject<ZoomNodeSettingWallpaperCtrlWrap >
{
	friend class ZoomWrapObject<ZoomNodeSettingWallpaperCtrlWrap >;
private:
	ZoomNodeSettingWallpaperCtrlWrap();
	~ZoomNodeSettingWallpaperCtrlWrap();
public:
	static void SetOnMeetingWallpaperChangedCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void SetOnMeetingWallpaperImageDownloadStatusCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void SetOnPersonalWallpaperChangedCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void SetOnPersonalWallpaperImageDownloadStatusCB(const v8::FunctionCallbackInfo<v8::Value>& args);

	static void IsMeetingWallpaperEnabled(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void IsMeetingWallpaperThumbsReady(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void GetCurrentMeetingWallpaperItem(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void GetMeetingWallpaperList(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void SetMeetingWallpaper(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void GetMeetingWallpaperItemByID(const v8::FunctionCallbackInfo<v8::Value>& args);

	static void IsPersonalWallpaperEnabled(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void GetCurrentPersonalWallpaperItem(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void GetPersonalWallpaperList(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void SetPersonalWallpaper(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void GetPersonalWallpaperItemByID(const v8::FunctionCallbackInfo<v8::Value>& args);	
	
	static v8::Persistent<v8::Function> constructor;
};
template<>
static void InitClassAttribute<ZoomNodeSettingWallpaperCtrlWrap >(const v8::Local<v8::FunctionTemplate>& tpl, v8::Isolate* isolate)
{
	tpl->SetClassName(v8::String::NewFromUtf8(
		isolate, "ZoomNodeSettingWallpaperCtrlWrap", v8::NewStringType::kInternalized).ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnMeetingWallpaperChangedCB", ZoomNodeSettingWallpaperCtrlWrap::SetOnMeetingWallpaperChangedCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnMeetingWallpaperImageDownloadStatusCB", ZoomNodeSettingWallpaperCtrlWrap::SetOnMeetingWallpaperImageDownloadStatusCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnPersonalWallpaperChangedCB", ZoomNodeSettingWallpaperCtrlWrap::SetOnPersonalWallpaperChangedCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnPersonalWallpaperImageDownloadStatusCB", ZoomNodeSettingWallpaperCtrlWrap::SetOnPersonalWallpaperImageDownloadStatusCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "IsMeetingWallpaperEnabled", ZoomNodeSettingWallpaperCtrlWrap::IsMeetingWallpaperEnabled);
	NODE_SET_PROTOTYPE_METHOD(tpl, "IsMeetingWallpaperThumbsReady", ZoomNodeSettingWallpaperCtrlWrap::IsMeetingWallpaperThumbsReady);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetCurrentMeetingWallpaperItem", ZoomNodeSettingWallpaperCtrlWrap::GetCurrentMeetingWallpaperItem);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetMeetingWallpaperList", ZoomNodeSettingWallpaperCtrlWrap::GetMeetingWallpaperList);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetMeetingWallpaper", ZoomNodeSettingWallpaperCtrlWrap::SetMeetingWallpaper);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetMeetingWallpaperItemByID", ZoomNodeSettingWallpaperCtrlWrap::GetMeetingWallpaperItemByID);
	NODE_SET_PROTOTYPE_METHOD(tpl, "IsPersonalWallpaperEnabled", ZoomNodeSettingWallpaperCtrlWrap::IsPersonalWallpaperEnabled);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetCurrentPersonalWallpaperItem", ZoomNodeSettingWallpaperCtrlWrap::GetCurrentPersonalWallpaperItem);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetPersonalWallpaperList", ZoomNodeSettingWallpaperCtrlWrap::GetPersonalWallpaperList);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetPersonalWallpaper", ZoomNodeSettingWallpaperCtrlWrap::SetPersonalWallpaper);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetPersonalWallpaperItemByID", ZoomNodeSettingWallpaperCtrlWrap::GetPersonalWallpaperItemByID);
}
template<>
static v8::Persistent<v8::Function>* GetConstructor<ZoomNodeSettingWallpaperCtrlWrap >() {
	return &ZoomNodeSettingWallpaperCtrlWrap::constructor;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif