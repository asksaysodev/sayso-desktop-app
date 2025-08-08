#ifndef _ZOOM_NODE_WRAP_UTIL_H_
#define _ZOOM_NODE_WRAP_UTIL_H_
#include "zoom_node_common_include.h"
#include "zoom_native_sdk_wrap_core_def.h"

class NodeWrapUtil {
public:
	// convert ZNUserInfomation to v8::Object
	static v8::Local<v8::Object> CreateUserInfoObject(v8::Isolate* isolate, const ZNUserInfomation& zn_userInfo);
	// convert ZNWallpaperItem to v8::Object
	static v8::Local<v8::Object> CreateWallpaperItemObject(v8::Isolate* isolate, const ZNWallpaperItem& zn_item);
	// convert ZNZoomSDKSharingSourceInfo to v8::Object
	static v8::Local<v8::Object> CreateSharingSourceInfoObject(v8::Isolate* isolate, const ZNZoomSDKSharingSourceInfo& zn_shareInfo);
	// convert ZNChatMsgInfo to v8::Object
	static v8::Local<v8::Object> CreateChatMsgInfoObject(v8::Isolate* isolate, const ZNChatMsgInfo& zn_chatMsg);
	// convert ZNVirtualBGImageInfo to v8::Object
	static v8::Local<v8::Array> CreateVirtualBGImageInfoObjectList(v8::Isolate* isolate, ZNList<ZNVirtualBGImageInfo>& zn_list);
	// convert ZNZoomSDKDocSharingSourceInfo to v8::Object
	static v8::Local<v8::Object> CreateZoomSDKDocSharingSourceInfoObject(v8::Isolate* isolate, const ZNZoomSDKDocSharingSourceInfo& zn_sourceinfo);

	// convert std::string to v8::String
	static v8::Local<v8::String> ToV8String(v8::Isolate* isolate, const std::string& str);
};

#endif