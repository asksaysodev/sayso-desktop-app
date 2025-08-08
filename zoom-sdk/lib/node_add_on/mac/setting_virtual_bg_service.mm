#include "setting_virtual_bg_wrap_core.h"
#include "meeting_service_wrap_core.h"
#import "Header_include.h"
#include "sdk_native_error.h"
#import "settingServiceDelegate.h"

ZSettingVirtualBGWrap::ZSettingVirtualBGWrap()
{
    m_pSink = 0;
}

ZSettingVirtualBGWrap::~ZSettingVirtualBGWrap()
{
    [[[[ZoomSDK sharedSDK] getSettingService] getVirtualBGSetting] setDelegate:nil];
    m_pSink = 0;
}

void ZSettingVirtualBGWrap::SetSink(ZNativeSDKVirtualBGSettingContextWrapSink* pSink)
{
    [[[[ZoomSDK sharedSDK] getSettingService] getVirtualBGSetting] setDelegate:[settingServiceDelegate share]];
    m_pSink = pSink;
}
void ZSettingVirtualBGWrap::Init()
{

}

void ZSettingVirtualBGWrap::Uninit()
{

}

bool ZSettingVirtualBGWrap::isValidImageItem(int64_t imageHandle)
{
    bool isValid = false;
    if (zn_bg_image_list.size() == 0) {
        return isValid;
    }

    for (int i = 0; i < zn_bg_image_list.size(); i++)
    {
        if (imageHandle == zn_bg_image_list[i].imageHandle)
        {
            isValid = true;
            break;
        }
    }
    return isValid;
}

bool ZSettingVirtualBGWrap::IsSupportVirtualBG()
{
    ZoomSDKSettingService *service = [[ZoomSDK sharedSDK] getSettingService];
    if (!service) {
        return false;
    }
    ZoomSDKVirtualBackgroundSetting *virtualBackgroundSetting = [service getVirtualBGSetting];
    if (!virtualBackgroundSetting) {
        return false;
    }
    return [virtualBackgroundSetting isSupportVirtualBG];
}

bool ZSettingVirtualBGWrap::IsDeviceSupportSmartVirtualBG()
{
    ZoomSDKSettingService *service = [[ZoomSDK sharedSDK] getSettingService];
    if (!service) {
        return false;
    }
    ZoomSDKVirtualBackgroundSetting *virtualBackgroundSetting = [service getVirtualBGSetting];
    if (!virtualBackgroundSetting) {
        return false;
    }
    return [virtualBackgroundSetting isDeviceSupportSmartVirtualBG];
}

bool ZSettingVirtualBGWrap::IsSupportVirtualBackgroundVideo()
{
    ZoomSDKSettingService *service = [[ZoomSDK sharedSDK] getSettingService];
    if (!service) {
        return false;
    }
    ZoomSDKVirtualBackgroundSetting *virtualBackgroundSetting = [service getVirtualBGSetting];
    if (!virtualBackgroundSetting) {
        return false;
    }
    return [virtualBackgroundSetting isSupportVirtualBackgroundVideo];
}

bool ZSettingVirtualBGWrap::IsDeviceSupportSmartVirtualBackgroundVideo()
{
    ZoomSDKSettingService *service = [[ZoomSDK sharedSDK] getSettingService];
    if (!service) {
        return false;
    }
    ZoomSDKVirtualBackgroundSetting *virtualBackgroundSetting = [service getVirtualBGSetting];
    if (!virtualBackgroundSetting) {
        return false;
    }
    return [virtualBackgroundSetting isDeviceSupportSmartVirtualBackgroundVideo];
}

bool ZSettingVirtualBGWrap::IsDeviceSupportGreenVirtualBackgroundVideo()
{
    ZoomSDKSettingService *service = [[ZoomSDK sharedSDK] getSettingService];
    if (!service) {
        return false;
    }
    ZoomSDKVirtualBackgroundSetting *virtualBackgroundSetting = [service getVirtualBGSetting];
    if (!virtualBackgroundSetting) {
        return false;
    }
    return [virtualBackgroundSetting isDeviceSupportGreenVirtualBackgroundVideo];
}

