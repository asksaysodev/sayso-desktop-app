#ifndef _ZOOM_NODE_SETTING_VIRTUAL_BG_CTRL_H_
#define _ZOOM_NODE_SETTING_VIRTUAL_BG_CTRL_H_
#include "zoom_node_common_include.h"
#include "zoom_native_sdk_wrap_core.h"
#include "zoom_singleton_wrap_class.h"

extern ZNativeSDKWrap _g_native_wrap;

class ZOOM_NODE_HIDE ZoomNodeSettingVirtualBGCtrlWrap :
	public ZoomWrapObject<ZoomNodeSettingVirtualBGCtrlWrap >
{
	friend class ZoomWrapObject<ZoomNodeSettingVirtualBGCtrlWrap >;
private:
	ZoomNodeSettingVirtualBGCtrlWrap();
	~ZoomNodeSettingVirtualBGCtrlWrap();
public:
	/// \brief Determine if the virtual background feature is supported by the meeting.
	/// \return TRUE indicates that the meeting supports the virtual background feature.
	static void IsSupportVirtualBG(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Determine if the smart virtual background feature can be supported by the machine.
	/// \return TRUE indicates that the machine can supports to use smart virtual background feature.
	static void IsDeviceSupportSmartVirtualBG(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Determine if the video virtual background feature is supported by the meeting.
	/// \return TRUE indicates that the meeting supports the video virtual background feature.
	static void IsSupportVirtualBackgroundVideo(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Determine if the smart virtual background video feature can be supported by the machine.
	/// \return TRUE indicates that the machine can supports to use smart virtual background video feature.
	static void IsDeviceSupportSmartVirtualBackgroundVideo(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Determine if the green virtual background video feature can be supported by the machine.
	/// \return TRUE indicates that the machine can supports to use green virtual background video feature.
	static void IsDeviceSupportGreenVirtualBackgroundVideo(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Determine if the green screen is using for the virtual background feature in the meeting.
	/// \return TRUE indicates to use the green screen for the virtual background feature.
	static void IsUsingGreenScreenOn(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Set to use the green screen for the virtual background feature.
	/// \param bUse Specify to use the green screen or not.TRUE means using the green screen. FALSE means using smart virtual background feature.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	///\remarks If the machine can not support smart virtual background feature, Calling of this interface with parameter 'FALSE'will return SDKERR_WRONG_USAGE.
	static void SetUsingGreenScreen(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Determine if the adding new virtual background item feature is supported by the meeting
	/// \return TRUE indicates that the meeting supports adding new virtual background item feature.
	static void IsAllowToAddNewVBItem(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Determine if the removing virtual background item feature is supported by the meeting
	/// \return TRUE indicates that the meeting supports removing virtual background item feature.
	/// \deprecated This interface is marked as deprecated.
	static void IsAllowToRemoveVBItem(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Add a new image as the virtual background image and to the image list.
	/// \param filePath Specify the file name of the image. It must be the full path with the file name.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	static void AddBGImage(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Remove an image from the virtual background image list.
	/// \param pRemoveImage Specify the image to remove. To get extended error information, see \link IVirtualBGImageInfo \endlink enum.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	static void RemoveBGImage(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Get the list of the virtual background images.
	/// \return If there are images in the list, the return value is a list of the poiters to IVirtualBGImageInfo.
	///Otherwise return nullptr. To get extended error information, see \link IVirtualBGImageInfo \endlink enum.
	static void GetBGImageList(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Specify an image to be the virtual background image.
	/// \param pImage Specify the image to use. To get extended error information, see \link IVirtualBGImageInfo \endlink enum.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	static void UseBGImage(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Get the selected color after called BeginSelectReplaceVBColor() and selected a color.
	/// \return If the function succeeds, the return value is the selected color.
	///Otherwise 0xFF000000. The value is the same one as the callback IVirtualBGSettingContextEvent.onGreenVBDidUpdateWithReplaceColor() does.
	static void GetBGReplaceColor(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Start to capture a color from video preview.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	static void BeginSelectReplaceVBColor(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Add a new video as the virtual background video and to the video list.
	/// \param filePath Specify the file name of the video. It must be the full path with the file name.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	static void AddBGVideo(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Remove a video from the virtual background video list.
	/// \param pRemoveVideo Specify the video to remove. To get extended error information, see \link IVirtualBGImageInfo \endlink enum.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	static void RemoveBGVideo(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Get the list of the virtual background videoes.
	/// \return If there are videoes in the list, the return value is a list of the poiters to IVirtualBGImageInfo.
	///Otherwise return nullptr. To get extended error information, see \link IVirtualBGImageInfo \endlink enum.
	static void GetBGVideoList(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Specify a video to be the virtual background video.
	/// \param pVideo Specify the video to use. To get extended error information, see \link IVirtualBGImageInfo \endlink enum.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	static void UseBGVideo(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Callback event of notification that the default virtual background images supplied by ZOOM are downloaded.
	static void SetOnVBImageDidDownloadedCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Callback event of notification that the virtual background effect is updated with the selected color.
	/// \param selectedColor The RGB value of the selected color, organized in the format 0xFFRRGGBB.
	static void SetOnGreenVBDidUpdateWithReplaceColorCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Callback event of notification that the virtual background image is changed.
	static void SetOnSelectedVBImageChangedCB(const v8::FunctionCallbackInfo<v8::Value>& args);
	/// \brief Callback event of notification that the result of adding video virtual background.
	/// \param filePath The file name with full path which you can use to generate your thumb for the virtual background video.
	static void SetOnVBVideoUploadedResultCB(const v8::FunctionCallbackInfo<v8::Value>& args);

	static v8::Persistent<v8::Function> constructor;
};
template<>
void InitClassAttribute<ZoomNodeSettingVirtualBGCtrlWrap>(const v8::Local<v8::FunctionTemplate>& tpl, v8::Isolate* isolate)
{
	tpl->SetClassName(v8::String::NewFromUtf8(
		isolate, "ZoomNodeSettingVirtualBGCtrlWrap", v8::NewStringType::kInternalized).ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	NODE_SET_PROTOTYPE_METHOD(tpl, "IsSupportVirtualBG", ZoomNodeSettingVirtualBGCtrlWrap::IsSupportVirtualBG);
	NODE_SET_PROTOTYPE_METHOD(tpl, "IsDeviceSupportSmartVirtualBG", ZoomNodeSettingVirtualBGCtrlWrap::IsDeviceSupportSmartVirtualBG);
	NODE_SET_PROTOTYPE_METHOD(tpl, "IsSupportVirtualBackgroundVideo", ZoomNodeSettingVirtualBGCtrlWrap::IsSupportVirtualBackgroundVideo);
	NODE_SET_PROTOTYPE_METHOD(tpl, "IsDeviceSupportSmartVirtualBackgroundVideo", ZoomNodeSettingVirtualBGCtrlWrap::IsDeviceSupportSmartVirtualBackgroundVideo);
	NODE_SET_PROTOTYPE_METHOD(tpl, "IsDeviceSupportGreenVirtualBackgroundVideo", ZoomNodeSettingVirtualBGCtrlWrap::IsDeviceSupportGreenVirtualBackgroundVideo);
	NODE_SET_PROTOTYPE_METHOD(tpl, "IsUsingGreenScreenOn", ZoomNodeSettingVirtualBGCtrlWrap::IsUsingGreenScreenOn);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetUsingGreenScreen", ZoomNodeSettingVirtualBGCtrlWrap::SetUsingGreenScreen);
	NODE_SET_PROTOTYPE_METHOD(tpl, "IsAllowToAddNewVBItem", ZoomNodeSettingVirtualBGCtrlWrap::IsAllowToAddNewVBItem);
	NODE_SET_PROTOTYPE_METHOD(tpl, "IsAllowToRemoveVBItem", ZoomNodeSettingVirtualBGCtrlWrap::IsAllowToRemoveVBItem);
	NODE_SET_PROTOTYPE_METHOD(tpl, "AddBGImage", ZoomNodeSettingVirtualBGCtrlWrap::AddBGImage);
	NODE_SET_PROTOTYPE_METHOD(tpl, "RemoveBGImage", ZoomNodeSettingVirtualBGCtrlWrap::RemoveBGImage);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetBGImageList", ZoomNodeSettingVirtualBGCtrlWrap::GetBGImageList);
	NODE_SET_PROTOTYPE_METHOD(tpl, "UseBGImage", ZoomNodeSettingVirtualBGCtrlWrap::UseBGImage);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetBGReplaceColor", ZoomNodeSettingVirtualBGCtrlWrap::GetBGReplaceColor);
	NODE_SET_PROTOTYPE_METHOD(tpl, "BeginSelectReplaceVBColor", ZoomNodeSettingVirtualBGCtrlWrap::BeginSelectReplaceVBColor);
	NODE_SET_PROTOTYPE_METHOD(tpl, "AddBGVideo", ZoomNodeSettingVirtualBGCtrlWrap::AddBGVideo);
	NODE_SET_PROTOTYPE_METHOD(tpl, "RemoveBGVideo", ZoomNodeSettingVirtualBGCtrlWrap::RemoveBGVideo);
	NODE_SET_PROTOTYPE_METHOD(tpl, "GetBGVideoList", ZoomNodeSettingVirtualBGCtrlWrap::GetBGVideoList);
	NODE_SET_PROTOTYPE_METHOD(tpl, "UseBGVideo", ZoomNodeSettingVirtualBGCtrlWrap::UseBGVideo);

	// Event Callbacks
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnVBImageDidDownloadedCB", ZoomNodeSettingVirtualBGCtrlWrap::SetOnVBImageDidDownloadedCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnGreenVBDidUpdateWithReplaceColorCB", ZoomNodeSettingVirtualBGCtrlWrap::SetOnGreenVBDidUpdateWithReplaceColorCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnSelectedVBImageChangedCB", ZoomNodeSettingVirtualBGCtrlWrap::SetOnSelectedVBImageChangedCB);
	NODE_SET_PROTOTYPE_METHOD(tpl, "SetOnVBVideoUploadedResultCB", ZoomNodeSettingVirtualBGCtrlWrap::SetOnVBVideoUploadedResultCB);
}
template<>
v8::Persistent<v8::Function>* GetConstructor<ZoomNodeSettingVirtualBGCtrlWrap >() {
	return &ZoomNodeSettingVirtualBGCtrlWrap::constructor;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