bool ZSettingVirtualBGWrap::IsUsingGreenScreenOn()
{
    ZoomSDKSettingService *service = [[ZoomSDK sharedSDK] getSettingService];
    if (!service) {
        return false;
    }
    ZoomSDKVirtualBackgroundSetting *virtualBackgroundSetting = [service getVirtualBGSetting];
    if (!virtualBackgroundSetting) {
        return false;
    }
    return [virtualBackgroundSetting isUsingGreenScreenOn];
}

ZNSDKError ZSettingVirtualBGWrap::SetUsingGreenScreen(bool bUse)
{
    ZoomSDKSettingService *service = [[ZoomSDK sharedSDK] getSettingService];
    if (!service) {
        return ZNSDKERR_SERVICE_FAILED;
    }
    ZoomSDKVirtualBackgroundSetting *virtualBackgroundSetting = [service getVirtualBGSetting];
    if (!virtualBackgroundSetting) {
        return ZNSDKERR_SERVICE_FAILED;
    }
    
    ZoomSDKError ret = [virtualBackgroundSetting setUsingGreenScreen:bUse];
    nativeErrorTypeHelp help;
    return help.ZoomSDKErrorType(ret);
}

bool ZSettingVirtualBGWrap::IsAllowToAddNewVBItem()
{
    ZoomSDKSettingService *service = [[ZoomSDK sharedSDK] getSettingService];
    if (!service) {
        return false;
    }
    ZoomSDKVirtualBackgroundSetting *virtualBackgroundSetting = [service getVirtualBGSetting];
    if (!virtualBackgroundSetting) {
        return false;
    }

    return [virtualBackgroundSetting isAllowAddNewVBItem];
}

bool ZSettingVirtualBGWrap::IsAllowToRemoveVBItem()
{
    ZoomSDKSettingService *service = [[ZoomSDK sharedSDK] getSettingService];
    if (!service) {
        return false;
    }
    ZoomSDKVirtualBackgroundSetting *virtualBackgroundSetting = [service getVirtualBGSetting];
    if (!virtualBackgroundSetting) {
        return false;
    }

    return [virtualBackgroundSetting isAllowRemoveVBItem];
}

ZNSDKError ZSettingVirtualBGWrap::AddBGImage(ZoomSTRING zn_filePath)
{
    ZoomSDKSettingService *service = [[ZoomSDK sharedSDK] getSettingService];
    if (!service) {
        return ZNSDKERR_SERVICE_FAILED;
    }
    ZoomSDKVirtualBackgroundSetting *virtualBackgroundSetting = [service getVirtualBGSetting];
    if (!virtualBackgroundSetting) {
        return ZNSDKERR_SERVICE_FAILED;
    }
    
    NSString *filePath = [NSString stringWithUTF8String:zn_filePath.c_str()];
    ZoomSDKError ret = [virtualBackgroundSetting addBGImage:filePath];
    nativeErrorTypeHelp help;
    return help.ZoomSDKErrorType(ret);
}

ZNSDKError ZSettingVirtualBGWrap::RemoveBGImage(int64_t imageHandle)
{
    if (!isValidImageItem(imageHandle)) {
        return ZNSDKERR_INVALID_PARAMETER;
    }
    
    ZoomSDKVirtualBGImageInfo* virtualBGImageInfo = (ZoomSDKVirtualBGImageInfo*)imageHandle;
    ZoomSDKSettingService *service = [[ZoomSDK sharedSDK] getSettingService];
    if (!service) {
        return ZNSDKERR_SERVICE_FAILED;
    }
    ZoomSDKVirtualBackgroundSetting *virtualBackgroundSetting = [service getVirtualBGSetting];
    if (!virtualBackgroundSetting) {
        return ZNSDKERR_SERVICE_FAILED;
    }
    
    ZoomSDKError ret = [virtualBackgroundSetting removeBGItem:virtualBGImageInfo];
    nativeErrorTypeHelp help;
    return help.ZoomSDKErrorType(ret);
}

ZNList<ZNVirtualBGImageInfo> ZSettingVirtualBGWrap::GetBGImageList()
{
    zn_bg_image_list.clear();
    ZoomSDKSettingService *service = [[ZoomSDK sharedSDK] getSettingService];
    if (!service) {
        return zn_bg_image_list;
    }
    ZoomSDKVirtualBackgroundSetting *virtualBackgroundSetting = [service getVirtualBGSetting];
    if (!virtualBackgroundSetting) {
        return zn_bg_image_list;
    }

    NSArray <ZoomSDKVirtualBGImageInfo *> *sdk_list = [virtualBackgroundSetting getBGItemList];
    nativeErrorTypeHelp help;
    for (ZoomSDKVirtualBGImageInfo *imageInfoItem in sdk_list) {
        if (imageInfoItem) {
            ZNVirtualBGImageInfo zn_virtualBGImageInfo = help.ConvertSDKVirtualBGImageInfoToZNVirtualBGImageInfo(imageInfoItem);
            zn_bg_image_list.push_back(zn_virtualBGImageInfo);
        }
    }
    return zn_bg_image_list;
}

ZNSDKError ZSettingVirtualBGWrap::UseBGImage(int64_t imageHandle)
{
    if (!isValidImageItem(imageHandle)) {
        return ZNSDKERR_INVALID_PARAMETER;
    }
    
    ZoomSDKVirtualBGImageInfo *item = (ZoomSDKVirtualBGImageInfo*)imageHandle;
    ZoomSDKSettingService *service = [[ZoomSDK sharedSDK] getSettingService];
    if (!service) {
        return ZNSDKERR_SERVICE_FAILED;
    }
    ZoomSDKVirtualBackgroundSetting *virtualBackgroundSetting = [service getVirtualBGSetting];
    if (!virtualBackgroundSetting) {
        return ZNSDKERR_SERVICE_FAILED;
    }
    
    ZoomSDKError ret = [virtualBackgroundSetting useBGItem:item];
    nativeErrorTypeHelp help;
    return help.ZoomSDKErrorType(ret);
}

ZNZoomSDKColor ZSettingVirtualBGWrap::GetBGReplaceColor()
{
    ZNZoomSDKColor zn_color;
    ZoomSDKSettingService *service = [[ZoomSDK sharedSDK] getSettingService];
    if (!service) {
        return zn_color;
    }
    ZoomSDKVirtualBackgroundSetting *virtualBackgroundSetting = [service getVirtualBGSetting];
    if (!virtualBackgroundSetting) {
        return zn_color;
    }

    NSColor* color = [virtualBackgroundSetting getVBReplaceColor];
    if (color) {
        CGFloat red = 0, green = 0, blue = 0, alpha = 0;
        [color getRed:&red green:&green blue:&blue alpha:&alpha];
        zn_color.red = (int)(red * 255);
        zn_color.green = (int)(green * 255);
        zn_color.blue = (int)(blue * 255);
    }
    return zn_color;
}

ZNSDKError ZSettingVirtualBGWrap::BeginSelectReplaceVBColor()
{
    ZoomSDKSettingService *service = [[ZoomSDK sharedSDK] getSettingService];
    if (!service) {
        return ZNSDKERR_SERVICE_FAILED;
    }
    ZoomSDKVirtualBackgroundSetting *virtualBackgroundSetting = [service getVirtualBGSetting];
    if (!virtualBackgroundSetting) {
        return ZNSDKERR_SERVICE_FAILED;
    }
    
    ZoomSDKError ret = [virtualBackgroundSetting startSelectReplaceVBColor];
    nativeErrorTypeHelp help;
    return help.ZoomSDKErrorType(ret);
}

ZNSDKError ZSettingVirtualBGWrap::AddBGVideo(ZoomSTRING zn_filePath)
{
    ZoomSDKSettingService *service = [[ZoomSDK sharedSDK] getSettingService];
    if (!service) {
        return ZNSDKERR_SERVICE_FAILED;
    }
    ZoomSDKVirtualBackgroundSetting *virtualBackgroundSetting = [service getVirtualBGSetting];
    if (!virtualBackgroundSetting) {
        return ZNSDKERR_SERVICE_FAILED;
    }
    
    NSString *filePath = [NSString stringWithUTF8String:zn_filePath.c_str()];
    ZoomSDKError ret = [virtualBackgroundSetting addBGVideo:filePath];
    nativeErrorTypeHelp help;
    return help.ZoomSDKErrorType(ret);
}

ZNSDKError ZSettingVirtualBGWrap::RemoveBGVideo(int64_t imageHandle)
{
    if (!isValidImageItem(imageHandle)) {
        return ZNSDKERR_INVALID_PARAMETER;
    }
    
    ZoomSDKVirtualBGImageInfo* virtualBGImageInfo = (ZoomSDKVirtualBGImageInfo*)imageHandle;
    ZoomSDKSettingService *service = [[ZoomSDK sharedSDK] getSettingService];
    if (!service) {
        return ZNSDKERR_SERVICE_FAILED;
    }
    ZoomSDKVirtualBackgroundSetting *virtualBackgroundSetting = [service getVirtualBGSetting];
    if (!virtualBackgroundSetting) {
        return ZNSDKERR_SERVICE_FAILED;
    }
    
    ZoomSDKError ret = [virtualBackgroundSetting removeBGItem:virtualBGImageInfo];
    nativeErrorTypeHelp help;
    return help.ZoomSDKErrorType(ret);
}

ZNList<ZNVirtualBGImageInfo> ZSettingVirtualBGWrap::GetBGVideoList()
{
    zn_bg_image_list.clear();
    ZoomSDKSettingService *service = [[ZoomSDK sharedSDK] getSettingService];
    if (!service) {
        return zn_bg_image_list;
    }
    ZoomSDKVirtualBackgroundSetting *virtualBackgroundSetting = [service getVirtualBGSetting];
    if (!virtualBackgroundSetting) {
        return zn_bg_image_list;
    }

    NSArray <ZoomSDKVirtualBGImageInfo *> *sdk_list = [virtualBackgroundSetting getBGItemList];
    nativeErrorTypeHelp help;
    for (ZoomSDKVirtualBGImageInfo *imageInfoItem in sdk_list) {
        if (imageInfoItem) {
            ZNVirtualBGImageInfo zn_virtualBGImageInfo = help.ConvertSDKVirtualBGImageInfoToZNVirtualBGImageInfo(imageInfoItem);
            zn_bg_image_list.push_back(zn_virtualBGImageInfo);
        }
    }
    return zn_bg_image_list;
}

ZNSDKError ZSettingVirtualBGWrap::UseBGVideo(int64_t imageHandle)
{
    if (!isValidImageItem(imageHandle)) {
        return ZNSDKERR_INVALID_PARAMETER;
    }
    
    ZoomSDKVirtualBGImageInfo *item = (ZoomSDKVirtualBGImageInfo*)imageHandle;
    ZoomSDKSettingService *service = [[ZoomSDK sharedSDK] getSettingService];
    if (!service) {
        return ZNSDKERR_SERVICE_FAILED;
    }
    ZoomSDKVirtualBackgroundSetting *virtualBackgroundSetting = [service getVirtualBGSetting];
    if (!virtualBackgroundSetting) {
        return ZNSDKERR_SERVICE_FAILED;
    }
    
    ZoomSDKError ret = [virtualBackgroundSetting useBGItem:item];
    nativeErrorTypeHelp help;
    return help.ZoomSDKErrorType(ret);
}

//callback
void ZSettingVirtualBGWrap::onVBImageDidDownloaded()
{
    if (m_pSink) {
        m_pSink->onVBImageDidDownloaded();
    }
}
void ZSettingVirtualBGWrap::onGreenVBDidUpdateWithReplaceColor(ZNZoomSDKColor zn_selectedColor)
{
    if (m_pSink) {
        m_pSink->onGreenVBDidUpdateWithReplaceColor(zn_selectedColor);
    }
}
void ZSettingVirtualBGWrap::onSelectedVBImageChanged()
{
    if (m_pSink) {
        m_pSink->onSelectedVBImageChanged();
    }
}
void ZSettingVirtualBGWrap::onVBVideoUploadedResult(bool success, ZNVBVideoError zn_error)
{
    if (m_pSink) {
        m_pSink->onVBVideoUploadedResult(success, zn_error);
    }
}